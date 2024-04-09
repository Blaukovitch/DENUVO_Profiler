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
//#include "3__EA_Origin_errors_stack.h"

//3rd pary code
//main SDK
#include "3__EA_Origin_errors_stack.h"
#include "3__EA_Origin_HANDLE.h"
#include "3__EA_Origin_Message_class.h"
#include "3__EA_Origin_Message_class_MESSAGE_IDs.h"
//crypto include
#include "../../CRYPTO/MD5/md5.h"
#include "../../CRYPTO/EA_AES/EA_AES.h"
#include <psapi.h>


// >>>>>>> import ___ dirtysock.dll <<<<<<<< //
#ifdef DIRTY_SOCK_DLL_AVIABLE
typedef LONG(NTAPI* DIRTY_SOCK_DLL_NetConnStartup)(__in char* pServerName);  //NetConnStartup
typedef size_t(NTAPI* DIRTY_SOCK_NetConnShutdown)(__in size_t exit_code);
typedef size_t(NTAPI* DIRTY_SOCK_NetConnIdle)(void);
typedef size_t(NTAPI* DIRTY_SOCK_NetConnDisconnect)(void);
typedef size_t(NTAPI* DIRTY_SOCK_ds_timeinsecs)(void);
#endif // DIRTY_SOCK_DLL_AVIABLE
// ____________ dirtysock.dll _______________ //

#pragma pack(1)


//EA ORIGIN STUB ACCESS
#define EA_GAME_LOCALE EAGameLocale
extern char aEA_GAME_LOCALE[];
//языки
#define LAN_LEN 6
extern char  LAN_EN[LAN_LEN];
extern char LAN_DE[LAN_LEN];
extern char LAN_ES[LAN_LEN];
extern char LAN_FR[LAN_LEN];
extern char LAN_IT[LAN_LEN];
extern char LAN_JP[LAN_LEN];
extern char* pLang_ptr[100]; //ссылка на языки

extern char aCONTENT_ID_UNRAVEL[];
extern char aCONTENT_NAME_UNRAVEL[];

extern char aContentid[];
extern char aEaconnectionid[];

typedef struct struct_EA_ACCESS_request
{
	char* ContentId;
	char* Title;
	char* MultiplayerId;
	char* Language;
}EA_ACCESS_request, *pEA_ACCESS_request; //идентефикационная карточка приложения

typedef struct struct_EA_Statrup_lp
{
	PVOID lp1;
	PVOID lp2;
	PVOID lp3;
	PVOID lp4;
}EA_Statrup_lp, *pEA_Statrup_lp; //набор указателей от EA_OriginStartup



#define CONTENT_ID_UNRAVEL "1031469"
#define CONTENT_NAME_UNRAVEL "Unravel"

//ORIGIN ERRORs
#define ORIGIN_ERROR_SDK_NOT_INITIALIZED 0xA100000F
#define ORIGIN_ERROR_SDK_NOT_INITIALIZED_string "The Origin SDK was not running"

//----- CONST DEFINITIONS
#define EA_probe_const1 3216
#define EA_CORE_connection_try_limit 30 //количество попыток соединения с Origin CORE
#define EA_RECV_ANSWER_BUFF_SIZE 0x20000 //начальный буфер под сообщения от Origin Core
#define EA_YOU_SDK_VERSION 0x9A00000 //версия Origin SDK (STUB) для текущего приложения


//----- STRING DEFINITIONS deffs
#define defws_ORIGIN_CLIENT_BRANCH L"SOFTWARE\\Wow6432Node\\Origin" //ветка origin в реестре
#define defws_ORIGIN_CLIENT_PATH L"ClientPath" //имя ключа установленного пути
#define defs_localhost "127.0.0.1" //cp
#define defs_EA_YOU_SDK_VERSION "9.10.1.7" //версия Origin SDK (STUB) для текущего приложения

//error score def
#define EA_DEF_ERROR_SCORE 0x3000000

//----- codedefs
#define codedef_delete_critical_and_semaphore(x_critical, x_semaphore_handle) 	EnterCriticalSection(&(x_critical)); LeaveCriticalSection(&(x_critical)); DeleteCriticalSection(&(x_critical)); CloseHandle((x_semaphore_handle));

