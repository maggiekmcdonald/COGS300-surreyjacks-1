/*
 * Dual Ultrasonic Sensor HC-SR04 - Left and Right Distance Measurement
 * 
 * LEFT Sensor Pins:
 *     VCC: +5VDC
 *     Trig: Pin 10
 *     Echo: Pin 11
 *     GND: GND
 * 
 * RIGHT Sensor Pins:
 *     VCC: +5VDC
 *     Trig: Pin 12
 *     Echo: Pin 13
 *     GND: GND
 */

// Left sensor pins
int trigPinLeft = 10;
int echoPinLeft = 11;

// Right sensor pins
int trigPinRight = 12;
int echoPinRight = 13;

long duration, cmLeft, cmRight;
 
void setup() {
  // Serial Port begin
  Serial.begin(9600);
  
  // Define inputs and outputs for LEFT sensor
  pinMode(trigPinLeft, OUTPUT);
  pinMode(echoPinLeft, INPUT);
  
  // Define inputs and outputs for RIGHT sensor
  pinMode(trigPinRight, OUTPUT);
  pinMode(echoPinRight, INPUT);
  
  Serial.println("Dual Ultrasonic Sensor Ready");
}

long readDistance(int trigPin, int echoPin) {
  // Ensure trigger pin is LOW
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Send 10 microsecond pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the echo pulse (timeout after 30ms = ~5 meters max)
  duration = pulseIn(echoPin, HIGH, 30000);
  
  // Convert the time into distance in cm
  return (duration / 2) / 29.1;
}
 
void loop() {
  // Read LEFT sensor
  cmLeft = readDistance(trigPinLeft, echoPinLeft);
  delay(50); // Small delay between sensor readings
  
  // Read RIGHT sensor
  cmRight = readDistance(trigPinRight, echoPinRight);
  
  // Display LEFT sensor
  if (cmLeft == 0) {
    Serial.print("Out of range L");
  } else if (cmLeft > 400) {
    Serial.print("Too far L");
  } else {
    Serial.print(cmLeft);
    Serial.print("cm L");
  }
  
  Serial.print(" | ");
  
  // Display RIGHT sensor
  if (cmRight == 0) {
    Serial.print("Out of range R");
  } else if (cmRight > 400) {
    Serial.print("Too far R");
  } else {
    Serial.print(cmRight);
    Serial.print("cm R");
  }
  
  Serial.println();
  
  delay(500);  // Wait 500ms between measurement cycles
}
