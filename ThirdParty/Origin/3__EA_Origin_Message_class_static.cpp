/*
	DENUVO_Profiler v0.3
	--------------------
	CRACKLAB, ELF
	--------------------
	Tiberium reversing
	********************
	2011	-	2024
*/

//класс обработчика сообщений
#include "../../stdafx.h"
#include "3__EA_Origin_Message_class.h"
#include <iostream>
#include <string>

//******************************************//
//СТАТИЧЕСКИЕ ФУНКЦИИ КЛАССА ПО ОБРАБОТКЕ СООБЩЕНИЙ ОТ Origin Core(OriginSDK)
//******************************************//

// ++++++++++++++++++ ПРОЦЕДУРЫ ИНИЦИАЛИЗАЦИИ ДАННЫХ В СТРУКТУРАХ ++++++++++++++++++ //
char** __fastcall EA_initial_Message_threadwork_stack(__in __out pEA_MESSAGE_THREAD_STACK* ppMessage_stack, __in size_t* lpEA_digital_signature)
{
	pEA_MESSAGE_THREAD_STACK pMessage_stack = *ppMessage_stack;
	pEA_MESSAGE_THREAD_STACK alloc_heap_inside = *(pEA_MESSAGE_THREAD_STACK*)pMessage_stack->heap_alloc_2;
	pEA_MESSAGE_THREAD_STACK selected_alloc = pMessage_stack; 
	//парсинг вложений
	while (!alloc_heap_inside->union_state_alloc.lp_alloc2)
	{
		if (alloc_heap_inside->ea_digital_signature >= *lpEA_digital_signature)
		{
			selected_alloc = alloc_heap_inside;
			alloc_heap_inside = *(pEA_MESSAGE_THREAD_STACK*)alloc_heap_inside->heap_alloc_1;
		}
		else
		{
			alloc_heap_inside = *(pEA_MESSAGE_THREAD_STACK*)alloc_heap_inside->heap_alloc_3;
		}
	}

	if (selected_alloc != alloc_heap_inside && *lpEA_digital_signature >= selected_alloc->ea_digital_signature)
		return &selected_alloc->lp_answer;

	pEA_MESSAGE_THREAD_STACK new_inside_instance = (pEA_MESSAGE_THREAD_STACK)malloc(sizeof(S_EA_MESSAGE_THREAD_STACK));

	if (new_inside_instance)
		new_inside_instance->heap_alloc_1 = *ppMessage_stack;


	if (&new_inside_instance->heap_alloc_2)
		new_inside_instance->heap_alloc_2 = *ppMessage_stack;

	if ((ULONG)new_inside_instance != -16)
		new_inside_instance->heap_alloc_3 = *ppMessage_stack;

	new_inside_instance->union_state_alloc.lp_alloc2 = 0;

	if ((ULONG)new_inside_instance != -32)
	{
		new_inside_instance->ea_digital_signature = *lpEA_digital_signature;
		new_inside_instance->lp_answer = 0;
	}
	//sub_141959280(v2, &v12, selected_alloc, v11, new_inside_instance1);

	return &new_inside_instance->lp_answer;
}

