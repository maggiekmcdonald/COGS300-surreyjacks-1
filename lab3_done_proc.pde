import processing.serial.*;

Serial myPort;

void setup() {
  size(400, 300);
  printArray(Serial.list());
  myPort = new Serial(this, Serial.list()[7], 9600);
}

void draw() {
  background(200);
  fill(0);
  textAlign(CENTER, CENTER);
  text("W/F: Forward\nS/B: Backward\nA/L: Left\nD/R: Right\nQ: Spin\nX: Stop\n8: Figure 8\nNumber keys 0-9 = speed", width/2, height/2);
}

int speed = 180; // default speed

void keyPressed() {
  // Set speed with number keys 0-9
  if (key >= '0' && key <= '9') {
    speed = (key - '0') * 28; // 0->0, 9->252
    println("Speed set to " + speed);
    return;
  }

  if (key == 'w' || key == 'W') myPort.write("F" + speed + "\n");
  if (key == 's' || key == 'S') myPort.write("B" + speed + "\n");
  if (key == 'a' || key == 'A') myPort.write("L" + speed + "\n");
  if (key == 'd' || key == 'D') myPort.write("R" + speed + "\n");
  if (key == 'q' || key == 'Q') myPort.write("S" + speed + "\n");
  if (key == 'x' || key == 'X') myPort.write("X\n");
  if (key == '8') myPort.write("8" + speed + "\n"); // Figure 8
}
