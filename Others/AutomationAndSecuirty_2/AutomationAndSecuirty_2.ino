/*
   This program can read time through RTC DS1307, rfid card and pir motion sensor and send its status
   to Arduino Controller (for local control)and Mqqt Dash (for remote Control).
   User can control brightness of 3 leds (here RGB),
   inbuilt led and angle of servo remotely and locally from respective application.
   Following Automation are also added
   > Inbuilt led turn on motion detection.
   > Green Led is on if valid card is detected else red.
   > Servo is at 180 degree for one minute at a set time (varible servoTime) else at 0 degree.

   RFID Connection
   RFID    ESP32
   SDA     5 (Any digital pin and change code accordingly)
   SCK     18
   MOSI    23
   MISO    19
   IRQ     Not connected
   GND     GND
   RST     17/TX2 (Any digital pin and change code accordingly)
   3.3V    3.3V

   RTC Connection
   RTC    ESP32
   VCC    Vin
   GND    GND
   SCL    22
   SDA    21

   RGB Connection
   RGB Led    Esp32
   RED        32 (Any digital pin and change code accordingly)
   Green      33 (Any digital pin and change code accordingly)
   Blue       25 (Any digital pin and change code accordingly)
   Anode/VCC  3.3V

   Inbuilt Led on Pin 2

   PIR Connection
   PIR    ESP32
   VCC    Vin
   GND    GND
   DOUT   12 (Any digital pin and change code accordingly)

   Servo Connection
   Serovo   ESP32
   BROWN    GND
   RED      Vin
   Yellow   13 (Any digital pin and change code accordingly)
*/
// All Includes
#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <RTClib.h>

// Your wifi SSID and Password
const char* ssid = "M 57"; //update
const char* password = "8376918157"; //update

// Gateway and IP must has same masking for first 3 places.
IPAddress ip(192, 168, 0, 200);           //Set IP  //update
IPAddress gateway(192, 168, 0, 1);        //Set Gateway // update
IPAddress subnet(255, 255, 255, 0);       //Set Subnet

// MQTT Broker Credentials flespi.io
const char mqttServer[] = "mqtt.flespi.io";
const char mqttUserName[] = "wYSb7M5ap343vrobJBOTD8F7oPHGzAeToWUjyOhNHbE0sQ10iDJLCcGQSdmhBq0N"; //
const char mqttPass[] = "";
const char clientID[] = "9899845428"; // Must be unique in the world.

//Rfid card id
String rfidCardId = "147154142460"; //replace with your card id
// Topics to subscibe
const char ledR[] = "LEDR";
const char ledB[] = "LEDB";
const char ledG[] = "LEDG";
const char led2[] = "LED2";
const char servo[] = "Servo";

// Topic to publish to and Id to link read button in app.
String id1 = "rfid";
String id2 = "motionSensor";

// Message to publish and write on app
String rfidTag = "False";
String pirFlag = "False";

// Pin define
#define RST_PIN  17
#define SS_PIN   5
int servoPin = 13;
int pirPin = 12;
const int R = 0, G = 1, B = 2; //PWM chanles
int redPin = 25, greenPin = 33, bluePin = 32;

// All instances
MFRC522 mfrc522(SS_PIN, RST_PIN);
WiFiServer server(80);
WiFiClient client;
PubSubClient mqttClient(client);
Servo myservo;
RTC_DS1307 rtc;

bool rtcFlag = true;
String currentDate, currentTime;// Varible holding time and date in format of buffer mentioned in rtcLoop().
bool change = true; // Varible is true when any change in pir or rfid flags.

void mqttSub() {
  mqttClient.subscribe(ledR);
  mqttClient.subscribe(ledG);
  mqttClient.subscribe(ledB);
  mqttClient.subscribe(led2);
  mqttClient.subscribe(servo);
}
void commandHandleMqtt(String t, String m) {
  Serial.print("Time is ");
  Serial.print(currentTime);
  Serial.print(" and Date is ");
  Serial.println(currentDate);
  Serial.print("Message arrived from Mqtt Dash app on topic [");
  Serial.print(t);
  Serial.print("] is ");
  int v = m.toInt();
  Serial.println(v);
  if (t == String(ledR)) {
    Serial.print("Red Brightness set to ");
    Serial.println(v);
    ledcWrite(R, 255 - v);
  }
  else if (t == String(ledG)) {
    Serial.print("Green Brightness set to ");
    Serial.println(v);
    ledcWrite(G, 255 - v);
  }
  else if (t == String(ledB)) {
    Serial.print("Blue Brightness set to ");
    Serial.println(v);
    ledcWrite(B, 255 - v);
  }
  else if (t == String(servo)) {
    Serial.print("Servo angle set to ");
    Serial.println(v);
    myservo.write(v);
  }
  else if (t == String(led2)) {
    Serial.print("Pin is 2");
    Serial.print("command is ");
    Serial.println(v);
    digitalWrite(2, v);
  }
  else {
    Serial.println("Command not recognized");
  }
}

