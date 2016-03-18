//SIM900A
#include <stdio.h>;  
#include <math.h>     
int i;
int len; 
int times;
int temp;
char post[]=
	{
		"POST /v1.1/device/20809/sensor/aaaaa/datapoints HTTP/1.1\r\n"
		"Host: api.yeelink.net\r\n"
		"Accept: */*\r\n"
		"U-ApiKey:bc861b4c13710e771df169d012ce7547\r\n"
		"Content-Length: dt\r\n"
		"Content-Type: application/x-www-form-urlencoded\r\n"
		"Connection: close\r\n"
		"\r\n"
		"{\"value\":xxxxxx}\r\n"
    };
char post_backup[]=
	{
		"POST /v1.1/device/20809/sensor/aaaaa/datapoints HTTP/1.1\r\n"
		"Host: api.yeelink.net\r\n"
		"Accept: */*\r\n"
		"U-ApiKey:bc861b4c13710e771df169d012ce7547\r\n"
		"Content-Length: dt\r\n"
		"Content-Type: application/x-www-form-urlencoded\r\n"
		"Connection: close\r\n"
		"\r\n"
		"{\"value\":xxxxxx}\r\n"
    };
int postdata_id[4];
long idint=0;
int data=0;


//DSM501A
int PMpin = 8;//DSM501A端口定义（PWM）
unsigned long starttime;
unsigned long duration;//定义中间量
unsigned long samplelastime_ms = 5000;//抽样时间（ms）
unsigned long lowpulseoccupancy = 0; //定义占有时间
float ratio = 0;//定义比率
float concentration = 0;//定义浓度
unsigned long pm25val;//定义浓度ug/m^3
const float pm25coef=0.00207916725464941;//定义将浓度换算成ug/m^3的标准量


//BMP180
#include "SFE_BMP180.h"
#include <Wire.h>
SFE_BMP180 pressure;// 创建一个气压计对象
double baseline; // 定义基准气压
const double baselineh=1013.2;

//HC-SR04
const int TrigPin = 2; //定义TrigPin为digitalx接口
const int EchoPin = 3; //定义Echopin为digitalx接口
float cm; 

void setup()
{
	Serial.begin(115200);
	//DSM501A
	pinMode(PMpin,INPUT);
	//BMP180
	pressure.begin();
        baseline = getP()-4;//获取标准气压并修正误差
	//HC-SR04
	pinMode(TrigPin, OUTPUT); //设定该接口为输出模式
	pinMode(EchoPin, INPUT); //设定该接口为输入模式
	//MQ9-1.5v供电
	pinMode(11,OUTPUT);
	analogWrite(11, 76.5); 
	//5V供电
	pinMode(13,OUTPUT);
	digitalWrite(13,HIGH);
}
 
void loop()
{
	PM();
	mq();
	//TemperatureANDPressure();
        bmp180();
	uv();
	distance();	
}

void mq()
{
    int mq2,mq9,mq135,co,so;
    mq2=analogRead(0);
	//mq2=mq2/20.48;
	mq9=analogRead(1);
	//mq9=mq9/20.48;
	mq135=analogRead(2);
	//mq135=mq135/20.48;
        co=analogRead(6);
        co=1024-co;
        so=analogRead(7);
	
    idint=36466;
    data=mq2;
	postdata();
	
	idint=36467;
    data=mq9;
	postdata();
	
	idint=36468;
    data=mq135;
	postdata();
	
	idint=36469;
    data=co;
	postdata();
	
        idint=36470;
    data=so;
	postdata();
	
}

void distance() 
{ 
	digitalWrite(TrigPin, LOW); 
	delayMicroseconds(2); //发送一个短时间的低电平脉冲到Trig
	digitalWrite(TrigPin, HIGH); 
	delayMicroseconds(10); //发送一个短时间的高电平脉冲到Trig
	digitalWrite(TrigPin, LOW);//Trig调回低电平 
 
	cm = pulseIn(EchoPin, HIGH) / 58.0; //将回波时间换算成cm
	cm = (int(cm * 100.0)) / 100.0; //保留两位小数 
	
	idint=36471;
    data=cm;
	postdata();

}

void uv()
{
    float valv,valw;
    int level;
    valv=analogRead(3);
    valw=0.05384*valv-2.69230;
	
	if(valw==0) level=0;
	if(valw>0&&valw<=2.5) level=1;
	if(valw>2.5&&valw<=5) level=2;
	if(valw>5&&valw<=7.5) level=3;
	if(valw>7.5&&valw<=10) level=4;
	if(valw>10&&valw<=12.5) level=5;
	if(valw>12.5&&valw<=15) level=6;
	if(valw>15&&valw<=20) level=7;
	if(valw>20&&valw<=25) level=8;
	if(valw>25&&valw<=30) level=9;
	if(valw>30&&valw<=35) level=10;
	if(valw>30) level=11;
	
	idint=36463;
    data=valw;
	postdata();
	
	idint=36464;
    data=level;
	postdata();
}
/*
void TemperatureANDPressure()
{
 
  int chk = DHT11.read(DHT11PIN);
 
  //Serial.print("Read sensor: ");
  switch (chk)
  {
    case DHTLIB_OK: 
                //Serial.println("OK"); 
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                //Serial.println("Checksum error"); 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
               // Serial.println("Time out error"); 
                break;
    default: 
                //Serial.println("Unknown error"); 
                break;
  }
  
	idint=36460;
    data=(float)DHT11.humidity;
	postdata();
	
	idint=36459;
    data=(float)DHT11.temperature-2;
	postdata();
}
*/
void PM() 
{
	starttime=millis();
	do
	{
		duration= pulseIn(PMpin, LOW);//定义采样时间
		lowpulseoccupancy=lowpulseoccupancy+duration;
	}
	while(millis()-starttime<samplelastime_ms);
	
	ratio = lowpulseoccupancy/(samplelastime_ms*10.0);  // Integer percentage 0=>100 整数百分比
        concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; //using spec sheet curve使用说明书曲线
	pm25val=pm25coef*concentration;//换算到ug/m^3
	

	idint = 36462;
        data=pm25val;
	postdata();
	
}

