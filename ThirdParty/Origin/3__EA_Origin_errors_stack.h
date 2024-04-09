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
//******************************************//
//			ОШИБКИ КЛИЕНТА (OriginSDK)
//******************************************//

//ORIGIN ERRORs
#define ORIGIN_ERROR_SDK_NOT_INITIALIZED 0xA100000F
#define ORIGIN_ERROR_SDK_NOT_INITIALIZED_description "The Origin SDK was not running"
#define ORIGIN_ERROR_CORE_NOTLOADED 0xA0020000
#define ORIGIN_ERROR_CORE_NOT_INSTALLED 0xA0020008
#define ORIGIN_ERROR_LSX_NO_RESPONSE 0x00000A1080001
#define ORIGIN_ERROR_CORE_INCOMPATIBLE_VERSION  0x00000A0020007
#define ORIGIN_ERROR_ALREADY_EXISTS 0x000000A3000010
#define ORIGIN_ERROR_INVALID_ARGUMENT 0x00000A2000004
#define ORIGIN_ERROR_NOT_FOUND 0x000000A2000008

#define ORIGIN_WARNING_SDK_ALREADY_INITIALIZED 0x42010001

#define EA_ERROR_ID_def size_t
#define EA_ERROR_DESCRIPTION_lp_EN_def char*
#define EA_ERROR_DESCRIPTION_lp_RU_def WCHAR*
#define EA_ERROR_MACRO_lp_EN_def char*

//def returns strings
#define def_str_NoAdditionalInfo "No additional error information available"
#define def_str_OriginError_printf "Origin Error: %s %s"

typedef struct ORIGIN_ERROR_struct
{
	EA_ERROR_ID_def error_id;
	EA_ERROR_MACRO_lp_EN_def ERROR_MACRO_0;
	EA_ERROR_MACRO_lp_EN_def ERROR_MACRO_1;
	EA_ERROR_DESCRIPTION_lp_EN_def ERROR_Description_EN;
	EA_ERROR_DESCRIPTION_lp_RU_def ERROR_Description_RU;
}ORIGIN_ERROR, *pORIGIN_ERROR;

enum ERROR_set_variants
{
	enERROR_set_skip_execution = 1, //дефолтное исполнение
	enERROR_set_two_unk, //?
	enERROR_set_three_unk, //?
	enERROR_set_Break_after_OutputDebugString //точка останова после вызова OutputDebugStringA
};


//--- units --- //
static ORIGIN_ERROR null_pointer00 =
{
	0X0,
	"NULL",
	"NULL",
	def_str_NoAdditionalInfo,
	L"Отсутсвует информация по ошибке"
};

static ORIGIN_ERROR A0 =
{
	0xA0010000,
	"ORIGIN_SUCCESS",
	"ORIGIN_ERROR_AREA_GENERAL",
	"The operation succeeded",
	L"Операция успешно завершена"
};

static ORIGIN_ERROR A1 = 
{
	0xA0010001,
	"ORIGIN_PENDING",
	"",
	"The operation is still waiting to complete",
	L"Операция находится в стадии ожидания"
};

static ORIGIN_ERROR A2 = 
{
	0xA0010002,
	"ORIGIN_ERROR_GENERAL",
	"",
	"An unspecified error has occured",
	L""
};
 
static ORIGIN_ERROR A3 = 
{
	0xA0010003,
	"ORIGIN_ERROR_INVALID_HANDLE",
	"ORIGIN_LEVEL_1",
	"The provided handle is invalid",
	L"Указанный хендл не корректен"
};

static ORIGIN_ERROR A4 = 
{
	0xA0010004,
	"ORIGIN_ERROR_OUT_OF_MEMORY",
	"ORIGIN_LEVEL_0",
	"Failed to allocate memory",
	L"Невозможно выделить память"
};  
   
static ORIGIN_ERROR A5 = 
{
	0xA0010005,
	"ORIGIN_ERROR_NOT_IMPLEMENTED",
	"",
	"The function is not implemented",
	L""
};

static ORIGIN_ERROR A6 = 
{
	0xA0010006,
	"ORIGIN_ERROR_INVALID_USER",
	"ORIGIN_LEVEL_2",
	"The specified user is not valid in this context, or the userId is invalid",
	L""
};