pEA_MESSAGE_ANSWER0_EVENT EA_initial_answer_enpointer(__in __out pEA_MESSAGE_ANSWER0_EVENT pMessageAnswerHeader, __in char* lpTypeString, __in char* lpSenderString)
{
	pMessageAnswerHeader->version_len_8 = 0;
	pMessageAnswerHeader->build_len_12 = 0;
	pMessageAnswerHeader->event_len_16 = 0;
	pMessageAnswerHeader->EALS_len_20 = 0;
	pMessageAnswerHeader->reserved1_len_24 = 0;
	pMessageAnswerHeader->digital_signature1_26 = 0;

	pMessageAnswerHeader->max_version_size_5 = MAX_BUILD_CHARS_LEN;
	pMessageAnswerHeader->max_build_size_9 = MAX_BUILD_CHARS_LEN;
	pMessageAnswerHeader->max_Event_size_13 = MAX_BUILD_CHARS_LEN;
	pMessageAnswerHeader->max_EALS_size_17 = MAX_BUILD_CHARS_LEN;
	pMessageAnswerHeader->max_reserved1_size_21 = MAX_BUILD_CHARS_LEN;
	pMessageAnswerHeader->max_reserved2_size_25 = MAX_BUILD_CHARS_LEN;

	
	pMessageAnswerHeader->version_6[0] = '\0';
	pMessageAnswerHeader->build_10[0] = '\0';
	pMessageAnswerHeader->event_14[0] = '\0';
	pMessageAnswerHeader->EALS_18[0] = '\0';
	pMessageAnswerHeader->reserved1_22[0] = '\0';
	pMessageAnswerHeader->reserved1_22[0] = '\0';

		if (!lpTypeString) 
		{
			pMessageAnswerHeader->event_len_16 = strlen(lpTypeString);
			strcpy_s(&pMessageAnswerHeader->event_14[0], MAX_BUILD_CHARS_LEN, lpTypeString);
		}

		if (!lpSenderString)
		{
			pMessageAnswerHeader->EALS_len_20 = strlen(lpSenderString);
			strcpy_s(&pMessageAnswerHeader->EALS_18[0], MAX_BUILD_CHARS_LEN, lpSenderString);
		}
}

pEA_MESSAGE_SENDER1_HEAD EA_initial_sender_enpointer(__in __out pEA_MESSAGE_SENDER1_HEAD pMessageSenderHeader, __in char* lpTypeString, __in char* lpSenderString)
{
	pMessageSenderHeader->ContentId.MAX_strlen = MAX_BUILD_CHARS_LEN;
	pMessageSenderHeader->Title.MAX_strlen = MAX_BUILD_CHARS_LEN;
	pMessageSenderHeader->MultiplayerId.MAX_strlen = MAX_BUILD_CHARS_LEN;
	pMessageSenderHeader->Language.MAX_strlen = MAX_BUILD_CHARS_LEN;
	pMessageSenderHeader->SDK_version.MAX_strlen = MAX_BUILD_CHARS_LEN;
	pMessageSenderHeader->EALS_send.MAX_strlen = MAX_BUILD_CHARS_LEN;
	pMessageSenderHeader->Request_send.MAX_strlen = MAX_BUILD_CHARS_LEN;

	pMessageSenderHeader->ContentId.len = 0;
	pMessageSenderHeader->Title.len = 0;
	pMessageSenderHeader->MultiplayerId.len = 0;
	pMessageSenderHeader->Language.len = 0;
	pMessageSenderHeader->SDK_version.len = 0;

	if (!lpTypeString)
	{
		pMessageSenderHeader->Request_send.len = strlen(lpTypeString);
		strcpy_s(&pMessageSenderHeader->Request_send.string[0], MAX_BUILD_CHARS_LEN, lpTypeString);
	}

	if (!lpSenderString)
	{
		pMessageSenderHeader->EALS_send.len = strlen(lpSenderString);
		strcpy_s(&pMessageSenderHeader->EALS_send.string[0], MAX_BUILD_CHARS_LEN, lpSenderString);
	}
}

// ++++++++++++++++++ СОЕДИНЕНИЕ ПО СОКЕТАМ С Origin CORE ++++++++++++++++++ //
bool __fastcall htons_connect(S_EA_WSA_ADDON* lpfiled_45, S_EA_SEMAPHORE_MUTEX_CRITICAL* lpfiled_31, CRITICAL_SECTION* lpfiled_14, u_short* lpPROBE_105)
{
	SOCKET v8; // rax
	struct sockaddr_in name; // [rsp+38h] [rbp-30h]
	memset(&name, 0, sizeof(name));

	lpfiled_45->socket_80 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (lpfiled_45->socket_80 == ERROR_INVALID_ANSWER)
		return false;

	name.sin_family = AF_INET;
	*(DWORD*)&name.sin_addr.S_un = inet_addr(defs_localhost);
	name.sin_port = htons(*lpPROBE_105);

	if (connect(lpfiled_45->socket_80, (struct sockaddr *)&name, sizeof(name)))
		return false;



	if (_beginthread((_beginthread_proc_type)EA_htons_recive, 0, (void*)lpfiled_45) == ERROR_INVALID_ANSWER) //EMULATE CREATEthread send/recive !!!
		return false;

#ifdef _EMULATE
	lpfiled_45->recive_thread_ready_72 = 1; //EMULATE !!
#endif // !1
	unsigned long v12 = 1;

	//ожидаем создания потока получения данных от EA_CORE
	while (!lpfiled_45->recive_thread_ready_72)
	{
		v12 = 1;
		SwitchThread(&v12);
	}
	return true;
}

