/*
	DENUVO_Profiler v0.3
	--------------------
	CRACKLAB, ELF
	--------------------
	Tiberium reversing
	********************
	2011	-	2024
*/

#include "../stdafx.h"
#include "ENGINE_SPIRUN7.h"
#include "ENGINE.h"
#include "PROFILER_ENGINE.h"
#include "ENGINE_Profile_operations.h"
#include "ENGINE_VM_operations.h"
#include "ENGINE_MAP_operations.h"
#include "ENGINE_Symbol_avanpost.h"

TCHAR tHOOK_template[MAX_PATH] = L"JMP $1";

BYTE tNOP_SLED[] =  {0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};

BYTE tLOCK_MARKER[] = {LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def,
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, 
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def,
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, 
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def,
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, 
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def,
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, 
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def,
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, 
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def,
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, 
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def,
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, 
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def,
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, 
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def,
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, 
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def,
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, 
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def,
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, 
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def,
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, 
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def,
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, 
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def,
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, 
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def,
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, 
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def,
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, 
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def,
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, 
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def,
LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def, LM_def, LM_def,LM_def};

bool DH_File_change_binary_dump(LPWSTR Dest_file, LPWSTR Src_binary_dump, size_t Dest_file_raw_offset, size_t Size_of_dump_data)
{
	 HANDLE hFile_dest = CreateFile(Dest_file,                // name of the write
                       GENERIC_WRITE,          // 
                       FILE_SHARE_WRITE,       // WRITE
                       NULL,                   // default security
                       OPEN_EXISTING,             // NEW
                       FILE_ATTRIBUTE_NORMAL,  // normal file
                       NULL);
		 if (hFile_dest == INVALID_HANDLE_VALUE) 
			  return false;

		 HANDLE hFile_src = CreateFile(Src_binary_dump,                // name of the write
                       GENERIC_READ,          // 
                       FILE_SHARE_READ|FILE_SHARE_WRITE,       // RW
                       NULL,                   // default security
                       OPEN_EXISTING,             // NEW
                       FILE_ATTRIBUTE_NORMAL,  // normal file
                       NULL);
		 if (hFile_src == INVALID_HANDLE_VALUE) 
			  goto error_0;

		 DWORD Hi_bound=0;
		 DWORD File_size = ::GetFileSize(hFile_src,&Hi_bound);
			if (File_size < Size_of_dump_data)
				 Size_of_dump_data = File_size;

		 File_size = ::GetFileSize(hFile_dest,&Hi_bound); 
			if (File_size < Size_of_dump_data)
			 Size_of_dump_data = File_size;

		 PVOID Allocate_DUMP = ::VirtualAlloc(NULL,Size_of_dump_data,MEM_COMMIT|MEM_COMMIT,PAGE_READWRITE);

		 if (Allocate_DUMP == NULL)
			 goto error_0;

		 DWORD dwPtr = SetFilePointer(hFile_dest, 
                                Dest_file_raw_offset, 
                                NULL, 
                                FILE_BEGIN ); 
	 if (dwPtr == INVALID_SET_FILE_POINTER) // Test for failure
			 goto error_0;

	 DWORD Return_Bytes=0;
		::ReadFile(hFile_src,Allocate_DUMP,Size_of_dump_data,&Return_Bytes,NULL);
		bool bErrorFlag =  ::WriteFile(hFile_dest,(PVOID)((UINT64)Allocate_DUMP+0x12E0),Size_of_dump_data,&Return_Bytes,NULL);
		FlushFileBuffers(hFile_dest);

		if (bErrorFlag == false)
         goto error_0;

     ::CloseHandle(hFile_dest);
	 ::CloseHandle(hFile_src);
	 ::VirtualFree(Allocate_DUMP,0,MEM_RELEASE);

      



	return true;

error_0:
	 ::CloseHandle(hFile_dest);
	 ::CloseHandle(hFile_src);
	return false;
}

bool DH_File_fill_addres_cells(LPWSTR Dest_file, LPWSTR Src_binary_dump, UINT64 File_load_Base_Address)
{
	 HANDLE hFile_dest = CreateFile(Dest_file,                // name of the write
                       GENERIC_WRITE,          // 
                       FILE_SHARE_WRITE,       // WRITE
                       NULL,                   // default security
                       OPEN_EXISTING,             // NEW
                       FILE_ATTRIBUTE_NORMAL,  // normal file
                       NULL);
		 if (hFile_dest == INVALID_HANDLE_VALUE) 
			  goto error_1;

		 HANDLE hFile_src = CreateFile(Src_binary_dump,                // name of the write
                       GENERIC_READ,          // 
                       FILE_SHARE_READ|FILE_SHARE_WRITE,       // RW
                       NULL,                   // default security
                       OPEN_EXISTING,             // NEW
                       FILE_ATTRIBUTE_NORMAL,  // normal file
                       NULL);
		 if (hFile_src == INVALID_HANDLE_VALUE) 
			  goto error_1;

		 DWORD Hi_bound=0;
		 DWORD File_size = ::GetFileSize(hFile_src,&Hi_bound); 

		 DWORD Source_bound = ::GetFileSize(Dest_file,&Hi_bound); 

		  PVOID Allocate_DUMP_cell_struct = ::VirtualAlloc(NULL,File_size,MEM_COMMIT|MEM_COMMIT,PAGE_READWRITE);

		  pDUMP_CELL_STRUCT Cell_pointer = (pDUMP_CELL_STRUCT)Allocate_DUMP_cell_struct;
		  BOOL bErrorFlag=false;
		   DWORD Return_Bytes=0;

		   ::ReadFile(hFile_src,Cell_pointer,File_size,&Return_Bytes,NULL);

		  do
		  {
			  if( Cell_pointer->Cell_mem_jmp-File_load_Base_Address < Source_bound)
			  {
			  	 DWORD dwPtr = SetFilePointer(hFile_dest, 
                               Cell_pointer->Cell_mem_jmp-File_load_Base_Address, 
                                NULL, 
                                FILE_BEGIN );
				// UINT64 A = (UINT64)Allocate_DUMP_cell_struct+File_size-0x100;
				// ::ReadFile(hFile_src,(LPVOID)A,8,&Return_Bytes,NULL);
				bErrorFlag = ::WriteFile(hFile_dest,&Cell_pointer->Operand_Address,sizeof(Cell_pointer->Operand_Address),&Return_Bytes,NULL);
			  }
			  Cell_pointer++;
		  }while(Cell_pointer->Cell_mem_jmp != 0);

		FlushFileBuffers(hFile_dest);

		

		if (bErrorFlag == false)
         goto error_1;

     ::CloseHandle(hFile_dest);
	 ::CloseHandle(hFile_src);
	 ::VirtualFree(Allocate_DUMP_cell_struct,0,MEM_RELEASE);

return true;

error_1:
	 ::CloseHandle(hFile_dest);
	 ::CloseHandle(hFile_src);
	return false;
}

pSET_FILES_COMPARE_EVENT __fastcall DH_PE_Files_compare(__in HMODULE PE_File_1, __in HMODULE PE_File_2, __out LPBYTE* ppSET_FILES_COMPARE_BYTES_buffer, __in __out pMAP_IDENTICAL_SECTIONS_STACK pIdentSections)
{
	
		if (PE_File_1 == NULL)
			return NULL;
		if (PE_File_2 == NULL)
			return NULL;
		if ((pIdentSections->NumOfSection_ALL == 0))
			return NULL;
		
		
		DWORD Image_size = pIdentSections->Protection_SECTION_1->SizeOfRawData; //register DWORD Image_size = DH_get_PE_DUMP_image_size((HANDLE)COMMUNICATE_CURRENT_PROCESS, PE_File_1);
		DWORD Image_size_2 = pIdentSections->Protection_SECTION_2->SizeOfRawData;//register DWORD Image_size_2 = DH_get_PE_DUMP_image_size((HANDLE)COMMUNICATE_CURRENT_PROCESS, PE_File_2);

		if (Image_size > Image_size_2)
			Image_size = Image_size_2;


		LPVOID Allocation_Diff_BYTES_Buffer = malloc(Image_size*2); //буффер под отличающейся байты 
		
		if(Allocation_Diff_BYTES_Buffer == NULL)
			return NULL;
		//pHEADER_FILES_COMPARE_RESULTS pHeader_compare_results = (pHEADER_FILES_COMPARE_RESULTS)malloc(sizeof(HEADER_FILES_COMPARE_RESULTS)); //заголовок файла записи результатов
		SERVICE_COUNT Sets_count = NULL; //счетчик кол-ва найденных различий

		DWORD Control_events_save = DEFAULT_NUM_OF_COMPARE_EVENTS; //max допустимое контрольное кол-во блоков для записи в текущий регион память
		LPVOID Allocation_Diff_EVENTs = (pSET_FILES_COMPARE_EVENT)malloc(sizeof(SET_FILES_COMPARE_EVENT)*Control_events_save); //плавающий указатель

		register pSET_FILES_COMPARE_EVENT poDiff_bytes_event = (pSET_FILES_COMPARE_EVENT)Allocation_Diff_EVENTs;

		LPBYTE poWriteBytesBuffer = (LPBYTE)Allocation_Diff_BYTES_Buffer; //буфер для записи разных байт
		

		register LPBYTE poFile_1 = (LPBYTE)PE_File_1 + pIdentSections->Protection_SECTION_1->VirtualAddress; //плавающий указатель №1 
		register LPBYTE poFile_2 = (LPBYTE)PE_File_2 + pIdentSections->Protection_SECTION_2->VirtualAddress; //плавающий указатель №2

		//set limits: 00000001413BB000 - 0000000144CDB216 / hmudule = 0000000140000000
		//poFile_1+= 0x13BB000;
		//poFile_2+= 0x13BB000;

		//LPBYTE secondary_poFile_1 = poFile_1; //плавающий указатель №1 для  child-цикла
		//LPBYTE secondary_poFile_2 = poFile_2; //плавающий указатель №2 для  child-цикла

		register SERVICE_COUNT i = Image_size; //кол-во полных итераций главного цикла
	//	DWORD secondary_i = i; //кол-во полных итераций главного цикла для child-цикла
		//DWORD a; //кол-во отличающихся байт
		//DWORD Save_i; //для подсчета кол-ва разных байт в одном заходе

		//LPBYTE Cycle_pointer_1; 
		//ULONGLONG File_ImageBase = 0x00000140000000;
		

		poDiff_bytes_event->Diff_Bytes_Count = local_find_next_byte_differece_in_DUMP(&poFile_1, &poFile_2, &i);
		//	ULONG64* dump_set = (ULONG64*)malloc(430 * sizeof(QWORD)); //spec
			//__asm
		//	{
			//	rdtsc
			//}
		if(poDiff_bytes_event->Diff_Bytes_Count == NULL)
			{
			pIdentSections->compare_event_table_count = 0;
			free(Allocation_Diff_EVENTs);
			free(Allocation_Diff_BYTES_Buffer);
			return (pSET_FILES_COMPARE_EVENT)MAP_RETURN_COMPARE_EQ_FILES;
			}
		do//главный цикл
		{	
			poDiff_bytes_event->Diff_bytes_file_1_DUMP_offset = (LPBYTE)(poWriteBytesBuffer - (LPBYTE)Allocation_Diff_BYTES_Buffer);
			memcpy(poWriteBytesBuffer, poFile_1, poDiff_bytes_event->Diff_Bytes_Count);
			poWriteBytesBuffer += poDiff_bytes_event->Diff_Bytes_Count;
			poDiff_bytes_event->Diff_bytes_file_2_DUMP_offset = (LPBYTE)(poWriteBytesBuffer - (LPBYTE)Allocation_Diff_BYTES_Buffer);
			memcpy(poWriteBytesBuffer, poFile_2, poDiff_bytes_event->Diff_Bytes_Count);
			poWriteBytesBuffer += poDiff_bytes_event->Diff_Bytes_Count;	
				
		
		//	if(poDiff_bytes_event->Diff_Bytes_Count == 4)
			{
				poDiff_bytes_event->Start_diff_Address= pIdentSections->AnyFILE_1_2_IMAGEBASE + ((SERVICE_ADDRES)poFile_1 - (SERVICE_ADDRES)PE_File_1);
				ULONG64 test2 = pIdentSections->AnyFILE_1_2_IMAGEBASE + ((SERVICE_ADDRES)poFile_2 - (SERVICE_ADDRES)PE_File_2); //TST
				//skip  import
				/*
				if (poDiff_bytes_event->Start_diff_Address > 0x0000145652000) //0000000145652000 -sw3 //0x00000141BF8FF8 - клубок
				{
					if (poDiff_bytes_event->Start_diff_Address < 0x0000014B280F93) //000000014B280F93 //0x00000141BFA310 - клубок
						goto get_new;
				}*/
			}	
				

				poFile_1 += poDiff_bytes_event->Diff_Bytes_Count;
				poFile_2 += poDiff_bytes_event->Diff_Bytes_Count;

				*poDiff_bytes_event++;
				Sets_count++;

			

				if(Sets_count >= Control_events_save-1) //recalculate выделенную память
				{ 
					pIdentSections->compare_event_table_count = 0;
					free(Allocation_Diff_EVENTs);
					free(Allocation_Diff_BYTES_Buffer);
					return (pSET_FILES_COMPARE_EVENT)MAP_RETURN_COMPARE_TOO_MANY_DIFFERECES;
					/*
					Control_events_save+=DEFAULT_NUM_OF_COMPARE_EVENTS; //увеличение числа допустимых записей
					LPVOID Old_Allocation_region = Allocation_Diff_EVENTs; //сохранение старого значения
					Allocation_Diff_EVENTs = realloc(Old_Allocation_region, Control_events_save*sizeof(SET_FILES_COMPARE_EVENT)); //выделение нового большего региона
					poDiff_bytes_event =  (pSET_FILES_COMPARE_EVENT)((ULONG64)Allocation_Diff_EVENTs + (poDiff_bytes_event - Old_Allocation_region));//корректный пересчет указателя
					*/
				}
		//get_new:
			poDiff_bytes_event->Diff_Bytes_Count = local_find_next_byte_differece_in_DUMP(&poFile_1, &poFile_2, &i);

		}while(poDiff_bytes_event->Diff_Bytes_Count != NULL);
compare_return_success:
		
	memcpy(&pIdentSections->File_1_protection_section_header, pIdentSections->Protection_SECTION_1,sizeof(IMAGE_SECTION_HEADER));
	pIdentSections->compare_event_table_count = Sets_count; //возвращаем кол-во блоков в таблице различий
	*ppSET_FILES_COMPARE_BYTES_buffer = (LPBYTE)Allocation_Diff_BYTES_Buffer; //возвращаем указатель на буффер байт для таблицы различий
	return (pSET_FILES_COMPARE_EVENT)Allocation_Diff_EVENTs; //возвращаем указатель на таблицу различий
}

__inline ULONG local_find_next_byte_differece_in_DUMP(
	__in __out LPBYTE *Current_file_DUMP_1_pointer, //первый указатель на дамп файла №1
	__in __out LPBYTE *Current_file_DUMP_2_pointer, //первый указатель на дамп файла №2
	__in __out SERVICE_COUNT* iteration_count //кол-во оставшихся итераций
	)
{

	*Current_file_DUMP_1_pointer = *Current_file_DUMP_1_pointer + 1;
	*Current_file_DUMP_2_pointer = *Current_file_DUMP_2_pointer + 1;
	LPBYTE First_diff_byte;
find_first_byte_diff:
	do//главный цикл
		{
	//		if(*iteration_count == 0x000003da)
		//		 return false;

		if (**Current_file_DUMP_1_pointer != **Current_file_DUMP_2_pointer) //если разные байты
			//конец условия разницы байт
			{
				LPBYTE Enumeration_Pointer_1 = *Current_file_DUMP_1_pointer;
				LPBYTE Enumeration_Pointer_2 = *Current_file_DUMP_2_pointer;
				do//главный цикл
				{
					if (*Enumeration_Pointer_1 == *Enumeration_Pointer_2) //если встретились одинаковые байты
						return (Enumeration_Pointer_1 - *Current_file_DUMP_1_pointer); //возвращаем разницу


					Enumeration_Pointer_1++;
					Enumeration_Pointer_2++;
				} while (*iteration_count = *iteration_count - 1);
				return false;
			}//END IF 

			*Current_file_DUMP_1_pointer = *Current_file_DUMP_1_pointer + 1;
			*Current_file_DUMP_2_pointer = *Current_file_DUMP_2_pointer + 1;
		}while(*iteration_count = *iteration_count - 1);

	return false;
}


