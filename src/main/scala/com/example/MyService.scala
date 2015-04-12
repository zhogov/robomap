package com.example

import akka.actor._
import akka.actor.Actor.Receive
import akka.util.ByteString
import com.github.jodersky.flow.{Parity, SerialSettings, Serial}
import com.github.jodersky.flow.Serial._
import com.github.jodersky.flow.internal.SerialConnection
import spray.routing._
import spray.http._
import MediaTypes._
import akka.io.IO
import scala.sys.process._

import com.fasterxml.jackson.databind.ObjectMapper
import com.fasterxml.jackson.module.scala.DefaultScalaModule

import scala.collection.mutable

object Storage {
  var array = List[Int]()
}

class ComActor extends Actor with ActorLogging {
  {
    val port = "/dev/ttyUSB0"
    try {
      ("kdesudo chmod 777 " + port).!!
    }
    catch {
      case e:RuntimeException => {} // No code to execute. Ignore error.
    }

    IO(Serial)(Boot.system) ! Serial.Open(port, SerialSettings(115200, 8, false, Parity(0)))
    SerialConnection.debug(true)
  }

  def receive = {
    case CommandFailed(cmd, reason) => {
      log.error(s"Connection failed, stopping terminal. Reason: ${reason}")
      context stop self
    }
    case Opened(port) => {
      log.info(s"Port ${port} is now open.")
      val operator = sender
      context become opened(operator)
      context watch operator
    }
  }

  var buffer = ""

  def processPoints(bufferString : String): Unit = {
    val mapper = new ObjectMapper()
    mapper.registerModule(DefaultScalaModule)
    Storage.array = mapper.readValue(bufferString, classOf[List[Int]])
  }

  def opened(operator: ActorRef): Receive = {
    case Received(data) => {
      data.foreach(x => {
        buffer = buffer + x.toChar
        if (x==0) {
          // got some nulls
          buffer = ""
        }
        if (x.toChar.equals(']')) {
          // process buffer
          processPoints (buffer)
          println (buffer)
          buffer = ""
        }
      })
      //log.info(s"Received data: ${data}")
    }
    case Closed => {
      log.info("Operator closed normally, exiting terminal.")
      context unwatch operator
      context stop self
    }
    case Terminated(`operator`) => {
      log.error("Operator crashed, exiting terminal.")
      context stop self
    }
  }
}

// we don't implement our route structure directly in the service actor because
// we want to be able to test it independently, without having to spin up an actor
class MyServiceActor extends Actor with MyService {
  val comActor = Boot.system.actorOf(Props[ComActor], "comActor")
  // the HttpService trait defines only one abstract member, which
  // connects the services environment to the enclosing actor or test
  def actorRefFactory = context

  // this actor only runs our route, but you could add
  // other things here, like request stream processing
  // or timeout handling
  def receive = runRoute(myRoute)
}

class Point(val x: Int, val y: Int)

// this trait defines our service behavior independently from the service actor
trait MyService extends HttpService {

  val myRoute = {
    path("") {
      getFromResource("web/index.html")
    } ~ {
      getFromResourceDirectory("web")
    } ~ path("points") {
      respondWithMediaType(`application/json`) {
        complete {
          val mapper = new ObjectMapper()
          mapper.registerModule(DefaultScalaModule)
          mapper.writeValueAsString(Storage.array)
        }
      }
    } ~ path("forward") {
      respondWithStatus(StatusCodes.OK) {
        complete {
          ""
        }
      }
    } ~ path("back") {
      respondWithStatus(StatusCodes.OK) {
        complete {
          ""
        }
      }
    } ~ path("left") {
      respondWithStatus(StatusCodes.OK) {
        complete {
          ""
        }
      }
    } ~ path("right") {
      respondWithStatus(StatusCodes.OK) {
        complete {
          ""
        }
      }
    } ~ path("scan") {
      respondWithStatus(StatusCodes.OK) {
        complete {
          ""
        }
      }
    }
  }
}