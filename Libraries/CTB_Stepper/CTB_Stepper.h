/*
 * CTBeta Stepper Library
 * Created by Jason.C.T Mar.2016
 * Powered by CTBeta Team. http://www.ctbeta.org/
 * 整理者：Jason.C.T
 * 整理时间：2016.3.14
 */
#ifndef  CTB_Stepper_H
#define CTB_Stepper_H

class CTB_Stepper

{

public:   

	CTB_Stepper();
	void init(int _SMPinY,int _SMPinZ,int _PinY_CW,int _PinZ_CW,int _SPEED);
//	void StepX();
	void StepY(int dir);
	void StepZ(int dir);
	void Step(int CLK);

	private:	
	int SMPinY,SMPinZ,PinY_CW,PinZ_CW;
	int SPEED;

}; 

#endif
