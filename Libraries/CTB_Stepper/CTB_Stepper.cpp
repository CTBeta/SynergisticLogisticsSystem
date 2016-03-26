/*
 * CTBeta Stepper Library
 * Created by Jason.C.T Mar.2016
 * Powered by CTBeta Team. http://www.ctbeta.org/
 * 整理者：Jason.C.T
 * 整理时间：2016.3.14
 */
#include <Arduino.h>
#include <CTB_Stepper.h>

int SMPinY,SMPinZ,PinY_CW,PinZ_CW;

CTB_Stepper::CTB_Stepper()
{	
}

/******************************************************************************
 * 函 数 名：init
 * 功能描述：初始化函数库
 * 输入参数：SMPinY(int),SMPinZ(int),PinY_CW(int),PinZ_CW(int),SPEED(int,>=40)
 * 返 回 值：无
 ******************************************************************************/
void CTB_Stepper::init(int _SMPinY,int _SMPinZ,int _PinY_CW,int _PinZ_CW,int _SPEED)
{
	SMPinY=_SMPinY;
	SMPinZ=_SMPinZ;
	PinY_CW=_PinY_CW;
	PinZ_CW=_PinZ_CW;
	SPEED=_SPEED;
		
	pinMode(SMPinY,OUTPUT);
	pinMode(PinY_CW,OUTPUT);  
	pinMode(SMPinZ,OUTPUT);
	pinMode(PinZ_CW,OUTPUT);
	
	digitalWrite(SMPinY,LOW);
	digitalWrite(PinY_CW,LOW);  
	digitalWrite(SMPinZ,LOW);
	digitalWrite(PinZ_CW,LOW);

}

/******************************************************************************
 * 函 数 名：StepY
 * 功能描述：驱动Y轴1步
 * 输入参数：行进方向(int)("0"为反向，"1"为正向)
 * 返 回 值：无
 ******************************************************************************/
void CTB_Stepper::StepY(int dir) 
{
	switch(dir)
	{
		case 0:digitalWrite(PinY_CW,LOW); break;
		case 1:digitalWrite(PinY_CW,HIGH); 
	}
	Step(SMPinY);
}

/******************************************************************************
 * 函 数 名：StepZ
 * 功能描述：驱动Z轴1步
 * 输入参数：行进方向(int)("0"为反向，"1"为正向)
 * 返 回 值：无
 ******************************************************************************/
void CTB_Stepper::StepZ(int dir) 
{
	switch(dir)
	{
		case 0:digitalWrite(PinZ_CW,LOW); break;
		case 1:digitalWrite(PinZ_CW,HIGH);
	}
	Step(SMPinZ);
}

/******************************************************************************
 * 函 数 名：Step
 * 功能描述：驱动
 * 输入参数：无
 * 返 回 值：无
 ******************************************************************************/
void CTB_Stepper::Step(int CLK)
{
	digitalWrite(CLK, HIGH);
	delayMicroseconds(SPEED);
	digitalWrite(CLK, LOW);
	delayMicroseconds(SPEED);
}

