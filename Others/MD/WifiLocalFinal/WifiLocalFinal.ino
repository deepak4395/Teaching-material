#include <WiFi.h>

// Your wifi SSID and Password
const char* ssid = "M 57"; //update
const char* password = "8376918157"; //update

// Gateway and IP must has same masking for first 3 places.
IPAddress ip(192, 168, 0, 200);           //Set IP  //update
IPAddress gateway(192, 168, 0, 1);        //Set Gateway // update
IPAddress subnet(255, 255, 255, 0);       //Set Subnet

// Define id and message
String mesg1 = "Hello.. everyone";
String id1 = "id1";
String mesg2 = "from Deepak";
String id2 = "id2";

WiFiServer server(80);
void setup()
{
  // Serial begin
  Serial.begin(115200);

  //Wifi Setup
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet); //update
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
  //Server begin
  server.begin();
  //pin Setup
  pinMode(2, OUTPUT);

  Serial.println("Setup done");
}
void loop()
{
  String s = appHandler();
  if (s != "") {
    // Manipulate the string according to your needs
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
  delay(10);
}
void appData(WiFiClient cl) {
  //To read
  cl.println("<br/><a id=" + id1 + ">" + String(mesg1) + "</a>");
  cl.println("<br/><a id=" + id2 + ">" + String(mesg2) + "</a>");
}

// No changes below this

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
