/*
 * CTBeta DigitalInput Library
 * Created by Jason.C.T Mar.2016
 * Powered by CTBeta Team. http://www.ctbeta.org/
 * 整理者：Jason.C.T
 * 整理时间：2016.3.14
 */
#include <Arduino.h>
#include <CTB_DigitalInput.h>

CTB_DigitalInput::CTB_DigitalInput()
{	
}

/******************************************************************************
 * 函 数 名：init
 * 功能描述：初始化函数库
 * 输入参数：无
 * 返 回 值：无
 ******************************************************************************/
void CTB_DigitalInput::init(int _pin[])
{
	for(int i=0;i<=7;i++)
	{
		pin[i]=_pin[i];
	}
	
	for(int i=0;i<=7;i++)
	{
		pinMode(pin[i],INPUT);
	}
 }
 
 /******************************************************************************
 * 函 数 名：get
 * 功能描述：读取数据
 * 输入参数：无
 * 返 回 值：读取到的数字（仅读一个）
 ******************************************************************************/

 int CTB_DigitalInput::get()
{
	int got=0;
	while(true)
	{
		for(int i=0;i<=7;i++)
		{
			if (digitalRead(pin[i])==1)
			{
				switch(i)
				{
					case 0:got=1;break;
					case 1:got=2;break;
					case 2:got=3;break;
					case 3:got=4;break;
					case 4:got=5;break;
					case 5:got=6;break;
					case 6:got=7;break;
					case 7:got=8;
				}
			}
		}
		if(got!=0)
		{
			break;
		}
		delay(10);
	}
	return got;
}
/******************************************************************************
 * 函 数 名：getword
 * 功能描述：读取数据流
 * 输入参数：数据长度(int)
 * 返 回 值：*got[]
 ******************************************************************************/

 /*
 int CTB_DigitalInput::getword(int num)
{
	int got[num];
	int find=1;
	while(true)
	{
		int tmp;
		for(int i=0;i<=7;i++)
		{
			if (digitalRead(pin[i])==1)
			{
				switch(i)
				{
					case 0:got[find]=1;break;
					case 1:got[find]=2;break;
					case 2:got[find]=3;break;
					case 3:got[find]=4;break;
					case 4:got[find]=5;break;
					case 5:got[find]=6;break;
					case 6:got[find]=7;break;
					case 7:got[find]=8;
				}
				find++;
			}
		}
		if(find==num)
		{
			break;
		}	
	}
	return got;
}*/

