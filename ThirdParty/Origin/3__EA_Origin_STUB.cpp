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

//3rd pary code
//EA Origin STUB main code (RING 1)
//******************************************//
//ГЛАВНЫЕ ПРОЦЕДУРЫ EA STUB (1 КОЛЬЦО)
//******************************************//

//ДЕФОЛТНЫЕ CONST
 char aEA_GAME_LOCALE[] = "EA_GAME_LOCALE";
//языки
 char  LAN_EN[LAN_LEN] = "en_US";
 char LAN_DE[LAN_LEN] = "de_DE";
 char LAN_ES[LAN_LEN] = "es_ES";
 char LAN_FR[LAN_LEN] = "fr_FR";
 char LAN_IT[LAN_LEN] = "it_IT";
 char LAN_JP[LAN_LEN] = "ja_JP";
 char* pLang_ptr[100] = { &LAN_EN[0], &LAN_DE[0], &LAN_ES[0], &LAN_FR[0], &LAN_IT[0], &LAN_JP[0], 0 };
 

 char aCONTENT_ID_UNRAVEL[] = CONTENT_ID_UNRAVEL;
 char aCONTENT_NAME_UNRAVEL[] = CONTENT_NAME_UNRAVEL;

 char aContentid[] = "ContentId";
 char aEaconnectionid[] = "EAConnectionId";

 //********* установка по глобальным переменным *********
 char* pEA_GAME_LOCALE_Lang = NULL;
 size_t pEA_GAME_LOCALE_Lang_array_id = 0; //en_US
 //EA_CALLBACK_DEFINITION func_EA_Callback;
 __int64 EA_score; //dword_1408BFD8C - целый массив!
 __int64 EA_callback_arg1; //коллбек
 pEA_HANDLE EA_Origin_Handle; //Origin HANDLE
 signed long EA_Origin_SDK_Status;
 BYTE EA_Interlocked_byte; //привязан к EA_Interlocked_Exchange_Add
 long EA_error_execute_subs; //0x0000001408FACC8 - обработчик ошибок EA
 pErrorOutFunc EA_error_execute_subs_alternative_out; //00000001408FACB0 - обработчик ошибок EA : альтернативный вывод информации об ошибке
 byte EA_hud_init_state; //byte_141779266 = -1;
 byte EA_callback_call_state; //byte_140A9F611

__int64 __fastcall ORIGIN_INIT(__in char* arg_pLang)
{
	char* pEAGame_locale = getenv(&aEA_GAME_LOCALE[0]); //получение EA_GAME_LOCALE

	if (!pEAGame_locale)
	{
		//устанавливаем вручную язык из агрументов
		char* fpLang = pLang_ptr[0];
		fpLang--;
		do
		{
			fpLang++;
			if (strcmp(fpLang, arg_pLang))
				continue;

			pEA_GAME_LOCALE_Lang = fpLang;
			break;
		} while (*fpLang != NULL);
		EA_out_info("language %d %s\n", pEAGame_locale, 0);
	}

	EA_ACCESS_request ea_init_req;
	ea_init_req.ContentId = &aCONTENT_ID_UNRAVEL[0];
	ea_init_req.MultiplayerId = &aCONTENT_ID_UNRAVEL[0];
	ea_init_req.Title = &aCONTENT_NAME_UNRAVEL[0];
	ea_init_req.Language = pLang_ptr[pEA_GAME_LOCALE_Lang_array_id];

	EA_Statrup_lp ea_out_answer; 
	//стартап origin
	EA_OriginStartup(0, 0, &ea_init_req, &ea_out_answer);
}

