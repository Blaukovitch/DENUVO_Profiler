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
//EA Origin STUB service code (RING 3)
//******************************************//
//второстепенные служебные процедуры EA STUB (3 КОЛЬЦО)
//******************************************//

void EA_out_info(char* formatstring, ...)
{
	char local_buff[0x4000];
	va_list args;
	va_start(args, formatstring);
	_vsnprintf(&local_buff[0], 0x4000, formatstring, args);
	::OutputDebugStringA(local_buff);
	va_end(args);
}

__int64 EA_CALL_callback(__in __int64 score, __in char* note_string)
{
	if (func_EA_Callback && ((score >> 0x18) & 0xF) < EA_score)
	//	func_EA_Callback(EA_callback_arg1, note_string);
	return 0;
}

bool EA_is_eah_init(void)
{
	return EA_Origin_Handle != 0;
}

 pEA_HANDLE EA_get_eah(void)
{
	 return  EA_Origin_Handle;
}

void EA_Interlocked_Exchange_Add(__in LPVOID a1)//sub_14195BBF0
{
	int result = ::InterlockedExchangeAdd((volatile unsigned __int64 *)((size_t)a1 + 8), 1u);
	__asm { pushfq }
	EA_Interlocked_byte = -21;
	__asm { popfq }

	if (result == 0)
	{
	//	v3 = dword_141344688;
		//qword_141344690[v3] = a1;
	//	dword_141344688 = v3 + 1;
		return;
	}

	char* err_descr = EA_ERROR_Get_description(ORIGIN_WARNING_SDK_ALREADY_INITIALIZED);
	EA_ERROR_out(ORIGIN_WARNING_SDK_ALREADY_INITIALIZED, err_descr, "E:\ebisu\hudson\workspace\SDK-Multi-Dist\c0b625bc\origin\sdk\9.10", 0x9Fu);
	return;
}

void EA_Init_HANDLE_FREE_HANHDLE_structure(void)
{
	free(EA_Origin_Handle);
}

EA_ERROR_DESCRIPTION_lp_EN_def _fastcall EA_GetErrorDescription(EA_ERROR_ID_def Error_ID)
{
	pORIGIN_ERROR* ffpEA_Origin_error_stack = &EA_Origin_error_stack[0];
	size_t i = ERROR_COUNT;
	ffpEA_Origin_error_stack--;
	do
	{ 
		ffpEA_Origin_error_stack++;
		pORIGIN_ERROR fpEA_Origin_error_stack = *ffpEA_Origin_error_stack;
		if (fpEA_Origin_error_stack->error_id != Error_ID)
			continue;
		return fpEA_Origin_error_stack->ERROR_MACRO_0;
	}while (--i);
}

bool __fastcall EA_test_Origin_Client_CORE(__in pEA_HANDLE lpEah1)
{
	WCHAR Core_path[MAX_PATH];

	if (!EA_get_core_path(&Core_path[0]))
		return false;

	return true;
}

bool EA_get_core_path(__in WCHAR* lpEACOREPath)
{
	HKEY hKey;
	unsigned long cbData; // [rsp+30h] [rbp-268h]
	unsigned long Type; // [rsp+34h] [rbp-264h]
	WCHAR Data[MAX_PATH];
	
	if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, defws_ORIGIN_CLIENT_BRANCH, 0, KEY_WOW64_32KEY| KEY_NOTIFY| KEY_ENUMERATE_SUB_KEYS| KEY_QUERY_VALUE, &hKey)
		|| (cbData = sizeof(Data),
			Type = 0,
			RegQueryValueExW(hKey, defws_ORIGIN_CLIENT_PATH, NULL, &Type, (BYTE*)Data, &cbData)))
		return false;
	
	RegCloseKey(hKey); // error fix!  в EA хендл реестра не закрывается!

	wcscpy(lpEACOREPath, &Data[0]);
	return true;
}

__int64 __fastcall EA_complete_Origin_Client_CORE(void)
{
	WCHAR Core_path[MAX_PATH];
	wchar_t Filename[MAX_PATH]; // [rsp+70h] [rbp-C38h]
	wchar_t Ext[MAX_PATH]; // [rsp+270h] [rbp-A38h]
	

	EA_get_core_path(&Core_path[0]);
	
	_wsplitpath_s(&Core_path[0], 0i64, 0i64, 0i64, 0i64, &Filename[0], MAX_PATH, &Ext[0], MAX_PATH);

	//EA_out_info("%sDebug%s", Filename, Ext);
	wcscat(&Filename[0], &Ext[0]);
	return get_Origin_CORE_PID(&Filename[0], &Ext[0]);
}

