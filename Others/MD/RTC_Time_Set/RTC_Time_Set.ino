#include <Wire.h>
#include <RTClib.h>
RTC_DS1307 rtc;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  Serial.println("RTC Setup Done");
}

void loop() {
  // put your main code here, to run repeatedly:

}
