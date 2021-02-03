#include <WiFiManager.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

ESP8266WiFiMulti WiFiMulti;

// select which pin will trigger the configuration portal when set to LOW
#define TRIGGER_PIN 0
const int BOTAO_0 = 2;

int timeout = 120; // seconds to run for

void setup() {
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP  
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("\n Starting");
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  pinMode(BOTAO_0, INPUT);
}

void loop() {
  // is configuration portal requested?
  if ( digitalRead(TRIGGER_PIN) == LOW) {
    WiFiManager wm;    

    //reset settings - for testing
    //wifiManager.resetSettings();
  
    // set configportal timeout
    wm.setConfigPortalTimeout(timeout);

    if (!wm.startConfigPortal("OnDemandAP")) {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
      //reset and try again, or maybe put it to deep sleep
      ESP.restart();
      delay(5000);
    }

    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");

  }

  // put your main code here, to run repeatedly:
  if ((WiFiMulti.run() == WL_CONNECTED) && digitalRead(BOTAO_0) == HIGH) {
    WiFiClient client;
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, "http://localhost:4200/api/v1/obs/:id").replace(":id", String(1))) {
      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);
        }
        else if (httpCode == HTTP_CODE_INTERNAL_SERVER_ERROR) {
          Serial.printf("[HTTP] deu ruim no GET.");
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
  }
}
