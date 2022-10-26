#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define SERVER_IP "shrimp-python-2.herokuapp.com" //這邊打上你自己的heroku網址

#ifndef STASSID
#define STASSID "BMTE503" //wifi ssid
#define STAPSK  "78157802" //wifi password
#endif

void setup() {

  Serial.begin(115200);
  pinMode(0, INPUT);

  Serial.println();
  Serial.println();
  Serial.println();

  WiFi.begin(STASSID, STAPSK);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  
}

void loop() {
  // wait for WiFi connection
  String mes;
  if (Serial.available()) {
    mes = Serial.readString();
    Serial.print("OK");
  }
  //FEED
  if ((WiFi.status() == WL_CONNECTED) and mes == "f") {

    WiFiClient client;
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    // configure traged server and url
    http.begin(client, "http://" SERVER_IP "/feed"); //HTTP
    http.addHeader("Content-Type", "application/json");

    
    Serial.print("[HTTP] POST...\n");
    // start connection and send HTTP header and body
    int httpCode = http.POST(" ");
    
    
    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
        Serial.println("received payload:\n<<");
        Serial.println(payload);
        Serial.println(">>");
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
  
  //WATER LEVEL
  if ((WiFi.status() == WL_CONNECTED) and mes == "w") {

    WiFiClient client;
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    // configure traged server and url
    http.begin(client, "http://" SERVER_IP "/wl"); //HTTP
    http.addHeader("Content-Type", "application/json");

    
    Serial.print("[HTTP] POST...\n");
    // start connection and send HTTP header and body
    int httpCode = http.POST(" ");
    
    
    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
        Serial.println("received payload:\n<<");
        Serial.println(payload);
        Serial.println(">>");
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
  delay(500);
}