void WSA_init_recv_stack(__in __out S_EA_WSA_ADDON__RECV_STACK* lpRecive_stack, __in size_t recive_buff_size, __in size_t buf_div)//sub_1419A4670((void **)v1 + 12, 0x20000ui64, 0i64);
{

	void *v4; // rax

	lpRecive_stack->buff_size_1 = recive_buff_size;
	lpRecive_stack->control_2 = 0;
	lpRecive_stack->control_3 = 0;
	lpRecive_stack->recv_answer_size_4 = ERROR_INVALID_ANSWER;
	if (!buf_div)
		lpRecive_stack->buf_div_5 = (recive_buff_size >> 4);
	else
		lpRecive_stack->buf_div_5 = buf_div;
	lpRecive_stack->buff_0 = (char*)malloc(recive_buff_size);
}

// ++++++++++++++++++ ПОТОК ПОЛУЧЕНИЯ ДАННЫХ Origin CORE, ПОТОК MessageThread - Origin CORE ++++++++++++++++++ //
//recv
_beginthread_proc_type EA_htons_recive(void* pParams)
{
	S_EA_WSA_ADDON* lpfiled_45 = reinterpret_cast<S_EA_WSA_ADDON*>(pParams);
	char* lpBuffer = lpfiled_45->recive_stack_96.buff_0;
	signed long reciv_buff_len = lpfiled_45->recive_stack_96.buff_size_1;
	size_t recv_ret = 0;
	bool submit_recv_answer_state = false;

	//поток получения данных от EA_CORE создан
	lpfiled_45->recive_thread_ready_72 = true; //ВЗВОДИМ ФЛАГ ГОТОВНОСТИ!

	//открыт ли сокет?
	if (lpfiled_45->socket_80 == ERROR_INVALID_ANSWER)
		return;
	do
	{
		//обновление указателя буфера и размера получаемого сообщения
		EnterCriticalSection(&lpfiled_45->Critical_wsa_recv_16);
		lpBuffer = recv_buffer_resize(&lpfiled_45->recive_stack_96);
		reciv_buff_len = calc_buff_len(&lpfiled_45->recive_stack_96);
		LeaveCriticalSection(&lpfiled_45->Critical_wsa_recv_16);

		if (reciv_buff_len <= 0)
		{
			Sleep(100); //ожидаем
		}
		else
		{
			recv_ret = recv(lpfiled_45->socket_80, lpBuffer, reciv_buff_len, 0); //ПОЛУЧЕНИЕ ДАННЫХ

			 //проверка на ошибки
			if ((recv_ret - 1) > IE_NOPEN)
			{
				//условие завершения сеанса
				if (lpfiled_45->socket_80 != ERROR_INVALID_ANSWER)
				{
					shutdown(lpfiled_45->socket_80, 2);
					closesocket(lpfiled_45->socket_80);
					lpfiled_45->socket_80 = ERROR_INVALID_ANSWER;
				}
			}
			else
			{
				EnterCriticalSection(&lpfiled_45->Critical_wsa_recv_16);

				submit_recv_answer_state = EA_htons_recive__submit_answer(&lpfiled_45->recive_stack_96, recv_ret);//v6 = sub_1419A4770((_QWORD *)v1 + 12, recv_answer);
																												  //v7 = 0;
																												  //if (v6)
																												  //v7 = 1;
				LeaveCriticalSection(&lpfiled_45->Critical_wsa_recv_16);
				//условие продолжения получения данных
				if (!submit_recv_answer_state)
					continue;
			}
		}

		EnterCriticalSection(&lpfiled_45->Critical_wsa_recv_16);
		MT_recv_twaiting_CUSTOM(lpfiled_45);
		LeaveCriticalSection(&lpfiled_45->Critical_wsa_recv_16);
	}while (lpfiled_45->socket_80 != ERROR_INVALID_ANSWER);

	free(lpBuffer);
	//флаг готовности потока сброшен
	lpfiled_45->recive_thread_ready_72 = false;
}