ULONG __fastcall get_Origin_CORE_PID(const WCHAR *lpFilename, const WCHAR *lpExt)
{
	ULONG dwProcessId[1024]; // [rsp+30h] [rbp-1238h]
	ULONG ret_bytes;

	unsigned long v4; // rbx
	HANDLE v5; // rax
	void *v6; // rdi
	int v8; // ebx
	WCHAR String1[MAX_PATH]; // [rsp+1030h] [rbp-238h]

	if (!EnumProcesses(&dwProcessId[0], sizeof(dwProcessId), &ret_bytes))
		return 0; // error fix!  отсуствует обработчик для количества процессов, превышающих 1024!

	v4 = 0;
	ret_bytes >>= 2;
	if (ret_bytes)
	{
		while (1)
		{
			v5 = OpenProcess(PROCESS_QUERY_INFORMATION| PROCESS_VM_READ, 0, dwProcessId[v4]);
			v6 = v5;
			if (v5)
			{
				if (GetModuleBaseNameW(v5, 0, &String1[0], MAX_PATH)
					&& (!lstrcmpiW(&String1[0], lpFilename) || !lstrcmpiW(&String1[0], lpExt)))
				{
					v8 = dwProcessId[v4];
					CloseHandle(v6);
					return v8;
				}
				CloseHandle(v6);
			}
			v4++;
			if (v4 >= ret_bytes)
				return 0;
		}
	}
	return 0;
}


char* get_string(__in char* pDst, __in char* pSrc, __in size_t* lpSet_Dst_strlong)
{
	size_t flen = strlen(pSrc);
	char* strpointer_new = NULL;

	if (flen > MAX_BUILD_CHARS_LEN && flen < (ULONG)-1)
	{ 
		strpointer_new = (char*)malloc((flen+1)*sizeof(char));
		strcpy(strpointer_new, pSrc);
		*(char**)pDst = (char*)strpointer_new; //вместо самой строки - ссылка на строку
		*lpSet_Dst_strlong = flen;
		return strpointer_new; //возвращаем указатель на выделенную память в случае, когда длина строки больше MAX_BUILD_CHARS_LEN
	}
	else
		strcpy(pDst, pSrc);

	*lpSet_Dst_strlong = flen;
return pDst; //возврат места назначения в случае когда =< MAX_BUILD_CHARS_LEN
}

size_t sscanf_ver(__in char* lpEA_VersionString)
{
	ULONG v2; // [rsp+40h] [rbp+8h]
	ULONG v3; // [rsp+48h] [rbp+10h]
	ULONG v4; // [rsp+50h] [rbp+18h]
	ULONG v5; // [rsp+58h] [rbp+20h]

	if (!lpEA_VersionString)
		return 0i64;
	sscanf(lpEA_VersionString, "%d,%d,%d,%d", &v2, &v3, &v4, &v5);
	return v5 + ((v4 + ((v3 + 16 * v2) << 8)) << 12);
}

char* strcpy_CHARS_LEN(__out pS_EA_M_UNIT pDst, __in char* pSrc, __in size_t lpSet_Dst_strlong)
{
	char* lpchar = NULL;

		if (lpSet_Dst_strlong == NULL) //фильтр #1
		{
			pDst->string[0] = '\0';
			pDst->len = 0;
			return &pDst->string[0];
		}

		pDst->len = lpSet_Dst_strlong; //сохраняем длину в месте назначения

		if (lpSet_Dst_strlong <= pDst->MAX_strlen) //если позволяет длина строки - копируем сразу
			strcpy(&pDst->string[0], pSrc);
		else //иначе - выделяем память под длинную строку
		{
				lpchar = (char*)malloc(lpSet_Dst_strlong+sizeof('\0')); //выделяем память
				*(char**)&pDst = lpchar; //пишем указатель		
				strcpy(lpchar, pSrc); //указатель в источнике	

			return &pDst->string[0];
		}
	return &pDst->string[0];
}

bool insert_into__hud_callback(__in pS_EA_HUD_CALLBACK_INFO pInput_CALLBACK, FARPROC hud_addr, FARPROC unk3) //sub_14196F520
{
	if (pInput_CALLBACK->hud_addres_local_40 != hud_addr)
	{
		pInput_CALLBACK->hud_addres_local_40 = hud_addr;
		pInput_CALLBACK->hud_unk3_48 = unk3;
		return true;
	}

return false;
}