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
//******************************************//
//ID (ФОРМЫ) сообщений ОБРАБОТЧИКА/ОТПРАВЛЕНИЯ СООБЩЕНИЙ ОТ Origin CORE
//******************************************//

#include "3__EA_Origin_Message_class.h"
#include "3__EA_Origin_STUB.h"

#define defs_ElectronicArts_LosAngeles_watermark "EALS" //ID поставщика - Eletronic Arts Los Angeles
#define defs_Message_event "Event" //Message 'Event'
#define defs_Request_event "Request" //Message 'Request'
#define defs_Response_event "Response" //Message 'Response'

#define EA_CRYPTO_SIGNATURE_1 0x34123549 //сигнатура XOR №1
#define EA_CRYPTO_SIGNATURE_2 0x5BD1E995 //сигнатура XOR №2

#define DENUVO_HIDDEN_CONST_1 0x5A2566BE //Скрытая константа 1

#define MD5_BYTE_LEN 32 //длина строки MD5
#define MD5_STR_LEN MD5_BYTE_LEN/2 //длина строки MD5

#define SPRINT_F_HEX_TO_STR "%02x"

#define MAX_BUILD_CHARS_LEN 15 //допустимая длина строки, при размещении в текущем блоке памяти БЕЗ выделения дополнительной
#define MAX_BUILD_CHARS_with_ZERO_LEN MAX_BUILD_CHARS_LEN+sizeof('\0')

#define MAX_WAITING_TIME 15000u

/*

(ID)___(EALS)-----------------------------------------------------------------__---------------------------------------------------------------(Origin CORE)
        wsapi.connect													      >> 
									                                          << [Event sender] 'приглашение' (сеансовый ключ, версия Origin CORE, билд)
01     [ChallengeResponse]ответ на приглашение(response,IDs, язык, ver.SDK *) >>
01                                                                            << [ChallengeAccepted] принятие ответа (response)
------------------------------------------------------------------------------__---------------------------------------------------------------------
	* Origin CORE != версия SDK
*/

union ucount
{
	volatile unsigned __int32 HI;
	volatile unsigned __int32 LO;
};

typedef struct struct_EA_MT_SIGNAL
{
	/*
	hSem            dq ? ; offset
	00000008 count_HI        dd ?
	0000000C count_LO        dd ?
	00000010 tick_byte1      db ?
	00000011 -      db ?
	00000012 field_12        db ?
	00000013 field_13        db ?
	00000014 initial         db ?
	00000015 -        db ?
	00000016 -        db ?
	00000017 -        db ?
	00000018 CRITICAL        CRITICAL_SECTION ?
	00000040 sema            ends
	*/

	//хендл семафора
	HANDLE hSem;
	//количество обращений?
	ucount count;
	//время ожидания
	size_t ticks;
	//флаг инициализации
	bool initial;
	//критическая секция
	CRITICAL_SECTION Crit_section_signal;
}EA_MT_SIGNAL, *pEA_MT_SIGNAL; //структура межпоточного взаимодействия

typedef struct EA_MESSAGE_UNIT_FULL
{
	size_t MAX_strlen; //максимально-допустимая длина сообщения для размещения в структуре MAX_BUILD_CHARS_LEN (иначе-выделение памяти malloc)
	char string[MAX_BUILD_CHARS_with_ZERO_LEN]; //строка
	size_t len; //длина строка (strlen)
}S_EA_M_UNIT, *pS_EA_M_UNIT;

