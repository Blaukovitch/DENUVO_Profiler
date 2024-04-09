
/*
	DENUVO_Profiler v0.3
	--------------------
	CRACKLAB, ELF
	--------------------
	Tiberium reversing
	********************
	2011	-	2024
*/
#include "stdafx.h"

CBitmap LAMP_TRUE;
CBitmap LAMP_FALSE;

UINT64 DH_Main_Count; 
UINT64 EnumProcBuffer=-1;
 TCHAR buffer[MAX_PATH];
 TCHAR digital_buffer[(sizeof(UINT64)+sizeof(TCHAR)+3)*2]={'0','x'};
 BYTE Byte_buff[USHRT_MAX];

 WCHAR Profle_Folder[MAX_PATH];

 void test(UINT64 *paf)
{
*paf = 0xDEADC0DE;
};