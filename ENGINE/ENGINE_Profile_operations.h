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
#include "ENGINE.h"
#include "../XED/XEDParse.h"

//------------------ define data ------------------ 
//данные по файлу описи профиля
#define PROFILE_READFILE_NAME L"DH_Profile.txt" //имя файла, создаваемого в директории 
#define PROFILE_READFILE_CONTENT L"This folder containt profiles for DH!" //текст в нём

//контейнеры профиля
//#define PROFILE_VM_nfo_CONTAINT_FILE_NAME L"VM.spr" //имя файла данных VM
#define PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN 32

#define PROFILE_VM_nfo_FOLDER_NAME L"VM\\"
#define PROFILE_VM_nfo_EXTENTION_CRITICAL L".vm0"
#define PROFILE_VM_nfo_EXTE_FIND L"*.vm0"

#define PROFILE_VM_nfo_BACKUP_FOLDER_NAME L"backed\\"

#define PROFILE_VM_addres_pool_EXTENTION_CRITICAL L".vm_ap"
#define PROFILE_VM_addres_pool_EXTE_FIND L"*.vm_ap"

#define PROFILE_PATCH_nfo__NOT_APPLIED_FOLDER_NAME L"NOT_APPLIED\\"
#define PROFILE_PATCH_nfo_FOLDER_NAME L"PATCH\\"
#define PROFILE_PATCH_nfo_EXTENTION_CRITICAL L".patch0"
#define PROFILE_PATCH_nfo_EXTE_FIND L"*.patch0"
#define PROFILE_PATCH_nfo_CONFIG_FILE_REGION_INI L"_patch_region.ini"

#define PROFILE_LIC_DATA_DUMP_nfo_FOLDER_NAME L"LICDMP\\"
#define PROFILE_LIC_DATA_DUMP_nfo_EXTENTION_CRITICAL L".licdmp"
#define PROFILE_LIC_DATA_DUMP_nfo_EXTE_FIND L"*.licdmp"

#define PROFILE_TRACE_VMP2_LOG_nfo_FOLDER_NAME L"TRACE\\"
#define PROFILE_TRACE_VMP2_LOG_nfo_EXTENTION_CRITICAL L".tr2"
#define PROFILE_TRACE_VMP2_LOG_nfo_EXTE_FIND L"*.tr2"


#define PROFILE_TRACE_VMPX_EXTENTION_CRITICAL L".ini"
#define PROFILE_TRACE_VMPX_EXTE_FIND L"*.ini"

#define PROFILE_VM_VMPX_EXTENTION_CRITICAL L".ini"
#define PROFILE_VM_VMPX_EXTE_FIND L"*.ini"

#define PROFILE_VM_BILLET_FOLDER_NAME L"billet\\"
#define PROFILE_VM_BILLET_EXTENTION_CRITICAL L".bll"
#define PROFILE_VM_BILLET_EXTE_FIND L"*.bll"

#define PROFILE_PIRATE_MAP_nfo_FOLDER_NAME L"PIRATE_MAP\\"
#define PROFILE_PIRATE_MAP_nfo_EXTENTION_CRITICAL L".pmap"
#define PROFILE_PIRATE_MAP_nfo_EXTE_FIND L"*.pmap"

#define PROFILE_IMPORT_nfo_CONTAINT_FILE_NAME L"IMPORT.spr" //имя файла данных таблицы имппорта

//PROFILE_PATCH_nfo_CONFIG_FILE_REGION_INI content
#define PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_Auto_Alloc_value L"auto"
#define PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_Auto_Alloc_default -1

#define PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section L"VMP_Section"
#define PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_name_key L"Name"
#define PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_address_key L"Address"
#define PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_size_key L"Size"

#define PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_original_copy L"VMP_Section_original_copy"
#define PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_original_copy_start_address_key L"Start"
#define PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_original_copy_end_address_key L"End"
#define PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_original_copy_is_req_memcpy_original_section_flag_key L"ReqMemcpy"

#define PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_patch_map L"VMP_patch_map"
#define PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_patch_map_start_address_key L"Start"
#define PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_patch_map_end_address_key L"End"
#define PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_patch_map_lockmarker_key L"LockMarker"
#define PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_patch_map_lockmarker_value L"FF"
#define PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_patch_map_lockmarker_default 0xFF
#define LM_def PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_patch_map_lockmarker_default

