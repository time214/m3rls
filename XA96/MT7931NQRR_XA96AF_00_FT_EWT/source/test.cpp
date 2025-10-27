/* ********************************************************************************************************************
888b     d888 88888888888 8888888888  .d8888b.   .d8888b.   d888   888b    888  .d88888b.  8888888b.  8888888b.  
8888b   d8888     888           d88P d88P  Y88b d88P  Y88b d8888   8888b   888 d88P" "Y88b 888   Y88b 888   Y88b 
88888b.d88888     888          d88P  888    888      .d88P   888   88888b  888 888     888 888    888 888    888 
888Y88888P888     888         d88P   Y88b. d888     8888"    888   888Y88b 888 888     888 888   d88P 888   d88P 
888 Y888P 888     888      88888888   "Y888P888      "Y8b.   888   888 Y88b888 888     888 8888888P"  8888888P"  
888  Y8P  888     888       d88P            888 888    888   888   888  Y88888 888 Y8b 888 888 T88b   888 T88b   
888   "   888     888      d88P      Y88b  d88P Y88b  d88P   888   888   Y8888 Y88b.Y8b88P 888  T88b  888  T88b  
888       888     888     d88P        "Y8888P"   "Y8888P"  8888888 888    Y888  "Y888888"  888   T88b 888   T88b 
   ******************************************************************************************************************** */
/*
MT7931:
--- test7
1, rSBU1_0V:                  TE-0.23ohm;
2, rSBU1_3V6:                 TE-0.2ohm;
3, rSBU2_0V:                  TE+0.42ohm;
4, rSBU2_3V6:                 TE+0.28ohm;
--- test17
5, TOVP_FLT_ASSERTION:        TE-7.5us;
--- test19
6, TOVP_Recovery_CC1_FET:     TE+0.08ms;
7, TOVP_Recovery_CC1_DB:      TE-0.09ms;
8, TOVP_Recovery_CC2_FET:     TE+0.08ms;
--- test20
9, TOVP_Recovery_SBU1:        TE-48us;
10, TOVP_Recovery_SBU2:       TE-82us;
*/
#include "stdafx.h"
#include "math.h"
#define SITENUM	2
#define DEBUG_OFF 0		// 0:disable relay off 1:enable

BYTE site = 0;
short i = 0, jj = 0;
short flag[SITENUM] = { 0, 0 };
double iCL[SITENUM] = { 0.0 }, Tmpv = 0.0f;

BYTE sitesta[SITENUM] = { 0 };
// ******************************************************************* VI SOURCE DEFINITION *******************************************************************
// FPVI
FPVI10 pviVPWR		(0,  "VPWR");
FPVI10 pviC_CC1		(1,  "C_CC1");
FPVI10 pviC_SBU1	(3,  "C_SBU1");

FPVI10 pviCC1			(2,  "CC1");
FPVI10 pviSBU1		(2,  "SBU1");
// FOVI
FOVI oviVBIAS			(0,  "VBIAS");
FOVI oviRPD1			(1,  "RPD1");
FOVI oviFLTB			(2,  "FLTB");
FOVI oviSBU2			(3,  "SBU2");
FOVI oviRPD2			(4,  "RPD2");
FOVI oviC_CC2			(5,  "C_CC2");
FOVI oviC_SBU2		(8,  "C_SBU2");
FOVI oviVBUS			(9,  "VBUS");
FOVI oviV33E			(10, "V33E");
FOVI oviSBU1			(11, "SBU1");
FOVI oviCC2				(12, "CC2");
// FOVI 6, 7, 13, 14, 15 free 
// OTHER
QTMU_PLUS qtmu0		(0);
CBIT128 rlyC;

DUT_API void HardWareCfg()	{
	/*For example: four channels dvi to config two sites*/
	/*StsSetModuleToSite(MD_DVI400, SITE_1, 0, 1, -1);
		StsSetModuleToSite(MD_DVI400, SITE_2, 2, 3, -1);*/

	// FPVI
	StsSetModuleToSite(MD_FPVI10, SITE_1, 0, 1, 2, 3, -1);	// SITE1, IDC1,2
	StsSetModuleToSite(MD_FPVI10, SITE_2, 4, 5, 6, 7, -1);	// SITE2, IDC3,4 
	// FOVI
	StsSetModuleToSite(MD_FOVI, SITE_1,  0,  1,  2,	 3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, -1);
	StsSetModuleToSite(MD_FOVI, SITE_2, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, -1);
	// QTMU
	StsSetModuleToSite(MD_QTMUPLUS, SITE_1, 0, 1 , -1);
	StsSetModuleToSite(MD_QTMUPLUS, SITE_2, 2, 3 , -1);
}

