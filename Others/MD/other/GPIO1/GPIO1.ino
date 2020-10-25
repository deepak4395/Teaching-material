int i;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  pinMode(4, INPUT_);
}

void loop() {
  i = digitalRead(4);
  //Serial.println(i);
  if (i == 1) digitalWrite(2, HIGH);
  else digitalWrite(2, LOW);
  // put your main code here, to run repeatedly:
}