#define PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_DH_hooks_and_filters L"DH_hooks_and_filters"
#define PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_DH_hooks_and_filters_start_address_key L"Start"
#define PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_DH_hooks_and_filters_end_address_key L"End"
#define PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_DH_hooks_and_filters_float_pointer_key L"fPointer"
// VMPX CONTEXT
#define PROFILE_SERVICE_CONFIG_FILE_VMPX_CONTEXT_INI_DH_Predict_STATIC_key L"_STATIC"
#define PROFILE_SERVICE_CONFIG_FILE_VMPX_CONTEXT_INI_DH_Predict_key_1 L"Predict_1"
#define PROFILE_SERVICE_CONFIG_FILE_VMPX_CONTEXT_INI_DH_Predict_key_2 L"Predict_2"
#define PROFILE_SERVICE_CONFIG_FILE_VMPX_CONTEXT_INI_DH_Predict_key_3 L"Predict_3"
#define PROFILE_SERVICE_CONFIG_FILE_VMPX_CONTEXT_INI_DH_Predict_key_4 L"Predict_4"

#define PROFILE_SERVICE_CONFIG_FILE_VMPX_CONTEXT_INI_DH_Critical_key L"Critical"

#define PROFILE_SERVICE_CONFIG_FILE_VMPX_CONTEXT_INI_DH_After_key_1 L"After_1"
#define PROFILE_SERVICE_CONFIG_FILE_VMPX_CONTEXT_INI_DH_After_key_2 L"After_2"

#define PROFILE_SERVICE_CONFIG_FILE_VMPX_CONTEXT_INI_DH_EXTRARCT_key L"_EXTRACT"
#define PROFILE_SERVICE_CONFIG_FILE_VMPX_CONTEXT_INI_DH_EXTRARCT_VALUE_LINK_key L"_EXTRACT_VALUE_LINK"

//CONTROL SYMBOLS
#define PROFILE_VMPX_CONTROL_SYMBOL L'$'
#define PROFILE_VMPX_CONTROL_SYMBOL_HIMSELF_ADDR L'S'
#define PROFILE_VMPX_CONTROL_SYMBOL_HIMSELF_ADDR_constant 0x140000000L

#define PROFILE_VMPX_CONTROL_SYMBOL_EXTRACT_ADDR L'E'
#define PROFILE_VMPX_CONTROL_SYMBOL_EXTRACT_ADDR_constant PROFILE_VMPX_CONTROL_SYMBOL_HIMSELF_ADDR_constant

#define PROFILE_VMPX_CONTROL_SYMBOL_CPU_REGISTER L'R' //default = x64 register (RAX, RBX, RDX, ..., R15)
#define PROFILE_VMPX_CONTROL_SYMBOL_CPU_REGISTER_QWORD_default L"RCX"
#define PROFILE_VMPX_CONTROL_SYMBOL_CPU_REGISTER_DWORD_default L"ECX"
#define PROFILE_VMPX_CONTROL_SYMBOL_CPU_REGISTER_WORD_default L"CX"
#define PROFILE_VMPX_CONTROL_SYMBOL_CPU_REGISTER_BYTE_default L"CL"
#define PROFILE_VMPX_CONTROL_SYMBOL_CPU_REGISTER_QWORD_default_ascii "rcx"
#define PROFILE_VMPX_CONTROL_SYMBOL_CPU_REGISTER_DWORD_default_ascii "ecx"
#define PROFILE_VMPX_CONTROL_SYMBOL_CPU_REGISTER_WORD_default_ascii "cx"
#define PROFILE_VMPX_CONTROL_SYMBOL_CPU_REGISTER_BYTE_default_ascii "cl"

//команды
#define PROFILE_READ_ALL -1 //читает все данные в файле
#define PROFILE_FIRST_FILE -1 //читает первый файла в директории

//максимально допустимое кол-во файлов одного контейнера
#define MAX_CONTAINTER_FILES 1000



