#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "FRITZ!Box 7730CL";
const char* password = "oi oi oi"; // Replace with your WiFi password

void setup() {
      Serial.begin(115200);
      WiFi.begin(ssid, password); // Connect to WiFi network
      while (WiFi.status() != WL_CONNECTED) {
            delay(1000);
            Serial.println("Connecting to WiFi...");
      }
      Serial.println("Connected to WiFi");

      pinMode(33, INPUT_PULLUP);
}

int lastButtonState = 2;

void loop() {
      int buttonState = digitalRead(33);

      if (buttonState != lastButtonState) {
            Serial.println(buttonState);
            lastButtonState = buttonState;

            HTTPClient http;

            char* path = "https://dev.bombenbau.com/";

            http.begin(path + String(buttonState));
            int httpCode = http.GET();
      }


      delay(100);
}