//std::string прототип
//ОТВЕТ №1 (ПРИГЛАШЕНИЕ)
typedef struct EA_MESSAGE_ANSWER0_EVENT_HEAD_POOL
{
	LPCVOID Virtual_proc_1; //стек вируатльных функций EA_Message_ops

								        //стек сообщения	<< (входящее)//
	char seance_key_2[MAX_BUILD_CHARS_with_ZERO_LEN]; //ссылка на содержимое контейнера "key" (входящие данные head - сеансовый ключ от Origin CORE)
	//LPVOID heap_alloc_3; //ссылка на данные (EA_Probe)
	size_t key_len_4; //длина полученного char* key_1;

	size_t max_version_size_5; //максимальный размер данных под "version" = MAX_BUILD_CHARS_LEN
	char version_6[MAX_BUILD_CHARS_with_ZERO_LEN]; //содержимое контейнера "version" (входящие данные head - версия Origin CORE)
	size_t version_len_8; //длина полученного char version_6[16]

	size_t max_build_size_9; //максимальный размер данных под "build" = MAX_BUILD_CHARS_LEN
	char build_10[MAX_BUILD_CHARS_with_ZERO_LEN]; //содержимое контейнера "build" (входящие данные head - билд Origin CORE)
	size_t build_len_12; //длина полученного char build_9[16]

	size_t max_Event_size_13; //максимальный размер данных под "Event" = MAX_BUILD_CHARS_LEN
	char event_14[MAX_BUILD_CHARS_with_ZERO_LEN]; //XML-tag Event
	size_t event_len_16; //длина полученного char event_12[16]. По дефолту статичен

	size_t max_EALS_size_17; //максимальный размер данных под поставщика "EALS" = MAX_BUILD_CHARS_LEN
	char EALS_18[MAX_BUILD_CHARS_with_ZERO_LEN]; //содержимое поставщика EALS
	size_t EALS_len_20; //длина полученного char EALS_15[16]. По дефолту статичен

	size_t max_reserved1_size_21; //максимальный размер данных reserved1
	char reserved1_22[MAX_BUILD_CHARS_with_ZERO_LEN]; //содержимое reserved1
	size_t reserved1_len_24; //длина полученного char reserved1[16]. По дефолту статичен

	size_t max_reserved2_size_25; //максимальный размер данных reserved2
	size_t digital_signature1_26;//поле 26
	size_t watermark_digital_signature_27;//цифровая сигнатура поставщика (EALS)
	size_t digital_signature2_28;//поле 28

								 //>>>> межпоточное взаимодействие <<<<//
	EA_MT_SIGNAL MT_Signal_stack; //стек сигнальщика

}S_EA_MESSAGE_ANSWER0_EVENT, *pEA_MESSAGE_ANSWER0_EVENT; //пул под заголовочное сообщение (самое первое от connect)

//ЗАПРОС №1 (ОТВЕТ НА ПРИГЛАШЕНИЕ)
typedef struct EA_MESSAGE_SEND1_HEAD_POOL
{
	LPCVOID Virtual_proc_1; //стек вируатльных функций EA_Message_ops
	
								   //стек сообщения	multi >> (исходящее) и << (входящее)//
	//исходящее >>
		char response[MAX_BUILD_CHARS_with_ZERO_LEN]; //ссылка на содержимое контейнера "response" (отправляемый запрос в Origin CORE). ChallengeResponse
		size_t response_len; //длина response;

		S_EA_M_UNIT seance_key; //ссылка на содержимое контейнера "key" (входящие данные head - сеансовый ключ от Origin CORE)

		//данные от EA_ACCESS_request
		S_EA_M_UNIT ContentId; //ContentId (ex: 1031469)
		S_EA_M_UNIT Title; //Title (ex: Unravel)
		S_EA_M_UNIT MultiplayerId; //MultiplayerId (ex: 1031469)
		S_EA_M_UNIT Language; //Language (ex: en_US)

		S_EA_M_UNIT SDK_version; //версия SDK (9.10.1.7)

		//данные отправителя
		S_EA_M_UNIT Request_send; //Тип сообщения - запрос ("Request")
		S_EA_M_UNIT EALS_send; //Тип сообщения - поставщик ("EALS")

	//входящее <<
		S_EA_M_UNIT response_accepted; // принятый запрос "response". ChallengeAccepted
		S_EA_M_UNIT Response_recv; //Тип сообщения - ответ ("Response")
		S_EA_M_UNIT EALS_recv; //Тип сообщения - поставщик ("EALS")
		S_EA_M_UNIT reserved1; //reserved?

								 //>>>> межпоточное взаимодействие <<<<//
		EA_MT_SIGNAL MT_Signal_stack;
	
								  //>>>> СТЕК ПЕРЕМЕННЫХ <<<<//
	bool is_Challenge_accepted; //флаг получения ответа от Origin CORE
	size_t waiting_time; //время ожидания ответа от MessageThread
	pEA_HANDLE eah0; //lp EA_HANDLE_MAIN_STACK
	size_t var_01; //?
	size_t var_02; //?
	size_t var_03; //?
	size_t var_04; //?

}S_EA_MESSAGE_SENDER1_HEAD, *pEA_MESSAGE_SENDER1_HEAD; //ОТВЕТ НА ПРИГЛАШЕНИЕ(самое первое от connect)