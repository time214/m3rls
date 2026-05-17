// Dut.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
    }
	return TRUE;
}
//if needed, you can move the following functions to the file of test.cpp.
//Do not change the definition of these functions, or the pgs can not be loaded.
DUT_API void UserLoad()
{	
	/******
	if (AllocConsole())
	{
	SetConsoleTitleA("AccoTEST Debug Window(MT7621NFOR");
	freopen("conout$", "w+t", stderr);
	//::DeleteMenu(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_BYCOMMAND);
	}
	******/
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
DUT_API void UserInit()
{
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
DUT_API void UserExit()
{		
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
DUT_API void OnSot()
{
}
/************************************************************************/
/*                                                                      */
/************************************************************************/

DUT_API void BinOutDut()
{			
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
DUT_API void OnNewLot(const char *Lotid)
{
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
DUT_API void OnWaferEnd(const char *Lotid)
{			
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