//МАКРОСЫ КОДА
#define A_RETURN_ORIGINAL_PROFLE_PATH HUNT_HIVE.PROFILE_PATH[HUNT_HIVE.PROFILE_PATH_WCSLEN] = (WCHAR)L'\0'
#define A_RETURN_ORIGINAL_VMPX_PROFLE_PATH HUNT_PROFILE_HIVE.VMPX_PROFILE_PATH[HUNT_PROFILE_HIVE.VMPX_PROFILE_PATH_WCSLEN] = (WCHAR)L'\0'
#define A_RETURN_ORIGINAL_PROFLE_PATH_MT_copy(tstr_0ref) tstr_0ref[HUNT_HIVE.PROFILE_PATH_WCSLEN] = (WCHAR)L'\0'
#define A_RETURN_ORIGINAL_TEMPORARY_PATH(end) TMP_Profle_Folder[end] = (WCHAR)L'\0'
#define A_INIT_BUFFER_PROFILE_STRING wcscpy(&HUNT_HIVE.PROFILE_PATH[0],Profile_path); wcscat(&HUNT_HIVE.PROFILE_PATH[0],Target_Process->ProcessName); wcscat(&HUNT_HIVE.PROFILE_PATH[0],L"\\\0")
#define A_INIT_TEMP_BUFFER_STRING wcscpy(&TMP_Profle_Folder[0],Profile_path); wcscat(&TMP_Profle_Folder[0],Target_Process->ProcessName)

//тип профиля
#define PROFILE_TYPE_COMMON_NFO 0 //тип - общая инфа
#define PROFILE_TYPE_VM 1 //тип - виртуальная машина
#define PROFILE_TYPE_IMPORT 2 //тип - данные импорта
#define PROFILE_TYPE_PATCH__NOT_APPLIED_FLAG 3 //тип - данные патчинга адресов с флагом "not_appied"
#define PROFILE_TYPE_PATCH_ONLINE 4 //тип - данные ДЕЙСТВУЮЩЕГО патчинга адресов
#define PROFILE_TYPE_LIC_DATA_DUMP 5//тип - входные данные для лицензии(PEB, IMAGE_DATA_DIRECTORY, KUSER...)
#define PROFILE_TYPE_VM_ADDRES_POOL 6 //тип - пул адресов хендлов виртуальной машины
#define PROFILE_TYPE_TRACE_VMP2_LOG 7 //тип - лог данных трассы от vmp2
#define PROFILE_TYPE_PATCH_REGIONS_INFO_INI 8 //тип - кофигурационный INI-файл -> информация о регионах памяти, предназначенных для патчинга
#define PROFILE_TYPE_PATCH_FILTER_ASM_STACK_INI 9 //тип - кофигурационный INI-файл -> стек инструкций по фильтрам (для патчинга)
#define PROFILE_TYPE_VM_VMPX_CONTEXT_INI 10 //тип - кофигурационный INI-файл -> стек контекстов виртуальной машины (для поиска)
#define PROFILE_TYPE_VM_BILLET 11 //тип - заготовка (сырая информация) для PROFILE_TYPE_VM. Содержит данные о VM_ENTER без анализа контекста и основного содержимого виртуальной машины. Используется для трассировщика
#define PROFILE_TYPE_PIRATE_MAP 12 //тип - пиратская карта(tmp) по указанному региону

//extern WCHAR CURRENT_Profile[MAX_PATH*2]; //сьрока профиля
//extern ULONG CURRENT_Profile_len; //длина профиля

//<<<<<<<<<<<<<<< codedef <<<<<<<<<<<<<<<<<
#define codedef_set_default_ops_filename(lpcstring) ScanFileName = (lpcstring)


//TYPE OF PROFILE enums !!!!!!!!!!!!!!!
enum VMPX_CONTROL_SYMBOLS
{PROFILE_VMPX_CONTROL_SYMBOL_HIMSELF_ADDRES = 1};

enum PROFILE_MT_STACKWORK_TYPE
{
	TW_pVM_THREADWORK = 1 //pVM_THREADWORK 
};

//++++++++++++++++++++++ structs ++++++++++++++++++++++
typedef struct FILE_ENUM_NAMES
{WCHAR FileName[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN];
}DH_ENUM_FILES_of_PROFILES, *pDH_ENUM_FILES_of_PROFILES;


