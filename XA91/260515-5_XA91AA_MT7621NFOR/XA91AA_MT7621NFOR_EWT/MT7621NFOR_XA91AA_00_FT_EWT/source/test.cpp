#include "stdafx.h"
#include "stdlib.h"
#include "math.h"

#define SITE_NUM 4
#define DDATA 1

FPVI10  pVIN_FPVI			(0, "VIN");
FPVI10  pOUT_FPVI			(1, "OUT");

FOVI    psEN    (1, "EN CTL");
FOVI    pEN     (2, "EN");
FOVI    pBIAS   (3, "BIAS");

CBIT128 cbit;
QTMU_PLUS qtmu0(0);

double adresult[SITE_NUM]={0};
// XA91_DB(XA81_TestBoard relay definition)
#define  VIN_CAP            0,  32, 64, 96      // K1, 2.2uF
#define  OUT_CAP            1,  33, 65, 97      // K2, 2.2uF
#define  BIAS_CAP           2,  34, 66, 98      // K3  0.1uF
#define  VIN_Zener          3,  35, 67, 99      // K4  Vz=5.3v(vin)
#define  REN_300KOhm        4,  36, 68, 100     // K5, fovi1 300K
#define  VIN_FPVI           5,  37, 69, 101     // K6
#define  OUT_FPVI           6,  38, 70, 102     // K7
#define  ENm_FOVI           9,  41, 73, 105     // K10
#define  BIAS_FOVI          10, 42, 74, 106     // K11
#define  Sweep_QTMU         13, 45, 77, 109     // K14
#define  EN_QTMU_B          14, 46, 78, 110     // K15
#define  OUT_QTMU_A         15, 47, 79, 111     // K16
// #define  OUT_FOVI           7,  39, 71, 103     // K8
// #define  ENs_FOVI           8,  40, 72, 104     // K9
// #define  FloatSource_GNG    11, 43, 75, 107     // K12  xxx
// #define  NC_FOVI            12, 44, 76, 108     // K13

//multisite settings should be included here
DUT_API void HardWareCfg()  {
	/*For example: four channels dvi to config two sites*/
	StsSetModuleToSite(MD_FOVI,SITE_1,  0,  1,  2,  3,  4,  5,  6,  7,  -1);
	StsSetModuleToSite(MD_FOVI,SITE_2,  8,  9,  10, 11, 12, 13, 14, 15, -1);
	StsSetModuleToSite(MD_FOVI,SITE_3,  16, 17, 18, 19, 20, 21, 22, 23, -1);
	StsSetModuleToSite(MD_FOVI,SITE_4,  24, 25, 26, 27, 28, 29, 30, 31, -1);
	
	StsSetModuleToSite(MD_FPVI10,SITE_1,    0, 1,  -1);
	StsSetModuleToSite(MD_FPVI10,SITE_2,    2, 3,  -1);
	StsSetModuleToSite(MD_FPVI10,SITE_3,    4, 5,  -1);
	StsSetModuleToSite(MD_FPVI10,SITE_4,    6, 7,  -1);

	StsSetModuleToSite(MD_QTMUPLUS,SITE_1,  0,  -1);
	StsSetModuleToSite(MD_QTMUPLUS,SITE_2,  1,  -1);
	StsSetModuleToSite(MD_QTMUPLUS,SITE_3,  2,  -1);
	StsSetModuleToSite(MD_QTMUPLUS,SITE_4,  3,  -1);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//initialize function will be called before all the test functions.
DUT_API void InitBeforeTestFlow()   {
	pEN.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
	pBIAS.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
	psEN.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
	pVIN_FPVI.Set(FV, 0, FPVI10_10V, FPVI10_100MA, RELAY_ON);
	pOUT_FPVI.Set(FV, 0, FPVI10_10V, FPVI10_100MA, RELAY_ON);
	delay_us(100);

	pEN.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_OFF);
	pBIAS.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_OFF);
	psEN.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_OFF);
	pVIN_FPVI.Set(FV, 0, FPVI10_10V, FPVI10_100MA, RELAY_OFF);
	pOUT_FPVI.Set(FV, 0, FPVI10_10V, FPVI10_100MA, RELAY_OFF);
	
	cbit.Init();	
	cbit.SetOn(-1);
	qtmu0.Init();
	delay_ms(1);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//initializefunction will be called after all the test functions.
