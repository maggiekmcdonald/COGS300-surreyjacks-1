// =====================================================
//  UNIVERSAL ROBOT CODE FOR LAB 05
//  Modes:
//   0 = Ultrasonic test
//   1 = Straight-line P-controller (Follow Me)
//   2 = Wall-following P-controller
//   3 = Manual control from Processing
// =====================================================

// Ultrasonic pins
int trigPin = 10;
int echoPin = 11;

// Motor pins (CHANGE THESE TO MATCH YOUR ROBOT)
int leftPWM = 5;
int leftDir = 4;
int rightPWM = 6;
int rightDir = 7;

// Control parameters
float setPointFollow = 25.0;   // Follow-me distance
float setPointWall = 20.0;     // Wall-follow distance
float pFollow = 5.0;           // P gain for follow-me
float pWall = 4.0;             // P gain for wall-follow
int baseSpeed = 120;           // Forward speed

// Mode variable
int mode = 0;

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(leftPWM, OUTPUT);
  pinMode(leftDir, OUTPUT);
  pinMode(rightPWM, OUTPUT);
  pinMode(rightDir, OUTPUT);
}

// -----------------------------------------------------
//  ULTRASONIC FUNCTION
// -----------------------------------------------------
long getDistanceCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long cm = duration * 0.0343 / 2;
  return cm;
}

// -----------------------------------------------------
//  MOTOR FUNCTION
// -----------------------------------------------------
void setMotor(int pwmPin, int dirPin, int speed) {
  if (speed >= 0) {
    digitalWrite(dirPin, HIGH);
    analogWrite(pwmPin, speed);
  } else {
    digitalWrite(dirPin, LOW);
    analogWrite(pwmPin, -speed);
  }
}

void stopRobot() {
  setMotor(leftPWM, leftDir, 0);
  setMotor(rightPWM, rightDir, 0);
}

// -----------------------------------------------------
//  MANUAL DRIVE COMMANDS
// -----------------------------------------------------
void driveForward() {
  setMotor(leftPWM, leftDir, 150);
  setMotor(rightPWM, rightDir, 150);
}

void driveBackward() {
  setMotor(leftPWM, leftDir, -150);
  setMotor(rightPWM, rightDir, -150);
}

void turnLeft() {
  setMotor(leftPWM, leftDir, -150);
  setMotor(rightPWM, rightDir, 150);
}

void turnRight() {
  setMotor(leftPWM, leftDir, 150);
  setMotor(rightPWM, rightDir, -150);
}

// -----------------------------------------------------
//  LOOP
// -----------------------------------------------------
void loop() {

  // Read distance
  long dist = getDistanceCM();

  // Send distance to Processing
  Serial.println(dist);

  // Check for mode or manual commands
  if (Serial.available()) {
    char cmd = Serial.read();

    if (cmd == '0') mode = 0;
    if (cmd == '1') mode = 1;
    if (cmd == '2') mode = 2;
    if (cmd == '3') mode = 3;

    // Manual commands only work in mode 3
    if (mode == 3) {
      if (cmd == 'F') driveForward();
      if (cmd == 'B') driveBackward();
      if (cmd == 'L') turnLeft();
      if (cmd == 'R') turnRight();
      if (cmd == 'S') stopRobot();
    }
  }

  // -------------------------
  // MODE 0: ULTRASONIC TEST
  // -------------------------
  if (mode == 0) {
    stopRobot();
    delay(50);
  }

  // -------------------------
  // MODE 1: FOLLOW-ME (P)
  // -------------------------
  if (mode == 1) {
    float error = dist - setPointFollow;
    float output = pFollow * error;
    int speed = constrain(output, -255, 255);

    setMotor(leftPWM, leftDir, speed);
    setMotor(rightPWM, rightDir, speed);
  }

  // -------------------------
  // MODE 2: WALL FOLLOWING (P)
  // -------------------------
  if (mode == 2) {
    float error = dist - setPointWall;
    float correction = pWall * error;

    int leftSpeed = constrain(baseSpeed + correction, 0, 255);
    int rightSpeed = constrain(baseSpeed - correction, 0, 255);

    setMotor(leftPWM, leftDir, leftSpeed);
    setMotor(rightPWM, rightDir, rightSpeed);
  }

  delay(50);
}
