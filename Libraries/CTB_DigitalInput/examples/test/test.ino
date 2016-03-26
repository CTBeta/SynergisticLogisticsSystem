#include <CTB_DigitalInput.h>
CTB_DigitalInput DI;
int val=0;
long times;
int pin[]={2,3,4,5,6,7,8,9};//定义pin

void setup() {
  DI.init(pin);
  Serial.begin(9600);
}

void loop() {
  if(val!=DI.get()||millis()-times>1000)//定义延迟
  {
      val=DI.get();
      Serial.println(val);
      times=millis();
  }
}
