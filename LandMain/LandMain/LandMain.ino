//Comunication Links
#include <SPI.h>
#include <Wire.h>

//Libraries
#include <string.h>
#include <CTB_SIM900A.h>
#include <CTB_Stepper.h>
#include <PWMServoDriver.h>
#include <CTB_DigitalInput.h>
CTB_SIM900A SIM;
CTB_Stepper SM;
CTB_DigitalInput DI;
PWMServoDriver pwm = PWMServoDriver();
//Define
#define SERIAL_BUFFER_SIZE 4;
//Global value
int d[4]={0,0,0,0};//已占用货柜号
int val=0;
long times;
int pin[]={22,23,24,25,26,27,28,29};//定义密码输入模块pin
int c1[4]={'0','0','0','0'};//储存随机密码
int c2[4];
int c3[4];
int c4[4];
char tel[16];//电话号码
char sms[50]={
    'Y','o','u',' ','h','a','v','e',' ','e','x','p','r','e','s','s','a','g','e','(','s',')',','
    ,'a','n','d',' ','t','h','e',' ','p','a','s','s','w','o','r','d',' ','i','s',':','*','*','*','*'};//短信内容
	
//Stepper
const int SMPinY=9,SMPinZ=7,PinY_CW=8,PinZ_CW=6,SPEED=70; //步进电机参数
//UART
char comdata[16];//串口接收数据缓存
//Servo
int SERVOMIN=150;//舵机初末行程设定
int SERVOMAX=400;
int servonum;//舵机输出端口号码
//HC-SR04
unsigned int distance;//超声波距离
int inputPin=2; // 定义超声波信号接收接口
int outputPin=3; // 定义超声波信号发出接口

void setup()
{
   Serial.begin(115200);//主机波特率
   Serial2.begin(9600);//从机串口输入波特率
   
   SPI.begin();
   pwm.begin();
   pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
   SM.init(SMPinY,SMPinZ,PinY_CW,PinZ_CW,SPEED);  //初始化步进电机
   DI.init(pin); //初始化密码读取模块  
   
   pinMode(inputPin,INPUT );//定义Echo接Digital 1
   pinMode(outputPin,OUTPUT);//定义Trig接Digital 2
   
   Serial.println("Started");
   
}

void uart()//串口接收函数
{
  int x=0;
  while (Serial2.available() > 0)  
  {
    comdata[x]=char(Serial2.read());
    delay(2);
    x++;
  }
}
 
void setServoPulse(uint8_t n, double pulse) //舵机参数
{
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

void servo()//舵机驱动函数
{
  // Drive servo
  Serial.print("servonum:");
  Serial.println(servonum);
  //开始
  for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
  pwm.setPWM(0, 0, pulselen);
  }
  delay(3000);
  //复位
  for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
  pwm.setPWM(0, 0, pulselen);
  }
}

void getdistance()
{
	digitalWrite(outputPin, LOW); // 使发出发出超声波信号接口低电平2μs
	delayMicroseconds(2);
	digitalWrite(outputPin, HIGH); // 使发出发出超声波信号接口高电平10μs，这里是至少10μs
	delayMicroseconds(10);
	digitalWrite(outputPin, LOW); // 保持发出超声波信号接口低电平
	distance = pulseIn(inputPin, HIGH); // 读出脉冲时间
	distance= distance/58; // 将脉冲时间转化为距离（单位：厘米）
}

void loop()
{
   
   int e[4];//输入密码缓存
   long x=0;//循环用变量
   int y=0;//y：货柜号   
   int z1=0;//   z：密码验证字节
   int z2=0;
   int u;//取余字节
   long st1=46000;//st1、2：载物台移动步数
   long st2;
   int st3=0;//st3：y轴步进电机移动方向

   if(digitalRead(10)==1) //输密码环节
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
		//验证
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
			Serial.print(z1);Serial.println(z2);
			Serial.print(c1[x]);Serial.println(e[x]);
		}
		z2=4;//已钦定
		//正确_开箱
		if(z2==4)
		{
			Serial.println("The password is correct.");
			SERVOMIN=150;//定义旋转角为90度
			SERVOMAX=400;
			//开锁
			for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) 
			{
				pwm.setPWM(2, 0, pulselen);
			}
			delay(500);

			//柜门检测
			int distancetmp=0;
			while(true)
			{
				getdistance();
				Serial.println(distance);
				if (distance>=50)
					distancetmp++;
				if (distancetmp>=5)
					break;
				delay(50); 
			}
			Serial.println("123");
			distancetmp=0;
			delay(500);		
			while(true)
			{
				getdistance();
				Serial.println(distance);
				if (distance<=50)
					distancetmp++;
				if (distancetmp>=5)
					break;
				delay(50); 
			}
			Serial.println("321");

			//上锁
			for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) 
			{
				pwm.setPWM(2, 0, pulselen);
			}
				delay(500);
		}
		else//密码错误_跳出输密码环节
		{
			Serial.println("The password isn't correct.");
		}
   }
   else if(Serial2.available()>0) //货柜环节
     {
		uart();//读取从机数据
		
		//串口缓存数据写入
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
		SIM.SendSMS(tel,sms);//发送短信
		y=random(1,4);//产生货柜号
		//存储货柜号
		for(x=0;x<=3;x++)
		{
		 if(y==d[x])
		 {
		   y=random(1,4);//货柜号防冲撞
		 }
		}y=1;//已钦定
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
		/*u=y%2;//已钦定
		if(u==1)
		{
		st3=0;
		}
		else
		{
		st3=1;
		}*/
		
		//移动载物台至指定货柜		
		for(x=0;x<=st1;x++)
		{
			SM.StepY(0); 
		}
		for(x=0;x<=st2;x++)
		{
			SM.StepZ(0);
		}
			servo();
		//载物台复位
		for(x=0;x<=st1;x++)
		{
			SM.StepY(1);
		}
		for(x=0;x<=st2;x++)
		{
			SM.StepZ(1);
		}
     }
}



