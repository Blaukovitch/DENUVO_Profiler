#include "../stdafx.h"
#include "ENGINE_SPIRUN7.h"
#include "ENGINE.h"
#include "PROFILER_ENGINE.h"
#include "ENGINE_VM_operations.h"
#include "ENGINE_Profile_operations.h"
#include "ENGINE_Symbol_avanpost.h"
/*
	DENUVO_Profiler v0.3
	--------------------
	CRACKLAB, ELF
	--------------------
	Tiberium reversing
	********************
	2011	-	2024
*/

// ******************************************************************************************************************
// ENGINE_Profile_operations_MT.cpp - операции с контейнерами (файлами) при многопоточной работе
// ******************************************************************************************************************

#define codedef_create_thread_shedule \
		*fphThread = ::CreateRemoteThreadEx(DH_Current_process_fast_handle,NULL,NULL, (LPTHREAD_START_ROUTINE)DH_PROFILE_Shedule_WRITE_threadwork_MT,pThreadWorkStack_pointer,NULL,NULL, fpTID);\
				if(*fphThread != NULL) \
					{fphThread++;fpTID++; succes_created_threads++;}


static volatile size_t afafa;
bool __fastcall DH_Profile_Shedule_threadwork__Write(__in PVOID pThreadWorkStack_pointer, __in SERVICE_TYPE StackType)
{

	if(!pThreadWorkStack_pointer) //фильтр №1
		return NULL;

	//ОБЩИЕ данные MT
	register LPDWORD fpTID; //Указатель на синхронный ThreadID стек
	register HANDLE* fphThread; //Указатель на синхронный Thread HANDLE стек
	size_t iterations_count = NULL; //количество итераций (количество сохраненных контейнеров)
	register LPDWORD fpTID_charged; //стек - сохраненные ID сохданных потоков. Источник!
	register LPDWORD fpTID_shedule; //стек планировщика ID потоков. Приёмник!
	bool* pReadyFlag; //флаг готовности планировщика
	SERVICE_COUNT* pHow_thread_has_been_created; //изначальное количество потоков
	volatile SERVICE_COUNT* pVolatile_TA_Active_THREADs;  //количество активных потоков, задействованных сейчас для исследования (требуется для планировщика задач). Переменная volatile
	LPVOID* pContextSave = NULL;

	switch (StackType)
	{
	case TW_pVM_THREADWORK: {
		pVM_THREADWORK pThreadWorkStack = reinterpret_cast<pVM_THREADWORK>(pThreadWorkStack_pointer);

		iterations_count = pThreadWorkStack->TA_Context_save_count; //количество сохраненных контекстов
		fpTID = &pThreadWorkStack->ThreadID[0]; //set null-fp
		fphThread = &pThreadWorkStack->hThread[0]; //set null-fp
		fpTID_charged = &pThreadWorkStack->ThreadID[0];
		fpTID_shedule = &pThreadWorkStack->Shedule_ThreadID[0];
		pReadyFlag = &pThreadWorkStack->Ready_flag;
		pHow_thread_has_been_created = &pThreadWorkStack->How_thread_has_been_created;
		pVolatile_TA_Active_THREADs = &pThreadWorkStack->TA_Active_Threads_count;
		pContextSave = (LPVOID*)&pThreadWorkStack->pVM_Context_save[0];
		break; }
	}

	if (iterations_count == NULL) //фильтр №1
		return false;
	// --------- get proc physical cores -----------------------
	size_t num_of_cores = 2;
		__asm
		{
		mov eax,1
		cpuid
		mov eax, 11111111b
		shr ebx, 16
		and ebx, eax
		lea rax, [num_of_cores]
		mov [rax], ebx
		}
	
		if(iterations_count < num_of_cores) //если ядер больше, чем цепочек
			num_of_cores = iterations_count;

		// -------- create threads == number of phy cores ----------------
		memset(fpTID_shedule, 0, num_of_cores * sizeof(DWORD)); //нулевые значения стека имеющихся потоков
		size_t succes_created_threads = NULL;
		size_t i = num_of_cores;
		*pReadyFlag = false;

	//	num_of_cores = 1;//test!
		switch(num_of_cores)
		{
			case 16:
				{codedef_create_thread_shedule}
			case 15:
				{codedef_create_thread_shedule}
			case 14:
				{codedef_create_thread_shedule}
			case 13:
				{codedef_create_thread_shedule}
			case 12:
				{codedef_create_thread_shedule}
			case 11:
				{codedef_create_thread_shedule}
			case 10:
				{codedef_create_thread_shedule}
			case 9:
				{codedef_create_thread_shedule}
			case 8:
				{codedef_create_thread_shedule}
			case 7:
				{codedef_create_thread_shedule}
			case 6:
				{codedef_create_thread_shedule}
			case 5:
				{codedef_create_thread_shedule}
			case 4:
				{codedef_create_thread_shedule}
			case 3:
				{codedef_create_thread_shedule}
			case 2:
				{codedef_create_thread_shedule}
			case 1:
			case 0:
				{codedef_create_thread_shedule break;}
			default:
				{
						do
						{
							*fphThread = ::CreateRemoteThreadEx(DH_Current_process_fast_handle,NULL,NULL, (LPTHREAD_START_ROUTINE)DH_PROFILE_Shedule_WRITE_threadwork_MT, pThreadWorkStack_pointer,NULL,NULL, fpTID);
							if(*fphThread != NULL) //condition for inc
								{fphThread++;fpTID++; succes_created_threads++;}
						}while(--i);				
				} //def cycle
		}//end coreswitcher

		//set scheduler step-size
		SERVICE_COUNT ii = iterations_count;

		//shedule ops
		register unsigned long thread_i = succes_created_threads;
		i = MAX_STACK_SEQ; //по всему стеку

		LPDWORD fpTID_charged_restore = fpTID_charged; //null fp FOR restore
		fpTID_shedule--;
		pContextSave--;
		do
		{
			fpTID_shedule++; //synchro inc  shedule_stack
			pContextSave++; //synchro inc  WRITE/SAVE context pointer
			if (*pContextSave == NULL)
				continue;

			if (ii-- == NULL)//количество успешно найденных контекстов для записи, разбросанных по всему стеку (учитвая все, втч последний)
				break;

					*fpTID_shedule = *fpTID_charged++; //shedule current context - current charged thread
					if(--thread_i != NULL) //check for thread-empty list
						continue;

						thread_i = succes_created_threads; //restore success threads count
						fpTID_charged = fpTID_charged_restore; //restore null-fp
		}while(--i);

		*pHow_thread_has_been_created = succes_created_threads; //начальное значение потоков
		*pVolatile_TA_Active_THREADs = succes_created_threads; //количество зайдествованных потоков
		*pReadyFlag = true; //get ready
		afafa = 0;
return true;
}

