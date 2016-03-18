/*
 * CTBeta SIM900A Library
 * Created by Jason.C.T Mar.2016
 * Powered by CTBeta Team. http://www.ctbeta.org/
 * 整理者：Jason.C.T
 * 整理时间：2016.3.14
 */
#include <Arduino.h>
#include <CTB_SIM900A.h>

CTB_SIM900A::CTB_SIM900A()
{	
}

/******************************************************************************
 * 函 数 名：init
 * 功能描述：初始化函数库
 * 输入参数：无
 * 返 回 值：无
 ******************************************************************************/
void CTB_SIM900A::init()
{
	Serial.println("AT"); 
	delay(100);
 }
/******************************************************************************
 * 函 数 名：closeTCP
 * 功能描述：结束TCP连接
 * 输入参数：无
 * 返 回 值：无
 ******************************************************************************/
void CTB_SIM900A::closeTCP()
{
	Serial.print("AT+CIPCLOSE=1\r\n"); 
	delay(200);
	Serial.print("AT+CIPSHUT\r\n"); 
	delay(200);
 }
/******************************************************************************
 * 函 数 名：SendSMS
 * 功能描述：发送SMS短信
 * 输入参数：电话号码(char) 短信内容(char)
 * 返 回 值：无
 ******************************************************************************/
void CTB_SIM900A::SendSMS(char tel[],char sms[])
{
	Serial.println("AT+CMGF=1");
	delay(200);
	Serial.print("AT+CMGS=\"");
	for(int i=0;i<=10;i++)
	{
		Serial.print(tel[i]);
	}
	Serial.println("\"");
	delay(300);
	for(int i=0;i<=strlen(sms);i++)
	{
		Serial.print(sms[i]);
	}
	Serial.println();
	delay(200);
	Serial.println((char)26); //输入CRTL+Z或串口输出0X1A终止符
}