void wifiServer() {
  // Check for any command from Arduino Controller app
  String s = appHandler();
  if (s != "") {
    // Manipulate the string according to your needs
    Serial.print("Time is ");
    Serial.print(currentTime);
    Serial.print(" and Date is ");
    Serial.println(currentDate);
    Serial.print("Message arrived from Ardino Controller app is ");
    Serial.println(s);

    String pin, command;
    if (s.indexOf("IN") >= 0) {
      pin = s.substring(3, 4);
      command = s.substring(4);
      int p = pin.toInt();
      Serial.print("Pin is ");
      Serial.print(p);
      Serial.print(" command is ");
      Serial.println(command);
      if (command.indexOf("ON") != -1) {
        digitalWrite(p, HIGH);
      }
      else {
        digitalWrite(p, LOW);
      }
    }
    else if (s.indexOf("red") >= 0) {
      int i = s.substring(3).toInt();
      Serial.print("Red Brightness set to ");
      Serial.println(i);
      ledcWrite(R, 255 - i);
    }
    else if (s.indexOf("blue") >= 0) {
      int i = s.substring(4).toInt();
      Serial.print("Blue Brightness set to ");
      Serial.println(i);
      ledcWrite(B, 255 - i);
    }
    else if (s.indexOf("green") >= 0) {
      int i = s.substring(5).toInt();
      Serial.print("Green Brightness set to ");
      Serial.println(i);
      ledcWrite(G, 255 - i);
    }
    else if (s.indexOf("servo") >= 0) {
      int i = s.substring(5).toInt();
      Serial.print("Servo angle set to ");
      Serial.println(i);
      myservo.write(i);
    }
    else if (s.indexOf("rgboff") >= 0) {
      Serial.println("RGB OFF");
      ledcWrite(R, 255);
      ledcWrite(G, 255);
      ledcWrite(B, 255);
    }
    else Serial.println("Command not recognised");
  }
}
String servoTime = "18:00";
void automations() {
  if (rfidTag == "True") ledcWrite(G, 0);
  else ledcWrite(G, 255);
  if (pirFlag == "True") digitalWrite(2, HIGH);
  else digitalWrite(2, LOW);
  if (currentTime == servoTime) myservo.write(180);
  else myservo.write(0);
}
void setup()
{
  // Serial begin
  Serial.begin(115200);
  Serial.println("Setup Started");
  //Wifi Setup
  wifiSetup();
  //Server begin
  server.begin();
  // rfid setup and initiate
  rfidSetup();
  // Setup Servo
  servoSetup();
  //PWM Setup
  pwmSetup(); //For RGB led brightness control
  //RTC Setup
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    rtcFlag = false;
    delay(5000);
  }
  //mqtt setup
  mqttClient.setServer(mqttServer, 1883);
  mqttClient.setCallback(callback_fun);
  //pir setup
  pinMode(pirPin, INPUT);
  //Inbuilt Led Setup
  pinMode(2, OUTPUT);
  Serial.println("Setup done");
}
void loop()
{
  wifiServer(); //Handles Wifi local server
  mqttLoop(); // Handles Mqtt Connection to Broker
  rfidLoop(); //rfid loop
  pirLoop(); //pir loop
  rtcLoop(); // RTC loop
  // automations();
  {
    if (change && mqttClient.connected()) {
      change = false;
      Serial.print("Time is ");
      Serial.print(currentTime);
      Serial.print(" and Date is ");
      Serial.println(currentDate);
      mqttPublish(id1, String(rfidTag)); //topic and message
      mqttPublish(id2, String(pirFlag)); //topic and message}
    }
  }
}

