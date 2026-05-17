#include "stdafx.h"
#include "stdlib.h"
#include "math.h"

#define SITE_NUM 4

FPVI10  pVIN_FPVI    (0, "VIN");
FPVI10  pOUT_FPVI    (1, "OUT");

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
// #define  VSET_10KOhm        3,  35, 67, 99      // K4   xxx
#define  REN_300KOhm        4,  36, 68, 100     // K5, fovi1 300K
#define  VIN_FPVI           5,  37, 69, 101     // K6
#define  OUT_FPVI           6,  38, 70, 102     // K7
// #define  OUT_FOVI           7,  39, 71, 103     // K8
// #define  ENs_FOVI           8,  40, 72, 104     // K9
#define  ENm_FOVI           9,  41, 73, 105     // K10
#define  BIAS_FOVI          10, 42, 74, 106     // K11
// #define  FloatSource_GNG    11, 43, 75, 107     // K12  xxx
// #define  NC_FOVI            12, 44, 76, 108     // K13
#define  Sweep_QTMU         13, 45, 77, 109     // K14
#define  EN_QTMU_B          14, 46, 78, 110     // K15
#define  OUT_QTMU_A         15, 47, 79, 111     // K16


//multisite settings should be included here
DUT_API void HardWareCfg()  {
	/*For example: four channels dvi to config two sites*/
	StsSetModuleToSite(MD_FOVI,SITE_1,  0,  1,  2,  3,  4,  5,  6,  7,  -1);
	StsSetModuleToSite(MD_FOVI,SITE_2,  8,  9,  10, 11, 12, 13, 14, 15, -1);
	StsSetModuleToSite(MD_FOVI,SITE_3,  16, 17, 18, 19, 20, 21, 22, 23, -1);
	StsSetModuleToSite(MD_FOVI,SITE_4,  24, 25, 26, 27, 28, 29, 30, 31, -1);
	
	StsSetModuleToSite(MD_FPVI10,SITE_1,    0,  1,  -1);
	StsSetModuleToSite(MD_FPVI10,SITE_2,    2,  3,  -1);
	StsSetModuleToSite(MD_FPVI10,SITE_3,    4,  5,  -1);
	StsSetModuleToSite(MD_FPVI10,SITE_4,    6,  7,  -1);

	StsSetModuleToSite(MD_QTMUPLUS,SITE_1,  0,  1,  -1);
	StsSetModuleToSite(MD_QTMUPLUS,SITE_2,  2,  3,  -1);
	StsSetModuleToSite(MD_QTMUPLUS,SITE_3,  4,  5,  -1);
	StsSetModuleToSite(MD_QTMUPLUS,SITE_4,  6,  7,  -1);
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
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//initializefunction will be called after all the test functions.
DUT_API void InitAfterTestFlow()    {		
	cbit.SetOn(-1);
	delay_ms(1);
	qtmu0.Init();
}
// ************************************************* user definition start *************************************************
int site = 0;
void power_dwn(void)	{
	pEN.Set(FV, 0.0, FOVI_5V, 	FOVI_10MA, 	RELAY_ON);
	psEN.Set(FV, 0.0, FOVI_5V, 	FOVI_10MA, 	RELAY_ON);
	pBIAS.Set(FV, 0.0, FOVI_5V,	FOVI_10MA, 	RELAY_ON);
	pVIN_FPVI.Set(FV, 0.0, FPVI10_5V, FPVI10_10MA,	RELAY_ON);
	pOUT_FPVI.Set(FV, 0.0, FPVI10_5V, FPVI10_10MA,	RELAY_ON);
	delay_ms(2);

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
	pVIN_FPVI.Set(FV, float(-1.0), FPVI10_1V, FPVI10_1MA, RELAY_ON);
	delay_ms(1);
	pVIN_FPVI.MeasureVI(10, 10);
	for (site = 0; site < SITE_NUM; site++)
		pre_VIN->SetTestResult(site, 0, pVIN_FPVI.GetMeasResult(site, MVRET));
	pVIN_FPVI.Set(FV, 0.0, FPVI10_1V, FPVI10_1MA, RELAY_ON);
	// *************************************** VOUT(2) ***************************************
	pOUT_FPVI.Set(FV,float(-1.0), FPVI10_1V, FPVI10_1MA, RELAY_ON);
	delay_ms(1);
	pOUT_FPVI.MeasureVI(10, 10);
	for (site = 0; site < SITE_NUM; site++)
		pre_OUT->SetTestResult(site, 0, pOUT_FPVI.GetMeasResult(site, MVRET));
	pOUT_FPVI.Set(FV, 0.0, FPVI10_1V, FPVI10_1MA, RELAY_ON);
    // *************************************** EN(6) ***************************************
	pEN.Set(FV, float(-1.0), FOVI_1V, FOVI_1MA, RELAY_ON);
	delay_ms(1);
	pEN.MeasureVI(10, 10);
	for (site = 0; site < SITE_NUM; site++)
		pre_EN->SetTestResult(site, 0, pEN.GetMeasResult(site, MVRET));
	pEN.Set(FV, 0.0, FOVI_1V, FOVI_1MA, RELAY_ON);
    // *************************************** BIAS(4) ***************************************
	pBIAS.Set(FV, float(-1.0), FOVI_1V, FOVI_1MA, RELAY_ON);
	delay_ms(1);
	pBIAS.MeasureVI(10, 10);
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

	float vbias_st = 0, vin_st = 0;
    // TODO: Add your function code here
    // RELAY SETTING
	cbit.SetOn(VIN_FPVI, BIAS_FOVI, ENm_FOVI, -1);
	delay_ms(1);

    // BIAS = 5V
    pBIAS.Set(FV, float(0), FOVI_10V, FOVI_10MA, RELAY_ON);
    delay_ms(2);

	for (vbias_st = 0; vbias_st <= float(5);) {
        pBIAS.Set(FV, float(vbias_st), FOVI_10V, FOVI_10MA, RELAY_ON);
        delay_us(300);

		vbias_st += float(500e-3f);
    }
    // VIN = 3V
    pVIN_FPVI.Set(FV, float(0), FPVI10_5V, FPVI10_10MA, RELAY_ON);
    delay_ms(2);

	for (vin_st = float(2.8); vin_st <= float(3.3);) {
        pVIN_FPVI.Set(FV, float(vin_st), FPVI10_5V, FPVI10_100UA, RELAY_ON);
        delay_us(300);

		vin_st += float(100e-3);
    }
    pBIAS.Set(FV, float(5), FOVI_10V, FOVI_100UA, RELAY_ON);
    pVIN_FPVI.Set(FV, float(3.3), FPVI10_5V, FPVI10_10MA, RELAY_ON);
    pEN.Set(FV, float(0), FOVI_5V, FOVI_100UA, RELAY_ON);
    delay_ms(20);
    // ****** SHUTDOWN CURRENT
	pBIAS.MeasureVI(100, 10);
	for (site = 0; site < SITE_NUM; site++)	{
		//adresult[site] = pBIAS.GetMeasResult(site, MIRET);
		//ISD_BIAS->SetTestResult(site, 0, adresult[site]*1e6);
		ISD_BIAS->SetTestResult(site, 0, fabs((pBIAS.GetMeasResult(site, MIRET))*1e6));
	}

    pVIN_FPVI.MeasureVI(100, 10);
	for (site = 0; site < SITE_NUM; site++)
		ISD_IN->SetTestResult(site, 0, fabs((pVIN_FPVI.GetMeasResult(site, MIRET))*1e6));

    // ****** QUIESCENT CURRENT
	cbit.SetOn(VIN_FPVI, BIAS_FOVI, VIN_CAP, BIAS_CAP, ENm_FOVI, -1);
	delay_ms(1);

    pBIAS.Set(FV, float(5), FOVI_10V, FOVI_10MA, RELAY_ON);

    // EN = 2V
    pEN.Set(FV, (float)2, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_ms(10);

    pBIAS.MeasureVI(100, 10);
    for (site = 0; site < SITE_NUM; site++)
		IQ_BIAS->SetTestResult(site, 0, pBIAS.GetMeasResult(site, MIRET)*1e6);

    pVIN_FPVI.MeasureVI(100, 10);
	for (site = 0; site < SITE_NUM; site++)
		IQ_VIN->SetTestResult(site, 0, pVIN_FPVI.GetMeasResult(site, MIRET)*1e6);
   
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
	float vtrig = 1.5, voutj[SITE_NUM] = {0};
    float i= 0, flag[SITE_NUM] = { 0 };

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
	cbit.SetOn(VIN_FPVI, OUT_FPVI, BIAS_FOVI, VIN_CAP, BIAS_CAP, OUT_CAP, ENm_FOVI, -1);
	delay_ms(1);

    // WIN = 3V
    pVIN_FPVI.Set(FV, float(2.8), FPVI10_5V, FPVI10_100MA, RELAY_ON);
    delay_us(500);
    pVIN_FPVI.Set(FV, float(3.0), FPVI10_5V, FPVI10_100MA, RELAY_ON);
    delay_us(500);
    pVIN_FPVI.Set(FV, float(3.3), FPVI10_5V, FPVI10_100MA, RELAY_ON);
    delay_ms(1);
    // EN = 2V
	pEN.Set(FV, (float)2.0, FOVI_5V, FOVI_100MA, RELAY_ON);
	delay_ms(1);

    // VOUT MEASUREMENT
    pOUT_FPVI.Set(FI, float(-0.1e-6), FPVI10_5V, FPVI10_100UA, RELAY_SENSE_ON);
    pBIAS.Set(FV, (float)1, FOVI_5V, FOVI_100MA, RELAY_ON);
    delay_ms(1);

    // ****** UVLO ON
    for (i = 1.5f; i <= 2.0f;)  {
        pBIAS.Set(FV, (float)i, FOVI_5V, FOVI_100MA, RELAY_ON);
        //delay_ms(1);
	    pOUT_FPVI.MeasureVI(20, 10);
        //pVIN_FPVI.MeasureVI(20, 10);

        for (site = 0; site < SITE_NUM; site++) {
            adresult[site] = pOUT_FPVI.GetMeasResult(site, MVRET);
			//adresult[site] = pVIN_FPVI.GetMeasResult(site, MIRET);
			

            if( (adresult[site] > vtrig) && flag[site] == 0)  {
			//if( (adresult[site] > (float)1000e-6f) && flag[site] == 0)  {
                uvlo_on[site] = i;
                flag[site] = 1;
            }
        }
        if(flag[0] && flag[1] && flag[2] && flag[3])
            break;
        i += float(1e-3);
    }

	// BIAS = 4V
	pBIAS.Set(FV, (float)4, FOVI_5V, FOVI_100MA, RELAY_ON);
    delay_ms(1);
	for(site=0; site < SITE_NUM; site++)	{
		pOUT_FPVI.MeasureVI(20, 10);
		voutj[site] = pOUT_FPVI.GetMeasResult(site, MVRET);
	}

    // ****** UVLO OFF
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

    for (i = 4.0; i >= float(1.5);)  {
        pBIAS.Set(FV, (float)i, FOVI_5V, FOVI_100MA, RELAY_ON);
        delay_us(500);
		pOUT_FPVI.MeasureVI(20, 10);
        
        for (site = 0; site < SITE_NUM; site++) {
            adresult[site] = pOUT_FPVI.GetMeasResult(site, MVRET);

			if( (adresult[site] < (voutj[site] - 200e-3f) && flag[site] == 0) )  {
                uvlo_off[site] = i;
                flag[site] = 1;
            }
        }
        if(flag[0] && flag[1] && flag[2] && flag[3])
            break;

		if(i<float(uvlo_on[site]))
			i += float(-1e-3);
		else
		    i += float(-100e-3);
    }

	// ****** EN CLAMPING
	//cbit.SetOn(VIN_FPVI, OUT_FPVI, BIAS_FOVI, VIN_CAP, BIAS_CAP, OUT_CAP, ENm_FOVI, REN_300KOhm, -1);
	cbit.SetOn(VIN_FPVI, BIAS_FOVI, VIN_CAP, BIAS_CAP, ENm_FOVI, REN_300KOhm, -1);
	delay_ms(1);
    
	// BIAS = 5V
    pBIAS.Set(FV, (float)4, FOVI_5V, FOVI_100MA, RELAY_ON);
    delay_ms(2);
    pBIAS.Set(FV, (float)5, FOVI_5V, FOVI_100MA, RELAY_ON);
    delay_us(500);

    // EN CTRL = 3.3V
	pEN.Set(FI, (float)0.0, FOVI_5V, FOVI_100UA, RELAY_SENSE_ON);
	delay_ms(1);
	psEN.Set(FV, (float)3.3, FOVI_5V, FOVI_100MA, RELAY_ON);
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
    pBIAS.Set(FV, float(4.0), FOVI_5V, FOVI_100MA, RELAY_ON);
    delay_us(500);
	pBIAS.Set(FV, float(4.5), FOVI_5V, FOVI_100MA, RELAY_ON);
    delay_us(500);
	pBIAS.Set(FV, float(5.0), FOVI_5V, FOVI_100MA, RELAY_ON);
    delay_ms(1);
    // VIN = 3V
    pVIN_FPVI.Set(FV, float(2.8), FPVI10_5V, FPVI10_1A, RELAY_ON);
    delay_us(500);
    pVIN_FPVI.Set(FV, float(3.0), FPVI10_5V, FPVI10_1A, RELAY_ON);
    delay_us(500);
    pVIN_FPVI.Set(FV, float(3.3), FPVI10_5V, FPVI10_1A, RELAY_ON);
    delay_ms(1);
    // EN = 2V
	pEN.Set(FV, (float)2.0, FOVI_5V, FOVI_10MA, RELAY_ON);
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
    CParam *Ilimit = StsGetParam(funcindex,"Ilimit");
    //}}AFX_STS_PARAM_PROTOTYPES

    // TODO: Add your function code here
    float ISW_C[SITE_NUM] = { 0.0 }, iload = 0;
    float SW_C[SITE_NUM] = {0.0f}, SW_V[SITE_NUM]={0.0f};
    int flag[SITE_NUM] = {0};

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
	cbit.SetOn(VIN_FPVI, OUT_FPVI, BIAS_FOVI, VIN_CAP, BIAS_CAP, OUT_CAP, ENm_FOVI, - 1);
	delay_ms(1);

    // BIAS = 3.3V
    pBIAS.Set(FV, float(3.0), FOVI_5V, FOVI_100MA, RELAY_ON);
    delay_us(500);
	pBIAS.Set(FV, float(3.1), FOVI_5V, FOVI_100MA, RELAY_ON);
    delay_us(500);
	pBIAS.Set(FV, float(3.3), FOVI_5V, FOVI_100MA, RELAY_ON);
    delay_ms(1);
    // VIN = 3.3V
    pVIN_FPVI.Set(FV, float(2.8), FPVI10_5V, FPVI10_10A, RELAY_ON);
    delay_us(500);
    pVIN_FPVI.Set(FV, float(3.0), FPVI10_5V, FPVI10_10A, RELAY_ON);
    delay_us(500);
    pVIN_FPVI.Set(FV, float(3.3), FPVI10_5V, FPVI10_10A, RELAY_ON);
    delay_ms(1);
    // NE = 2V
	pEN.Set(FV, (float)2.0, FOVI_10V, FOVI_10MA, RELAY_ON);
	delay_ms(10);
	pOUT_FPVI.Set(FI, -10e-3f, FPVI10_10V, FPVI10_10A, RELAY_ON);
    
	// VOUT EMPTY LOADING
	pOUT_FPVI.MeasureVI(50, 20);
	for(site=0; site<SITE_NUM; site++ )
		SW_V[site] = pOUT_FPVI.GetMeasResult(site, MVRET);

    // TEST MODE SEQUENCE ( VEN = VBIAS + 1.5 )
	pEN.Set(FV, (float)4.8, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_us(350);
	pEN.Set(FV, -1*(float)0.6, FOVI_10V, FOVI_100MA, RELAY_ON);
	delay_us(350);
	
	// VBIAS = 5V & RELEASE EN
	pBIAS.Set(FV, float(5.0), FOVI_5V, FOVI_100MA, RELAY_ON);
    delay_ms(1);
	pEN.Set(FV, (float)0, FOVI_10V, FOVI_10MA, RELAY_OFF);
	delay_us(350);

	pOUT_FPVI.MeasureVI(50, 20);
	for(site=0; site<SITE_NUM; site++ )
		SW_V[site] = pOUT_FPVI.GetMeasResult(site, MVRET);	// woLOADING

   	for(iload=1000.0f; iload <= 2500.0f;)   {
	    pOUT_FPVI.Set(FI, -1.0e-3f*iload, FPVI10_10V, FPVI10_10A, RELAY_ON);	// ILOAD SOURCE
   		delay_ms(1);;
   		pOUT_FPVI.MeasureVI(50, 10);	
   		for(site=0; site < SITE_NUM; site++)    {
			pOUT_FPVI.Set(FI, float(0e-3), FPVI10_5V, FPVI10_1A, RELAY_ON);		// RECHARGE
			delay_ms(1);
   			
			ISW_C[site] = pOUT_FPVI.GetMeasResult(site, MIRET);
			SW_C[site] = pOUT_FPVI.GetMeasResult(site, MVRET);
   			if((flag[site] == 0) && (SW_C[site] < (SW_V[site] -0.1f)))    {		// OCP TRIGGER LEVEL
				ISW_C[site] = iload;
				flag[site] = 1;
   			}
   		}
		
   		if((flag[0] == 1) && (flag[1] == 1) && (flag[2] == 1) && (flag[3] == 1))	
            break;
   		
   		iload += 15.0f; 
   	}	

	for (site = 0; site < SITE_NUM; site++)
		Ilimit->SetTestResult(site, 0, ISW_C[site]);
	
	power_dwn();
	PIN2OFF();

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
	//cbit.SetOn(VIN_FPVI, BIAS_FOVI, ENm_FOVI, VIN_CAP, BIAS_CAP, OUT_QTMU_A, -1);	// OUT-CHA, woCAP
	delay_ms(1);

    // ****** TSS1, OUT SS
    // BIAS = 5V
    pBIAS.Set(FV, float(4.0), FOVI_5V, FOVI_100MA, RELAY_ON);
    delay_us(500);
	pBIAS.Set(FV, float(4.5), FOVI_5V, FOVI_100MA, RELAY_ON);
    delay_us(500);
	pBIAS.Set(FV, float(5.0), FOVI_5V, FOVI_100MA, RELAY_ON);
    delay_ms(1);
    // VIN = 3.3V
    pVIN_FPVI.Set(FV, float(2.8), FPVI10_5V, FPVI10_10MA, RELAY_ON);
    delay_us(500);
    pVIN_FPVI.Set(FV, float(3.0), FPVI10_5V, FPVI10_10MA, RELAY_ON);
    delay_us(500);
    pVIN_FPVI.Set(FV, float(3.3), FPVI10_5V, FPVI10_10MA, RELAY_ON);
    delay_ms(1);

	// TMU SETTING
	qtmu0.Connect();
	delay_ms(1);
	qtmu0.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_5V, QTMU_PLUS_FILTER_PASS);
	qtmu0.SetStopInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_5V, QTMU_PLUS_FILTER_PASS);
	qtmu0.SetStartTrigger(1.6f, QTMU_PLUS_POS_SLOPE);
	qtmu0.SetStopTrigger(3.1f, QTMU_PLUS_POS_SLOPE);
	qtmu0.SetInSource(QTMU_PLUS_SINGLE_SOURCE);
	//qtmu0.ChannelSetup(QTMU_PLUS_CHA_START);
	delay_ms(2);
	qtmu0.SetTimeOut(10);

    // EN = 2V
	pEN.Set(FV, (float)2.0, FOVI_5V, FOVI_10MA, RELAY_ON);

    // TSS1
	for(site=0; site < SITE_NUM; site++ )  {
		//pOUT_FPVI.MeasureVI(600, 2);	// SOFTVIEW
		qtmu0.SetSinglePulseMeas(QTMU_PLUS_COARSE, QTMU_PLUS_TIME_US, site);
		qtmu0.SinglePlsMeas(site);
		ts1[site]=qtmu0.GetMeasureResult(site);
	}

    // TSS3
	cbit.SetOn(VIN_FPVI, BIAS_FOVI, VIN_CAP, BIAS_CAP, OUT_CAP, OUT_QTMU_A, REN_300KOhm, -1);		// OUT-CHA, wCAP
	//cbit.SetOn(VIN_FPVI, BIAS_FOVI, VIN_CAP, BIAS_CAP, OUT_QTMU_A, REN_300KOhm, -1);		// OUT-CHA, woCAP

    // RELEASE EN V/I
    pEN.Set(FV, (float)0, FOVI_5V, FOVI_10MA, RELAY_ON);
	delay_ms(1);
	pEN.Set(FV, (float)0, FOVI_5V, FOVI_10MA, RELAY_OFF);
	delay_ms(1);

	qtmu0.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_5V, QTMU_PLUS_FILTER_PASS);
	qtmu0.SetStopInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_5V, QTMU_PLUS_FILTER_PASS);
	qtmu0.SetStartTrigger(1.6f, QTMU_PLUS_POS_SLOPE);
	qtmu0.SetStopTrigger(3.1f, QTMU_PLUS_POS_SLOPE);
	qtmu0.SetInSource(QTMU_PLUS_SINGLE_SOURCE);
	delay_ms(2);
	qtmu0.SetTimeOut(10);

    // EN CTL = 3.3v
	psEN.Set(FV, (float)3.3, FOVI_5V, FOVI_10MA, RELAY_ON);

	for(site=0; site < SITE_NUM; site++ )  {
		qtmu0.SetSinglePulseMeas(QTMU_PLUS_COARSE, QTMU_PLUS_TIME_US, site);
		qtmu0.SinglePlsMeas(site);
		ts3[site]=qtmu0.GetMeasureResult(site);
	}

    // TSS2
    // EN CTRL = 0V
	psEN.Set(FV, (float)0, FOVI_5V, FOVI_10MA, RELAY_ON);
	delay_ms(1);

	cbit.SetOn(VIN_FPVI, BIAS_FOVI, VIN_CAP, BIAS_CAP, EN_QTMU_B, REN_300KOhm, -1);	// ENC-CHA, EN-CHB, REN=300K
	delay_ms(1);
    
	qtmu0.SetStartInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_5V, QTMU_PLUS_FILTER_PASS);
	qtmu0.SetStopInput(QTMU_PLUS_IMPEDANCE_1M, QTMU_PLUS_VRNG_5V, QTMU_PLUS_FILTER_PASS);
	qtmu0.SetStartTrigger(1.8f, QTMU_PLUS_POS_SLOPE);
	qtmu0.SetStopTrigger(1.0f, QTMU_PLUS_POS_SLOPE);
	qtmu0.SetInSource(QTMU_PLUS_DUAL_SOURCE);
	delay_ms(2);
	qtmu0.SetTimeOut(10);

    // psEN = 2V
	psEN.Set(FV, (float)2, FOVI_5V, FOVI_10MA, RELAY_ON);

	for(site=0; site < SITE_NUM; site++ )  {
		qtmu0.SetSinglePulseMeas(QTMU_PLUS_COARSE, QTMU_PLUS_TIME_US, site);
		qtmu0.SinglePlsMeas(site);
		ts2[site]=qtmu0.GetMeasureResult(site);
	}
	
    for (site=0; site < SITE_NUM; site++)   {
      TS_ST1->SetTestResult(site, 0, ts1[site]);
      TS_ST2->SetTestResult(site, 0, ts2[site]);
	  TS_ST3->SetTestResult(site, 0, ts3[site]);
    }
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
	pVIN_FPVI.Set(FV, float(-1), FPVI10_1V, FPVI10_1MA, RELAY_ON);
	delay_ms(1);
	pVIN_FPVI.MeasureVI(10, 10);
	for (site = 0; site < SITE_NUM; site++)
		post_VIN->SetTestResult(site, 0, pVIN_FPVI.GetMeasResult(site, MVRET));
	pVIN_FPVI.Set(FV, 0.0, FPVI10_1V, FPVI10_1MA, RELAY_ON);
	// *************************************** VOUT ***************************************
	pOUT_FPVI.Set(FV,float(-1), FPVI10_1V, FPVI10_1MA, RELAY_ON);
	delay_ms(1);
	pOUT_FPVI.MeasureVI(10, 10);
	for (site = 0; site < SITE_NUM; site++)
		post_OUT->SetTestResult(site, 0, pOUT_FPVI.GetMeasResult(site, MVRET));
	pOUT_FPVI.Set(FV, 0.0, FPVI10_1V, FPVI10_1MA, RELAY_ON);
    // *************************************** EN ***************************************
	pEN.Set(FV, float(-1), FOVI_1V, FOVI_1MA, RELAY_ON);
	delay_ms(1);
	pEN.MeasureVI(10, 10);
	for (site = 0; site < SITE_NUM; site++)
		post_EN->SetTestResult(site, 0, pEN.GetMeasResult(site, MVRET));
	pEN.Set(FV, 0.0, FOVI_1V, FOVI_1MA, RELAY_ON);
    // *************************************** BIAS ***************************************
	pBIAS.Set(FV, float(-1), FOVI_1V, FOVI_1MA, RELAY_ON);
	delay_ms(1);
	pBIAS.MeasureVI(10, 10);
	for (site = 0; site < SITE_NUM; site++)
		post_BIAS->SetTestResult(site, 0, pBIAS.GetMeasResult(site, MVRET));
	pBIAS.Set(FV, 0.0, FOVI_1V, FOVI_1MA, RELAY_ON);
    
	PIN2OFF();
	return 0;
}
