#include <MirfHardwareSpiDriver.h>
#include <MirfSpiDriver.h>
#include <Mirf.h>
#include <nRF24L01.h>

#include <SPI.h>

// sends a string via the nRF24L01
void transmit(const char *string)
{
  byte c; 
  
  for( int i=0 ; string[i]!=0x00 ; i++ )
  { 
    c = string[i];
    Mirf.send(&c);
    while( Mirf.isSending() ) ;
  }
}

void setup()
{
  Serial.begin(57600);

  // init the transceiver
  Mirf.csnPin=4;
  Mirf.cePin=5;
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  
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

void loop() 
{
  Serial.println("AZAZA");  
  delay(400);
  // transmit a separator
  transmit("A");
 
  // ... just take your time
  delay(400);

  if( Mirf.dataReady() )
    {
       byte c;
       // well, get it
       Mirf.getData(&c);

       int incoming = c;
       if (incoming == 'f') {
        Serial.print("Froward");
      }
       if (incoming == 'b') {
        Serial.print("Back");
      }
       if (incoming == 'l') {
        Serial.print("Left");
      }
       if (incoming == 'r') {
         Serial.print("Right");
       }
       if (incoming == 's') {
         Serial.print("Scanning");
         transmit("[50,50,50,50,50,50,50,50,50,50,50,50]");
       }

      // ... and transmit it

    }
}

