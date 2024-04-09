/*
	DENUVO_Profiler v0.3
	--------------------
	CRACKLAB, ELF
	--------------------
	Tiberium reversing
	********************
	2011	-	2024
*/

#pragma once

//3rd pary code

//ДЕФОЛТНОЕ ОШИБОЧНОЕ ЗНАЧЕНИЕ
#define ERROR_INVALID_ANSWER -1 
//дефолтное время задержки
#define MT_WAITING_DEFAULT_TIME_MSEC 1500

//>>>>>>>>>>>>>>> add-ons <<<<<<<<<<<<<<
//SEMAPHORE
/*__int64 __fastcall sub_1419A5F70(__int64 a1, int a2)
{
  __int64 v2; // rbx

  v2 = a1;
  *(_DWORD *)(a1 + 16) = 0x7FFFFFFF;
  *(_QWORD *)a1 = 0i64;
  *(_QWORD *)(a1 + 8) = 0i64;
  *(_BYTE *)(a1 + 20) = 1;
  *(_DWORD *)(a1 + 16) = 0x7FFFFFFF;
  if ( a2 < 0 )
    a2 = 0;
  *(_BYTE *)(a1 + 20) = 1;
  *(_DWORD *)(a1 + 8) = a2;
  *(_QWORD *)a1 = CreateSemaphoreA(0i64, 0, 0x3FFFFFFF, 0i64);
  return v2;
}*/
//#define MAXLONG32 0x7FFFFFFF
typedef struct EA_SEMAPHORE_ADDON
{
	HANDLE hSemaphore; //*(_QWORD *)a1 = CreateSemaphoreA(0i64, 0, 0x3FFFFFFF, 0i64);
	size_t Initial; //*(_DWORD *)(a1 + 8) = a2; a2 = 0
	size_t Limit; //*(_DWORD *)(a1 + 16) = 0x7FFFFFFF; MAXLONG32 = 0x7FFFFFFF
	bool is_init; //*(_BYTE *)(a1 + 20) = 1; true/false flag
}S_EA_SEMAPHORE, pS_EA_SEMAPHORE;

//MUTEX_SEMAPHORE
/*v5 = (signed __int64)(v1 + 64);
  *(_DWORD *)(v5 + 40) = 0;
  *(_BYTE *)(v5 + 44) = 1;
  *(_QWORD *)v5 = 0i64;
  *(_QWORD *)(v5 + 8) = 0i64;
  *(_QWORD *)(v5 + 16) = 0i64;
  *(_QWORD *)(v5 + 24) = 0i64;
  *(_QWORD *)(v5 + 32) = 0i64;
  mutex((__int64)(v1 + 64), 0i64);*/
typedef struct EA_MUTEX_SEMAPHORE_ADDON
{
	CRITICAL_SECTION Critical_0; //*(_QWORD *)v5 = 0i64; InitializeCriticalSectionAndSpinCount((LPCRITICAL_SECTION)a1, 0x100u);
	bool is_init_flag; //*(_BYTE *)(v5 + 44) = 1;	
}S_EA_MUTEX_SEMAPHORE, *pS_EA_MUTEX_SEMAPHORE;

//SEMAPHORE_MUTEX_CRITICAL_ADDON
typedef struct EA_SEMAPHORE_MUTEX_CRITICAL_ADDON
{
	LPVOID f_1;
	volatile signed __int32 interlocked; //v2 = (volatile signed __int32 *)((char *)lpStruct + 16);//_InterlockedExchange(v2 - 4, 0);
	S_EA_SEMAPHORE Semaphore_1; //semaphore_single((LPVOID)v2, 0i64);
	S_EA_SEMAPHORE Semaphore_2; //semaphore_single(v1 + 40, 0i64);
	CRITICAL_SECTION Critical_1; //InitializeCriticalSectionAndSpinCount((LPCRITICAL_SECTION)a1, 0x100u); || v6 = CreateMutexA(0i64, 0, v5);
}S_EA_SEMAPHORE_MUTEX_CRITICAL, *pS_EA_SEMAPHORE_MUTEX_CRITICAL;

