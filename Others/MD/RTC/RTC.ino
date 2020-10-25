#include <Wire.h>
#include <RTClib.h>
/*
   RTC    ESP32
   VCC    Vin
   GND    GND
   SCL    22
   SDA    21
*/
RTC_DS1307 rtc;
void setup() {
  Serial.begin(115200);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  } // end rtc.begin()
  // following line sets the RTC to the date & time this sketch was compiled
   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0)); // year, month, day, hour, minute, seconds
  pinMode(2, OUTPUT);
}

void loop() {

  DateTime now = rtc.now();

  //buffer can be defined using following combinations:
  //hh - the hour with a leading zero (00 to 23)
  //mm - the minute with a leading zero (00 to 59)
  //ss - the whole second with a leading zero where applicable (00 to 59)
  //YYYY - the year as four digit number
  //YY - the year as two digit number (00-99)
  //MM - the month as number with a leading zero (01-12)
  //MMM - the abbreviated English month name ('Jan' to 'Dec')
  //DD - the day as number with a leading zero (01 to 31)
  //DDD - the abbreviated English day name ('Mon' to 'Sun')

  char buf1[] = "hh:mm";
  String s = now.toString(buf1);
  Serial.println(s);
  if (s == "18:27") {
    digitalWrite(2, HIGH);
  }
  else {
    digitalWrite(2, LOW);
  }

  char buf2[] = "YYMMDD-hh:mm:ss";
  Serial.println(now.toString(buf2));

  char buf3[] = "Today is DDD, MMM DD YYYY";
  Serial.println(now.toString(buf3));

  char buf4[] = "MM-DD-YYYY";
  //String s = now.toString(buf4);
  Serial.println(s);

  delay(1000);
}
