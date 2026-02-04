// ===== Motor pins =====
const int IN1 = 3;  
const int IN2 = 6;
const int IN3 = 4;  
const int IN4 = 5;
const int ENA = 9;
const int ENB = 10;

// ===== Ultrasonic pins =====
const int trigPin = 7;
const int echoPin = 2;

// ===== Wall following parameters =====
const int desiredDistance = 10;  // cm from wall
const int baseSpeed = 160;
const float Kp = 2.0;            // steering gain (tune this)

void setup() {
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  stopMotors();
}

void loop() {
  int distance = readUltrasonic();

  Serial.print("Right distance (cm): ");
  Serial.println(distance);

  // Error: positive = too far from wall
  int error = distance - desiredDistance;

  // Proportional steering
  int correction = Kp * error;

  int leftSpeed  = baseSpeed + correction;
  int rightSpeed = baseSpeed - correction;

  // Safety limits
  leftSpeed  = constrain(leftSpeed, 0, 255);
  rightSpeed = constrain(rightSpeed, 0, 255);

  driveForward(leftSpeed, rightSpeed);

  delay(20);
}

// ===== Ultrasonic read =====
int readUltrasonic() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 25000); // timeout ~4m

  if (duration == 0) return 100; // no wall detected → far away

  int distance = duration * 0.034 / 2;
  return distance;
}

// ===== Motor control =====
void driveForward(int leftSpeed, int rightSpeed) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
