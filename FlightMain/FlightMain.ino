int tmp;
void setup() {
  pinMode(2,INPUT);
  pinMode(3,OUTPUT);
}

void loop() {
  if(digitalRead(2)==1&&tmp!=1)
  {
    digitalWrite(3,HIGH);
    delay(5000);
    tmp=1;
  } 
}