//Message Thread
_beginthread_proc_type EA_htons_send_CUSTOM(void* pParams)
{
	pEA_HANDLE lpEah = reinterpret_cast<pEA_HANDLE>(pParams);
	//флаг обработки входящего сообщения
	bool recv_encrypt_state1 = 0;

	//флаг готовности потока EA_htons_send
	lpEah->Message_Thread_READY_7a = true;
	//если цикл занят, то выходим из потока
	if (lpEah->Message_Thread_waiting_7b)
		{lpEah->Message_Thread_READY_7a = false; return 0;}

	//основной цикл обработки сообщений
	do
	{
		::EnterCriticalSection(&lpEah->Message_Recv_critical_14);
		recv_encrypt_state1 = 0;

	} while (!lpEah->Message_Thread_waiting_7b);

}

// ++++++++++++++++++ ПРОЦЕДУРЫ ОБМЕНА ДАННЫМИ ПОТОКА ПОЛУЧЕНИЯ ДАННЫХ Origin CORE, ПОТОКА MessageThread - Origin CORE ++++++++++++++++++ //
bool EA_htons_recive__submit_answer(__in __out pS_EA_WSA_ADDON__RECV_STACK lpRecive_stack, __in size_t recv_ret)
{
	lpRecive_stack->control_3 += recv_ret;

	if (lpRecive_stack->recv_answer_size_4 != ERROR_INVALID_ANSWER)
		return false;

	register size_t recv_ret_count = lpRecive_stack->control_2;
	if (recv_ret_count >= lpRecive_stack->control_3)
		return false;
	while (*(BYTE *)(*(size_t*)lpRecive_stack + recv_ret_count))
	{
		if (++recv_ret_count >= lpRecive_stack->control_3)
			return false;
	}
	lpRecive_stack->recv_answer_size_4 = recv_ret_count;
	return true;
}


