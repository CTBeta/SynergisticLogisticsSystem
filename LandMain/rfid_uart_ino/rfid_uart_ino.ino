unsigned char GetV[7]=    {0x40,0x00,0x00,0x00,0x00,0x00,0x0D};//获取模块版本（GetVersion）
unsigned char RfRest[8]=  {0x40,0x03,0x00,0x01,0x01,0x00,0x00,0x0D};//射频重启（RfReset）
unsigned char Request[8]= {0x40,0x04,0x00,0x01,0x01,0x00,0x00,0x0D};//查询（Request）
unsigned char Anticoll[9]={0x40,0x05,0x00,0x02,0x93,0x00,0x00,0x00,0x0D};//防冲突（Anticoll）
unsigned char Select[12]= {0x40,0x06,0x00,0x05,0x93,0x1C,0x4F,
                           0xBE,0xA7,0x00,0x00,0x0D};//选卡（Select）
unsigned char GetNum[8]=  {0x40,0x07,0x00,0x01,0x00,0x00,0x00,0x0D};//读取卡号（GetCardNum）
unsigned char ATTCT[15]=  {0x40,0x08,0x00,0x08,0x00,0x00,0xff,0xff,
                          0xff,0xff,0xff,0xff,0x00,0x00,0x0D};//认证（Authentication）
unsigned char read[8]=    {0x40,0x09,0x00,0x01,0x00,0x00,0x00,0x0D};//读块（ReadBlock）
unsigned char write[24]=  {0x40,0x10,0x00,0x11,0x02,0x11,0x34,0x56,
                          0x78,0x9A,0xCD,0xEF,0x00,0x11,0x34,0x56,
                          0x78,0x9A,0xCD,0xEF,0x11,0x00,0x00,0x0D};//写块（WriteBlock）
unsigned char m[2],CardID[10],data[24];
unsigned char a[30];
unsigned char n[5];

void setup()
{
  Serial1.begin(115200);
  Serial.begin(115200);
  delay(600);
  m[0]=0x08;
  m[1]=0x0D;
  n[0]=0x04;//
  n[1]=0x08;//选卡验证字节
}
void loop()
{
  int i=0;
  //查询
  Serial1.write(Request,8);
  for(i=0;i<=9;i++)
  {
    a[i]=Serial1.read();
  }
  if(a[5]==n[0])
  {
    Serial.println("The type of the card is S50.");
  }
   //选卡
  Serial1.write(Select,12);
  for(i=0;i<=8;i++)
  {a[i]=Serial1.read();}
  //选卡
  Serial1.write(ATTCT,15);
  for(i=0;i<=7;i++);
  {a[i]=Serial1.read();}
  if(a[4]==n[1])
  {
    //写卡
    Serial1.write(write,24);
    //读取卡号
    Serial1.write(GetNum,8);
    for(i=0;i<=15;i++)
    {
      a[i]=Serial1.read();
    }
    if(a[4]==m[0]) 
    {
      if (a[4]!=m[1]) 
      {
        for(i=0;i<10;i++)
        {
          CardID[i]=Serial1.read();
        }
        Serial.print("The card number is :");
        for(i=4;i<10;i++)
        {
          Serial.print(CardID[i],DEC);
        }
         Serial.println();
      }
    }
    //读块
    Serial.write(read,8);
    for(i=0;i<=23;i++)
    {
      a[i]=Serial1.read();
    }
    for(i=5;i<=20;i++)
    {
      data[i]=a[i];
    }
    Serial.println((char*)data);
  }
  else
  {
    Serial.println("Error!");
  }
}
