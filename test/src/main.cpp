#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

// const char* ssid = "FRITZ!Box 7730CL";
// const char* password = "oi oi oi"; // Replace with your WiFi password

const char *ssid = "FRITZ!Box 7730SIGMA";
const char *password = "i accidentally ate crack";

const float V_ZERO_CURRENT = 2.5; // Sensor Zero-current voltage (2.5V for a 5V system)
const float SENSITIVITY = 0.100;  // Sensor Sensitivity (100 mV/A for 20A version)
const int NUM_SAMPLES = 20;       // Number of samples to take

#define SENSOR_UNOPTIMIZED 35 // Analog input pin that the sensor to the optimized solar panel is attached to
#define SENSOR_OPTIMIZED 33   // Analog input pin that the sensor to the unoptimized solar panel is attached to
#define LED_OPTIMIZED 19
#define LED_UNOPTIMIZED 18

void setup()
{
      Serial.begin(115200);
      WiFi.begin(ssid, password); // Connect to WiFi network

      while (WiFi.status() != WL_CONNECTED)
      {
            delay(1000);
            Serial.println("Connecting to WiFi...");
      }
      Serial.println("Connected to WiFi");

      pinMode(LED_OPTIMIZED, OUTPUT);
      pinMode(LED_UNOPTIMIZED, OUTPUT);
}

void loop()
{
      float currentSumSqr1 = 0;
      float currentSumSqr2 = 0;

      // Take a number of samples and convert to current
      for (int i = 0; i < NUM_SAMPLES; i++)
      {
            int sensorValue1 = analogRead(SENSOR_UNOPTIMIZED);          // Read the analog input
            float voltage1 = sensorValue1 * (20.0 / 4095.0);            // Convert the reading to voltage
            float current1 = (voltage1 - V_ZERO_CURRENT) / SENSITIVITY; // Convert voltage to current
            currentSumSqr1 += current1 * current1;                      // Add square of current to sum

            // Repeat for the optimized solar panel
            int sensorValue2 = analogRead(SENSOR_OPTIMIZED);
            float voltage2 = sensorValue1 * (20.0 / 4095.0);
            float current2 = (voltage2 - V_ZERO_CURRENT) / SENSITIVITY;
            currentSumSqr2 += current2 * current2;
      }

      // Calculate RMS current for the unoptimized solar panel
      float currentSqrAverage1 = currentSumSqr1 / NUM_SAMPLES; // Mean of the squared current values
      float currentAverage1 = sqrt(currentSqrAverage1);        // Root Mean Square of the current

      float currentMilliAverage1 = map(currentAverage1, 126.55, 132.60, 0, 850);
      currentMilliAverage1 = max(0.0f, min(currentMilliAverage1, 850.0f));

      // Repeat for the optimized solar panel
      float currentSqrAverage2 = currentSumSqr2 / NUM_SAMPLES;
      float currentAverage2 = sqrt(currentSqrAverage2);

      float currentMilliAverage2 = map(currentAverage2, 126.55, 132.60, 0, 850);
      currentMilliAverage2 = max(0.0f, min(currentMilliAverage2, 850.0f));

      // Display the results on the LEDs
      int unoptimizedLEDOutput = map(currentMilliAverage1, 0, 850, 0, 255);
      int optimizedLEDOutput = map(currentMilliAverage2, 0, 850, 0, 255);
      analogWrite(LED_UNOPTIMIZED, unoptimizedLEDOutput);
      analogWrite(LED_OPTIMIZED, optimizedLEDOutput);

      // Print the results to the Serial Monitor
      Serial.print("RMS3+ Current (unoptimized) = ");
      Serial.print(currentMilliAverage1);
      Serial.println(" mA");
      Serial.print(currentAverage1);
      Serial.println(" A");
      Serial.print("RMS3+ Current (optimized) = ");
      Serial.print(currentMilliAverage2);
      Serial.println(" mA");
      Serial.print(currentAverage2);
      Serial.println(" A");

      // Send the results to the server
      HTTPClient http;
      String url = "https://dev.bombenbau.com/optimized/" + String(currentMilliAverage2) + "/unoptimized/" + String(currentMilliAverage1);
      http.begin(url);
      int httpCode = http.GET();

      delay(1000); // Wait for a second before next reading
}