static ORIGIN_ERROR A7 = 
{
	0xA0010007,
	"ORIGIN_ERROR_INVALID_ARGUMENT",
	"",
	"One or more arguments are invalid",
	L""
};
   
static ORIGIN_ERROR A8 = 
{
	0xA0010008,
	"ORIGIN_ERROR_NO_CALLBACK_SPECIFIED",
	"",
	"The asynchronous operation expected a callback, but no callback was specified",
	L""
};

static ORIGIN_ERROR A9 = 
{
	0xA0010009,
	"ORIGIN_ERROR_BUFFER_TOO_SMALL",
	"",
	"The provided buffer doesn't have enough space to contain the requested data",
	L""
};

static ORIGIN_ERROR A10 = 
{
	0xA001000A,
	"ORIGIN_ERROR_TOO_MANY_VALUES_IN_LIST",
	"",
	"We are currently only supporting one item in the list",
	L""
};

static ORIGIN_ERROR A11 = 
{
	0xA0010000B,
	"ORIGIN_ERROR_NOT_FOUND",
	"",
	"The requested item was not found",
	L""
};

static ORIGIN_ERROR A12 = 
{
	0xA0010000C,
	"ORIGIN_ERROR_INVALID_PERSONA",
	"",
	"The specified persona is not valid in this context, or the personaId is invalid",
	L""
};

static ORIGIN_ERROR A13 = 
{
	0xA0010000D,
	"ORIGIN_ERROR_NO_NETWORK",
	"",
	"No internet connection available",
	L"Отсутсвует интернет-соединение"
};
    
static ORIGIN_ERROR A14 = 
{
	0xA0010000E,
	"ORIGIN_ERROR_NO_SERVICE",
	"",
	"Origin services are unavailable",
	L"Службы Origin не доступны"
};
       
static ORIGIN_ERROR A15 = 
{
	0xA0010000F,
	"ORIGIN_ERROR_NOT_LOGGED_IN",
	"",
	"The user isn't logged in. No valid session is available",
	L""
};

static ORIGIN_ERROR A16 =
{
	0xA00100010,
	"ORIGIN_ERROR_MANDATORY_ORIGIN_UPDATE_PENDING",
	"",
	"There is a mandatory update pending for Origin, this will prevent origin from going online",
	L""
};

static ORIGIN_ERROR A17 =
{
	0xA00100011,
	"ORIGIN_ERROR_ACCOUNT_IN_USE",
	"",
	"The account is currently in use by another Origin instance",
	L""
};


static ORIGIN_ERROR A19 =
{
	0xA00100012,
	"ORIGIN_ERROR_TOO_MANY_INSTANCES",
	"",
	"Too many instances of the OriginSDK created",
	L""
};

static ORIGIN_ERROR A20 =
{
	0xA00100013,
	"ORIGIN_ERROR_ALREADY_EXISTS",
	"ORIGIN_LEVEL_3",
	"The item already exists in the list",
	L""
};

static ORIGIN_ERROR A21 =
{
	0xA00100014,
	"ORIGIN_ERROR_INVALID_OPERATION",
	"",
	"The requested operation cannot be performed",
	L""
};

static ORIGIN_ERROR A22 =
{
	0xA00100015,
	"ORIGIN_ERROR_AGE_RESTRICTED",
	"",
	"The item has age restrictions",
	L""
};

static ORIGIN_ERROR A23 =
{
	0xA00100016,
	"ORIGIN_ERROR_BANNED",
	"",
	" The user is banned",
	L""
};

static ORIGIN_ERROR A24 =
{
	0xA00100017,
	"ORIGIN_ERROR_NOT_READY",
	"",
	"The item is not ready",
	L""
};

static ORIGIN_ERROR A25 =
{
	0xA00100018,
	"ORIGIN_ERROR_SDK_NOT_INITIALIZED",
	"",
	"The Origin SDK was not running",
	L""
};

static ORIGIN_ERROR A26 =
{
	0xA00100019,
	"ORIGIN_ERROR_SDK_INVALID_ALLOCATOR_DEALLOCATOR_COMBINATION",
	"",
	"Make sure that you provide both an allocator and a deallocator",
	L""
};

static ORIGIN_ERROR A27 =
{
	0xA00100020,
	"ORIGIN_ERROR_SDK_IS_RUNNING",
	"",
	"The Origin SDK is running. This operation should only be done before the SDK is initialized or after the SDK is shutdown",
	L""
};


