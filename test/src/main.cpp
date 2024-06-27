#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

// const char* ssid = "FRITZ!Box 7730CL";
// const char* password = "oi oi oi"; // Replace with your WiFi password

const char *ssid = "WLAN-HAHE-2";
const char *password = "20328687415601431680200280";

const int analogInPin = 35;       // Analog input pin that the sensor is attached to
const float V_ZERO_CURRENT = 2.5; // Zero-current voltage (2.5V for a 5V system)
const float SENSITIVITY = 0.100;  // Sensitivity (185 mV/A for 5A version)
const int NUM_SAMPLES = 20;     // Number of samples to take

#define LED_OPTIMIZED 5
#define LED_UNOPTIMIZED 6

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
      float currentSumSqr = 0;

      // Take a number of samples and convert to current
      for (int i = 0; i < NUM_SAMPLES; i++)
      {
            int sensorValue = analogRead(analogInPin);           // Read the analog input
            float voltage = sensorValue * (20.0 / 4095.0); // Convert the reading to voltage
            float current = (voltage-V_ZERO_CURRENT) / SENSITIVITY;               // Convert voltage to current
            currentSumSqr += current * current;                  // Add square of current to sum
      }

      // Calculate RMS current
      float currentSqrAverage = currentSumSqr / NUM_SAMPLES; // Mean of the squared current values
      float currentAverage = sqrt(currentSqrAverage);        // Root Mean Square of the current

      // Print the results to the Serial Monitor
      Serial.print("RMS3+ Current = ");
      Serial.print(currentAverage);
      Serial.println(" A");

      delay(1000); // Wait for a second before next reading
}