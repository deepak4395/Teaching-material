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
  ledcSetup(1, 5000, 8);
  ledcSetup(2, 5000, 8);
  ledcAttachPin(19, 0); //green
  ledcAttachPin(18, 1); // blue
  ledcAttachPin(21, 2); //red
  ledcWrite(0, 0); //3.3
  ledcWrite(1, 0);
  ledcWrite(2, 0);
  delay(3000);
  ledcWrite(0, 255); //3.3
  ledcWrite(1, 255);
  ledcWrite(2, 255);
  delay(3000);
}
int i;
void loop() {
  // put your main code here, to run repeatedly:
  //  for (g = 0; g < 256; g++) {
  //    ledcWrite(0, g);
  //    delay(10);
  //  }
  //  delay(1000);
  //  for (r = 0; r < 256; r++) {
  //    ledcWrite(1, r);
  //    delay(10);
  //  }
  //  delay(1000);
  i = 0;
  while (i < 256) {
    ledcWrite(1, i);
    i = i + 1;
    delay(10);
  }
  i = 0;
  while (i < 256) {
    ledcWrite(0, i);
    i = i + 1;
    delay(10);
  }
  i = 0;
  while (i < 256) {
    ledcWrite(2, i);
    i = i + 1;
    delay(10);
  }
  i = 0;
  while (i < 256) {
    ledcWrite(2, i);
    ledcWrite(1, i);
    i = i + 1;
    delay(10);
  }
  i = 0;
  while (i < 256) {
    ledcWrite(0, i);
    ledcWrite(1, i);
    i = i + 1;
    delay(10);
  }
  while (i < 256) {
    ledcWrite(0, i);
    ledcWrite(2, i);
    i = i + 1;
    delay(10);
  }
  while (i < 256) {
    ledcWrite(0, i);
    ledcWrite(1, i);
    ledcWrite(2, i);
    i = i + 1;
    delay(10);
  }

}