// ******** EA	G L O B A L S *************** //
extern char* pEA_GAME_LOCALE_Lang; //установленный язык (текст) - берется из _getenv(EAGameLocale)
extern size_t pEA_GAME_LOCALE_Lang_array_id; //язык игры?(порядковый элемент массива pLang_ptr) - вшит дефолтно?
typedef LONG(NTAPI* EA_CALLBACK_DEFINITION)(size_t a1, size_t a2, unsigned long* a3);
extern __int64 EA_score; //dword_1408BFD8C - целый массив!
extern __int64 EA_callback_arg1; //0000000141344738
extern pEA_HANDLE EA_Origin_Handle; //[0x0000000141344680
extern signed long EA_Origin_SDK_Status; //0000000141344688
extern BYTE EA_Interlocked_byte; //привязан к EA_Interlocked_Exchange_Add
extern long EA_error_execute_subs; //0x0000001408FACC8 - обработчик ошибок EA
typedef void(*pErrorOutFunc)(size_t a1, int byte_break, const CHAR *out_string); // ФУНКЦИЯ ВЫВОДА ОШИБОК
extern pErrorOutFunc EA_error_execute_subs_alternative_out; //00000001408FACB0 - обработчик ошибок EA: альтернативный вывод информации об ошибке
extern byte EA_hud_init_state; //byte_141779266 = -1;
extern byte EA_callback_call_state; //byte_140A9F611

//EA ORIGIN STUB - main
extern __int64 __fastcall ORIGIN_INIT(__in char* arg_pLang); //инициализация
extern void EA_out_info(char* formatstring, ...);//вывод отладочной информации
extern bool EA_OriginStartup(__in size_t a1, __in size_t a2, __in pEA_ACCESS_request pEA_access_req_struct, __out pEA_Statrup_lp pEA_Startup_lp_struct); //первоначальная функция соединения клиента с сервером
extern size_t __fastcall EA_REGISTER_HUD_CALLBACK(unsigned int count, EA_CALLBACK_DEFINITION hud_addr, FARPROC unk3); //регистрация hood
//EA ORIGIN STUB
extern __int64 EA_CALL_callback(__in __int64 score, __in char* note_string); //callback
extern bool EA_is_eah_init(void); //проверка EA_Origin_Handle
extern pEA_HANDLE EA_get_eah(void); //получение EA_Origin_Handle (обычно следует за EA_is_eah_init)
extern pEA_HANDLE EA_GET_GAME_INFO(__in size_t a1, __in size_t a2, __in pEA_ACCESS_request pEA_access_req_struct);//попытка инициализации хендла
extern void EA_Init_HANDLE_FREE_HANHDLE_structure(void); //функция очистки хенждла
void EA_Interlocked_Exchange_Add(__in LPVOID a1); //_InterlockedExchangeAdd //sub_14195BBF0
pS_EA_WSA_ADDON EA_WSA_StartUP(__in __out pS_EA_WSA_ADDON lpWSA_struct);//клиент WSA_Startup
extern pEA_HANDLE EA_Init_HANDLE_Fill_HANHDLE_structure(__in __out pEA_HANDLE pEA_handle);//иниализация хструктуры ендла
extern EA_ERROR_DESCRIPTION_lp_EN_def _fastcall EA_GetErrorDescription(EA_ERROR_ID_def Error_ID); //описатель EA ошибок
extern pEA_HANDLE __fastcall EA_probe(pEA_HANDLE lpEah1, int a2, int a3, __in pEA_ACCESS_request pEA_access_req_struct);//тестовый запуск и инциализация
extern bool __fastcall EA_test_Origin_Client_CORE(__in pEA_HANDLE lpEah1);//проверка существования установочной папки Origin_CORE в реестре
extern __int64 __fastcall EA_complete_Origin_Client_CORE(void); //извлечение install path из реестра, получение имени файла модуля Origin_CORE и поиск его в запущенных процессах. Возврат - PID
extern bool EA_get_core_path(__in WCHAR* lpEACOREPath); //извлечение пути Origin_CORE
extern ULONG __fastcall get_Origin_CORE_PID(const WCHAR *lpFilename, const WCHAR *lpExt);//найти PID, соответсвующий имени файла
extern void __fastcall SwitchThread(__in ULONG* time); //wait-процедура
extern char* get_string(__in char* pDst, __in char* pSrc, __in size_t* lpSet_Dst_strlong); //копирование строки с фиксированной длинной. Расширение строки (указатель), если длина больше предельно допустимой MAX_BUILD_CHARS_with_ZERO_LEN
extern size_t sscanf_ver(__in char* lpEA_VersionString); //получение цифровой hex версии 
extern char* strcpy_CHARS_LEN(__out pS_EA_M_UNIT pDst, __in char* pSrc, __in size_t lpSet_Dst_strlong);//копирование char строк с учетом MAX длины MAX_BUILD_CHARS_LEN

// ------------------- межпоточка

//инициация стека межпоточного взаимодействия
extern pEA_MT_SIGNAL MT_Signal_initial(__in pEA_MT_SIGNAL pSignal_stack_init, __in size_t start_ticks); 
//ожидание получения данных от потока
extern size_t __fastcall MT_waitng_thread_signal(__in pEA_MT_SIGNAL pSignal_stack_init, __in size_t* pWaiting_ticks);
//ожидание отправки сигнала о получении данных
extern void MT_recv_twaiting_CUSTOM(__in pS_EA_WSA_ADDON pWSA_setup);