DUT_API void InitAfterTestFlow()    {		
	cbit.SetOn(-1);
	qtmu0.Init();
	delay_ms(1);
}
// ************************************************* user definition start *************************************************
int site = 0;
void power_dwn(void)	{
	pEN.Set(FV, 0.0, FOVI_5V, 	FOVI_10MA, 	RELAY_ON);
	psEN.Set(FV, 0.0, FOVI_5V, 	FOVI_10MA, 	RELAY_ON);
	pBIAS.Set(FV, 0.0, FOVI_5V,	FOVI_10MA, 	RELAY_ON);
	pVIN_FPVI.Set(FV, 0.0, FPVI10_5V, FPVI10_10MA,	RELAY_ON);
	pOUT_FPVI.Set(FV, 0.0, FPVI10_5V, FPVI10_10MA,	RELAY_ON);
	delay_ms(1);

	pEN.Set(FV, 0.0, FOVI_5V, 	FOVI_10MA, 	RELAY_OFF);
	psEN.Set(FV, 0.0, FOVI_5V, 	FOVI_10MA, 	RELAY_OFF);
	pBIAS.Set(FV, 0.0, FOVI_5V,	FOVI_10MA, 	RELAY_OFF);
	pVIN_FPVI.Set(FV, 0.0, FPVI10_5V, FPVI10_10MA,	RELAY_OFF);
	pOUT_FPVI.Set(FV, 0.0, FPVI10_5V, FPVI10_10MA,	RELAY_OFF);
	delay_ms(1);
	qtmu0.Disconnect();
	cbit.SetOn(-1);
}
void PIN2GND(void)	{
	pEN.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
	pBIAS.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_ON);
	pVIN_FPVI.Set(FV, 0, FPVI10_10V, FPVI10_100MA, RELAY_ON);
	pOUT_FPVI.Set(FV, 0, FPVI10_10V, FPVI10_100MA, RELAY_ON);
	delay_us(100);
}
void PIN2OFF(void)	{
	pEN.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_OFF);
	pBIAS.Set(FV, 0, FOVI_10V, FOVI_100MA, RELAY_OFF);
	pVIN_FPVI.Set(FV, 0, FPVI10_10V, FPVI10_100MA, RELAY_OFF);
	pOUT_FPVI.Set(FV, 0, FPVI10_10V, FPVI10_100MA, RELAY_OFF);
	delay_us(100);
}
void POWER_ON(void)	{
	// BIAS = 5V
  pBIAS.Set(FV, float(3), FOVI_5V, FOVI_100MA, RELAY_ON);
  delay_us(500);
	pBIAS.Set(FV, float(3.1), FOVI_5V, FOVI_100MA, RELAY_ON);
  delay_us(500);
	pBIAS.Set(FV, float(3.3), FOVI_5V, FOVI_100MA, RELAY_ON);
  delay_ms(1);
  // VIN = 3.3V
	pVIN_FPVI.Set(FV, float(0.1), FPVI10_2V, FPVI10_1A, RELAY_ON);
  delay_ms(2);
	pVIN_FPVI.Set(FV, float(0.2), FPVI10_2V, FPVI10_1A, RELAY_ON);
  delay_ms(2);
	pVIN_FPVI.Set(FV, float(0.3), FPVI10_2V, FPVI10_1A, RELAY_ON);
  delay_ms(2);
	pVIN_FPVI.Set(FV, float(0.5), FPVI10_2V, FPVI10_1A, RELAY_ON);
  delay_ms(2);
  pVIN_FPVI.Set(FV, float(0.6), FPVI10_2V, FPVI10_1A, RELAY_ON);
  delay_ms(2);
  pVIN_FPVI.Set(FV, float(0.7), FPVI10_2V, FPVI10_1A, RELAY_ON);
  delay_ms(1);
  pVIN_FPVI.Set(FV, float(0.8), FPVI10_2V, FPVI10_1A, RELAY_ON);
  delay_ms(2);//20
}
// ************************************************* user definition end *************************************************
/************************************************************************/
/*                                                                      */
/************************************************************************/
//Fail site hardware set function will be called after failed params, it can be called for serveral times. 
DUT_API void SetupFailSite(const unsigned char*byFailSite)
{			
}
/************************************************************************/
/*                                                                      */
DUT_API int preContinuity(short funcindex, LPCTSTR funclabel)   {
  //{{AFX_STS_PARAM_PROTOTYPES
  CParam *pre_VIN = StsGetParam(funcindex,"pre_VIN");
  CParam *pre_OUT = StsGetParam(funcindex,"pre_OUT");
  CParam *pre_EN = StsGetParam(funcindex,"pre_EN");
  CParam *pre_BIAS = StsGetParam(funcindex,"pre_BIAS");
  //}}AFX_STS_PARAM_PROTOTYPES

  // TODO: Add your function code here
  power_dwn();

	// RELAY SETTING
	cbit.SetOn(VIN_FPVI, OUT_FPVI, ENm_FOVI, BIAS_FOVI, -1);
	delay_ms(1);
	PIN2GND();
	delay_us(50);
	// *************************************** VIN(3) ***************************************
	pVIN_FPVI.Set(FI, float(-1.0), FPVI10_2V, FPVI10_1MA, RELAY_ON);
	delay_ms(1);
	pVIN_FPVI.MeasureVI(20, 10);
	for (site = 0; site < SITE_NUM; site++)
		pre_VIN->SetTestResult(site, 0, pVIN_FPVI.GetMeasResult(site, MVRET));
	pVIN_FPVI.Set(FV, 0.0, FPVI10_1V, FPVI10_1MA, RELAY_ON);
	// *************************************** VOUT(2) ***************************************
	pOUT_FPVI.Set(FI,float(-1.0), FPVI10_2V, FPVI10_1MA, RELAY_ON);
	delay_ms(1);
	pOUT_FPVI.MeasureVI(20, 10);
	for (site = 0; site < SITE_NUM; site++)
		pre_OUT->SetTestResult(site, 0, pOUT_FPVI.GetMeasResult(site, MVRET));
	pOUT_FPVI.Set(FV, 0.0, FPVI10_1V, FPVI10_1MA, RELAY_ON);
  // *************************************** EN(6) ***************************************
	pEN.Set(FI, float(-1.0), FOVI_2V, FOVI_1MA, RELAY_ON);
	delay_ms(1);
	pEN.MeasureVI(20, 10);
	for (site = 0; site < SITE_NUM; site++)
		pre_EN->SetTestResult(site, 0, pEN.GetMeasResult(site, MVRET));
	pEN.Set(FV, 0.0, FOVI_1V, FOVI_1MA, RELAY_ON);
    // *************************************** BIAS(4) ***************************************
	pBIAS.Set(FI, float(-1.0), FOVI_2V, FOVI_1MA, RELAY_ON);
	delay_ms(1);
	pBIAS.MeasureVI(20, 10);
	for (site = 0; site < SITE_NUM; site++)
		pre_BIAS->SetTestResult(site, 0, pBIAS.GetMeasResult(site, MVRET));
	pBIAS.Set(FV, 0.0, FOVI_1V, FOVI_1MA, RELAY_ON);
	PIN2OFF();

  return 0;
}


