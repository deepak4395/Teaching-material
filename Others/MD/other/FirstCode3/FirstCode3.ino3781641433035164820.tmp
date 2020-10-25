const int n = 4;
int checkNumber = 7 ;
int checkNumber2[n] = {7, 9, 10, 11};
bool result = true; // true
bool result2[n] = {true, true, true, true}; // true
int remainder2 = -1;
int remainder3[n] = { -1, -1, -1, -1};
int i = 2;
void setup() {
  // put your setup code here, to run once:
  // Serial Coomunication, UART
  Serial.begin(9600);
  // We divide the number from 2 to n/2
  // if and else statemet.
  for (int x = 0; x < n; x = x + 1 ) {
    Serial.println(x);
    i = 2;
    remainder2 = -1;
    while (i < checkNumber2[x]) {
      remainder2 = checkNumber2[x] % i;
      if (remainder2 == 0) {
        result2[x] = false;
        break;
      }
      else {
        i = i + 1;
      }
    }
  }
  for (int i = 0; i < n; i = i + 1) {
    Serial.print("Number ");
    Serial.print(checkNumber2[i]);
    Serial.print(" is ");
    Serial.print(result2[i]);
    Serial.println(" for prime.");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