bool EA_Authorize__send_head_request_recipient(__in __out  pEA_HANDLE lpEah1, __in char* lpSeance_key, __in pEA_ACCESS_request pEA_access_req_struct)
{
	//Блок данных MD5
	MD5_CTX md5_hash_context;
	byte Hash[MD5_BYTE_LEN];
	char* lpMD5_Str = new char[MAX_PATH];
	//AES
	AES_CONTEXT aes_ctx;
	AES_CONTEXT aes_ctx_waiting;
	S_EA_M_UNIT AES_out;
	S_EA_M_UNIT AES_out_waiting;
	//SEANCE_KEY
	S_EA_M_UNIT SEANCE_KEY;

	EA_Message_ops ea_initial_caller_class; //в оригинале идёт отдельный класс на send

	//форма заполнения XML-запроса ("ответ на приглашение")
	S_EA_MESSAGE_SENDER1_HEAD EALS_send_request_header;

	//строковые данные
		//std::string str[10];
	

	//* MD5 хеширование нашего ключа (II часть response) *//
	const ULONG  hash_trash = GetTickCount();
	MD5_Init(&md5_hash_context);
	MD5_Update(&md5_hash_context, (const unsigned char*)&hash_trash, sizeof(ULONG)); //000000014195BCDF | call <unravel_dump_scy.MD5_Update\>                              | VMPX_CALL
	MD5_Final(&Hash[0], &md5_hash_context);

	//* MD5 hex в строку (II часть response) *//
	size_t i = MD5_STR_LEN;
	byte* fpHash = &Hash[0];
	char Dest[sizeof(size_t)];
	memset(&Dest[0], 0, sizeof(Dest));
	*lpMD5_Str = '\0'; //нулевое начало для strcat
	do
	{
		sprintf(Dest, SPRINT_F_HEX_TO_STR, *fpHash++);
		if (!Dest[0])
			continue;
		strcat(lpMD5_Str, Dest);
	} while (--i); //fix optimization! порядок цикла (от 16 до 0). Плавающий указатель fpHash

	EALS_send_request_header.Virtual_proc_1 = &ea_initial_caller_class;
	//* инициализация структуры-пула обмена сообщениями. Заполнение формы запроса ChallengeResponse для отправки (send) *//
	EA_initial_sender_enpointer(&EALS_send_request_header, defs_Request_event, defs_ElectronicArts_LosAngeles_watermark);
	//** инициализация структуры-пула обмена сообщениями. Заполнение формы ответа ChallengeAccepted при получении (recv) **//
	EALS_send_request_header.EALS_recv.MAX_strlen = MAX_BUILD_CHARS_LEN;
	EALS_send_request_header.Response_recv.MAX_strlen = MAX_BUILD_CHARS_LEN;
	EALS_send_request_header.response_accepted.MAX_strlen = MAX_BUILD_CHARS_LEN;
	strcpy(&EALS_send_request_header.Response_recv.string[0], defs_Response_event); //Response = ответ
	strcpy(&EALS_send_request_header.EALS_recv.string[0], defs_ElectronicArts_LosAngeles_watermark); //EALS_send_request_header.EALS_recv == EALS_send_request_header.EALS_send
	EALS_send_request_header.var_01 = 0;
	EALS_send_request_header.var_02 = 0;
	EALS_send_request_header.var_03 = 0;
	EALS_send_request_header.var_04 = 0;
	 //** инициализация структуры-пула обмена сообщениями. Заполнение MT-блока **//
	MT_Signal_initial(&EALS_send_request_header.MT_Signal_stack, 0);
	InitializeCriticalSectionAndSpinCount(&EALS_send_request_header.MT_Signal_stack.Crit_section_signal, UCHAR_MAX + 1);

	//* AES. Копирование seance_key *//
	SEANCE_KEY.MAX_strlen = MAX_BUILD_CHARS_LEN;
	AES_out.MAX_strlen = MAX_BUILD_CHARS_LEN;
	AES_out.len =  0;
	if (*lpSeance_key)
		strcpy_CHARS_LEN(&SEANCE_KEY, lpSeance_key, strlen(lpSeance_key));
	//** AES. Генерация последовальности в буфере **//
	RAND_GEN(&aes_ctx, 0);
	//** AES. Формирование запроса (response) с генерацией AES **//
	get_string(&EALS_send_request_header.response[0], AES_crypt(&aes_ctx, &AES_out, &EALS_send_request_header.seance_key), &AES_out.len);
	//*** AES. Освобождение данных после выполнения шифрования ***//
	if (AES_out.len >= MAX_BUILD_CHARS_with_ZERO_LEN)
		free(*(char**)&AES_out.string[0]);
	if (SEANCE_KEY.len >= MAX_BUILD_CHARS_with_ZERO_LEN)
		free(*(char**)&SEANCE_KEY.string[0]);
	AES_out.string[0] = '\0';
	SEANCE_KEY.string[0] = '\0';

	//* Заполнение карточки игры - данные EA_ACCESS_request и версия SDK *//
	strcpy_CHARS_LEN(&EALS_send_request_header.ContentId, pEA_access_req_struct->ContentId, strlen(pEA_access_req_struct->ContentId));
	strcpy_CHARS_LEN(&EALS_send_request_header.Title, pEA_access_req_struct->Title, strlen(pEA_access_req_struct->Title));
	strcpy_CHARS_LEN(&EALS_send_request_header.MultiplayerId, pEA_access_req_struct->MultiplayerId, strlen(pEA_access_req_struct->MultiplayerId));
	strcpy_CHARS_LEN(&EALS_send_request_header.Language, pEA_access_req_struct->Language, strlen(pEA_access_req_struct->Language));
	strcpy_CHARS_LEN(&EALS_send_request_header.SDK_version, defs_EA_YOU_SDK_VERSION, sizeof(defs_EA_YOU_SDK_VERSION));
	
	//* Ответный key от нас = MD5 строка. Запрос к Origin CORE - ответ на приглашение (ID=1) *//
	strcpy_CHARS_LEN(&EALS_send_request_header.seance_key, lpMD5_Str, sizeof(lpMD5_Str));
		if (EA_Authorize__send_head_request_recipient___waiting_send_delivered(&EALS_send_request_header, MAX_WAITING_TIME))
		{
			//* AES №2. Обслуживание пришедшего сообщения (ChallengeAccepted) от Origin CORE*//
			RAND_GEN(&aes_ctx_waiting, 0);
			AES_decrypt(&aes_ctx_waiting, &AES_out_waiting, &EALS_send_request_header.response_accepted);

			//* Установка флага готовновности к обмену - открытый LSA-ответ (не требуется AES_decrypt) *//
			Set_exchange_flag_state(&lpEah1->wsa_stack_45, true);

			//* сохранение первых двух символов из ответа в стеке WSA*//
			char* pResponce_ptr = &EALS_send_request_header.response_accepted.string[0];
			if(EALS_send_request_header.response_accepted.len >= MAX_BUILD_CHARS_with_ZERO_LEN)
				pResponce_ptr = *(char**)&EALS_send_request_header.response_accepted.string[0];
			save_first_two_chars(&lpEah1->wsa_stack_45, *(unsigned short*)(pResponce_ptr));

			//* КЛЮЧЕВОЕ СРАВНЕНИЕ В ПРОЦЕДУРЕ !!!!!!!!! MD5_string - ОТВЕТ ОТ AES №2 !!!!!!!!!!! КЛЮЧЕВОЕ СРАВНЕНИЕ В ПРОЦЕДУРЕ *//
			char* pAESwaiting_ptr = &AES_out_waiting.string[0];
			if (AES_out_waiting.len >= MAX_BUILD_CHARS_with_ZERO_LEN)
				pAESwaiting_ptr = *(char**)&AES_out_waiting.string[0];
			bool result = false;
			if (memcmp(pAESwaiting_ptr, lpMD5_Str, strlen(lpMD5_Str))) // <-- СРАВНЕНИЕ
				result = true;

			//* освобождение памяти, если требуется *//
			if (AES_out_waiting.len >= MAX_BUILD_CHARS_with_ZERO_LEN)
				free(*(char**)&AES_out_waiting.string[0]);

			//*-*//
			free_id1(&EALS_send_request_header);
			free(lpMD5_Str);
			return result;
		}//end if (EA_Authorize__send_head_request_recipient___waiting_send_delivered)

	//*-*//
	free_id1(&EALS_send_request_header);
	free(lpMD5_Str);
	return false;
}