DUT_API int _2_SupplyCurrent_Test(short funcindex, LPCTSTR funclabel)   {
  //{{AFX_STS_PARAM_PROTOTYPES
  CParam *ISD_BIAS = StsGetParam(funcindex,"ISD_BIAS");
  CParam *ISD_IN = StsGetParam(funcindex,"ISD_IN");
  CParam *IQ_BIAS = StsGetParam(funcindex,"IQ_BIAS");
  CParam *IQ_VIN = StsGetParam(funcindex,"IQ_VIN");
  //}}AFX_STS_PARAM_PROTOTYPES

  // TODO: Add your function code here
  // RELAY SETTING
	float vbias_st = 0, vin_st = 0;
	float ISD_bias[SITE_NUM] = { 999 }, ISD_vin[SITE_NUM] = { 999 }, IQ_bias[SITE_NUM] = { 999 }, IQ_vin[SITE_NUM] = { 999 };
	
	cbit.SetOn(VIN_FPVI, BIAS_FOVI, ENm_FOVI, -1);//, VIN_CAP, OUT_CAP, BIAS_CAP, -1);
	delay_ms(1);

  // BIAS = 5V
  pBIAS.Set(FV, float(0), FOVI_10V, FOVI_100MA, RELAY_ON);

	for (vbias_st = 0; vbias_st <= float(3.3);) {// 240423-2 modified
    pBIAS.Set(FV, float(vbias_st), FOVI_5V, FOVI_100MA, RELAY_ON, 2);
    delay_us(300);
		vbias_st += float(300e-3f);// 240423-2 modified
  }

  // VIN = 3.3V
  pVIN_FPVI.Set(FV, float(0.2), FPVI10_5V, FPVI10_100MA, RELAY_ON, 2);
	for (vin_st = float(0.2); vin_st <= float(0.8);) {
    pVIN_FPVI.Set(FV, float(vin_st), FPVI10_5V, FPVI10_100MA, RELAY_ON, 2);
    delay_us(300);
		vin_st += float(100e-3);
  }
  pBIAS.Set(FV, float(3.3), FOVI_5V, FOVI_100UA, RELAY_ON);
  delay_ms(2);//10

  pVIN_FPVI.Set(FV, float(0.8), FPVI10_2V, FPVI10_100UA, RELAY_ON);
	delay_ms(5);
  pEN.Set(FV, float(0), FOVI_5V, FOVI_100UA, RELAY_ON);
  delay_ms(2);//200

  // ********************************************************************************* SHUTDOWN CURRENT
	pBIAS.MeasureVI(20, 10);
	for (site = 0; site < SITE_NUM; site++)	{
		ISD_bias[site] = fabs(pBIAS.GetMeasResult(site, MIRET)) * 1e6;
		ISD_BIAS->SetTestResult(site, 0, ISD_bias[site]);
	}

  pVIN_FPVI.MeasureVI(20, 10);
	for (site = 0; site < SITE_NUM; site++)	{
		ISD_vin[site] = fabs(pVIN_FPVI.GetMeasResult(site, MIRET)) * 1e6;
		ISD_IN->SetTestResult(site, 0, ISD_vin[site]);
	}
  
	pVIN_FPVI.Set(FV, float(0.8), FPVI10_2V, FPVI10_100MA, RELAY_ON);
	delay_ms(5);//50
	pVIN_FPVI.Set(FV, float(0.8), FPVI10_2V, FPVI10_100UA, RELAY_ON);
	delay_ms(5);

  // ********************************************************************************* QUIESCENT CURRENT
	cbit.SetOn(VIN_FPVI, BIAS_FOVI, BIAS_CAP, OUT_CAP, ENm_FOVI, -1);
	delay_ms(1);

  pBIAS.Set(FV, float(3.0), FOVI_5V, FOVI_100MA, RELAY_ON);

  // EN = 2V
  pEN.Set(FV, (float)1.8, FOVI_2V, FOVI_10MA, RELAY_ON);
	delay_ms(20);//100
	pBIAS.Set(FV, float(3.3), FOVI_5V, FOVI_100UA, RELAY_ON, 2);
	delay_ms(5);

  pBIAS.MeasureVI(1000, 2);
	for (site = 0; site < SITE_NUM; site++)	{
		IQ_bias[site] = (pBIAS.GetMeasResult(site, MIRET)) * 1e6;
		IQ_BIAS->SetTestResult(site, 0, IQ_bias[site]);
	}
	delay_ms(100);//200
    pVIN_FPVI.MeasureVI(1000, 10);
	for (site = 0; site < SITE_NUM; site++)	{
		IQ_vin[site] = (pVIN_FPVI.GetMeasResult(site, MIRET)) * 1e6;
		// 240417-3, add ofs 1.5uA
		// 240516-4, ofs 0.6uA
		IQ_vin[site] += float(0.6);
		IQ_VIN->SetTestResult(site, 0, IQ_vin[site]);
	}
	power_dwn();
	PIN2OFF();

  return 0;
}


