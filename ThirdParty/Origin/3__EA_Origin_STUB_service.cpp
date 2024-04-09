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
//EA Origin STUB service code (RING 2)
//******************************************//
//ключевые служебные процедуры EA STUB (2 КОЛЬЦО)
//******************************************//

//>>>>>>>> 2 КОЛЬЦО <<<<<<<<< 1 ступень//
pEA_HANDLE EA_Init_HANDLE_Fill_HANHDLE_structure(__in __out pEA_HANDLE pEA_handle)
{
	pEA_handle->free_EA_proc = (FARPROC)&EA_Init_HANDLE_FREE_HANHDLE_structure;
	::InterlockedExchange(&pEA_handle->Exchange_volatile, 0);
	pEA_handle->filed_1 = 0;
	pEA_handle->lpMessage_thread_stack_4 = (pEA_MESSAGE_THREAD_STACK)malloc(sizeof(S_EA_MESSAGE_THREAD_STACK));
		pEA_handle->lpMessage_thread_stack_4->heap_alloc_1 = pEA_handle->lpMessage_thread_stack_4;
		pEA_handle->lpMessage_thread_stack_4->heap_alloc_2 = pEA_handle->lpMessage_thread_stack_4;
		pEA_handle->lpMessage_thread_stack_4->heap_alloc_3 = pEA_handle->lpMessage_thread_stack_4;
		*(WORD*)&pEA_handle->lpMessage_thread_stack_4->union_state_alloc = 0x101;
		pEA_handle->lpMessage_thread_stack_4->ea_digital_signature = 0;
		pEA_handle->lpMessage_thread_stack_4->lp_answer = 0;
	pEA_handle->filed_3 = 0;
	pEA_handle->lpStructure_0x40 = 0; //malloc!
	pEA_handle->filed_5 = 0;
	pEA_handle->MessageThread_state_6 = 0;
	pEA_handle->Message_Thread_READY_7a = 0;
	pEA_handle->Message_Thread_waiting_7b = 0;
	::InitializeCriticalSection(&pEA_handle->Message_Thread_critical_8);
	::InitializeCriticalSection(&pEA_handle->Message_Recv_critical_14);
	::InitializeCriticalSection(&pEA_handle->Critical_20);
	memset(&pEA_handle->Semaphore_26, 0, sizeof(pEA_handle->Semaphore_26));
	pEA_handle->filed_29 = 0;
	pEA_handle->filed_30 = 0;
	memset(&pEA_handle->Semaphore_mutex_31, 0, sizeof(pEA_handle->Semaphore_mutex_31));
	EA_WSA_StartUP(&pEA_handle->wsa_stack_45);
	memset(&pEA_handle->filed_63__array_4[0], 0, sizeof(pEA_handle->filed_63__array_4));
	memset(&pEA_handle->filed_68__array_4[0], 0, sizeof(pEA_handle->filed_68__array_4));
	memset(&pEA_handle->filed_73__array_4[0], 0, sizeof(pEA_handle->filed_73__array_4));
	pEA_handle->send_MESSAGE_ID_inc_78 = 1;
	memset(&pEA_handle->HUD_CALLBACKS_stack_79[0], 0, sizeof(pEA_handle->HUD_CALLBACKS_stack_79));
	*(__int64*)&pEA_handle->ORIGIN_CORE_VER_106[0] = 0; //*(__int64*)&pEA_handle->ORIGIN_CORE_VER_106[8] = 0;
	pEA_handle->origin_core_ver_len_108 = 0;
	pEA_handle->max_origin_core_ver_len_109 = MAX_BUILD_CHARS_LEN;
	memset(&pEA_handle->game_id[0], 0, sizeof(pEA_handle->game_id[0]));
	pEA_handle->game_id_len_112 = 0;
	pEA_handle->max_game_id_len_113 = MAX_BUILD_CHARS_LEN;
	pEA_handle->filed_114 = 0;
	pEA_handle->filed_115 = 0;
	pEA_handle->filed_116 = 0;
	pEA_handle->filed_117 = 0;
	pEA_handle->filed_118 = 0;
}

