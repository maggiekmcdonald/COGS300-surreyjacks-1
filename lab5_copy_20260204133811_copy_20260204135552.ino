// =====================================================
//  LAB 05: ULTRASONIC + P-CONTROL + WALL FOLLOWING
//  Modes:
//   0 = Ultrasonic test
//   1 = Straight-line P-controller (Follow Me)
//   2 = Wall-following P-controller
//   3 = Manual control from Processing
// =====================================================

// -------------------- PIN SETUP ----------------------

// Ultrasonic pins (your available pins)
int trigPin = 7;
int echoPin = 2;

// Motor pins (adjust PWM pins if needed)
int leftPWM = 11;     // PWM-capable
int leftDir = 8;      // free pin
int rightPWM = 5;     // MUST be PWM-capable (5 or 6)
int rightDir = 4;     // existing robot pin

// -------------------- CONTROL VALUES -----------------

float setPointFollow = 25.0;   // Follow-me distance
float setPointWall = 20.0;     // Wall-follow distance
float pFollow = 5.0;           // P gain for follow-me
float pWall = 4.0;             // P gain for wall-follow
int baseSpeed = 120;           // Forward speed

int mode = 0;                  // Default mode

// =====================================================
//  SETUP
// =====================================================
void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(leftPWM, OUTPUT);
  pinMode(leftDir, OUTPUT);
  pinMode(rightPWM, OUTPUT);
  pinMode(rightDir, OUTPUT);
}

// =====================================================
//  ULTRASONIC FUNCTION
// =====================================================
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

// =====================================================
//  MOTOR FUNCTIONS
// =====================================================
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

// =====================================================
//  MAIN LOOP
// =====================================================
void loop() {

  long dist = getDistanceCM();
  Serial.println(dist);   // send to Processing

  // -------------------- MODE SWITCHING --------------------
  if (Serial.available()) {
    char cmd = Serial.read();

    if (cmd == '0') mode = 0;
    if (cmd == '1') mode = 1;
    if (cmd == '2') mode = 2;
    if (cmd == '3') mode = 3;

    // Manual control only in mode 3
    if (mode == 3) {
      if (cmd == 'F') driveForward();
      if (cmd == 'B') driveBackward();
      if (cmd == 'L') turnLeft();
      if (cmd == 'R') turnRight();
      if (cmd == 'S') stopRobot();
    }
  }

  // -------------------- MODE 0: ULTRASONIC TEST --------------------
  if (mode == 0) {
    stopRobot();
  }

  // -------------------- MODE 1: FOLLOW-ME --------------------
  if (mode == 1) {
    float error = dist - setPointFollow;
    float output = pFollow * error;
    int speed = constrain(output, -255, 255);

    setMotor(leftPWM, leftDir, speed);
    setMotor(rightPWM, rightDir, speed);
  }

  // -------------------- MODE 2: WALL FOLLOWING --------------------
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