DUT_API int _3_UVLO_Threshold_Test(short funcindex, LPCTSTR funclabel)  {
	//{{AFX_STS_PARAM_PROTOTYPES
	CParam *UVLO_Rising = StsGetParam(funcindex,"UVLO_Rising");
	CParam *UVLO_Falling = StsGetParam(funcindex,"UVLO_Falling");
	CParam *UVLO_VHYS = StsGetParam(funcindex,"UVLO_VHYS");
	CParam *VEN_CLP = StsGetParam(funcindex,"VEN_CLP");
	//}}AFX_STS_PARAM_PROTOTYPES

  // TODO: Add your function code here
  float uvlo_on[SITE_NUM] = { 0.0 }, uvlo_off[SITE_NUM] = { 0.0 }, uvlo_hys[SITE_NUM] = { 0.0 };
  float en_clamp[SITE_NUM] = { 0.0 };
	float vtrig = 0.6, voutj[SITE_NUM] = {0};
  float i= 0, flag[SITE_NUM] = { 0 };

	DWORD CurEnbSite = 0;
	BYTE sitesta[SITE_NUM];

	StsGetSiteStatus(sitesta, SITE_NUM);
	for(site = 0; site < SITE_NUM; site++)
		CurEnbSite += sitesta[site] << site;

	StsSetSiteStatus(CurEnbSite & 0x0f);
	for(site=0; site < SITE_NUM; site++)	{
		flag[site] = 1;
		if((CurEnbSite >> site) & 0x1)
			flag[site] = 0;
		else 
			flag[site] = 1;
	}

  // RELAY SETTING
	cbit.SetOn(VIN_FPVI, OUT_FPVI, BIAS_FOVI, VIN_CAP, BIAS_CAP, OUT_CAP, ENm_FOVI, -1);
	delay_ms(1);

  // WIN = 1.5V
  pVIN_FPVI.Set(FV, float(0.5), FPVI10_5V, FPVI10_100MA, RELAY_ON, 2);
  delay_us(500);
  pVIN_FPVI.Set(FV, float(0.8), FPVI10_5V, FPVI10_100MA, RELAY_ON, 2);
  delay_us(500);

  delay_ms(1);
  // EN = 2V
	pEN.Set(FV, (float)1.8, FOVI_5V, FOVI_100MA, RELAY_ON);
	delay_ms(1);

  // VOUT MEASUREMENT
	pOUT_FPVI.Set(FI, float(-0.1e-6), FPVI10_5V, FPVI10_100UA, RELAY_ON);
  pBIAS.Set(FV, (float)1, FOVI_5V, FOVI_100MA, RELAY_ON);
  delay_ms(1);

  // *********************************************************************** UVLO ON
	for (i = 1.5f; i <= 2.1f;)  {
		pBIAS.Set(FV, (float)i, FOVI_5V, FOVI_100MA, RELAY_ON);
    delay_ms(1);
	  pOUT_FPVI.MeasureVI(20, 10);

    for (site = 0; site < SITE_NUM; site++) {
			adresult[site] = pOUT_FPVI.GetMeasResult(site, MVRET);
      if( (adresult[site] > vtrig) && flag[site] == 0)  {
        uvlo_on[site] = i;
        flag[site] = 1;
			}
    }
    if(flag[0] && flag[1] && flag[2] && flag[3])
      break;
    i += float(3e-3);
   }
	// BIAS = 4V
	pBIAS.Set(FV, (float)4, FOVI_5V, FOVI_100MA, RELAY_ON);
    delay_ms(1);
	for(site=0; site < SITE_NUM; site++)	{
		pOUT_FPVI.MeasureVI(20, 10);
		voutj[site] = pOUT_FPVI.GetMeasResult(site, MVRET);
	}

  // *********************************************************************** UVLO OFF
	CurEnbSite = 0;
	StsGetSiteStatus(sitesta, SITE_NUM);
	for(site = 0; site < SITE_NUM; site++)
		CurEnbSite += sitesta[site] << site;

	StsSetSiteStatus(CurEnbSite & 0x0f);
	for(site=0; site < SITE_NUM; site++)	{
		flag[site] = 1;
		if((CurEnbSite >> site) & 0x1)
			flag[site] = 0;
		else 
			flag[site] = 1;
	}

  for (i = 2.0; i >= float(1.5);)  {
    pBIAS.Set(FV, (float)i, FOVI_5V, FOVI_100MA, RELAY_ON);
    delay_us(500);
		pOUT_FPVI.MeasureVI(20, 10);
        
    for (site = 0; site < SITE_NUM; site++) {
      adresult[site] = pOUT_FPVI.GetMeasResult(site, MVRET);

			if( (adresult[site] < float(0.2) && flag[site] == 0) )  {
        uvlo_off[site] = i;
        flag[site] = 1;
      }
    }
    if(flag[0] && flag[1] && flag[2] && flag[3])
			break;

		if(i<float(uvlo_on[site]))
			i += float(-5e-3);
		else
		    i += float(-5e-3);
  }

	// *********************************************************************** EN CLAMPING
	cbit.SetOn(VIN_FPVI, BIAS_FOVI, VIN_CAP, BIAS_CAP, ENm_FOVI, REN_300KOhm, -1);
	delay_ms(1);
    
	// BIAS = 5V
  pBIAS.Set(FV, (float)3.0, FOVI_5V, FOVI_100MA, RELAY_ON);
  delay_ms(1);
  pBIAS.Set(FV, (float)3.3, FOVI_5V, FOVI_100MA, RELAY_ON);
  delay_us(500);

  // EN CTRL = 3.3V
	pEN.Set(FI, (float)0.0, FOVI_5V, FOVI_100UA, RELAY_SENSE_ON);
	delay_ms(1);
	psEN.Set(FV, (float)1.8, FOVI_5V, FOVI_100MA, RELAY_ON);
	delay_ms(1);

  pEN.MeasureVI(20, 10);
  for (site = 0; site < SITE_NUM; site++) 
    en_clamp[site] = pEN.GetMeasResult(site, MVRET);
    
	// DATA LISTING
  for (site = 0 ; site < SITE_NUM; site++) {
    UVLO_Rising->SetTestResult(site, 0, uvlo_on[site]);
    UVLO_Falling->SetTestResult(site, 0, uvlo_off[site]);
    UVLO_VHYS->SetTestResult(site, 0, (uvlo_on[site] - uvlo_off[site]) * 1e3);
    VEN_CLP->SetTestResult(site, 0, en_clamp[site]);
  }
  power_dwn();
	PIN2OFF();

	return 0;
}


