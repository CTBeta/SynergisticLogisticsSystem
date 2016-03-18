#include <SPI.h>
#include <RFID.h>
#include <CTB_SIM900A.h>
#include <CTB_Stepper.h>

CTB_Stepper SM;
const int SMPinY=9,SMPinZ=1,PinY_CW=8,PinZ_CW=1,SPEED=100; //步进电机参数
RFID rfid(10,5); //D10--读卡器MOSI引脚、D5--读卡器RST引脚

//RC522
//4字节卡序列号，第5字节为校验字节
unsigned char serNum[5];
//写卡数据
unsigned char sectorKeyA[16][16] = {
 {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
 {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
 {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},};
//新扇区A密码，16个扇区，每个扇区密码6Byte
unsigned char sectorNewKeyA[16][16] = {
 {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
 {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xff,0x07,0x80,0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
 {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xff,0x07,0x80,0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},};

void setup()
{
   Serial.begin(9600);
   SPI.begin();
   rfid.init();
   SM.init(SMPinY,SMPinZ,PinY_CW,PinZ_CW,SPEED);
}

void loop()
{
   char tel[11];
   char sms[50]={
    'Y','o','u',' ','h','a','v','e',' ','e','x','p','r','e','s','s','a','g','e','(','s',')',','
    ,'a','n','d',' ','t','h','e',' ','p','a','s','s','w','o','r','d',' ','i','s',':','*','*','*','*'};
   char c[4];//储存临时随机密码
   int d[4]={0,0,0,0};//已占用货柜号
   int x,y,st1,st2=0;//y：货柜号   st：载物台移动步数
   CTB_SIM900A SIM;
   unsigned char i,tmp;
   unsigned char status;
   unsigned char str[MAX_LEN];
   unsigned char RC_size;
   unsigned char blockAddr; //选择操作的块地址0～63
  
   //找卡
   rfid.isCard();
   //读取卡序列号
   if (rfid.readCardSerial())
   {
  
     Serial.print(rfid.serNum[0],HEX);
     Serial.print(rfid.serNum[1],HEX);
     Serial.print(rfid.serNum[2],HEX);
     Serial.print(rfid.serNum[3],HEX);
     Serial.print(rfid.serNum[4],HEX);
     Serial.println(" ");
   }
  
   //选卡，返回卡容量（锁定卡片，防止多次读写）
   rfid.selectTag(rfid.serNum);
   
   //读卡
   blockAddr = 7; //数据块7
   status = rfid.auth(PICC_AUTHENT1A, blockAddr, sectorNewKeyA[blockAddr/4], rfid.serNum);
   if (status == MI_OK) //认证
   {
     //读数据
     blockAddr = blockAddr - 3 ; //数据块4
     if( rfid.read(blockAddr, str) == MI_OK)
     {
       //输出卡内内容
       Serial.print("Read from the card ,the data is : ");
       Serial.println((char *)str);
       //发送短信及取货密码
       SIM.init();//初始化SIM900A
       for(x=0;x<=10;x++)
       {
        tel[x]=str[x];//提取收件人电话号码
       }
       for(x=43;x<=46;x++)
       {
        sms[x]=random(1,8)+48;//产生随机密码
       }
       //暂存密码
       c[0]=sms[43];
       c[1]=sms[44];
       c[2]=sms[45];
       c[3]=sms[46];
       SIM.SendSMS(tel,sms);//发送短信
       y=random(1,4);//产生货柜号
       //存储货柜号
       for(x=0;x<=3;x++)
       {
         if(y==c[x])
         {
           y=random(1,4);
         }
         if(d[x]==0)
         {
           d[x]=y;
         }
       }
       //取载物台移动步数
       if(y<=2)
       {
         st1=50000;
         st2=y*50000;
       }
       else
       {
         st1=50000;
         st2=y/2*50000;
       }
       //移动载物台至指定货柜
       for(x=0;x<=st1;x++)
       {
         SM.StepY(0);
       }
       for(x=0;x<=st2;x++)
       {
         SM.StepZ(0);
       }
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
   rfid.halt();
}
