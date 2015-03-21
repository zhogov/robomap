package com.example

import akka.actor.Actor
import spray.routing._
import spray.http._
import MediaTypes._
import spray.json.DefaultJsonProtocol._
import spray.json.DefaultJsonProtocol

import scala.collection.mutable

// we don't implement our route structure directly in the service actor because
// we want to be able to test it independently, without having to spin up an actor
class MyServiceActor extends Actor with MyService {

  // the HttpService trait defines only one abstract member, which
  // connects the services environment to the enclosing actor or test
  def actorRefFactory = context

  // this actor only runs our route, but you could add
  // other things here, like request stream processing
  // or timeout handling
  def receive = runRoute(myRoute)
}

case class Point(x: Int, y: Int)


// this trait defines our service behavior independently from the service actor
trait MyService extends HttpService {

  val myRoute = {
    path("") {
      getFromResource("web/index.html")
    } ~ {
      getFromResourceDirectory("web")
    } ~ path("azaza") {
      respondWithMediaType(`application/json`) {
        complete {
          val list = mutable.MutableList[Point]()
          list+=new Point(scala.util.Random.nextInt(700), scala.util.Random.nextInt(700))
          list+=new Point(scala.util.Random.nextInt(700), scala.util.Random.nextInt(700))
          list+=new Point(scala.util.Random.nextInt(700), scala.util.Random.nextInt(700))
          list+=new Point(scala.util.Random.nextInt(700), scala.util.Random.nextInt(700))
          list+=new Point(scala.util.Random.nextInt(700), scala.util.Random.nextInt(700))
          list+=new Point(scala.util.Random.nextInt(700), scala.util.Random.nextInt(700))
          var s = "["
          list.foreach {point => s=s+jsonFormat2(Point).write(point).toString()+","}
          if (s.endsWith(",")) s = s.substring(0, s.length-1)
          s=s+"]"
          s
        }
      }
    }
  }


}