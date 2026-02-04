// ===== Motor pins =====
const int IN1 = 3;  // Motor A
const int IN2 = 6;
const int IN3 = 4;  // Motor B (reversed)
const int IN4 = 5;
const int ENA = 9;
const int ENB = 10;

// Photocell
const int photocellPin = A0;
const int threshold = 150; // adjust depending on your tape/lighting

void setup() {
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  stopMotors();
}

void loop() {
  int sensorValue = analogRead(photocellPin);
  Serial.println(sensorValue);

  if (sensorValue > threshold) {
    // Tape detected → move forward
    forward(180);
  } else {
    // Tape lost → turn right until tape is seen
    spinRight(150);
  }

  delay(10);
}

// ===== Motor functions =====
void forward(int speed) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void spinRight(int speed) {
  // Spin clockwise in place
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}