void pwrSW_off(void)	{
#ifdef DEBUG_OFF
	pviVPWR.Set		(FV,	float(0), FPVI10_5V, FPVI10_10MA, RELAY_OFF);
	pviC_CC1.Set	(FV,	float(0), FPVI10_5V, FPVI10_10MA, RELAY_OFF);
	pviCC1.Set		(FV,	float(0), FPVI10_5V, FPVI10_10MA, RELAY_OFF);
	pviC_SBU1.Set	(FV,	float(0), FPVI10_5V, FPVI10_10MA, RELAY_OFF);
	
	oviVBIAS.Set	(FV,  float(0), FOVI_5V,	FOVI_10MA,		RELAY_OFF);
	oviRPD1.Set		(FV,  float(0), FOVI_5V,	FOVI_10MA,		RELAY_OFF);
	oviFLTB.Set		(FV,  float(0), FOVI_5V,	FOVI_10MA,		RELAY_OFF);
	oviSBU2.Set		(FV,  float(0), FOVI_5V,	FOVI_10MA,		RELAY_OFF);
	oviRPD2.Set		(FV,  float(0), FOVI_5V,	FOVI_10MA,		RELAY_OFF);
	oviC_CC2.Set	(FV,  float(0), FOVI_5V,	FOVI_10MA,		RELAY_OFF);
	oviC_SBU2.Set	(FV,  float(0), FOVI_5V,	FOVI_10MA,		RELAY_OFF);
	oviVBUS.Set		(FV,  float(0), FOVI_5V,	FOVI_10MA,		RELAY_OFF);
	oviV33E.Set		(FV,  float(0), FOVI_5V,	FOVI_10MA,		RELAY_OFF);
	oviSBU1.Set		(FV,  float(0), FOVI_5V,	FOVI_10MA,		RELAY_OFF);
	oviCC2.Set		(FV,  float(0), FOVI_5V,	FOVI_10MA,		RELAY_OFF);
	delay_ms(1);
#endif
}
void pwrZero(void)	{
	pviVPWR.Set		(FV,	float(0), FPVI10_5V, FPVI10_10MA, RELAY_ON);
	pviC_CC1.Set	(FV,	float(0), FPVI10_5V, FPVI10_10MA, RELAY_ON);
	pviCC1.Set		(FV,	float(0), FPVI10_5V, FPVI10_10MA, RELAY_ON);
	pviC_SBU1.Set	(FV,	float(0), FPVI10_5V, FPVI10_10MA, RELAY_ON);

	oviVBIAS.Set	(FV,	float(0), FOVI_5V,	 FOVI_10MA,		RELAY_ON);
	oviRPD1.Set		(FV,	float(0), FOVI_5V,	 FOVI_10MA,		RELAY_ON);
	oviFLTB.Set		(FV,	float(0), FOVI_5V,	 FOVI_10MA,		RELAY_ON);
	oviSBU2.Set		(FV,	float(0), FOVI_5V,	 FOVI_10MA,		RELAY_ON);
	oviRPD2.Set		(FV,	float(0), FOVI_5V,	 FOVI_10MA,		RELAY_ON);
	oviC_CC2.Set	(FV,	float(0), FOVI_5V,	 FOVI_10MA,		RELAY_ON);
	oviC_SBU2.Set	(FV,	float(0), FOVI_5V,	 FOVI_10MA,		RELAY_ON);
	oviVBUS.Set		(FV,	float(0), FOVI_5V,	 FOVI_10MA,		RELAY_ON);
	oviV33E.Set		(FV,	float(0), FOVI_5V,	 FOVI_10MA,		RELAY_ON);
	oviSBU1.Set		(FV,	float(0), FOVI_5V,	 FOVI_10MA,		RELAY_ON);
	oviCC2.Set		(FV,	float(0), FOVI_5V,	 FOVI_10MA,		RELAY_ON);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//initialize function will be called before all the test functions.
DUT_API void InitBeforeTestFlow()	{
	qtmu0.Init();
	rlyC.Init();
	rlyC.SetOn(-1);
	delay_ms(1);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//initializefunction will be called after all the test functions.
DUT_API void InitAfterTestFlow()	{		
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//Fail site hardware set function will be called after failed params, it can be called for serveral times. 
DUT_API void SetupFailSite(const unsigned char*byFailSite)	{			
}
// test0
// ******************************************************************* Continuity *******************************************************************
// ******************************************************************* Continuity *******************************************************************
// ******************************************************************* Continuity *******************************************************************
DUT_API int OS_TEST(short funcindex, LPCTSTR funclabel)	{
  //{{AFX_STS_PARAM_PROTOTYPES
    CParam *C_CC1I_GND = StsGetParam(funcindex,"C_CC1I_GND");
    CParam *C_CC2I_GND = StsGetParam(funcindex,"C_CC2I_GND");
    CParam *C_SBU1I_GND = StsGetParam(funcindex,"C_SBU1I_GND");
    CParam *C_SBU2I_GND = StsGetParam(funcindex,"C_SBU2I_GND");
    CParam *VBUS_GND = StsGetParam(funcindex,"VBUS_GND");
    CParam *VPWR_GND = StsGetParam(funcindex,"VPWR_GND");
    CParam *V33E_GND = StsGetParam(funcindex,"V33E_GND");
    CParam *SBU2_GND = StsGetParam(funcindex,"SBU2_GND");
    CParam *SBU1_GND = StsGetParam(funcindex,"SBU1_GND");
    CParam *CC2_GND = StsGetParam(funcindex,"CC2_GND");
    CParam *CC1_GND = StsGetParam(funcindex,"CC1_GND");
    CParam *FLT_GND = StsGetParam(funcindex,"FLT_GND");
    CParam *VBIAS_GND = StsGetParam(funcindex,"VBIAS_GND");
    CParam *RPD1 = StsGetParam(funcindex,"RPD1");
    CParam *RPD2 = StsGetParam(funcindex,"RPD2");
  //}}AFX_STS_PARAM_PROTOTYPES

  // TODO: Add your function code here
	rlyC.SetOn(C_SBU1_Fpvi, C_SBU2_Fovi, VBIAS_Fovi, C_CC1_Fpvi, C_CC2_Fovi, RPD2_Fovi, RPD1_Fovi, FLTB_Fovi, VPWR_Fpvi, CC2_Fovi, CC1_Fpvi, SBU2_Fovi, SBU1_Fpvi, VBUS_Fovi, V33E_Fovi, -1);
	pwrZero();
	delay_ms(1);
	
	// C_SUB1(1)
	pviC_SBU1.Set(FI, -1.0e-3, FPVI10_2V, FPVI10_10MA, RELAY_ON);
	delay_us(500);
	pviC_SBU1.MeasureVI(10, 10);
	for (site = 0; site < SITENUM; site++)
		C_SBU1I_GND->SetTestResult(site, 0, pviC_SBU1.GetMeasResult(site, MVRET));
	pviC_SBU1.Set(FV, 0.0f, FPVI10_2V, FPVI10_10MA, RELAY_ON);

	// C_SUB2(2)
	oviC_SBU2.Set(FI, -1.0e-3, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_us(500);
	oviC_SBU2.MeasureVI(10, 10);
	for (site = 0; site < SITENUM; site++)
		C_SBU2I_GND->SetTestResult(site, 0, oviC_SBU2.GetMeasResult(site, MVRET));
	oviC_SBU2.Set(FV, 0.0, FOVI_2V, FOVI_10MA, RELAY_ON);

	// VBIAS(3)
	oviVBIAS.Set(FV, -2.0, FOVI_2V, FOVI_1MA, RELAY_ON);
	delay_us(2000);//500
	oviVBIAS.MeasureVI(10, 10);
	for (site = 0; site < SITENUM; site++)
		VBIAS_GND->SetTestResult(site, 0, oviVBIAS.GetMeasResult(site, MVRET));
	oviVBIAS.Set(FV, 0.0, FOVI_2V, FOVI_10MA, RELAY_ON);

	// C_CC1(4)
	pviC_CC1.Set(FI, -1.0e-3, FPVI10_2V, FPVI10_10MA, RELAY_ON);
	delay_us(500);
	pviC_CC1.MeasureVI(10, 10);
	for (site = 0; site < SITENUM; site++)
		C_CC1I_GND->SetTestResult(site, 0, pviC_CC1.GetMeasResult(site, MVRET));
	pviC_CC1.Set(FV, 0.0f, FPVI10_2V, FPVI10_10MA, RELAY_ON);

	// C_CC2(5)
	oviC_CC2.Set(FI, -1.0e-3, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_us(500);
	oviC_CC2.MeasureVI(10, 10);
	for (site = 0; site < SITENUM; site++)
		C_CC2I_GND->SetTestResult(site, 0, oviC_CC2.GetMeasResult(site, MVRET));
	oviC_CC2.Set(FV, 0.0, FOVI_2V, FOVI_10MA, RELAY_ON);

	// RPD2(6)
	oviRPD2.Set(FI, -0.5e-3, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_us(500);
	oviRPD2.MeasureVI(10, 10);
	for (site = 0; site < SITENUM; site++)
		RPD2->SetTestResult(site, 0, oviRPD2.GetMeasResult(site, MVRET));
	oviRPD2.Set(FV, 0.0, FOVI_2V, FOVI_10MA, RELAY_ON);

	// RPD1(7)
	oviRPD1.Set(FI, -0.5e-3, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_us(500);
	oviRPD1.MeasureVI(10, 10);
	for (site = 0; site < SITENUM; site++)
		RPD1->SetTestResult(site, 0, oviRPD1.GetMeasResult(site, MVRET));
	oviRPD1.Set(FV, 0.0, FOVI_2V, FOVI_10MA, RELAY_ON);

	// FLTB(9)
	oviFLTB.Set(FI, -1.0e-3, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(2);//1
	oviFLTB.MeasureVI(10, 10);
	for (site = 0; site < SITENUM; site++)
		FLT_GND->SetTestResult(site, 0, oviFLTB.GetMeasResult(site, MVRET));
	oviFLTB.Set(FV, 0.0, FOVI_2V, FOVI_10MA, RELAY_ON);

	// VPWR(10)
	pviVPWR.Set(FV, -2.0f, FPVI10_2V, FPVI10_10MA, RELAY_ON);
	delay_us(500);
	pviVPWR.MeasureVI(10, 10);
	for(site=0; site < SITENUM; site++)
		VPWR_GND->SetTestResult(site, 0, pviVPWR.GetMeasResult(site, MVRET));
	pviVPWR.Set(FV, 0.0f, FPVI10_2V, FPVI10_10MA, RELAY_ON);

	// CC2(11)
	oviCC2.Set(FI, -0.5e-3, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_us(500);
	oviCC2.MeasureVI(10, 10);
	for (site = 0; site < SITENUM; site++)
		CC2_GND->SetTestResult(site, 0, oviCC2.GetMeasResult(site, MVRET));
	oviCC2.Set(FV, 0.0, FOVI_2V, FOVI_10MA, RELAY_ON);

	// CC1(12)
	pviCC1.Set(FI, -1.0e-3, FPVI10_2V, FPVI10_10MA, RELAY_ON);
	delay_us(500);
	pviCC1.MeasureVI(10, 10);
	for (site = 0; site < SITENUM; site++)
		CC1_GND->SetTestResult(site, 0, pviCC1.GetMeasResult(site, MVRET));
	pviCC1.Set(FV, 0.0f, FPVI10_2V, FPVI10_10MA, RELAY_ON);

	// SBU2
	oviSBU2.Set(FI, -0.5e-3, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_us(500);
	oviSBU2.MeasureVI(10, 10);
	for (site = 0; site < SITENUM; site++)
		SBU2_GND->SetTestResult(site, 0, oviSBU2.GetMeasResult(site, MVRET));
	oviSBU2.Set(FV, 0.0, FOVI_2V, FOVI_10MA, RELAY_ON);

	// SBU1(15)
	pviSBU1.Set(FI, -1.0e-3, FPVI10_2V, FPVI10_10MA, RELAY_ON);
	delay_us(500);
	pviSBU1.MeasureVI(10, 10);
	for (site = 0; site < SITENUM; site++)	
		SBU1_GND->SetTestResult(site, 0, pviSBU1.GetMeasResult(site, MVRET));
	pviSBU1.Set(FV, 0.0f, FPVI10_2V, FPVI10_10MA, RELAY_ON);

	// VBUS(16)
	oviVBUS.Set(FI, -1.0e-3, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_us(500);
	oviVBUS.MeasureVI(10, 10);
	for (site = 0; site < SITENUM; site++)
		VBUS_GND->SetTestResult(site, 0, oviVBUS.GetMeasResult(site, MVRET));
	oviVBUS.Set(FV, 0.0, FOVI_2V, FOVI_10MA, RELAY_ON);

	// V33E(17)
	oviV33E.Set(FI, -1.0e-3, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_us(500);
	oviV33E.MeasureVI(10, 10);
	for(site=0; site < SITENUM; site++)
		V33E_GND->SetTestResult(site, 0, oviV33E.GetMeasResult(site, MVRET));
	oviV33E.Set(FV, 0.0, FOVI_2V, FOVI_10MA, RELAY_ON);

	pwrSW_off();
	rlyC.SetOn(-1);
	delay_ms(1);
	
	return 0;
}
// test1
// ******************************************************************* CC Pin OVP MOSFET *******************************************************************
// ******************************************************************* CC Pin OVP MOSFET *******************************************************************
// ******************************************************************* CC Pin OVP MOSFET *******************************************************************
DUT_API int CC_OVP_R(short funcindex, LPCTSTR funclabel)	{
  //{{AFX_STS_PARAM_PROTOTYPES
    CParam *CC_OVP_Ron_CC1O_5V = StsGetParam(funcindex,"CC_OVP_Ron_CC1O_5V");
    CParam *CC_OVP_Ron_CC1O_0V = StsGetParam(funcindex,"CC_OVP_Ron_CC1O_0V");
    CParam *CC_OVP_Ron_CC1O_1V2 = StsGetParam(funcindex,"CC_OVP_Ron_CC1O_1V2");
    CParam *Ron_FLAT = StsGetParam(funcindex,"Ron_FLAT");
    CParam *CC_OVP_Ron_CC1O_5V_2 = StsGetParam(funcindex,"CC_OVP_Ron_CC1O_5V_2");
    CParam *CC_OVP_Ron_CC1O_0V_2 = StsGetParam(funcindex,"CC_OVP_Ron_CC1O_0V_2");
    CParam *CC_OVP_Ron_CC1O_1V2_2 = StsGetParam(funcindex,"CC_OVP_Ron_CC1O_1V2_2");
    CParam *Ron_FLAT_2 = StsGetParam(funcindex,"Ron_FLAT_2");
  //}}AFX_STS_PARAM_PROTOTYPES

  // TODO: Add your function code here
	double Ron11[SITENUM] = { 0.0f }, Ron12[SITENUM] = { 0.0f }, Ron13[SITENUM] = { 0.0f };
	double Ron21[SITENUM] = { 0.0f }, Ron22[SITENUM] = { 0.0f }, Ron23[SITENUM] = { 0.0f };
  double vCCUSBC[SITENUM] = { 0.0f }, vCCSYS[SITENUM] = {0.0f };
	double deltaV[SITENUM] = { 0.0f }, iLoading[SITENUM] = { 0.0f };
	// ****************** CC2 MOSFET ON RESISTANCE 5V 1-5 ************************************************************************
	// ****************** CC2 MOSFET ON RESISTANCE 5V 1-5 ************************************************************************
	rlyC.SetOn(VPWR_Cap, V33E_Cap, C_CC2_Fpvi, CC2_Fpvi, V33E_Fovi, RPD1_Fovi, RPD2_Fovi, FLTB_100K, VPWR_Fpvi, -1);
	delay_ms(1);
	// RPD1, RPD2=0
	oviRPD1.Set(FV, 0.0f, FOVI_2V, FOVI_10MA, RELAY_ON);
	oviRPD2.Set(FV, 0.0f, FOVI_2V, FOVI_10MA, RELAY_ON);
	// V33E=3.3
	oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_100MA, RELAY_ON);
  delay_ms(1);

	// CC1=5
	pviCC1.Set(FV, 5.0f, FPVI10_10V, FPVI10_1A, RELAY_ON);
  delay_ms(1);
	// C_CC1 100mA loading
	pviC_CC1.Set(FI, -100.0e-3f, FPVI10_10V, FPVI10_1A, RELAY_ON);
  delay_ms(1);

	pviCC1.MeasureVI(200, 20);
	pviC_CC1.MeasureVI(100, 20);
  for(site = 0; site < SITENUM; site++)	{
		vCCUSBC[site] = pviC_CC1.GetMeasResult(site, MVRET);
    vCCSYS[site] = pviCC1.GetMeasResult(site, MVRET);
		iLoading[site] = pviC_CC1.GetMeasResult(site, MIRET);
    deltaV[site] = vCCUSBC[site] - vCCSYS[site];
    Ron21[site]= deltaV[site] / iLoading[site] * 1.0e3f;
		// LOG DATA
  	CC_OVP_Ron_CC1O_5V_2->SetTestResult(site, 0, Ron21[site]);			 
 	}
	// C_CC1 w/o loading
	pviC_CC1.Set(FI, -1.0e-6f, FPVI10_10V, FPVI10_1A, RELAY_ON);
  delay_ms(1);
	// ****************** CC2 MOSFET ON RESISTANCE 1.2V 1-6 ******************
	// ****************** CC2 MOSFET ON RESISTANCE 1.2V 1-6 ******************
	pviCC1.Set(FV, 1.2f, FPVI10_10V, FPVI10_1A, RELAY_ON);
  delay_ms(1);
	pviC_CC1.Set(FI, -100.0e-3f, FPVI10_10V, FPVI10_1A, RELAY_ON);
  delay_ms(10);

	pviCC1.MeasureVI(1000, 5);
	pviC_CC1.MeasureVI(1000, 5);
  for(site = 0; site < SITENUM; site++)	{
		vCCUSBC[site] = pviC_CC1.GetMeasResult(site, MVRET);
    vCCSYS[site] = pviCC1.GetMeasResult(site, MVRET);
		iLoading[site] = pviC_CC1.GetMeasResult(site, MIRET);
    deltaV[site] = vCCUSBC[site] - vCCSYS[site];
    Ron22[site]= deltaV[site] / iLoading[site] * 1.0e3f;
		// LOG DATA
  	CC_OVP_Ron_CC1O_1V2_2->SetTestResult(site, 0, Ron22[site]);	
 	}
	// C_CC2 w/o loading
	pviC_CC1.Set(FI, -1.0e-6f, FPVI10_10V, FPVI10_1A, RELAY_ON);
  delay_ms(1);
	// ****************** CC2 MOSFET ON RESISTANCE 0.3V 1-7, 1-8 ******************
	// ****************** CC2 MOSFET ON RESISTANCE 0.3V 1-7, 1-8 ******************
	pviCC1.Set(FV, 0.3f, FPVI10_10V, FPVI10_1A, RELAY_ON);
  delay_ms(1);
	pviC_CC1.Set(FI, -100.0e-3f, FPVI10_10V, FPVI10_1A, RELAY_ON);
  delay_ms(10);

	pviCC1.MeasureVI(1000, 5);
	pviC_CC1.MeasureVI(1000, 5);
  for(site = 0; site < SITENUM; site++)	{
		vCCUSBC[site] = pviC_CC1.GetMeasResult(site, MVRET);
    vCCSYS[site] = pviCC1.GetMeasResult(site, MVRET);
		iLoading[site] = pviC_CC1.GetMeasResult(site, MIRET);
    deltaV[site] = vCCUSBC[site] - vCCSYS[site];
    Ron23[site]= deltaV[site] / iLoading[site] * 1.0e3f;
		// LOG DATA
  	CC_OVP_Ron_CC1O_0V_2->SetTestResult(site, 0, Ron23[site]);	
  	Ron_FLAT_2->SetTestResult(site, 0, fabs(Ron22[site]-Ron23[site]));	
 	}
	// C_CC2 w/o loading
	pviC_CC1.Set(FI, 0.0e-12f, FPVI10_10V, FPVI10_1A, RELAY_ON);
  delay_us(100);
	pviCC1.Set(FV, 0.0f, FPVI10_10V, FPVI10_1A, RELAY_ON);
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);

	pviC_CC1.Set(FI, 0.0e-12f, FPVI10_10V, FPVI10_1A, RELAY_OFF);
	pviCC1.Set(FV, 0.0f, FPVI10_10V, FPVI10_1A, RELAY_OFF);
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_OFF);
	delay_us(10);
	rlyC.SetOn(-1);

	// ****************** CC1 MOSFET ON RESISTANCE 5V 1-1 ************************************************************************
	// ****************** CC1 MOSFET ON RESISTANCE 5V 1-1 ************************************************************************
	rlyC.SetOn(VPWR_Cap, V33E_Cap, C_CC1_Fpvi, CC1_Fpvi, V33E_Fovi, RPD1_Fovi, RPD2_Fovi, FLTB_100K, VPWR_Fpvi, -1);
	delay_ms(1);
	// RPD1, RPD2=0
	oviRPD1.Set(FV, 0.0f, FOVI_2V, FOVI_10MA, RELAY_ON);
	oviRPD2.Set(FV, 0.0f, FOVI_2V, FOVI_10MA, RELAY_ON);
	// V33E=3.3
	oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_100MA, RELAY_ON);
  delay_ms(1);

	// CC1=5
	pviCC1.Set(FV, 5.0f, FPVI10_10V, FPVI10_1A, RELAY_ON);
  delay_ms(1);
	// C_CC1 100mA loading
	pviC_CC1.Set(FI, -100.0e-3f, FPVI10_10V, FPVI10_1A, RELAY_ON);
  delay_ms(1);

	pviCC1.MeasureVI(200, 20);
	pviC_CC1.MeasureVI(100, 20);
  for(site = 0; site < SITENUM; site++)	{
		vCCUSBC[site] = pviC_CC1.GetMeasResult(site, MVRET);
    vCCSYS[site] = pviCC1.GetMeasResult(site, MVRET);

		vCCUSBC[site] += 0.010f;

		iLoading[site] = pviC_CC1.GetMeasResult(site, MIRET);
    deltaV[site] = vCCUSBC[site] - vCCSYS[site];
    Ron11[site]= deltaV[site] / iLoading[site] * 1.0e3f;
		Ron11[site] += float(0);
		// LOG DATA
  	CC_OVP_Ron_CC1O_5V->SetTestResult(site, 0, Ron11[site]);			 
 	}
	// C_CC1 w/o loading
	pviC_CC1.Set(FI, -1.0e-6f, FPVI10_10V, FPVI10_1A, RELAY_ON);
  delay_ms(1);
	// ****************** CC1 MOSFET ON RESISTANCE 1.2V 1-2 ******************
	// ****************** CC1 MOSFET ON RESISTANCE 1.2V 1-2 ******************
	pviCC1.Set(FV, 1.2f, FPVI10_10V, FPVI10_1A, RELAY_ON);
  delay_ms(1);
	pviC_CC1.Set(FI, -100.0e-3f, FPVI10_10V, FPVI10_1A, RELAY_ON);
  delay_ms(10);

	pviCC1.MeasureVI(1000, 5);
	pviC_CC1.MeasureVI(1000, 5);
  for(site = 0; site < SITENUM; site++)	{
		vCCUSBC[site] = pviC_CC1.GetMeasResult(site, MVRET);
    vCCSYS[site] = pviCC1.GetMeasResult(site, MVRET);

		vCCUSBC[site] += 0.010f;

		iLoading[site] = pviC_CC1.GetMeasResult(site, MIRET);
    deltaV[site] = vCCUSBC[site] - vCCSYS[site];
    Ron12[site]= deltaV[site] / iLoading[site] * 1.0e3f;
		Ron12[site] += float(-0);
		// LOG DATA
  	CC_OVP_Ron_CC1O_1V2->SetTestResult(site, 0, Ron12[site]);	
 	}
	// C_CC2 w/o loading
	pviC_CC1.Set(FI, -1.0e-6f, FPVI10_10V, FPVI10_1A, RELAY_ON);
  delay_ms(1);
	// ****************** CC1 MOSFET ON RESISTANCE 0.3V 1-3, 1-4 ******************
	// ****************** CC1 MOSFET ON RESISTANCE 0.3V 1-3, 1-4 ******************
	pviCC1.Set(FV, 0.3f, FPVI10_10V, FPVI10_1A, RELAY_ON);
  delay_ms(1);
	pviC_CC1.Set(FI, -100.0e-3f, FPVI10_10V, FPVI10_1A, RELAY_ON);
  delay_ms(10);

	pviCC1.MeasureVI(1000, 5);
	pviC_CC1.MeasureVI(1000, 5);
  for(site = 0; site < SITENUM; site++)	{
		vCCUSBC[site] = pviC_CC1.GetMeasResult(site, MVRET);
    vCCSYS[site] = pviCC1.GetMeasResult(site, MVRET);

		vCCUSBC[site] += 0.010f;

		iLoading[site] = pviC_CC1.GetMeasResult(site, MIRET);
    deltaV[site] = vCCUSBC[site] - vCCSYS[site];
    Ron13[site]= deltaV[site] / iLoading[site] * 1.0e3f;
		Ron13[site] += float(-0);
		// LOG DATA
  	CC_OVP_Ron_CC1O_0V->SetTestResult(site, 0, Ron13[site]);	
  	Ron_FLAT->SetTestResult(site, 0, fabs(Ron12[site]-Ron13[site]));	
 	}
	// C_CC2 w/o loading
	pviC_CC1.Set(FI, 0.0e-12f, FPVI10_10V, FPVI10_1A, RELAY_ON);
  delay_us(100);
	pviCC1.Set(FV, 0.0f, FPVI10_10V, FPVI10_1A, RELAY_ON);
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);

	pviC_CC1.Set(FI, 0.0e-12f, FPVI10_10V, FPVI10_1A, RELAY_OFF);
	pviCC1.Set(FV, 0.0f, FPVI10_10V, FPVI10_1A, RELAY_OFF);
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_OFF);
	delay_us(10);
	rlyC.SetOn(-1);

  return 0;
}
// test2
// ******************************************************************* DeadBattery pull down resistance *******************************************************************
// ******************************************************************* DeadBattery pull down resistance *******************************************************************
// ******************************************************************* DeadBattery pull down resistance *******************************************************************
DUT_API int Effective_Resistance(short funcindex, LPCTSTR funclabel)	{
	//{{AFX_STS_PARAM_PROTOTYPES
    CParam *RD = StsGetParam(funcindex,"RD");
    CParam *RD2 = StsGetParam(funcindex,"RD2");
    CParam *iC_CC1 = StsGetParam(funcindex,"iC_CC1");
    CParam *iC_CC2 = StsGetParam(funcindex,"iC_CC2");
  //}}AFX_STS_PARAM_PROTOTYPES
  // TODO: Add your function code here
	double resRWN1[SITENUM] = { 0.0f }, resRWN2[SITENUM] = { 0.0f };
  double iCC_C[SITENUM] = { 0.0f }, vCC_C[SITENUM] = { 0.0f };

	// V33E, VBUS, C_CC1
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	oviVBUS.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	pviC_CC1.Set(FV, 0.0f, FPVI10_10V, FPVI10_10MA, RELAY_ON);
	oviC_CC2.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	rlyC.SetOn(FLTB_100K, VPWR_Cap, V33E_Cap, C_CC1_Fpvi, C_CC2_Fovi, V33E_Fovi, VBUS_Fovi, VPWR_Fpvi, RPD1_Fovi, RPD2_Fovi, -1);
	delay_ms(1);
	// C_CC1, RPD1 set 2.6
	pviC_CC1.Set(FV, 2.6f, FPVI10_10V, FPVI10_10MA, RELAY_ON);
	oviRPD1.Set(FV, 2.6f, FOVI_10V, FOVI_10MA, RELAY_ON);
	// C_CC2, RPD2 set 2.6
	oviC_CC2.Set(FV, 2.6f, FOVI_10V, FOVI_10MA, RELAY_ON);
	oviRPD2.Set(FV, 2.6f, FOVI_10V, FOVI_10MA, RELAY_ON);
  delay_ms(10);

	pviC_CC1.MeasureVI(200, 10);
	for(site = 0; site < SITENUM; site++)	{
		vCC_C[site] = pviC_CC1.GetMeasResult(site, MVRET);
		iCC_C[site] = pviC_CC1.GetMeasResult(site, MIRET);
		resRWN1[site] = vCC_C[site] / iCC_C[site] * 1e-3f;
		iC_CC1->SetTestResult(site, 0, iCC_C[site]*1e6f);
 	}
	oviC_CC2.MeasureVI(200, 10);
	for(site = 0; site < SITENUM; site++)	{
		vCC_C[site] = oviC_CC2.GetMeasResult(site, MVRET);
		iCC_C[site] = oviC_CC2.GetMeasResult(site, MIRET);
		resRWN2[site] = vCC_C[site] / iCC_C[site] * 1e-3f;
		iC_CC2->SetTestResult(site, 0, iCC_C[site]*1e6f);
 	}
	
	for(site = 0; site < SITENUM; site++)	{
		// LOG DATA
		RD	->SetTestResult(site, 0, resRWN1[site]);
		RD2	->SetTestResult(site, 0, resRWN2[site]);
	}
	pwrZero();
	pwrSW_off();
	delay_ms(1);
	rlyC.SetOn(-1);

  return 0;
}
// test3
// ******************************************************************* VTHDB *******************************************************************
// ******************************************************************* VTHDB *******************************************************************
// ******************************************************************* VTHDB *******************************************************************
DUT_API int DB_Threshold(short funcindex, LPCTSTR funclabel)	{
	//{{AFX_STS_PARAM_PROTOTYPES
    CParam *VTH_DB = StsGetParam(funcindex,"VTH_DB");
    CParam *VTH_DB2 = StsGetParam(funcindex,"VTH_DB2");
  //}}AFX_STS_PARAM_PROTOTYPES

  // TODO: Add your function code here
  BYTE sitesta[SITENUM] = { 0 };
	short flag[SITENUM] = {0};
	double VTHDB_V[SITENUM] = { 0.0f };
  double iCC_C[SITENUM] = { 0.0f };
	double iJudge[SITENUM] = { 80.0f, 80e-6f };	// 80uA

	// V33E, VBUS, C_CC1, RPD1
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	oviVBUS.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	pviC_CC1.Set(FV, 0.0f, FPVI10_2V, FPVI10_10MA, RELAY_ON);
	oviRPD1.Set(FV, 0.0f, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	rlyC.SetOn(FLTB_100K, VPWR_Cap, V33E_Cap, C_CC1_Fpvi, C_CC2_Fovi, V33E_Fovi, VBUS_Fovi, VPWR_Fpvi, RPD1_Fovi, RPD2_Fovi, -1);
	delay_ms(1);
	// C_CC1, RPD1
	pviC_CC1.Set(FV, 500e-3f, FPVI10_2V, FPVI10_10MA, RELAY_ON);
	oviRPD1.Set(FV, 500e-3f, FOVI_2V, FOVI_10MA, RELAY_ON);
	// C_CC2, RPD2
	oviC_CC2.Set(FV, 500e-3f, FOVI_2V, FOVI_10MA, RELAY_ON);
	oviRPD2.Set(FV, 500e-3f, FOVI_2V, FOVI_10MA, RELAY_ON);
  delay_ms(1);
	
	// C_CC1, RPD1
	pviC_CC1.Set(FV, 600E-3f, FPVI10_2V, FPVI10_10MA, RELAY_ON);
	oviRPD1.Set(FV, 600e-3f, FOVI_2V, FOVI_10MA, RELAY_ON);
	// C_CC2, RPD2
	oviC_CC2.Set(FV, 600e-3f, FOVI_2V, FOVI_10MA, RELAY_ON);
	oviRPD2.Set(FV, 600e-3f, FOVI_2V, FOVI_10MA, RELAY_ON);
  delay_us(100);

	// C_CC1, RPD1
	pviC_CC1.Set(FV, 650e-3f, FPVI10_2V, FPVI10_10MA, RELAY_ON);
	oviRPD1.Set(FV, 650e-3f, FOVI_2V, FOVI_10MA, RELAY_ON);
	// C_CC2, RPD2
	oviC_CC2.Set(FV, 650e-3f, FOVI_2V, FOVI_10MA, RELAY_ON);
	oviRPD2.Set(FV, 650e-3f, FOVI_2V, FOVI_10MA, RELAY_ON);
  delay_us(100);
  for(site = 0; site < SITENUM; site++)
		flag[site] = 1;

  StsGetSiteStatus(sitesta, SITENUM);
 	for(site = 0; site < SITENUM; site++)	{
  	if(sitesta[site])	
 			flag[site] = 0;
		else
			flag[site] = 1;
	}
	// ************************************ VTH_DB1 ************************************ 3-1
	// ************************************ VTH_DB1 ************************************ 3-1
	for(Tmpv = 650.0f; Tmpv <= 1300.0f;)	{
		pviC_CC1.Set(FV, Tmpv * 1.0e-3f, FPVI10_2V, FPVI10_10MA, RELAY_ON);
		oviRPD1.Set(FV, Tmpv * 1.0e-3f, FOVI_2V, FOVI_10MA, RELAY_ON);
  	delay_us(100);

	  pviC_CC1.MeasureVI(10, 10);
  	for(site = 0; site < SITENUM; site++)	{
  		iCC_C[site] = fabs(pviC_CC1.GetMeasResult(site, MIRET));
 			if((flag[site] == 0) && (iCC_C[site]*1.0e6f >= iJudge[site]))	{
				VTHDB_V[site] = pviC_CC1.GetMeasResult(site, MVRET);
				VTHDB_V[site] *= 1e3f;
				flag[site] = 1;
			}
  		Tmpv += 1.0f;
  	}
		if((flag[0] == 1) && (flag[1] == 1))
			break;
	}
	for(site = 0; site < SITENUM; site++)
		VTH_DB->SetTestResult(site, 0, VTHDB_V[site]);			 

	// ************************************ VTH_DB2 ************************************ 3-2
	// ************************************ VTH_DB2 ************************************ 3-2
  StsGetSiteStatus(sitesta, SITENUM);
 	for(site = 0; site < SITENUM; site++)	{
		VTHDB_V[site] = float(0);
  	if(sitesta[site])	
 			flag[site] = 0;
		else
			flag[site] = 1;
	}

	for(Tmpv = 650.0f; Tmpv <= 1300.0f;)	{
		oviC_CC2.Set(FV, Tmpv * 1.0e-3f, FOVI_2V, FOVI_10MA, RELAY_ON);
		oviRPD2.Set(FV, Tmpv * 1.0e-3f, FOVI_2V, FOVI_10MA, RELAY_ON);
  	delay_us(100);

	  oviC_CC2.MeasureVI(10, 10);
  	for(site = 0; site < SITENUM; site++)	{
  		iCC_C[site] = fabs(oviC_CC2.GetMeasResult(site, MIRET));
 			if((flag[site] == 0) && (iCC_C[site]*1.0e6f >= iJudge[site]))	{
				VTHDB_V[site] = oviC_CC2.GetMeasResult(site, MVRET);
				VTHDB_V[site] *= 1e3f;
				flag[site] = 1;
			}
  		Tmpv += 1.0f;
  	}
		if((flag[0] == 1) && (flag[1] == 1))
			break;
	}
	for(site = 0; site < SITENUM; site++)
		VTH_DB2->SetTestResult(site, 0, VTHDB_V[site]);			 

	pwrZero();
	pwrSW_off();
	rlyC.SetOn(-1);

  return 0;
}
// test4
// ******************************************************************* VOVP *******************************************************************
// ******************************************************************* VOVP *******************************************************************
// ******************************************************************* VOVP *******************************************************************
DUT_API int CC_OVP_Threshold(short funcindex, LPCTSTR funclabel)	{
	//{{AFX_STS_PARAM_PROTOTYPES
    CParam *OVPCC_Rising = StsGetParam(funcindex,"OVPCC_Rising");
    CParam *OVPCC_Falling = StsGetParam(funcindex,"OVPCC_Falling");
    CParam *OVPCC_VHYS = StsGetParam(funcindex,"OVPCC_VHYS");
    CParam *OVPCC_Rising2 = StsGetParam(funcindex,"OVPCC_Rising2");
    CParam *OVPCC_Falling2 = StsGetParam(funcindex,"OVPCC_Falling2");
    CParam *OVPCC_VHYS2 = StsGetParam(funcindex,"OVPCC_VHYS2");
  //}}AFX_STS_PARAM_PROTOTYPES

  // TODO: Add your function code here
  BYTE sitesta[SITENUM] = { 0 };
	short flag[SITENUM] = { 0 };
	double VOVP1_H[SITENUM] = { 0.0f }, VOVP1_L[SITENUM] = { 0.0f }, VOVP1hys[SITENUM] = { 0.0f }, vCC[SITENUM] = { 0.0f };
	double VOVP2_H[SITENUM] = { 0.0f }, VOVP2_L[SITENUM] = { 0.0f }, VOVP2hys[SITENUM] = { 0.0f };
	double vJudgement = 3.0f;

	// V33E, CC1, C_CC1, FLTB
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	pviCC1.Set(FI, 0.0e-12f, FPVI10_10V, FPVI10_10UA, RELAY_ON);
	oviCC2.Set(FI, 0.0e-12f, FOVI_10V, FOVI_10UA, RELAY_ON);
	pviC_CC1.Set(FV, 0.0f, FPVI10_10V, FPVI10_10MA, RELAY_ON);
	oviFLTB.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	rlyC.SetOn(VPWR_Cap, V33E_Cap, C_CC1_Fpvi, C_CC2_Fovi, V33E_Fovi, CC1_Fpvi, CC2_Fovi, FLTB_Fovi, -1);
	delay_ms(1);
	oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(2);
	oviFLTB.Set(FV, -1.0f, FOVI_5V, FOVI_10MA, RELAY_ON);
	delay_ms(6);

	oviFLTB.Set(FV, 3.3f, FOVI_5V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	// C_CC1, C_CC22 5.3V
	pviC_CC1.Set(FV, 5.3f, FPVI10_10V, FPVI10_10MA, RELAY_ON);
	oviC_CC2.Set(FV, 5.3f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(3);

  for(site = 0; site < SITENUM; site++)
		flag[site] = 1;
  StsGetSiteStatus(sitesta, SITENUM);
	for(site = 0; site < SITENUM; site++)	{
  	if(sitesta[site])	
 			flag[site] = 0;	
		else
			flag[site] = 1;
	}
	// ************************************ OVP1 RISING ************************************ 4-1
	// ************************************ OVP1 RISING ************************************ 4-1
  for(Tmpv = 5.3f; Tmpv < 6.55f;)	{
	  pviC_CC1.Set(FV, Tmpv, FPVI10_10V, FPVI10_10MA, RELAY_ON);
  	delay_us(100);

	  pviCC1.MeasureVI(10, 10);
  	for(site = 0; site< SITENUM; site++)	{
  		vCC[site] = fabs(pviCC1.GetMeasResult(site, MVRET));
 			if( (flag[site] == 0) && (vCC[site] < vJudgement) )	{
				pviC_CC1.MeasureVI(10, 10);
				VOVP1_H[site] = pviC_CC1.GetMeasResult(site, MVRET);
				VOVP1_H[site] += float(0e-3);
  			flag[site] = 1;
  		}
  	}
  	if( (flag[0] == 1) && (flag[1] == 1) )
			break;
  	Tmpv += 0.005f;
  }	
	// ************************************ OVP1 FALLING ************************************ 4-2, 4-3
	// ************************************ OVP1 FALLING ************************************ 4-2, 4-3
	StsGetSiteStatus(sitesta, SITENUM);
	for(site = 0; site < SITENUM; site++)	{
  	if(sitesta[site])	
 			flag[site] = 0;	
		else
			flag[site] = 1;
	}
  for(Tmpv = 6.35f; Tmpv > 5.5f;)	{
	  pviC_CC1.Set(FV, Tmpv, FPVI10_10V, FPVI10_10MA, RELAY_ON);
  	delay_us(100);

	  pviCC1.MeasureVI(10, 10);
  	for(site = 0; site< SITENUM; site++)	{
  		vCC[site] = fabs(pviCC1.GetMeasResult(site, MVRET));
 			if( (flag[site] == 0) && (vCC[site] > vJudgement) )	{
				pviC_CC1.MeasureVI(10, 10);
				VOVP1_L[site] = pviC_CC1.GetMeasResult(site, MVRET);
				VOVP1_L[site] += float(0e-3);
  			flag[site] = 1;
  		}
  	}
  	if( (flag[0] == 1) && (flag[1] == 1) )
			break;
  	Tmpv += -0.005f;
  }	

	// ************************************ OVP2 RISING ************************************ 4-4
	// ************************************ OVP2 RISING ************************************ 4-4
	StsGetSiteStatus(sitesta, SITENUM);
	for(site = 0; site < SITENUM; site++)	{
  	if(sitesta[site])	
 			flag[site] = 0;	
		else
			flag[site] = 1;
	}

  for(Tmpv = 5.3f; Tmpv < 6.55f;)	{
	  oviC_CC2.Set(FV, Tmpv, FOVI_10V, FOVI_10MA, RELAY_ON);
  	delay_us(100);

	  oviCC2.MeasureVI(10, 10);
  	for(site = 0; site< SITENUM; site++)	{
  		vCC[site] = fabs(oviCC2.GetMeasResult(site, MVRET));
 			if( (flag[site] == 0) && (vCC[site] < vJudgement) )	{
				oviC_CC2.MeasureVI(10, 10);
				VOVP2_H[site] = oviC_CC2.GetMeasResult(site, MVRET);
				VOVP2_H[site] += float(0e-3);
  			flag[site] = 1;
  		}
  	}
  	if( (flag[0] == 1) && (flag[1] == 1) )
			break;
  	Tmpv += 0.005f;
  }	
	// ************************************ OVP2 FALLING ************************************ 4-5, 4-6
	// ************************************ OVP2 FALLING ************************************ 4-5, 4-6
	StsGetSiteStatus(sitesta, SITENUM);
	for(site = 0; site < SITENUM; site++)	{
  	if(sitesta[site])	
 			flag[site] = 0;	
		else
			flag[site] = 1;
	}
  for(Tmpv = 6.35f; Tmpv > 5.5f;)	{
	  oviC_CC2.Set(FV, Tmpv, FOVI_10V, FOVI_10MA, RELAY_ON, 1);
  	delay_us(100);

	  oviCC2.MeasureVI(10, 10);
  	for(site = 0; site< SITENUM; site++)	{
  		vCC[site] = fabs(oviCC2.GetMeasResult(site, MVRET));
 			if( (flag[site] == 0) && (vCC[site] > vJudgement) )	{
				oviC_CC2.MeasureVI(10, 10);
				VOVP2_L[site] = oviC_CC2.GetMeasResult(site, MVRET);
				VOVP2_L[site] += float(0e-3);
  			flag[site] = 1;
  		}
  	}
  	if( (flag[0] == 1) && (flag[1] == 1) )
			break;
  	Tmpv += -0.005f;
  }	

	for(site = 0; site < SITENUM; site++)	{
 		OVPCC_Rising	->SetTestResult(site, 0, VOVP1_H[site]);
		OVPCC_Falling	->SetTestResult(site, 0, VOVP1_L[site]);
		OVPCC_VHYS		->SetTestResult(site, 0, fabs(VOVP1_L[site] - VOVP1_H[site]) * 1e3);
		OVPCC_Rising2	->SetTestResult(site, 0, VOVP2_H[site]);
		OVPCC_Falling2->SetTestResult(site, 0, VOVP2_L[site]);
		OVPCC_VHYS2		->SetTestResult(site, 0, fabs(VOVP2_L[site] - VOVP2_H[site]) * 1e3);
	}
	pwrZero();
	pwrSW_off();
	rlyC.SetOn(-1);
	delay_ms(1);

	return 0;
}
// test5
// ******************************************************************* CCx clamping voltage *******************************************************************
// ******************************************************************* CCx clamping voltage *******************************************************************
// ******************************************************************* CCx clamping voltage *******************************************************************
DUT_API int CC_ShortClamp(short funcindex, LPCTSTR funclabel)	{
	//{{AFX_STS_PARAM_PROTOTYPES
    CParam *VST_VBUS_CC_ClampV = StsGetParam(funcindex,"VST_VBUS_CC_ClampV");
    CParam *VST_VBUS_CC_ClampV2 = StsGetParam(funcindex,"VST_VBUS_CC_ClampV2");
  //}}AFX_STS_PARAM_PROTOTYPES

  // TODO: Add your function code here
	// ****************** CC1 short clamping voltage ****************** 5-1
	// ****************** CC1 short clamping voltage ****************** 5-1
	short loopt = 20;
  double CC1_Clamp_V[SITENUM] = { 0.0f }, CC2_Clamp_V[SITENUM] = { 0.0f };
	double vtemp[SITENUM] = { 0.0f };

	// V33E, C_CC1, CC1
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	pviC_CC1.Set(FV, 0.0f, FPVI10_50V, FPVI10_1A, RELAY_ON);
	pviCC1.Set(FI, 0.0e-12f, FPVI10_50V, FPVI10_10UA, RELAY_SENSE_ON);
	delay_ms(1);

	for(site = 0; site < SITENUM; site++)
		flag[site] = 1;
  StsGetSiteStatus(sitesta, SITENUM);
	for(site = 0; site < SITENUM; site++)	{
  	if(sitesta[site])	flag[site] = 0;	
		else	flag[site] = 1;
	}

	rlyC.SetOn(VPWR_Cap, V33E_Cap, C_CC1_Fpvi, CC1_Fpvi, V33E_Fovi, FLTB_100K, VPWR_Fpvi, RPD1_Fovi, RPD2_Fovi, -1);
	delay_ms(1);
	// RPD1, RPD2
	oviRPD1.Set(FV, 0.0f, FOVI_2V, FOVI_100MA, RELAY_ON);
	oviRPD2.Set(FV, 0.0f, FOVI_2V, FOVI_100MA, RELAY_ON);
	delay_ms(1);
	oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_100MA, RELAY_ON);
  delay_ms(5);

	for(jj = 0; jj < int(loopt); jj++)	{
		for(site = 0; site < SITENUM; site++)	{
			pviC_CC1.Set(FV, 36.0f, FPVI10_50V, FPVI10_1A, RELAY_ON);
			pviCC1.MeasureVI(20, 2);
			CC1_Clamp_V[site] = pviCC1.GetMeasResult(site, MVRET, MAX_RESULT);
			if (CC1_Clamp_V[site] > vtemp[site])
				vtemp[site] = CC1_Clamp_V[site];
			pviC_CC1.Set(FV, 0.0f, FPVI10_50V, FPVI10_1A, RELAY_ON);
			delay_ms(5);
 		}
	}
	pviC_CC1.Set(FV, 0.0f, FPVI10_50V, FPVI10_1A, RELAY_ON);
  delay_ms(1);

	for(site = 0; site < SITENUM; site++)	{
		CC1_Clamp_V[site] = vtemp[site];
		VST_VBUS_CC_ClampV->SetTestResult(site, 0, CC1_Clamp_V[site]);
	}
//#ifdef DEBUG
	// ****************** CC2 short clamping voltage ****************** 5-2
	// ****************** CC2 short clamping voltage ****************** 5-2
	// V33E, C_CC2, CC2
	pviCC1.Set(FI, 0.0e-12f, FPVI10_50V, FPVI10_10UA, RELAY_SENSE_ON);
	delay_ms(1);

	for(site = 0; site < SITENUM; site++)	{
		vtemp[site] = 0;
		flag[site] = 1;
	}
  StsGetSiteStatus(sitesta, SITENUM);
	for(site = 0; site < SITENUM; site++)	{
  	if(sitesta[site])	flag[site] = 0;	
		else	flag[site] = 1;
	}

	rlyC.SetOn(VPWR_Cap, V33E_Cap, C_CC2_Fpvi, CC2_Fpvi, V33E_Fovi, FLTB_100K, VPWR_Fpvi, RPD1_Fovi, RPD2_Fovi, -1);
	delay_ms(1);
	// RPD1, RPD2
	oviRPD1.Set(FV, 0.0f, FOVI_2V, FOVI_100MA, RELAY_ON);
	oviRPD2.Set(FV, 0.0f, FOVI_2V, FOVI_100MA, RELAY_ON);
	delay_ms(1);
	oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_100MA, RELAY_ON);
  delay_ms(5);

	for(jj = 0; jj < int(loopt); jj++)	{
		for(site = 0; site < SITENUM; site++)	{
			pviC_CC1.Set(FV, 36.0f, FPVI10_50V, FPVI10_1A, RELAY_ON);
			pviCC1.MeasureVI(20, 2);
			CC2_Clamp_V[site] = pviCC1.GetMeasResult(site, MVRET, MAX_RESULT);
			if (CC2_Clamp_V[site] > vtemp[site])
				vtemp[site] = CC2_Clamp_V[site];
			pviC_CC1.Set(FV, 0.0f, FPVI10_50V, FPVI10_1A, RELAY_ON);
			delay_ms(5);
 		}
	}
	pviC_CC1.Set(FV, 0.0f, FPVI10_50V, FPVI10_1A, RELAY_ON);
  delay_ms(1);

	for(site = 0; site < SITENUM; site++)	{
		CC2_Clamp_V[site] = vtemp[site];
		VST_VBUS_CC_ClampV2->SetTestResult(site, 0, CC2_Clamp_V[site]);
	}
//#endif
	pwrZero();
	pwrSW_off();
	rlyC.SetOn(-1);
	delay_ms(1);
	
	return 0;
}
// test6
// ******************************************************************* C_CC Current Limit *******************************************************************
// ******************************************************************* C_CC Current Limit *******************************************************************
// ******************************************************************* C_CC Current Limit *******************************************************************
DUT_API int CCx_current_limit(short funcindex, LPCTSTR funclabel)	{
	//{{AFX_STS_PARAM_PROTOTYPES
  CParam *CCX_CurrentLimit = StsGetParam(funcindex,"CCX_CurrentLimit");
  CParam *CCX_CurrentLimit2 = StsGetParam(funcindex,"CCX_CurrentLimit2");
  //}}AFX_STS_PARAM_PROTOTYPES

	double vPWR[SITENUM] = { 0.0f };
	double CCT[SITENUM] = { 0.0f }, iload_st = 700.0, iload_end = 1700.0;

	for(site = 0; site < SITENUM; site++)
		flag[site] = 1;
  StsGetSiteStatus(sitesta, SITENUM);
	for(site = 0; site < SITENUM; site++)	{
  	if(sitesta[site])	flag[site] = 0;	
		else	flag[site] = 1;
	}

	// VPWR, V33E, RPD1, RPD2
	pviVPWR.Set(FI, 0.0e-12f, FPVI10_10V, FPVI10_100MA, RELAY_ON);
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	oviRPD1.Set(FV, 0.0f, FOVI_2V, FOVI_10MA, RELAY_ON);
	oviRPD2.Set(FV, 0.0f, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	// ******************************* C_CC1 Current Limit ******************************* 6-1
	// ******************************* C_CC1 Current Limit ******************************* 6-1
 	rlyC.SetOn(VPWR_Cap, V33E_Cap, V33E_Fovi, VPWR_Fpvi, RPD1_Fovi, C_CC1_Fpvi, CC1_Fpvi, FLTB_100K, -1);

	delay_ms(1);
	oviRPD1.Set(FV, 0.0f, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_100MA, RELAY_ON);
	pviCC1.Set(FV, 5.0f, FPVI10_10V, FPVI10_1A, RELAY_ON);
	delay_ms(3);

  for(Tmpv = iload_st; Tmpv < iload_end;)	{
		pviC_CC1.Set(FI, Tmpv*-1.0e-3f, FPVI10_10V, FPVI10_10A, RELAY_ON);
  	delay_us(100);

	  pviC_CC1.MeasureVI(10, 10);
  	for(site = 0; site < SITENUM; site++)	{
			vPWR[site] = pviC_CC1.GetMeasResult(site, MVRET);

  		if((flag[site] == 0) && (vPWR[site] <=1.0f))	{
				iCL[site] = Tmpv ;
  			flag[site] = 1;
 			}
 		}
 		if((flag[0] == 1) && (flag[1] == 1))
			break;
 		Tmpv += 5.0f;
 	}	

  for(Tmpv = 1000.0f; Tmpv >= 0.0f; Tmpv += -50.0f)
		pviC_CC1.Set(FI, Tmpv*-1.0e-3f, FPVI10_10V, FPVI10_10A, RELAY_ON);
	delay_us(100);

	pviC_CC1.Set(FI, 0.0e-12f, FPVI10_10V, FPVI10_10A, RELAY_ON);
	delay_us(100);
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_us(100);
  
	for(site = 0; site < SITENUM; site++)	{
	  CCX_CurrentLimit->SetTestResult(site, 0, iCL[site]);
		iCL[site] = 0;
	}

//#ifdef DEBUG
	for(site = 0; site < SITENUM; site++)
		flag[site] = 1;
  StsGetSiteStatus(sitesta, SITENUM);
	for(site = 0; site < SITENUM; site++)	{
  	if(sitesta[site])	flag[site] = 0;	
		else	flag[site] = 1;
	}
	// ******************************* C_CC2 Current Limit ******************************* 6-2, 6-3
	// ******************************* C_CC2 Current Limit ******************************* 6-2, 6-3
 	rlyC.SetOn(VPWR_Cap, V33E_Cap, V33E_Fovi, VPWR_Fpvi, RPD2_Fovi, C_CC2_Fpvi, CC2_Fpvi, FLTB_100K, -1);

	delay_ms(1);
	oviRPD2.Set(FV, 0.0f, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_100MA, RELAY_ON);
	pviCC1.Set(FV, 5.0f, FPVI10_10V, FPVI10_1A, RELAY_ON);
	delay_ms(3);
	// source loading
	pviC_CC1.Set(FI, 700e-3f, FPVI10_10V, FPVI10_10A, RELAY_ON);
  delay_ms(10);

  for(Tmpv = iload_st; Tmpv < iload_end;)	{
		pviC_CC1.Set(FI, Tmpv*-1.0e-3f, FPVI10_10V, FPVI10_10A, RELAY_ON);
  	delay_us(100);

	  pviC_CC1.MeasureVI(10, 10);
  	for(site = 0; site < SITENUM; site++)	{
			vPWR[site] = pviC_CC1.GetMeasResult(site, MVRET);

  		if((flag[site] == 0) && (vPWR[site] <=1.0f))	{
				iCL[site] = Tmpv ;
  			flag[site] = 1;
 			}
 		}
 		if((flag[0] == 1) && (flag[1] == 1))
			break;
 		Tmpv += 5.0f;
 	}	
//#endif

  for(Tmpv = 1000.0f; Tmpv >= 0.0f; Tmpv += -50.0f)
		pviC_CC1.Set(FI, Tmpv*-1.0e-3f, FPVI10_10V, FPVI10_10A, RELAY_ON);
	delay_us(100);

	pviC_CC1.Set(FI, 0.0e-12f, FPVI10_10V, FPVI10_10A, RELAY_ON);
	delay_us(100);
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_us(100);
  
	for(site = 0; site < SITENUM; site++)	{
	  CCX_CurrentLimit2->SetTestResult(site, 0, iCL[site]);
		iCL[site] = 0;
	}

	pwrZero();
	pwrSW_off();
	rlyC.SetOn(-1);
	delay_ms(1);

  // TODO: Add your function code here
  return 0;
}
// test7
// ******************************************************************* SBU OVP Resistance *******************************************************************
// ******************************************************************* SBU OVP Resistance *******************************************************************
DUT_API int VBUS_OVP_Mosfet(short funcindex, LPCTSTR funclabel)	{
	//{{AFX_STS_PARAM_PROTOTYPES
    CParam *SBUS_Ron_0V = StsGetParam(funcindex,"SBUS_Ron_0V");
    CParam *SBUS_Ron_3V6 = StsGetParam(funcindex,"SBUS_Ron_3V6");
    CParam *SBUS_Ron_FALT = StsGetParam(funcindex,"SBUS_Ron_FALT");
    CParam *SBUS_Ron_0V2 = StsGetParam(funcindex,"SBUS_Ron_0V2");
    CParam *SBUS_Ron_3V62 = StsGetParam(funcindex,"SBUS_Ron_3V62");
    CParam *SBUS_Ron_FALT2 = StsGetParam(funcindex,"SBUS_Ron_FALT2");
  //}}AFX_STS_PARAM_PROTOTYPES

  // TODO: Add your function code here
	for(site = 0; site < SITENUM; site++)
		flag[site] = 1;
  StsGetSiteStatus(sitesta, SITENUM);
	for(site = 0; site < SITENUM; site++)	{
  	if(sitesta[site])	flag[site] = 0;	
		else	flag[site] = 1;
	}

	// ****************** SBU1 Ron1 and On resistance flatness ****************** 7-1
	// ****************** SBU1 Ron1 and On resistance flatness ****************** 7-1
	double Ron1[SITENUM] = { 0.0f }, Ron1_0V[SITENUM] = { 0.0f }, Ron1_3V6[SITENUM] = { 0.0f };
	double Ron2[SITENUM] = { 0.0f }, Ron2_0V[SITENUM] = { 0.0f }, Ron2_3V6[SITENUM] = { 0.0f };
	double vSBU1out[SITENUM] = {0.0f}, vSBU1in[SITENUM] = { 0.0f };
	double vSBU2out[SITENUM] = {0.0f}, vSBU2in[SITENUM] = { 0.0f };
	double deltaV[SITENUM] = { 0.0f }, iLoading = 200e-3f, iload[SITENUM] = { 9999, 9999 };

	// V33E, C_SUB1, SUB1
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	pviSBU1.Set(FV, 0.0f, FPVI10_10V, FPVI10_1A, RELAY_ON);
	pviC_SBU1.Set(FI, 0.0e-12f, FPVI10_10V, FPVI10_1A, RELAY_ON);
	delay_ms(1);

	rlyC.SetOn(VPWR_Cap, V33E_Cap, C_SBU1_Fpvi, SBU1_Fpvi, V33E_Fovi, RPD1_Fovi, RPD2_Fovi, VPWR_Fpvi, -1);
	delay_ms(1);
	// RPD1, RPD2
	oviRPD1.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	oviRPD2.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	// V33E
	oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_100MA, RELAY_ON);
  delay_ms(1);

	// ************************************ SBU1 3.6V, C_SBU1 source 100mA ************************************ 7-2
	// ************************************ SBU1 3.6V, C_SBU1 source 100mA ************************************ 7-2
	pviSBU1.Set(FV, 3.6f, FPVI10_10V, FPVI10_1A, RELAY_ON);
  delay_ms(1);

	pviC_SBU1.Set(FI, -iLoading, FPVI10_10V, FPVI10_1A, RELAY_ON);
  delay_ms(1);

	pviSBU1.MeasureVI(100, 10);
	pviC_SBU1.MeasureVI(100, 10);
  for(site = 0; site < SITENUM; site++)	{
		vSBU1out[site] = pviC_SBU1.GetMeasResult(site, MVRET);
    vSBU1in[site] = pviSBU1.GetMeasResult(site, MVRET);
		iload[site] = pviSBU1.GetMeasResult(site, MIRET);
    deltaV[site] = vSBU1in[site] - vSBU1out[site];
		Ron1_3V6[site] = deltaV[site] / 0.2f;
		Ron1_3V6[site] += -0.2f;  // offset
		SBUS_Ron_3V6->SetTestResult(site, 0, Ron1_3V6[site]);			 
 	}
	// ************************************ SBU1 0.4V, C_SBU1 source 100mA ************************************ 7-3
	// ************************************ SBU1 0.4V, C_SBU1 source 100mA ************************************ 7-3
	pviSBU1.Set(FV, 0.0f/*0.4f*/, FPVI10_10V, FPVI10_1A, RELAY_ON);
  delay_ms(10);

	pviSBU1.MeasureVI(1000, 10);
	pviC_SBU1.MeasureVI(1000, 10);
  for(site = 0; site < SITENUM; site++)	{
		vSBU1out[site] = pviC_SBU1.GetMeasResult(site, MVRET);
    vSBU1in[site] = pviSBU1.GetMeasResult(site, MVRET);
		iload[site] = pviSBU1.GetMeasResult(site, MIRET);
    deltaV[site] = vSBU1in[site] - vSBU1out[site];
    //Ron1_0V[site]= deltaV[site] / iload[site];//iLoading;
		Ron1_0V[site] = deltaV[site] / 0.2f;
		Ron1_0V[site] += -0.23f;  // offset
  	SBUS_Ron_0V		->SetTestResult(site, 0, Ron1_0V[site]);	
  	SBUS_Ron_FALT	->SetTestResult(site, 0, fabs(Ron1_3V6[site]-Ron1_0V[site]));	
 	}

	// ****************** SBU2 Ron2 and On resistance flatness ****************** 7-4
	// ****************** SBU2 Ron2 and On resistance flatness ****************** 7-4
	// V33E, C_SUB2, SUB2
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_1A, RELAY_ON);
	oviSBU2.Set(FV, 0.0f, FOVI_10V, FOVI_1A, RELAY_ON);
	oviC_SBU2.Set(FI, 0.0e-12f, FOVI_10V, FOVI_1A, RELAY_ON);
	delay_ms(1);

	rlyC.SetOn(VPWR_Cap, V33E_Cap, C_SBU2_Fovi, SBU2_Fovi, V33E_Fovi, RPD1_Fovi, RPD2_Fovi, VPWR_Fpvi, -1);
	delay_ms(1);
	oviRPD1.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	oviRPD2.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(1);
	oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_1A, RELAY_ON);
  delay_ms(1);

	// ************************************ SBU2 3.6V, C_SBU2 source 100mA ************************************ 7-5
	// ************************************ SBU2 3.6V, C_SBU2 source 100mA ************************************ 7-5
	oviSBU2.Set(FV, 3.6f, FOVI_5V, FOVI_1A, RELAY_ON);
  delay_ms(1);
	oviC_SBU2.Set(FI, -iLoading, FOVI_10V, FOVI_1A, RELAY_ON);
  delay_ms(1);

	oviSBU2.MeasureVI(100, 10);
	oviC_SBU2.MeasureVI(100, 10);
  for(site = 0; site < SITENUM; site++)	{
		vSBU2out[site] = oviC_SBU2.GetMeasResult(site, MVRET);
    vSBU2in[site] = oviSBU2.GetMeasResult(site, MVRET);
		iload[site] = oviSBU2.GetMeasResult(site, MIRET);
    deltaV[site] = vSBU2in[site] - vSBU2out[site];
    //Ron2_3V6[site]= deltaV[site] / iload[site];//iLoading;
		Ron2_3V6[site] = deltaV[site] / 0.2f;
		Ron2_3V6[site] += 0.23f; // offset
 	}
	// ************************************ SBU2 0.4V, C_SBU2 source 100mA ************************************ 7-6
	// ************************************ SBU2 0.4V, C_SBU2 source 100mA ************************************ 7-6
	oviSBU2.Set(FV, 0.0f/*0.4f*/, FOVI_5V, FOVI_1A, RELAY_ON);
  delay_ms(10);

	oviSBU2.MeasureVI(1000, 10);
	oviC_SBU2.MeasureVI(1000, 10);
  for(site = 0; site < SITENUM; site++)	{
		vSBU2out[site] = oviC_SBU2.GetMeasResult(site, MVRET);
    vSBU2in[site] = oviSBU2.GetMeasResult(site, MVRET);
		iload[site] = oviSBU2.GetMeasResult(site, MIRET);
    deltaV[site] = vSBU2in[site] - vSBU2out[site];
    //Ron2_0V[site]= deltaV[site] / iload[site];//iLoading;
		Ron2_0V[site] = deltaV[site] / 0.2f;
		Ron2_0V[site] += -0.1f;  // offset
  	SBUS_Ron_FALT2	->SetTestResult(site, 0, fabs(Ron2_3V6[site]-Ron2_0V[site]));	

/*		SBUS_Ron_3V62	->SetTestResult(site, 0, Ron2_3V6[site]);
		SBUS_Ron_0V2	->SetTestResult(site, 0, Ron2_0V[site]);*/	
		SBUS_Ron_3V62	->SetTestResult(site, 0, Ron2_0V[site]);
		SBUS_Ron_0V2	->SetTestResult(site, 0, Ron2_3V6[site]);
 	}
	oviC_SBU2.Set(FI, 0, FOVI_10V, FOVI_1A, RELAY_ON);
  delay_ms(1);

	pwrZero();
	pwrSW_off();
	rlyC.SetOn(-1);
	delay_ms(1);

  return 0;
}

// test8
// ******************************************************************* SBU OVP Threshold *******************************************************************
// ******************************************************************* SBU OVP Threshold *******************************************************************
// ******************************************************************* SBU OVP Threshold *******************************************************************
DUT_API int SBU_OVP_Threshold(short funcindex, LPCTSTR funclabel)	{
	//{{AFX_STS_PARAM_PROTOTYPES
    CParam *OVPSBU_Rising = StsGetParam(funcindex,"OVPSBU_Rising");
    CParam *OVPSBU_Falling = StsGetParam(funcindex,"OVPSBU_Falling");
    CParam *OVPSBU_VHYS = StsGetParam(funcindex,"OVPSBU_VHYS");
    CParam *OVPSBU_Rising2 = StsGetParam(funcindex,"OVPSBU_Rising2");
    CParam *OVPSBU_Falling2 = StsGetParam(funcindex,"OVPSBU_Falling2");
    CParam *OVPSBU_VHYS2 = StsGetParam(funcindex,"OVPSBU_VHYS2");
  //}}AFX_STS_PARAM_PROTOTYPES

  // TODO: Add your function code here
	double vSBU1out[SITENUM] = { 0.0f }, vSBU1_OVP_H[SITENUM] = { 0.0f }, vSBU1_OVP_L[SITENUM] = { 0.0f }, vSBU1_OVP_HYS[SITENUM] = { 0.0f };
	double vSBU2out[SITENUM] = { 0.0f }, vSBU2_OVP_H[SITENUM] = { 0.0f }, vSBU2_OVP_L[SITENUM] = { 0.0f }, vSBU2_OVP_HYS[SITENUM] = { 0.0f };

	// V33E
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	pviSBU1.Set(FI, 0.0e-12f, FPVI10_10V, FPVI10_10UA, RELAY_ON);
	pviC_SBU1.Set(FV, 0.0f, FPVI10_10V, FPVI10_10MA, RELAY_ON);
	oviSBU2.Set(FI, 0.0e-12f, FOVI_10V, FOVI_10UA, RELAY_ON);
	oviC_SBU2.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	oviFLTB.Set(FV, 0.0f, FOVI_5V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	rlyC.SetOn(V33E_Cap, C_SBU1_Fpvi, C_SBU2_Fovi, V33E_Fovi, SBU1_Fpvi, SBU2_Fovi, FLTB_Fovi, -1);
	delay_ms(1);
	// TEST MODE SETTING
	oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(1);
	oviFLTB.Set(FV, -1.0f, FOVI_5V, FOVI_10MA, RELAY_ON);
	delay_ms(6);

	// TEST MODE No.2
	oviFLTB.Set(FV, 1.5f, FOVI_5V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	// TEST MODE No.3
	oviFLTB.Set(FV, 3.3f, FOVI_5V, FOVI_10MA, RELAY_ON);
	delay_ms(2);

	// C_SBU1, C_SBU2
	pviC_SBU1.Set(FV, 4.0f, FPVI10_10V, FPVI10_10MA, RELAY_ON);
	oviC_SBU2.Set(FV, 4.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
  delay_ms(1);

	for(site = 0; site < SITENUM; site++)
		flag[site] = 1;
  StsGetSiteStatus(sitesta, SITENUM);
	for(site = 0; site < SITENUM; site++)	{
  	if(sitesta[site])	flag[site] = 0;	
		else	flag[site] = 1;
	}
	// SBU1 ************ OVP RAMP UP **************************************************************************************** 8-1
	// SBU1 ************ OVP RAMP UP **************************************************************************************** 8-1
	for(Tmpv = 4.0f; Tmpv < 5.35f;)	{
		pviC_SBU1.Set(FV, Tmpv, FPVI10_10V, FPVI10_10MA, RELAY_ON);
  	delay_us(100);

	  pviSBU1.MeasureVI(100, 10);
  	for(site = 0; site < SITENUM; site++)	{
			vSBU1out[site] = pviSBU1.GetMeasResult(site, MVRET);
  		if((flag[site] == 0) && (vSBU1out[site] <= 3.9f))	{
				pviC_SBU1.MeasureVI(100, 10);
				vSBU1_OVP_H[site] = pviC_SBU1.GetMeasResult(site, MVRET);
  			flag[site] = 1;
  		}
 		}
 		if((flag[0] == 1) && (flag[1] == 1))
			break;
 		Tmpv += 5e-3f;
  }	

	// SBU1 ************ OVP RAMP DOWN **************************************************************************************** 8-2, 8-3
	// SBU1 ************ OVP RAMP DOWN **************************************************************************************** 8-2, 8-3
	for(site = 0; site < SITENUM; site++)
		flag[site] = 1;
  StsGetSiteStatus(sitesta, SITENUM);
	for(site = 0; site < SITENUM; site++)	{
  	if(sitesta[site])	flag[site] = 0;	
		else	flag[site] = 1;
	}

	for(Tmpv = 5.3f; Tmpv > 3.8f;)	{
		pviC_SBU1.Set(FV, Tmpv, FPVI10_10V, FPVI10_10MA, RELAY_ON);
  	delay_us(100);

	  pviSBU1.MeasureVI(100, 10);
  	for(site = 0; site < SITENUM; site++)	{
			vSBU1out[site] = pviSBU1.GetMeasResult(site, MVRET);
  		if((flag[site] == 0) && (vSBU1out[site] > 4.0f))	{
				pviC_SBU1.MeasureVI(100, 10);
				vSBU1_OVP_L[site] = pviC_SBU1.GetMeasResult(site, MVRET);
				vSBU1_OVP_HYS[site] = (vSBU1_OVP_H[site] - vSBU1_OVP_L[site])*1e3;
  			flag[site] = 1;
  		}
 		}
 		if((flag[0] == 1) && (flag[1] == 1))
			break;
 		Tmpv += -5e-3f;
  }	

	// SBU2 ************ OVP RAMP UP **************************************************************************************** 8-4
	// SBU2 ************ OVP RAMP UP **************************************************************************************** 8-4
	for(site = 0; site < SITENUM; site++)
		flag[site] = 1;
  StsGetSiteStatus(sitesta, SITENUM);
	for(site = 0; site < SITENUM; site++)	{
  	if(sitesta[site])	flag[site] = 0;	
		else	flag[site] = 1;
	}
	pviC_SBU1.Set(FV, 4.0f, FPVI10_10V, FPVI10_10MA, RELAY_ON);
	oviC_SBU2.Set(FV, 4.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
  delay_ms(1);

	for(Tmpv = 4.0f; Tmpv < 5.35f;)	{
		oviC_SBU2.Set(FV, Tmpv, FOVI_10V, FOVI_10MA, RELAY_ON);
  	delay_us(100);

	  oviSBU2.MeasureVI(100, 10);
  	for(site = 0; site < SITENUM; site++)	{
			vSBU2out[site] = oviSBU2.GetMeasResult(site, MVRET);
  		if((flag[site] == 0) && (vSBU2out[site] <= 3.9f))	{
				oviC_SBU2.MeasureVI(100, 10);
				vSBU2_OVP_H[site] = oviC_SBU2.GetMeasResult(site, MVRET);
  			flag[site] = 1;
  		}
 		}
 		if((flag[0] == 1) && (flag[1] == 1))
			break;
 		Tmpv += 5e-3f;
  }	

	// SBU2 ************ OVP RAMP DOWN **************************************************************************************** 8-5, 8-6
	// SBU2 ************ OVP RAMP DOWN **************************************************************************************** 8-5, 8-6
	for(site = 0; site < SITENUM; site++)
		flag[site] = 1;
  StsGetSiteStatus(sitesta, SITENUM);
	for(site = 0; site < SITENUM; site++)	{
  	if(sitesta[site])	flag[site] = 0;	
		else	flag[site] = 1;
	}

	for(Tmpv = 5.3f; Tmpv > 3.8f;)	{
		oviC_SBU2.Set(FV, Tmpv, FOVI_10V, FOVI_10MA, RELAY_ON);
  	delay_us(100);

	  oviSBU2.MeasureVI(100, 10);
  	for(site = 0; site < SITENUM; site++)	{
			vSBU2out[site] = oviSBU2.GetMeasResult(site, MVRET);
  		if((flag[site] == 0) && (vSBU2out[site] > 4.0f))	{
				oviC_SBU2.MeasureVI(100, 10);
				vSBU2_OVP_L[site] = oviC_SBU2.GetMeasResult(site, MVRET);
				vSBU2_OVP_HYS[site] = (vSBU2_OVP_H[site] - vSBU2_OVP_L[site])*1e3;
  			flag[site] = 1;
  		}
 		}
 		if((flag[0] == 1) && (flag[1] == 1))
			break;
 		Tmpv += -5e-3f;
  }	

	// ========================================================================================================================================================
  for(site = 0; site < SITENUM; site++)	{
		OVPSBU_Rising		->SetTestResult(site, 0, vSBU1_OVP_H[site]);
  	OVPSBU_Falling	->SetTestResult(site, 0, vSBU1_OVP_L[site]);
  	OVPSBU_VHYS			->SetTestResult(site, 0, vSBU1_OVP_HYS[site]);
		OVPSBU_Rising2	->SetTestResult(site, 0, vSBU2_OVP_H[site]);
  	OVPSBU_Falling2	->SetTestResult(site, 0, vSBU2_OVP_L[site]);
  	OVPSBU_VHYS2		->SetTestResult(site, 0, vSBU2_OVP_HYS[site]);
 	}
	pwrZero();
	pwrSW_off();
	rlyC.SetOn(-1);
	delay_ms(1);

  return 0;
}
// test9
// ******************************************************************* SBU ShortClamping *******************************************************************
// ******************************************************************* SBU ShortClamping *******************************************************************
// ******************************************************************* SBU ShortClamping *******************************************************************
DUT_API int SBU_ShortClamp(short funcindex, LPCTSTR funclabel)	{
	//{{AFX_STS_PARAM_PROTOTYPES
    CParam *VST_VBUS_SBU_ClampV = StsGetParam(funcindex,"VST_VBUS_SBU_ClampV");
    CParam *VST_VBUS_SBU_ClampV2 = StsGetParam(funcindex,"VST_VBUS_SBU_ClampV2");
  //}}AFX_STS_PARAM_PROTOTYPES

  // TODO: Add your function code here
	short loopt = 5;
  double SBU_Clamp_V[SITENUM]={0.0f}, vtemp[SITENUM] = { 0.0f };

	for(site = 0; site < SITENUM; site++)
		flag[site] = 1;
  StsGetSiteStatus(sitesta, SITENUM);
	for(site = 0; site < SITENUM; site++)	{
  	if(sitesta[site])	flag[site] = 0;	
		else	flag[site] = 1;
	}

	// V33E
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	pviC_SBU1.Set(FV, 0.0f, FPVI10_50V, FPVI10_1A, RELAY_ON);
	pviSBU1.Set(FI, 0.0e-12f, FPVI10_50V, FPVI10_10UA, RELAY_ON);
	oviC_SBU2.Set(FV, 0.0f, FOVI_50V, FOVI_1A, RELAY_ON);
	oviSBU2.Set(FI, 0.0e-12f, FOVI_50V, FOVI_10UA, RELAY_ON);
	delay_ms(1);

	rlyC.SetOn(VPWR_Cap, V33E_Cap, C_SBU1_Fpvi, C_SBU2_Fovi, SBU1_Fpvi, SBU2_Fovi, V33E_Fovi, FLTB_100K, -1);
	delay_ms(1);
	oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_100MA, RELAY_ON);
  delay_ms(5);
	// ************************ SBU1 short clamping ************************
	// ************************ SBU1 short clamping ************************
	for(jj = 0; jj < int(loopt); jj++)	{
		for(site = 0; site < SITENUM; site++)	{
			pviC_SBU1.Set(FV, 36.0f, FPVI10_50V, FPVI10_100MA, RELAY_ON);
			pviSBU1.MeasureVI(200, 5);
			SBU_Clamp_V[site] = pviSBU1.GetMeasResult(site, MVRET,MAX_RESULT);

			if (SBU_Clamp_V[site] > vtemp[site])
				vtemp[site] = SBU_Clamp_V[site];
			pviC_SBU1.Set(FV, 0.0f, FPVI10_50V, FPVI10_100MA, RELAY_ON);
			delay_ms(1);
		}
		pviC_SBU1.Set(FV, 0.0f, FPVI10_50V, FPVI10_100MA, RELAY_ON);
	  delay_ms(1);
	}
	for(site = 0; site < SITENUM; site++)
  	VST_VBUS_SBU_ClampV->SetTestResult(site, 0, SBU_Clamp_V[site]);			 


	// ************************ SBU2 short clamping ************************
	// ************************ SBU2 short clamping ************************
	for(site = 0; site < SITENUM; site++)
		flag[site] = 1;
  StsGetSiteStatus(sitesta, SITENUM);
	for(site = 0; site < SITENUM; site++)	{
  	if(sitesta[site])	flag[site] = 0;	
		else	flag[site] = 1;
	}
	for(jj = 0; jj < int(loopt); jj++)	{
		for(site = 0; site < SITENUM; site++)	{
			oviC_SBU2.Set(FV, 36.0f, FOVI_50V, FOVI_1A, RELAY_ON);
			oviSBU2.MeasureVI(200, 5);
			SBU_Clamp_V[site] = oviSBU2.GetMeasResult(site, MVRET,MAX_RESULT);

			if (SBU_Clamp_V[site] > vtemp[site])
				vtemp[site] = SBU_Clamp_V[site];
			oviC_SBU2.Set(FV, 0.0f, FOVI_50V, FOVI_1A, RELAY_ON);
			delay_ms(1);
		}
		oviC_SBU2.Set(FV, 0.0f, FOVI_50V, FOVI_1A, RELAY_ON);
	  delay_ms(1);
	}
	for(site = 0; site < SITENUM; site++)
  	VST_VBUS_SBU_ClampV2->SetTestResult(site, 0, SBU_Clamp_V[site]);			 

  return 0;
}
// test10
// ******************************************************************* Quiescent current *******************************************************************
// ******************************************************************* Quiescent current *******************************************************************
// ******************************************************************* Quiescent current *******************************************************************
DUT_API int QuiescentCurrent(short funcindex, LPCTSTR funclabel)	{
	//{{AFX_STS_PARAM_PROTOTYPES
    CParam *IQ_VBUS = StsGetParam(funcindex,"IQ_VBUS");
    CParam *IQ_V33E = StsGetParam(funcindex,"IQ_V33E");
    CParam *IQ2_VBUS = StsGetParam(funcindex,"IQ2_VBUS");
    CParam *IQ2_V33E = StsGetParam(funcindex,"IQ2_V33E");
    CParam *CC1I_Leak = StsGetParam(funcindex,"CC1I_Leak");
    CParam *CC1O_Leak = StsGetParam(funcindex,"CC1O_Leak");
    CParam *CC2I_Leak = StsGetParam(funcindex,"CC2I_Leak");
    CParam *CC2O_Leak = StsGetParam(funcindex,"CC2O_Leak");
    CParam *SBU1I_Leak = StsGetParam(funcindex,"SBU1I_Leak");
    CParam *SBU1O_Leak = StsGetParam(funcindex,"SBU1O_Leak");
    CParam *SBU2I_Leak = StsGetParam(funcindex,"SBU2I_Leak");
    CParam *SBU2O_Leak = StsGetParam(funcindex,"SBU2O_Leak");
    CParam *CC1I_OVP_3V3_Leak = StsGetParam(funcindex,"CC1I_OVP_3V3_Leak");
    CParam *CC1I_OVP_0V_Leak = StsGetParam(funcindex,"CC1I_OVP_0V_Leak");
    CParam *CC2I_OVP_3V3_Leak = StsGetParam(funcindex,"CC2I_OVP_3V3_Leak");
    CParam *CC2I_OVP_0V_Leak = StsGetParam(funcindex,"CC2I_OVP_0V_Leak");
    CParam *CC1O_OVP_3V3_Leak = StsGetParam(funcindex,"CC1O_OVP_3V3_Leak");
    CParam *CC1O_OVP_0V_Leak = StsGetParam(funcindex,"CC1O_OVP_0V_Leak");
    CParam *CC2O_OVP_3V3_Leak = StsGetParam(funcindex,"CC2O_OVP_3V3_Leak");
    CParam *CC2O_OVP_0V_Leak = StsGetParam(funcindex,"CC2O_OVP_0V_Leak");
    CParam *SBU1I_OVP_3V3_Leak = StsGetParam(funcindex,"SBU1I_OVP_3V3_Leak");
    CParam *SBU1O_OVP_3V3_Leak = StsGetParam(funcindex,"SBU1O_OVP_3V3_Leak");
    CParam *SBU1I_OVP_0V_Leak = StsGetParam(funcindex,"SBU1I_OVP_0V_Leak");
    CParam *SBU1O_OVP_0V_Leak = StsGetParam(funcindex,"SBU1O_OVP_0V_Leak");
    CParam *SBU2I_OVP_3V3_Leak = StsGetParam(funcindex,"SBU2I_OVP_3V3_Leak");
    CParam *SBU2O_OVP_3V3_Leak = StsGetParam(funcindex,"SBU2O_OVP_3V3_Leak");
    CParam *SBU2I_OVP_0V_Leak = StsGetParam(funcindex,"SBU2I_OVP_0V_Leak");
    CParam *SBU2O_OVP_0V_Leak = StsGetParam(funcindex,"SBU2O_OVP_0V_Leak");
  //}}AFX_STS_PARAM_PROTOTYPES

	// TODO: Add your function code here
	for(site = 0; site < SITENUM; site++)
		flag[site] = 1;
  StsGetSiteStatus(sitesta, SITENUM);
	for(site = 0; site < SITENUM; site++)	{
  	if(sitesta[site])	flag[site] = 0;	
		else	flag[site] = 1;
	}
	// ********************************** 10-1 **********************************
	// V33E, VBUS
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	oviVBUS.Set(FV, 0.0f, FOVI_50V, FOVI_100UA, RELAY_ON);
	// RPD1, RPD2
	oviRPD1.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	oviRPD2.Set(FV, 0.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	rlyC.SetOn(VBUS_Fovi, V33E_Fovi, V33E_Cap, VPWR_Cap, RPD1_Fovi, RPD2_Fovi, VPWR_Fpvi, -1);
	delay_ms(1);
	// ******************** V33E quiescent current 5V and 3.3 ********************	// 0
	// ******************** VBUS quiescent current 5V and 3.3 ********************	// 1
	// VBUS=5
	oviVBUS.Set(FV, 5.0f, FOVI_50V, FOVI_1MA, RELAY_ON);
	delay_ms(1);
	// V33E=3.3
	oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_10MA, RELAY_ON);
  delay_ms(5);

	oviV33E.MeasureVI(100, 20);
	oviVBUS.MeasureVI(100, 20);
  for(site = 0; site < SITENUM; site++)	{
	  IQ_V33E->SetTestResult(site, 0, (fabs(oviV33E.GetMeasResult(site, MIRET))*1e6f) - 0.0f);  // offset	
    IQ_VBUS->SetTestResult(site, 0, fabs(oviVBUS.GetMeasResult(site, MIRET))*1e6f - 0.0f);
 	}

	// ******************** V33E quiescent current 36V and 3.3 ********************	// 2
	// ******************** VBUS quiescent current 36V and 3.3 ********************	// 3
	oviVBUS.Set(FV, 10.0f, FOVI_50V, FOVI_1MA, RELAY_ON);
	delay_ms(1);
	oviVBUS.Set(FV, 20.0f, FOVI_50V, FOVI_1MA, RELAY_ON);
	delay_ms(1);

	oviVBUS.Set(FV, 25.0f, FOVI_50V, FOVI_1MA, RELAY_ON);
	delay_ms(1);
	oviVBUS.Set(FV, 23.0f, FOVI_50V, FOVI_1MA, RELAY_ON);
	delay_ms(1);
	oviVBUS.Set(FV, 36.0f, FOVI_50V, FOVI_1MA, RELAY_ON);
	delay_ms(1);

	oviV33E.MeasureVI(100, 20);
	oviVBUS.MeasureVI(100, 20);
	for(site = 0; site < SITENUM; site++)	{
	  IQ2_V33E->SetTestResult(site, 0, (fabs(oviV33E.GetMeasResult(site, MIRET))*1e6f) - 0.0f); // offset	
	  IQ2_VBUS->SetTestResult(site, 0, (fabs(oviVBUS.GetMeasResult(site, MIRET))*1e6f) - 0.0f); // offset
 	}
	pwrZero();
	pwrSW_off();
	delay_ms(1);
	// ********************************** 10-2 **********************************
	// ******************** Leakage for C_CC1 ********************	// 4
	rlyC.SetOn(V33E_Cap, VPWR_Cap,V33E_Fovi, C_CC1_Fpvi,CC1_Fpvi, VPWR_Fpvi, FLTB_100K, RPD1_Fovi, RPD2_Fovi, -1);
	delay_ms(1);

	oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(6);

	pviC_CC1.Set(FV, 3.6f, FPVI10_10V, FPVI10_100UA, RELAY_ON);
	delay_ms(1);

	pviC_CC1.MeasureVI(100, 20);
  for(site = 0; site < SITENUM; site++)
	  CC1I_Leak->SetTestResult(site, 0, fabs(pviC_CC1.GetMeasResult(site, MIRET))*1e6f);		

	pviC_CC1.Set(FV, 0.0f, FPVI10_10V, FPVI10_100UA, RELAY_ON);
	delay_us(100);
	pviC_CC1.Set(FV, 0.0f, FPVI10_10V, FPVI10_100UA, RELAY_OFF);
	delay_ms(1);

	// ******************** Leakage for CC1 ********************	// 5
	pviCC1.Set(FV, 3.6f, FPVI10_10V, FPVI10_100UA, RELAY_ON);
	delay_ms(3);

	pviCC1.MeasureVI(100, 20);
 	for(site = 0; site < SITENUM; site++)
  	CC1O_Leak->SetTestResult(site, 0, fabs(pviCC1.GetMeasResult(site, MIRET))*1e6f);		

	pwrZero();
	pwrSW_off();
	delay_ms(1);

	// ******************** Leakage for C_CC2 ********************	// 6
	rlyC.SetOn(V33E_Cap, VPWR_Cap,V33E_Fovi, C_CC2_Fovi,CC2_Fovi, VPWR_Fpvi, FLTB_100K, RPD1_Fovi, RPD2_Fovi, -1);
	delay_ms(1);
	oviCC2.Set(FV, 0.0f, FOVI_10V, FOVI_100UA, RELAY_OFF);
	delay_ms(1);

	oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(6);

	oviC_CC2.Set(FV, 3.6f, FOVI_10V, FOVI_100UA, RELAY_ON);
	delay_ms(6);

	oviC_CC2.MeasureVI(100, 20);
	for(site = 0; site < SITENUM; site++)
	  CC2I_Leak->SetTestResult(site, 0, fabs(oviC_CC2.GetMeasResult(site, MIRET))*1e6f);		
	
	// ******************** Leakage for CC2 ********************	// 7
	oviC_CC2.Set(FV, 0.0f, FOVI_10V, FOVI_100UA, RELAY_ON);
	delay_us(100);
	oviC_CC2.Set(FV, 0.0f, FOVI_10V, FOVI_100UA, RELAY_OFF);
	delay_ms(1);

	oviCC2.Set(FV, 3.6f, FOVI_10V, FOVI_100UA, RELAY_ON);
	delay_ms(3);

	oviCC2.MeasureVI(100, 20);
  for(site = 0; site < SITENUM; site++)
	  CC2O_Leak->SetTestResult(site, 0, fabs(oviCC2.GetMeasResult(site, MIRET))*1e6f);		

	pwrZero();
	pwrSW_off();
	delay_ms(1);
	// ********************************** 10-3 **********************************
	// ******************** Leakage for C_SUB1 ********************	// 8
	// RPD1, RPD2, SBU1
	oviRPD1.Set(FV, 0.0f, FOVI_2V, FOVI_10MA, RELAY_ON);
	oviRPD2.Set(FV, 0.0f, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	pviSBU1.Set(FV, 0.0f, FPVI10_10V, FPVI10_100UA, RELAY_OFF);
	pviC_SBU1.Set(FV, 0.0f, FPVI10_10V, FPVI10_100UA, RELAY_ON);

	rlyC.SetOn(V33E_Cap, VPWR_Cap,V33E_Fovi, C_SBU1_Fpvi,SBU1_Fpvi, VPWR_Fpvi, FLTB_100K, RPD1_Fovi, RPD2_Fovi, -1);
	delay_ms(1);
	pviSBU1.Set(FV, 0.0f, FPVI10_10V, FPVI10_100UA, RELAY_OFF);
	delay_ms(3);

	oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(3);

	pviC_SBU1.Set(FV, 3.6f, FPVI10_10V, FPVI10_10UA, RELAY_ON);
	delay_ms(9);

	pviC_SBU1.MeasureVI(100, 20);
	 	for(site = 0; site < SITENUM; site++)
 		  SBU1I_Leak->SetTestResult(site, 0, fabs(pviC_SBU1.GetMeasResult(site, MIRET))*1e9f);		

	// ******************** Leakage for SUB1 **********************	// 9
	pviC_SBU1.Set(FV, 0.0f, FPVI10_10V, FPVI10_100UA, RELAY_ON);
	delay_us(100);
	pviC_SBU1.Set(FV, 0.0f, FPVI10_10V, FPVI10_100UA, RELAY_OFF);
	delay_ms(1);

	pviSBU1.Set(FV, 3.6f, FPVI10_10V, FPVI10_10UA, RELAY_ON);
	delay_ms(9);

	pviSBU1.MeasureVI(100, 20);	
		for(site = 0; site < SITENUM; site++)	
 		  SBU1O_Leak->SetTestResult(site, 0, fabs(pviSBU1.GetMeasResult(site, MIRET))*1e9f);		

	pviSBU1.Set(FV, 0.0f, FPVI10_10V, FPVI10_100UA, RELAY_ON);
	pviC_SBU1.Set(FV, 0.0f, FPVI10_10V, FPVI10_100UA, RELAY_ON);
	pviCC1.Set(FV, 0.0f, FPVI10_10V, FPVI10_100UA, RELAY_ON);
	pviC_CC1.Set(FV, 0.0f, FPVI10_10V, FPVI10_100UA, RELAY_ON);
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_us(100);

	// ******************** Leakage for C_SBU2 ********************	// 10
	oviRPD1.Set(FV, 0.0f, FOVI_2V, FOVI_10MA, RELAY_ON);
	oviRPD2.Set(FV, 0.0f, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	rlyC.SetOn(V33E_Cap, VPWR_Cap,V33E_Fovi, C_SBU2_Fovi,SBU2_Fovi, VPWR_Fpvi, FLTB_100K, RPD1_Fovi, RPD2_Fovi, -1);
	delay_ms(1);

	oviSBU2.Set(FV, 0.0f, FOVI_10V, FOVI_100UA, RELAY_OFF);
	delay_ms(1);

	oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(6);

	oviC_SBU2.Set(FV, 3.6f, FOVI_10V, FOVI_10UA, RELAY_ON);
	delay_ms(9);

	oviC_SBU2.MeasureVI(100, 20);
  	for(site = 0; site < SITENUM; site++)
  		SBU2I_Leak->SetTestResult(site, 0, fabs(oviC_SBU2.GetMeasResult(site, MIRET))*1e9f);		

	// ******************** Leakage for SUB2 **********************	// 11
	oviC_SBU2.Set(FV, 0.0f, FOVI_10V, FOVI_100UA, RELAY_ON);
	delay_us(100);
	oviC_SBU2.Set(FV, 0.0f, FOVI_10V, FOVI_100UA, RELAY_OFF);
	delay_ms(1);

	oviSBU2.Set(FV, 3.6f, FOVI_10V, FOVI_10UA, RELAY_ON);
	delay_ms(9);

	oviSBU2.MeasureVI(100, 20);
  	for(site = 0; site < SITENUM; site++)
  	{
  		  SBU2O_Leak->SetTestResult(site, 0, fabs(oviSBU2.GetMeasResult(site, MIRET))*1e9f);		
 	}
	oviSBU2.Set(FV, 0.0f, FOVI_10V, FOVI_100UA, RELAY_ON);
	oviC_SBU2.Set(FV, 0.0f, FOVI_10V, FOVI_100UA, RELAY_ON);
	oviCC2.Set(FV, 0.0f, FOVI_10V, FOVI_100UA, RELAY_ON);
	oviC_CC2.Set(FV, 0.0f, FOVI_10V, FOVI_100UA, RELAY_ON);
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(1);
	// ********************************** 10-4 **********************************
	// ******************** Leakage for C_CC1 3.3V ****************	// 12
  //========================================================================
  pviCC1.Set(FV, 0.0f, FPVI10_1V, FPVI10_100UA, RELAY_ON);
	pviC_CC1.Set(FV, 0.0f, FPVI10_50V, FPVI10_10MA, RELAY_ON);
	oviVBUS.Set(FV, 0.0f, FOVI_50V, FOVI_10MA, RELAY_ON);

	rlyC.SetOn(V33E_Cap, VPWR_Cap, V33E_Fovi, VBUS_Fovi, VPWR_Fpvi, FLTB_100K,RPD1_Fovi, RPD2_Fovi, C_CC1_Fpvi, CC1_Fpvi, -1);
	delay_ms(10);

	oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(5);
	oviVBUS.Set(FV, 36.0f, FOVI_50V, FOVI_10MA, RELAY_ON);
	pviC_CC1.Set(FV, 36.0f, FPVI10_50V, FPVI10_10MA, RELAY_ON);
	delay_ms(20);
	pviCC1.Set(FV, 0.0f, FPVI10_5V, FPVI10_1MA, RELAY_ON);
	delay_ms(50);

	pviC_CC1.MeasureVI(100, 20);
  for(site = 0; site < SITENUM; site++)
 	  CC1I_OVP_3V3_Leak->SetTestResult(site, 0, fabs(pviC_CC1.GetMeasResult(site, MIRET))*1e6f);		

	// ******************** Leakage for C_CC1 0V ******************	// 13
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(6);

	pviC_CC1.MeasureVI(100, 20);
	for(site = 0; site < SITENUM; site++)
	  CC1I_OVP_0V_Leak->SetTestResult(site, 0, fabs(pviC_CC1.GetMeasResult(site, MIRET))*1e6f);		

  pviCC1.Set(FV, 0.0f, FPVI10_1V, FPVI10_10UA, RELAY_ON);
	pviC_CC1.Set(FV, 0.0f, FPVI10_50V, FPVI10_10MA, RELAY_ON);
	oviVBUS.Set(FV, 0.0f, FOVI_50V, FOVI_10MA, RELAY_ON);
	delay_us(100);
	oviVBUS.Set(FV, 0.0f, FOVI_50V, FOVI_10MA, RELAY_OFF);
	delay_ms(1);


	// ******************** Leakage for C_CC2 3.3V ****************	// 14
	oviC_CC2.Set(FV, 0.0f, FOVI_50V, FOVI_10MA, RELAY_ON);
	oviCC2.Set(FV, 0.0f, FOVI_50V, FOVI_10MA, RELAY_ON);
	oviVBUS.Set(FV, 0.0f, FOVI_50V, FOVI_10MA, RELAY_ON);

	rlyC.SetOn(V33E_Cap, VPWR_Cap,V33E_Fovi, VBUS_Fovi, VPWR_Fpvi, FLTB_100K,RPD1_Fovi, RPD2_Fovi, C_CC2_Fovi, CC2_Fovi, -1);
	delay_ms(10);

	oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(5);
	oviVBUS.Set(FV, 36.0f, FOVI_50V, FOVI_10MA, RELAY_ON);
	oviC_CC2.Set(FV, 36.0f, FOVI_50V, FOVI_10MA, RELAY_ON);	// TO 10mA RANGE, 250826
	delay_ms(20);
	oviCC2.Set(FV, 0.0f, FOVI_5V, FOVI_1MA, RELAY_ON);
	delay_ms(50);

	oviC_CC2.MeasureVI(100, 20);
  	for(site = 0; site < SITENUM; site++)
 		  CC2I_OVP_3V3_Leak->SetTestResult(site, 0, (fabs(oviC_CC2.GetMeasResult(site, MIRET))*1e6f) + 0e-6f); // offset

	// ******************** Leakage for C_CC2 0V ******************	// 15
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(6);

	oviC_CC2.MeasureVI(100, 20);
	for(site = 0; site < SITENUM; site++)
	  CC2I_OVP_0V_Leak->SetTestResult(site, 0, (fabs(oviC_CC2.GetMeasResult(site, MIRET))*1e6f) + 0e-6f);    // offset	

	oviC_CC2.Set(FV, 0.0f, FOVI_50V, FOVI_10MA, RELAY_ON);
	oviCC2.Set(FV, 0.0f, FOVI_50V, FOVI_10MA, RELAY_ON);
	oviVBUS.Set(FV, 0.0f, FOVI_50V, FOVI_10MA, RELAY_ON);
	delay_us(100);
	oviVBUS.Set(FV, 0.0f, FOVI_50V, FOVI_10MA, RELAY_OFF);
	delay_ms(1);
	// ********************************** 10-5 **********************************
	// ******************** Leakage for CC1 3.3V *****************	// 16
  pviCC1.Set(FV, 0.0f, FPVI10_1V, FPVI10_100UA, RELAY_ON);
	pviC_CC1.Set(FV, 0.0f, FPVI10_50V, FPVI10_10MA, RELAY_ON);
	oviVBUS.Set(FV, 0.0f, FOVI_50V, FOVI_10MA, RELAY_ON);

	rlyC.SetOn(V33E_Cap, VPWR_Cap, V33E_Fovi, VPWR_Fpvi, FLTB_100K, RPD1_Fovi, RPD2_Fovi, C_CC1_Fpvi, CC1_Fpvi, -1);
	delay_ms(1);
	oviRPD1.Set(FV, 0.0f, FOVI_2V, FOVI_10MA, RELAY_ON);
	oviRPD2.Set(FV, 0.0f, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(5);
	pviC_CC1.Set(FV, 36.0f, FPVI10_50V, FPVI10_10MA, RELAY_ON);
	delay_ms(2);
	pviCC1.Set(FV, 0.0f, FPVI10_50V, FPVI10_10UA, RELAY_ON);
	delay_ms(2); 

	pviCC1.MeasureVI(100, 20);
 	for(site = 0; site < SITENUM; site++)
	  CC1O_OVP_3V3_Leak->SetTestResult(site, 0, fabs(pviCC1.GetMeasResult(site, MIRET))*1e9f);		

	// ******************** Leakage for CC1 0V ********************	// 17
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(6);

	pviCC1.MeasureVI(100, 20);
  for(site = 0; site < SITENUM; site++)
	  CC1O_OVP_0V_Leak->SetTestResult(site, 0, fabs(pviCC1.GetMeasResult(site, MIRET))*1e9f);		

  pviCC1.Set(FV, 0.0f, FPVI10_1V, FPVI10_10UA, RELAY_ON);
	pviC_CC1.Set(FV, 0.0f, FPVI10_50V, FPVI10_10MA, RELAY_ON);
	oviVBUS.Set(FV, 0.0f, FOVI_50V, FOVI_10MA, RELAY_ON);
	delay_us(100);
	oviVBUS.Set(FV, 0.0f, FOVI_50V, FOVI_10MA, RELAY_OFF);
	delay_ms(1);

	// ******************** Leakage for CC2 3.3V ******************	// 18
	oviC_CC2.Set(FV, 0.0f, FOVI_50V, FOVI_10MA, RELAY_ON);
	oviCC2.Set(FV, 0.0f, FOVI_50V, FOVI_10MA, RELAY_ON);
	oviVBUS.Set(FV, 0.0f, FOVI_50V, FOVI_10MA, RELAY_ON);

	rlyC.SetOn(V33E_Cap, VPWR_Cap,V33E_Fovi, VPWR_Fpvi, FLTB_100K, RPD1_Fovi, RPD2_Fovi, C_CC2_Fpvi, CC2_Fpvi, -1);
	delay_ms(1);

	oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(5);
	pviC_CC1.Set(FV, 36.0f, FPVI10_50V, FPVI10_100MA, RELAY_ON);
	delay_ms(2);
	pviCC1.Set(FV, 0.0f, FPVI10_50V, FPVI10_10UA, RELAY_ON);
	delay_ms(2);

	pviCC1.MeasureVI(100, 20);
	for(site = 0; site < SITENUM; site++)
		CC2O_OVP_3V3_Leak->SetTestResult(site, 0, fabs(pviCC1.GetMeasResult(site, MIRET))*1e9f);		

	// ******************** Leakage for CC2 0V ********************	// 19	
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(6);

	pviCC1.MeasureVI(100, 20);
	for(site = 0; site < SITENUM; site++)
		CC2O_OVP_0V_Leak->SetTestResult(site, 0, fabs(pviCC1.GetMeasResult(site, MIRET))*1e9f);		

	pviC_CC1.Set(FV, 0.0f, FPVI10_50V, FPVI10_10MA, RELAY_ON);
	pviCC1.Set(FV, 0.0f, FPVI10_50V, FPVI10_10MA, RELAY_ON);
	
	oviVBUS.Set(FV, 0.0f, FOVI_50V, FOVI_10MA, RELAY_ON);
	delay_us(100);
	oviVBUS.Set(FV, 0.0f, FOVI_50V, FOVI_10MA, RELAY_OFF);
	delay_ms(1);
	// ********************************** 10-6 **********************************
	// ******************** Leakage for C_SBU1 3.3V ******************	// 20
	// ******************** Leakage for C_SBU1 3.3V ******************	// 21
	pviSBU1.Set(FV, 0.0f, FPVI10_1V, FPVI10_10UA, RELAY_ON);
	pviC_SBU1.Set(FV, 0.0f, FPVI10_50V, FPVI10_10MA, RELAY_ON);

	rlyC.SetOn(V33E_Cap, VPWR_Cap,V33E_Fovi,  VPWR_Fpvi, FLTB_100K, RPD1_Fovi, RPD2_Fovi, C_SBU1_Fpvi, SBU1_Fpvi, -1);
	delay_ms(1);
	oviRPD1.Set(FV, 0.0f, FOVI_2V, FOVI_10MA, RELAY_ON);
	oviRPD2.Set(FV, 0.0f, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(5);
	pviC_SBU1.Set(FV, 36.0f, FPVI10_50V, FPVI10_10MA, RELAY_ON);
	delay_ms(2);
	pviSBU1.Set(FV, 0.0f, FPVI10_50V, FPVI10_10UA, RELAY_ON);
	delay_ms(2);

	pviC_SBU1.MeasureVI(100, 20);
	pviSBU1.MeasureVI(100, 20);
	for(site = 0; site < SITENUM; site++)	{
		 SBU1I_OVP_3V3_Leak->SetTestResult(site, 0, fabs(pviC_SBU1.GetMeasResult(site, MIRET))*1e6f);		
  	 SBU1O_OVP_3V3_Leak->SetTestResult(site, 0, fabs(pviSBU1.GetMeasResult(site, MIRET))*1e9f);		
	}

	// ******************** Leakage for C_SBU1 0V ********************	// 22
	// ******************** Leakage for C_SBU1 0V ********************	// 23
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(6);

	pviC_SBU1.MeasureVI(100, 20);
	pviSBU1.MeasureVI(100, 20);
  for(site = 0; site < SITENUM; site++) 	{
  	SBU1I_OVP_0V_Leak->SetTestResult(site, 0, fabs(pviC_SBU1.GetMeasResult(site, MIRET))*1e6f);	
  	SBU1O_OVP_0V_Leak->SetTestResult(site, 0, fabs(pviSBU1.GetMeasResult(site, MIRET))*1e9f);	
 	}
  pviSBU1.Set(FV, 0.0f, FPVI10_1V, FPVI10_10UA, RELAY_ON);
	pviC_SBU1.Set(FV, 0.0f, FPVI10_50V, FPVI10_10MA, RELAY_ON);
	delay_us(100);
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
		delay_ms(1);
	// ********************************** 10-7 **********************************
	// ******************** Leakage for C_SBU2 3.3V ******************	// 24
	// ******************** Leakage for C_SBU2 3.3V ******************	// 25
	oviSBU2.Set(FV, 0.0f, FOVI_50V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	oviC_SBU2.Set(FV, 0.0f, FOVI_50V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	rlyC.SetOn(V33E_Cap, VPWR_Cap, V33E_Fovi, VPWR_Fpvi, FLTB_100K, RPD1_Fovi, RPD2_Fovi, C_SBU2_Fovi, SBU2_Fovi, -1);
	delay_ms(1);
	oviRPD1.Set(FV, 0.0f, FOVI_2V, FOVI_10MA, RELAY_ON);
	oviRPD2.Set(FV, 0.0f, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(5);
	oviC_SBU2.Set(FV, 36.0f, FOVI_50V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	oviSBU2.Set(FV, 0.0f, FOVI_50V, FOVI_10UA, RELAY_ON);
	delay_ms(1);

	oviC_SBU2.MeasureVI(100, 20);
	oviSBU2.MeasureVI(100, 20);
	for(site = 0; site < SITENUM; site++)	{
		SBU2I_OVP_3V3_Leak->SetTestResult(site, 0, fabs(oviC_SBU2.GetMeasResult(site, MIRET))*1e6f);
  	SBU2O_OVP_3V3_Leak->SetTestResult(site, 0, fabs(oviSBU2.GetMeasResult(site, MIRET))*1e9f);		
 	}
	// ******************** Leakage for C_SBU2 0V ********************	// 26
	// ******************** Leakage for C_SBU2 0V ********************	// 27
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(6);

	oviC_SBU2.MeasureVI(100, 20);
	oviSBU2.MeasureVI(100, 20);
	for(site = 0; site < SITENUM; site++)	{
		SBU2I_OVP_0V_Leak->SetTestResult(site, 0, fabs(oviC_SBU2.GetMeasResult(site, MIRET))*1e6f);
  	SBU2O_OVP_0V_Leak->SetTestResult(site, 0, fabs(oviSBU2.GetMeasResult(site, MIRET))*1e9f);	
 	}

	pwrZero();
	pwrSW_off();
	rlyC.SetOn(-1);
	delay_ms(1);
	
	return 0;
}
// test11
// ******************************************************************* VPWR LoadSwitch on resistance *******************************************************************
// ******************************************************************* VPWR LoadSwitch on resistance *******************************************************************
// ******************************************************************* VPWR LoadSwitch on resistance *******************************************************************
DUT_API int VPWER_LoadSW_R(short funcindex, LPCTSTR funclabel)	{
	//{{AFX_STS_PARAM_PROTOTYPES
    CParam *RV33E_Ron = StsGetParam(funcindex,"RV33E_Ron");
  //}}AFX_STS_PARAM_PROTOTYPES

	// TODO: Add your function code here
	for(site = 0; site < SITENUM; site++)
		flag[site] = 1;
  StsGetSiteStatus(sitesta, SITENUM);
	for(site = 0; site < SITENUM; site++)	{
  	if(sitesta[site])	flag[site] = 0;	
		else	flag[site] = 1;
	}
	double r33E[SITENUM] = { 0.0f }, vPWRLoading = 10e-3f, iload[SITENUM] = { 0.0f };
  double v33E[SITENUM] = { 0.0f }, vPWR[SITENUM] = { 0.0f };

	// V33E, VPWR
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	pviVPWR.Set(FI, 0.0e-12f, FPVI10_10V, FPVI10_100MA, RELAY_ON);
	delay_ms(1);

	rlyC.SetOn(VPWR_Cap, V33E_Cap, V33E_Fovi, VPWR_Fpvi, -1);
	delay_ms(1);

	oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(6);
	pviVPWR.Set(FI, -vPWRLoading, FPVI10_10V, FPVI10_100MA, RELAY_ON);
	delay_ms(1);

	oviV33E.MeasureVI(100, 10);
	pviVPWR.MeasureVI(100, 10);
	for(site = 0; site < SITENUM; site++)	{
		v33E[site] = oviV33E.GetMeasResult(site, MVRET);
    vPWR[site] = pviVPWR.GetMeasResult(site, MVRET);
		iload[site] = pviVPWR.GetMeasResult(site, MIRET);
		RV33E_Ron->SetTestResult(site, 0, (vPWR[site] - v33E[site])/(iload[site] + 0.1e-12));
 	}
	pwrZero();
	pwrSW_off();
	rlyC.SetOn(-1);
	delay_ms(1);
	
	return 0;
}
// test12
// ******************************************************************* V33E UVLO Threshold voltage *******************************************************************
// ******************************************************************* V33E Threshold voltage *******************************************************************
// ******************************************************************* V33E Threshold voltage *******************************************************************
DUT_API int UVLO_Threshold(short funcindex, LPCTSTR funclabel)	{
	//{{AFX_STS_PARAM_PROTOTYPES
    CParam *UVLO_Rising = StsGetParam(funcindex,"UVLO_Rising");
    CParam *UVLO_Falling = StsGetParam(funcindex,"UVLO_Falling");
    CParam *UVLO_VHYS = StsGetParam(funcindex,"UVLO_VHYS");
  //}}AFX_STS_PARAM_PROTOTYPES

  // TODO: Add your function code here
	for(site = 0; site < SITENUM; site++)
		flag[site] = 1;
  StsGetSiteStatus(sitesta, SITENUM);
	for(site = 0; site < SITENUM; site++)	{
  	if(sitesta[site])	flag[site] = 0;	
		else	flag[site] = 1;
	}
	double vPWR[SITENUM] = { 0.0f }, v33ER[SITENUM] = { 0.0f }, v33EL[SITENUM] = { 0.0f }, v33EHys[SITENUM] = { 0.0f };
	double iLoading = 1e-3f, vjudgement = 2.8f;

	// VPWR, V33E, VBUS
	pviVPWR.Set(FI, 0.0e-12f, FPVI10_10V, FPVI10_100MA, RELAY_ON);
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	oviVBUS.Set(FV, 0.0f, FOVI_50V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	rlyC.SetOn(VPWR_Cap, V33E_Cap, V33E_Fovi, VBUS_Fovi,RPD1_Fovi, RPD2_Fovi, FLTB_100K, VPWR_Fpvi, -1);
	delay_ms(1);
	// RPD1, RPD2
	oviRPD1.Set(FV, 0.0f, FOVI_2V, FOVI_10MA, RELAY_ON);
	oviRPD2.Set(FV, 0.0f, FOVI_2V, FOVI_10MA, RELAY_ON);
	// V33E, VBUS, VPWR
	oviV33E.Set(FV, 3.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(6);
	oviVBUS.Set(FV, 5.0f, FOVI_50V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	pviVPWR.Set(FI, -iLoading, FPVI10_10V, FPVI10_100MA, RELAY_ON);
	delay_ms(1)	;
	oviV33E.Set(FV, 2.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(5);
	// ********************* V33E RISING, RAMP UP V33E ********************* 12-1
	// ********************* V33E RISING, RAMP UP V33E ********************* 12-1
 	for(Tmpv = 2.0f; Tmpv < 3.0f;)	{
		oviV33E.Set(FV, Tmpv, FOVI_10V, FOVI_100MA, RELAY_ON);
  	delay_us(100);

	  pviVPWR.MeasureVI(10, 10);
		for(site = 0; site < SITENUM; site++)	{
			vPWR[site] = pviVPWR.GetMeasResult(site, MVRET);
	 		if((flag[site] == 0) && (vPWR[site] < vjudgement))	{
			  oviV33E.MeasureVI(10, 10);
				v33ER[site] = oviV33E.GetMeasResult(site, MVRET);
				flag[site] = 1;
  		}
  	}
  	if((flag[0] == 1) && (flag[1] == 1))
			break;
  	Tmpv += 2e-3f;
  }	
	// ********************* V33E FALLING, RAMP DOWN V33E ********************* 12-2, 12-3
	// ********************* V33E FALLING, RAMP DOWN V33E ********************* 12-2, 12-3
	for(site = 0; site < SITENUM; site++)
		flag[site] = 1;
  StsGetSiteStatus(sitesta, SITENUM);
	for(site = 0; site < SITENUM; site++)	{
  	if(sitesta[site])	flag[site] = 0;	
		else	flag[site] = 1;
	}

	// V33E = 3.0V
	oviV33E.Set(FV, 3.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
  delay_ms(1);

  for(Tmpv = 3.0f; Tmpv > 2.0f;)	{
		oviV33E.Set(FV, Tmpv, FOVI_10V, FOVI_100MA, RELAY_ON);
  	delay_us(100);
    
		pviVPWR.MeasureVI(10, 10);
  	for(site = 0; site < SITENUM; site++)	{
			vPWR[site] = pviVPWR.GetMeasResult(site, MVRET);

  		if((flag[site] == 0) && (vPWR[site] > vjudgement + 0.2f))	{
				v33EL[site] = Tmpv;
				v33EHys[site] = v33ER[site] - v33EL[site];
  			flag[site] = 1;
			}
		}
  	if((flag[0] == 1) && (flag[1] == 1))
			break;
		Tmpv += -2e-3f;
	}
 	for(site = 0; site < SITENUM; site++)	{
		UVLO_Rising	->SetTestResult(site, 0, v33ER[site]);
  	UVLO_Falling->SetTestResult(site, 0, v33EL[site]);
  	UVLO_VHYS		->SetTestResult(site, 0, v33EHys[site]*1e3);
 	}
	pwrZero();
	pwrSW_off();
	rlyC.SetOn(-1);
	delay_ms(1);
   
	return 0;
}
// test13
// ******************************************************************* VPWR UVLO  *******************************************************************
// ******************************************************************* VPWR UVLO  *******************************************************************
// ******************************************************************* VPWR UVLO *******************************************************************
DUT_API int VPWR_UVLO_Threshold(short funcindex, LPCTSTR funclabel)	{
	//{{AFX_STS_PARAM_PROTOTYPES
    CParam *VPER_UVLO_Rising = StsGetParam(funcindex,"VPER_UVLO_Rising");
    CParam *VPWR_UVLO_Falling = StsGetParam(funcindex,"VPWR_UVLO_Falling");
    CParam *VPWR_UVLO_VHYS = StsGetParam(funcindex,"VPWR_UVLO_VHYS");
  //}}AFX_STS_PARAM_PROTOTYPES
	
	for(site = 0; site < SITENUM; site++)
		flag[site] = 1;
  StsGetSiteStatus(sitesta, SITENUM);
	for(site = 0; site < SITENUM; site++)	{
  	if(sitesta[site])	flag[site] = 0;	
		else	flag[site] = 1;
	}

	double vSBU1out[SITENUM] = { 0.0f }, vUVLOH[SITENUM] = { 0.0f }, vUVLOL[SITENUM] = { 0.0f }, vUVLOHys[SITENUM] = { 0.0f };
	double iLoading = 1e-3f;

	// VPWR, C_SBU1 = 0V
	pviVPWR.Set(FV, 0.0f, FPVI10_10V, FPVI10_100MA, RELAY_ON);
	pviC_SBU1.Set(FV, 0.0f, FPVI10_2V, FPVI10_10MA, RELAY_ON);
	delay_ms(1);

	rlyC.SetOn(VPWR_Cap, V33E_Cap, RPD1_Fovi, RPD2_Fovi, FLTB_100K, VPWR_Fpvi, C_SBU1_Fpvi, SBU1_Fpvi, -1);
	delay_ms(1);
	// C_SBU1 FORCE 1.5V
	pviC_SBU1.Set(FV, 1.5f, FPVI10_2V, FPVI10_10MA, RELAY_ON);
	delay_ms(1);
	// RPD1, RPD2
	oviRPD1.Set(FV, 0.0f, FOVI_2V, FOVI_10MA, RELAY_ON);
	oviRPD2.Set(FV, 0.0f, FOVI_2V, FOVI_10MA, RELAY_ON);

	pviSBU1.Set(FI, -1*iLoading, FPVI10_10V, FPVI10_100MA, RELAY_ON);
	delay_ms(1);
	// VPWR
	pviVPWR.Set(FV, 2.0f, FPVI10_10V, FPVI10_100MA, RELAY_ON);
	delay_ms(5);
	// ************************************ VPWR UVLO RISING, RAMP UP VPWR ************************************ 13-1
	// ************************************ VPWR UVLO RISING, RAMP UP VPWR ************************************ 13-1
	for (Tmpv = 2.0f; Tmpv < 3.0f;)	{
		pviVPWR.Set(FV, Tmpv, FPVI10_10V, FPVI10_100MA, RELAY_ON);
		delay_us(100);

		pviSBU1.MeasureVI(20, 10);
		for (site = 0; site<SITENUM; site++)	{
			vSBU1out[site] = pviSBU1.GetMeasResult(site, MVRET);

			if ((flag[site] == 0) && (vSBU1out[site] >= 1.0f))	{
				vUVLOH[site] = Tmpv;
				flag[site] = 1;
			}
		}
		if ((flag[0] == 1) && (flag[1] == 1))
			break;
		Tmpv += 0.002f;
	}
	// ************************************ VPWR UVLO FALLING, RAMP DOWN VPWR ************************************ 13-2, 13-3
	// ************************************ VPWR UVLO FALLING, RAMP DOWN VPWR ************************************ 13-2, 13-3
	pviVPWR.Set(FV, 3.0f, FPVI10_10V, FPVI10_100MA, RELAY_ON);
	delay_ms(1);

	for(site = 0; site < SITENUM; site++)
		flag[site] = 1;
  StsGetSiteStatus(sitesta, SITENUM);
	for(site = 0; site < SITENUM; site++)	{
  	if(sitesta[site])	flag[site] = 0;	
		else	flag[site] = 1;
	}

	for (Tmpv = 3.0f; Tmpv > 2.0f;)	{
		pviVPWR.Set(FV, Tmpv, FPVI10_10V, FPVI10_100MA, RELAY_ON);
		delay_us(100);

		pviSBU1.MeasureVI(10, 10);
		for (site = 0; site<SITENUM; site++)	{
			vSBU1out[site] = pviSBU1.GetMeasResult(site, MVRET);

			if ((flag[site] == 0) && (vSBU1out[site] <= 1.0f))	{
				vUVLOL[site] = Tmpv;
				vUVLOHys[site] = vUVLOH[site] - vUVLOL[site];
				flag[site] = 1;
			}
		}
		if ((flag[0] == 1) && (flag[1] == 1))
			break;
		Tmpv -= 0.005f;
	}
	for (site = 0; site<SITENUM; site++)	{
		VPER_UVLO_Rising	->SetTestResult(site, 0, vUVLOH[site]);
		VPWR_UVLO_Falling	->SetTestResult(site, 0, vUVLOL[site]);
		VPWR_UVLO_VHYS		->SetTestResult(site, 0, vUVLOHys[site]*1.0e3f);
	}

	pwrZero();
	pwrSW_off();
	rlyC.SetOn(-1);
	delay_ms(1);
	
	return 0;
}
// test14
// ******************************************************************* VPWR LowV *******************************************************************
// ******************************************************************* VPWR LowV *******************************************************************
// ******************************************************************* VPWR LowV *******************************************************************
DUT_API int VPWR_LowVoltage(short funcindex, LPCTSTR funclabel)	{
	//{{AFX_STS_PARAM_PROTOTYPES
    CParam *VDEAD = StsGetParam(funcindex,"VDEAD");
    CParam *VDEAD2 = StsGetParam(funcindex,"VDEAD2");
  //}}AFX_STS_PARAM_PROTOTYPES

  // TODO: Add your function code here
	double vPWR[SITENUM]={0.0f}, iLoading = 10e-3f, vtemp[SITENUM] = { 0.0f, 0.0f };

	for(site = 0; site < SITENUM; site++)
		flag[site] = 1;
  StsGetSiteStatus(sitesta, SITENUM);
	for(site = 0; site < SITENUM; site++)	{
  	if(sitesta[site])	flag[site] = 0;	
		else	flag[site] = 1;
	}

	// VPWR, V33E, VBUS
	pviVPWR.Set(FI, 0.0e-12f, FPVI10_10V, FPVI10_100MA, RELAY_ON);
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	oviVBUS.Set(FV, 0.0f, FOVI_50V, FOVI_100MA, RELAY_ON);
	delay_ms(1);
	// ********************* VBUS SET 4V ********************* 14-1
	// ********************* VBUS SET 4V ********************* 14-1
	rlyC.SetOn(VPWR_Cap, V33E_Cap, V33E_Fovi, VPWR_Fpvi, VBUS_Fovi, -1);
	delay_ms(1);
	oviVBUS.Set(FV, 4.0f, FOVI_50V, FOVI_100MA, RELAY_ON);
	delay_ms(5);
	pviVPWR.Set(FI, -iLoading, FPVI10_10V, FPVI10_100MA, RELAY_ON);
	delay_ms(1);
		
	oviVBUS.MeasureVI(10, 10);
	for(site = 0; site < SITENUM; site++){
		vtemp[site] = oviVBUS.GetMeasResult(site, MVRET);
		if(vtemp[site] > 3.9f && vtemp[site] < 5.0f)	{
			pviVPWR.MeasureVI(10, 10);
			VDEAD->SetTestResult(site, 0, pviVPWR.GetMeasResult(site, MVRET));
		}
		else	{
			VDEAD->SetTestResult(site, 0, vtemp[site]*1000);
		}
	}
	// ********************* VBUS SET 36V ********************* 14-2
	// ********************* VBUS SET 36V ********************* 14-2
	oviVBUS.Set(FV, 36.0f, FOVI_50V, FOVI_100MA, RELAY_ON);
	delay_us(100);

	oviVBUS.MeasureVI(10, 10);
	for(site = 0; site < SITENUM; site++){
		vtemp[site] = oviVBUS.GetMeasResult(site, MVRET);
		if(vtemp[site] > 35.9f && vtemp[site] < 37.0f)	{
			pviVPWR.MeasureVI(10, 10);
			VDEAD2->SetTestResult(site, 0, pviVPWR.GetMeasResult(site, MVRET));
		}
		else	{
			VDEAD2->SetTestResult(site, 0, vtemp[site]*1000);
		}
	}
	pwrZero();
	pwrSW_off();
	rlyC.SetOn(-1);
	delay_ms(1);
	
	return 0;
}
// test15
// ******************************************************************* VPWR CurrentLimit *******************************************************************
// ******************************************************************* VPWR CurrentLimit *******************************************************************
// ******************************************************************* VPWR CurrentLimit *******************************************************************
DUT_API int VPWR_CurrentLimit(short funcindex, LPCTSTR funclabel)	{
	//{{AFX_STS_PARAM_PROTOTYPES
    CParam *CurrentLimit = StsGetParam(funcindex,"CurrentLimit");
  //}}AFX_STS_PARAM_PROTOTYPES

  // TODO: Add your function code here
	double vPWR[SITENUM] = { 0.0f }, iCL[SITENUM] = { 0.0f };

	for(site = 0; site < SITENUM; site++)
		flag[site] = 1;
  StsGetSiteStatus(sitesta, SITENUM);
	for(site = 0; site < SITENUM; site++)	{
  	if(sitesta[site])	flag[site] = 0;	
		else	flag[site] = 1;
	}

	// VPWR, V33E, VBUS
	pviVPWR.Set(FI, 0.0e-12f, FPVI10_10V, FPVI10_100MA, RELAY_ON);
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	oviVBUS.Set(FV, 0.0f, FOVI_50V, FOVI_100MA, RELAY_ON);
	delay_ms(1);

	rlyC.SetOn(VPWR_Cap, V33E_Cap, V33E_Fovi, VPWR_Fpvi, VBUS_Fovi, -1);
	delay_ms(1);
	oviVBUS.Set(FV, 5.0f, FOVI_50V, FOVI_100MA, RELAY_ON);
	delay_ms(5);

	// VPWR
	pviVPWR.Set(FI, -20.0e-3f, FPVI10_10V, FPVI10_100MA, RELAY_ON);
  delay_ms(1);
	pviVPWR.Set(FI, -25.0e-3f, FPVI10_10V, FPVI10_100MA, RELAY_ON);
  delay_us(100);
	pviVPWR.Set(FI, -30.0e-3f, FPVI10_10V, FPVI10_100MA, RELAY_ON);
  delay_us(100);

 	for(Tmpv = 30.0f; Tmpv < 75.0f;)	{
		pviVPWR.Set(FI, float(-1e-3 * Tmpv), FPVI10_10V, FPVI10_100MA, RELAY_ON);
  	delay_us(300);

	  pviVPWR.MeasureVI(20, 10);
		for(site = 0; site < SITENUM; site++)	{
			vPWR[site] = pviVPWR.GetMeasResult(site, MVRET);

  		if((flag[site] == 0) && (vPWR[site] < 1.0f))	{
				iCL[site] = pviVPWR.GetMeasResult(site, MIRET);
  			flag[site] = 1;
  		}
  		if((flag[0] == 1) && (flag[1] == 1))
				break;
  	}
  	Tmpv += 0.05f;
	}
 	for(site = 0; site < SITENUM; site++)
	  CurrentLimit->SetTestResult(site, 0, (fabs(iCL[site]*1e3) - 0.0f));

	pwrZero();
	pwrSW_off();
	rlyC.SetOn(-1);
	delay_ms(1);
 
	return 0;
}
// test16
// ******************************************************************* FLTB OUTPUT LOW *******************************************************************
// ******************************************************************* FLTB OUTPUT LOW *******************************************************************
// ******************************************************************* FLTB OUTPUT LOW *******************************************************************
DUT_API int FLT_LV_Test(short funcindex, LPCTSTR funclabel)	{
	//{{AFX_STS_PARAM_PROTOTYPES
    CParam *FLT_VOL = StsGetParam(funcindex,"FLT_VOL");
  //}}AFX_STS_PARAM_PROTOTYPES

	// TODO: Add your function code here
	for(site = 0; site < SITENUM; site++)
		flag[site] = 1;
  StsGetSiteStatus(sitesta, SITENUM);
	for(site = 0; site < SITENUM; site++)	{
  	if(sitesta[site])	flag[site] = 0;	
		else	flag[site] = 1;
	}
	// V33E
	oviFLTB.Set(FI, 0.0e-12f, FOVI_5V, FOVI_10MA, RELAY_ON);
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	pviC_CC1.Set(FV, 0.0f, FPVI10_10V, FPVI10_10MA, RELAY_ON);
	delay_ms(1);

	rlyC.SetOn(FLTB_100K, VPWR_Cap, V33E_Cap, C_CC1_Fpvi, V33E_Fovi, FLTB_Fovi, -1);
	delay_ms(1);
	oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(6);
	pviC_CC1.Set(FV, 8.0f, FPVI10_10V, FPVI10_10MA, RELAY_ON);
	delay_ms(1);

	// Sink 3mA
	oviFLTB.Set(FI, 3.0e-3f, FOVI_5V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	oviFLTB.MeasureVI(100, 10);
 	
	for(site = 0; site < SITENUM; site++)
		FLT_VOL->SetTestResult(site, 0, oviFLTB.GetMeasResult(site, MVRET));
	
	pwrZero();
	pwrSW_off();
	rlyC.SetOn(-1);
	delay_ms(1);
	
	return 0;
}

// test17
// ******************************************************************* TOVP_FLT ASSERTION *******************************************************************
// ******************************************************************* TOVP_FLT ASSERTION *******************************************************************
// ******************************************************************* TOVP_FLT ASSERTION *******************************************************************
DUT_API int TOVP_test(short funcindex, LPCTSTR funclabel)	{
	//{{AFX_STS_PARAM_PROTOTYPES
    CParam *TOVP_FLT_ASSERTION = StsGetParam(funcindex,"TOVP_FLT_ASSERTION");
    CParam *TOVP_FLT_DEASSERTION = StsGetParam(funcindex,"TOVP_FLT_DEASSERTION");
  //}}AFX_STS_PARAM_PROTOTYPES
  // TODO: Add your function code here

	// ************************************ TOVP FLT ASSERTION ************************************ 17-1
	// ************************************ TOVP FLT ASSERTION ************************************ 17-1
	// C_CC1 to FLTB (0A->0B), Rising to Falling, TOVP_FLT_ASSERTION, FLTB from High to Low
	double FLT_Down_T[SITENUM] = { 0.0f }, FLT_Up_T[SITENUM] = { 0.0f };

	// FLTB, V33E
	oviFLTB.Set(FI, 0.0e-12f, FOVI_5V, FOVI_10MA, RELAY_ON);
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	pviC_CC1.Set(FV, 0.0f, FPVI10_10V, FPVI10_10MA, RELAY_ON);
	delay_ms(1);

  rlyC.SetOn(V33E_Cap, VPWR_Cap, C_CC1_Fpvi, V33E_Fovi, FLTB_Fovi, C_CC1_QTMU_A, FLTB_QTMU_B, RPD1_Fovi, RPD2_Fovi, -1);
	delay_ms(1);
	qtmu0.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
  qtmu0.SetStopInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
  qtmu0.SetStartTrigger(6.5f,QTMU_PLUS_POS_SLOPE); 
  qtmu0.SetStopTrigger(0.3f,QTMU_PLUS_NEG_SLOPE);
 	qtmu0.SetInSource(QTMU_PLUS_DUAL_SOURCE);
  qtmu0.Connect();
  delay_ms(1);
	// RPD1, RPD2 SET 0V
	oviRPD1.Set(FV, 0.0f, FOVI_2V, FOVI_10MA, RELAY_ON);
	oviRPD2.Set(FV, 0.0f, FOVI_2V, FOVI_10MA, RELAY_ON);
	// FLTB, V33E SET 3.3
  oviFLTB.Set(FV, 3.3f, FOVI_5V, FOVI_100UA, RELAY_ON);
	oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(5);
	// C_CC1 FROM 1.5V
	pviC_CC1.Set(FV, 1.5f, FPVI10_10V, FPVI10_10MA, RELAY_ON);
	delay_ms(2);
	
	qtmu0.SetSinglePulseMeas(QTMU_PLUS_COARSE, QTMU_PLUS_TRNG_US);   
  qtmu0.SetTimeOut(10);
	for(site = 0; site < SITENUM; site++)  {
	  pviC_CC1.Set(FV, 8.0f, FPVI10_10V, FPVI10_10MA, RELAY_ON);
    qtmu0.SinglePlsMeas(site);
    FLT_Down_T[site]=qtmu0.GetMeasureResult(site);
 	}

	// ************************************ TOVP FLT DEASSERTION ************************************ 17-2
	// ************************************ TOVP FLT DEASSERTION ************************************ 17-2
	// C_CC1 to FLTB (0A->0B), Falling to Rising, TOVP_FLT_DEASSERTION, FLTB from Low to High
	pviC_CC1.Set(FV, 8.0f, FPVI10_10V, FPVI10_10MA, RELAY_ON);
	delay_ms(1);
  qtmu0.SetStartTrigger(6.5f,QTMU_PLUS_NEG_SLOPE); 
  qtmu0.SetStopTrigger(0.3f,QTMU_PLUS_POS_SLOPE); 
  delay_ms(3);
	
	qtmu0.SetSinglePulseMeas(QTMU_PLUS_COARSE, QTMU_PLUS_TRNG_US);   
  qtmu0.SetTimeOut(10);
	for(site = 0; site < SITENUM; site++)	{
		pviC_CC1.Set(FV, 0.0f, FPVI10_10V, FPVI10_10MA, RELAY_ON);
    qtmu0.SinglePlsMeas(site);
	  FLT_Up_T[site]=qtmu0.GetMeasureResult(site);
 	}
 	for(site = 0; site < SITENUM; site++)	{
		TOVP_FLT_ASSERTION->SetTestResult(site, 0, (FLT_Down_T[site] - 7.5f));             // offset
  	TOVP_FLT_DEASSERTION->SetTestResult(site, 0, ((FLT_Up_T[site]*1.0e-3f) - 0.0f));   // offset
 	}
	pwrZero();
	pwrSW_off();
	rlyC.SetOn(-1);
	delay_ms(1);
 
	return 0;

}
// test18
// ******************************************************************* TonFET and Dead Battery *******************************************************************
// ******************************************************************* TonFET and Dead Battery *******************************************************************
// ******************************************************************* TonFET and Dead Battery *******************************************************************
DUT_API int CC_Timing_Test1(short funcindex, LPCTSTR funclabel)	{
  //{{AFX_STS_PARAM_PROTOTYPES
    CParam *Ton_FET = StsGetParam(funcindex,"Ton_FET");
    CParam *Ton_FET_DB = StsGetParam(funcindex,"Ton_FET_DB");
  //}}AFX_STS_PARAM_PROTOTYPES

  // TODO: Add your function code here

	// TODO: Add your function code here
	double CC1O_Down_T[SITENUM] = { 0.0f }, CC1O_Up_T[SITENUM] = { 0.0f };
	double CC2O_Down_T[SITENUM] = { 0.0f }, CC2O_Up_T[SITENUM] = { 0.0f };
	double CC1I_C[SITENUM] = {0.0f}, CC1I_tmp_C[SITENUM][2000] = { {0.0f}, {0.0f} }, CC1I_C_T[SITENUM] = {0.0f};
  int inx=0;

	// *************************************************** CC1 ***************************************************
	// *************************************************** CC1 ***************************************************
	// V33E(0A) TO CC1(0B), SWAP QTMU
	rlyC.SetOn(FLTB_100K, VPWR_Cap, V33E_Cap, C_CC1_Fpvi, V33E_Fovi, CC1_qtmuA, V33E_QTMU_B, QTMU_Swap, RPD1_Fovi, -1);
	delay_ms(1);

  qtmu0.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_5V, QTMU_PLUS_FILTER_PASS);
  qtmu0.SetStopInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_5V, QTMU_PLUS_FILTER_PASS);
  qtmu0.SetStartTrigger(2.5f,QTMU_PLUS_POS_SLOPE); 
  qtmu0.SetStopTrigger(0.8f,QTMU_PLUS_POS_SLOPE); 
 	qtmu0.SetInSource(QTMU_PLUS_DUAL_SOURCE);
  qtmu0.Connect();
  delay_ms(1);
	// ****************************** TON1(CC1) FET ******************************
	// C_CC1, C_CC2, RPD1, RPD2 SET 1.5
	pviC_CC1.Set(FV, 1.5f, FPVI10_10V, FPVI10_10MA, RELAY_ON);
	oviRPD1.Set(FV, 1.5f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(10);

	qtmu0.SetSinglePulseMeas(QTMU_PLUS_COARSE, QTMU_PLUS_TRNG_US);   
  qtmu0.SetTimeOut(10);
	for(site = 0; site < SITENUM; site++)  {
		oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_100MA, RELAY_ON);
    qtmu0.SinglePlsMeas(site);
    CC1O_Up_T[site]=qtmu0.GetMeasureResult(site);
 	}
  // ****************************** TON1(CC1) FET DeadBattery ****************************** 18-2
  // ****************************** TON1(CC1) FET DeadBattery ****************************** 18-2
  // ****************************** TON1(CC1) FET DeadBattery ****************************** 18-2
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	pviC_CC1.Set(FI, -0.01e-6f, FPVI10_10V, FPVI10_10MA, RELAY_SENSE_ON);
	delay_ms(1);

	rlyC.SetOn(FLTB_100K, VPWR_Cap, V33E_Cap, C_CC1_Fpvi, V33E_Fovi, C_CC1_QTMU_A, V33E_QTMU_B, QTMU_Swap, RPD1_Fovi, -1);
	delay_ms(1);
  qtmu0.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_5V, QTMU_PLUS_FILTER_PASS);
  qtmu0.SetStopInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_5V, QTMU_PLUS_FILTER_PASS);
  qtmu0.SetStartTrigger(2.5f,QTMU_PLUS_POS_SLOPE); 
  qtmu0.SetStopTrigger(0.07f,QTMU_PLUS_POS_SLOPE);		// EWT#4
	////qtmu0.SetStopTrigger(0.1f,QTMU_PLUS_POS_SLOPE);			// EWT#3
  delay_ms(1);	
	qtmu0.SetSinglePulseMeas(QTMU_PLUS_COARSE, QTMU_PLUS_TRNG_US);   
  qtmu0.SetTimeOut(10);
	for(site = 0; site < SITENUM; site++)  {
		oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_100MA, RELAY_ON);
    qtmu0.SinglePlsMeas(site);
    CC1O_Down_T[site]=qtmu0.GetMeasureResult(site);
 	}

  for(site = 0; site < SITENUM; site++)	{
	  Ton_FET			->SetTestResult(site, 0, CC1O_Up_T[site]*1.0e-3f);
	  Ton_FET_DB	->SetTestResult(site, 0, CC1O_Down_T[site]*1.0e-3f);
 	}
	pwrZero();
	pwrSW_off();
	rlyC.SetOn(-1);
	delay_ms(1);
 
	return 0;
}

// test19
// ******************************************************************* SBU Response and Recovery time ***********************************************************
// ******************************************************************* SBU Response and Recovery time ***********************************************************
// ******************************************************************* SBU Response and Recovery time ***********************************************************
DUT_API int CC_Timing_Test2(short funcindex, LPCTSTR funclabel)	{
  //{{AFX_STS_PARAM_PROTOTYPES
    CParam *TOVP_Response_CC = StsGetParam(funcindex,"TOVP_Response_CC");
    CParam *TOVP_Recovery_CC1_FET = StsGetParam(funcindex,"TOVP_Recovery_CC1_FET");
    CParam *TOVP_Recovery_CC1_DB = StsGetParam(funcindex,"TOVP_Recovery_CC1_DB");
    CParam *TOVP_Recovery_CC2_FET = StsGetParam(funcindex,"TOVP_Recovery_CC2_FET");
    CParam *TOVP_Recovery_CC2_DB = StsGetParam(funcindex,"TOVP_Recovery_CC2_DB");
  //}}AFX_STS_PARAM_PROTOTYPES

  // TODO: Add your function code here
	// CC1, CC2
	double Tovp_Recovery_CC1FET_T[SITENUM] = {0.0f}, Tovp_Recovery_CC1DB_T[SITENUM] = {0.0f};
	double Tovp_Recovery_CC2FET_T[SITENUM] = {0.0f}, Tovp_Recovery_CC2DB_T[SITENUM] = {0.0f};
	double Tovp_RESP_T[SITENUM] = {0.0f};
	
	double CC1I_C		[SITENUM] = { 0.0f }, CC1I_tmp_C[SITENUM][2000] = { {0.0f}, {0.0f} };
	double CC1I_C_T	[SITENUM] = { 0.0f };
  short inx=0;

	// 19-1, C_CC1(0A) -> CC1(0B)
	rlyC.SetOn(FPVI2_nonKelvin, VPWR_Cap, V33E_Cap, V33E_Fovi, C_CC1_QTMU_A, CC1_to_QTMU0B, FLTB_Fovi, RPD1_Fovi, C_CC1_Fpvi,  -1);
	delay_ms(1);
	// RPD1 = C_CC1 = 5V
	oviRPD1.Set(FV, 5.0f, FOVI_10V, FOVI_10MA, RELAY_ON);
	// C_CC1 input
	pviC_CC1.Set(FV, 5.0f, FPVI10_50V, FPVI10_100MA, RELAY_ON);
	delay_ms(1);
	// QTMU setting(C_CC1 to CC1, A->B)
  qtmu0.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
  qtmu0.SetStopInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
  qtmu0.SetStartTrigger(7.0f, QTMU_PLUS_POS_SLOPE);
  qtmu0.SetStopTrigger(0.5f, QTMU_PLUS_NEG_SLOPE);
 	qtmu0.SetInSource(QTMU_PLUS_DUAL_SOURCE);
  qtmu0.Connect();
  delay_ms(3);

	// FLTB = 3.3V
	oviFLTB.Set(FV, 3.3f, FOVI_5V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	// V33E = 3.3V
	oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	qtmu0.SetSinglePulseMeas(QTMU_PLUS_COARSE, QTMU_PLUS_TRNG_NS);   
  qtmu0.SetTimeOut(50);
	for(site = 0; site < SITENUM; site++)	{
		pviC_CC1.Set(FV, 8.0f, FPVI10_50V, FPVI10_1A, RELAY_ON);
		qtmu0.SinglePlsMeas(site);
		Tovp_RESP_T[site]=qtmu0.GetMeasureResult(site);
 	}

	// 19-2
	// *************************************** TOVP_Recovery_CC1_FET ***************************************
	rlyC.SetOn(/*FPVI2_nonKelvin,*/ VPWR_Cap, V33E_Cap, V33E_Fovi, C_CC1_QTMU_A, CC1_to_QTMU0B, C_CC1_Fpvi, FLTB_Fovi, RPD1_Fovi, -1);
	pviC_CC1.Set(FV, 24.0f, FPVI10_50V, FPVI10_10MA, RELAY_ON);
	oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(2);
  qtmu0.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
  qtmu0.SetStopInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
	qtmu0.SetStartTrigger(23.0f, QTMU_PLUS_NEG_SLOPE);
  qtmu0.SetStopTrigger(1.3f, QTMU_PLUS_POS_SLOPE);
 	qtmu0.SetInSource(QTMU_PLUS_DUAL_SOURCE);
  qtmu0.Connect();
  delay_ms(1);

	for(site = 0; site < SITENUM; site++ )	{
		qtmu0.SetSinglePulseMeas(QTMU_PLUS_COARSE, QTMU_PLUS_TRNG_US, site);   
    qtmu0.SetTimeOut(10);
	  pviC_CC1.Set(FV, 1.5f, FPVI10_50V, FPVI10_10MA, RELAY_ON);
    delay_us(100);	// wait 100us
    qtmu0.SinglePlsMeas(site);
		Tovp_Recovery_CC1FET_T[site] = qtmu0.GetMeasureResult(site);
		Tovp_Recovery_CC1FET_T[site] += 80e-6f; // offset
 	}
	qtmu0.Disconnect(); 
	// 19-3
	// *************************************** TOVP_Recovery_CC1_DB ***************************************
	rlyC.SetOn(VPWR_Cap, V33E_Cap, V33E_Fovi, C_CC1_Fpvi, FLTB_Fovi, RPD1_Fovi, -1);
	delay_ms(1);
	
	oviRPD1.Set(FV, 24.0f, FOVI_50V, FOVI_10MA, RELAY_ON);
	pviC_CC1.Set(FV, 24.0f, FPVI10_50V, FPVI10_10MA, RELAY_ON);
	delay_ms(3);

	pviC_CC1.Set(FV, 1.5f, FPVI10_50V, FPVI10_10MA, RELAY_ON);
	delay_us(100);
	pviC_CC1.MeasureVI(1000, 10);
  for(site = 0; site < SITENUM; site++)
		pviC_CC1.BlockRead(site, 0, 500, CC1I_tmp_C[site], MIRET);

	pviC_CC1.Set(FV, 0.0f, FPVI10_50V, FPVI10_10MA, RELAY_ON);
	delay_ms(1);

	for(site = 0; site < SITENUM; site++)
		flag[site] = 1;

  StsGetSiteStatus(sitesta, SITENUM);
  for(site = 0; site < SITENUM; site++)	{
		if(sitesta[site])	{
 			flag[site] = 0;	
 		}
	}
 	for(site = 0; site < SITENUM; site++)	{
		CC1I_C_T[site] = 0.0f;
		for (inx=0; inx < 1000; inx += 1)	{
			if ((flag[site] == 0) && (CC1I_tmp_C[site][inx] *1.0e6f) <= 180.0f)	{		// <150uA
				CC1I_C_T[site]=inx;
				flag[site] = 1;
			}
			if((flag[0] == 1) && (flag[1] == 1))
				break;
		}
    Tovp_Recovery_CC1DB_T[site]=(CC1I_C_T[site] * 10.0f);
		Tovp_Recovery_CC1DB_T[site] += -90.0e-6f; // offset
	}

	// 19-4
	// *************************************** TOVP_Recovery_CC1_FET2 ***************************************
	rlyC.SetOn(VPWR_Cap, V33E_Cap, V33E_Fovi, C_CC1_QTMU_A, CC1_to_QTMU0B, C_CC1_Fpvi, V33E_Cap, FLTB_Fovi, RPD1_Fovi, -1);
	pviC_CC1.Set(FV, 24.0f, FPVI10_50V, FPVI10_10MA, RELAY_ON);
	oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(2);
  qtmu0.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
  qtmu0.SetStopInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_25V, QTMU_PLUS_FILTER_PASS);
	qtmu0.SetStartTrigger(20.0f, QTMU_PLUS_NEG_SLOPE);
  qtmu0.SetStopTrigger(0.03f, QTMU_PLUS_POS_SLOPE);
 	qtmu0.SetInSource(QTMU_PLUS_DUAL_SOURCE);
  qtmu0.Connect();
  delay_ms(1);

	for(site = 0; site < SITENUM; site++ )	{
		qtmu0.SetSinglePulseMeas(QTMU_PLUS_COARSE, QTMU_PLUS_TRNG_US, site);   
    qtmu0.SetTimeOut(10);
	  pviC_CC1.Set(FV, 1.5f, FPVI10_50V, FPVI10_10MA, RELAY_ON);
    delay_ms(10);	// wait 1ms
    qtmu0.SinglePlsMeas(site);
		Tovp_Recovery_CC2FET_T[site]=qtmu0.GetMeasureResult(site);
		Tovp_Recovery_CC2FET_T[site] += 80.0e-6f;  // offset
 	}
	qtmu0.Disconnect(); 
  // 19-5
	// *************************************** TOVP_Recovery_CC1_DB2 ***************************************
	rlyC.SetOn(VPWR_Cap, V33E_Cap, V33E_Fovi, C_CC1_Fpvi, FLTB_Fovi, RPD1_Fovi, -1);
	delay_ms(1);
	oviRPD1.Set(FV, 24.0f, FOVI_50V, FOVI_10MA, RELAY_ON);
	pviC_CC1.Set(FV, 24.0f, FPVI10_50V, FPVI10_10MA, RELAY_ON);
	delay_ms(3);

	pviC_CC1.Set(FV, 1.5f, FPVI10_50V, FPVI10_10MA, RELAY_ON);
	delay_us(100);
	pviC_CC1.MeasureVI(1000, 10);
  for(site = 0; site < SITENUM; site++)
		pviC_CC1.BlockRead(site, 0, 500, CC1I_tmp_C[site], MIRET);

	pviC_CC1.Set(FV, 0.0f, FPVI10_50V, FPVI10_10MA, RELAY_ON);
	delay_ms(1);

	for(site = 0; site < SITENUM; site++)
		flag[site] = 1;

  StsGetSiteStatus(sitesta, SITENUM);
  for(site = 0; site < SITENUM; site++)	{
		if(sitesta[site])	{
 			flag[site] = 0;	
 		}
	}
 	for(site = 0; site < SITENUM; site++)	{
		CC1I_C_T[site] = 0.0f;
		for (inx=0; inx < 1000; inx += 1)	{
			if ((flag[site] == 0) && (CC1I_tmp_C[site][inx] *1.0e6f) <= 180.0f)	{	// 150uA
				CC1I_C_T[site]=inx;
				flag[site] = 1;
			}
			if((flag[0] == 1) && (flag[1] == 1))
				break;
		}
    Tovp_Recovery_CC2DB_T[site]=(CC1I_C_T[site] * 10.0f);
		Tovp_Recovery_CC2DB_T[site] += 0e-6f; // offset
	}
	for(site = 0; site < SITENUM; site++)	{
		TOVP_Response_CC			->SetTestResult(site, 0, (Tovp_RESP_T[site]							*1.0e-0f + (-0.0)));	// 19-1
    TOVP_Recovery_CC1_FET	->SetTestResult(site, 0, (Tovp_Recovery_CC1FET_T[site]	*1.0e-3f + (-0.0)));	// 19-2
    TOVP_Recovery_CC1_DB	->SetTestResult(site, 0, (Tovp_Recovery_CC1DB_T[site]		*1.0e-3f + ( 0.3)));	// 19-3
		TOVP_Recovery_CC2_FET	->SetTestResult(site, 0, (Tovp_Recovery_CC2FET_T[site]	*1.0e-3f + (-0.13)));	// 19-4
    TOVP_Recovery_CC2_DB	->SetTestResult(site, 0, (Tovp_Recovery_CC2DB_T[site]		*1.0e-3f + (-0.0)));	// 19-5
 	}
	pwrZero();
	pwrSW_off();
	rlyC.SetOn(-1);
	delay_ms(1);
 
	return 0;
}

// test20
// ******************************************************************* SBU recovery time *******************************************************************
// ******************************************************************* SBU recovery time *******************************************************************
// ******************************************************************* SBU recovery time *******************************************************************
DUT_API int SBU_Timing_Test(short funcindex, LPCTSTR funclabel)	{
  //{{AFX_STS_PARAM_PROTOTYPES
    CParam *TOVP_Response_SBU = StsGetParam(funcindex,"TOVP_Response_SBU");
    CParam *TOVP_Recovery_SBU1 = StsGetParam(funcindex,"TOVP_Recovery_SBU1");
    CParam *TOVP_Recovery_SBU2 = StsGetParam(funcindex,"TOVP_Recovery_SBU2");
  //}}AFX_STS_PARAM_PROTOTYPES

  // TODO: Add your function code here
	// C_SBU1 Rising(T1), SBU1 Falling(T2), positive edge to negative edge
	// C_SBU1 Falling(T3), SBU1 Rising(T4), negative edge to positive edge
	// C_SBU1 Falling(T5), SBU1 Rising(T6), negative edge to positive edge
  double Tovp_RESP_SBU_T[SITENUM] = { 0.0f }, Tovp_Recovery_SBU1_T[SITENUM] = { 0.0f }, Tovp_Recovery_SBU2_T[SITENUM] = { 0.0f };

  rlyC.SetOn(-1);
	delay_ms(1);
	rlyC.SetOn(FPVI2_nonKelvin, VPWR_Cap, FLTB_100K, V33E_Cap, V33E_Fovi, FLTB_qtmuAB_SBU1, RPD1_Fovi, RPD2_Fovi, -1);
	delay_ms(1);
	qtmu0.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_5V, QTMU_PLUS_FILTER_PASS);
  qtmu0.SetStopInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_5V, QTMU_PLUS_FILTER_PASS);
	qtmu0.SetStartTrigger(2.7f,QTMU_PLUS_POS_SLOPE);
  qtmu0.SetStopTrigger(1.4f,QTMU_PLUS_NEG_SLOPE);
 	qtmu0.SetInSource(QTMU_PLUS_DUAL_SOURCE);
  qtmu0.Connect();
	delay_us(100);
	// C_SBU1
	pviC_SBU1.Set(FV, 0.0f, FPVI10_50V, FPVI10_10MA, RELAY_ON);
	delay_ms(1);
	// V33E SET 3.3
	oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(2);
	// RPD1, RPD2 SET 0V
	oviRPD1.Set(FV, 0.0f, FOVI_2V, FOVI_10MA, RELAY_ON);
	oviRPD2.Set(FV, 0.0f, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	// ******************************* SBU1 OVP Response Time, C_SBU1(P) TO SBU1(N) ******************************* 20-1
	// ******************************* SBU1 OVP Response Time, C_SBU1(P) TO SBU1(N) ******************************* 20-1
	pviC_SBU1.Set(FV, 24.0f, FPVI10_50V, FPVI10_10MA, RELAY_ON);
	delay_ms(1);

	qtmu0.SetSinglePulseMeas(QTMU_PLUS_FINE, QTMU_PLUS_TRNG_NS);   
  qtmu0.SetTimeOut(10);
	for(site = 0; site < SITENUM; site++)	{
		rlyC.SetOn(FPVI2_nonKelvin, VPWR_Cap, FLTB_100K, V33E_Cap, V33E_Fovi, FLTB_qtmuAB_SBU1, RPD1_Fovi, RPD2_Fovi, C_SBU1_Fpvi, -1);	// C_SBU1(0A) TO SBU1(0B)
    qtmu0.SinglePlsMeas(site);
    Tovp_RESP_SBU_T[site]=qtmu0.GetMeasureResult(site);
 	}
	// ******************************* SBU1 OVP Recovery Time1, C_SBU1(N) TO SBU1(P) ******************************* 20-2
	// ******************************* SBU1 OVP Recovery Time1, C_SBU1(N) TO SBU1(P) ******************************* 20-2
	pviC_SBU1.Set(FV, 24.0f, FPVI10_50V, FPVI10_1A, RELAY_ON);
	delay_ms(3);

  qtmu0.SetStartInput(QTMU_PLUS_IMPEDANCE_1M,QTMU_PLUS_VRNG_25V,QTMU_PLUS_FILTER_10MHz);
  qtmu0.SetStopInput(QTMU_PLUS_IMPEDANCE_1M,QTMU_PLUS_VRNG_5V,QTMU_PLUS_FILTER_10MHz);
  qtmu0.SetStartTrigger(22.0f,QTMU_PLUS_NEG_SLOPE); 
  qtmu0.SetStopTrigger(1.1f,QTMU_PLUS_POS_SLOPE); 

	qtmu0.SetSinglePulseMeas(QTMU_PLUS_COARSE, QTMU_PLUS_TRNG_US);   
  qtmu0.SetTimeOut(10);
	for(site = 0; site < SITENUM; site++)	{
	  pviC_SBU1.Set(FV, 1.5f, FPVI10_50V, FPVI10_1A, RELAY_ON, 0.5);
    delay_us(100);	// condition1
    qtmu0.SinglePlsMeas(site);
		Tovp_Recovery_SBU1_T[site] = qtmu0.GetMeasureResult(site);
		Tovp_Recovery_SBU1_T[site] += -48.0f; // offset
 	}
	// ******************************* SBU1 OVP Recovery Time2, C_SBU1(N) TO SBU1(P) ******************************* 20-3
	// ******************************* SBU1 OVP Recovery Time2, C_SBU1(N) TO SBU1(P) ******************************* 20-3
	pviC_SBU1.Set(FV, 24.0f, FPVI10_50V, FPVI10_1A, RELAY_ON);
	delay_ms(3);

  qtmu0.SetStartInput(QTMU_PLUS_IMPEDANCE_1M,QTMU_PLUS_VRNG_25V,QTMU_PLUS_FILTER_10MHz);
  qtmu0.SetStopInput(QTMU_PLUS_IMPEDANCE_1M,QTMU_PLUS_VRNG_5V,QTMU_PLUS_FILTER_10MHz);
  qtmu0.SetStartTrigger(22.0f,QTMU_PLUS_NEG_SLOPE); 
  qtmu0.SetStopTrigger(1.1f,QTMU_PLUS_POS_SLOPE); 

	qtmu0.SetSinglePulseMeas(QTMU_PLUS_COARSE, QTMU_PLUS_TRNG_US);   
  qtmu0.SetTimeOut(10);
	for(site = 0; site < SITENUM; site++)  {
	  pviC_SBU1.Set(FV, 1.5f, FPVI10_50V, FPVI10_1A, RELAY_ON);
    delay_ms(1);	// condition2
    qtmu0.SinglePlsMeas(site);
    Tovp_Recovery_SBU2_T[site] = qtmu0.GetMeasureResult(site);
		Tovp_Recovery_SBU2_T[site] += -82.0f; // offset
 	}
	//============================================================================
  for(site = 0; site < SITENUM; site++)	{
		TOVP_Response_SBU->SetTestResult(site, 0, (Tovp_RESP_SBU_T[site]*1.0e3f));	// to ns
    TOVP_Recovery_SBU1->SetTestResult(site, 0, (Tovp_Recovery_SBU1_T[site]));
    TOVP_Recovery_SBU2->SetTestResult(site, 0, (Tovp_Recovery_SBU2_T[site]));
 	}
	pwrZero();
	pwrSW_off();
	rlyC.SetOn(-1);
	delay_ms(1);

	return 0;
}
// test21
// ******************************************************************* Reference voltage *******************************************************************
// ******************************************************************* Reference voltage *******************************************************************
// ******************************************************************* Reference voltage *******************************************************************
DUT_API int Reference_Voltage(short funcindex, LPCTSTR funclabel)	{
	//{{AFX_STS_PARAM_PROTOTYPES
    CParam *VREF = StsGetParam(funcindex,"VREF");
  //}}AFX_STS_PARAM_PROTOTYPES

  // TODO: Add your function code here
	double vRef[SITENUM] = { 0.0f };

	for(site = 0; site < SITENUM; site++)
		flag[site] = 1;
  StsGetSiteStatus(sitesta, SITENUM);
	for(site = 0; site < SITENUM; site++)	{
  	if(sitesta[site])	flag[site] = 0;	
		else	flag[site] = 1;
	}
	oviV33E.Set(FV, 0.0f, FOVI_10V, FOVI_100MA, RELAY_ON);
	oviFLTB.Set(FV, 0.0f, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	rlyC.SetOn(VPWR_Cap, V33E_Cap, V33E_Fovi, FLTB_Fovi, -1);
	delay_ms(1);

	// ************************ Test Mode Setting ************************
	// ************************ Test Mode Setting ************************
	oviV33E.Set(FV, 3.3f, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(10);
	oviFLTB.Set(FV, -1.0f, FOVI_2V, FOVI_100MA, RELAY_ON);
	delay_ms(1);
	// TM2(Reference), set FLTB = 1.5V
	oviFLTB.Set(FV, 1.5f, FOVI_2V, FOVI_100MA, RELAY_ON);
	delay_ms(1);
	// Latch
	oviFLTB.Set(FV, 1.5f, FOVI_2V, FOVI_100MA, RELAY_OFF);
	delay_ms(1);
	// Measurement 
	oviFLTB.Set(FI, 0.1e-6f, FOVI_2V, FOVI_10UA, RELAY_ON);
	delay_ms(10);

	oviFLTB.MeasureVI(200, 10);
	for(site = 0; site < SITENUM; site++)	{
		vRef[site] = oviFLTB.GetMeasResult(site, MVRET);
	  VREF->SetTestResult(site, 0, vRef[site]);			 
	}
	pwrZero();
	pwrSW_off();
	rlyC.SetOn(-1);
	delay_ms(1);

  return 0;
}