HMODULE DH_Load_PE_as_datafile(__in LPWSTR PE_File_path)
{
	HMODULE hmFile_PE = ::LoadLibraryExW(PE_File_path, NULL, NULL);
	
	if (hmFile_PE == NULL)
		return NULL;
	//hmFile_PE = (HMODULE)((ULONG64)hmFile_PE - 1); - fix for  LOAD_FILE_AS_DATABASE
	return hmFile_PE;
}


PIMAGE_SECTION_HEADER DH_MAP_Get_main_protection_section_point(__in HMODULE hmModule)
{
	PIMAGE_DOS_HEADER dosHeader;        //Pointer to DOS Header
   register PIMAGE_NT_HEADERS ntHeader;         //Pointer to NT Header
   
	
	dosHeader = (PIMAGE_DOS_HEADER)hmModule;
	ntHeader = PIMAGE_NT_HEADERS64(dosHeader->e_lfanew + (ULONG64)dosHeader);

	PIMAGE_SECTION_HEADER pSecHeader_vpp = (PIMAGE_SECTION_HEADER)((ULONG_PTR)(ntHeader) + FIELD_OFFSET(IMAGE_NT_HEADERS, OptionalHeader)	+ ntHeader->FileHeader.SizeOfOptionalHeader);

	signed long i =  ntHeader->FileHeader.NumberOfSections;

	if(i < 2)
		return NULL;
	i--; //не учитывать последнюю секцию

	PIMAGE_SECTION_HEADER Section_point = (PIMAGE_SECTION_HEADER)pSecHeader_vpp;
	do
	{
		
		if(Section_point->SizeOfRawData < pSecHeader_vpp->SizeOfRawData)
			Section_point = pSecHeader_vpp;

		pSecHeader_vpp++;
	}while(--i);

return Section_point;
}

////////////////////////////////// P * A * T * C * H //////////////////////////////
UINT DH_PATCH_Add_new_vmp_execute_address_patch(SERVICE_ADDRES Critical_address_array[], 
	SERVICE_ARRAY_ID Critical_address_relative_addrpool_ID_array[], 
	WCHAR* owner_filename,
	size_t i_Containt_handler_ArrayID, 
	SERVICE_TYPE Addres_asm_instruction_type, 
	SERVICE_TYPE Addres_owner_type_of_profile)
{
	if (Critical_address_array == NULL) 
		return NULL;

	/////////// allocate memory
	pPATCH_INFO_STACK local_patch_info = (pPATCH_INFO_STACK)malloc(sizeof(PATCH_INFO_STACK));
		memset(local_patch_info, NULL, sizeof(PATCH_INFO_STACK));
		
	/////////// build info stack
	//local_patch_info->Containt_HELD_GROUP[0] = (pPATCH_HELD_GROUP)((ULONG64)local_patch_info+sizeof(PATCH_INFO_STACK)); //update указателя на HELD_GROUP
	local_patch_info->owner_as_profile__of_HELD_GROUP = Addres_owner_type_of_profile; //владелец патча
	local_patch_info->type_of_target_addres = Addres_asm_instruction_type; //тип инструкции
	local_patch_info->is_new_added = true; //новый патч
	local_patch_info->is_not_applied = true; // "NOT_APPLIED" флаг
	local_patch_info->last_error_status = ERROR_SUCCESS; //last error
	itoa_16_fast(Critical_address_array[0],&local_patch_info->name[0]);

	 //(ОПЦИОНАЛЬНО)//
	if(owner_filename != NULL)
		memcpy(&local_patch_info->owner_name[0], owner_filename, wcslen(owner_filename)*sizeof(WCHAR)); //имя файла-владельца патча

	local_patch_info->i_Containt_handler_ArrayID = i_Containt_handler_ArrayID;
//	if (Parent_subroutine_address != NULL)
	//	local_patch_info->parent_subroutine_address = Parent_subroutine_address; //адрес начала sub_
		
	/////////// copy critical address
	register size_t i = MAX_HANHELD_TYPE_DETERMINE_CRITICAL_ADDRESES-1;
	pPATCH_HELD_GROUP pHeldFLOW_POINTER = (pPATCH_HELD_GROUP)&local_patch_info->Containt_HELD_GROUP[0];
	local_patch_info->HELD_GROUP_Fill_count = NULL;
	Critical_address_array++;
	Critical_address_relative_addrpool_ID_array++;
	pHeldFLOW_POINTER--;
	do ///copy Critical_address_array cycle 
	{
		Critical_address_array--;
		Critical_address_relative_addrpool_ID_array--;
		pHeldFLOW_POINTER++;
			if (*Critical_address_array == NULL)
				continue;
		//////////////////////////////
		pHeldFLOW_POINTER->target_addres = *Critical_address_array;
	//	if(*Critical_address_array == 0x000000014170f829)
		//	Beep(0,0);
		pHeldFLOW_POINTER->Containt_HELD_GROUP_relative_to_AddrPool_ID = *Critical_address_relative_addrpool_ID_array;
		local_patch_info->HELD_GROUP_Fill_count++;
	}while(i--);// OLD - memcpy(&local_patch_info->Containt_HELD_GROUP[0], &Critical_address_array[0], sizeof(SERVICE_ADDRES) * MAX_HANHELD_TYPE_DETERMINE_CRITICAL_ADDRESES);
	
	
	ULONG RetBytes;

	/// write profile data 
	bool write_result = DH_Write_Current_hunt_profile_CONTAINER(PROFILE_TYPE_PATCH__NOT_APPLIED_FLAG,&local_patch_info->name[0],(BYTE*)local_patch_info,sizeof(PATCH_INFO_STACK),&RetBytes);

	/////////// free allocated memory
	free(local_patch_info);
}


UINT DH_PATCH_Add_new_vmp_DATA_address_patch(SERVICE_ADDRES Patch_start_coverage_address, 
	size_t Size_Of_patch_data,
	WCHAR* owner_filename,
	SERVICE_TYPE Addres_owner_type_of_profile)
{
	if (Patch_start_coverage_address == NULL) 
		return NULL;

	/////////// allocate memory
	pPATCH_INFO_STACK local_patch_info = (pPATCH_INFO_STACK)malloc(SIZE__PATH_FILE_STACK_and_HELD_GROUP);
		memset(local_patch_info, NULL, SIZE__PATH_FILE_STACK_and_HELD_GROUP);

	/////////// build info stack
	//local_patch_info->Containt_HELD_GROUP[0] = (pPATCH_HELD_GROUP)((ULONG64)local_patch_info+sizeof(PATCH_INFO_STACK)); //update указателя на HELD_GROUP
	local_patch_info->owner_as_profile__of_HELD_GROUP = Addres_owner_type_of_profile; //владелец патча
	local_patch_info->Containt_HELD_GROUP[0].patch_COUNT_original_target_instruction_bytes = Size_Of_patch_data; //кол-во покрываемых байт

	local_patch_info->is_new_added = true; //новый патч
	local_patch_info->is_not_applied = true; // "NOT_APPLIED" флаг
	local_patch_info->last_error_status = ERROR_SUCCESS; //last error
	itoa_16_fast(Patch_start_coverage_address,&local_patch_info->name[0]);

	 //(ОПЦИОНАЛЬНО)//
	if(owner_filename != NULL)
		memcpy(&local_patch_info->owner_name[0], owner_filename, wcslen(owner_filename)*sizeof(WCHAR)); //имя файла-владельца патча


	/////////// copy critical address
	memcpy(&local_patch_info->Containt_HELD_GROUP[0], &Patch_start_coverage_address, sizeof(SERVICE_ADDRES) * MAX_HANHELD_TYPE_DETERMINE_CRITICAL_ADDRESES);

	ULONG RetBytes;

	/// write profile data 
	bool write_result = DH_Write_Current_hunt_profile_CONTAINER(PROFILE_TYPE_PATCH__NOT_APPLIED_FLAG,&local_patch_info->name[0],(BYTE*)local_patch_info,SIZE__PATH_FILE_STACK_and_HELD_GROUP,&RetBytes);

	/////////// free allocated memory
	free(local_patch_info);
}


TCHAR* __fastcall DH_VM_PATCH_OWNERS_type_to_string(SERVICE_TYPE owner_type)
 {
 switch(owner_type)
			{
			case PATCH_OWNER_VM_INSIDE_EXECUTE: {return _T("VM_in_Execute");break;}
			case PATCH_OWNER_VM_OUTSIDE_EXECUTE: {return _T("VM_out_Execute");break;}
			
			case PATCH_OWNER_VM_COVERAGE: {return _T("VM_COVERAGE");break;}

			case PATCH_OWNER_VM_SUPPLY15_DATA: {return _T("VM_Supply15_Data");break;}
			case PATCH_OWNER_VM_PCODE_DATA: {return _T("VM_pcode_Data");break;}
			case PATCH_OWNER_VM_CONSTANT_DATA: {return _T("VM_const_Data");break;}
			}
 return NULL;
 }

pPATCH_SUPPORTING_REGION_INI_STACK __fastcall DH_MAP_Release_patch_region_config(void)
{
	register pPATCH_SUPPORTING_REGION_INI_STACK pPatch_stack;
		if(HUNT_HIVE.PATCH_Support_region_stack.allocated_pool == NULL)
		{
			DWORD reta = NULL;
			pPatch_stack = (pPATCH_SUPPORTING_REGION_INI_STACK)DH_Read_Current_hunt_profile_CONTAINER(PROFILE_TYPE_PATCH_REGIONS_INFO_INI,TS_DEF_NA,PROFILE_READ_ALL, &reta);

			if(pPatch_stack == NULL)
				return NULL;

			//clear validate bits
			pPatch_stack->is_Region_VMP_patch_map_validate = false;
			pPatch_stack->is_Region_VMP_original_copy_validate = false;
			pPatch_stack->is_Region_VMP_hook_and_filter_validate = false;
			
			//INIT PATCH struct
			HUNT_HIVE.PATCH_Support_region_stack.allocated_pool = (LPBYTE)pPatch_stack;
			HUNT_HIVE.PATCH_Support_region_stack.region_size = sizeof(PATCH_SUPPORTING_REGION_INI_STACK);
			HUNT_HIVE.PATCH_Support_region_stack.type_of_region = REGION_TYPE_PATCH_SUPPORT_REGIONS_STRUCT;
		}
		else
			pPatch_stack = (pPATCH_SUPPORTING_REGION_INI_STACK)HUNT_HIVE.PATCH_Support_region_stack.allocated_pool;

	
	
	if(pPatch_stack->VMP_Section_size == NULL || pPatch_stack->VMP_Section_start_address == NULL)
		return NULL;

	register MEMORY_BASIC_INFORMATION mem_basic_nfo;
	memset(&mem_basic_nfo,0, sizeof(mem_basic_nfo));

	pPatch_stack->All_Support_regions_retail_size = NULL;

	//register pDH_APOST_symbol_stack pTmpSymbol = (pDH_APOST_symbol_stack)malloc(sizeof(DH_APOST_symbol_stack));
		//memset(&pTmpSymbol,NULL, sizeof(DH_APOST_symbol_stack));
		

	if(pPatch_stack->is_Region_VMP_original_copy_auto_alloc == true && pPatch_stack->is_Region_VMP_original_copy_validate == false)
		//{pPatch_stack->Region_VMP_original_copy_start = (SERVICE_ADDRES)malloc(pPatch_stack->VMP_Section_size);}
	{
		if (pPatch_stack->Region_VMP_original_copy_start = (SERVICE_ADDRES) ::VirtualAllocEx(HUNT_HIVE.tOpenProcHANDLE,NULL,pPatch_stack->VMP_Section_size,MEM_COMMIT|MEM_COMMIT,PAGE_READWRITE) != NULL)
		{
			::VirtualQueryEx(HUNT_HIVE.tOpenProcHANDLE,&pPatch_stack->Region_VMP_original_copy_start,&mem_basic_nfo,sizeof(mem_basic_nfo));
			pPatch_stack->Region_VMP_original_copy_end = mem_basic_nfo.RegionSize + pPatch_stack->Region_VMP_patch_map_start;
			pPatch_stack->All_Support_regions_retail_size += mem_basic_nfo.RegionSize;
		}//end if
	}//end if


	if(pPatch_stack->is_Region_VMP_patch_map_auto_alloc == true && pPatch_stack->is_Region_VMP_patch_map_validate == false)
	//	{pPatch_stack->Region_VMP_patch_map_start = (SERVICE_ADDRES)malloc(pPatch_stack->VMP_Section_size);}
	{

		pPatch_stack->Region_VMP_patch_map_start = (SERVICE_ADDRES) ::VirtualAllocEx(HUNT_HIVE.tOpenProcHANDLE,NULL,pPatch_stack->VMP_Section_size,MEM_COMMIT|MEM_COMMIT,PAGE_READWRITE);
		if (pPatch_stack->Region_VMP_patch_map_start != NULL);
		{
			::VirtualQueryEx(HUNT_HIVE.tOpenProcHANDLE,&pPatch_stack->Region_VMP_patch_map_start,&mem_basic_nfo,sizeof(mem_basic_nfo));
			//pPatch_stack->Region_VMP_patch_map_end = mem_basic_nfo.RegionSize + pPatch_stack->Region_VMP_patch_map_start;
			pPatch_stack->Region_VMP_patch_map_end =  pPatch_stack->VMP_Section_size + pPatch_stack->Region_VMP_patch_map_start;
			pPatch_stack->All_Support_regions_retail_size += mem_basic_nfo.RegionSize;
		}//end if

	}//end if

	if(pPatch_stack->is_Region_VMP_hook_and_filter_auto_alloc == true && pPatch_stack->is_Region_VMP_hook_and_filter_validate == false )
		//{pPatch_stack->Region_VMP_original_copy_start = (SERVICE_ADDRES)malloc(DEFAULT_Region_VMP_hook_and_filter_AUTO_ALLOCATION_SIZE);}
	{
		if (pPatch_stack->Region_VMP_hook_and_filter_start = (SERVICE_ADDRES) ::VirtualAllocEx(HUNT_HIVE.tOpenProcHANDLE,NULL,DEFAULT_Region_VMP_hook_and_filter_AUTO_ALLOCATION_SIZE,MEM_COMMIT|MEM_COMMIT,PAGE_EXECUTE_READWRITE) != NULL)
		{
			::VirtualQueryEx(HUNT_HIVE.tOpenProcHANDLE,&pPatch_stack->Region_VMP_hook_and_filter_start,&mem_basic_nfo,sizeof(mem_basic_nfo));
			pPatch_stack->Region_VMP_hook_and_filter_end = mem_basic_nfo.RegionSize + pPatch_stack->Region_VMP_hook_and_filter_start;
			pPatch_stack->All_Support_regions_retail_size += mem_basic_nfo.RegionSize;
		}//end if
	}//end if

	//validate regions + write symboilic data
	//pDH_APOST_symbol_stack pTmpSymbol = (pDH_APOST_symbol_stack)malloc(sizeof(DH_APOST_symbol_stack));
	//memset(&pTmpSymbol,NULL, sizeof(DH_APOST_symbol_stack));

	size_t reat_buf;
	if ((pPatch_stack->Region_VMP_original_copy_start != NULL) && (!::DH_ReadProcessAddres_DATA(&HUNT_HIVE.tOpenProcHANDLE, &pPatch_stack->Region_VMP_original_copy_start, sizeof(DWORD), &reat_buf)) && (!::DH_ReadProcessAddres_DATA(&HUNT_HIVE.tOpenProcHANDLE, &pPatch_stack->Region_VMP_original_copy_end-sizeof(QWORD), sizeof(DWORD), &reat_buf)))
		pPatch_stack->is_Region_VMP_original_copy_validate = true;
	else
		pPatch_stack->is_Region_VMP_original_copy_validate = false;

	if ((pPatch_stack->Region_VMP_patch_map_start != NULL) && (!::DH_ReadProcessAddres_DATA(&HUNT_HIVE.tOpenProcHANDLE, &pPatch_stack->Region_VMP_patch_map_start, sizeof(DWORD), &reat_buf)) && (!::DH_ReadProcessAddres_DATA(&HUNT_HIVE.tOpenProcHANDLE, &pPatch_stack->Region_VMP_patch_map_end-sizeof(QWORD), sizeof(DWORD), &reat_buf)))
		pPatch_stack->is_Region_VMP_patch_map_validate = true;
	else
		pPatch_stack->is_Region_VMP_patch_map_validate = false;

	if ((pPatch_stack->Region_VMP_hook_and_filter_start != NULL) && (!::DH_ReadProcessAddres_DATA(&HUNT_HIVE.tOpenProcHANDLE, &pPatch_stack->Region_VMP_hook_and_filter_start, sizeof(DWORD), &reat_buf)) && (!::DH_ReadProcessAddres_DATA(&HUNT_HIVE.tOpenProcHANDLE, &pPatch_stack->Region_VMP_hook_and_filter_end-sizeof(QWORD), sizeof(DWORD), &reat_buf)))
		pPatch_stack->is_Region_VMP_hook_and_filter_validate = true;
	else
		pPatch_stack->is_Region_VMP_hook_and_filter_validate = false;

	if ((pPatch_stack->VMP_Section_start_address != NULL) && (!::DH_ReadProcessAddres_DATA(&HUNT_HIVE.tOpenProcHANDLE, &pPatch_stack->VMP_Section_start_address, sizeof(DWORD), &reat_buf)) && (!::DH_ReadProcessAddres_DATA(&HUNT_HIVE.tOpenProcHANDLE, &pPatch_stack->VMP_Section_start_address+pPatch_stack->VMP_Section_size-sizeof(QWORD), sizeof(DWORD), &reat_buf)))
		pPatch_stack->is_VMP_section_validate = true;
	else
		pPatch_stack->is_VMP_section_validate = false;

	/*
	if(pPatch_stack->is_VMP_section_validate == true && pPatch_stack->is_Region_VMP_original_copy_validate == true && pPatch_stack->is_Region_VMP_hook_and_filter_validate == true && pPatch_stack->is_Region_VMP_patch_map_validate == true)
	{
		//HI vm section
		pTmpSymbol->Symbol_SHORT_assign = FILTER_SUBDEFINE_CONTROL_SYMBOL_VM_HI_BORDER;
		pTmpSymbol->Symbol_type_as_enum = APOST_HI_VM_SECTION_BORDER;
		pTmpSymbol->Variable = pPatch_stack->VMP_Section_start_address + pPatch_stack->VMP_Section_size;
		DH_APOST_ADDNS(pTmpSymbol);
		//LO vm section
		pTmpSymbol->Symbol_SHORT_assign = FILTER_SUBDEFINE_CONTROL_SYMBOL_VM_LO_BORDER;
		pTmpSymbol->Symbol_type_as_enum = APOST_LO_VM_SECTION_BORDER;
		pTmpSymbol->Variable = pPatch_stack->VMP_Section_start_address;
		DH_APOST_ADDNS(pTmpSymbol);
	}*/

	return (pPATCH_SUPPORTING_REGION_INI_STACK)HUNT_HIVE.PATCH_Support_region_stack.allocated_pool;
}