DWORD WINAPI DH_PROFILE_Shedule_WRITE_threadwork_MT(LPVOID Param) //VM profile, VM billets
{
	register size_t TID = ::GetCurrentThreadId(); //get current TID
	pVM_THREADWORK pThreadWorkStack = reinterpret_cast<pVM_THREADWORK>(Param);

	
	register LPDWORD fpTID_shedule_stack = &pThreadWorkStack->Shedule_ThreadID[0]; //set null fp
	//SERVICE_ERROR* fpErr_seq_state = &pThreadWorkStack->Err_seq_state[0]; //set null fp
	pVMPX_VM_PROFILE_INFO_STACK* fpResult_VM_Context = &pThreadWorkStack->pVM_Context_save[0];
	DWORD WriteBy = NULL;

	DISASM Dissasm_struct;

	//set local copy of HUNT_HIVE.PROFILE_PATH
	TCHAR MT_PROFILE_PATH[MAX_PATH * 2];
	register TCHAR* pMT_PROFILE_PATH_0ptr = &MT_PROFILE_PATH[0];
	wcscpy(&MT_PROFILE_PATH[0], &HUNT_HIVE.PROFILE_PATH[0]);

	//fpErr_seq_state--;
	fpTID_shedule_stack--;
	fpResult_VM_Context--;

		//waiting mode
		do {::SleepEx(0, false);}
		while(pThreadWorkStack->Ready_flag == false);
	
	//set scheduler step-size
	SERVICE_COUNT iterations_count = pThreadWorkStack->TA_Context_save_count;
	SERVICE_COUNT i = MAX_STACK_SEQ;
	do
	{
		//fpErr_seq_state++;
		fpTID_shedule_stack++;
		fpResult_VM_Context++;

		register pVMPX_VM_PROFILE_INFO_STACK pSAVE_VMPX_VM_InfoStack = *fpResult_VM_Context;

		if (pSAVE_VMPX_VM_InfoStack == 0) //фильтр №1
			continue;

		if (iterations_count-- == 0)//подсчёт всех найденных контейнеров без привязки по потокам (общий для всех)
			break;

		if (*fpTID_shedule_stack != TID) //поиск своихъ
			continue;


		if((pSAVE_VMPX_VM_InfoStack->is_vm_billet == false) && (pSAVE_VMPX_VM_InfoStack->identeficated_handles_count != NULL))
			DH_Write_Current_hunt_profile_CONTAINER_MT(PROFILE_TYPE_VM, &pSAVE_VMPX_VM_InfoStack->vm_name[0], (BYTE*)pSAVE_VMPX_VM_InfoStack, pSAVE_VMPX_VM_InfoStack->retail_VM_profile_stack_size, pMT_PROFILE_PATH_0ptr);
		else
			DH_Write_Current_hunt_profile_CONTAINER_MT(PROFILE_TYPE_VM_BILLET, &pSAVE_VMPX_VM_InfoStack->vm_name[0], (BYTE*)pSAVE_VMPX_VM_InfoStack, pSAVE_VMPX_VM_InfoStack->retail_VM_profile_stack_size, pMT_PROFILE_PATH_0ptr);
		//free memory
		::VirtualFree(pSAVE_VMPX_VM_InfoStack, 0, MEM_RELEASE);
		*fpResult_VM_Context = NULL;

	}while(--i);

	::InterlockedDecrement(&pThreadWorkStack->TA_Active_Threads_count);
return NULL;
}