DUT_API int _4_RDSon_Test(short funcindex, LPCTSTR funclabel)   {
  //{{AFX_STS_PARAM_PROTOTYPES
  CParam *RDSON = StsGetParam(funcindex,"RDSON");
  //}}AFX_STS_PARAM_PROTOTYPES

  float loading[SITE_NUM] = { 0.0 }, vds[SITE_NUM] = { 0.0 }, rdson[SITE_NUM] = { 0.0 };
  float vd[SITE_NUM] = { 0 }, vs[SITE_NUM] = { 0 }, flag[SITE_NUM] = { 0 };
  // TODO: Add your function code here
  // RELAY SETTING
	cbit.SetOn(VIN_FPVI, OUT_FPVI, BIAS_FOVI, VIN_CAP, BIAS_CAP, OUT_CAP, ENm_FOVI, -1);
	delay_ms(1);

  // BIAS = 5V
  pBIAS.Set(FV, float(3.0), FOVI_5V, FOVI_100MA, RELAY_ON);
  delay_us(500);
	pBIAS.Set(FV, float(3.1), FOVI_5V, FOVI_100MA, RELAY_ON);
  delay_us(500);
	pBIAS.Set(FV, float(3.3), FOVI_5V, FOVI_100MA, RELAY_ON);
  delay_ms(1);
  // VIN = 3V
  pVIN_FPVI.Set(FV, float(0.5), FPVI10_5V, FPVI10_1A, RELAY_ON);
  delay_us(500);
  pVIN_FPVI.Set(FV, float(0.7), FPVI10_5V, FPVI10_1A, RELAY_ON);
  delay_us(500);
  pVIN_FPVI.Set(FV, float(0.8), FPVI10_5V, FPVI10_1A, RELAY_ON);
  delay_ms(1);
  // EN = 2V
	pEN.Set(FV, (float)1.8, FOVI_5V, FOVI_10MA, RELAY_ON);
	delay_ms(10);

  // VOUT SOURCE
  pOUT_FPVI.Set(FI, float(-10e-3), FPVI10_5V, FPVI10_1A, RELAY_ON);
  delay_ms(2);

  pOUT_FPVI.Set(FI, float(-1000e-3), FPVI10_5V, FPVI10_1A, RELAY_ON);
  delay_ms(2);
	pOUT_FPVI.MeasureVI(100, 10);
  pVIN_FPVI.MeasureVI(100, 10);

  for (site=0; site < SITE_NUM; site++) {
		vs[site] = pOUT_FPVI.GetMeasResult(site, MVRET);
		vd[site] = pVIN_FPVI.GetMeasResult(site, MVRET);
		loading[site] = pOUT_FPVI.GetMeasResult(site, MIRET);
  }
  pOUT_FPVI.Set(FI, float(-0e-3), FPVI10_5V, FPVI10_1A, RELAY_ON);
  delay_ms(1);
  pOUT_FPVI.Set(FI, float(-0e-3), FPVI10_5V, FPVI10_1A, RELAY_OFF);
  delay_ms(10);
    
  for (site = 0; site < SITE_NUM; site++) {
    rdson[site] = -((vd[site]-vs[site])/loading[site])*1e3;
    RDSON->SetTestResult(site, 0, rdson[site]);
  } 
  power_dwn();
	PIN2OFF();

  return 0;
}