bool __fastcall DH_MAP_Close_patch_region_config(pPATCH_SUPPORTING_REGION_INI_STACK pPatch_stack)
{
	if(pPatch_stack->is_Region_VMP_original_copy_auto_alloc == true)
		//{free((void*)pPatch_stack->Region_VMP_original_copy_start);}
		::VirtualFreeEx(HUNT_HIVE.tOpenProcHANDLE, (LPVOID*)pPatch_stack->Region_VMP_original_copy_start, 0, MEM_RELEASE);

	if(pPatch_stack->is_Region_VMP_patch_map_auto_alloc == true)
		//{free((void*)pPatch_stack->Region_VMP_patch_map_start);}
		::VirtualFreeEx(HUNT_HIVE.tOpenProcHANDLE, (LPVOID*)pPatch_stack->Region_VMP_patch_map_start, 0, MEM_RELEASE);

	if(pPatch_stack->is_Region_VMP_hook_and_filter_auto_alloc == true)
		//{free((void*)pPatch_stack->Region_VMP_original_copy_start);}
		::VirtualFreeEx(HUNT_HIVE.tOpenProcHANDLE, (LPVOID*)pPatch_stack->Region_VMP_hook_and_filter_start, 0, MEM_RELEASE);
}

//P * A * T * C * H  DO APPLY N/A STACK
size_t __fastcall DH_MAP_Do_APPLY_NA_STACK(pPATCH_SUPPORTING_REGION_INI_STACK pPatch_stack)
{
	pDH_ENUM_FILES_of_PROFILES pPatch_na_contant = (pDH_ENUM_FILES_of_PROFILES)::VirtualAlloc(NULL, sizeof(DH_ENUM_FILES_of_PROFILES[MAX_CONTAINTER_FILES]), MEM_COMMIT, PAGE_READWRITE);

	//TCHAR current_loaded_VMstack[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN]; - optimize

	register size_t i = DH_Get_Current_hunt_profile_ALL_FILE_CONTAINERS(PROFILE_TYPE_PATCH__NOT_APPLIED_FLAG, pPatch_na_contant);
	if(i==0)
		{::VirtualFree(pPatch_na_contant,  0, MEM_RELEASE); return NULL;}

	register pPATCH_INFO_STACK	pSelected_PATCH_Stack = NULL;
	pDH_ENUM_FILES_of_PROFILES fFlowPatch_na_patch = pPatch_na_contant;
	size_t reta = NULL;
	SERVICE_COUNT NumOfSuccessApplied = NULL;
	fFlowPatch_na_patch--;


	do
	{
		fFlowPatch_na_patch++;
		pSelected_PATCH_Stack = (pPATCH_INFO_STACK)DH_Read_Current_hunt_profile_CONTAINER(PROFILE_TYPE_PATCH__NOT_APPLIED_FLAG, &fFlowPatch_na_patch->FileName[0],PROFILE_READ_ALL,(PULONG)&reta);
		
		if(pSelected_PATCH_Stack == NULL) //filter #1
			continue;
		
		pVM_INFO_STACK pSelected_Stack = (pVM_INFO_STACK)DH_Read_Current_hunt_profile_CONTAINER(PROFILE_TYPE_VM, &pSelected_PATCH_Stack->owner_name[0],PROFILE_READ_ALL,(PULONG)&reta);

			if(pSelected_Stack == NULL) //filter #2
				{pSelected_PATCH_Stack->last_error_status = PATCH_ERROR_VM_STACK_ABSENT; continue;}

		//CONTAINTER OWNER TYPE VARIATOR
		switch(pSelected_PATCH_Stack->owner_as_profile__of_HELD_GROUP) //PATCH_TYPE_OWNERS_ENUMS
		{
		
		case PATCH_OWNER_VM_INSIDE_EXECUTE: //VM_INSIDE_EXECUTe
			{
			
				DH_Profile_Containter_name_convert_extension_to_new(&pSelected_PATCH_Stack->owner_name[0], PROFILE_TYPE_VM_ADDRES_POOL, PROFILE_TYPE_VM);
				pVM_AsmAddr_POOL_STACK pSelAddrPool = (pVM_AsmAddr_POOL_STACK)DH_Read_Current_hunt_profile_CONTAINER(PROFILE_TYPE_VM_ADDRES_POOL, &pSelected_PATCH_Stack->owner_name[0],PROFILE_READ_ALL,(PULONG)&reta);
					DH_Profile_Containter_name_convert_extension_to_new(&pSelected_PATCH_Stack->owner_name[0], PROFILE_TYPE_VM, PROFILE_TYPE_VM_ADDRES_POOL);

				if(pSelAddrPool == NULL)
					{pSelected_PATCH_Stack->last_error_status = PATCH_ERROR_ADDR_POOL_ABSENT; break;}

			SERVICE_TYPE last_type;
				if(pSelected_PATCH_Stack->is_new_added == true)
				{
					XEDPARSE* pXED_parse_asm = (XEDPARSE*)malloc(sizeof(XEDPARSE)); //new XEDPARSE
					memset(pXED_parse_asm, 0, sizeof(XEDPARSE));
					pXED_parse_asm->x64 = true;
					last_type = DH_MAP_Do_APPLY_NA_STACK__Get_BEST_Patch_on_TYPEDEFs_asm(pSelected_PATCH_Stack, pSelAddrPool, pSelected_Stack, pXED_parse_asm);
					DH_MAP_Do_APPLY_NA_STACK__WritePM_Arbitary_Filter(pSelected_PATCH_Stack);
					free(pXED_parse_asm);
				}

				if(last_type != NULL)
					NumOfSuccessApplied++; //SUCCESS ++

				free(pSelAddrPool);
			break;
			}//end PATCH_OWNER_VM_INSIDE_EXECUTE case

		case PATCH_OWNER_VM_COVERAGE: //adddddr pooooooooooooool
			{
				DH_Profile_Containter_name_convert_extension_to_new(&pSelected_PATCH_Stack->owner_name[0], PROFILE_TYPE_VM_ADDRES_POOL, PROFILE_TYPE_VM);
				pVM_AsmAddr_POOL_STACK pAddrPool = (pVM_AsmAddr_POOL_STACK)DH_Read_Current_hunt_profile_CONTAINER(PROFILE_TYPE_VM_ADDRES_POOL, &pSelected_PATCH_Stack->owner_name[0],PROFILE_READ_ALL,(PULONG)&reta);
				DH_Profile_Containter_name_convert_extension_to_new(&pSelected_PATCH_Stack->owner_name[0], PROFILE_TYPE_VM, PROFILE_TYPE_VM_ADDRES_POOL);

				if(pAddrPool == NULL)
					break;

					register size_t i = VM_HANDLES_MAX_COUNT;
					register size_t ii;
					register size_t ff;
					
					do //main coverage cycle
					{
						ii = pAddrPool->VM_HANHELD_addr_pool[i].HANHELD_asm_instruction_count;
						if(ii == NULL)
							continue;

						ff = NULL;
						BYTE* pAsm_instruction_JUMP_MARKER = &pAddrPool->VM_HANHELD_addr_pool[i].asm_instruction_unconditional_FLOW_CONTROL_MARKER[0];
						SERVICE_ADDRES* pAsm_instruction_addres = &pAddrPool->VM_HANHELD_addr_pool[i].asm_instruction_addres[0];
						
						do //slave coverage cycle
						{
							SERVICE_ADDRES PatchMap_RVA = codedef_calc_remap_addr(*pAsm_instruction_addres, pPatch_stack->VMP_Section_start_address, pPatch_stack->Region_VMP_patch_map_start);
								switch(*pAsm_instruction_JUMP_MARKER)
								{
								case FLOW_CONTROL_MARKER_JUMP_CONST: 
									{DH_MAP_Do_APPLY_NA_STACK__Mark_Patch_MAP(PatchMap_RVA, 5); break;} //E9 B2 C9 17 00 | jmp unravel_dump_scy.14192063
								case FLOW_CONTROL_MARKER_JUMP_REG: 
									{DH_MAP_Do_APPLY_NA_STACK__Mark_Patch_MAP(PatchMap_RVA, 2); break;} //FF E1 | jmp rcx
								case FLOW_CONTROL_MARKER_RET: 
									{DH_MAP_Do_APPLY_NA_STACK__Mark_Patch_MAP(PatchMap_RVA, 1); break;} //C3 | ret
								default:
									{
										size_t PredictSize = *(pAsm_instruction_addres + 1) - *pAsm_instruction_addres;
										if(PredictSize <= 16) //filter - limit instruction size
											DH_MAP_Do_APPLY_NA_STACK__Mark_Patch_MAP(PatchMap_RVA, PredictSize); //instruction size
									break;
									}//default case
								}//end swtich FLOW CONTROL			
							
						pAsm_instruction_JUMP_MARKER++;
						pAsm_instruction_addres++;
						}while(--ii);
					}while(i--);
				
					NumOfSuccessApplied++; //SUCCESS ++
				free(pAddrPool); //FREE
			break;
			} //end PATCH_OWNER_VM_COVERAGE case 
		case PATCH_OWNER_VM_SUPPLY15_DATA: //suply15 coverage
			{
				//SERVICE_ADDRES PatchMap_RVA = pPatch_stack->Region_VMP_patch_map_start + ((SERVICE_ADDRES)pSelected_Stack->r12_supply_15_start_address - pPatch_stack->VMP_Section_start_address);
				SERVICE_ADDRES PatchMap_RVA = codedef_calc_remap_addr(pSelected_Stack->r12_supply_15_start_address, pPatch_stack->VMP_Section_start_address, pPatch_stack->Region_VMP_patch_map_start);
				DH_MAP_Do_APPLY_NA_STACK__Mark_Patch_MAP(PatchMap_RVA, LIMIT_HANDLES_ON_SUPPLY*sizeof(CELL_ADDRES));
				NumOfSuccessApplied++; //SUCCESS ++
			break;
			}//end PATCH_OWNER_VM_SUPPLY15_DATA case
		}
		//CONTAINTER OWNER TYPE VARIATOR end
		
	{DWORD reta; DH_Write_Current_hunt_profile_CONTAINER(PROFILE_TYPE_PATCH__NOT_APPLIED_FLAG, &fFlowPatch_na_patch->FileName[0], (LPBYTE)pSelected_PATCH_Stack, sizeof(PATCH_INFO_STACK), &reta);} //re-save data
	free(pSelected_PATCH_Stack);
	free(pSelected_Stack);
	}while(i--);


	::VirtualFree(pPatch_na_contant, 0, MEM_RELEASE);

return NumOfSuccessApplied;
}

__forceinline void __fastcall DH_MAP_Do_APPLY_NA_STACK__Mark_Patch_MAP(SERVICE_ADDRES pDesd_addr, size_t size_write_data)
{
		//memset(&MAP_mark_buffer, PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_patch_map_lockmarker_default, sizeof(MAP_mark_buffer));
		
		_ASSERT(size_write_data > 1000);
	
		DH_WriteProcessAddres_DATA_nocheck((LPVOID)pDesd_addr,size_write_data, &tLOCK_MARKER[0]);
}


__forceinline void __fastcall DH_MAP_Do_APPLY_NA_STACK__ROLL_BACK_Mark_Patch_MAP(SERVICE_ADDRES pDesd_addr, size_t size_write_data)
{
		_ASSERT(size_write_data > 1000);
	
		DH_WriteProcessAddres_DATA_nocheck((LPVOID)pDesd_addr,size_write_data, &MAP_mark_ROLL_BACK_buffer[0]);
}

