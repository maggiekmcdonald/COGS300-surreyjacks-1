// IR sensors
const int leftSensor = A0;
const int rightSensor = A1;

// Motor driver pins
const int enA = 10;  // LEFT motor PWM
const int enB = 11;  // RIGHT motor PWM
const int in1 = 6;
const int in2 = 3;
const int in3 = 4;
const int in4 = 5;

// Thresholds
const int threshold = 475; // adjust after testing

// Base speed
const int baseSpeed = 180;
const int turnSpeed = 120;

void setup() {
  pinMode(leftSensor, INPUT);
  pinMode(rightSensor, INPUT);

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  int leftValue = analogRead(leftSensor);
  int rightValue = analogRead(rightSensor);

  Serial.print("Left: "); Serial.print(leftValue);
  Serial.print(" Right: "); Serial.println(rightValue);

  if (leftValue < threshold && rightValue < threshold) {
    // Both sensors on black: keep going forward
    moveForward(baseSpeed, baseSpeed);
  } 
  else if (leftValue > threshold && rightValue < threshold) {
    // Left sees white → turn right gently
    moveForward(turnSpeed, baseSpeed);
  } 
  else if (leftValue < threshold && rightValue > threshold) {
    // Right sees white → turn left gently
    moveForward(baseSpeed, turnSpeed);
  } 
  else if (leftValue > threshold && rightValue > threshold) {
    // Both see white → on track, go forward
    moveForward(baseSpeed, baseSpeed);
  }
}

// Function to move forward with separate left/right speeds
void moveForward(int leftSpeed, int rightSpeed) {
  // Left motor
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, leftSpeed);

  // Right motor
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, rightSpeed);
}

void stopMotors() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}

