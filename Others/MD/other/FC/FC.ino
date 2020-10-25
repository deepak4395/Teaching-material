// (Inline Comment)Arduino Integrated Development Enviornment

/*
   Group Comment
   Lets take number 52
   C, C++
   n=6
   7,8,9,10,11,12
*/
const int n = 6;
int checkNumber = 7 ;
int checkNumber1[n] = {789, 833, 577, 920, 29, 1111111};
bool result = true ; // false
bool result1[n] = {true, true, true, true, true, true} ;
int r = -1;
int Q;
int i = 2;
void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600); //baud rate bits per seconds.
  Q = checkNumber / 2; // +,-, *,
  for (int x = 0; x < n; x = x + 1) {
    Serial.println(x);
    i = 2;
    r = -1;
    while (i < checkNumber1[x]) {
      r = checkNumber1[x] % i;
      if (r == 0) {
        // Number is not prime
        result1[x] = false;
        break;
      }
      else {
        i = i + 1;
      }
    }

  }
  for (int i = 0; i < n; i = i + 1) {
    Serial.print("For number ");
    Serial.print(checkNumber1[i]);
    Serial.print(" result is ");
    Serial.println(result1[i]);
  }

  //  Serial.println();
  //  Serial.print("Q= ");
  //  Serial.println(Q);
  //  Serial.print("R= ");
  //  Serial.println(r);
}

void loop() {
  // put your main code here, to run repeatedly:

}
