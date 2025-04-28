#include <Servo.h>

// Set output pins
const int TriggerPin = 11;
const int EchoPin = 12;
const int motorSignalPin = 9;
const int buzzerPin = 7;

// Starting location
const int startAngle = 90;

// Rotation limits
const int minimumAngle = 6;
const int maximumAngle = 175;

// Speed
const int degreesPerCycle = 1;

// Library class instance
Servo motor;

// Timing variables
unsigned long previousMillis = 0; // Store last time the buzzer beeped
const long interval = 100; // Interval for the buzzer (in milliseconds)

void setup() {
  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  motor.attach(motorSignalPin);
  pinMode(buzzerPin, OUTPUT); // Set buzzer pin as output
  Serial.begin(9600);
}

void loop() {
  static int currentAngle = startAngle;
  static int motorRotateAmount = degreesPerCycle;

  // Move motor
  motor.write(currentAngle);

  // Calculate distance
  int distance = CalculateDistance();
  SerialOutput(currentAngle, distance);

  // Buzzer logic (without blocking)
  unsigned long currentMillis = millis();  // Get current time
  if (distance > 5 && distance < 40) { // Only beep if object is within range
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;  // Update last time the buzzer beeped
      tone(buzzerPin, 1000);  // Send 1000Hz tone
      delay(50);  // Small delay to create beep
      noTone(buzzerPin);  // Stop the tone
    }
  } else {
    noTone(buzzerPin); // Stop the buzzer if nothing is in range
  }

  // Update motor position
  currentAngle += motorRotateAmount;

  // Reverse motor direction if limits reached
  if (currentAngle <= minimumAngle || currentAngle >= maximumAngle) {
    motorRotateAmount = -motorRotateAmount;
  }

  // Delay for servo movement timing
  delay(10);
}

int CalculateDistance() {
  // Trigger the ultrasonic sensor and record the time taken for the reflect
  digitalWrite(TriggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerPin, LOW);
  long duration = pulseIn(EchoPin, HIGH);
  
  // Convert the duration to a distance
  float distance = duration * 0.017F;
  return int(distance);
}

void SerialOutput(const int angle, const int distance) {
  // Convert the angle and distance to a string and serial print
  Serial.println(String(angle) + "," + String(distance));
}