__forceinline SERVICE_TYPE __fastcall DH_MAP_Do_APPLY_NA_STACK__Get_BEST_Patch_on_TYPEDEFs_asm(__in __out pPATCH_INFO_STACK pPatchArrive, pVM_AsmAddr_POOL_STACK pVMAddrPoolArrive, __in pVM_INFO_STACK pVM_Info_Arrive, __in XEDPARSE* pXEDParse)
{
	register size_t i = pPatchArrive->HELD_GROUP_Fill_count;
	register	pPATCH_SUPPORTING_REGION_INI_STACK pSupportRegion = (pPATCH_SUPPORTING_REGION_INI_STACK)HUNT_HIVE.PATCH_Support_region_stack.allocated_pool;
		//if(pSupportRegion == NULL)
			//return false;
	
	SERVICE_TYPE last_pathing_method = NULL;
	do //parse cycle --------- crit addr
	{
		register SERVICE_ADDRES TargetTypedefAddr = pPatchArrive->Containt_HELD_GROUP[i].target_addres;
		register pPATCH_HELD_GROUP pIteraion_PatchArrive = &pPatchArrive->Containt_HELD_GROUP[i];
	
		if(TargetTypedefAddr == NULL) //filter #1
			continue;
	//	if(TargetTypedefAddr == 0x000000014170f829)
		//	Beep(0,0);
		pIteraion_PatchArrive->target_addres__assign_path_method = NULL;

			if (DH_VerifyProcessAddres(HUNT_HIVE.tOpenProcHANDLE, (LPVOID)TargetTypedefAddr) == NULL) //filter #2
				{pIteraion_PatchArrive->target_addres__assign_path_method = PATCH_TYPEDEF_ADDR__ERR_NOT_VERIFY; pPatchArrive->last_error_status = PATCH_ERROR_TARG_ADDR_NOT_VERIFY; continue;}

		register SERVICE_ADDRES* fpAddrPool = &pVMAddrPoolArrive->VM_HANHELD_addr_pool[pPatchArrive->i_Containt_handler_ArrayID].asm_instruction_addres[pPatchArrive->Containt_HELD_GROUP[i].Containt_HELD_GROUP_relative_to_AddrPool_ID]; //указатель на target_address в пуле адресов
		register SERVICE_ADDRES* fpAddrRETURNPool = &pVMAddrPoolArrive->VM_HANHELD_addr_pool[pPatchArrive->i_Containt_handler_ArrayID].asm_instruction_unconditional_FLOW_CONTROL_MARKER_dest_addres[pPatchArrive->Containt_HELD_GROUP[i].Containt_HELD_GROUP_relative_to_AddrPool_ID]; //указатель на адреса возвратов в пуле адресов
		fpAddrPool--; //calc true add pool location
		fpAddrRETURNPool--;  //calc true add ret pool location

		if(TargetTypedefAddr != *fpAddrPool) //filter #2 - verify addr pool location
			{pPatchArrive->last_error_status = PATCH_ERROR_TARGET_ADR_in_ADDR_POOL_BAD; continue;}
		register BYTE* fpAddrFlowControlMarker = &pVMAddrPoolArrive->VM_HANHELD_addr_pool[pPatchArrive->i_Containt_handler_ArrayID].asm_instruction_unconditional_FLOW_CONTROL_MARKER[pPatchArrive->Containt_HELD_GROUP[i].Containt_HELD_GROUP_relative_to_AddrPool_ID]; //указатель на target_address в пуле адресов
		fpAddrFlowControlMarker--; //calc true add pool location
		//	SERVICE_ARRAY_ID id_Addr_Pool = DH_MAP_Do_APPLY_NA_STACK__Find_TargetAdd_in_AddressPool(TargetTypedefAddr,&pVMAddrPoolArrive->VM_HANHELD_addr_pool[pPatchArrive->i_Containt_handler_ArrayID]);
			//if (id_Addr_Pool == -1)
				//continue;
										//save original asm %%%%%%%%%%%%%%%%%%%%%%%%%
			size_t original_asm_different_count = *(fpAddrPool+1) - *fpAddrPool; //fill original asm data
				if (original_asm_different_count >= 1 && original_asm_different_count < PATCH_SAFE_BOUND)
							{pIteraion_PatchArrive->patch_COUNT_original_target_instruction_bytes = original_asm_different_count; ::DH_ReadProcessAddres_DATA(&HUNT_HIVE.tOpenProcHANDLE, (LPVOID)*fpAddrPool, original_asm_different_count, &pIteraion_PatchArrive->patch_BYTES_original_target_instruction_bytes[0]);}
				else
					return NULL;

//			if(TargetTypedefAddr==0x00000001416adeea)
	//			Beep(0,0);

			/////// METHOD - "NOP" SLED & JMP const NEXT /////// ---- HI PRIORITY
				if(*(fpAddrFlowControlMarker+1) == FLOW_CONTROL_MARKER_JUMP_CONST)
					{
						pIteraion_PatchArrive->patch__HOOK_set_addres = *(fpAddrPool+1); //set HOOK desd
					codedef_set_pathcing_intreception_type(PATCH_NOP_SLED_AND_NEXT_JUMP_CONST); 
						if(*(fpAddrFlowControlMarker+2) == FLOW_CONTROL_MARKER_JUMP_CONST)
								pIteraion_PatchArrive->patch__return_addres_for_FILTER = *(fpAddrRETURNPool+2); //OPTIMIZE jmp patch
						else
								pIteraion_PatchArrive->patch__return_addres_for_FILTER = *(fpAddrRETURNPool+1);
					}//end true branch
			 //<<<<<<<< METHOD - "NOP" SLED & JMP const NEXT >>>>>>//
				else
				{
						/////// METHOD - "NOP" SLED & JMP const PREV ///////
						if(*(fpAddrFlowControlMarker-1) == FLOW_CONTROL_MARKER_JUMP_CONST)
							{
							pIteraion_PatchArrive->patch__HOOK_set_addres = *(fpAddrPool-1); //set HOOK desd
							codedef_set_pathcing_intreception_type(PATCH_NOP_SLED_AND_PREV_JUMP_CONST);
								if(*(fpAddrFlowControlMarker+2) == FLOW_CONTROL_MARKER_JUMP_CONST)
										pIteraion_PatchArrive->patch__return_addres_for_FILTER = *(fpAddrRETURNPool+2); //OPTIMIZE jmp patch
								else
										pIteraion_PatchArrive->patch__return_addres_for_FILTER = *(fpAddrPool+2);

							} //end true branch
					
									else

										{/////// METHOD - INSTRUCTION DISPLACEMENT (DOWN) ///////
											signed long calc_jmp_size = PATCH_JMP_INSTRECEPT_CONST_SIZE;
											BYTE* fpAddrFlowControlMarker__local_copy = fpAddrFlowControlMarker;
											SERVICE_ADDRES* fpAddrPool__local_copy = fpAddrPool;
											SERVICE_ADDRES* fpAddrPool__local_RETURN_copy = fpAddrRETURNPool;
											SERVICE_ADDRES* fpAddrPool__First_displacement_asm = fpAddrPool+1;
											size_t asm_different_count = NULL;

										
											//когда одна critical-инструкция меньше PATCH_JMP_INSTRECEPT_CONST_SIZE
												if(original_asm_different_count < PATCH_JMP_INSTRECEPT_CONST_SIZE)
												{
													
													do
													{
														fpAddrFlowControlMarker__local_copy++;
														fpAddrPool__local_copy++;
														fpAddrPool__local_RETURN_copy++;
														asm_different_count = *fpAddrPool__local_copy - *fpAddrPool;
														if (asm_different_count >= PATCH_JMP_INSTRECEPT_CONST_SIZE && asm_different_count < PATCH_SAFE_BOUND)
															{
																pIteraion_PatchArrive->patch__HOOK_set_addres = *(fpAddrPool); //hook addr
																codedef_set_pathcing_intreception_type(PATCH_DISPLACE_OTHERS_ASM); //set type
																pIteraion_PatchArrive->patch_COUNT_original_displace_JMP_intreceptor_instruction_bytes = asm_different_count - original_asm_different_count;
																if(*(fpAddrFlowControlMarker__local_copy) != FLOW_CONTROL_MARKER_JUMP_CONST) //one jump const
																	pIteraion_PatchArrive->patch__return_addres_for_FILTER = *fpAddrPool__local_copy;
																else
																	if(*(fpAddrFlowControlMarker__local_copy+1) == FLOW_CONTROL_MARKER_JUMP_CONST) //dobule jmp const - const
																		pIteraion_PatchArrive->patch__return_addres_for_FILTER = *(fpAddrPool__local_RETURN_copy+1);
																	else
																		pIteraion_PatchArrive->patch__return_addres_for_FILTER = *(fpAddrPool__local_copy+1);

																::DH_ReadProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE, (LPVOID)*(fpAddrPool__First_displacement_asm), pIteraion_PatchArrive->patch_COUNT_original_displace_JMP_intreceptor_instruction_bytes, &pIteraion_PatchArrive->patch_BYTES_original_displace_JMP_intreceptor_instruction_bytes[0]);
																
																pIteraion_PatchArrive->patch__original_bytes_addres = *fpAddrPool__First_displacement_asm;
																break;
															}//end if 
													}while((*fpAddrFlowControlMarker__local_copy != FLOW_CONTROL_MARKER_JUMP_CONST) && (calc_jmp_size-- != NULL));

										//<<<<<<<<<<<< METHOD - INSTRUCTION DISPLACEMENT WHEN TOO CLOSE JMP(DOWN) -  no enouth space for HOOKset >>>>>>>>>>>>>>>>>>>>>>> ///
														if((*fpAddrFlowControlMarker__local_copy == FLOW_CONTROL_MARKER_JUMP_CONST) && (pIteraion_PatchArrive->target_addres__assign_path_method == NULL) && (asm_different_count < PATCH_JMP_INSTRECEPT_CONST_SIZE))
														{
															pIteraion_PatchArrive->patch__HOOK_set_addres =				*(fpAddrPool__local_copy);
															pIteraion_PatchArrive->patch__return_addres_for_FILTER =	*fpAddrPool__local_RETURN_copy;
															pIteraion_PatchArrive->patch_COUNT_original_displace_JMP_intreceptor_instruction_bytes = asm_different_count;
															codedef_set_pathcing_intreception_type(PATCH_COMBINE__NOP_SLED_and_DISPLACE_OTHERS_ASM_TOO_CLOSE_NEXT_JMP); //set type

															::DH_ReadProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE, (LPVOID)*(fpAddrPool__First_displacement_asm), pIteraion_PatchArrive->patch_COUNT_original_displace_JMP_intreceptor_instruction_bytes, &pIteraion_PatchArrive->patch_BYTES_original_displace_JMP_intreceptor_instruction_bytes[0]);
														}//<<<<<<<<<<<< end METHOD - INSTRUCTION DISPLACEMENT WHEN TOO CLOSE JMP(DOWN) >>>>>>>>>>>>>>>>>>>>>>>> ///
										

													if(pIteraion_PatchArrive->target_addres__assign_path_method == NULL)
														pPatchArrive->last_error_status = PATCH_ERROR_PATCH_METHOD_NOT_ASSIGNED;
											//	free(pDissasm_struct);
												} //end if
										}//<<<<<<<< end METHOD - INSTRUCTION DISPLACEMENT >>>>>>//
		} //<<<<<<<< METHOD - "NOP" SLED & JMP const PREV >>>>>>//

				if(pIteraion_PatchArrive->patch__return_addres_for_FILTER == NULL)
					Beep(0,0);

			if(last_pathing_method == NULL)
				continue;
			//release patch strategy
			pPATCH_SUPPORTING_VMPX_PROFILE_INI pArbiratyFilter = DH_MAP_Do_APPLY_NA_STACK__Get_Arbitary_Filter(pPatchArrive, pVM_Info_Arrive);
			if(pArbiratyFilter == NULL) //FILTER #2
				{pPatchArrive->last_error_status = PATCH_ERROR_ARBITARY_FILTER_NOT_ASSINGED; continue;}

			////////// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ CONSTRUCT RETAIL PATCH BYTES ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
			if (DH_MAP_Do_APPLY_NA_STACK__Construct_Retail_PATCH_byte_tables(pIteraion_PatchArrive, pVM_Info_Arrive, pArbiratyFilter, pSupportRegion, pXEDParse) == NULL)
				{pPatchArrive->last_error_status = PATCH_ERROR_CANNOT_CONSTRUCT_RETAIL_ARBITARY_FILTER; return NULL;}
			
			//pIteraion_PatchArrive->

	}while(i--); // end parse cycle --------- crit addr
return last_pathing_method;
}

__forceinline SERVICE_ARRAY_ID __fastcall DH_MAP_Do_APPLY_NA_STACK__Find_TargetAdd_in_AddressPool(SERVICE_ADDRES TargetTypedefAddr, pVM_hanheld_addres_pool pAddrPool_ReqHANHELD)
{
	size_t ii = pAddrPool_ReqHANHELD->HANHELD_asm_instruction_count;

	if(ii == NULL)
		return -1;

	register SERVICE_ADDRES* pAsm_instruction_addres = &pAddrPool_ReqHANHELD->asm_instruction_addres[MAX_ASM_INSTUCTION_POOL];
		//pAsm_instruction_addres--;
				do //subcyle
				{
					pAsm_instruction_addres--;
					if (*pAsm_instruction_addres != TargetTypedefAddr)
						continue;

					return (ii);
				}while(ii--); //end subcycle
return -1;
}

pPATCH_SUPPORTING_VMPX_PROFILE_INI_STACK __fastcall DH_MAP_Do_INIT_Arbitary_FILTERs(__in 	WCHAR *VMPX_profile_name, __in	SERVICE_TYPE VMP_Version)
{
	DWORD reta = 0;
	register pPATCH_SUPPORTING_VMPX_PROFILE_INI_STACK pRaw_VMPX_Profiler = (pPATCH_SUPPORTING_VMPX_PROFILE_INI_STACK)DH_Read_Current_hunt_profile_CONTAINER(PROFILE_TYPE_PATCH_FILTER_ASM_STACK_INI, VMPX_profile_name,PROFILE_READ_ALL,(PULONG)&reta); //read x_profiler
	if(pRaw_VMPX_Profiler == NULL)
		return NULL;

	
	if (DH_MAP_Predict_VMPX_PROFILE_filter(pRaw_VMPX_Profiler) == NULL)
			return NULL;

	//ajust filter as active VMP version
	switch(VMP_Version)
	{
	case VM_TYPE_VMP2: 
		{
			if(HUNT_HIVE.PATCH_ArbitaryFilter_VMP2.allocated_pool == NULL)
			{
			HUNT_HIVE.PATCH_ArbitaryFilter_VMP2.allocated_pool = (LPBYTE)pRaw_VMPX_Profiler;
			HUNT_HIVE.PATCH_ArbitaryFilter_VMP2.region_size = sizeof(PATCH_SUPPORTING_VMPX_PROFILE_INI_STACK);
			HUNT_HIVE.PATCH_ArbitaryFilter_VMP2.type_of_region = REGION_TYPE_PATCH_ARBITARY_FILTER_STRUCT;
			}
			else
			{
				::VirtualFreeEx(DH_Current_process_fast_handle, HUNT_HIVE.PATCH_ArbitaryFilter_VMP2.allocated_pool, 0,MEM_RELEASE); //free(HUNT_HIVE.PATCH_ArbitaryFilter_VMP2.allocated_pool);
				HUNT_HIVE.PATCH_ArbitaryFilter_VMP2.allocated_pool = (LPBYTE)pRaw_VMPX_Profiler;
			}
		break;
		} //END CASE 2
	case VM_TYPE_VMP3:
		{
			if(HUNT_HIVE.PATCH_ArbitaryFilter_VMP3new.allocated_pool == NULL)
			{
			HUNT_HIVE.PATCH_ArbitaryFilter_VMP3new.allocated_pool = (LPBYTE)pRaw_VMPX_Profiler;
			HUNT_HIVE.PATCH_ArbitaryFilter_VMP3new.region_size = sizeof(PATCH_SUPPORTING_VMPX_PROFILE_INI_STACK);
			HUNT_HIVE.PATCH_ArbitaryFilter_VMP3new.type_of_region = REGION_TYPE_PATCH_ARBITARY_FILTER_STRUCT; 
			}
			else
			{
				::VirtualFreeEx(DH_Current_process_fast_handle, HUNT_HIVE.PATCH_ArbitaryFilter_VMP3new.allocated_pool, 0,MEM_RELEASE);
				HUNT_HIVE.PATCH_ArbitaryFilter_VMP3new.allocated_pool = (LPBYTE)pRaw_VMPX_Profiler;
			}
		break;
		} //END CASE 3
	default: {free(pRaw_VMPX_Profiler); return NULL;} //not support variants
	}//end version swtich

return pRaw_VMPX_Profiler;
}

