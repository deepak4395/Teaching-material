/*
   Group Comment
   Find Prime no.
   We are going to check is number prime or not.
*/
// This is a line comment

int checkNumber = 99 ;
bool result = false; // true
int remainder2 = -1;
int Divident = -1;
void setup() {
  // put your setup code here, to run once:
  // Serial Coomunication, UART
  Serial.begin(9600);
  // We divide the number from 2 to n/2
  // if and else statemet.
  Divident = 99 / 2; // *, +, -,
  remainder2 = 99 % 2;
  Serial.print("Dividnet = ");
  Serial.println(Divident);
  Serial.print("Remainder = ");
  Serial.println(remainder2);
  if (remainder == 0) {
    result = false;
  }
  else {

  }
}

void loop() {
  // put your main code here, to run repeatedly:

}