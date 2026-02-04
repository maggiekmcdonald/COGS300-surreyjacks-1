import processing.serial.*;

Serial myPort;
String distance = "0";

void setup() {
  size(400, 250);
  println(Serial.list());

  // CHANGE THIS INDEX to your Arduino port
  myPort = new Serial(this, Serial.list()[0], 9600);
}

void draw() {
  background(30);

  fill(255);
  textSize(20);
  text("Distance: " + distance + " cm", 50, 60);

  textSize(16);
  text("Modes:", 50, 110);
  text("0 = Ultrasonic Test", 50, 130);
  text("1 = Follow Me (P)", 50, 150);
  text("2 = Wall Follow (P)", 50, 170);
  text("3 = Manual Control", 50, 190);

  text("Manual: W A S D, Space = Stop", 50, 220);

  // Read distance from Arduino
  while (myPort.available() > 0) {
    String incoming = myPort.readStringUntil('\n');
    if (incoming != null) {
      distance = trim(incoming);
    }
  }
}

void keyPressed() {
  // Mode switching
  if (key == '0') myPort.write('0');
  if (key == '1') myPort.write('1');
  if (key == '2') myPort.write('2');
  if (key == '3') myPort.write('3');

  // Manual control (mode 3)
  if (key == 'w' || key == 'W') myPort.write('F');
  if (key == 's' || key == 'S') myPort.write('B');
  if (key == 'a' || key == 'A') myPort.write('L');
  if (key == 'd' || key == 'D') myPort.write('R');
  if (key == ' ') myPort.write('S');
}
