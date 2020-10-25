// Always give time for PIR to warm up. A time of atleaset 50 seconds is sufficient.
// Led is on for fixed pirDelay if there is a motion.
int pirPin = 13;
long timePir = millis();
bool pirFlag = false;
const int pirDelay = 5000;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(pirPin, INPUT);
}
void loop() {
  MotionSensor();
  digitalWrite(2, pirFlag);
}

void MotionSensor() {
  if ( !pirFlag || (millis() - timePir) > pirDelay) {
    if (digitalRead(pirPin)) {
      Serial.println("Motion Detected");
      pirFlag = true;
      timePir = millis();
    }
    else {
      pirFlag = false;
    }
  }
}