void bmp180()
{
	double a,h,p,t;
	p = getP()-4;// 获得一个气压值并修正误差
	a = pressure.altitude(p,baselineh);//获得基于海平面的高度值
	h = pressure.altitude(p,baseline);//获得基于基准气压的高度值
	
	idint=36461;
    data=p;
	postdata();
	
	idint=36763;
    data=a;
	postdata();
	
	idint=36767;
    data=h;
	postdata();

	t = getT();// 获得一个温度值
        t=t-5;
	idint=36459;
    data=t;
	postdata();

}
//
double getP()
{
  char status;
  double T,P,p0,a;
  status = pressure.startTemperature();
  if (status != 0)
  {
	delay(status);
    status = pressure.getTemperature(T);
    if (status != 0)
    {
      status = pressure.startPressure(3);
      if (status != 0)
      {
        delay(status);
        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          return P;
        }
      }
    }
  }
}

double getT()
{
	
  char status;
  double T,p0;
  status = pressure.startTemperature();
  if (status != 0)
  {
    delay(status);
    status = pressure.getTemperature(T);
    if (status != 0)
    {
      status = pressure.startPressure(3);
      return T;
    }
  }
}



char postdata() 
{  
connectTCP();	
 /*   //int转char
	temp=idint/10000;
	postdata_id[0]=temp;
	Serial.println(postdata_id[0]);
	
	temp=(idint%10000)/1000;
	postdata_id[1]=temp;
	Serial.println(postdata_id[1]);
	
	temp=(idint%1000)/100;
	postdata_id[2]=temp;
	Serial.println(postdata_id[2]);
	
	temp=(idint%100)/10;
	postdata_id[3]=temp;
	Serial.println(postdata_id[3]);
	
	temp=idint%10;
	postdata_id[4]=temp;
	Serial.println(postdata_id[4]);
	
	//填写ID
	len=strlen(post);
	for(times=0;times<len;times++) 
	{
	  if(post[times]=='a'&&post[times+1]=='a'&&post[times+2]=='a')
	   { 
		  post[times]=postdata_id[0];
		  post[times+1]=postdata_id[1];
		  post[times+2]=postdata_id[2];
		  post[times+3]=postdata_id[3];
		  post[times+4]=postdata_id[4];
		  break;        
	   }
	}*/
	//填写数据长度
	temp=0;
	len=strlen(post);   
	//temp=Serial.print(data,1);//////////////////////////////////
        count();
      //  Serial.print(temp);
	for(times=0;times<len;times++)
	{
	   if(post[times]=='d'&&post[times+1]=='t')
	   {
			post[times]=(temp+10)/10+0x30;  
			post[times+1]=(temp+10)%10+0x30;
			break;
	   }
	}
	//填改传感器数值，发送数据
	for(times=0;times<len;times++)
	 {
	   if(post[times]=='x'&&post[times+1]=='x'&&post[times+2]=='x') 
	   {
			Serial.print(data,1);                 
			times=times+6;
		}
		if(post[times]=='a'&&post[times+1]=='a'&&post[times+2]=='a') 
	   {
			Serial.print(idint,1);                 
			times=times+5;
		}
		Serial.print(post[times]);
	 } 
	Serial.write(26);//send order
	delay(3000);
	for(times=0;times<len;times++)
	{
		post[times]=post_backup[times];
	}       
}
void connectTCP()//TCP连接
{
	int i;
	Serial.print("ati\r\n"); 
	delay(1000);
	Serial.print("ati\r\n"); 
	delay(1000);
	Serial.print("ati\r\n"); 
	delay(1000);
	delay(500); 
	Serial.print("AT+CGCLASS=\"B\"\r\n"); 
	delay(500); 
	Serial.print("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n"); 
	delay(500);
	Serial.print("AT+CGATT=1\r\n"); 
	delay(500); 
	Serial.print("AT+CIPCSGP=1,\"CMNET\"\r\n"); //uninet
	delay(500);  
	Serial.print("AT+CLPORT=\"TCP\",\"2000\"\r\n"); 
	delay(500);  
	Serial.print("AT+CIPSTART=\"TCP\",\"42.96.164.52\",\"80\"\r\n"); //the ip addr to yeelink
	delay(500);   
	for(i=0;i<=9;i++)
	{
		delay(1000); 
	}
	Serial.print("AT+CIPSEND\r\n"); 
	delay(500);
 }
void closeTCP()//结束TCP连接
{
	Serial.print("AT+CIPCLOSE=1\r\n"); 
	delay(500);
	Serial.print("AT+CIPSHUT\r\n"); 
	delay(500);
 }
void count()
 {
   int element,count;
   count=0;
   element=data;
   while(element!=0)
   {
     element=element/10;
     count++;
   }
   temp=count;
 }
 