DUT_API int _5_Current_Limit(short funcindex, LPCTSTR funclabel)    {
  //{{AFX_STS_PARAM_PROTOTYPES
  CParam *Ilimit_TM = StsGetParam(funcindex,"Ilimit_TM");
  CParam *Ilimit = StsGetParam(funcindex,"Ilimit");
  //}}AFX_STS_PARAM_PROTOTYPES

  // TODO: Add your function code here
  float ISW_C[SITE_NUM] = { 0.0 }, iload = 0;
  float SW_C[SITE_NUM] = {0.0f}, SW_V[SITE_NUM]={0.0f};
  int flag[SITE_NUM] = {0};

	DWORD CurEnbSite = 0;
	BYTE sitesta[SITE_NUM];

	StsGetSiteStatus(sitesta, SITE_NUM);
	for(site = 0; site < SITE_NUM; site++)
		CurEnbSite += sitesta[site] << site;

	StsSetSiteStatus(CurEnbSite & 0x0f);
	for(site=0; site < SITE_NUM; site++)	{
		flag[site] = 1;
		if((CurEnbSite >> site) & 0x1)
			flag[site] = 0;
		else 
			flag[site] = 1;
	}
  // RELAY SETTING
	cbit.SetOn(VIN_FPVI, OUT_FPVI, BIAS_FOVI, VIN_CAP, BIAS_CAP, OUT_CAP, ENm_FOVI, VIN_Zener, - 1);
	delay_ms(1);

  // BIAS = 3.3V
  pBIAS.Set(FV, float(3.0), FOVI_5V, FOVI_1A, RELAY_ON, 2);
  delay_us(500);
	pBIAS.Set(FV, float(3.1), FOVI_5V, FOVI_1A, RELAY_ON, 2);
  delay_us(500);
	pBIAS.Set(FV, float(3.3), FOVI_5V, FOVI_1A, RELAY_ON, 2);
  delay_ms(10);
  // VIN = 3.3V
  pVIN_FPVI.Set(FV, float(0.3), FPVI10_5V, FPVI10_10A, RELAY_ON, 2);
  delay_us(500);
  pVIN_FPVI.Set(FV, float(0.5), FPVI10_5V, FPVI10_10A, RELAY_ON, 2);
  delay_us(500);
  pVIN_FPVI.Set(FV, float(0.8), FPVI10_5V, FPVI10_10A, RELAY_ON, 2);
  delay_ms(1);
  // EN = 2V
	pEN.Set(FV, (float)1.8, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(1);
	pOUT_FPVI.Set(FI, -1e-3f, FPVI10_10V, FPVI10_10A, RELAY_ON);
    
	// VOUT EMPTY LOADING
	pOUT_FPVI.MeasureVI(50, 20);
	for(site=0; site<SITE_NUM; site++ )
		SW_V[site] = pOUT_FPVI.GetMeasResult(site, MVRET);

  // TEST MODE SEQUENCE ( VEN = VBIAS + 1.5 )
	pEN.Set(FV, (float)4.8, FOVI_10V, FOVI_1A, RELAY_ON);
	delay_us(350);
	pEN.Set(FV, -1*(float)0.6, FOVI_10V, FOVI_1A, RELAY_ON);
	delay_us(350);
	
	// VBIAS = 5V & RELEASE EN
	pBIAS.Set(FV, float(3.3), FOVI_5V, FOVI_1A, RELAY_ON);
  delay_ms(1);
	pEN.Set(FV, (float)0, FOVI_10V, FOVI_1A, RELAY_OFF);
	delay_us(350);

	pOUT_FPVI.MeasureVI(50, 20);
	for(site=0; site<SITE_NUM; site++ )
		SW_V[site] = pOUT_FPVI.GetMeasResult(site, MVRET);

	for(iload=1300.0f; iload <= 3100.0f;)   {
	  pOUT_FPVI.Set(FI, -1.0e-3f*iload, FPVI10_10V, FPVI10_10A, RELAY_ON);	// ILOAD SOURCE
		//delay_us(100);
   	pOUT_FPVI.MeasureVI(20, 10);	
   	for(site=0; site < SITE_NUM; site++)    {
			ISW_C[site] = pOUT_FPVI.GetMeasResult(site, MIRET);
			SW_C[site] = pOUT_FPVI.GetMeasResult(site, MVRET);
   		if((flag[site] == 0) && (SW_C[site] < 0.3f))    {	// OCP TRIGGER LEVEL
				ISW_C[site] = iload;
				flag[site] = 1;
   		}
   	}
   	if((flag[0] == 1) && (flag[1] == 1) && (flag[2] == 1) && (flag[3] == 1))	
      break;
   		
		iload += 10;
  }	
	pOUT_FPVI.Set(FI, float(0e-3), FPVI10_5V, FPVI10_1A, RELAY_ON);
	delay_ms(1);
	pOUT_FPVI.Set(FI, float(0e-3), FPVI10_5V, FPVI10_1A, RELAY_OFF);
	for (site = 0; site < SITE_NUM; site++)	{
		Ilimit_TM->SetTestResult(site, 0, ISW_C[site]);
		Ilimit->SetTestResult(site, 0, ISW_C[site]*float(3.0));	// 240417-3 add scale 3.15x
																														// 240516-4 scale 3.0x
	}
	power_dwn();
	PIN2OFF();
	cbit.SetOn(-1);

  return 0;
}


DUT_API int _6_TSS(short funcindex, LPCTSTR funclabel)  {
  //{{AFX_STS_PARAM_PROTOTYPES
  CParam *TS_ST1 = StsGetParam(funcindex,"TS_ST1");
  CParam *TS_ST2 = StsGetParam(funcindex,"TS_ST2");
  CParam *TS_ST3 = StsGetParam(funcindex,"TS_ST3");
  //}}AFX_STS_PARAM_PROTOTYPES

  // TODO: Add your function code here
  float ts1[SITE_NUM] = { 0.0 }, ts2[SITE_NUM] = { 0.0 }, ts3[SITE_NUM] = { 0.0 };

  // RELAY SETTING
	cbit.SetOn(VIN_FPVI, BIAS_FOVI, ENm_FOVI, VIN_CAP, BIAS_CAP, OUT_CAP, OUT_QTMU_A, -1);	// OUT-CHA, wCAP
	delay_ms(1);

	pVIN_FPVI.Set(FI, -500e-3f, FPVI10_5V, FPVI10_1A, RELAY_ON);
	pOUT_FPVI.Set(FI, -500e-3f, FPVI10_5V, FPVI10_1A, RELAY_ON);
	delay_ms(10);

  // TSS1 *****************************************************************
	POWER_ON();
	// TMU SETTING
	qtmu0.Init();
	delay_ms(2);
	qtmu0.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_5V, QTMU_PLUS_FILTER_10MHz);
	qtmu0.SetStopInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_5V, QTMU_PLUS_FILTER_10MHz);
	qtmu0.SetStartTrigger(0.10f, QTMU_PLUS_POS_SLOPE);
	qtmu0.SetStopTrigger(0.70f, QTMU_PLUS_POS_SLOPE);	// new condition (vcc/vbias)
	qtmu0.SetInSource(QTMU_PLUS_SINGLE_SOURCE);
	qtmu0.Connect();
	delay_ms(20);
	qtmu0.SetTimeOut(5);

  // EN = 2V
	pEN.Set(FV, (float)1.8, FOVI_2V, FOVI_100MA, RELAY_ON);
	qtmu0.SetSinglePulseMeas(QTMU_PLUS_COARSE, QTMU_PLUS_TIME_US, site);
	qtmu0.SinglePlsMeas(site);
  // TSS1 *****************************************************************
	for(site=0; site < SITE_NUM; site++ )  {
		ts1[site]=qtmu0.GetMeasureResult(site);
		ts1[site] += float(0);	// 240417-3, ofs=4us
	}
	// EN = 2V
	pEN.Set(FV, (float)2, FOVI_2V, FOVI_100MA, RELAY_ON);
	pVIN_FPVI.Set(FI, -500e-3f, FPVI10_5V, FPVI10_1A, RELAY_ON);
	pOUT_FPVI.Set(FI, -500e-3f, FPVI10_5V, FPVI10_1A, RELAY_ON);
	delay_ms(10);
	power_dwn();
  // EN = 2V
	pEN.Set(FV, (float)0, FOVI_2V, FOVI_100MA, RELAY_ON);
	delay_ms(10);
	pEN.Set(FV, (float)0, FOVI_2V, FOVI_100MA, RELAY_OFF);
  // TSS3 *****************************************************************
	qtmu0.Init();
	delay_ms(2);
	cbit.SetOn(VIN_FPVI, BIAS_FOVI, VIN_CAP, BIAS_CAP, OUT_CAP, OUT_QTMU_A, REN_300KOhm, -1);		// OUT-CHA, wCAP
	delay_ms(2);
	POWER_ON();
	qtmu0.Connect();

	qtmu0.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_5V, QTMU_PLUS_FILTER_10MHz);
	qtmu0.SetStopInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_5V, QTMU_PLUS_FILTER_10MHz);
	qtmu0.SetStartTrigger(0.08f, QTMU_PLUS_POS_SLOPE);
	qtmu0.SetStopTrigger(0.75f, QTMU_PLUS_POS_SLOPE);
	qtmu0.SetInSource(QTMU_PLUS_SINGLE_SOURCE);
	qtmu0.SetTimeOut(100);

	pBIAS.MeasureVI(10, 10);
	pVIN_FPVI.MeasureVI(10, 10);
	
	// EN CTL = 3.3v
	psEN.Set(FV, (float)1.8, FOVI_2V, FOVI_100MA, RELAY_ON);
	delay_us(100);
	qtmu0.SetSinglePulseMeas(QTMU_PLUS_COARSE, QTMU_PLUS_TIME_US, site);
	qtmu0.SinglePlsMeas(site);

	for(site=0; site < SITE_NUM; site++ )  {
		ts3[site]=qtmu0.GetMeasureResult(site);
		ts3[site] += float(110);	// 240417-3, ofs=-6us
															// 240522-3, ofs=110us
	}
	psEN.Set(FV, (float)0, FOVI_2V, FOVI_100MA, RELAY_ON);
	qtmu0.Disconnect();
	qtmu0.Init();
	delay_ms(2);
  // TSS2 *****************************************************************
  // EN CTRL = 0V
	psEN.Set(FV, (float)0, FOVI_2V, FOVI_100MA, RELAY_ON);
	delay_ms(1);
	pBIAS.Set(FV, float(0), FOVI_2V, FOVI_100MA, RELAY_ON);
  delay_ms(1);
  pVIN_FPVI.Set(FV, float(0), FPVI10_5V, FPVI10_1A, RELAY_ON);
  delay_ms(1);

	//cbit.SetOn(VIN_FPVI, BIAS_FOVI, VIN_CAP, BIAS_CAP, OUT_CAP, EN_QTMU_B, OUT_QTMU_A, REN_300KOhm, -1);
	cbit.SetOn(VIN_FPVI, BIAS_FOVI, VIN_CAP, BIAS_CAP, OUT_CAP, EN_QTMU_B, OUT_QTMU_A, REN_300KOhm, Sweep_QTMU, -1);
	delay_ms(10);

	POWER_ON();
	qtmu0.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_5V, QTMU_PLUS_FILTER_10MHz);
	qtmu0.SetStopInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_5V, QTMU_PLUS_FILTER_10MHz);
	qtmu0.SetStartTrigger(1.12f, QTMU_PLUS_POS_SLOPE);
	qtmu0.SetStopTrigger(1.28f, QTMU_PLUS_NEG_SLOPE);
	qtmu0.SetInSource(QTMU_PLUS_SINGLE_SOURCE);
	qtmu0.Connect();
	delay_ms(20);
	qtmu0.SetTimeOut(10);

  // psEN = 2V
	psEN.Set(FV, (float)1.2, FOVI_5V, FOVI_100MA, RELAY_ON);
	delay_ms(2);
	psEN.Set(FV, (float)1.8, FOVI_5V, FOVI_100MA, RELAY_ON);
	qtmu0.SetSinglePulseMeas(QTMU_PLUS_COARSE, QTMU_PLUS_TIME_US, site);
	qtmu0.SinglePlsMeas(site);
	
	for(site=0; site < SITE_NUM; site++ )  {

		ts2[site]=qtmu0.GetMeasureResult(site);
		ts2[site] += float(0);	// 240417-3, ofs=30us
	}
	
  for (site=0; site < SITE_NUM; site++)   {
    TS_ST1->SetTestResult(site, 0, ts1[site]);
    TS_ST2->SetTestResult(site, 0, ts2[site]);
	  TS_ST3->SetTestResult(site, 0, ts3[site]);
  }

	if(DDATA)	{
		fprintf(stderr, "TSS1(us): %8.2f, %8.2f, %8.2f, %8.2f\n", ts1[0], ts1[1], ts1[2], ts1[3]);
		fprintf(stderr, "TSS2(us): %8.2f, %8.2f, %8.2f, %8.2f\n", ts2[0], ts2[1], ts2[2], ts2[3]);
		fprintf(stderr, "TSS3(us): %8.2f, %8.2f, %8.2f, %8.2f\n", ts3[0], ts3[1], ts3[2], ts3[3]);
	}
	qtmu0.Disconnect();
  power_dwn();
	PIN2OFF();
    
	return 0;
}