__forceinline pPATCH_SUPPORTING_VMPX_PROFILE_INI_STACK __fastcall DH_MAP_Predict_VMPX_PROFILE_filter(__in pPATCH_SUPPORTING_VMPX_PROFILE_INI_STACK pFilterRAWStack)
{
	register size_t i_loaded = HANHELD_TYPE_ENUMS_COUNT-1; //register size_t i_loaded = pFilterRAWStack->Loaded_HANHELDS_FILTERS;
	register size_t f = NULL;
	register pPATCH_SUPPORTING_VMPX_PROFILE_INI pVMPX_Profile = &pFilterRAWStack->VMPX_HANHELD_TYPE_ENUMS[0]; //VMPX_HANHELD_TYPE_ENUMS
	register TCHAR* pAsmInstruction = &pFilterRAWStack->VMPX_HANHELD_TYPE_ENUMS[0].Assembly_instruction1[0]; //asm stack
	register size_t h_pointer;
	register TCHAR fpControlSymbol;
	register SERVICE_TYPE* fpPredictControlSymbolType;
	register size_t* fpPredictAsmSize;

	bool* fpVPX_is_control_present;
	size_t* fpFirst_control_symbol_offset;

	TCHAR tmp_asm_regroup[MAX_PATH];
	pVMPX_Profile--;

	SERVICE_ADDRES fpPredictCONST; //константа предсказания
	XEDPARSE* pXED_parse_asm = (XEDPARSE*)malloc(sizeof(XEDPARSE));
					memset(pXED_parse_asm, 0, sizeof(XEDPARSE));
					pXED_parse_asm->x64 = true;

	register pPATCH_SUPPORTING_REGION_INI_STACK pPatch_stack = (pPATCH_SUPPORTING_REGION_INI_STACK)HUNT_HIVE.PATCH_Support_region_stack.allocated_pool;
	do
	{
	pVMPX_Profile++;
	h_pointer=NULL;
		f = pVMPX_Profile->Assembly_count;
		if(f == NULL)
			continue;

		pAsmInstruction = &pVMPX_Profile->Assembly_instruction1[0];
		pAsmInstruction-=sizeof(pVMPX_Profile->Assembly_instruction1)/sizeof(TCHAR);
		fpVPX_is_control_present = &pVMPX_Profile->Control_symbols_is_present[1];
		fpVPX_is_control_present--;
		fpFirst_control_symbol_offset = &pVMPX_Profile->First_control_symbol_offset[1];
		fpFirst_control_symbol_offset--;
		fpPredictControlSymbolType = &pVMPX_Profile->Predict_POST_BUILD_Control_symbol_type[1];
		fpPredictControlSymbolType--;
		fpPredictAsmSize = &pVMPX_Profile->Predict_POST_BUILD_asm_instruction_increment_size[1];
		fpPredictAsmSize--;
		fpPredictCONST = PREDICT_ADDR_CONST; //константа предсказания

		//	fpPredictCONST = PREDICT_ADDR_CONST; - set null
		register size_t increment_asm_size = NULL; 
		do///////////////////////// +++++++++++ //////////////
		{
			pAsmInstruction+=sizeof(pVMPX_Profile->Assembly_instruction1)/sizeof(TCHAR);
			fpVPX_is_control_present++;
			fpFirst_control_symbol_offset++;
			fpPredictControlSymbolType++;
			fpPredictAsmSize++;

			if(*fpVPX_is_control_present == true)
			{
			//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
			//
					register size_t istring = wcslen(pAsmInstruction);
					if(istring == 2) //skip $R
						{*fpPredictControlSymbolType = FILTER_SUBDEFINE_CONTROL_SYMBOL_RETURN; if(--f == NULL)break;  continue;}
					//memcpy(&tmp_asm_regroup[0], pAsmInstruction, *(fpFirst_control_symbol_offset)); //fast copy until 1-st control symbol - <<1 as *sizeof(TCHAR)
					register TCHAR* fpDesdString    =  (TCHAR*)((SERVICE_ADDRES)pAsmInstruction + *fpFirst_control_symbol_offset);
					//register TCHAR* fpTmpFormString = (TCHAR*)((SERVICE_ADDRES)&tmp_asm_regroup[0]);
					tmp_asm_regroup[0] = L'\0';
					if(*(fpDesdString+1) != END_STR)
					wcscpy(&tmp_asm_regroup[0], fpDesdString+1);

					// __________________ convert control symbols to retail string form __________________ 
						switch(*fpDesdString)
						{
						case FILTER_SUBDEFINE_CONTROL_SYMBOL_VM_HI_BORDER:
							{itoa_16_fast(pPatch_stack->VMP_Section_size + pPatch_stack->VMP_Section_start_address, fpDesdString-1); wcscat(fpDesdString, &tmp_asm_regroup[0]); quick_UNICODE_to_ASCII(pAsmInstruction, pXED_parse_asm->instr); break;}//end FILTER_SUBDEFINE_CONTROL_SYMBOL_VM_HI_BORDER 
							// NEW - {quick_UNICODE_to_ASCII_with_ltoa16(pAsmInstruction,pXED_parse_asm->instr, fpDesdString-1, pPatch_stack->VMP_Section_size + pPatch_stack->VMP_Section_start_address); break;}
						case FILTER_SUBDEFINE_CONTROL_SYMBOL_VM_LO_BORDER:
							{itoa_16_fast(pPatch_stack->VMP_Section_start_address, fpDesdString-1); wcscat(fpDesdString, &tmp_asm_regroup[0]); quick_UNICODE_to_ASCII(pAsmInstruction, pXED_parse_asm->instr); break;}
							//{quick_UNICODE_to_ASCII_with_ltoa16(pAsmInstruction,pXED_parse_asm->instr, fpDesdString-1, pPatch_stack->VMP_Section_start_address); break;}
						case FILTER_SUBDEFINE_CONTROL_SYMBOL_ORIGINAL_COPY:
							{ itoa_16_fast(pPatch_stack->Region_VMP_original_copy_start - pPatch_stack->VMP_Section_start_address, fpDesdString-1); wcscat(fpDesdString, &tmp_asm_regroup[0]); quick_UNICODE_to_ASCII(pAsmInstruction, pXED_parse_asm->instr); break;}
							//{quick_UNICODE_to_ASCII_with_ltoa16(pAsmInstruction,pXED_parse_asm->instr, fpDesdString-1, pPatch_stack->Region_VMP_original_copy_start - pPatch_stack->VMP_Section_start_address); break;}
						case FILTER_SUBDEFINE_CONTROL_SYMBOL_PATCH_MAP:
							{ itoa_16_fast(pPatch_stack->Region_VMP_patch_map_start - pPatch_stack->VMP_Section_start_address, fpDesdString-1); wcscat(fpDesdString, &tmp_asm_regroup[0]); quick_UNICODE_to_ASCII(pAsmInstruction, pXED_parse_asm->instr); break;}
							//{quick_UNICODE_to_ASCII_with_ltoa16(pAsmInstruction,pXED_parse_asm->instr, fpDesdString-1, pPatch_stack->Region_VMP_patch_map_start - pPatch_stack->VMP_Section_start_address); break;}
						case FILTER_SUBDEFINE_CONTROL_SYMBOL_VM_SUPPLY15:
							{itoa_16_fast(PREDICT_ADDR_SUPPLY15, fpDesdString-1); wcscat(fpDesdString, &tmp_asm_regroup[0]); quick_UNICODE_to_ASCII(pAsmInstruction, pXED_parse_asm->instr);break;}
						case FILTER_SUBDEFINE_CONTROL_SYMBOL_MARKED_1:
							{*fpPredictControlSymbolType=1; quick_UNICODE_to_ASCII_with_ltoa16(pAsmInstruction,pXED_parse_asm->instr, fpDesdString-1, PREDICT_ADDR_DISTANCE); break;}
						case FILTER_SUBDEFINE_CONTROL_SYMBOL_MARKED_2:
							{*fpPredictControlSymbolType=2; quick_UNICODE_to_ASCII_with_ltoa16(pAsmInstruction,pXED_parse_asm->instr, fpDesdString-1, PREDICT_ADDR_DISTANCE); break;}
						case FILTER_SUBDEFINE_CONTROL_SYMBOL_MARKED_3:
							{*fpPredictControlSymbolType=3; quick_UNICODE_to_ASCII_with_ltoa16(pAsmInstruction,pXED_parse_asm->instr, fpDesdString-1, PREDICT_ADDR_DISTANCE); break;}
						case FILTER_SUBDEFINE_CONTROL_SYMBOL_MARKED_4:
							{*fpPredictControlSymbolType=4; quick_UNICODE_to_ASCII_with_ltoa16(pAsmInstruction,pXED_parse_asm->instr, fpDesdString-1, PREDICT_ADDR_DISTANCE); break;}
						case FILTER_SUBDEFINE_CONTROL_SYMBOL_MARKED_5:
							{*fpPredictControlSymbolType=5; quick_UNICODE_to_ASCII_with_ltoa16(pAsmInstruction,pXED_parse_asm->instr, fpDesdString-1, PREDICT_ADDR_DISTANCE); break;}
						case FILTER_SUBDEFINE_CONTROL_SYMBOL_MARKED_6:
							{*fpPredictControlSymbolType=6; quick_UNICODE_to_ASCII_with_ltoa16(pAsmInstruction,pXED_parse_asm->instr, fpDesdString-1, PREDICT_ADDR_DISTANCE); break;}
						case FILTER_SUBDEFINE_CONTROL_SYMBOL_MARKED_7:
							{*fpPredictControlSymbolType=7; quick_UNICODE_to_ASCII_with_ltoa16(pAsmInstruction,pXED_parse_asm->instr, fpDesdString-1, PREDICT_ADDR_DISTANCE); break;}
						case FILTER_SUBDEFINE_CONTROL_SYMBOL_MARKED_8:
							{*fpPredictControlSymbolType=8; quick_UNICODE_to_ASCII_with_ltoa16(pAsmInstruction,pXED_parse_asm->instr, fpDesdString-1, PREDICT_ADDR_DISTANCE); break;}
						case FILTER_SUBDEFINE_CONTROL_SYMBOL_MARKED_9:
							{*fpPredictControlSymbolType=9; quick_UNICODE_to_ASCII_with_ltoa16(pAsmInstruction,pXED_parse_asm->instr, fpDesdString-1, PREDICT_ADDR_DISTANCE); break;}
						}//end CS switch

				}//end true for CS
			else
				quick_UNICODE_to_ASCII(pAsmInstruction, pXED_parse_asm->instr);


			// __________________ build "predict DRAFT stack" __________________ //
			{
			pXED_parse_asm->cip = fpPredictCONST; //set current position addres
		
					if (XEDParseAssemble(pXED_parse_asm) == XEDPARSE_ERROR) //XED parse +++++++++++++++++++++++++
						{free(pXED_parse_asm); return pFilterRAWStack;}

				*fpPredictAsmSize = (increment_asm_size += pXED_parse_asm->dest_size); //INC asm size + save inc predict size//

			fpPredictCONST+=pXED_parse_asm->dest_size; //up to next asm addr
			}// __________________ end DRAFT stack __________________ //
			
			//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
		}while(--f); //parse asm stack ------------------- /////////
		
		bool b_repeat_iteration = false;
		// __________________ build "predict PRECISION stack" __________________ //
		pAsmInstruction = &pVMPX_Profile->Assembly_instruction1[0];
		pAsmInstruction-=sizeof(pVMPX_Profile->Assembly_instruction1)/sizeof(TCHAR);
		fpVPX_is_control_present = &pVMPX_Profile->Control_symbols_is_present[1];
		fpVPX_is_control_present--;
		fpFirst_control_symbol_offset = &pVMPX_Profile->First_control_symbol_offset[1];
		fpFirst_control_symbol_offset--;
		fpPredictControlSymbolType = &pVMPX_Profile->Predict_POST_BUILD_Control_symbol_type[1];
		fpPredictControlSymbolType--;
		fpPredictAsmSize = &pVMPX_Profile->Predict_POST_BUILD_asm_instruction_increment_size[1];
		fpPredictAsmSize--;

		increment_asm_size = NULL;
		
		{ // -----------------------------------------
			f = pVMPX_Profile->Assembly_count; //upd f
			do
			{
			pAsmInstruction+=sizeof(pVMPX_Profile->Assembly_instruction1)/sizeof(TCHAR);
			fpVPX_is_control_present++;
			fpFirst_control_symbol_offset++;
			fpPredictControlSymbolType++;
			fpPredictAsmSize++;



					if(*fpVPX_is_control_present == false) //filter#1
						{
								if (b_repeat_iteration == false) //get 1st diff pos
										increment_asm_size = *fpPredictAsmSize;
								else
								{
									// >>>>>>>>> CALC new size for non-CS asm instructions
														{	//XED parse block #2A (support)
														pXED_parse_asm->cip = PREDICT_ADDR_CONST + increment_asm_size; //restore asm current position
														
														quick_UNICODE_to_ASCII(pAsmInstruction, &pXED_parse_asm->instr[0]);
															if (XEDParseAssemble(pXED_parse_asm) == XEDPARSE_ERROR) //XED parse +++++++++++++++++++++++++
																	{free(pXED_parse_asm); return NULL;}

															*fpPredictAsmSize = (increment_asm_size += pXED_parse_asm->dest_size); //INC asm size + save inc predict size//
														} //XED parse block #2A (support) end
								}//end else
						continue; //возврат к основному циклу = точно предсказана инструкция БЕЗ CS
						} //get prev asm predict size
					if(*fpPredictControlSymbolType == FILTER_SUBDEFINE_CONTROL_SYMBOL_RETURN) //filter#2
						continue; //возврат к основному циклу = пропускаем $RETURN

					//работаем с инструкцией, содержащей CS
					register TCHAR* fpDesdString    =  (TCHAR*)((SERVICE_ADDRES)pAsmInstruction + *fpFirst_control_symbol_offset); //restore CS shift

					register SERVICE_ARRAY_ID RestoreLabelDestPosition; //restore LABEL Desdonation position
						//labels 1-9
						switch(*fpPredictControlSymbolType)
						{
						case 1: {RestoreLabelDestPosition = pVMPX_Profile->Label[1]; break;}
						case 2: {RestoreLabelDestPosition = pVMPX_Profile->Label[2]; break;}
						case 3: {RestoreLabelDestPosition = pVMPX_Profile->Label[3]; break;}
						case 4: {RestoreLabelDestPosition = pVMPX_Profile->Label[4]; break;}
						case 5: {RestoreLabelDestPosition = pVMPX_Profile->Label[5]; break;}
						case 6: {RestoreLabelDestPosition = pVMPX_Profile->Label[6]; break;}
						case 7: {RestoreLabelDestPosition = pVMPX_Profile->Label[7]; break;}
						case 8: {RestoreLabelDestPosition = pVMPX_Profile->Label[8]; break;}
						case 9: {RestoreLabelDestPosition = pVMPX_Profile->Label[9]; break;}
						default: {
							if (b_repeat_iteration == false) //get 1st diff pos
										increment_asm_size = *fpPredictAsmSize;
								else
								{
									// >>>>>>>>> CALC new size for non-LABEL asm instructions
														{	//XED parse block #2B (support)
														pXED_parse_asm->cip = PREDICT_ADDR_CONST + increment_asm_size; //restore asm current position
														
															quick_UNICODE_to_ASCII(pAsmInstruction, &pXED_parse_asm->instr[0]);
															if (XEDParseAssemble(pXED_parse_asm) == XEDPARSE_ERROR) //XED parse +++++++++++++++++++++++++
																	{free(pXED_parse_asm); return NULL;}

															*fpPredictAsmSize = (increment_asm_size += pXED_parse_asm->dest_size); //INC asm size + save inc predict size//
														} //XED parse block #2B (support) end
								}//end else}
						
								 continue; 
								 } //end default switch
						}//end switch
					
							{	//XED parse block #2 (!!!)
									pXED_parse_asm->cip = PREDICT_ADDR_CONST + increment_asm_size; //restore asm current position
									SERVICE_ADDRES RestoreJumpOperand = PREDICT_ADDR_CONST + (pVMPX_Profile->Predict_POST_BUILD_asm_instruction_increment_size[RestoreLabelDestPosition] - *fpPredictAsmSize); //restore desd operand
									quick_UNICODE_to_ASCII_with_ltoa16(pAsmInstruction,pXED_parse_asm->instr, fpDesdString-1, RestoreJumpOperand);

										if (XEDParseAssemble(pXED_parse_asm) == XEDPARSE_ERROR) //XED parse +++++++++++++++++++++++++
											{free(pXED_parse_asm); return pFilterRAWStack;}

									increment_asm_size += pXED_parse_asm->dest_size; //INC asm size
									if (*fpPredictAsmSize == increment_asm_size)
											continue; //SUCCESS DRAFT predict!!!
								
									//	*************** SET CORRECTION for PREDICT STACK *************** 
											*fpPredictAsmSize = increment_asm_size;
											b_repeat_iteration = true; //recalc
							} //XED parse block #2 (!!!) end

			}while(--f); //precision prect cycle
		}// -----------------------------------------
		// __________________ end PRECISION stack __________________ //

		/*
		// R E P E A T !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if (b_repeat_iteration == true)
		{
	// __________________ REPEAT (-------------) PRECISION stack __________________ //
				pAsmInstruction = &pVMPX_Profile->Assembly_instruction1[0];
					pAsmInstruction-=sizeof(pVMPX_Profile->Assembly_instruction1)/sizeof(TCHAR);
					fpVPX_is_control_present = &pVMPX_Profile->Control_symbols_is_present[1];
					fpVPX_is_control_present--;
					fpFirst_control_symbol_offset = &pVMPX_Profile->First_control_symbol_offset[1];
					fpFirst_control_symbol_offset--;
					fpPredictControlSymbolType = &pVMPX_Profile->Predict_POST_BUILD_Control_symbol_type[1];
					fpPredictControlSymbolType--;
					fpPredictAsmSize = &pVMPX_Profile->Predict_POST_BUILD_asm_instruction_increment_size[1];
					fpPredictAsmSize--;

					increment_asm_size = NULL;
		
					{ // -----------------------------------------
						f = pVMPX_Profile->Assembly_count; //upd f
						do
						{
						pAsmInstruction+=sizeof(pVMPX_Profile->Assembly_instruction1)/sizeof(TCHAR);
						fpVPX_is_control_present++;
						fpFirst_control_symbol_offset++;
						fpPredictControlSymbolType++;
						fpPredictAsmSize++;



								if(*fpVPX_is_control_present == false) //filter#1
									{
											if (b_repeat_iteration == false) //get 1st diff pos
													increment_asm_size = *fpPredictAsmSize;
											else
											{
												// >>>>>>>>> CALC new size for non-CS asm instructions
																	{	//XED parse block #2A (support)
																	pXED_parse_asm->cip = PREDICT_ADDR_CONST + increment_asm_size; //restore asm current position
														
																		if (XEDParseAssemble(pXED_parse_asm) == XEDPARSE_ERROR) //XED parse +++++++++++++++++++++++++
																				{free(pXED_parse_asm); return NULL;}

																		*fpPredictAsmSize = (increment_asm_size += pXED_parse_asm->dest_size); //INC asm size + save inc predict size//
																	} //XED parse block #2A (support) end
											}
										continue;} //get prev asm predict size
								if(*fpPredictControlSymbolType == FILTER_SUBDEFINE_CONTROL_SYMBOL_RETURN) //filter#2
									continue;

								register TCHAR* fpDesdString    =  (TCHAR*)((SERVICE_ADDRES)pAsmInstruction + *fpFirst_control_symbol_offset); //restore CS shift

								register SERVICE_ARRAY_ID RestoreLabelDestPosition; //restore LABEL Desdonation position
									//labels 1-9
									switch(*fpPredictControlSymbolType)
									{
									case 1: {RestoreLabelDestPosition = pVMPX_Profile->Label[1]; break;}
									case 2: {RestoreLabelDestPosition = pVMPX_Profile->Label[2]; break;}
									case 3: {RestoreLabelDestPosition = pVMPX_Profile->Label[3]; break;}
									case 4: {RestoreLabelDestPosition = pVMPX_Profile->Label[4]; break;}
									case 5: {RestoreLabelDestPosition = pVMPX_Profile->Label[5]; break;}
									case 6: {RestoreLabelDestPosition = pVMPX_Profile->Label[6]; break;}
									case 7: {RestoreLabelDestPosition = pVMPX_Profile->Label[7]; break;}
									case 8: {RestoreLabelDestPosition = pVMPX_Profile->Label[8]; break;}
									case 9: {RestoreLabelDestPosition = pVMPX_Profile->Label[9]; break;}
									default: {if (b_repeat_iteration == false) //get 1st diff pos
										increment_asm_size = *fpPredictAsmSize;
														else
														{
															// >>>>>>>>> CALC new size for non-LABEL asm instructions
																				{	//XED parse block #2B (support)
																				pXED_parse_asm->cip = PREDICT_ADDR_CONST + increment_asm_size; //restore asm current position
														
																					if (XEDParseAssemble(pXED_parse_asm) == XEDPARSE_ERROR) //XED parse +++++++++++++++++++++++++
																							{free(pXED_parse_asm); return NULL;}

																					*fpPredictAsmSize = (increment_asm_size += pXED_parse_asm->dest_size); //INC asm size + save inc predict size//
																				} //XED parse block #2B (support) end
														}//end else}
						
								 continue;} //end default switch}
									}//cs present SWITCH
						
					
										{	//XED parse block #2 (!!!)
												pXED_parse_asm->cip = PREDICT_ADDR_CONST + increment_asm_size; //restore asm current position
												SERVICE_ADDRES RestoreJumpOperand = PREDICT_ADDR_CONST + (pVMPX_Profile->Predict_POST_BUILD_asm_instruction_increment_size[RestoreLabelDestPosition] - *fpPredictAsmSize); //restore desd operand
												quick_UNICODE_to_ASCII_with_ltoa16(pAsmInstruction,pXED_parse_asm->instr, fpDesdString-1, RestoreJumpOperand);

													if (XEDParseAssemble(pXED_parse_asm) == XEDPARSE_ERROR) //XED parse +++++++++++++++++++++++++
														{free(pXED_parse_asm); return pFilterRAWStack;}

												increment_asm_size += pXED_parse_asm->dest_size; //INC asm size
												if (*fpPredictAsmSize == increment_asm_size)
														continue; //SUCCESS DRAFT predict!!!
								
												//	*************** SET CORRECTION for PREDICT STACK *************** 
														*fpPredictAsmSize = increment_asm_size;
														b_repeat_iteration = true; //recalc
										} //XED parse block #2 (!!!) end

						}while(--f); //precision prect cycle
					}// -----------------------------------------
					// __________________ end REPEAT PRECISION stack __________________ //
			}//end if for REPEAT iteration
			*/

	}while(i_loaded--);//do{}while cycle - parse filter stack

	

free(pXED_parse_asm);
return pFilterRAWStack;
}