static LPVOID EA_WSA_struct[5];
pS_EA_WSA_ADDON EA_WSA_StartUP(__in __out pS_EA_WSA_ADDON lpWSA_struct)
{

	WSAData WSAData; // [sp+30h] [bp-1B8h]@1

	lpWSA_struct->lpStruct = &EA_WSA_struct[0]; //*(size_t *)a1 = &unk_1406E8C80;
	//lpWSA_struct->filed_8 = 0; //nullptr((size_t)a1 + 8);
	lpWSA_struct->revice_responce_status_10 = ERROR_INVALID_ANSWER;
	::InitializeCriticalSectionAndSpinCount(&lpWSA_struct->Critical_wsa_recv_16, 100); //InitializeCriticalSectionAndSpinCount((LPCRITICAL_SECTION)((size_t)a1 + 16), 0x100u);
	lpWSA_struct->recive_thread_ready_72 = false; //	*(BYTE *)((size_t)a1 + 72) = 0;
	lpWSA_struct->socket_80 = 0; //*(size_t *)((size_t)a1 + 80) = -1i64;
	lpWSA_struct->exchange_flag_ready_88 = 0; //	*(BYTE *)((size_t)a1 + 88) = 0;
	WSA_init_recv_stack(&lpWSA_struct->recive_stack_96, EA_RECV_ANSWER_BUFF_SIZE, 0); //memset(&lpWSA_struct->recv_stack[0], 0, sizeof(lpWSA_struct->recv_stack)); //sub_1419A4670((size_t)a1 + 96, 0x20000i64, 0i64);
	

	::WSAStartup(WINSOCK_VERSION, &WSAData); //WSAStartup(0x202u, &WSAData);

	return lpWSA_struct;
}

//>>>>>>>> 2 КОЛЬЦО <<<<<<<<< 2 ступень//
pEA_HANDLE __fastcall EA_probe(pEA_HANDLE lpEah1, int a2, int a3, __in pEA_ACCESS_request pEA_access_req_struct)
{
	S_EA_MESSAGE_ANSWER0_EVENT EALS_answer_header;
	EA_Message_ops ea_initial_caller_class;

	if (!pEA_access_req_struct) //filter #0
		return (pEA_HANDLE)ORIGIN_ERROR_CORE_NOTLOADED;

	lpEah1->PROBE_105 = a2;
	strcpy(&lpEah1->game_id[0], pEA_access_req_struct->ContentId); //strcpy_s(&lpEah1->game_id[0], strlen(game_id), game_id);

	if (a3)
		lpEah1->PROBE_105 = a3;
	else
		lpEah1->PROBE_105 = EA_probe_const1;

	//* проверка установленного core в реестре *//
	if(!EA_test_Origin_Client_CORE(lpEah1))
		return (pEA_HANDLE)ORIGIN_ERROR_CORE_NOT_INSTALLED;

	//** извлечение path из реестра и получение PID запущенного процесса core **//
	if (!EA_complete_Origin_Client_CORE())
	{
		//if (lpEah1->PROBE_105 & 0x200)
			return (pEA_HANDLE)ORIGIN_ERROR_CORE_NOTLOADED;
		//запуск процесса origin.exe __int64 __fastcall start_ORIGIN_SERVER(ea_handle *eahx)
	}

	//* инициализация и запуск обработчика-парсера XML-сообщений MessageThread EA*//
	//ссылка на class обработки сообщений MessageThread
	EALS_answer_header.Virtual_proc_1 = &ea_initial_caller_class;

	//** инициализация структуры-пула обмена сообщениями**//
	EA_initial_answer_enpointer(&EALS_answer_header, defs_Message_event, defs_ElectronicArts_LosAngeles_watermark);

	::EnterCriticalSection(&lpEah1->Message_Thread_critical_8);
	//** создание цифровой сигнатуры поставщика **//
	size_t digital_watermark = (ea_initial_caller_class.EA_digital_watermark_signature(defs_ElectronicArts_LosAngeles_watermark)) << 32;
		//! стек для извлеченных значений из XML-сообщений передается как &lpEah1->lpMessage_thread_stack_4->heap_alloc_2->lp_answer !//
	EA_initial_Message_threadwork_stack(&lpEah1->lpMessage_thread_stack_4, &digital_watermark);
	::LeaveCriticalSection(&lpEah1->Message_Thread_critical_8);

	//** создание потока сообщений (Message Thread) **//
	EA_begin_message_thread(lpEah1);
	MT_Signal_initial(&EALS_answer_header.MT_Signal_stack, 0);
	InitializeCriticalSectionAndSpinCount(&EALS_answer_header.MT_Signal_stack.Crit_section_signal, UCHAR_MAX+1);

	//* попытки соединения с Origin CORE *//
	ULONG count = 0;
	while (!htons_connect(
		&lpEah1->wsa_stack_45,
		&lpEah1->Semaphore_mutex_31,
		&lpEah1->Message_Recv_critical_14,
		(u_short*)&lpEah1->PROBE_105))
	{
		ULONG wait_time = 1000; //mili seconds
		SwitchThread((DWORD *)&wait_time);
		if (++count >= EA_CORE_connection_try_limit)
		{
			codedef_delete_critical_and_semaphore(EALS_answer_header.MT_Signal_stack.Crit_section_signal, EALS_answer_header.MT_Signal_stack.hSem)
			return (pEA_HANDLE)ORIGIN_ERROR_CORE_NOTLOADED;
		}
	}

	//* ожидание ответа от потока MessageThread - получение head (получение LSX 'приглашения') *//
	size_t iticks = GetTickCount64() + MT_WAITING_DEFAULT_TIME_MSEC;
	if (MT_waitng_thread_signal(&EALS_answer_header.MT_Signal_stack, &iticks))
	{
		codedef_delete_critical_and_semaphore(EALS_answer_header.MT_Signal_stack.Crit_section_signal, EALS_answer_header.MT_Signal_stack.hSem)
		return (pEA_HANDLE)ORIGIN_ERROR_LSX_NO_RESPONSE;
	}

	//* (проверка) контейнер 'version' (версия Origin CORE) из заголовчного сообщения ('приглашения') *//
	//** копирование строки 'версия' Origin CORE в EA handle. Захват указателя на строку **//
	char* locked_lpointer_origin_core_ver = &lpEah1->ORIGIN_CORE_VER_106[0];
	if (&lpEah1->ORIGIN_CORE_VER_106[0] != &EALS_answer_header.version_6[0])
		locked_lpointer_origin_core_ver = get_string(&lpEah1->ORIGIN_CORE_VER_106[0], &EALS_answer_header.version_6[0], &lpEah1->origin_core_ver_len_108);
		if (sscanf_ver(locked_lpointer_origin_core_ver) < EA_YOU_SDK_VERSION)
			/*  if ( eah0->field_0[109] >= 0x10ui64 ) //v55 > указателя стека
			v106 = *v106;
			*v55 = v106;
			v55[3] = eah0;*/
				return (pEA_HANDLE)ORIGIN_ERROR_CORE_INCOMPATIBLE_VERSION;

	//* отправка запроса Request *//
	EA_Authorize__send_head_request_recipient(lpEah1, &EALS_answer_header.seance_key_2[0], pEA_access_req_struct);

	codedef_delete_critical_and_semaphore(EALS_answer_header.MT_Signal_stack.Crit_section_signal, EALS_answer_header.MT_Signal_stack.hSem)

}

