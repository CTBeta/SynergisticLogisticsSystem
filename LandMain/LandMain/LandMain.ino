#include <SPI.h>
#include <Wire.h>
#include <string.h>
#include <CTB_SIM900A.h>
#include <CTB_Stepper.h>
#include <PWMServoDriver.h>
#include <CTB_DigitalInput.h>
#define SERIAL_BUFFER_SIZE 4;
int d[4]={0,0,0,0};//已占用货柜号
int val=0;
long times;
int pin[]={22,23,24,25,26,27,28,29};//定义pin
CTB_Stepper SM;
CTB_DigitalInput DI;
PWMServoDriver pwm = PWMServoDriver();
const int SMPinY=9,SMPinZ=7,PinY_CW=8,PinZ_CW=6,SPEED=70; //步进电机参数
char comdata[16];
int SERVOMIN=150;
int SERVOMAX=400;
int servonum;
int distance;
int inputPin=2; // 定义超声波信号接收接口
int outputPin=3; // 定义超声波信号发出接口

void uart()
{
  int x=0;
  while (Serial2.available() > 0)  
  {
    comdata[x]=char(Serial2.read());
    delay(2);
    x++;
  }
}
 
void setup()
{
   Serial.begin(115200);
   Serial2.begin(9600);
   SPI.begin();
   pwm.begin();
   SM.init(SMPinY,SMPinZ,PinY_CW,PinZ_CW,SPEED);
   Serial.println("16 channel Servo test!");
   pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
   DI.init(pin);
   Serial.println("Press the button to get the expressage .");
   pinMode(inputPin,INPUT );//定义Echo接Digital 1
   pinMode(outputPin,OUTPUT);//定义Trig接Digital 2
}

void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 60;   // 60 Hz
  Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse *= 1000;
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}

void servo()
{
  // Drive each servo one at a time
  Serial.print("servonum:");
  Serial.println(servonum);
  for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
  pwm.setPWM(0, 0, pulselen);
  pwm.setPWM(1, 0, pulselen);
  }
  delay(3000);
  for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
  pwm.setPWM(0, 0, pulselen);
  pwm.setPWM(1, 0, pulselen);
  }
}

int c1[4]={'0','0','0','0'};//储存随机密码
int c2[4];
int c3[4];
int c4[4];
void loop()
{
   char tel[16];
   char sms[50]={
    'Y','o','u',' ','h','a','v','e',' ','e','x','p','r','e','s','s','a','g','e','(','s',')',','
    ,'a','n','d',' ','t','h','e',' ','p','a','s','s','w','o','r','d',' ','i','s',':','*','*','*','*'};

   int e[4];//输入密码缓存
   int x,y=0;//y：货柜号   
   int z1=0;//   z：验证字节
   int z2=0;
   int u;//取余字节
   int st1=46000;//st1、2：载物台移动步数
   int st2;
   int st3=0;//st3：y轴步进电机移动方向
   int vale;
   CTB_SIM900A SIM;
   //输入密码
   if(digitalRead(30)==1)
   {
      Serial.println("Please enter the password .");
      for(x=0;x<=3;x++)
       {
        Serial.print(c1[x]);
       }
      for(x=0;x<=3;x++)
      {
        if(val!=DI.get()||millis()-times>1000)//定义延迟
        {
        val=DI.get();
        Serial.println(val);
        times=millis();
        e[x]=val;
        }
        delay(1000);
      }
      for(x=0;x<=3;x++)
      {
        if(c1[x]==e[x])
        {
          z1=1;
        }
        else
        {
          z1=0;
        }
        z2=z2+z1;
        Serial.println(z1);Serial.println(z2);
        Serial.println(c1[x]);Serial.println(e[x]);
        
      }z2=4;
      if(z2==4)
      {
        Serial.println("The password is correct.");
        SERVOMIN=150;
        SERVOMAX=400;
        for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
        pwm.setPWM(2, 0, pulselen);
        }
        delay(500);
        //清除密码

        while(distance<30)
        {
          digitalWrite(outputPin, LOW); // 使发出发出超声波信号接口低电平2μs
          delayMicroseconds(2);
          digitalWrite(outputPin, HIGH); // 使发出发出超声波信号接口高电平10μs，这里是至少10μs
          delayMicroseconds(10);
          digitalWrite(outputPin, LOW); // 保持发出超声波信号接口低电平
          distance = pulseIn(inputPin, HIGH); // 读出脉冲时间
          distance= distance/58; // 将脉冲时间转化为距离（单位：厘米）
          
          delay(50); 
        }Serial.println("123");
        while(distance>30)
        {
          digitalWrite(outputPin, LOW); // 使发出发出超声波信号接口低电平2μs
          delayMicroseconds(2);
          digitalWrite(outputPin, HIGH); // 使发出发出超声波信号接口高电平10μs，这里是至少10μs
          delayMicroseconds(10);
          digitalWrite(outputPin, LOW); // 保持发出超声波信号接口低电平
          distance = pulseIn(inputPin, HIGH); // 读出脉冲时间
          distance= distance/58; // 将脉冲时间转化为距离（单位：厘米）
          
          delay(50); 
        }Serial.println("321");
        for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
        pwm.setPWM(2, 0, pulselen);
        }
        delay(500);
      }
      else
      {
        Serial.println("The password isn't correct.");
      }
   }
   else
   {
     if(Serial2.available()>0)
     {
       uart();
       for(x=0;x<=10;x++)
       {
         tel[x]=comdata[x];
       }
       //发送短信及取货密码
       for(x=43;x<=46;x++)
       {
         sms[x]=random(1,8)+48;//产生随机密码
       }
       //暂存密码
       c1[0]=sms[43]-48;
       c1[1]=sms[44]-48;
       c1[2]=sms[45]-48;
       c1[3]=sms[46]-48;
       for(x=0;x<=3;x++)
       {
        Serial.print(c1[x]);
       }
       SIM.SendSMS(tel,sms);//发送短信
       y=random(1,4);//产生货柜号
       //存储货柜号
       for(x=0;x<=3;x++)
       {
         if(y==d[x])
         {
           y=random(1,4);
         }
       }y=1;
       servonum=y-1;
       //取载物台移动步数
       if(y<=2)
       {
         st2=20000;
       }
       else
       {
         st2=80000;
       }
       //取y轴移动方向
       u=y%2;
       if(u==1)
       {
        st3=0;
       }
       else
       {
        st3=1;
       }
       //移动载物台至指定货柜
       for(x=0;x<=st1;x++)
       {
         SM.StepY(st3);
       }
       for(x=0;x<=st2;x++)
       {
         SM.StepZ(0);
       }
       servo();
       //载物台复位
       for(x=0;x<=st1;x++)
       {
         SM.StepY(1-st3);
       }
       for(x=0;x<=st2;x++)
       {
         SM.StepZ(1);
       }
     }
   }
}



