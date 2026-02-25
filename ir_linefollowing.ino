// Arduino Line Follower Robot Code
//More Information: https://circuitdigest.com/microcontroller-projects/arduino-uno-line-follower-robot 
#define enA 10//Enable1 L293 Pin enA 
#define in1 6 //Motor1  L293 Pin in1 
#define in2 3 //Motor1  L293 Pin in1 
#define in3 5 //Motor2  L293 Pin in1 
#define in4 4 //Motor2  L293 Pin in1 
#define enB 11 //Enable2 L293 Pin enB 
#define R_S 12//ir sensor Right
#define L_S 2 //ir sensor Left

int motorSpeed = 100; 


void setup(){ 
  pinMode(R_S, INPUT); 
  pinMode(L_S, INPUT); 
  pinMode(enA, OUTPUT); 
  pinMode(in1, OUTPUT); 
  pinMode(in2, OUTPUT); 
  pinMode(in3, OUTPUT); 
  pinMode(in4, OUTPUT); 
  pinMode(enB, OUTPUT);

  // Use PWM instead of HIGH
  analogWrite(enA, motorSpeed);
  analogWrite(enB, motorSpeed);

  delay(1000);
}

void loop(){  
if((digitalRead(R_S) == 1)&&(digitalRead(L_S) == 1)){forward();}   //if Right Sensor and Left Sensor are at White color then it will call forword function
if((digitalRead(R_S) == 0)&&(digitalRead(L_S) == 1)){turnRight();} //if Right Sensor is Black and Left Sensor is White then it will call turn Right function  
if((digitalRead(R_S) == 1)&&(digitalRead(L_S) == 0)){turnLeft();}  //if Right Sensor is White and Left Sensor is Black then it will call turn Left function
if((digitalRead(R_S) == 0)&&(digitalRead(L_S) == 0)){Stop();} //if Right Sensor and Left Sensor are at Black color then it will call Stop function
}

void forward(){
  analogWrite(enA, 80);  
  analogWrite(enB, 80);

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void turnRight(){
  analogWrite(enA, 90);   // slower right motor
  analogWrite(enB, 130);  // faster left motor

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void turnLeft(){
  analogWrite(enA, 130);
  analogWrite(enB, 90);

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
void Stop(){ //stop
digitalWrite(in1, LOW); //Right Motor forword Pin 
digitalWrite(in2, LOW); //Right Motor backword Pin 
digitalWrite(in3, LOW); //Left Motor backword Pin 
digitalWrite(in4, LOW); //Left Motor forword Pin 
}