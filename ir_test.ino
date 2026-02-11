int leftSensor = A1;
int rightSensor = A0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int leftValue = analogRead(leftSensor);
  int rightValue = analogRead(rightSensor);
  Serial.print("Left: "); Serial.print(leftValue);
  Serial.print(" Right: "); Serial.println(rightValue);
  delay(200);
}