//********** DH PROFILE func ********** //
extern bool DH_InitProfiles_Engine_Path( __out LPWSTR Profile_path); //инициализация папки профилей (true/false)
extern bool DH_Is_HAVE_hunt_Profile( __in pDH_ProcInfo Target_Process,  __in LPWSTR Profile_path); //проверяет существует ли профиль (true/false)
extern ULONG DH_SetCurrentHuntProfile( __in pDH_ProcInfo Target_Process,  __in  LPWSTR Profile_path); //устанавливает текущий профиль (return длина WCHAR строки)
extern ULONG DH_GetCurrentHuntProfile( __out LPWSTR Profile_path); //возвращает текущий профиль (возвращает длину строки)
extern bool DH_CreateNewHuntProfile( __in pDH_ProcInfo Target_Process,  __in LPWSTR Profile_path); //создает новый hunt профиль (true/false)
extern void DH_Profile_Verify_Folders(__in UINT PROFILE_x_TYPE);  //проверить папки, создать недостающие
extern bool DH_Write_Current_hunt_profile_CONTAINER_savedlg(__in UINT PROFILE_x_TYPE, //тип контейнера
	__in BYTE* write_buff, //буфер записи
	__in size_t num_bytes_to_write //кол-во байт записи  sizeof(струкрура контейнера)
		);//save CFileDlg dlg - для сохранения дампа

extern PVOID DH_Read_Current_hunt_profile_CONTAINER(
	__in UINT PROFILE_x_TYPE, //тип контейнера
	__in WCHAR* PROFILE_x_CONTAINT_FILE_NAME, //имя файла контейнера (
	__in size_t num_bytes_of_read, //кол-во байт для чтения  (PROFILE_READ_ALL или число)
	__out PULONG Return_Readed_Bytes //кол-во, которое было реально прочитано
	);//чтение указанного контейнера. (возвращает адрес с буфером данных)

bool DH_Write_Current_hunt_profile_CONTAINER(
	__in UINT PROFILE_x_TYPE, //тип контейнера
	__in WCHAR* PROFILE_x_CONTAINT_FILE_NAME,  //имя файла контейнера
	__in BYTE* write_buff, //буфер записи
	__in size_t num_bytes_to_write, //кол-во байт записи  sizeof(струкрура контейнера)
	__out PULONG Return_Writed_Bytes);  //записывает данные HIVE профиля с типом

extern bool DH_Test_Current_hunt_profile_CONTAINER(
	__in UINT PROFILE_x_TYPE, //тип контейнера
	__in WCHAR* PROFILE_x_CONTAINT_FILE_NAME //имя файла контейнера
	); //проверка указанного контейнера. (true-false)

extern SERVICE_COUNT DH_Get_Current_hunt_profile_ALL_FILE_CONTAINERS(
	__in UINT PROFILE_x_TYPE, //тип контейнера
	__out DH_ENUM_FILES_of_PROFILES File_names_buffer[] //имя файла контейнера (результат malloc)
	);//получение всех имён файлов. (возвращает кол-во файлов)

extern bool DH_Profile_Containter_name_convert_extension_to_new(
	__in WCHAR* PROFILE_x_CONTAINT_FILE_NAME, //имя контейнера
	__in SERVICE_TYPE New_PROFILE_x_TYPE, //новый тип
	__in SERVICE_TYPE Current_PROFILE_x_TYPE //старый тип
	); //конвертирует расширение контейнера с использованием указания типов

extern bool DH_LOCAL_is_filename_containt_profile_extension(__in UINT PROFILE_x_TYPE,__in WCHAR* PROFILE_x_CONTAINT_FILE_NAME); //имя файла содержит требуемое расширение файла?

extern bool DH_Profile_Create_Service_File(
	__in SERVICE_TYPE PROFILE_x_TYPE, //по профилю
	__in TCHAR* File_full_patch //полный путь к файлу
	);  //создание типового служебного файла (требуется полный путь)

extern PVOID DH_Profile_PARSE_Arbitary_FILTER_CONFIG(
	__in TCHAR* pRaw_VMPX_Profiler, //указатель на прочитанный буфер конфига
	__in size_t Raw_size //размер буфера
	); //парсит профилер vp_x фильтра


extern PVOID DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG(
	__in TCHAR* pRaw_VMPX_Profiler, //указатель на прочитанный буфер конфига
	__in size_t Raw_size, //размер буфера
	__in TCHAR* pINIFilename //имя INI файла
	);//парсит профилер vp_x контекста

extern SERVICE_COUNT DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__GetSectionKeys(
	__in TCHAR* pBranchName, //имя секции (ветки)
	__in LPVOID pResultUnitTypeStack, //конечный стек сбора данных
	__in TCHAR* pINIFilename //имя INI файла
	);//производная DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG. Ввыполняет сбор ключей из указанной секции (ветки).


