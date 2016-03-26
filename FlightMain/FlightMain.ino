#include <Servo.h> 
Servo myservo;
int tmp=0;
void setup() {
  pinMode(8,INPUT);
  myservo.attach(9);
  pinMode(13,OUTPUT);
  myservo.write(0);
  
}

void loop() {
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);   
  if(digitalRead(8)==1&&tmp!=1)
  {
    delay(5000);
    myservo.write(180); 
    tmp=1;
  } 
}
