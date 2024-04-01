#include <AFMotor.h>
const int trigPin = 9;
const int echoPin = 10;

AF_DCMotor motor(1, MOTOR12_1KHZ);

long duration;
int distance;



void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Pulse send
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  // Pulse off
  digitalWrite(trigPin, LOW);
  // Time Read
  duration = pulseIn(echoPin, HIGH);

  // Distance Calculation
  distance = duration * 0.034 / 2;
  motor.setSpeed(250);

  if(distance==20 && distance==100){
    Serial.println("far");
    motor.run(RELEASE);

  }
  else if(distance<=20){
    Serial.println("close");
    motor.run(FORWARD);
  }
  else if (distance>=100){
    Serial.println("very far");
    motor.run(BACKWARD);
  }


  delay(500);


}