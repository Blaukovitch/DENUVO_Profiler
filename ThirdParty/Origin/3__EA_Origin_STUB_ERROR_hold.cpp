/*
	DENUVO_Profiler v0.3
	--------------------
	CRACKLAB, ELF
	--------------------
	Tiberium reversing
	********************
	2011	-	2024
*/

#include "../../stdafx.h"
#include "3__EA_Origin_STUB.h"
#include "3__EA_Origin_errors_stack.h"
//3rd pary code
//EA Origin STUB error hold (RING 2a)
//***************************************************************************************//
//ключевые служебные процедуры EA STUB по обработке собственных ошибок (трюм ошибок)
//***************************************************************************************//

//>>>>>>>> 2-A  ќЋ№÷ќ <<<<<<<<< 1 ступень//
char* EA_ERROR_Get_description(__in EA_ERROR_ID_def ea_error_id)
{
	EA_CALL_callback(EA_DEF_ERROR_SCORE, "OriginGetErrorDescription entered");

	pORIGIN_ERROR error_info = EA_ERROR_find_error_description_by_error_code(ea_error_id);
	if (error_info)
		return error_info->ERROR_Description_EN;

	return def_str_NoAdditionalInfo;
}


pORIGIN_ERROR EA_ERROR_find_error_description_by_error_code(__in EA_ERROR_ID_def ea_error_id)
{
	//стек ошибок
	pORIGIN_ERROR ppERROR_stack_Local = EA_Origin_error_stack[0];

	register size_t i = 0;
	
	//перечисл€ем код ошибок
	while (ppERROR_stack_Local->error_id != ea_error_id)
	{
		ppERROR_stack_Local++;
		i++;
		if (i >= ERROR_COUNT)
			return NULL;
	}
	//возвращаем описание 
	return ppERROR_stack_Local;
}

bool EA_ERROR_out(__in unsigned long bit_lo, char* formatstring, ...)
{
	char local_buff[0x4000];
	va_list args;
	va_start(args, formatstring);
	_vsnprintf(&local_buff[0], 0x4000, formatstring, args);

	if (EA_error_execute_subs & enERROR_set_skip_execution)
		EA_error_execute_subs_alternative_out((size_t)EA_error_execute_subs_alternative_out, bit_lo, formatstring);
	else
	{
		EA_error_execute_subs_alternative_out = (pErrorOutFunc)EA_ERROR_OutputDebugString;
		EA_error_execute_subs_alternative_out((size_t)EA_error_execute_subs_alternative_out, bit_lo, formatstring);
	}
	
	va_end(args);
}

void __fastcall EA_ERROR_OutputDebugString(size_t a1, int byte_break, const CHAR *out_string)
{
	OutputDebugStringA(out_string);
	if (byte_break == enERROR_set_Break_after_OutputDebugString)
	{
		if (IsDebuggerPresent())
			DebugBreak();
	}
}

//наша функа (в оригинале RU-€зыка нет)
