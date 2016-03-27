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
  
  digitalWrite(13, HIGH);   
  delay(1000);              
  digitalWrite(13, LOW);    
  delay(1000);   
  
  if(digitalRead(8)==1&&tmp!=1)
  {
    delay(5000);
    myservo.write(180); 
    tmp=1;
  } 
}