bool EA_Authorize__send_head_request_recipient___waiting_send_delivered(__in __out pEA_MESSAGE_SENDER1_HEAD pSend1, __in size_t waiting_time)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return false;
}

// ++++++++++++++++++ ПРОЦЕДУРЫ ОБСЛУЖИВАНИЯ ПОТОКА ПОЛУЧЕНИЯ ДАННЫХ Origin CORE, ПОТОКА MessageThread - Origin CORE (процедуры вызываются из потоков) ++++++++++++++++++ //
void __fastcall EA_begin_message_thread(__in pEA_HANDLE hEa)
{
	hEa->MessageThread_state_6 = 1;

//	if (_beginthread((_beginthread_proc_type)EA_htons_send, 0, (void*)hEa) == ERROR_INVALID_ANSWER) //EMULATE CREATEthread send/recive !!!
		return;
}


void __fastcall Set_exchange_flag_state(__in pS_EA_WSA_ADDON pWSA_setup, __in bool state)
{
	pWSA_setup->exchange_flag_ready_88 = state;
}

void __fastcall save_first_two_chars(__in pS_EA_WSA_ADDON pWSA_setup, __in ULONG two_chars)
{
	pWSA_setup->first_chars_92 = two_chars; //*(_DWORD *)(a1 + 92) = a2;
}