__forceinline pPATCH_SUPPORTING_VMPX_PROFILE_INI __fastcall DH_MAP_Do_APPLY_NA_STACK__Get_Arbitary_Filter(__in __out pPATCH_INFO_STACK pPatchArrive, __in pVM_INFO_STACK pVM_Info_Arrive)
{
	register pPATCH_SUPPORTING_VMPX_PROFILE_INI_STACK pSelectArbitaryStack;

	switch(pVM_Info_Arrive->vm_version)
	{
		case VM_TYPE_VMP2: 
			{
				if(HUNT_HIVE.PATCH_ArbitaryFilter_VMP2.allocated_pool == NULL)
					return NULL;

				pSelectArbitaryStack = (pPATCH_SUPPORTING_VMPX_PROFILE_INI_STACK)HUNT_HIVE.PATCH_ArbitaryFilter_VMP2.allocated_pool;
			break;
			} //end case VMP2
		case VM_TYPE_VMP3:
			{
				if(HUNT_HIVE.PATCH_ArbitaryFilter_VMP3new.allocated_pool == NULL)
					return NULL;

				pSelectArbitaryStack = (pPATCH_SUPPORTING_VMPX_PROFILE_INI_STACK)HUNT_HIVE.PATCH_ArbitaryFilter_VMP3new.allocated_pool;
			break;
			}//end case VMP3
		default:{return NULL;}
	}//end switch

	register SERVICE_TYPE findtype = pPatchArrive->type_of_target_addres;
	register pPATCH_SUPPORTING_VMPX_PROFILE_INI pVMPX = (pPATCH_SUPPORTING_VMPX_PROFILE_INI)&pSelectArbitaryStack->VMPX_HANHELD_TYPE_ENUMS[findtype];
	register SERVICE_TYPE* pVMPX_key_type = (SERVICE_TYPE*)&pSelectArbitaryStack->KeyType[findtype];
	if(*pVMPX_key_type == findtype) //QUICK ACCESS TABLE
		return pVMPX;
	/*/
	register size_t f = pSelectArbitaryStack->Loaded_HANHELDS_FILTERS;
	register pPATCH_SUPPORTING_VMPX_PROFILE_INI pVMPX = (pPATCH_SUPPORTING_VMPX_PROFILE_INI)&pSelectArbitaryStack->VMPX_HANHELD_TYPE_ENUMS[0];
	register SERVICE_TYPE findtype = pPatchArrive->type_of_target_addres;
	pVMPX--;
	pVMPX_key_type--;
	do
	{
		pVMPX++;
		pVMPX_key_type++;
		if(*pVMPX_key_type != findtype)
			continue;

		return pVMPX;
	}while(f--); //find req type
	*/
return NULL;
}

__forceinline SERVICE_COUNT __fastcall DH_MAP_Do_APPLY_NA_STACK__Construct_Retail_PATCH_byte_tables(__in pPATCH_HELD_GROUP pHELDGroup, __in pVM_INFO_STACK pVM_Info_Arrive, __in pPATCH_SUPPORTING_VMPX_PROFILE_INI pArbitaryFilterUnit, pPATCH_SUPPORTING_REGION_INI_STACK pSupportRegion, __in XEDPARSE* pXEDParse)
{
			register size_t reg_count_i = pArbitaryFilterUnit->Assembly_count; //
			SERVICE_ADDRES FreeHaFAddr = pSupportRegion->Region_VMP_float_pointer; //указатель на свободное место в регионе для Hooks & Filters
			register SERVICE_ADDRES fpFreeHaFAddr = pSupportRegion->Region_VMP_float_pointer; //floater по свободному региону - привязка по инструкциям
			register LPBYTE fpWriteBytesTABLE = pHELDGroup->patch_BYTES__FILTER_instruction_bytes; //таблица байт для записи фильтра!
			register WCHAR* pAsmInstruction = &pArbitaryFilterUnit->Assembly_instruction1[0];
			
			register SERVICE_ADDRES* pPredictAsmSizes = &pArbitaryFilterUnit->Predict_POST_BUILD_asm_instruction_increment_size[1]; //стек предсказания размеров инструкций
			register SERVICE_TYPE* fpPredictControlSymbolType = &pArbitaryFilterUnit->Predict_POST_BUILD_Control_symbol_type[1];
			register size_t* fpFirst_control_symbol_offset = &pArbitaryFilterUnit->First_control_symbol_offset[1]; //указатель на первый CS

			//заполнение структуры
			pHELDGroup->patch__desdonation_FILTER_addres = FreeHaFAddr; //начальный адрес 
			
			//HELD
			PBYTE fpPATCHHELDArray = &pHELDGroup->patch_BYTES__FILTER_instruction_bytes[0]; //массив из патченных байт
			pHELDGroup->patch_COUNT__FILTER_instruction_bytes = NULL;
			
		///-------------------/// calculate FILTER asm bytes after PRECISION predict ///-------------------///
		pPredictAsmSizes--;
		fpPredictControlSymbolType--;
		fpFirst_control_symbol_offset--;
		pAsmInstruction-=sizeof(pArbitaryFilterUnit->Assembly_instruction1)/sizeof(TCHAR);
		do
		{
		pPredictAsmSizes++;
		fpPredictControlSymbolType++;
		fpFirst_control_symbol_offset++;
		pAsmInstruction+=sizeof(pArbitaryFilterUnit->Assembly_instruction1)/sizeof(TCHAR);

		pXEDParse->cip = FreeHaFAddr;
			if(*fpPredictControlSymbolType == NULL) //complete instruction
			{
				quick_UNICODE_to_ASCII(pAsmInstruction, pXEDParse->instr);
			}
			else //req construct
			{
				if (*fpPredictControlSymbolType == FILTER_SUBDEFINE_CONTROL_SYMBOL_RETURN)
					continue;
				
				register SERVICE_ARRAY_ID RestoreLabelDestPosition;
				register TCHAR* fpDesdString    =  (TCHAR*)((SERVICE_ADDRES)pAsmInstruction + *fpFirst_control_symbol_offset); //restore CS shift

				switch(*fpPredictControlSymbolType) //switch CS - labels
						{
						case 1: {RestoreLabelDestPosition = pArbitaryFilterUnit->Label[1];  break;}
						case 2: {RestoreLabelDestPosition = pArbitaryFilterUnit->Label[2];  break;}
						case 3: {RestoreLabelDestPosition = pArbitaryFilterUnit->Label[3];  break;}
						case 4: {RestoreLabelDestPosition = pArbitaryFilterUnit->Label[4];  break;}
						case 5: {RestoreLabelDestPosition = pArbitaryFilterUnit->Label[5];  break;}
						case 6: {RestoreLabelDestPosition = pArbitaryFilterUnit->Label[6];  break;}
						case 7: {RestoreLabelDestPosition = pArbitaryFilterUnit->Label[7];  break;}
						case 8: {RestoreLabelDestPosition = pArbitaryFilterUnit->Label[8];  break;}
						case 9: {RestoreLabelDestPosition = pArbitaryFilterUnit->Label[9];  break;}
						default: continue;
						}//cs present SWITCH
				SERVICE_ADDRES RestoreJumpOperand = FreeHaFAddr + (pArbitaryFilterUnit->Predict_POST_BUILD_asm_instruction_increment_size[RestoreLabelDestPosition] - *(pPredictAsmSizes-1)); //расчетный адрес операнда - назначения прыжка
				quick_UNICODE_to_ASCII_with_ltoa16(pAsmInstruction,pXEDParse->instr, fpDesdString-1, RestoreJumpOperand);
			}//end if
			

				if (XEDParseAssemble(pXEDParse) == XEDPARSE_ERROR) //<<<<<<<<<< cycle XED parse
						return NULL;

						memcpy(fpPATCHHELDArray, &pXEDParse->dest[0], pXEDParse->dest_size); //cpy asms
			
			fpPATCHHELDArray+=pXEDParse->dest_size; //upd floatpointer to HELD bytes array
			FreeHaFAddr+=pXEDParse->dest_size; //upd next asm addr
			pHELDGroup->patch_COUNT__FILTER_instruction_bytes+=pXEDParse->dest_size; //upd asm counter
		}while(--reg_count_i);
		///+++++++++++++++++++++++++++/// remap & copy diplacement bytes (if present) ///+++++++++++++++++++++++++++///
		if (pHELDGroup->patch_COUNT_original_displace_JMP_intreceptor_instruction_bytes != NULL)
		{
						  
			DISASM Dissasm_struct;
			Dissasm_struct.EIP = (size_t)&pHELDGroup->patch_BYTES_original_displace_JMP_intreceptor_instruction_bytes[0];
			Dissasm_struct.Archi = 64;
			Dissasm_struct.VirtualAddr = pHELDGroup->patch__original_bytes_addres;
			pXEDParse->cip = FreeHaFAddr;
			pHELDGroup->patch_COUNT_original_displace_JMP_intreceptor_instruction_bytes = DH_MAP_Do_APPLY_NA_STACK__Remap_and_refresh_asm_pool(pHELDGroup->patch_COUNT_original_displace_JMP_intreceptor_instruction_bytes, &Dissasm_struct, pXEDParse);

			
				memcpy(fpPATCHHELDArray, &pHELDGroup->patch_BYTES_original_displace_JMP_intreceptor_instruction_bytes[0], pHELDGroup->patch_COUNT_original_displace_JMP_intreceptor_instruction_bytes); //add displacement bytes
				pHELDGroup->patch_COUNT__FILTER_instruction_bytes+=pHELDGroup->patch_COUNT_original_displace_JMP_intreceptor_instruction_bytes; //upd filter size
				FreeHaFAddr+=pHELDGroup->patch_COUNT_original_displace_JMP_intreceptor_instruction_bytes; //upd adrr
				fpPATCHHELDArray+=pHELDGroup->patch_COUNT_original_displace_JMP_intreceptor_instruction_bytes; //up fp
		}
		///+++++++++++++++++++++++++++/// calculate RETURN asm bytes ///+++++++++++++++++++++++++++///
		if(pHELDGroup->patch__return_addres_for_FILTER != NULL) //return
		{
			pXEDParse->cip = FreeHaFAddr;
			quick_UNICODE_to_ASCII_with_ltoa16(tHOOK_template,pXEDParse->instr, &tHOOK_template[4], pHELDGroup->patch__return_addres_for_FILTER);

			if (XEDParseAssemble(pXEDParse) == XEDPARSE_ERROR) //<<<<<<<<<< XED parse - RETURN
						return NULL;

			memcpy(fpPATCHHELDArray, &pXEDParse->dest[0], pXEDParse->dest_size); //add return to original
			pHELDGroup->patch_COUNT__FILTER_instruction_bytes+=pXEDParse->dest_size; //upd asm counter
			FreeHaFAddr+=pXEDParse->dest_size; //upd addr
		}
		///+++++++++++++++++++++++++++/// calculate HOOK asm bytes ///+++++++++++++++++++++++++++///
		{
		quick_UNICODE_to_ASCII_with_ltoa16(tHOOK_template,pXEDParse->instr, &tHOOK_template[4], pHELDGroup->patch__desdonation_FILTER_addres);
		pXEDParse->cip = pHELDGroup->patch__HOOK_set_addres;

			if (XEDParseAssemble(pXEDParse) == XEDPARSE_ERROR) //<<<<<<<<<< XED parse - HOOK
						return NULL;

			memcpy(&pHELDGroup->patch_BYTES_HOOK_bytes[0], &pXEDParse->dest[0], pXEDParse->dest_size);
			pHELDGroup->patch_COUNT_HOOK_instruction_bytes  = pXEDParse->dest_size;
		}
		/////////////////// !!!!!!!!!!!!!!!!!!!!!!! //////////////
	pSupportRegion->Region_VMP_float_pointer = (FreeHaFAddr & HOOK_AND_FILTERS__alignment_AND_CONST) + HOOK_AND_FILTERS__alignment_ADD_CONST; //upd указатель на свободное место в регионе для Hooks & Filters (с алигментом)
	return pHELDGroup->patch_COUNT__FILTER_instruction_bytes;
}

