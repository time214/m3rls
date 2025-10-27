// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__3811CD50_B7B0_42B9_9E73_805A91708537__INCLUDED_)
#define AFX_STDAFX_H__3811CD50_B7B0_42B9_9E73_805A91708537__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>

#define DUT_API extern "C" __declspec(dllexport)
#include <string>
using namespace std;
#include "usertype.h"
#include "userres.h"


// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__3811CD50_B7B0_42B9_9E73_805A91708537__INCLUDED_)
// ******************************************************************* TEST MODE ******************************************************************
// TM1: FLTB = GND
// TM2: FLTB = 1.5v,	Reference Voltage
// TM3: FLTB = 3.3v,	C_CCx/C_SBUx OVP threshold
// Step1. Set V33E = 3.3v
// Step2. Pull VFLTP = -1v, hold 200us
// Step3. Release FLTB that is latched into TM
// Step4. When V33E < UVLO_OFF, exit TM
// TEST FLOW ***********************************************
// test0: Continuity
// test1: CCx OVP on resistance
// test2: DeadBattery pull down resistance
// test3: VTHDB
// test4: VOVP
// test5: CCx clamping voltage
// test6: CCx current limit
// test7: SBU OVP Resistance
// test8: SBU OVP Threshold voltage
// test9: SBU clamping voltage
// test10: SupplyCurrent
// test11: VPWR LoadSwitch Resistance
// test12: UVLO
// test13: VPWR Threshold and Hysteresis
// test14: VPWR DeadPWR voltage
// test15: VPWR current limit
// test16: FLTB output low threshold voltage
// test17: OVP timing
// test18: CCx on time
// test19: CCx recovery time
// test20: SBU recovery time
// test21: VREF
// test22: Continuity

// test23: SBU FET on time, removed
// test24: DieCheck, removed
// ******************************************************************* PIN DESCTIPTION ST *******************************************************************
// PACKAGE TYPE: QFN3320 w/EP
// PIN01:	C_SBU1
// PIN02:	C_SBU2
// PIN03:	VBIAS
// PIN04:	C_CC1
// PIN05:	C_CC2
// PIN06:	RPD2
// PIN07:	RPD1
// PIN09:	FLTB
// PIN10:	VPWR
// PIN11:	CC2
// PIN12:	CC1
// PIN14:	SBU2
// PIN15:	SBU1
// PIN16:	VBUS, NC
// PIN17:	V33E, NC
// PIN19:	NC
// PIN20:	NC
// PIN08:	GND
// PIN13:	GND
// PIN18:	GND
// ******************************************************************* PIN DESCTIPTION END *******************************************************************
// ******************************************************************* RELAY CONTROL ******************************************************************
// QTMU_0A: NC)S1_QTMU_A,		SWAP BY CBIT13 51ohm
// QTMU_0B: NC)S1_QTMU_B
// QTMU_1A: NC)S2_QTMU_A		SWAP BY CBIT45 resNP
// QTMU_1B: NC)S2_QTMU_B

// FLTB_100K, RPD1_Fovi, RPD2_Fovi, VPWR_Cap, V33E_Cap, V33E_Fovi				// POS
// IDC1, SITE1, SITE2
#define FLTB_100K					0, 64		// S1_K1
#define VPWR_Cap					1, 65   // S1_K2
#define VBIAS_Fovi        2, 66   // S1_K3 , VI
#define RPD1_Fovi					3, 67		// S1_K4 , VI
#define VPWR_Fpvi					4, 68   // S1_K5 , VI
#define FLTB_Fovi					5, 69   // S1_K6 , VI
#define SBU2_Fovi					6, 70   // S1_K7 , VI
#define RPD2_Fovi         7, 71   // S1_K8 , VI
#define C_CC1_Fpvi        8, 72   // S1_K9 , VI
#define C_CC2_Fovi				9, 73   // S1_K10, VI

#define FLTB_qtmuAB_SBU1 10, 74   // S1_K11, SBU1-QTMU0B, C_SBU1-QTMU0A
#define CC1_qtmuA        11, 75   // S1_K12, CC1-QTMU0A
#define QTMU_Swap        13, 77		// S1_K14, QTMU_SWAP
#define C_CC1_QTMU_A     15, 79		// S1-K16, C_CC1-QTMU0A
#define V33E_QTMU_B      42,106   // S2_K11, V33E-QTMU0B
#define CC1_QTMU_B			 43,107	  // S2_K12, CC1-QTMU0B

#define CC1_to_QTMU0B    46,110   // S2_K15
#define FLTB_QTMU_B      47,111   // S2_K16

#define C_CC2_Fpvi			 12, 76		// S1_K13
#define FPVI2_nonKelvin  14, 78   // S1_K15, VI
// IDC2
#define V33E_Cap         32, 96		// S2_K1
#define C_SBU2_Fovi			 34, 98   // S2_K3 , VI
#define VBUS_Fovi	       35, 99   // S2_K4 , VI
#define CC1_Fpvi				 36,100	  // S2_K5 , VI
#define V33E_Fovi        37,101   // S2_K6 , VI
#define SBU1_Fpvi        38,102		// S2_K7 , VI
#define CC2_Fovi	       39,103   // S2_K8 , VI
#define C_SBU1_Fpvi	     40,104   // S2_K9 , VI
#define CC2_Fpvi				 44,108		// S2_K13, VI

// ************************************ This project is laser trimming ************************************
// ************************************ This project is laser trimming ************************************
// ************************************ This project is laser trimming ************************************