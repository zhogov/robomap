#include <MirfHardwareSpiDriver.h>
#include <MirfSpiDriver.h>
#include <Mirf.h>
#include <nRF24L01.h>

#include <SPI.h>

void setup()
{
  Serial.begin(57600);

  // init the transceiver
  Mirf.csnPin=4;
  Mirf.cePin=5;
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  
  // name the receiving channel - must match tranmitter setting!
  Mirf.setRADDR((byte *)"TX_01");
  
  // just a single byte is transmitted
  Mirf.payload = 1;

  // we use channel 90 as it is outside of WLAN bands 
  // or channels used by wireless surveillance cameras 
  Mirf.channel = 90;
  
  // now config the device.... 
  Mirf.config();  
  
  // Set 1MHz data rate - this increases the range slightly
  Mirf.configRegister(RF_SETUP,0x06);
}

void loop()
{

  byte c; 

  // is there any data pending? 
  if( Mirf.dataReady() )
  {
     // well, get it
     Mirf.getData(&c);

    // ... and write it out to the PC
     Serial.print(c);
  }
}

