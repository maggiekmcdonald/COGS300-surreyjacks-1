// ===================== PINS =====================
// Right ultrasonic
const int trigRight = 7;
const int echoRight = 2;

// Front ultrasonic
const int trigFront = 8;
const int echoFront = 9;

// Motor driver
const int enA = 10;   // Motor A enable (PWM) -> in1/in2 side
const int enB = 11;   // Motor B enable (PWM) -> in3/in4 side
const int in1 = 6;
const int in2 = 3;
const int in3 = 4;
const int in4 = 5;

// ===================== TUNING =====================
int driveSpeed = 90;        // forward speed
int turnSpeed  = 110;       // turning speed
int minPWM = 0;
int maxPWM = 255;

// If you want to slow ONLY in3/in4 motor (Motor B), set < 1.0
float scaleA = 1.00;  // Motor A scale
float scaleB = 0.80;  // Motor B scale (in3/in4)  <-- reduce to slow it

// Wall detection thresholds (cm)
float frontWallDist = 20.0;
float rightWallDist = 20.0;

// "No wall" threshold (cm) - optional for timeouts
float farClamp = 250.0;

// Timings (ms)
int turnTimeMs = 350;   // how long to pivot for a turn (tune!)
int loopDelay  = 40;

// ===================== ULTRASONIC READ =====================
float readCM(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH, 30000UL);
  if (duration == 0) return 999.0;
  return (duration * 0.0343) / 2.0;
}

// Average 3 reads for stability
float readAvg3(int trigPin, int echoPin) {
  float a = readCM(trigPin, echoPin); delay(5);
  float b = readCM(trigPin, echoPin); delay(5);
  float c = readCM(trigPin, echoPin);

  if (a > farClamp) a = farClamp;
  if (b > farClamp) b = farClamp;
  if (c > farClamp) c = farClamp;

  return (a + b + c) / 3.0;
}

// ===================== MOTOR HELPERS =====================
void writePWM(int pwmA, int pwmB) {
  pwmA = constrain(pwmA, minPWM, maxPWM);
  pwmB = constrain(pwmB, minPWM, maxPWM);

  pwmA = (int)(pwmA * scaleA);
  pwmB = (int)(pwmB * scaleB);

  pwmA = constrain(pwmA, minPWM, maxPWM);
  pwmB = constrain(pwmB, minPWM, maxPWM);

  analogWrite(enA, pwmA);
  analogWrite(enB, pwmB);
}

void stopMotors() {
  writePWM(0, 0);
}

void forward() {
  // both forward
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
  writePWM(driveSpeed, driveSpeed);
}

void pivotLeft() {
  // (SWAPPED) A forward, B backward
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);  digitalWrite(in4, HIGH);
  writePWM(turnSpeed, turnSpeed);
}

void pivotRight() {
  // (SWAPPED) A backward, B forward
  digitalWrite(in1, LOW);  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
  writePWM(turnSpeed, turnSpeed);
}

// ===================== SETUP =====================
void setup() {
  Serial.begin(9600);

  pinMode(trigRight, OUTPUT);
  pinMode(echoRight, INPUT);
  pinMode(trigFront, OUTPUT);
  pinMode(echoFront, INPUT);

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  stopMotors();
  delay(300);
}

// ===================== LOOP =====================
void loop() {
  float rightCM = readAvg3(trigRight, echoRight);
  float frontCM = readAvg3(trigFront, echoFront);

  bool rightWall = (rightCM < rightWallDist);
  bool frontWall = (frontCM < frontWallDist);

  Serial.print("Right: "); Serial.print(rightCM);
  Serial.print("  Front: "); Serial.print(frontCM);
  Serial.print("  | rightWall="); Serial.print(rightWall);
  Serial.print(" frontWall="); Serial.println(frontWall);

  // RULES YOU REQUESTED:
  // 1) Wall on right AND wall in front -> turn LEFT
  if (rightWall && frontWall) {
    stopMotors();
    delay(60);
    pivotLeft();
    delay(turnTimeMs);
    stopMotors();
    delay(60);
  }
  // 2) Nothing on right AND nothing in front -> turn RIGHT
  else if (!rightWall && !frontWall) {
    stopMotors();
    delay(60);
    pivotRight();
    delay(turnTimeMs);
    stopMotors();
    delay(60);
  }
  // Otherwise: go forward
  else {
    forward();
  }

  delay(loopDelay);
}
