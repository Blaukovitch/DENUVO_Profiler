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
#include "ENGINE.h"
#include "ENGINE_Symbol_avanpost.h"

// ******************************************************************************************************************
// Symbol_avanpost - хранение базы символов и их значений
// ******************************************************************************************************************


 bool __fastcall DH_APOST_ADDNS(__in pDH_APOST_symbol_stack pTmpSymbol_struct)
{
	register pDH_APOST_symbol_stack pFree_symbol_cell = (pDH_APOST_symbol_stack)((SERVICE_ADDRES)HUNT_HIVE.Symbol_avanpost_stack.allocated_pool + (HUNT_HIVE.Symbol_Count * sizeof(DH_APOST_symbol_stack)));
	memcpy(pFree_symbol_cell,pTmpSymbol_struct,sizeof(DH_APOST_symbol_stack));

	register size_t wstrlen = wcslen(&pFree_symbol_cell->Symbol_full_String[0]); //FULL STRING
	pFree_symbol_cell->Symbol_full_String_len = wstrlen;
	if(wstrlen != NULL)
		_totupper(pFree_symbol_cell->Symbol_full_String[0]);

	if(pFree_symbol_cell->Variable != NULL)
	{
		itoa_16_fast(pFree_symbol_cell->Variable, &pFree_symbol_cell->Variable_str[0]);
		wstrlen = wcslen(&pFree_symbol_cell->Variable_str[0]);
		pFree_symbol_cell->Variable_str_len = wstrlen;
	}
	else
		pFree_symbol_cell->Variable_str_len = NULL;
		
	HUNT_HIVE.Symbol_Count++;
}