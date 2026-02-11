// ===================== PINS =====================
// Right ultrasonic
const int trigRight = 7;
const int echoRight = 2;

// Front ultrasonic
const int trigFront = 8;
const int echoFront = 9;

// Motor driver
const int enA = 10;   // LEFT motor PWM
const int enB = 11;   // RIGHT motor PWM
const int in1 = 6;
const int in2 = 3;
const int in3 = 4;
const int in4 = 5;

// ===================== SPEED SETTINGS =====================
const int baseSpeed     = 80;   // straight forward speed
const int turnSpeed     = 70;   // turning speed
const int approachSpeed = 60;   // approaching front wall
const float Kp          = 3.0; // proportional gain for wall following

// ===================== DISTANCE SETTINGS =====================
const int desiredRightDist = 15;  // cm from right wall
const int frontStopDist   = 15;   // cm from front wall
const int wallLostDist    = 60;   // right wall considered lost

// ===================== STATE =====================
enum RobotState {
  FORWARD,
  TURN_RIGHT,
  APPROACH_FRONT,
  TURN_LEFT
};

RobotState state = FORWARD;

void setup() {
  Serial.begin(9600);

  // Ultrasonic pins
  pinMode(trigRight, OUTPUT);
  pinMode(echoRight, INPUT);
  pinMode(trigFront, OUTPUT);
  pinMode(echoFront, INPUT);

  // Motor pins
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  stopMotors();
  Serial.println("Smooth wall follower ready");
}

void loop() {
  // Read sensors
  int rightDist = readUltrasonic(trigRight, echoRight);
  int frontDist = readUltrasonic(trigFront, echoFront);

  Serial.print("Right: ");
  Serial.print(rightDist);
  Serial.print(" cm | Front: ");
  Serial.print(frontDist);
  Serial.println(" cm");

  switch (state) {

    case FORWARD: {
      // Inside corner: front wall detected
      if (frontDist <= frontStopDist + 2) {
        Serial.println("Approaching front wall -> TURN_LEFT");
        state = APPROACH_FRONT;
        stopMotors();
      }
      // Outside corner: right wall clearly gone
      else if (rightDist > wallLostDist && rightDist < 180) {
        Serial.println("Outside corner -> TURN_RIGHT");
        state = TURN_RIGHT;
      }
      // Normal wall following
      else {
        // Proportional steering
        int error = desiredRightDist - rightDist;
        int leftSpeed  = baseSpeed + Kp * error;
        int rightSpeed = baseSpeed - Kp * error;

        // Clamp speeds
        leftSpeed  = constrain(leftSpeed, 0, 255);
        rightSpeed = constrain(rightSpeed, 0, 255);

        driveForward(leftSpeed, rightSpeed);
      }
      break;
    }

    case TURN_RIGHT:
      spinRight(turnSpeed);
      if (rightDist <= desiredRightDist + 2) {
        Serial.println("Right wall found -> FORWARD");
        state = FORWARD;
      }
      break;

    case APPROACH_FRONT:
      driveForward(approachSpeed, approachSpeed);
      if (frontDist <= frontStopDist) {
        Serial.println("Front reached -> TURN_LEFT");
        state = TURN_LEFT;
      }
      break;

    case TURN_LEFT:
      spinLeft(turnSpeed);
      if (rightDist <= desiredRightDist + 2) {
        Serial.println("Wall on right -> FORWARD");
        state = FORWARD;
      }
      break;
  }

  delay(20); // small delay
}

// ===================== ULTRASONIC =====================
int readUltrasonic(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 25000);
  if (duration == 0) return 200;  // no echo

  return duration * 0.034 / 2;
}

// ===================== MOTOR CONTROL =====================
void driveForward(int leftSpeed, int rightSpeed) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  analogWrite(enA, leftSpeed);
  analogWrite(enB, rightSpeed);
}

void spinRight(int speed) {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  analogWrite(enA, speed);
  analogWrite(enB, speed);
}

void spinLeft(int speed) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(enA, speed);
  analogWrite(enB, speed);
}

void stopMotors() {
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}

