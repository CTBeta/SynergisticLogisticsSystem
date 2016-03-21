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
void CTB_DigitalInput::init(int pin[])
{
	for(int i=0;i<=7;i++)
	{
		pinMode(pin[i],INPUT);
	}
	
 }
