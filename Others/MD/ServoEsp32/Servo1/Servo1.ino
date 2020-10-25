#include <ESP32Servo.h>
/*
   Serovo   ESP32
   BROWN    GND
   RED      Vin
   Yellow   13 (Any Digital Pin)
*/

Servo myservo;  // create servo object to control a servo
int servoPin = 13;      // GPIO pin used to connect the servo control (digital out)

void setup()
{

  // Setup Servo
  servoSetup();
  myservo.write(0);
}

void loop() {
  for (int i = 0; i < 181; i++) {
    myservo.write(i);                  // set the servo position according to the scaled value
    delay(10);                          // wait for the servo to get there
  }
  for (int i = 180; i >= 0; i--) {
    myservo.write(i);                  // set the servo position according to the scaled value
    delay(10);                          // wait for the servo to get there
  }
}

void servoSetup() {
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);// Standard 50hz servo
  myservo.attach(servoPin);
}
