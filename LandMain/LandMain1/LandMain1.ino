#include <SPI.h>
#include <RFID.h>

RFID rfid(10,5);    //D10--读卡器MOSI引脚、D5--读卡器RST引脚

//4字节卡序列号，第5字节为校验字节
unsigned char serNum[5];
//原扇区A密码，16个扇区，每个扇区密码6Byte
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
}

void loop()
{
  int x=0;
  unsigned char i,tmp;
  unsigned char status;
  unsigned char str[16];
  unsigned char RC_size;
  unsigned char blockAddr;        //选择操作的块地址0～63

  //找卡
  rfid.isCard();
  if (rfid.readCardSerial())
  {
   blockAddr = 7;
  }
  //选卡，返回卡容量（锁定卡片，防止多次读写）
  rfid.selectTag(rfid.serNum);

  //读卡
  blockAddr = 7;                //数据块7
  status = rfid.auth(PICC_AUTHENT1A, blockAddr, sectorNewKeyA[blockAddr/4], rfid.serNum);
  if (status == MI_OK)  //认证
  {
    //读数据
    blockAddr = blockAddr - 3 ; //数据块4
    if( rfid.read(blockAddr, str) == MI_OK)
    {
      for(x=0;x<=10;x++)
      {
        Serial.print(str[x]-48);
      }
    }
  }
  rfid.halt();
}