void __fastcall free_id1(__in pEA_MESSAGE_SENDER1_HEAD pMessage_send1)
{
	EnterCriticalSection(&pMessage_send1->MT_Signal_stack.Crit_section_signal);
		if (pMessage_send1->response_len >= MAX_BUILD_CHARS_with_ZERO_LEN)
			free(*(char**)&pMessage_send1->response);
		if (pMessage_send1->response_accepted.len >= MAX_BUILD_CHARS_with_ZERO_LEN)
			free(*(char**)&pMessage_send1->response_accepted.string[0]);
	LeaveCriticalSection(&pMessage_send1->MT_Signal_stack.Crit_section_signal);
	DeleteCriticalSection(&pMessage_send1->MT_Signal_stack.Crit_section_signal);

	CloseHandle(&pMessage_send1->MT_Signal_stack.hSem);
}

bool get_string_from_recv_buufer(__in pS_EA_WSA_ADDON pWSA_setup, __in pS_EA_M_UNIT pIncoming_message)
{
	//контекст AES для полученного шифротекста от OriginCORE
	AES_CONTEXT aes_incoming;

	if (pWSA_setup->revice_responce_status_10 != ERROR_INVALID_ANSWER)
	{
		::EnterCriticalSection(&pWSA_setup->Critical_wsa_recv_16);

		if (pWSA_setup->exchange_flag_ready_88)
		{
			RAND_GEN(&aes_incoming, pWSA_setup->aes_seed_23);
			recv_buff_self_pointer(&pWSA_setup->recive_stack_96);

		}//end if(pWSA_setup->exchange_flag_ready_88)
	}//end if(pWSA_setup->revice_responce_status_10 != ERROR_INVALID_ANSWER)
}

// ------------- recv buffer handle TYPICAL operations -------------  //
size_t calc_buff_len(__in pS_EA_WSA_ADDON__RECV_STACK pRecvStack)
{
	return (pRecvStack->buff_size_1 - pRecvStack->control_3);
}

char* recv_buff_self_pointer(__in pS_EA_WSA_ADDON__RECV_STACK pRecvStack)
{
	 // v1 = a1[2];
	if (pRecvStack->control_2 == pRecvStack->control_3 || pRecvStack->recv_answer_size_4 == ERROR_INVALID_ANSWER)
		return NULL;
	
	return (pRecvStack->buff_0 + pRecvStack->control_2); //result = (char *)(v1 + *a1);
}

char* recv_buffer_resize(__in pS_EA_WSA_ADDON__RECV_STACK pRecvStack)
{
	///ЕСЛИ СВОБОДНОГО МЕСТА В БУФЕРЕ НЕ ХВАТАЕТ
	if ((pRecvStack->buff_size_1 + pRecvStack->control_2 - pRecvStack->control_3) < pRecvStack->buf_div_5)
	{
		//увеличиваем буфер в 2 раза
		char* resize_buffalloc = (char*)malloc(2 * pRecvStack->buff_size_1);
		memcpy(resize_buffalloc, pRecvStack->buff_0, pRecvStack->buff_size_1);
		pRecvStack->buff_size_1 *= 2;
		free(pRecvStack->buff_0);
		pRecvStack->buff_0 = resize_buffalloc;
	}

	if (!(pRecvStack->control_2 + pRecvStack->buff_size_1 - pRecvStack->control_3))
		return 0;

	//копируем остаток
	if (pRecvStack->control_3 >= pRecvStack->control_2 && pRecvStack->control_2)
	{
		size_t leave_size = pRecvStack->control_3 - pRecvStack->control_2;
		if (leave_size)
			memcpy((void *)*pRecvStack->buff_0, (const void *)(*pRecvStack->buff_0 + pRecvStack->control_2), leave_size);
		pRecvStack->control_3 -= pRecvStack->control_2;
		//ответ получен весь??
		if (pRecvStack->recv_answer_size_4 == ERROR_INVALID_ANSWER)
			pRecvStack->recv_answer_size_4 = ERROR_INVALID_ANSWER;
		else
			pRecvStack->recv_answer_size_4 = pRecvStack->recv_answer_size_4 - pRecvStack->control_2;
		pRecvStack->control_2 = 0;
	}

	//ссылка на новый буфер
	return (char*)(pRecvStack->control_3 + (size_t)pRecvStack->buff_0);
}
