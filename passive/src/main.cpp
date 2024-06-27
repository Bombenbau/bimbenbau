#include <Arduino.h>
#include <Servo.h>

// Altitudes and Azimuths given in the project specifications, 1st value: altitude, 2nd value: azimtuh
int servoCoordinates[5][2] = {{10, 160}, {25, 120}, {50, 90}, {25, 60}, {10, 20}}; // For azimuth: 0 = 90, 180 = -90 n the Project Specifications to give the value directly to the servo; altitude values adjusted for servo error

// The servos are to be moved to the locations given in the project specifications
// The servos are controlled by a PWM signal, which is generated by the ESP32

#define SERVO_AXIS_X 11
#define SERVO_AXIS_Y 10
#define BUTTON_PIN 2

Servo servoX;
Servo servoY;

void setup()
{
	servoX.attach(SERVO_AXIS_X);
	servoY.attach(SERVO_AXIS_Y);

	pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop()
{
	// Move the servos to each of the positions
	for (auto &rows : servoCoordinates)
	{
		servoY.write(rows[0]); // Write altitude
		servoX.write(rows[1]); // Write azimuth

		//Move on button press
		// Wait, until the button is pressed
		while (digitalRead(BUTTON_PIN) == HIGH)
			delay(10);

		// Wait, until the button is released
		while (digitalRead(BUTTON_PIN) == LOW)
			delay(10);
	}
}