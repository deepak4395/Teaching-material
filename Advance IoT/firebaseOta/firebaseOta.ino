#define FIREBASE_HOST "ble-beacon-a5913.firebaseio.com"
#define FIREBASE_AUTH "uOIi0l52NWuMmY0HFBhFbVXQvC9ONrPhIY5tJnfw"
#define WIFI_SSID "M 57"
#define WIFI_PASSWORD "8376918157"
#define FIREBASE_UPDATE_AVAILABLE "/firmware/updateAvailable"
#define FIREBASE_FIRMWARE_URL "/firmware/URL"
long UPDATE_CHECK_TIME = 5000;
String ver = "3.0.0.0";

#include <firebaseOTA.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

FirebaseData firebaseData;

void setup() {
  Serial.begin(115200);
  Serial.print("Running Firmware Ver: ");
  Serial.println(ver);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  checkUpdate();
  Serial.println(" Advance iot batch is great");
  delay(1000);
}
long lastUpdateCheck = millis();
void checkUpdate() {
  if (millis() - lastUpdateCheck > UPDATE_CHECK_TIME) {
    lastUpdateCheck = millis();
    if (Firebase.getBool(firebaseData, FIREBASE_UPDATE_AVAILABLE))
    {
      bool b = firebaseData.boolData();
      if (b) {
        Serial.println("Update Available");
        if (!Firebase.setBool(firebaseData, FIREBASE_UPDATE_AVAILABLE, false))
        {
          Serial.print("Error in setBool, ");
          Serial.println(firebaseData.errorReason());
        }
        if (Firebase.getString(firebaseData, FIREBASE_FIRMWARE_URL)) {
          String s = firebaseData.stringData();
          Serial.println("Firmare URL=");
          Serial.println(s);
          updateFirmware(s);
        }
        else {
          Serial.print("Error in getString, ");
          Serial.println(firebaseData.errorReason());
        }
      }
      else Serial.println("No Updates");
    } else {
      Serial.print("Error in getBool, ");
      Serial.println(firebaseData.errorReason());
    }
  }
}