//WSA data
/*_int64 __fastcall WSA_Startup(__int64 a1)
{
  __int64 aa1; // rbx
  struct WSAData WSAData; // [rsp+30h] [rbp-1B8h]

  aa1 = a1;
  *(_QWORD *)a1 = &unk_1406E8C80;
  setnullptr((ULONG *)(a1 + 8));
  InitializeCriticalSectionAndSpinCount((LPCRITICAL_SECTION)(aa1 + 16), 0x100u);
  *(_BYTE *)(aa1 + 72) = 0;
  *(_QWORD *)(aa1 + 80) = -1i64;
  *(_BYTE *)(aa1 + 88) = 0;
  sub_1419A4670((void **)(aa1 + 96), 0x20000ui64, 0i64);
  WSAStartup(0x202u, &WSAData);
  return aa1;
}*/
//буфер входящего сообщения 
typedef struct EA_WSA_ADDON_RECV //LPVOID recv_stack[5]; //sub_1419A4670((void **)(aa1 + 96), 0x20000ui64, 0i64); //дублирование v7 = (_BYTE *)sub_1419A4720(pWSA_setup + 12);
{
	//указатель на буфер
	char* buff_0; //*a1 = malloc_0(buff_size);
	//размер буфера
	signed long buff_size_1; //a1[1] = (void *)buff_size;
	//
	size_t control_2; // a1[2] = 0i64;
	//оставшейся место в буфере
	size_t control_3; //a1[3] = 0i64;
	//размер полученного ответа
	size_t recv_answer_size_4; //a1[4] = (void *)-1i64;
	//делитель буфера
	size_t buf_div_5; //a1[5] = v4;
}S_EA_WSA_ADDON__RECV_STACK, *pS_EA_WSA_ADDON__RECV_STACK;


typedef struct EA_WSA_ADDON
{
	LPVOID lpStruct; //*(_QWORD *)a1 = &unk_1406E8C80;
	
	/* -------------------------------------------------------- */
	/* (reinterpret) ФЛАГИ ПОЛУЧЕНИЯ/ОБРАБОТКИ СООБЩЕНИЯ        */
	/* -------------------------------------------------------- */
	/* кастомная реализация - volatile size_t messagestate_8_8; //setnullptr((ULONG *)(a1 + 8)); */
	// режим ожидания
#define CUSTOM_WAITING_SIGNAL 0
	// сообщение полностью получено
#define CUSTOM_RECV_FINAL CUSTOM_WAITING_SIGNAL+1
	// сообщение полностью обработано XMS-парсером
#define CUSTOM_MESSAGE_PARSE_FINAL CUSTOM_RECV_FINAL+1
	volatile size_t messagestate_8; //setnullptr((ULONG *)(a1 + 8));
	/* -------------------------------------------------------- */
	__int64 revice_responce_status_10; // v1[10] = -1i64;
	//карта полученных сообщений
	
	//критическая секция - получение данных (recv)
	CRITICAL_SECTION Critical_wsa_recv_16;  //InitializeCriticalSectionAndSpinCount((LPCRITICAL_SECTION)(aa1 + 16), 0x100u);
	//SEED для полученного шифротекста от OriginCore
	size_t aes_seed_23; //RAND_GEN((__int64)&v21, *((_DWORD *)v3 + 23)); - процедура "get_string_from_recv_buufer"
	// флаг готовности потока recv
	bool recive_thread_ready_72; //*(_BYTE *)(aa1 + 72) = 0;
	// активный сокет
	SOCKET socket_80; //*(_QWORD *)(aa1 + 80) = -1i64; 
	// флаг готовности к обмену - устаналивается в зависимости от шифрованности сообщения
	byte exchange_flag_ready_88; //filed_88; //*(_BYTE *)(aa1 + 88) = 0;
	ULONG first_chars_92; //НЕ ИНИЦИАЛИЗИРУЕТСЯ! void __fastcall sub_14198A340(__int64 a1, int a2) 	*(_DWORD *)(a1 + 92) = a2;
	// стек полученного сообщения
	S_EA_WSA_ADDON__RECV_STACK recive_stack_96; //LPVOID recv_stack[5]; //sub_1419A4670((void **)(aa1 + 96), 0x20000ui64, 0i64);
}S_EA_WSA_ADDON, *pS_EA_WSA_ADDON;

