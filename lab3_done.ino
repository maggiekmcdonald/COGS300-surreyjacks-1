// ===== Motor Pins =====
const int IN1 = 3;   // Motor A
const int IN2 = 6;
const int IN3 = 4;   // Motor B (reversed)
const int IN4 = 5;
const int ENA = 9;   // PWM speed Motor A
const int ENB = 10;  // PWM speed Motor B

// ===== Global Variables =====
int currentSpeed = 180;     // current speed 0-255
char currentAction = 'X';   // current movement: F/B/L/R/S/X/8

// Figure-8 timing
unsigned long lastMoveTime = 0;
int figure8Step = 0;

void setup() {
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  stopMotors();
}

// ===== Main loop =====
void loop() {
  unsigned long now = millis();

  // ----- 1. Read serial commands -----
  while (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    if (cmd.length() == 0) continue;

    char action = cmd.charAt(0);

    // Speed change: number keys 0-9
    if (action >= '0' && action <= '9') {
      currentSpeed = (action - '0') * 28; // 0->0, 9->252
    } else {
      // Movement command
      currentAction = action;
      // Reset figure-8 if command changes
      if (currentAction != '8') figure8Step = 0;
      lastMoveTime = now;
    }
  }

  // ----- 2. Execute movement -----
  if (currentAction == '8') {
    // Non-blocking figure-8 state machine
    handleFigure8(now);
  } else {
    // Normal movement
    switch(currentAction) {
      case 'F': forward(currentSpeed); break;
      case 'B': backward(currentSpeed); break;
      case 'L': left(currentSpeed); break;
      case 'R': right(currentSpeed); break;
      case 'S': spin(currentSpeed); break;
      case 'X': stopMotors(); break;
    }
  }

  delay(20); // small loop delay to avoid CPU spamming
}

// ===== Movement functions =====
void forward(int s) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);   // Motor B reversed
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, s);
  analogWrite(ENB, s);
}

void backward(int s) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);  // Motor B reversed
  digitalWrite(IN4, LOW);
  analogWrite(ENA, s);
  analogWrite(ENB, s);
}

void left(int s) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, s);
  analogWrite(ENB, s);
}

void right(int s) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, s);
  analogWrite(ENB, s);
}

void spin(int s) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, s);
  analogWrite(ENB, s);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

// ===== Figure-8 Handler =====
void handleFigure8(unsigned long now) {
  switch (figure8Step) {
    case 0:
      forward(currentSpeed);
      if (now - lastMoveTime > 800) { // move forward first loop
        lastMoveTime = now;
        figure8Step = 1;
      }
      break;
    case 1:
      right(currentSpeed);
      if (now - lastMoveTime > 500) { // turn right
        lastMoveTime = now;
        figure8Step = 2;
      }
      break;
    case 2:
      forward(currentSpeed);
      if (now - lastMoveTime > 800) { // move forward second loop
        lastMoveTime = now;
        figure8Step = 3;
      }
      break;
    case 3:
      left(currentSpeed);
      if (now - lastMoveTime > 500) { // turn left to complete 8
        lastMoveTime = now;
        figure8Step = 0;
        currentAction = 'X'; // stop after figure-8
      }
      break;
  }
}