static ORIGIN_ERROR A28 =
{
	0xA00100021,
	"ORIGIN_ERROR_SDK_NOT_ALL_RESOURCES_RELEASED",
	"",
	"The game is still holding on to the resource handles. Call #OriginDestroyHandle on resources that are no longer needed",
	L""
};

static ORIGIN_ERROR A29 =
{
	0xA00100000,
	"ORIGIN_ERROR_SDK_INVALID_RESOURCE",
	"",
	"The resource in the resource map is invalid. Memory corruption?",
	L""
};
   
static ORIGIN_ERROR A30 =
{
	0xA00100000,
	"ORIGIN_ERROR_SDK_INTERNAL_ERROR",
	"",
	"The SDK experienced an internal error",
	L""
};

static ORIGIN_ERROR A31 =
{
	0xA00100000,
	"ORIGIN_ERROR_SDK_INTERNAL_BUFFER_TOO_SMALL",
	"",
	"The internal buffer that the SDK is using is not big enough to receive the response. Inform OriginSDK Support",
	L""
};

static ORIGIN_ERROR A32 =
{
	0xA00100000,
	"ORIGIN_WARNING_SDK_ALREADY_INITIALIZED",
	"",
	"The Origin SDK is already initialized",
	L""
};

static ORIGIN_ERROR A33 =
{
	0xA00100000,
	"ORIGIN_WARNING_SDK_STILL_RUNNING",
	"",
	"The Origin SDK is still running",
	L""
};

static ORIGIN_ERROR A34 =
{
	0xA00100000,
	"ORIGIN_WARNING_SDK_ENUMERATOR_IN_USE",
	"",
	"The Enumerator associated with the handle was in use",
	L""
};

static ORIGIN_ERROR A35 =
{
	0xA00100000,
	"ORIGIN_WARNING_SDK_ENUMERATOR_TERMINATED",
	"",
	" The Enumerator associated with the handle was not finished",
	L""
};

static ORIGIN_ERROR A36 =
{
	0xA00100000,
	"ORIGIN_ERROR_CORE_NOTLOADED",
	"",
	"The Origin desktop application is not loaded",
	L""
};

static ORIGIN_ERROR A37 =
{
	0xA00100000,
	"ORIGIN_ERROR_CORE_LOGIN_FAILED",
	"",
	"Origin couldn't authenticate with the Origin Servers",
	L""
};

static ORIGIN_ERROR A38 =
{
	0xA00100000,
	"ORIGIN_ERROR_CORE_AUTHENTICATION_FAILED",
	"",
	"Origin seems to be running, but the LSX Authentication Challenge failed. No communication with Core is possible",
	L""
};
 
 static ORIGIN_ERROR A39 =
{
	0xA00100000,
	"ORIGIN_ERROR_CORE_SEND_FAILED",
	"",
	"Sending data to Origin failed",
	L""
};
 
static ORIGIN_ERROR A40 =
{
	0xA00100000,
	"ORIGIN_ERROR_CORE_RECEIVE_FAILED",
	"",
	"Receiving data from Origin failed",
	L""
};

static ORIGIN_ERROR A41 =
{
	0xA00100000,
	"ORIGIN_ERROR_CORE_RESOURCE_NOT_FOUND",
	"",
	"The requested resource could not be located",
	L""
};

static ORIGIN_ERROR A42 =
{
	0xA00100000,
	"ORIGIN_ERROR_CORE_INCOMPATIBLE_VERSION",
	"",
	"The Origin version is too old to work with this SDK version",
	L""
};

static ORIGIN_ERROR A43 =
{
	0xA00100000,
	"ORIGIN_ERROR_CORE_NOT_INSTALLED",
	"",
	"The Origin installation couldn't be found",
	L""
};

static ORIGIN_ERROR A44 =
{
	0xA00100000,
	"ORIGIN_WARNING_IGO_NOTLOADED",
	"",
	"The IGO could not be loaded, so SDK functionality is degraded",
	L""
};

static ORIGIN_ERROR A45 =
{
	0xA00100000,
	"ORIGIN_WARNING_IGO_SUPPORT_NOTLOADED",
	"",
	"IGO support is not loaded, so SDK functionality is degraded",
	L""
};

