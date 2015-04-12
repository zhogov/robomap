#include "MirfHardwareSpiDriver.h"
#include "MirfSpiDriver.h"
#include "Mirf.h"
#include "nRF24L01.h"

#include <SPI.h>

// Моторы подключаются к клеммам M1+,M1-,M2+,M2-
// Motor shield использует четыре контакта 6,5,7,4 для управления моторами
#define SPEED_LEFT       6
#define SPEED_RIGHT      5
#define DIR_LEFT         7
#define DIR_RIGHT        4

// Скорость, с которой мы движемся вперёд (0-255)
#define SPEED            254

// Коэффициент, задающий во сколько раз нужно затормозить
// одно из колёс для поворота
#define BRAKE_K          4


void runForward() {
    analogWrite(SPEED_LEFT, SPEED);
    analogWrite(SPEED_RIGHT, SPEED);

    // Если в DIR_LEFT или DIR_RIGHT пишем HIGH, мотор будет двигать соответствующее колесо
    // вперёд, если LOW - назад.
    digitalWrite(DIR_LEFT, HIGH);
    digitalWrite(DIR_RIGHT, HIGH);
    
    delay(1500);    
    analogWrite(SPEED_LEFT, 0);
    analogWrite(SPEED_RIGHT, 0);
}

void runBackward() {
    analogWrite(SPEED_LEFT, SPEED);
    analogWrite(SPEED_RIGHT, SPEED);

    // Если в DIR_LEFT или DIR_RIGHT пишем HIGH, мотор будет двигать соответствующее колесо
    // вперёд, если LOW - назад.
    digitalWrite(DIR_LEFT, LOW);
    digitalWrite(DIR_RIGHT, LOW);
    
    delay(1500);    
    analogWrite(SPEED_LEFT, 0);
    analogWrite(SPEED_RIGHT, 0);
}

void steerRight() {
    // Замедляем правое колесо относительно левого,
    // чтобы начать поворот
    analogWrite(SPEED_RIGHT, SPEED / BRAKE_K);
    analogWrite(SPEED_LEFT, SPEED);

    digitalWrite(DIR_LEFT, HIGH);
    digitalWrite(DIR_RIGHT, HIGH);
    
    delay(500);    
    analogWrite(SPEED_LEFT, 0);
    analogWrite(SPEED_RIGHT, 0);
}

void steerLeft() {
    analogWrite(SPEED_LEFT, SPEED / BRAKE_K);
    analogWrite(SPEED_RIGHT, SPEED);

    digitalWrite(DIR_LEFT, HIGH);
    digitalWrite(DIR_RIGHT, HIGH);
    
    delay(500);    
    analogWrite(SPEED_LEFT, 0);
    analogWrite(SPEED_RIGHT, 0);
}

// sends a string via the nRF24L01
void transmit(const char *string) {
    byte c;
    for( int i=0 ; string[i]!=0x00 ; i++ ) {
        c = string[i];
        Mirf.send(&c);
        while( Mirf.isSending() ) ;
    }
}

void setup() {
    Serial.begin(57600);

    // Настраивает выводы платы 4,5,6,7 на вывод сигналов

    // init the transceiver
    Mirf.csnPin=3;
    Mirf.cePin=2;
    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();

    // name the receiving channel - must match tranmitter setting!
    Mirf.setRADDR((byte *)"TX_02");

    // just a single byte is transmitted
    Mirf.payload = 1;

    // we use channel 90 as it is outside of WLAN bands
    // or channels used by wireless surveillance cameras
    Mirf.channel = 90;

    // now config the device....
    Mirf.config();

    // Set 1MHz data rate - this increases the range slightly
    Mirf.configRegister(RF_SETUP,0x06);

    // Set address - this one must match the
    // address the receiver is using!
    Mirf.setTADDR((byte *)"TX_01");
}

void loop() {
    if( Mirf.dataReady() ) {
        byte c;
        // well, get it
        Mirf.getData(&c);

        Serial.println(c);

        int incoming = c;
        if (incoming == 'f') {
            Serial.print("Forward");
            runForward();
        }
        if (incoming == 'b') {
            Serial.print("Back");
            runBackward();
        }
        if (incoming == 'l') {
            Serial.print("Left");
            steerLeft();
        }
        if (incoming == 'r') {
            Serial.print("Right");
            steerRight();
        }
        if (incoming == 's') {
            Serial.print("Scanning");
            transmit("[50,50,50,50,50,50,50,50,50,50,50,50]");
        }
    }
}