DUT_API int postContinuity(short funcindex, LPCTSTR funclabel)  {
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *post_VIN = StsGetParam(funcindex,"post_VIN");
    CParam *post_OUT = StsGetParam(funcindex,"post_OUT");
    CParam *post_EN = StsGetParam(funcindex,"post_EN");
    CParam *post_BIAS = StsGetParam(funcindex,"post_BIAS");
    //}}AFX_STS_PARAM_PROTOTYPES

    // TODO: Add your function code here
    power_dwn();

	cbit.SetOn(VIN_FPVI, OUT_FPVI, ENm_FOVI, BIAS_FOVI, -1);
	delay_ms(1);
	PIN2GND();
	delay_us(50);
	// *************************************** VIN ***************************************
	pVIN_FPVI.Set(FI, float(-1), FPVI10_2V, FPVI10_1MA, RELAY_ON);
	delay_ms(1);
	pVIN_FPVI.MeasureVI(10, 10);
	for (site = 0; site < SITE_NUM; site++)
		post_VIN->SetTestResult(site, 0, pVIN_FPVI.GetMeasResult(site, MVRET));
	pVIN_FPVI.Set(FV, 0.0, FPVI10_1V, FPVI10_1MA, RELAY_ON);
	// *************************************** VOUT ***************************************
	pOUT_FPVI.Set(FI,float(-1), FPVI10_2V, FPVI10_1MA, RELAY_ON);
	delay_ms(1);
	pOUT_FPVI.MeasureVI(10, 10);
	for (site = 0; site < SITE_NUM; site++)
		post_OUT->SetTestResult(site, 0, pOUT_FPVI.GetMeasResult(site, MVRET));
	pOUT_FPVI.Set(FV, 0.0, FPVI10_1V, FPVI10_1MA, RELAY_ON);
    // *************************************** EN ***************************************
	pEN.Set(FI, float(-1), FOVI_2V, FOVI_1MA, RELAY_ON);
	delay_ms(1);
	pEN.MeasureVI(10, 10);
	for (site = 0; site < SITE_NUM; site++)
		post_EN->SetTestResult(site, 0, pEN.GetMeasResult(site, MVRET));
	pEN.Set(FV, 0.0, FOVI_1V, FOVI_1MA, RELAY_ON);
    // *************************************** BIAS ***************************************
	pBIAS.Set(FI, float(-1), FOVI_2V, FOVI_1MA, RELAY_ON);
	delay_ms(1);
	pBIAS.MeasureVI(10, 10);
	for (site = 0; site < SITE_NUM; site++)
		post_BIAS->SetTestResult(site, 0, pBIAS.GetMeasResult(site, MVRET));
	pBIAS.Set(FV, 0.0, FOVI_1V, FOVI_1MA, RELAY_ON);
    
	PIN2OFF();
	return 0;
}