void appData(WiFiClient cl) {
  //To write to Arduino Controller application
  cl.println("<br/><a id=" + id1 + ">" + String(rfidTag) + "</a>");
  cl.println("<br/><a id=" + id2 + ">" + String(pirFlag) + "</a>");
}
void rtcLoop() {
  if (rtcFlag) {
    DateTime now = rtc.now();
    char time1[] = "hh:mm";
    char date1[] = "DD-MM-YYYY";
    currentDate = now.toString(date1);
    currentTime = now.toString(time1);
  }
}
long timePir = millis();
const int pirDelay = 5000;
String pirFlagP;
void pirLoop() {
  if ( pirFlag == "False" || (millis() - timePir) > pirDelay) {
    if (digitalRead(pirPin)) {
      Serial.println("Motion Detected");
      pirFlag = "True";
      timePir = millis();
    }
    else {
      pirFlag = "False";
    }
    if (pirFlag != pirFlagP) {
      pirFlagP = pirFlag;
      change = true;
    }
  }
}
String sP = "";
void rfidLoop() {
  String s = rfid(); //check for card if available return unique key else none.
  if (s != sP) {
    change = true;
    sP = s;
    Serial.print("Card detected with id ");
    Serial.println(s);
    if (s == rfidCardId ) {
      rfidTag = "True";
    }
    else {
      rfidTag = "False";
    }
  }
}
// Servo function
void servoSetup() {
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);// Standard 50hz servo
  myservo.attach(servoPin);
  myservo.write(0); // Set initial angle to zero.
}
// PWM function
void pwmSetup() {
  // 3 PWM channels with frequency 5000 and 8 bit
  //R=0, G=1, B=2
  ledcSetup(R, 5000, 8);
  ledcSetup(G, 5000, 8);
  ledcSetup(B, 5000, 8);
  ledcAttachPin(redPin, R);
  ledcAttachPin(greenPin, G);
  ledcAttachPin(bluePin, B);
  //Set initial brightness to zero
  ledcWrite(R, 255);
  ledcWrite(G, 255);
  ledcWrite(B, 255);
}

// No changes below this
// Wifi Function
void wifiSetup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  //WiFi.config(ip, gateway, subnet); //update
  Serial.print("Connecting");
  int tries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    tries = tries + 1;
    if (tries > 20) {
      Serial.println("Failed to connect.");
      break;
    }
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected");
    Serial.println("IP Address : ");
    Serial.println(WiFi.localIP());
  }
}
//Wifi Server Function
String appHandler() {
  String s;
  WiFiClient client = server.available();  // try to get client
  if (client) {  // got client?
    boolean currentLineIsBlank = true;
    String request;
    while (client.connected()) {
      if (client.available()) {   // client data available to read
        char c = client.read(); // read 1 byte (character) from client
        request += c;  // save the HTTP request 1 char at a time
        // last line of client request is blank and ends with \n
        // respond to client only after last line received
        if (c == '\n' && currentLineIsBlank) {
          // Send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          // Send web page
          client.println("<!DOCTYPE html>");
          client.println("<html>");
          client.println("<head>");
          client.println("</head>");
          client.println("<body>");
          appData(client);
          client.println("</body>");
          client.println("</html>");
          s = request.substring(request.indexOf("GET") + 6, request.indexOf("HTTP/"));
          break;
        }
        // every line of text received from the client ends with \r\n
        if (c == '\n') {
          // last character on line of received text
          // starting new line with next character read
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // a text character was received from client
          currentLineIsBlank = false;
        }
      }
    }
    delay(10);      // give the web browser time to receive the data
    client.stop(); // close the connection
  }
  return s;
}
// Mqtt Function
long mqttRetry = 5000;
void mqttLoop() {
  if (!mqttClient.connected() && ((millis() - mqttRetry) > 5000))
  {
    Serial.print("Attempting MQTT connection...");
    if (mqttClient.connect(clientID, mqttUserName, mqttPass)) {
      mqttSub();
      Serial.println("connected and subscribed");
    }
    else {
      Serial.print("failed to connect to mqttServer, ");
      Serial.println(" will try again in 5 seconds");
      mqttRetry = millis();
    }
  }
  mqttClient.loop();
}

void mqttPublish(String t, String m) {
  if (mqttClient.publish( t.c_str(), m.c_str())) Serial.println("Message published");
}

void callback_fun(char* topic, byte* payload, unsigned int length) {
  String t = String(topic);
  String m = "";
  for (int i = 0; i < length; i++) {
    m = m + (char)payload[i];
  }
  commandHandleMqtt(t, m);
}

// rfid function
void rfidSetup() {
  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);
}
String rfid() {
  String s[2] = {"", ""};
  int loopRfid = 2;
  while (loopRfid > 0) {
    if ( mfrc522.PICC_IsNewCardPresent()) {
      if ( mfrc522.PICC_ReadCardSerial()) {
        for (int i = 0; i < 5; i++) {
          s[loopRfid - 1] = s[loopRfid - 1] + mfrc522.uid.uidByte[i];
        }
      }
    }
    else s[loopRfid - 1] = "None";
    loopRfid--;
  }
  if (s[0] != s[1]) {
    if (s[0] == "None") {
      return s[1];
    }
    else return s[0];
  }
  else return s[0];
}