bool __inline DH_Write_Current_hunt_profile_CONTAINER_MT(__in UINT PROFILE_x_TYPE, __in WCHAR* PROFILE_x_CONTAINT_FILE_NAME, __in BYTE* write_buff, __in size_t num_bytes_to_write, __in TCHAR* pLocalHUNTPROFILEPATCH_copy)  //записывает данные HIVE профиля с типом MT
{
	if (num_bytes_to_write == NULL)
		return false;

	register bool in_filename_ext_preset = DH_LOCAL_is_filename_containt_profile_extension(PROFILE_x_TYPE, PROFILE_x_CONTAINT_FILE_NAME);

	
	switch (PROFILE_x_TYPE)
	{
	case PROFILE_TYPE_VM:
	{
		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_VM_nfo_FOLDER_NAME);
		if (in_filename_ext_preset == false)
			wcscat(PROFILE_x_CONTAINT_FILE_NAME, PROFILE_VM_nfo_EXTENTION_CRITICAL);
		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_x_CONTAINT_FILE_NAME);
		break;
	}
	case PROFILE_TYPE_PATCH_ONLINE:
	{
		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_PATCH_nfo_FOLDER_NAME);
		if (in_filename_ext_preset == false)
			wcscat(PROFILE_x_CONTAINT_FILE_NAME, PROFILE_PATCH_nfo_EXTENTION_CRITICAL);
		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_x_CONTAINT_FILE_NAME);
		break;
	}

	case PROFILE_TYPE_PATCH__NOT_APPLIED_FLAG:
	{
		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_PATCH_nfo_FOLDER_NAME);
		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_PATCH_nfo__NOT_APPLIED_FOLDER_NAME);
		if (in_filename_ext_preset == false)
			wcscat(PROFILE_x_CONTAINT_FILE_NAME, PROFILE_PATCH_nfo_EXTENTION_CRITICAL);
		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_x_CONTAINT_FILE_NAME);
		break;
	}
	case PROFILE_TYPE_VM_ADDRES_POOL:
	{
		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_VM_nfo_FOLDER_NAME);
		if (in_filename_ext_preset == false)
			wcscat(PROFILE_x_CONTAINT_FILE_NAME, PROFILE_VM_addres_pool_EXTENTION_CRITICAL);
		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_x_CONTAINT_FILE_NAME);
		break;
	}
	case PROFILE_TYPE_TRACE_VMP2_LOG:
	{
		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_TRACE_VMP2_LOG_nfo_FOLDER_NAME);
		if (in_filename_ext_preset == false)
			wcscat(PROFILE_x_CONTAINT_FILE_NAME, PROFILE_TRACE_VMP2_LOG_nfo_EXTENTION_CRITICAL);
		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_x_CONTAINT_FILE_NAME);
		break;
	}
	case PROFILE_TYPE_PATCH_REGIONS_INFO_INI:
	{
		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_PATCH_nfo_FOLDER_NAME);
		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_PATCH_nfo_CONFIG_FILE_REGION_INI);
		break;
	}
	case PROFILE_TYPE_VM_BILLET:
	{
		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_VM_nfo_FOLDER_NAME);
		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_VM_BILLET_FOLDER_NAME);
		if (in_filename_ext_preset == false)
			wcscat(PROFILE_x_CONTAINT_FILE_NAME, PROFILE_VM_BILLET_EXTENTION_CRITICAL);
		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_x_CONTAINT_FILE_NAME);
		break;
	}

	case PROFILE_TYPE_PIRATE_MAP:
	{
		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_PIRATE_MAP_nfo_FOLDER_NAME);
		if (in_filename_ext_preset == false)
			wcscat(PROFILE_x_CONTAINT_FILE_NAME, PROFILE_PIRATE_MAP_nfo_EXTENTION_CRITICAL);
		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_x_CONTAINT_FILE_NAME);
		break;
	}
	}

	register HANDLE hFile = CreateFile(pLocalHUNTPROFILEPATCH_copy,                // name of the write
		GENERIC_WRITE,          // 
		FILE_SHARE_WRITE,       // WRITE
		NULL,                   // default security
		CREATE_ALWAYS,             // NEW
		FILE_ATTRIBUTE_NORMAL,  // normal file
		NULL);
	if (hFile == INVALID_HANDLE_VALUE) //try rewrite
		{A_RETURN_ORIGINAL_PROFLE_PATH_MT_copy(pLocalHUNTPROFILEPATCH_copy); return false;}

	unsigned long Return_Writed_Bytes;
	bool bErrorFlag = ::WriteFile(hFile, write_buff, num_bytes_to_write, &Return_Writed_Bytes, NULL);

	::CloseHandle(hFile);
	if (bErrorFlag == false)
			{A_RETURN_ORIGINAL_PROFLE_PATH_MT_copy(pLocalHUNTPROFILEPATCH_copy); return false;}

	A_RETURN_ORIGINAL_PROFLE_PATH_MT_copy(pLocalHUNTPROFILEPATCH_copy);
	return true;
	
}

