int tmp;
void setup() {
  pinMode(2,INPUT);
  pinMode(3,OUTPUT);
  pinMode(13,OUTPUT);
}

void loop() {
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);   
  if(digitalRead(2)==1&&tmp!=1)
  {
    digitalWrite(3,HIGH);
    delay(5000);
    tmp=1;
  } 
}
