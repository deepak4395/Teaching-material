/*
   Pwm Pulse width Modulation
   Chanel number 0-12
   Pin 2
   Frequency 5000
   Bit 8bit =2^8=256 0-255 0-3.3v
   Bit 10bit= 2^10=1024 0-1023 0-3.3v
*/
void setup() {
  // put your setup code here, to run once:
  ledcSetup(0, 5000, 8);
  ledcSetup(1, 10000, 10);
  ledcAttachPin(2, 0);
  ledcAttachPin(4, 1);
  ledcWrite(0, 255); //3.3
  delay(2000);
  ledcWrite(0, 200);//2
  delay(2000);
  ledcWrite(0, 150);
  delay(2000);
  ledcWrite(0, 100);
  delay(2000);
  ledcWrite(0, 0);
  delay(2000);
}
int i = 0;
void loop() {
  // put your main code here, to run repeatedly:
  while (i < 256) {
    ledcWrite(0, i);
    i = i + 1;
    delay(10);
  }
  while (i > 0) {
    ledcWrite(0, i);
    i = i - 1;
    delay(10);
  }
}