//>>>>>>>> 1 КОЛЬЦО <<<<<<<<<//
bool EA_OriginStartup(__in size_t a1, __in size_t a2, __in pEA_ACCESS_request pEA_access_req_struct, __out pEA_Statrup_lp pEA_Startup_lp_struct)
{
	EA_ACCESS_request ea_init_req_local;
	pEA_HANDLE eah_local = 0;

	//коллбек
	EA_CALL_callback(EA_DEF_ERROR_SCORE, "OriginDestroyHandle entered");

	//проверка EA хендла
	if (!EA_is_eah_init() || _bittest((const LONG*)&a1, 0xAu))
	{
		memcpy(&ea_init_req_local, pEA_access_req_struct, sizeof(EA_ACCESS_request));

		//eviroment переменные (если имеются)
		char* pContentId = getenv(aContentid);
		char* pEaconnectionid = getenv(aEaconnectionid);

		if ((pContentId || pEaconnectionid) && !_bittest((const LONG*)&a1, 0xBu))
		{
			ea_init_req_local.ContentId = pContentId;
			if (pEaconnectionid)
				ea_init_req_local.ContentId = pEaconnectionid;
		}
		//создание eah (EA handle) хендла, установление первичного доверенного соединения с EA Core, отправка ID игрушки, получение информации о сервере, заполнение данных в eah
		eah_local = EA_GET_GAME_INFO(0, 0, pEA_access_req_struct);

		if (eah_local) //если возвратилась ошибка
		{
			//берем описание ошибки
			char* error_description = EA_ERROR_Get_description((EA_ERROR_ID_def)eah_local);
			//отправляем информацию пользователю (первый аргумент == 4, то после OutputDebugString будет сгенерирована точка останова)
			EA_ERROR_out(enERROR_set_skip_execution, def_str_OriginError_printf, "failed to get game info", error_description);
			return false;
		}

		//регистрация hud callback в стеке ввызовов hud'a. указываем индекс и адрес callback'a + ещё аргумент
		size_t ea_ud_ret = EA_REGISTER_HUD_CALLBACK(0, &func_EA_Callback, 0);

		if (ea_ud_ret) //если возвратилась ошибка
		{
			//берем описание ошибки
			char* error_description = EA_ERROR_Get_description((EA_ERROR_ID_def)ea_ud_ret);
			//отправляем информацию пользователю (первый аргумент == 4, то после OutputDebugString будет сгенерирована точка останова)
			EA_ERROR_out(enERROR_set_skip_execution, def_str_OriginError_printf, "failed to register hud callback", error_description);
			return false;
		}

#ifdef DIRTY_SOCK_DLL_AVIABLE

#endif // DIRTY_SOCK_DLL_AVIABLE
	}
}

//>>>>>>>> 1 КОЛЬЦО <<<<<<<<< 1 ступень//
pEA_HANDLE EA_GET_GAME_INFO(__in size_t a1, __in size_t a2, __in pEA_ACCESS_request pEA_access_req_struct) //000000014195C310
{
	if (EA_Origin_SDK_Status == 0x10)
		return (pEA_HANDLE)ORIGIN_ERROR_SDK_NOT_INITIALIZED;
	else
	{
		//создание EA хендла
		EA_Origin_Handle = (pEA_HANDLE)malloc(sizeof(EA_HANDLE));
		if (EA_Origin_Handle)
			EA_Origin_Handle = EA_Init_HANDLE_Fill_HANHDLE_structure(EA_Origin_Handle);
		EA_Interlocked_Exchange_Add(EA_Origin_Handle);
		//попытка создать соединение с Origin_CORE
		EA_probe(EA_Origin_Handle, 0, 0, pEA_access_req_struct); //	result = sub_14195EB30(init_success, a1, a2, pEA_access_req_struct);
	}
//	return result;
}

//>>>>>>>> 1 КОЛЬЦО <<<<<<<<< 2 ступень//
size_t __fastcall EA_REGISTER_HUD_CALLBACK(unsigned int count, EA_CALLBACK_DEFINITION hud_addr, FARPROC unk3)
{
	EA_CALL_callback(EA_DEF_ERROR_SCORE, "OriginRegisterEventCallback entered");

	//проверяем инициализацию eah (хендла EA)
	if (EA_is_eah_init())
	{
		size_t v6 = __readeflags();
		EA_hud_init_state = -1;
		__writeeflags(v6);

		//извлекаем хендл EA, вызываем функцию регистрации
		pEA_HANDLE eah_local = EA_get_eah();
		EA_HOLD_ON_HUD(eah_local, count, (FARPROC)hud_addr, unk3);
	}
}

LONG func_EA_Callback(size_t a1, size_t a2, unsigned long* a3)
{
	char v3; // al

	if (*a3 == 1)
	{
		EA_callback_call_state = 1;
	}
	else
	{
		v3 = EA_callback_call_state;
		if (!*a3)
			v3 = 0;
		EA_callback_call_state = v3;
	}
	//if (qword_140A78890)
		//sub_1416D2DC0(qword_140A78890);
	return 0;
}