PVOID __inline DH_Read_Current_hunt_profile_CONTAINER__readall_MT(__in ULONG PROFILE_x_TYPE, __in WCHAR* PROFILE_x_CONTAINT_FILE_NAME, __in TCHAR* pLocalHUNTPROFILEPATCH_copy)  //считывает данные - не предназнеачен для чтения данных с конфигурационных файлов
{
	register HANDLE FindHandle = 0;

	register bool in_filename_ext_preset = DH_LOCAL_is_filename_containt_profile_extension(PROFILE_x_TYPE, PROFILE_x_CONTAINT_FILE_NAME);

	register LPCWSTR ScanFileName;
	switch (PROFILE_x_TYPE)
	{
	case PROFILE_TYPE_VM:
	{
		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_VM_nfo_FOLDER_NAME);

		if (in_filename_ext_preset == false)
			wcscat(PROFILE_x_CONTAINT_FILE_NAME, PROFILE_VM_nfo_EXTENTION_CRITICAL);

		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_x_CONTAINT_FILE_NAME);
		codedef_set_default_ops_filename(pLocalHUNTPROFILEPATCH_copy);
		break;
	}

	case PROFILE_TYPE_PATCH__NOT_APPLIED_FLAG:
	{
		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_PATCH_nfo_FOLDER_NAME);
		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_PATCH_nfo__NOT_APPLIED_FOLDER_NAME);

		if (in_filename_ext_preset == false)
			wcscat(PROFILE_x_CONTAINT_FILE_NAME, PROFILE_PATCH_nfo_EXTENTION_CRITICAL);

		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_x_CONTAINT_FILE_NAME);
		codedef_set_default_ops_filename(pLocalHUNTPROFILEPATCH_copy);
		break;
	}

	case PROFILE_TYPE_PATCH_ONLINE:
	{
		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_PATCH_nfo_FOLDER_NAME);

		if (in_filename_ext_preset == false)
			wcscat(PROFILE_x_CONTAINT_FILE_NAME, PROFILE_PATCH_nfo_EXTENTION_CRITICAL);

		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_x_CONTAINT_FILE_NAME);
		codedef_set_default_ops_filename(pLocalHUNTPROFILEPATCH_copy);
		break;
	}

	case PROFILE_TYPE_LIC_DATA_DUMP:
	{
		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_LIC_DATA_DUMP_nfo_FOLDER_NAME);
		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_x_CONTAINT_FILE_NAME);
		codedef_set_default_ops_filename(pLocalHUNTPROFILEPATCH_copy);
		break;
	}

	case PROFILE_TYPE_VM_ADDRES_POOL:
	{
		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_VM_nfo_FOLDER_NAME);

		if (in_filename_ext_preset == false)
			wcscat(PROFILE_x_CONTAINT_FILE_NAME, PROFILE_VM_addres_pool_EXTENTION_CRITICAL);

		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_x_CONTAINT_FILE_NAME);
		codedef_set_default_ops_filename(pLocalHUNTPROFILEPATCH_copy);
		break;
	}

	case PROFILE_TYPE_TRACE_VMP2_LOG:
	{
		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_TRACE_VMP2_LOG_nfo_FOLDER_NAME);

		if (in_filename_ext_preset == false)
			wcscat(PROFILE_x_CONTAINT_FILE_NAME, PROFILE_TRACE_VMP2_LOG_nfo_EXTENTION_CRITICAL);

		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_x_CONTAINT_FILE_NAME);
		codedef_set_default_ops_filename(pLocalHUNTPROFILEPATCH_copy);
		break;
	}

	case PROFILE_TYPE_VM_BILLET:
	{
		if (in_filename_ext_preset == false)
			wcscat(PROFILE_x_CONTAINT_FILE_NAME, PROFILE_VM_VMPX_EXTENTION_CRITICAL);

		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_VM_nfo_FOLDER_NAME);
		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_VM_BILLET_FOLDER_NAME);
		codedef_set_default_ops_filename(pLocalHUNTPROFILEPATCH_copy);
		break;
	}

	case PROFILE_TYPE_PIRATE_MAP:
	{
		if (in_filename_ext_preset == false)
			wcscat(PROFILE_x_CONTAINT_FILE_NAME, PROFILE_PIRATE_MAP_nfo_EXTENTION_CRITICAL);

		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_PIRATE_MAP_nfo_FOLDER_NAME);
		wcscat(pLocalHUNTPROFILEPATCH_copy, PROFILE_x_CONTAINT_FILE_NAME);
		codedef_set_default_ops_filename(pLocalHUNTPROFILEPATCH_copy);
		break;
	}
	default: {__assume(0); return false; }
	}

	DWORD Return_Readed_Bytes = NULL;
	if (::GetFileAttributesW(ScanFileName) == (DWORD)INVALID_HANDLE_VALUE)
	{
		A_RETURN_ORIGINAL_PROFLE_PATH_MT_copy(pLocalHUNTPROFILEPATCH_copy);
		//A_RETURN_ORIGINAL_VMPX_PROFLE_PATH;
		return false;
	}

	HANDLE hFile = CreateFile(ScanFileName,                // name of the write
		GENERIC_READ,          // 
		0,                      // do not share
		NULL,                   // default security
		OPEN_EXISTING,             // 
		FILE_ATTRIBUTE_NORMAL,  // normal file
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		A_RETURN_ORIGINAL_PROFLE_PATH_MT_copy(pLocalHUNTPROFILEPATCH_copy);
		//A_RETURN_ORIGINAL_VMPX_PROFLE_PATH;
		return false;
	}

	ULONG sizlimits = 0x100000; unsigned long num_bytes_of_read = ::GetFileSize(hFile, &sizlimits);


	// if(num_bytes_of_read < sizeof(PROFILER_VM))
	//return false;

	PVOID allocate = malloc(num_bytes_of_read + 1); //allocate mem
													//PVOID allocate = ::VirtualAlloc(NULL, num_bytes_of_read, MEM_COMMIT, PAGE_READWRITE);

	bool bErrorFlag = ReadFile(hFile, allocate, num_bytes_of_read, &Return_Readed_Bytes, NULL);
	::CloseHandle(hFile);
	if (bErrorFlag == false || Return_Readed_Bytes == NULL)
	{
		A_RETURN_ORIGINAL_PROFLE_PATH_MT_copy(pLocalHUNTPROFILEPATCH_copy);
		free(allocate);
		//A_RETURN_ORIGINAL_VMPX_PROFLE_PATH;
		return false;
	}

	A_RETURN_ORIGINAL_PROFLE_PATH_MT_copy(pLocalHUNTPROFILEPATCH_copy);
	//A_RETURN_ORIGINAL_VMPX_PROFLE_PATH;
	return allocate;

}