static ORIGIN_ERROR A46 =
{
	0xA00100000,
	"ORIGIN_ERROR_IGO_ILLEGAL_ANCHOR_POINT",
	"",
	"The combination of anchor point bits doesn't resolve to a proper dialog anchor point",
	L""
};

static ORIGIN_ERROR A47 =
{
	0xA00100000,
	"ORIGIN_ERROR_IGO_ILLEGAL_DOCK_POINT",
	"",
	"The combination of dock point bits doesn't resolve to a proper dock point",
	L""
};

static ORIGIN_ERROR A48 =
{
	0xA00100000,
	"ORIGIN_ERROR_IGO_NOT_AVAILABLE",
	"",
	"The IGO is not available",
	L""
};

static ORIGIN_ERROR A49 =
{
	0xA00100000,
	"ORIGIN_ERROR_NO_MULTIPLAYER_ID",
	"",
	"It is not possible to set the presence to JOINABLE when no multiplayer Id is defined on the offer",
	L""
};

static ORIGIN_ERROR A50 =
{
	0xA00100000,
	"ORIGIN_ERROR_LSX_INVALID_RESPONSE",
	"",
	"The LSX Decoder didn't expect this response",
	L""
};

static ORIGIN_ERROR A51 =
{
	0xA00100000,
	"ORIGIN_ERROR_LSX_NO_RESPONSE",
	"",
	"The LSX server didn't respond within the set timeout",
	L""
};

static ORIGIN_ERROR A52 =
{
	0xA00100000,
	"ORIGIN_ERROR_LSX_INVALID_REQUEST",
	"",
	"The LSX Decoder didn't expect this request",
	L""
};

//commerce
static ORIGIN_ERROR A53 =
{
	0xA00100000,
	"ORIGIN_ERROR_COMMERCE_NO_SUCH_STORE",
	"",
	"The store could not be found",
	L""
};

static ORIGIN_ERROR A54 =
{
	0xA00100000,
	"ORIGIN_ERROR_COMMERCE_NO_SUCH_CATALOG",
	"",
	"The catalog could not be found",
	L""
};

static ORIGIN_ERROR A55 =
{
	0xA00100000,
	"ORIGIN_ERROR_COMMERCE_INVALID_REPLY",
	"",
	"The server reply was not understood",
	L""
};

static ORIGIN_ERROR A56 =
{
	0xA00100000,
	"ORIGIN_ERROR_COMMERCE_NO_CATEGORIES",
	"",
	"No categories were found",
	L""
};


static ORIGIN_ERROR A57 =
{
	0xA00100000,
	"ORIGIN_ERROR_COMMERCE_NO_PRODUCTS",
	"",
	"No products were found",
	L""
};

//proxy
static ORIGIN_ERROR A58 =
{
	0xA00100000,
	"ORIGIN_ERROR_PROXY",
	"",
	"Base proxy error. You shouldn't get this error",
	L""
};

static ORIGIN_ERROR A59 =
{
	0xA00100000,
	"ORIGIN_SUCCESS_PROXY_OK",
	"ORIGIN_LEVEL_4",
	"Server success: OK",
	L""
};

static ORIGIN_ERROR A60 =
{
	0xA00100000,
	"ORIGIN_SUCCESS_PROXY_CREATED",
	"",
	"Server success: Created",
	L""
};

static ORIGIN_ERROR A61 =
{
	0xA00100000,
	"ORIGIN_SUCCESS_PROXY_ACCEPTED",
	"",
	"Server success: Accepted",
	L""
};

static ORIGIN_ERROR A62 =
{
	0xA00100000,
	"ORIGIN_SUCCESS_PROXY_NON_AUTH_INFO",
	"",
	"Server success: Non-Authoritative Information",
	L""
};

static ORIGIN_ERROR A63 =
{
	0xA00100000,
	"ORIGIN_SUCCESS_PROXY_NO_CONTENT",
	"",
	" Server success: No Content",
	L""
};

static ORIGIN_ERROR A64 =
{
	0xA00100000,
	"ORIGIN_SUCCESS_RESET_CONTENT",
	"",
	"Server success: Reset Content",
	L""
};

static ORIGIN_ERROR A65 =
{
	0xA00100000,
	"ORIGIN_SUCCESS_PARTIAL_CONTENT",
	"",
	"Server success: Partial Content",
	L""
};