SERVICE_COUNT DH_MAP_Do_APPLY_NA_STACK__WritePM_Arbitary_Filter(__in pPATCH_INFO_STACK pWritePatch)
{
	register size_t i = pWritePatch->HELD_GROUP_Fill_count; //counter
	
	register SERVICE_COUNT icountsuccess = NULL; //SUCCESS count

	register pPATCH_HELD_GROUP pIteraion_PatchArrive = &pWritePatch->Containt_HELD_GROUP[0];
	pIteraion_PatchArrive--;
	do //main loop //parse cycle --------- crit addr
	{
		 pIteraion_PatchArrive++;

			if(pIteraion_PatchArrive->patch_COUNT__FILTER_instruction_bytes == NULL) //filter #1
				continue;

			if(pIteraion_PatchArrive->patch_COUNT_HOOK_instruction_bytes == NULL) //filter #2
				continue;

			if(pIteraion_PatchArrive->patch_COUNT_original_target_instruction_bytes == NULL) //filter #3
				continue;

			DH_WriteProcessAddres_DATA_nocheck((LPVOID)pIteraion_PatchArrive->patch__desdonation_FILTER_addres, pIteraion_PatchArrive->patch_COUNT__FILTER_instruction_bytes, &pIteraion_PatchArrive->patch_BYTES__FILTER_instruction_bytes[0]); //write filter code
			DH_WriteProcessAddres_DATA_nocheck((LPVOID)pIteraion_PatchArrive->patch__HOOK_set_addres, pIteraion_PatchArrive->patch_COUNT_HOOK_instruction_bytes, &pIteraion_PatchArrive->patch_BYTES_HOOK_bytes[0]); //write HOOK code

			if(pIteraion_PatchArrive->target_addres__assign_path_method == PATCH_NOP_SLED_AND_NEXT_JUMP_CONST || pIteraion_PatchArrive->target_addres__assign_path_method == PATCH_NOP_SLED_AND_PREV_JUMP_CONST)
				{
				DH_WriteProcessAddres_DATA_nocheck((LPVOID)pIteraion_PatchArrive->target_addres, pIteraion_PatchArrive->patch_COUNT_original_target_instruction_bytes, &tNOP_SLED[0]); //write NOP-SLED
				memcpy(&pIteraion_PatchArrive->patch_BYTES_revoke_target_instruction_bytes[0], &tNOP_SLED[0], pIteraion_PatchArrive->patch_COUNT_original_target_instruction_bytes); //save revoke instructions
				}//end TRUE pIteraion_PatchArrive->target_addres__assign_path_method == )
			else
			{
				if (pIteraion_PatchArrive->target_addres__assign_path_method == PATCH_COMBINE__NOP_SLED_and_DISPLACE_OTHERS_ASM_TOO_CLOSE_NEXT_JMP)
				{
					//NOP-target
					DH_WriteProcessAddres_DATA_nocheck((LPVOID)pIteraion_PatchArrive->target_addres, pIteraion_PatchArrive->patch_COUNT_original_target_instruction_bytes, &tNOP_SLED[0]); //write NOP-SLED
					memcpy(&pIteraion_PatchArrive->patch_BYTES_revoke_target_instruction_bytes[0], &tNOP_SLED[0], pIteraion_PatchArrive->patch_COUNT_original_target_instruction_bytes); //save revoke instructions

					//NOP-displace asm
					DH_WriteProcessAddres_DATA_nocheck((LPVOID)pIteraion_PatchArrive->patch__original_bytes_addres, pIteraion_PatchArrive->patch_COUNT_original_displace_JMP_intreceptor_instruction_bytes, &tNOP_SLED[0]); //write NOP-SLED
					memcpy(&pIteraion_PatchArrive->patch_BYTES_original_displace_JMP_intreceptor_instruction_bytes[0], &tNOP_SLED[0], pIteraion_PatchArrive->patch_COUNT_original_displace_JMP_intreceptor_instruction_bytes); //save revoke instructions
				}//end if PATCH_COMBINE__NOP_SLED_and_DISPLACE_OTHERS_ASM_TOO_CLOSE_NEXT_JMP
			}//end else pIteraion_PatchArrive->target_addres__assign_path_method ==PATCH_NOP_SLED_AND_NEXT_JUMP_CONST || pIteraion_PatchArrive->target_addres__assign_path_method == PATCH_NOP_SLED_AND_PREV_JUMP_CONSTPATCH_NOP_SLED_AND_NEXT_JUMP_CONST || pIteraion_PatchArrive->target_addres__assign_path_method == PATCH_NOP_SLED_AND_PREV_JUMP_CONST

			icountsuccess++;
	}while(--i); //END main loop

return icountsuccess;
}


//__forceinline SERVICE_COUNT __fastcall DH_MAP_Do_APPLY_NA_STACK__Remap_and_refresh_asm_pool(__in __out BYTE* pAsmPool, __in size_t PoolSize, __in SERVICE_ADDRES Source_CIP, __in SERVICE_ADDRES Source_VA,  __in SERVICE_ADDRES Desd_CIP, __in XEDPARSE* pXEDParse)
__forceinline SERVICE_COUNT __fastcall DH_MAP_Do_APPLY_NA_STACK__Remap_and_refresh_asm_pool(__in SERVICE_COUNT AsmCount, __in PDISASM pFILL_Dissasm, __in XEDPARSE* pFILL_XEDParse)
{
	_ASSERT(AsmCount < 200);

	BYTE CopyAsm[MAX_ASM_POOL_IN_STACK_BYTES];
	register PBYTE fpCopyAsm = &CopyAsm[0]; //flow pointer

	//register DISASM Dissasm_struct;
	
	//if(AsmCount > MAX_ASM_POOL_IN_STACK_BYTES)
		//return NULL;

	register size_t asm_instruct_size;
	register SERVICE_COUNT new_asm_size = NULL;
	SERVICE_ADDRES SaveDesdBuffer = pFILL_Dissasm->EIP;

		do
		{
			asm_instruct_size = Disasm(pFILL_Dissasm);

			pFILL_Dissasm->EIP += asm_instruct_size;
			pFILL_Dissasm->VirtualAddr += asm_instruct_size;

			DH_DISASM_to_XED_string(pFILL_Dissasm->CompleteInstr, pFILL_XEDParse->instr); //strcpy(pFILL_XEDParse->instr, pFILL_Dissasm->CompleteInstr);
			if (XEDParseAssemble(pFILL_XEDParse) == XEDPARSE_ERROR)
					return NULL;

			pFILL_XEDParse->cip += pFILL_XEDParse->dest_size;
			memcpy(fpCopyAsm, pFILL_XEDParse->dest, pFILL_XEDParse->dest_size);
			fpCopyAsm +=  pFILL_XEDParse->dest_size;

			new_asm_size += pFILL_XEDParse->dest_size;

			if(asm_instruct_size == AsmCount)
				break;

		}while(--AsmCount); //asm size parse

	memcpy((void*)SaveDesdBuffer, &CopyAsm[0], new_asm_size);

return new_asm_size;
}


SERVICE_COUNT __fastcall DH_MAP_Take_patch_list_snapshot(__in __out pPATCH_SNAPSHOT pPatch_Snapshot)
{
	register pPATCH_INFO_STACK	Selected_PATCH_Stack = NULL;
	register SERVICE_COUNT Snapshots_Count = NULL;
	pDH_ENUM_FILES_of_PROFILES pAviable_online_patches = (pDH_ENUM_FILES_of_PROFILES)malloc(MAX_CONTAINTER_FILES * sizeof(DH_ENUM_FILES_of_PROFILES));

	register size_t i = DH_Get_Current_hunt_profile_ALL_FILE_CONTAINERS(PROFILE_TYPE_PATCH__NOT_APPLIED_FLAG, &pAviable_online_patches[0]);

	if(i == NULL)
		return NULL;

	register pDH_ENUM_FILES_of_PROFILES fpCurrentLoadPatchProfile = &pAviable_online_patches[0];
	fpCurrentLoadPatchProfile--;
		do
		{
			ULONG reta;
			fpCurrentLoadPatchProfile++;
			Selected_PATCH_Stack = (pPATCH_INFO_STACK)DH_Read_Current_hunt_profile_CONTAINER(PROFILE_TYPE_PATCH__NOT_APPLIED_FLAG, &fpCurrentLoadPatchProfile->FileName[0],PROFILE_READ_ALL,&reta);

			if(Selected_PATCH_Stack == NULL) //filter #1
				continue;

			if(Selected_PATCH_Stack->owner_as_profile__of_HELD_GROUP == PATCH_OWNER_VM_COVERAGE) //filter #2 - exclude my DH_coverage prototype (ADDRES POOL)
				{free(Selected_PATCH_Stack); continue;}
					
				register size_t ii = Selected_PATCH_Stack->HELD_GROUP_Fill_count;
				if(ii == NULL)
						continue;

				register pPATCH_HELD_GROUP pCurrentHELD = &Selected_PATCH_Stack->Containt_HELD_GROUP[0];
					do
					{
						//COPY ORIGINAL TARGET
						pPatch_Snapshot->checkpoint_address1 = pCurrentHELD->target_addres; //адрес оригинальной инструкции (PATCH_OWNER_VM_OUTSIDE_EXECUTE и PATCH_OWNER_VM_INSIDE_EXECUTE) или начала блока данных (PATCH_OWNER_VM_SUPPLY15_DATA, PATCH_OWNER_VM_PCODE_DATA, PATCH_OWNER_VM_CONSTANT_DATA)
						pPatch_Snapshot->COUNT_of_original_bytes_in_checkpoint1 = pCurrentHELD->patch_COUNT_original_target_instruction_bytes; //кол-во байт
						memcpy(&pPatch_Snapshot->original_BYTES_checkpoint1[0], &pCurrentHELD->patch_BYTES_original_target_instruction_bytes[0], pCurrentHELD->patch_COUNT_original_target_instruction_bytes); //original
						memcpy(&pPatch_Snapshot->reworke_BYTES_checkpoint1[0], &pCurrentHELD->patch_BYTES_revoke_target_instruction_bytes[0], pCurrentHELD->patch_COUNT_original_target_instruction_bytes); //and his revoke

						//COPY HOOK SET
						if(Selected_PATCH_Stack->owner_as_profile__of_HELD_GROUP == PATCH_OWNER_VM_INSIDE_EXECUTE || Selected_PATCH_Stack->owner_as_profile__of_HELD_GROUP == PATCH_OWNER_VM_OUTSIDE_EXECUTE)
						{
							pPatch_Snapshot->checkpoint_address2 = pCurrentHELD->patch__HOOK_set_addres;
							pPatch_Snapshot->COUNT_of_original_bytes_in_checkpoint2 = pCurrentHELD->patch_COUNT_HOOK_instruction_bytes;
							memcpy(&pPatch_Snapshot->reworke_BYTES_checkpoint2[0], &pCurrentHELD->patch_BYTES_HOOK_bytes[0], pCurrentHELD->patch_COUNT_HOOK_instruction_bytes);
						} //end if filter - PATCH_OWNER_VM_INSIDE_EXECUTE || PATCH_OWNER_VM_OUTSIDE_EXECUTE
						else
						{pPatch_Snapshot->checkpoint_address2 = NULL; pPatch_Snapshot->checkpoint_address2 = NULL;}

							memcpy(&pPatch_Snapshot->name[0], &Selected_PATCH_Stack->name[0], sizeof(Selected_PATCH_Stack->name));
							memcpy(&pPatch_Snapshot->owner_name[0], &Selected_PATCH_Stack->owner_name[0], sizeof(Selected_PATCH_Stack->owner_name));
							pPatch_Snapshot->owner_as_profile__of_HELD_GROUP = Selected_PATCH_Stack->owner_as_profile__of_HELD_GROUP;

						pCurrentHELD++;
						Snapshots_Count++;
						pPatch_Snapshot++;
					}while(--ii);
			free(Selected_PATCH_Stack);
		}while(--i); //main snapshot loop

	
free(pAviable_online_patches);
return Snapshots_Count;
}