DUT_API int _5_RDSG(short funcindex, LPCTSTR funclabel)
{
    //{{AFX_STS_PARAM_PROTOTYPES
    CParam *RDSG = StsGetParam(funcindex,"RDSG");
    //}}AFX_STS_PARAM_PROTOTYPES

	float rDSG[SITE_NUM] = { 0 };
	double vset = 300e-3f;
	int flag[SITE_NUM] = { 0 };
    // TODO: Add your function code here
	DWORD CurEnbSite = 0;
	BYTE sitesta[SITE_NUM];
	
	StsGetSiteStatus(sitesta, SITE_NUM);
	CurEnbSite = StsGetsSiteStatus();
	StsSetSiteStatus(CurEnbSite & 0xf);
	for(site=0; site<SITE_NUM; site++)	{
		CurEnbSite = (CurEnbSite >> site)&&0x1;
		if(CurEnbSite == 1)	
			flag[site] = 0;
		else 
			flag[site] = 1;
	}

    // RELAY SETTING
	cbit.SetOn(OUT_FPVI, BIAS_FOVI, VIN_CAP, BIAS_CAP, OUT_CAP, VIN_Zener, -1);
	delay_ms(1);

	pBIAS.Set(FV, (float)3.3, FOVI_5V, FOVI_100MA, RELAY_ON); // 240423-2 frorm 5v to 3.3v for BIAS
	delay_ms(2);
    // VOUT 300mV
    pOUT_FPVI.Set(FV, float(vset), FPVI10_1V, FPVI10_100MA, RELAY_ON);
    delay_ms(1);

	pOUT_FPVI.MeasureVI(20, 10);
	for (site = 0; site < SITE_NUM; site++)	{
		adresult[site] = pOUT_FPVI.GetMeasResult(site, MIRET);
        RDSG->SetTestResult(site, 0, (vset/adresult[site]));
	}
    
	power_dwn();
	PIN2OFF();
    return 0;
}
