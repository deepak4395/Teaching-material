/*
 * Code reads the tag and prints its unique ID.
 * Make Sure all files are available in the tab bar. RFID.cpp, RFID.h
 * RFID    ESP32
 * SDA     5
 * SCK     18
 * MOSI    23
 * MISO    19
 * IRQ     Not connected
 * GND     GND
 * RST     Any digital pin and change code accordingly
 * 3.3V    3.3V
 */
#include "Arduino.h"
#include "RFID.h"

#define RFID_PIN_RST	4
#define RFID_PIN_SDA	5

String sP = "";
RFID rfid(RFID_PIN_SDA, RFID_PIN_RST);

void setup()
{
  Serial.begin(9600);
  while (!Serial) ; // wait for serial port to connect. Needed for native USB
  Serial.println("start");
  rfid.init();
}

void loop()
{
  String rfidtag = rfid.readTag();
  //Serial.println(rfidtag);
  cardPresence(rfidtag);
}

void cardPresence(String s) {
  if (s != sP) {
    sP = s;
    if (s.indexOf("None") == -1) {
      Serial.print("Card detected with id ");
      Serial.println(sP);
    }
    else {
      Serial.println("No card present");
    }
  }
}
