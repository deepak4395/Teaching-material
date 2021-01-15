/*
   Mqtt Library : PubSubClient by Nick O'Leary, Availale in Library Manger (Ctrl+shift+I)
   Mqtt Dash: https://play.google.com/store/apps/details?id=net.routix.mqttdash
   Update the code with your MQTT Broker details to avoid interfacing of other users to control your system.
   Set your own broker at flespi.io

   This code controls inbuilt led from mqtt dash app and send/publish random to same app.
*/
#include <WiFi.h>
#include <PubSubClient.h>
// Wifi Credentials
char ssid[] = "M 57"; //Put your wifi name.
char password[] = "8376918157"; //Put your wifi password.

// MQTT Broker Credentials flespi.io
const char server[] = "industrial.api.ubidots.com";
char mqttUserName[] = "BBFF-xOOEgZbhctePwa45L4dWwOJOsc7QDn";
char mqttPass[] = "";
const char clientID[] = "0800956539"; // change according to your no. must be unique in the world.
// Topics to subscibe and publish
const char topicSub[] = "/v1.6/devices/esp32-2/led";
String topicPub = "/v1.6/devices/esp32-2"; // /v1.6/devices/<device name>
//Messages to sent
String ms = "{\"ms\": 1}"; // {"<sensor label>": <sensor vale>} json format
String ldrS="{\"ldrValue\": 1}";
//String ms = "\"";
WiFiClient client;
PubSubClient mqttClient(client);

void mqttSub() {
  mqttClient.subscribe(topicSub);
}
void callback_fun(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived from [");
  Serial.print(topic);
  Serial.print("]");
  String s = "";
  for (int i = 0; i < length; i++) {
    s = s + (char)payload[i];
  }
  Serial.println(s);
  if ()
  if (topic = "led") {
    if (s.indexOf("1") >= 0) {
      digitalWrite(2, HIGH);
    }
    else digitalWrite(2, LOW);
  }
}

void setup() {
  Serial.begin(9600);
  //Wifi Setup
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
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
  //mqtt setup
  mqttClient.setServer(server, 1883);
  mqttClient.setCallback(callback_fun);
  //pin setup
  pinMode(2, OUTPUT);
  pinMode(23,INPUT_PULLDOWN);
  Serial.println("Setup Done");
}
int i = 0,ldrV=0;
void loop() {
  
  mqttLoop();
  //mqttPublish(topicPub, String(rfid)); //topic and message
  mqttPublish(topicPub, String(ms));
  //mqttPublish(topicPub, String(light));
  delay(1000);
  i++;
  ms = "{\"ms\": " + String(i) + "}";
  Serial.println(ms); //{"ms":0}
  ldrV=analogRead(23); // minimum Value =500 and Maximum value 2000, Treshold= (minV + maxV)/2 ~ 1250
  ldrS = "{\"ldrValue\": " + String(ldrV) + "}";
  mqttPublish(topicPub, String(ldrS));
}
// No changes below this
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
      Serial.print("failed, Reason =");
      Serial.print(mqttClient.state());
      Serial.println(" will try again in 5 seconds");
      mqttRetry = millis();
    }
  }
  mqttClient.loop();
}

void mqttPublish(String t, String m) {
  if (mqttClient.publish( t.c_str(), m.c_str())) Serial.println("Message published");
}

//String light = "{\"light\": 2300}";
//String rfid = "{\"temperature\": 27}";