static ORIGIN_ERROR A66 =
{
	0xA00100000,
	"ORIGIN_ERROR_PROXY_BAD_REQUEST",
	"",
	"Server error: Bad Request",
	L""
};

static ORIGIN_ERROR A67 =
{
	0xA00100000,
	"ORIGIN_ERROR_PROXY_UNAUTHORIZED",
	"",
	"Server error: Unauthorized",
	L""
};

static ORIGIN_ERROR A68 =
{
	0xA00100000,
	"ORIGIN_ERROR_PROXY_PAYMENT_REQUIRED",
	"",
	"Server error: Payment Required",
	L""
};

static ORIGIN_ERROR A69 =
{
	0xA00100000,
	"ORIGIN_ERROR_PROXY_FORBIDDEN",
	"",
	"Server error: Forbidden",
	L""
};

static ORIGIN_ERROR A70 =
{
	0xA00100000,
	"ORIGIN_ERROR_PROXY_NOT_FOUND",
	"",
	"Server error: Not found",
	L""
};

static ORIGIN_ERROR A71 =
{
	0xA00100000,
	"ORIGIN_ERROR_PROXY_METHOD_NOT_ALLOWED",
	"",
	"Server error: Method not Allowed",
	L""
};

static ORIGIN_ERROR A72 =
{
	0xA00100000,
	"ORIGIN_ERROR_PROXY_NOT_ACCEPTABLE",
	"",
	"Server error: Not Acceptable",
	L""
};

static ORIGIN_ERROR A73 =
{
	0xA00100000,
	"ORIGIN_ERROR_PROXY_REQUEST_TIMEOUT",
	"",
	"Server error: Request Timeout",
	L""
};

static ORIGIN_ERROR A74 =
{
	0xA00100000,
	"ORIGIN_ERROR_PROXY_CONFLICT",
	"",
	"Server error: Conflict",
	L""
};

static ORIGIN_ERROR A75 =
{
	0xA00100000,
	"ORIGIN_ERROR_PROXY_INTERNAL_ERROR",
	"",
	"Server error: Internal Server Error",
	L""
};

static ORIGIN_ERROR A76 =
{
	0xA00100000,
	"ORIGIN_ERROR_PROXY_NOT_IMPLEMENTED",
	"",
	"Server error: Not Implemented",
	L""
};

static ORIGIN_ERROR A77 =
{
	0xA00100000,
	"ORIGIN_ERROR_PROXY_BAD_GATEWAY",
	"",
	"Server error: Bad Gateway",
	L""
};

static ORIGIN_ERROR A78 =
{
	0xA00100000,
	"ORIGIN_ERROR_PROXY_SERVICE_UNAVAILABLE",
	"",
	"Server error: Service Unavailable",
	L""
};

static ORIGIN_ERROR A79 =
{
	0xA00100000,
	"ORIGIN_ERROR_PROXY_GATEWAY_TIMEOUT",
	"",
	"Server error: Gateway Timeout",
	L""
};

//def
//A18 - пропущен
#define ERROR_COUNT 0x4F //79
static pORIGIN_ERROR EA_Origin_error_stack[ERROR_COUNT] = {&A0, &A1, &A2, &A3, &A4, &A5, &A6, &A7, &A8, &A9, &A10,
&A11, &A12, &A13, &A14, &A15, &A16, &A17, &A19, &A20, 
&A21, &A22, &A23, &A24, &A25, &A26, &A27, &A28, &A29, &A30, 
&A31, &A32, &A33, &A34, &A35, &A36, &A37, &A38, &A39, &A40, 
&A41, &A42, &A43, &A44, &A45, &A46, &A47, &A48, &A49, &A50,
&A51, &A52, &A53, &A54, &A55, &A56, &A57, &A58, &A59, &A60,
&A61, &A62, &A63, &A64, &A65, &A66, &A67, &A68, &A69, &A70,
&A71, &A72, &A73, &A74, &A75, &A76, &A77, &A78, &A79
};

//SUBSD DEFS
extern pORIGIN_ERROR EA_ERROR_find_error_description_by_error_code(__in EA_ERROR_ID_def ea_error_id); //возвращает стек ошибки
extern void __fastcall EA_ERROR_OutputDebugString(size_t a1, int byte_break, const CHAR *out_string); //дефолтный вариант вывода ошибок
