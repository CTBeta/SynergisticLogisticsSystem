/*
 * CTBeta SIM900A Library
 * Created by Jason.C.T Mar.2016
 * Powered by CTBeta Team. http://www.ctbeta.org/
 * 整理者：Jason.C.T
 * 整理时间：2016.3.14
 */
#ifndef  CTB_SIM900A_H
#define CTB_SIM900A_H

class CTB_SIM900A

{

public:   

	CTB_SIM900A();
	void init();
	void closeTCP();
	void SendSMS(char tel[],char sms[]);


}; 

#endif
