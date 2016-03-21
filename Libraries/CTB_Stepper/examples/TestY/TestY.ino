const int SMPinY=9,SMPinZ=1,PinY_CW=8,PinZ_CW=1,SPEED=100;
#include <CTB_Stepper.h>
CTB_Stepper SM;
void setup() 
{ 
  Serial.begin(9600);
  SM.init(SMPinY,SMPinZ,PinY_CW,PinZ_CW,SPEED);
}

void loop() 
{
 for(int i=0;i<=5000;i++)
  {
    SM.StepY(0);
  }
  for(int i=0;i<=5000;i++)
  {
    SM.StepY(1);
  }
}