// ------------------ статические функции класса MessageThread

extern bool __fastcall htons_connect(S_EA_WSA_ADDON* lpfiled_45, S_EA_SEMAPHORE_MUTEX_CRITICAL* lpfiled_31, CRITICAL_SECTION* lpfiled_14, u_short* lpPROBE_105); //соединение host to client
extern _beginthread_proc_type EA_htons_recive(void* pParams);// ПОТОК ПОЛУЧЕНИЯ ДАННЫХ
extern _beginthread_proc_type EA_htons_send_CUSTOM(void* pParams);//ПОТОК ОБРАБОТКИ ДАННЫХ от recive/ОТПРАВКИ ДАННЫХ
extern void WSA_init_recv_stack(__in __out S_EA_WSA_ADDON__RECV_STACK* lpRecive_stack, __in size_t recive_buff_size, __in size_t buf_div); //инициализация стека получения данных
extern bool EA_htons_recive__submit_answer(__in __out pS_EA_WSA_ADDON__RECV_STACK lpRecive_stack, __in size_t recv_ret); //обработчик recv ответов от EA_CORE
extern void __fastcall EA_begin_message_thread(__in pEA_HANDLE hEa); //"OriginSDK message thread". Поток сообщений
extern char** __fastcall EA_initial_Message_threadwork_stack(__in __out pEA_MESSAGE_THREAD_STACK* ppMessage_stack, __in size_t* lpEA_digital_signature); //инициация стека "OriginSDK message thread"
extern pEA_MESSAGE_ANSWER0_EVENT EA_initial_answer_enpointer(__in __out pEA_MESSAGE_ANSWER0_EVENT pMessageAnswerEvent, __in char* lpTypeString, __in char* lpSenderString); //инициализация формы готовых ответов от event ("приглашение")
extern bool EA_Authorize__send_head_request_recipient(__in __out  pEA_HANDLE lpEah1, __in char* lpSeance_key, __in pEA_ACCESS_request pEA_access_req_struct); //формирование ответа на head (приглашение) - авторизавция в Origin CORE.
extern pEA_MESSAGE_SENDER1_HEAD EA_initial_sender_enpointer(__in __out pEA_MESSAGE_SENDER1_HEAD pMessageSenderHeader, __in char* lpTypeString, __in char* lpSenderString); //инициализация формы готовых запросов для своего заголовка header ("ответ на приглашение")
extern bool EA_Authorize__send_head_request_recipient___waiting_send_delivered(__in __out pEA_MESSAGE_SENDER1_HEAD pSend1, __in size_t waiting_time); //отправка сообщения #1 Origin CORE. Формирование XML (LSX)-запроса происходит здесь
extern void __fastcall Set_exchange_flag_state(__in pS_EA_WSA_ADDON pWSA_setup, __in bool state);//установка флага готовности к обмену
extern void __fastcall save_first_two_chars(__in pS_EA_WSA_ADDON pWSA_setup, __in ULONG two_chars);//сохранение первых двух символов из ответа
extern void __fastcall free_id1(__in pEA_MESSAGE_SENDER1_HEAD pMessage_send1);//освобождение памяти 
extern char* recv_buffer_resize(__in pS_EA_WSA_ADDON__RECV_STACK pRecvStack); //рескалинг буфера получения сообщения
extern size_t calc_buff_len(__in pS_EA_WSA_ADDON__RECV_STACK pRecvStack); //вычисление остатка буфера сообщения
extern char* recv_buff_self_pointer(__in pS_EA_WSA_ADDON__RECV_STACK pRecvStack); //следующая свободная область

// --------------------- функции HUD
extern bool insert_into__hud_callback(__in pS_EA_HUD_CALLBACK_INFO pInput_CALLBACK, FARPROC hud_addr, FARPROC unk3); //вставляет данные в позицию HUD CALLBACK
extern FARPROC EA_HOLD_ON_HUD(__in pEA_HANDLE lpEah2, unsigned int count, FARPROC hud_addr, FARPROC unk3);
extern LONG func_EA_Callback(size_t a1, size_t a2, unsigned long* a3);

// --------------------- обработка ошибок
char* EA_ERROR_Get_description(__in EA_ERROR_ID_def ea_error_id); //получить описание ошибки по коду
bool EA_ERROR_out(__in unsigned long bit_lo, char* formatstring, ...); //вывести информацию об ошибке пользователю (по дефолту - отладочный режим и в OutputDebugStringA)

//ext

