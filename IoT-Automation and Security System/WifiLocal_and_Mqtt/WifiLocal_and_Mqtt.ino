#include <WiFi.h>
#include <PubSubClient.h>

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
String rfidTag = "True";
String pirFlag = "True";

WiFiServer server(80);
WiFiClient client;
PubSubClient mqttClient(client);
bool change = true;
void mqttSub() {
  mqttClient.subscribe(ledR);
  mqttClient.subscribe(ledG);
  mqttClient.subscribe(ledB);
  mqttClient.subscribe(led2);
  mqttClient.subscribe(servo);
}
void commandHandleMqtt(String t, String m) {
  Serial.print("Message arrived from Mqtt Dash app on topic [");
  Serial.print(t);
  Serial.print("] is ");
  Serial.println(m);
}

void wifiServer() {
   // Check for any command from Arduino Controller app
  String s = appHandler();
  if (s != "") {
    // Manipulate the string according to your needs
    Serial.print("Message arrived from Ardino Controller app is ");
    Serial.println(s);
    
    String pin, command;
    if (s.indexOf("IN") >= 0) {
      pin = s.substring(3, 4);
      command = s.substring(4);
      Serial.print("Pin number is ");
      Serial.print(pin);
      Serial.print(" and command is ");
      Serial.println(command);
      int p = pin.toInt();
      if (command.indexOf("ON") != -1) {
        Serial.println("Led is on");
        digitalWrite(p, HIGH);
      }
      else {
        Serial.println("Led is off");
        digitalWrite(p, LOW);
      }
    }
  }
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
  //mqtt setup
  mqttClient.setServer(mqttServer, 1883);
  mqttClient.setCallback(callback_fun);
  //pin Setup
  pinMode(2, OUTPUT);
  Serial.println("Setup done");
}
void loop()
{
  wifiServer(); //Handles Wifi local server
  mqttLoop(); // Handles Mqtt Connection to Broker
  if (change && mqttClient.connected()) {
    change = false;
    mqttPublish(id1, String(rfidTag)); //topic and message
    mqttPublish(id2, String(pirFlag)); //topic and message}
  }
}

void appData(WiFiClient cl) {
  //To write to Arduino Controller application
  cl.println("<br/><a id=" + id1 + ">" + String(rfidTag) + "</a>");
  cl.println("<br/><a id=" + id2 + ">" + String(pirFlag) + "</a>");
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
long mqttRetry = millis();
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