pPATCH_SNAPSHOT __fastcall DH_MAP_AntiRewrite_Sentinel(__in pPATCH_SNAPSHOT pSnapList, __in SERVICE_COUNT* pSnapListCount, __out SERVICE_COUNT* pAllRewritesSnapsCOUNT)
{
	BYTE outbuff[MAX_STACK_ALLOCATE_BUFFER_BYTES];
	register SERVICE_COUNT HowLeave_Bytes; //сколько осталось байт до повторения итерации
	register SERVICE_COUNT DiffBytes_count; //ко-во различных байт (не должно быть более HowLeave_Bytes);
	register PBYTE pOutBuff = &outbuff[0];
	//register SERVICE_ADDRES* fpRewriteAddresPos = &pSnapList->Rewrite_addres[0]; //start point address
	//register SERVICE_COUNT* fpRewriteAddresBytes = &pSnapList->Bytes_rewrite[0]; //bytes rewrite
	//register SERVICE_TYPE* fpRewriteAddresAbilites = &pSnapList->Detected_rewrite_abilites[0]; //rewrite abilites
	SERVICE_COUNT* pCount_rewrite_bytes1 = &pSnapList->COUNT_all_rewrites_checkpoint1;
	SERVICE_COUNT* pCount_rewrite_bytes2 = &pSnapList->COUNT_all_rewrites_checkpoint2;
	SERVICE_COUNT  Snap_with_rewrites_COUNT;	//SERVICE_COUNT* pCount_ALL_rewrite = &pSnapList->COUNT_Rewrite_struct;
	register SERVICE_COUNT SnapListCount = *pSnapListCount;
	bool UNLOCK_Rewrites;

	*pCount_rewrite_bytes1 = NULL;
	*pCount_rewrite_bytes2 = NULL;
	Snap_with_rewrites_COUNT = NULL;

	pPATCH_SNAPSHOT FirstSnapShot_INC = NULL;

pSnapList--;
	do
	{
		pSnapList++;
		register SERVICE_ADDRES* fpRewriteAddresPos = &pSnapList->Rewrite_addres[0]; //start point address
		register SERVICE_COUNT* fpRewriteAddresBytes = &pSnapList->Bytes_rewrite[0]; //bytes rewrite
		register SERVICE_TYPE* fpRewriteAddresAbilites = &pSnapList->Detected_rewrite_abilites[0]; //rewrite abilites
		register SERVICE_COUNT* fpRewriteCOUNT = &pSnapList->COUNT_Rewrite_struct; //COUNT of REWRITE struct
		*fpRewriteCOUNT = NULL; //SET CLEAR!

				//check ORIGINAL instruction set
				if(pSnapList->checkpoint_address1 != pSnapList->checkpoint_address2) //HOOKset IS NOT ORIGINAL ASM
				{
					if (::DH_ReadProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE, (LPVOID)pSnapList->checkpoint_address1, pSnapList->COUNT_of_original_bytes_in_checkpoint1, pOutBuff) == true)
					{

						HowLeave_Bytes = pSnapList->COUNT_of_original_bytes_in_checkpoint1; //first var
						PBYTE fpDump1 = pOutBuff; //dump1 fp
						PBYTE fpDump2 = &pSnapList->reworke_BYTES_checkpoint1[0]; //dump2 fp
						SERVICE_ADDRES fpTarget_process_addres = pSnapList->checkpoint_address1; //target proc addres
						UNLOCK_Rewrites = false;
						do
						{
								//ПРОВЕРКА ------- ORIGINAL INSTRUCTION ---------//
							DiffBytes_count = DH_MAP_AntiRewrite_Sentinel__compare_two_dumps(fpDump1, fpDump2, HowLeave_Bytes);
							if(DiffBytes_count != 0)
								{
								
									*fpRewriteAddresPos++ = fpTarget_process_addres;
									*fpRewriteAddresBytes++ = DiffBytes_count;
									*fpRewriteAddresAbilites++ = PATCH_REWRITE_TARGET_is_VM_ORIGINAL_TYPE_ASM;
									*fpRewriteCOUNT+=1; //inc rewrite counts

									fpTarget_process_addres+=DiffBytes_count; //UPD target process fp
									HowLeave_Bytes-=DiffBytes_count; //UPD leave count
									fpDump1+=DiffBytes_count; //upd dump1 fp
									fpDump2+=DiffBytes_count; //upd dump2 fp

									*pCount_rewrite_bytes1+=DiffBytes_count;
									UNLOCK_Rewrites = true;
								}//END IF (DiffBytes_count != 0)

						}while(DiffBytes_count != NULL);

							if(UNLOCK_Rewrites == true) //req only 1 iteration!
							{
								if(FirstSnapShot_INC == NULL)
									{FirstSnapShot_INC = pSnapList; *pSnapListCount = SnapListCount-1;} //SET RETURN VARIABLE

								Snap_with_rewrites_COUNT++;
							}//if (UNLOCK_Rewrites == true)
					}//END check original instruction
				}//critical filter #1

				//check HOOKshot instruction set (if present) - 
				if(pSnapList->checkpoint_address2 != NULL)
				{
					if (::DH_ReadProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE, (LPVOID)pSnapList->checkpoint_address2, pSnapList->COUNT_of_original_bytes_in_checkpoint2, pOutBuff) == true)
					{
	
						HowLeave_Bytes = pSnapList->COUNT_of_original_bytes_in_checkpoint2; //first var
						PBYTE fpDump1 = pOutBuff; //dump1 fp
						PBYTE fpDump2 = &pSnapList->reworke_BYTES_checkpoint2[0]; //dump2 fp
						SERVICE_ADDRES fpTarget_process_addres = pSnapList->checkpoint_address2; //target proc addres
						UNLOCK_Rewrites = false;
						do
						{
								//ПРОВЕРКА ------- HOOKset INSTRUCTION ---------//
							DiffBytes_count = DH_MAP_AntiRewrite_Sentinel__compare_two_dumps(fpDump1, fpDump2, HowLeave_Bytes);
							if(DiffBytes_count != 0)
								{
									*fpRewriteAddresPos++ = fpTarget_process_addres;
									*fpRewriteAddresBytes++ = DiffBytes_count;
									*fpRewriteAddresAbilites++ = PATCH_REWRITE_TARGET_is_MY_HOOK;
									*fpRewriteCOUNT+=1; //inc rewrite counts

									fpTarget_process_addres+=DiffBytes_count; //UPD target process fp
									HowLeave_Bytes-=DiffBytes_count; //UPD leave count
									fpDump1+=DiffBytes_count; //upd dump1 fp
									fpDump2+=DiffBytes_count; //upd dump2 fp

									*pCount_rewrite_bytes2+=DiffBytes_count;
									UNLOCK_Rewrites = true;

									if(FirstSnapShot_INC == NULL)
										FirstSnapShot_INC = pSnapList; //SET RETURN VARIABLE
								}//END IF (DiffBytes_count != 0)

						}while(DiffBytes_count != NULL);

							if(UNLOCK_Rewrites == true) //req only 1 iteration!
							{
								if(FirstSnapShot_INC == NULL)
									{FirstSnapShot_INC = pSnapList; *pSnapListCount = SnapListCount-1;} //SET RETURN VARIABLE

								Snap_with_rewrites_COUNT++;
							}//if (UNLOCK_Rewrites == true)
					}//END IF ReadProcess
				}//END check HOOKshot instruction

			
	}while(--SnapListCount); //snaplist cycle

*pAllRewritesSnapsCOUNT = Snap_with_rewrites_COUNT;
return FirstSnapShot_INC; //первый попавшейся с последующим инкрементом
}

//__forceinline SERVICE_COUNT __fastcall DH_MAP_AntiRewrite_Sentinel__compare_two_dumps(__in PBYTE Dump1, __in PBYTE Dump2, __in SERVICE_COUNT DumpCount, __in __out SERVICE_ADDRES* pF1rstDiff_Addr, __in __out SERVICE_COUNT* pF1rstDiff_Count)
__forceinline SERVICE_COUNT __fastcall DH_MAP_AntiRewrite_Sentinel__compare_two_dumps(__in PBYTE Dump1, __in PBYTE Dump2, __in SERVICE_COUNT DumpCount)
{
	if(DumpCount == NULL)
		return NULL;

	//register SERVICE_ADDRES fpRewriteAddrStartPoint = *pF1rstDiff_Addr;
	register SERVICE_COUNT DiffBytesCount = 0;
	Dump1--;
	Dump2--;
	do
	{
		Dump1++;
		Dump2++;
		if(*Dump1 == *Dump2)
			continue;
		//	{fpRewriteAddrStartPoint++; continue;}
		

		//*pF1rstDiff_Addr = fpRewriteAddrStartPoint;
			
			do
			{
				Dump1++;
				Dump2++;
				DiffBytesCount++;
				if(*Dump1 != *Dump2)
					continue;
				break;
			}while(--DumpCount); //DIFFERENT iteration
		break;
	}while(--DumpCount); //main loop

return DiffBytesCount;
}

 TCHAR* __fastcall DH_PATCH_ERROR_type_to_string(__in SERVICE_ERROR Err_type)
 {
 switch(Err_type)
			{
			case PATCH_ERROR_VM_STACK_ABSENT: {return _T("VM_STACK_ABSENT"); break;}
			case PATCH_ERROR_ADDR_POOL_ABSENT: {return _T("ADDR_POOL_ABSENT"); break;}
			case PATCH_ERROR_TARGET_ADR_in_ADDR_POOL_BAD: {return _T("ADDR_POOL_BAD"); break;}
			case PATCH_ERROR_PATCH_METHOD_NOT_ASSIGNED: {return _T("METHOD_NOT_ASSIGNED"); break;}
			case PATCH_ERROR_TARG_ADDR_NOT_VERIFY: {return _T("NOT_VERIFY"); break;}
			case PATCH_ERROR_ARBITARY_FILTER_NOT_ASSINGED: {return _T("FILTER_NOT_ASSINGED"); break;}
			case PATCH_ERROR_CANNOT_CONSTRUCT_RETAIL_ARBITARY_FILTER: {return _T("CONSTRUCT_FAILED"); break;}
			}
 return NULL;
 }

  TCHAR* __fastcall DH_PATCH_ReWRITE_type_to_string(__in SERVICE_ERROR Rewrite_type)
 {
 switch(Rewrite_type)
			{
			case PATCH_REWRITE_TARGET_is_VM_ORIGINAL_TYPE_ASM: {return _T("R_Original"); break;}
			case PATCH_REWRITE_TARGET_is_VM_INSIDE_ASM: {return _T("R_INSIDE"); break;}
			case PATCH_REWRITE_TARGET_is_MY_HOOK: {return _T("R_HOOKshot"); break;}
			case PATCH_REWRITE_TARGET_is_VM_OUTSIDE_ASM: {return _T("R_OUTSIDE"); break;}
			case PATCH_REWRITE_TARGET_is_VM_COVERAGE: {return _T("R_COVER"); break;}
			case PATCH_REWRITE_TARGET_is_VM_SUPPLY15_DATA: {return _T("R_SUPPLY15"); break;}
			case PATCH_REWRITE_TARGET_is_VM_PCODE_DATA: {return _T("R_PCODE"); break;}
			case PATCH_REWRITE_TARGET_is_VM_CONSTANT_DATA: {return _T("R_CONST"); break;}
			}
 return NULL;
 }

bool DH_MAP_XEDParse_one_step(__in XEDPARSE* pXED_parse_asm)
{
		//pXED_parse_asm->cip = fpPredictCONST; //set current position addres
		
					if (XEDParseAssemble(pXED_parse_asm) == XEDPARSE_ERROR) //XED parse +++++++++++++++++++++++++
						return false;
}

char* __fastcall DH_MAP_Determine_identical_protection_section(__in HMODULE PE_File_1, __in HMODULE PE_File_2, __out pMAP_IDENTICAL_SECTIONS_STACK pIdentSections)
{
	if ((PE_File_1 == NULL) || (PE_File_2 == NULL)) //FILTER #1
		return NULL;

	PIMAGE_DOS_HEADER pDosHeader1 = (PIMAGE_DOS_HEADER)PE_File_1;
	PIMAGE_DOS_HEADER pDosHeader2 = (PIMAGE_DOS_HEADER)PE_File_2;

	if ((pDosHeader1->e_magic != IMAGE_DOS_SIGNATURE) || (pDosHeader2->e_magic != IMAGE_DOS_SIGNATURE))//CHECK MZ
		return NULL;

	PIMAGE_NT_HEADERS pNT_Head1 = (PIMAGE_NT_HEADERS)((SERVICE_ADDRES)PE_File_1 + pDosHeader1->e_lfanew);
	PIMAGE_NT_HEADERS pNT_Head2 = (PIMAGE_NT_HEADERS)((SERVICE_ADDRES)PE_File_2 + pDosHeader2->e_lfanew);
	
	
	register PIMAGE_SECTION_HEADER pSecHeader_vpp1 = IMAGE_FIRST_SECTION(pNT_Head1); //register PIMAGE_SECTION_HEADER pSecHeader_vpp1 = (PIMAGE_SECTION_HEADER)((ULONG_PTR)(pNT_Head1)+FIELD_OFFSET(IMAGE_NT_HEADERS, OptionalHeader) + pNT_Head1->FileHeader.SizeOfOptionalHeader); //TRANSLATE TO REMOTE VIRTUAL
	register PIMAGE_SECTION_HEADER pSecHeader_vpp2 = IMAGE_FIRST_SECTION(pNT_Head2); //register PIMAGE_SECTION_HEADER pSecHeader_vpp2 = (PIMAGE_SECTION_HEADER)((ULONG_PTR)(pNT_Head2)+FIELD_OFFSET(IMAGE_NT_HEADERS, OptionalHeader) + pNT_Head2->FileHeader.SizeOfOptionalHeader); //TRANSLATE TO REMOTE VIRTUAL

	size_t NumOfSection_1 = pNT_Head1->FileHeader.NumberOfSections;
	size_t NumOfSection_2 = pNT_Head2->FileHeader.NumberOfSections;
	register PIMAGE_SECTION_HEADER* fpSection_1_for_save_pointer = &pIdentSections->IDENTICAL_Section_vpp_1[0];
	register PIMAGE_SECTION_HEADER* fpSection_2_for_save_pointer = &pIdentSections->IDENTICAL_Section_vpp_2[0];

	if (NumOfSection_1 == 0 || NumOfSection_2 == 0)//FILTER #2
		return NULL;

	if (pNT_Head1->OptionalHeader.ImageBase != pNT_Head2->OptionalHeader.ImageBase) //FILTER #3
		return NULL;

	pIdentSections->AnyFILE_1_2_IMAGEBASE = pNT_Head1->OptionalHeader.ImageBase;
	//сохранить количество секций
	pIdentSections->NumOfSection_ALL = 0;

	//НАЧАЛЬНЫЕ ЗНАЧЕНИЯ
	pIdentSections->Protection_SECTION_1 = pSecHeader_vpp1;
	pIdentSections->Protection_SECTION_2 = pSecHeader_vpp2;

	bool Succes_identical_protection_section_found = false;

	pSecHeader_vpp1--;
	do
	{
		pSecHeader_vpp1++;

		register PIMAGE_SECTION_HEADER pSecHeader_vpp2_cycle = pSecHeader_vpp2;
		register size_t NumOfSection_2_f = NumOfSection_2;
		
		pSecHeader_vpp2_cycle--;
		do
		{
			pSecHeader_vpp2_cycle++;

			if (memcmp(&pSecHeader_vpp2_cycle->Name[0], &pSecHeader_vpp1->Name[0], IMAGE_SIZEOF_SHORT_NAME))
				continue;

			Succes_identical_protection_section_found = true;
			//имена секций совпали
			*fpSection_1_for_save_pointer = pSecHeader_vpp1;
			*fpSection_2_for_save_pointer = pSecHeader_vpp2_cycle;
			pIdentSections->NumOfSection_ALL++;

			//определяем секцию, наиболее подходящую под секцию защиты (можно было отедльный цикл за раз)
			if (pIdentSections->Protection_SECTION_2->SizeOfRawData < pSecHeader_vpp2_cycle->SizeOfRawData)
			{
				if (pIdentSections->Protection_SECTION_1->SizeOfRawData < pSecHeader_vpp1->SizeOfRawData)
					switch(pSecHeader_vpp2_cycle->Characteristics & 0xFF00000FF)
						case IMAGE_SCN_CNT_CODE | IMAGE_SCN_CNT_INITIALIZED_DATA:
						case IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ:
						case IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ | IMAGE_SCN_CNT_INITIALIZED_DATA:
						case IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE:
						case IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE | IMAGE_SCN_CNT_INITIALIZED_DATA:
						{
							pIdentSections->Protection_SECTION_2 = pSecHeader_vpp2;
							pIdentSections->Protection_SECTION_1 = pSecHeader_vpp1;
						break;}
			}

			fpSection_1_for_save_pointer++;
			fpSection_2_for_save_pointer++;
			break;
		}while (--NumOfSection_2_f);
	}while (--NumOfSection_1);

	if (Succes_identical_protection_section_found == false)
		return NULL;

	return (char*)&pIdentSections->Protection_SECTION_1->Name[0];
}

char* __fastcall DH_MAP_Verify_and_set_protection_section(__in char* pSource_Section_name, __in pMAP_IDENTICAL_SECTIONS_STACK pIdentSections)
{
	register SERVICE_COUNT i = pIdentSections->NumOfSection_ALL;

	if (i == 0) //filter #1
		return NULL;
	if (strlen(pSource_Section_name) == 0)
		return NULL;

	register PIMAGE_SECTION_HEADER fpIdentSection_vpp = pIdentSections->IDENTICAL_Section_vpp_1[0];
	register PIMAGE_SECTION_HEADER fpIdentSection_vpp_2 = pIdentSections->IDENTICAL_Section_vpp_2[0];
	fpIdentSection_vpp--;
	fpIdentSection_vpp_2--;
	do
	{
		fpIdentSection_vpp++;
		fpIdentSection_vpp_2++;
		if (strcmp(pSource_Section_name, (const char*)&fpIdentSection_vpp->Name[0]) != 0)
			continue;
		if (strcmp(pSource_Section_name, (const char*)&fpIdentSection_vpp_2->Name[0]) != 0)
			continue;
		//УСТАНОВИМ НОВЫЕ ДАННЫЕ ДЛЯ СТРУКТУРЫ pMAP_IDENTICAL_SECTIONS_STACK
		pIdentSections->Protection_SECTION_1 = fpIdentSection_vpp;
		pIdentSections->Protection_SECTION_2 = fpIdentSection_vpp_2;
		return (char*)&fpIdentSection_vpp->Name[0];
	} while (--i);
return 0;
}