//>>>>>>>> 2 КОЛЬЦО <<<<<<<<< 3 ступень//
FARPROC EA_HOLD_ON_HUD(__in pEA_HANDLE lpEah2, unsigned int count, FARPROC hud_addr, FARPROC unk3)
{
	char* error_descr_local;
	EA_ERROR_ID_def error_id_local;
	size_t error_string_position;
	//unk3a = unk3;
	//hud_addr_local = hud_addr;
	if (count == ERROR_INVALID_ANSWER)
	{
		pS_EA_HUD_CALLBACK_INFO* callback_stack_pointer_79 = &lpEah2->HUD_CALLBACKS_stack_79[0];
		size_t i = const_HUD_CALLBACK_STACK_SIZE;
		do
		{
			pS_EA_HUD_CALLBACK_INFO fpHud_callback = *callback_stack_pointer_79;
			//v9 = (struct _RTL_CRITICAL_SECTION *)(*(__int64(__fastcall **)(__int64))(*(_QWORD *)*eah_filed_79 + 48i64))(*eah_filed_79);
			EnterCriticalSection(&fpHud_callback->hud_crit_section);
			fpHud_callback->hud_addres_local_40 = hud_addr; //*(_QWORD *)(v8 + 40) = hud_addr_local;
			fpHud_callback->hud_unk3_48 = unk3;  //*(_QWORD *)(v8 + 48) = unk3a;
			LeaveCriticalSection(&fpHud_callback->hud_crit_section);
			++callback_stack_pointer_79;
			--i;
		} while (i);
		return 0;
	}
	else
	{
		if (count > const_HUD_CALLBACK_STACK_SIZE - 1)
		{
			error_descr_local = EA_ERROR_Get_description(ORIGIN_ERROR_NOT_FOUND);
			error_string_position = 93;
			error_id_local = ORIGIN_ERROR_NOT_FOUND;
		}
		else if (hud_addr)
		{
			
				
			if (insert_into__hud_callback(lpEah2->HUD_CALLBACKS_stack_79[count], hud_addr, unk3)) ////if ((unsigned __int8)sub_14196F520(eah1->field_0[(signed int)count + 79], hud_addr, unk3))
					return 0; //return 0i64;
			error_descr_local = EA_ERROR_Get_description(ORIGIN_ERROR_ALREADY_EXISTS);
			error_id_local = ORIGIN_ERROR_ALREADY_EXISTS;
			error_string_position = 84;
		}
		else
		{
			error_descr_local = EA_ERROR_Get_description(ORIGIN_ERROR_INVALID_ARGUMENT);
			error_id_local = 88;
			error_string_position = ORIGIN_ERROR_INVALID_ARGUMENT;
		}
		return (FARPROC)EA_ERROR_out(error_id_local, error_descr_local, "E:\ebisu\hudson\workspace\SDK-Multi-Dist\c0b625bc\origin\sdk\9.10", error_string_position);
	}
	return 0;
}