/*
extern bool DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Normalize_and_assembly(
	__in TCHAR* pAsmInstruction, //входящая инструкция
	__in size_t AsmInstruction_size, //длина строки инструкции (нужна для перебора CS)
	__in XEDPARSE* pXED_parse_asm, //указатель на структуру XEDPARSE
	__out SERVICE_TYPE* pSetCS_type, //указатель на сохранение типа контрольного символа (если присутсвует - то > NULL)
	__out size_t*		pSetCS_first_symbol, //указатель на сохранение позиции первого контрольного символа
	__out bool*	pSetStaticFlag //указатель на флаг static/dynamic
	);//производная DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG. Выполняет поиск контрольных символов в строке асм инструкции, из замену, а также заполнение структуры XEDPARSE
	*/


extern bool DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Normalize_and_assembly(
	__in SERVICE_TYPE Ident_normalize, //тип нормализируемой функции (согласно VMPX_CONTEXT_TYPE_ASMS)
	__in size_t AsmInstruction_size, // now! длина строки инструкции(нужна для перебора CS)
	__out LPVOID pResultUnitTypeStack // ссылка на VM_VMPX_PROFILE_INI
	);//производная DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG. Выполняет поиск контрольных символов в строке асм инструкции, из замену, а также заполнение структуры XEDPARSE с выставлением необходимых флагов

extern bool DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Release_normalize_instruction(
	__in __out LPVOID pResultUnitTypeStack, // ссылка на заполненный VM_VMPX_PROFILE_INI
	__in SERVICE_TYPE Ident_normalize, //тип нормализируемой функции (согласно VMPX_CONTEXT_TYPE_ASMS)
	__in PVOID pInstruction_pool, //ссылка на пул инструкций (для сравнения)
	__in PVOID pSeqStack //ссылка на стек цепочек - для самостоятельного заполнения
	//__in SERVICE_TYPE Asm_operation //выполняемое конечное действие нормализации - аcсемблирование ИЛИ дизассемблирование (XED_PARSE__ASSEMBLY - BEA_ENGINE__DISSASM)
	);//производная DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG. Окончательно реализует инструкцию в которой содержатся контрольные символы.

extern bool DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Release_normalize_instruction__NO_WRITE(
	__in __out LPVOID pResultUnitTypeStack, 
	__in SERVICE_TYPE Ident_normalize, 
	__in PVOID pInstruction_pool
	); //ТО-ЖЕ САМОЕ ЧТО И DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Release_normalize_instruction. Только без операций записи данных.

////////////////////////////////// [M] [T] //////////////////////////////

extern bool __fastcall DH_Profile_Shedule_threadwork__Write(
	__in PVOID pThreadWorkStack_pointer, //входящий указатель на стек
	__in SERVICE_TYPE StackType //тип стека (определяется PROFILE_MT_STACKWORK_TYPE)
	); //многопоточная операция записи стека ThreadWork.

static DWORD WINAPI DH_PROFILE_Shedule_WRITE_threadwork_MT(
	LPVOID Param
	); //дочерняя функция DH_Profile_Shedule_threadwork__Write. Выполнение расписания задач по записи контейнеров

extern bool __inline DH_Write_Current_hunt_profile_CONTAINER_MT(
	__in UINT PROFILE_x_TYPE, //тип контейнера
	__in WCHAR* PROFILE_x_CONTAINT_FILE_NAME,  //имя файла контейнера
	__in BYTE* write_buff, //буфер записи
	__in size_t num_bytes_to_write, //кол-во байт записи  sizeof(струкрура контейнера)
	__in TCHAR* pLocalHUNTPROFILEPATCH_copy //локальная копия HUNT_HIVE.PROFILE_PATCH строки для исключения пеерезаписи несколькими потоками
	);//записывает данные HIVE профиля с типом MT. МНОГОПОТОЧНАЯ ОПТИМИЗАЦИЯ!
extern PVOID __inline DH_Read_Current_hunt_profile_CONTAINER__readall_MT(
	__in ULONG PROFILE_x_TYPE, //тип контейнера
	__in WCHAR* PROFILE_x_CONTAINT_FILE_NAME, //имя файла контейнера
	__in TCHAR* pLocalHUNTPROFILEPATCH_copy //локальная копия HUNT_HIVE.PROFILE_PATCH строки для исключения пеерезаписи несколькими потоками
	);  //считывает данные HIVE профиля - не предназнеачен для чтения данных с конфигурационных файлов!!! МНОГОПОТОЧНАЯ ОПТИМИЗАЦИЯ!