typedef struct EA_MESSAGE_THREAD_STACK ////_RBX->field_0[4] = (__int64)alloc_0x30;
{
	//указатель #1
	LPVOID heap_alloc_1;
	//указатель #2
	LPVOID heap_alloc_2;
	//указатель #3
	LPVOID heap_alloc_3;
	union a
		{
		BYTE lp_alloc2;
		BYTE lp_alloc3;
	}union_state_alloc;
	//цифровая сигнатура
	size_t ea_digital_signature;
	//указатель на буфер с ответом
	char* lp_answer;
}S_EA_MESSAGE_THREAD_STACK, *pEA_MESSAGE_THREAD_STACK;


//HUD struct
typedef struct EA_HUD_CALLBACK
{
	FARPROC addr1[5];//блок данных
	FARPROC hud_addres_local_40; //2й аргумент адрес устанавливаемого HUD
	FARPROC hud_unk3_48; //3й аргумент
	CRITICAL_SECTION hud_crit_section; //критическая секция HUD блока
}S_EA_HUD_CALLBACK_INFO,*pS_EA_HUD_CALLBACK_INFO;

// ********************************** HANDLE MAIN ****************************************************************//
//#pragma pack(1)
typedef struct EA_HANDLE_MAIN_STACK
{
	//--- COMMON ---
	FARPROC free_EA_proc; 						//ФУНКЦИЯ ОСВОБОЖДЕНИЯ ПАМЯТИ ХЕНДЛА //pHandle->field_0[0] = (__int64)&EA_Init_HANDLE_FREE_HANHDLE_structure;
	volatile unsigned long Exchange_volatile;	// _InterlockedExchange((volatile signed __int32 *)&pHandle->field_0[1], 0);
	ULONG filed_1; 								//v2 = LODWORD(pHandle->field_0[1]);
	LPVOID lpStructure_0x40;		//filed_2   		// pHandle->field_0[2] = 0i64; //_RBX->field_0[2] = (__int64)alloc_0x40;
	size_t filed_3; 				//pHandle->field_0[3] = 0i64;
	
	//СТЕК ПЕРЕДАЧИ MessageThread
	// 
	pEA_MESSAGE_THREAD_STACK lpMessage_thread_stack_4; 	 //field_4		//_RBX->field_0[4] = (__int64)alloc_0x30;
	size_t filed_5;					//_RBX->field_0[5] = 0i64;
	ULONG MessageThread_state_6;	//setnullptr((ULONG *)&_RBX->field_0[6]);
	//флаг готовности потока обработки LSX-сообщений
	BYTE Message_Thread_READY_7a;		//LOBYTE(_RBX->field_0[7]) = 0;
	//флаг цикла обработки LSX-сообщений 
	bool Message_Thread_waiting_7b;	//BYTE1(_RBX->field_0[7]) = 0;

	//--- MT ---
	// описание: 
	
	CRITICAL_SECTION Message_Thread_critical_8;  	//sub_1419A5ED0((LPCRITICAL_SECTION)&_RBX->field_0[8], 0i64, 1);
	CRITICAL_SECTION Message_Recv_critical_14;	 //sub_1419A5ED0((LPCRITICAL_SECTION)&_RBX->field_0[14], 0i64, 1);
	CRITICAL_SECTION Critical_20; 	//sub_1419A5ED0((LPCRITICAL_SECTION)&_RBX->field_0[20], 0i64, 1); 
	S_EA_SEMAPHORE Semaphore_26; 	//sub_1419A5F70((__int64)&_RBX->field_0[26], 0); SEMAPHORE
	size_t filed_29; 				//_RBX->field_0[29] = 0i64;
	ULONG filed_30;					//setnullptr((ULONG *)&_RBX->field_0[30]);
	S_EA_SEMAPHORE_MUTEX_CRITICAL Semaphore_mutex_31; //  LOBYTE(v5) = 1;  sub_1419A5DC0(&_RBX->field_0[31], 0i64, v5);

	//--- WSA ---
	// описание: служебные данные сетевого соденинения WSA
	
	S_EA_WSA_ADDON wsa_stack_45; 	//WSA_Startup((__int64)&_RBX->field_0[45]);

	//--- incoming messages stack ---
	// описание: стек входящих сообщений, которые были обработаны
	LPVOID filed_63__array_4[4]; 	//v6 = &_RBX->field_0[63];   alloc_0x10 = malloc_0(0x10ui64); *v6 = 0i64; v6[1] = 0i64; v6[2] = 0i64; v6[3] = 0i64; v6[4] = 0i64;
	LPVOID filed_68__array_4[4];    //v8 = &_RBX->field_0[68];  *v8 = (__int64)alloc_0x10_1; *v8 = 0i64; v8[1] = 0i64; v8[2] = 0i64; v8[3] = 0i64; v8[4] = 0i64;
	LPVOID filed_73__array_4[4]; 	//v10 = &_RBX->field_0[73];   *v10 = 0i64; v10[1] = 0i64; v10[2] = 0i64; v10[3] = 0i64; v10[4] = 0i64; alloc_0x10_2 = malloc_0(0x10ui64); *v10 = (__int64)alloc_0x10_2;

	//--- endl numeric ---

	//--- ID отправленных сообщений пWSA_Startup ---//
	// описание: ориентир по отправке запросов в OriginCore
	
	size_t send_MESSAGE_ID_inc_78; 	//_RAX = 1i64; __asm { xchg    rax, [rbx+270h] }
	
	//--- стек регистрации HUD callback ---//
	// описание: процедуры, которые необходимо вызывать при возникновении event'ов
	
	#define const_HUD_CALLBACK_STACK_SIZE 26
	pS_EA_HUD_CALLBACK_INFO HUD_CALLBACKS_stack_79[const_HUD_CALLBACK_STACK_SIZE];   //memset(&_RBX->field_0[79], 0, 0xD0ui64); 0xD0 = 208/sizef(QWORD) = 26
	
	//Probe
	size_t PROBE_105;				//.........LODWORD(eah1->field_0[105]) = a2;
	
	//обьявления аналогично pEA_MESSAGE_ANSWER_HEAD
	char ORIGIN_CORE_VER_106[16]; 	//LOBYTE(_RBX->field_0[106]) = 0; //size_t filed_107; 				//!NOT INIT!
	
	//Версия установленного OriginCore
	size_t origin_core_ver_len_108; //_RBX->field_0[108] = 0i64;
	size_t max_origin_core_ver_len_109;				//_RBX->field_0[109] = 15i64;
	//game_id - идентефикатор игры
	char game_id[16]; 				//LOBYTE(_RBX->field_0[110]) = 0; //size_t filed_111; 				//!NOT INIT!
	size_t game_id_len_112; 				//_RBX->field_0[112] = 0i64;
	size_t max_game_id_len_113; 				//_RBX->field_0[113] = 15i64;
	
	size_t filed_114; 				//_RBX->field_0[114] = 0i64;
	size_t filed_115;				//_RBX->field_0[115] = 0i64;
	size_t filed_116; 				//_RBX->field_0[116] = 0i64;
	size_t filed_117;				//_RBX->field_0[117] = 0i64;
	size_t filed_118;				//_RBX->field_0[118] = 0i64;
}EA_HANDLE, *pEA_HANDLE;

