/*
	DENUVO_Profiler v0.3
	--------------------
	CRACKLAB, ELF
	--------------------
	Tiberium reversing
	********************
	2011	-	2024
*/

#include "../stdafx.h"
#pragma once
#include "ENGINE.h"
#include "ENGINE_VM_operations.h"
#include "KI_USER.h"
#include "W_PEB.h"
#include "../XED/XEDParse.h"

#pragma comment (lib, "../XED/XEDParse.lib")
//------------------ define data ------------------ 

extern TCHAR tHOOK_template[MAX_PATH];
extern BYTE tNOP_SLED[];

#define HOOK_AND_FILTERS__alignment_ADD_CONST 0x10
#define HOOK_AND_FILTERS__alignment_AND_CONST 0xFFFFFFFFFFFFFFF0

#define DEFAULT_NUM_OF_COMPARE_EVENTS 3510

#define MAX_ASM_SAVE_BYTES 50
#define MAX_ANY_DATA_SAVE_BYTES 150

#define MAX_INTRECEPTOR_BYTES 1500

#define MAX_STACK_ALLOCATE_BUFFER_BYTES 10000

#define DEFAULT_Region_VMP_hook_and_filter_AUTO_ALLOCATION_SIZE 0x140000

#define MAP_RETURN_COMPARE_EQ_FILES -1 //файлы одинаковы
#define MAP_RETURN_COMPARE_TOO_MANY_DIFFERECES -2 //слишком огромное количество различий

#define SIZE__PATH_FILE_STACK_and_HELD_GROUP sizeof(PATCH_INFO_STACK) + sizeof(PATCH_HELD_GROUP)

//patch erros
#define PATCH_TYPEDEF_ADDR__ERR_NOT_VERIFY -1

#define PATCH_JMP_INSTRECEPT_CONST_SIZE 5 //sizeof(jmp intrecept - E9 A8 D2 0B 00) = 5 bytes
#define PATCH_SAFE_BOUND 32

#define FILTER_DEFINE_CONTROL_SYMBOL '$'
#define FILTER_SUBDEFINE_CONTROL_SYMBOL_VM_HI_BORDER 'H'
#define FILTER_SUBDEFINE_CONTROL_SYMBOL_VM_LO_BORDER 'L'
#define FILTER_SUBDEFINE_CONTROL_SYMBOL_ORIGINAL_COPY 'C'
#define FILTER_SUBDEFINE_CONTROL_SYMBOL_PATCH_MAP 'P'
#define FILTER_SUBDEFINE_CONTROL_SYMBOL_RETURN 'R'
#define FILTER_SUBDEFINE_CONTROL_SYMBOL_MARKED_1 '1'
#define FILTER_SUBDEFINE_CONTROL_SYMBOL_MARKED_2 '2'
#define FILTER_SUBDEFINE_CONTROL_SYMBOL_MARKED_3 '3'
#define FILTER_SUBDEFINE_CONTROL_SYMBOL_MARKED_4 '4'
#define FILTER_SUBDEFINE_CONTROL_SYMBOL_MARKED_5 '5'
#define FILTER_SUBDEFINE_CONTROL_SYMBOL_MARKED_6 '6'
#define FILTER_SUBDEFINE_CONTROL_SYMBOL_MARKED_7 '7'
#define FILTER_SUBDEFINE_CONTROL_SYMBOL_MARKED_8 '8'
#define FILTER_SUBDEFINE_CONTROL_SYMBOL_MARKED_9 '9'
#define FILTER_SUBDEFINE_CONTROL_SYMBOL_VM_SUPPLY15 'X'

#define FILTER_DEFINE_LABEL_SYMBOL '%'
#define FILTER_DEFINE_BRANCH_OPEN_SYMBOL '['
#define FILTER_DEFINE_BRANCH_CLOSE_SYMBOL ']'
#define FILTER_DEFINE_COMMENT_SYMBOL ';'
#define FILTER_DEFINE_END_STRING_SYMBOL 0x0A

#define MAX_ASM_POOL_IN_STACK_BYTES 200
#define MAX_ASM_PARSE_ITERATIONS 100

#define MAX_PATCH_SNAPSHOTS 1500

#define MAX_REWRITE_SITES 10

#define FILTER_MARKED_JMP 1

#define END_STR 0xD

enum PARSE_ERRORS {PARSE_ERROR_NO_OPEN = 1,
	PARSE_ERROR_NO_CLOSE, PARSE_ERROR_UNSUPPORTED_KEY};

//TYPE OF PATCHES enums !!!!!!!!!!!!!!!
enum PATCH_TYPE_METHOD_ENUMS {
	PATCH_NOT_APPLIED, //флаг "not applied" (disabled)
	PATCH_ONLY_NOP, //ТОЛЬКО за"NOP"ить ЦЕЛЕВУЮ ИНСТРУКЦИЮ 
	PATCH_NOP_AND_JMP_TO_INTRECEPTOR_with_RETURN, //за"NOP"ить ЦЕЛЕВУЮ ИНСТРУКЦИЮ и ВЫПОЛНИТЬ ПРЫЖОК В ПЕРЕХВАТЧИК с ПОСЛЕДУЮЩИМ ВОЗВРАЩЕНИЕМ
	PATCH_NOP_AND_JMP_TO_INTRECEPTOR__NO_RETURN //за"NOP"ить ЦЕЛЕВУЮ ИНСТРУКЦИЮ и ВЫПОЛНИТЬ ПРЫЖОК В ПЕРЕХВАТЧИК БЕЗ ВОЗВРАЩЕНИЯ В ТЕЛО ИНСТРУКЦИИ
	};

enum PATCH_TYPE_METHOD_INTRECEPT_RELEASE_ENUMS {
	PATCH_NOP_SLED_AND_NEXT_JUMP_CONST = 1, //"NOP" SLED target-инструкции и использование СЛЕДУЮЩЕГО JMP const для перехватчика
	PATCH_NOP_SLED_AND_PREV_JUMP_CONST, //"NOP" SLED target-инструкции и использование ПРЕДЫДУЩЕГО JMP const для перехватчика
	PATCH_DISPLACE_OTHERS_ASM, //ВЫТЕСНЕНИЕ инструкций с target-асм для jmp на перехватчик
	PATCH_COMBINE__NOP_SLED_and_DISPLACE_OTHERS_ASM_TOO_CLOSE_NEXT_JMP //ВЫТЕСНЕНИЕ инструкций с target-асм для NOP и последующим использованием jmp на перехватчик
	};

enum PATCH_TYPE_OWNERS_ENUMS {
	//executable
	PATCH_OWNER_VM_INSIDE_EXECUTE = 1, //тип - выполняемый код внутри VM
	PATCH_OWNER_VM_OUTSIDE_EXECUTE, //тип - выполняемый код за пределами островков VM
	//coverage addreses
	PATCH_OWNER_VM_COVERAGE, //тип - адреса покрытия хендлов VM
	//data
	PATCH_OWNER_VM_SUPPLY15_DATA, //тип - хранилище №1,5 (таблица хендлов)
	PATCH_OWNER_VM_PCODE_DATA, //тип - лента p-code
	PATCH_OWNER_VM_CONSTANT_DATA //тип - константы, используемые VM
};

enum PATCH_ERROR_STATUS {
	//executable
	PATCH_ERROR_SUCCESS = 0, //без ошибок
	PATCH_ERROR_VM_STACK_ABSENT, //отсутсвует контейнер VM stack
	PATCH_ERROR_ADDR_POOL_ABSENT, //отсутсвует контейнер Addr pool
	PATCH_ERROR_TARGET_ADR_in_ADDR_POOL_BAD, //не совпадает позиция Target Addr в пуле адресов хендла VM
	PATCH_ERROR_PATCH_METHOD_NOT_ASSIGNED, //нет возможности назначить известнеые методы патчинга/установки хука для target address
	PATCH_ERROR_TARG_ADDR_NOT_VERIFY, //target address не прошел верификацию (ReadProcessMemory)
	PATCH_ERROR_ARBITARY_FILTER_NOT_ASSINGED, //не удалось назначить arbitary filter
	PATCH_ERROR_CANNOT_CONSTRUCT_RETAIL_ARBITARY_FILTER, //не удалось ассемблировать окончательный arbitary filter
};

enum PATCH_MAP_REWRITE_DISPLACE_TARGET {
	//executable
	PATCH_REWRITE_TARGET_is_VM_ORIGINAL_TYPE_ASM = 1, //перезаписана оригинальная типовая инструкция внутри VM
	PATCH_REWRITE_TARGET_is_VM_INSIDE_ASM, //перезаписана любая другая инструкция внутри VM не используемая DH
	PATCH_REWRITE_TARGET_is_MY_HOOK, //перезапись нашего хука
	PATCH_REWRITE_TARGET_is_VM_OUTSIDE_ASM, //перезаписана любая другая инструкция вне VM
	//coverage addreses
	PATCH_REWRITE_TARGET_is_VM_COVERAGE, //перезаписаны адреса покрытия хендлов VM
	//data
	PATCH_REWRITE_TARGET_is_VM_SUPPLY15_DATA, //перезаписано хранилище №1,5 (таблица хендлов)
	PATCH_REWRITE_TARGET_is_VM_PCODE_DATA, //перезаписана лента p-code
	PATCH_REWRITE_TARGET_is_VM_CONSTANT_DATA //перезаписаны константы, используемые VM
};

// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& code definitions &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
#define codedef_set_pathcing_intreception_type(release_type) pIteraion_PatchArrive->target_addres__assign_path_method = (release_type); last_pathing_method = (release_type)
#define codedef_no_key pOPEN_BRANCH = NULL

static BYTE MAP_mark_buffer[10000];
static BYTE MAP_mark_ROLL_BACK_buffer[10000] = {NULL};
//++++++++++++++++++++++ structs ++++++++++++++++++++++
typedef struct DH_Dump_Cell_service_struct
{
	SERVICE_ADDRES Cell_mem_jmp; //ячейка для операнда прыжка в выделенную память
	//OPERATIONAL_OPERAND_CELL_ADDRES Operand_Address; //ячейка для операнда прыжка в выделенную память
	DWORD Operand_Address; //ячейка для операнда прыжка в выделенную память
}DUMP_CELL_STRUCT, *pDUMP_CELL_STRUCT; //СЕРВИСНАЯ СТРУКТУРА ХРАНЕНИЯ ДАННЫХ В ЯЧЕЙКАХ


// струтура контейнера дампа результатов проверки двух файлов
typedef struct DH_Files_different_result
{
ULONG64 Start_diff_Address; //адрес первого различающегося байта в файле
ULONG Diff_Bytes_Count; //кол-во различных байтов (вместе с первым)
LPBYTE Diff_bytes_file_1_DUMP_offset; //смещение сдампленных разных байт в блоке дампа для файла №1
LPBYTE Diff_bytes_file_2_DUMP_offset; //смещение сдампленных разных байт в блоке дампа для файла №2
}SET_FILES_COMPARE_EVENT, *pSET_FILES_COMPARE_EVENT;

/*
typedef struct DH_Files_compare_results
{
	SERVICE_COUNT compare_event_table_count; //кол-во блоков сравнения в таблице результатов

	ULONGLONG   File_1_ImageBase; //ImageBase любого из файлов (должны быть равны)
	IMAGE_SECTION_HEADER File_1_protection_section_header;//секция защиты любого из файлов (должны быть равны)

	bool Option_Align_to_multiple_DWORD_QWORD; //выравнивать кол-во различных байт до размера DWORD - QWORD, если меньше (например, если 3 или 7)
}HEADER_FILES_COMPARE_RESULTS, *pHEADER_FILES_COMPARE_RESULTS; //результаты сравнения файлов
*/

// ++++++++++ structs P A T C H ++++++++++++++++
typedef struct DH_PATCH_target_information_struct
{
	SERVICE_ADDRES target_addres; //группа важных однотипных адресов для патчинга (ex: в одном хендлике)
	SERVICE_TYPE   target_addres__assign_path_method; //присвоенный метод патчинга для каждого адреса PATCH_TYPE_ENUMS (ex: в одном хендлике)
	SERVICE_ARRAY_ID Containt_HELD_GROUP_relative_to_AddrPool_ID; //привязка группы патчинга к адресному пулу

	//critical patch
	SERVICE_ADDRES patch__HOOK_set_addres; //начальный адрес хука
	SERVICE_ADDRES patch__desdonation_FILTER_addres; //начальный адрес "перехватчика"
	SERVICE_ADDRES patch__return_addres_for_FILTER; //адрес возврата после отработки "перехватчика"
	SERVICE_ADDRES patch__original_bytes_addres; //начальный адрес замещаемых байт

	//оригинальные байты
	BYTE patch_BYTES_original_target_instruction_bytes[MAX_ASM_SAVE_BYTES]; //ОРИГИНАЛЬНАЯ ИНСТРУКЦИЯ
	SERVICE_COUNT patch_COUNT_original_target_instruction_bytes; //счетчик сохраненных байт оригинальной инструкции

	BYTE patch_BYTES_original_displace_JMP_intreceptor_instruction_bytes[MAX_ASM_SAVE_BYTES]; //АСМ ИНСТРУКЦИИ ВЫТЕСНЯЕМЫЕ "JMP intrecept"
	SERVICE_COUNT patch_COUNT_original_displace_JMP_intreceptor_instruction_bytes; //счетчик сохраненных байт вытесняемых "JMP intrecept"

		//(PATCH_OWNER_VM_SUPPLY15_DATA, PATCH_OWNER_VM_PCODE_DATA, PATCH_OWNER_VM_CONSTANT_DATA - байты в регионе)//
	BYTE patch_BYTES_original_ANY_DATA_bytes[MAX_ANY_DATA_SAVE_BYTES]; //оригинальные байты в регионах PATCH_OWNER_VM_SUPPLY15_DATA, PATCH_OWNER_VM_PCODE_DATA, PATCH_OWNER_VM_CONSTANT_DATA
	SERVICE_COUNT patch_COUNT_original_ANY_DATA_bytes; //количество данных в регионе

	//наши байты
	BYTE patch_BYTES_revoke_target_instruction_bytes[MAX_ASM_SAVE_BYTES]; //перезаписанные байты ОРИГИНАЛЬНОЙ ИНСТРУКЦИИ

	BYTE patch_BYTES_HOOK_bytes[MAX_ASM_SAVE_BYTES]; //хук на фильтр-перехватчик
	SERVICE_COUNT patch_COUNT_HOOK_instruction_bytes; //счетчик байт хука на фильтр

	BYTE patch_BYTES__FILTER_instruction_bytes[MAX_INTRECEPTOR_BYTES]; //АСМ ИНСТРУКЦИИ ПЕРЕХВАТЧИКА-фильтра!!!
	SERVICE_COUNT patch_COUNT__FILTER_instruction_bytes; //счетчик байт перехватчика-фильтра!!!
		//(PATCH_OWNER_VM_SUPPLY15_DATA, PATCH_OWNER_VM_PCODE_DATA, PATCH_OWNER_VM_CONSTANT_DATA - байты в регионе)//
	BYTE patch_BYTES_revoke_ANY_DATA_bytes[MAX_ANY_DATA_SAVE_BYTES]; //патченные байты в регионах PATCH_OWNER_VM_SUPPLY15_DATA, PATCH_OWNER_VM_PCODE_DATA, PATCH_OWNER_VM_CONSTANT_DATA
	

	//структура REWRITE
	SERVICE_TYPE Detected_rewrite_abilites[MAX_REWRITE_SITES]; //куда попадает перезапись байт, если присуствует
	SERVICE_ADDRES Rewrite_addres[MAX_REWRITE_SITES]; //начальный адрес перезаписи 
	SERVICE_COUNT Bytes_rewrite[MAX_REWRITE_SITES]; //БАЙТ ПЕРЕЗАПИСАНО
}PATCH_HELD_GROUP, *pPATCH_HELD_GROUP;

typedef struct PATCH_Struct
{
	WCHAR name[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN]; //имя patch-стека
	SERVICE_TYPE owner_as_profile__of_HELD_GROUP;//владелец группы адресов по линии профиля (PATCH_TYPE_OWNERS_ENUMS)
	WCHAR owner_name[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN]; //имя файла владельца patch-стека

	PATCH_HELD_GROUP Containt_HELD_GROUP[MAX_HANHELD_TYPE_DETERMINE_CRITICAL_ADDRESES]; //группа патчинга
	SERVICE_COUNT HELD_GROUP_Fill_count; //количество заполненных HELD_GROUP
	//held common info
	SERVICE_TYPE type_of_target_addres;//тип адреса (ex: из HANHELD_TYPE_ENUMS)
	SERVICE_ADDRES parent_subroutine_address; //адрес функции (саба) в которой находится патч (например для PROFILE_TYPE_VM это HANHELD.HANDELD_Primary_Addres)

	SERVICE_ARRAY_ID i_Containt_handler_ArrayID; //ссылка на HANHELD Containt_handler[VM_HANDLES_MAX_COUNT] в привязанном стеке VM

	bool is_not_applied; //флаг "not applied"
	bool is_new_added; //флаг "только что добавили"
	SERVICE_ERROR last_error_status; //переменная ошибок патчинга
}PATCH_INFO_STACK, *pPATCH_INFO_STACK;


typedef struct PATCH_SUPPORTING_REGION_INI_Struct
{
	WCHAR VMP_SectionName[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN]; //имя секции vmp
	SERVICE_ADDRES VMP_Section_start_address; //адрес секции (RVA)
	size_t VMP_Section_size; //размер секции (в байтах)
	bool is_VMP_section_validate; //подтверждение валидации VMP Section?

	SERVICE_ADDRES Region_VMP_original_copy_start; //адрес начала региона - оригнальной копии vmp секции
	SERVICE_ADDRES Region_VMP_original_copy_end; //адрес конца региона - оригнальной копии vmp секции
	bool is_Region_VMP_original_copy_auto_alloc; //авто выделение региона Region_VMP_original_copy_auto
	bool is_Region_VMP_original_copy_REQ_MEMCPY; //требуется скопировать содержимое VMP_Section в Region_VMP_original_copy?
	bool is_Region_VMP_original_copy_validate; //подтверждение валидации Region_VMP_original_copy?

	SERVICE_ADDRES Region_VMP_patch_map_start; //адрес начала региона - карты патчинга секции
	SERVICE_ADDRES Region_VMP_patch_map_end; //адрес конца региона - карты патчинга секции
	BYTE		   Region_VMP_patch_map_LOCK_byte; //значение LOCK байта (по дефолту 0xFF)
	bool is_Region_VMP_patch_map_auto_alloc; //авто выделение региона Region_VMP_patch_map?
	bool is_Region_VMP_patch_map_validate; //подтверждение валидации Region_vmp_patch_map?

	SERVICE_ADDRES Region_VMP_hook_and_filter_start; //адрес начала региона - служебный для hook's и фильтров vmp
	SERVICE_ADDRES Region_VMP_hook_and_filter_end; //адрес конца региона - служебный для hook's и фильтров vmp
	SERVICE_ADDRES Region_VMP_float_pointer; //плавающий указатель на свободное место
	bool is_Region_VMP_hook_and_filter_auto_alloc; //авто выделение региона Region_VMP_hook_and_filter?
	bool is_Region_VMP_hook_and_filter_validate; //подтверждение валидации Region_VMP_hook_and_filter?

	size_t All_Support_regions_retail_size; //суммарный размер всех дополнительных регионов памяти
}PATCH_SUPPORTING_REGION_INI_STACK, *pPATCH_SUPPORTING_REGION_INI_STACK;

typedef struct PATCH_SUPPORTING_VMPX_PROFILE_INI_Struct
{
	WCHAR Assembly_instruction1[MAX_PATH]; //инструкция 1
	WCHAR Assembly_instruction2[MAX_PATH]; //инструкция 2
	WCHAR Assembly_instruction3[MAX_PATH]; //инструкция 3
	WCHAR Assembly_instruction4[MAX_PATH]; //инструкция 4
	WCHAR Assembly_instruction5[MAX_PATH]; //инструкция 5
	WCHAR Assembly_instruction6[MAX_PATH]; //инструкция 6
	WCHAR Assembly_instruction7[MAX_PATH]; //инструкция 7
	WCHAR Assembly_instruction8[MAX_PATH]; //инструкция 8
	WCHAR Assembly_instruction9[MAX_PATH]; //инструкция 9
	WCHAR Assembly_instruction10[MAX_PATH]; //инструкция 10
	WCHAR Assembly_instruction11[MAX_PATH]; //инструкция 11
	WCHAR Assembly_instruction12[MAX_PATH]; //инструкция 12
	WCHAR Assembly_instruction13[MAX_PATH]; //инструкция 13
	WCHAR Assembly_instruction14[MAX_PATH]; //инструкция 14

	WCHAR Assembly_instruction15[MAX_PATH]; //инструкция 1
	WCHAR Assembly_instruction16[MAX_PATH]; //инструкция 2
	WCHAR Assembly_instruction17[MAX_PATH]; //инструкция 3
	WCHAR Assembly_instruction18[MAX_PATH]; //инструкция 4
	WCHAR Assembly_instruction19[MAX_PATH]; //инструкция 5
	WCHAR Assembly_instruction20[MAX_PATH]; //инструкция 6
	WCHAR Assembly_instruction21[MAX_PATH]; //инструкция 7
	WCHAR Assembly_instruction22[MAX_PATH]; //инструкция 8
	WCHAR Assembly_instruction23[MAX_PATH]; //инструкция 9
	WCHAR Assembly_instruction24[MAX_PATH]; //инструкция 10
	WCHAR Assembly_instruction25[MAX_PATH]; //инструкция 11
	WCHAR Assembly_instruction26[MAX_PATH]; //инструкция 12
	WCHAR Assembly_instruction27[MAX_PATH]; //инструкция 13
	WCHAR Assembly_instruction28[MAX_PATH]; //инструкция 14
	SERVICE_COUNT Assembly_count; //КОЛ-ВО инструкций

	SERVICE_ARRAY_ID Label[10]; //метка для каждой инструкции (1-9)
	bool Control_symbols_is_present[28]; //наличие/отсутствие контрольных символов в строке
	size_t First_control_symbol_offset[28]; //смещение от начала строки первого контрольного символа (байты)
	SERVICE_TYPE Predict_POST_BUILD_Control_symbol_type[28]; //контрольные символы, используемые на последнем этапе сборки (JMP по меткам и возврат в оригинальный код)
	size_t Predict_POST_BUILD_asm_instruction_increment_size[28]; //рассчетные размеры адресов инструкций (для JMP по меткам) с инкрементом
	bool is_request_return_to_original_code; //установлен ли символ возврата в оригинальный код?
}PATCH_SUPPORTING_VMPX_PROFILE_INI, *pPATCH_SUPPORTING_VMPX_PROFILE_INI;

typedef struct PATCH_SUPPORTING_VMPX_PROFILE_INI_STACK_Struct
{
	SERVICE_TYPE KeyType[HANHELD_TYPE_ENUMS_COUNT];
	PATCH_SUPPORTING_VMPX_PROFILE_INI VMPX_HANHELD_TYPE_ENUMS[HANHELD_TYPE_ENUMS_COUNT];
	SERVICE_COUNT Loaded_HANHELDS_FILTERS; //кол-во загруженных фильтров в PATCH_SUPPORTING_VMPX_PROFILE_INI
}PATCH_SUPPORTING_VMPX_PROFILE_INI_STACK, *pPATCH_SUPPORTING_VMPX_PROFILE_INI_STACK;

typedef struct PATCH_LIST_SNAPSHOT
{
	//основополагающие адреса
	SERVICE_ADDRES checkpoint_address1; //адрес оригинальной инструкции (PATCH_OWNER_VM_SUPPLY15_DATA, PATCH_OWNER_VM_PCODE_DATA, PATCH_OWNER_VM_CONSTANT_DATA - начало региона)
	SERVICE_ADDRES checkpoint_address2; //адрес хука (используется только для PATCH_OWNER_VM_OUTSIDE_EXECUTE и PATCH_OWNER_VM_INSIDE_EXECUTE)

	//счетчики
	SERVICE_COUNT  COUNT_of_original_bytes_in_checkpoint1; //кол-во байт для проверки в массиве #1 (original == reworke)
	SERVICE_COUNT  COUNT_of_original_bytes_in_checkpoint2; //кол-во байт для проверки в массиве #2 (original == reworke)
	SERVICE_COUNT  COUNT_all_rewrites_checkpoint1; //кол-во ПЕРЕЗАПИСАННЫХ байт #1
	SERVICE_COUNT  COUNT_all_rewrites_checkpoint2; //кол-во ПЕРЕЗАПИСАННЫХ байт #2

	//типы checkpoints
	SERVICE_TYPE TYPE_of_checkpoint1; //тип байтов чекпоинта #1 (enum PATCH_MAP_REWRITE_DISPLACE_TARGET)
	SERVICE_TYPE TYPE_of_checkpoint2; //тип байтов чекпоинта #1 (enum PATCH_MAP_REWRITE_DISPLACE_TARGET)

	//оригинальные байты
	BYTE original_BYTES_checkpoint1[MAX_ANY_DATA_SAVE_BYTES]; //оригинальные байты checkpoint #1
	BYTE original_BYTES_checkpoint2[MAX_ANY_DATA_SAVE_BYTES]; //оригинальные байты checkpoint #2

	//перезаписанные нами байты
	BYTE reworke_BYTES_checkpoint1[MAX_ANY_DATA_SAVE_BYTES]; //наши байты checkpoint #1
	BYTE reworke_BYTES_checkpoint2[MAX_ANY_DATA_SAVE_BYTES]; //наши байты checkpoint #2
	
	//служебная инфа
	WCHAR name[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN]; //имя patch-стека
	SERVICE_TYPE owner_as_profile__of_HELD_GROUP;//владелец группы адресов по линии профиля (PATCH_TYPE_OWNERS_ENUMS)
	WCHAR owner_name[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN]; //имя файла владельца patch-стека

	//структура REWRITE
	SERVICE_COUNT COUNT_Rewrite_struct; //количество элементов в структуре REWRITE
	SERVICE_TYPE Detected_rewrite_abilites[MAX_REWRITE_SITES]; //куда попадает перезапись байт, если присуствует
	SERVICE_ADDRES Rewrite_addres[MAX_REWRITE_SITES]; //начальный адрес перезаписи 
	SERVICE_COUNT Bytes_rewrite[MAX_REWRITE_SITES]; //БАЙТ ПЕРЕЗАПИСАНО
}PATCH_SNAPSHOT, *pPATCH_SNAPSHOT;

#define MAX_APPROX_SECTION_COUNT 150
typedef struct MAP_IDENTICAL_SECTIONS
{
	PIMAGE_SECTION_HEADER Protection_SECTION_1; //ссылка на секцию защиты в файле №1. Подразумевается для выбора сканирования
	PIMAGE_SECTION_HEADER Protection_SECTION_2; //ссылка на секцию защиты в файле №2. Подразумевается для выбора сканирования

	PIMAGE_SECTION_HEADER IDENTICAL_Section_vpp_1[MAX_APPROX_SECTION_COUNT]; //ссылки на общие секций по первому файлу
	PIMAGE_SECTION_HEADER IDENTICAL_Section_vpp_2[MAX_APPROX_SECTION_COUNT]; //ссылки на общие секций по второму файлу
	WORD NumOfSection_ALL; //ОБЩЕЕ количество одинаковых секций
	ULONGLONG AnyFILE_1_2_IMAGEBASE; //ImageBase одного из файлов. ДОЛЖНА ЮЫТЬ ОДИНАКОВА!!!!!!!!!!!!

	//ДАННЫЕ СРАВНЕНИЯ////////////
	SERVICE_COUNT compare_event_table_count; //кол-во блоков сравнения в таблице результатов

	ULONGLONG   File_1_ImageBase; //ImageBase любого из файлов (должны быть равны)
	IMAGE_SECTION_HEADER File_1_protection_section_header;//секция защиты любого из файлов (должны быть равны)

	bool Option_Align_to_multiple_DWORD_QWORD; //выравнивать кол-во различных байт до размера DWORD - QWORD, если меньше (например, если 3 или 7)
}MAP_IDENTICAL_SECTIONS_STACK, *pMAP_IDENTICAL_SECTIONS_STACK;

//******************************* F U N C! ***************************************
bool DH_File_change_binary_dump(LPWSTR Dest_file, //файл назначения
	LPWSTR Src_binary_dump, //исходный дамп
	size_t Dest_file_raw_offset, //сырое смещение от начала файла
	size_t Size_of_dump_data //размер данных дампа
	); //заменяет данные в файле

bool DH_File_fill_addres_cells(LPWSTR Dest_file, //файл назначения
	LPWSTR Src_binary_dump, //исходный дамп
	UINT64 File_load_Base_Address //базовый адрес загрузки дампа (для вычета)
	); //заполняет указанные ячейки адресами ра


pSET_FILES_COMPARE_EVENT __fastcall DH_PE_Files_compare(
	HMODULE PE_File_1, //дамп PE_файла №1
	HMODULE PE_File_2, //дамп PE_файла №2
	LPBYTE* ppSET_FILES_COMPARE_BYTES_buffer, //выделенная память под дамп различающихся байт
	__in __out pMAP_IDENTICAL_SECTIONS_STACK pIdentSections //служебная структура, информирущая о размерах таблицы сравнения и различных данных сравниваемых PE файлов. Так-же включает опции сравнения
	); //сравнивает файлы, выделяет память под таблицу результатов сравнений и возвращает указатель на неё

__inline ULONG local_find_next_byte_differece_in_DUMP(
	__in LPBYTE *Current_file_DUMP_1_pointer, //первый указатель на дамп файла №1
	__in LPBYTE *Current_file_DUMP_2_pointer, //первый указатель на дамп файла №2
	__in __out SERVICE_COUNT* iteration_count //кол-во оставшихся итераций
	); //находит первое встречное различие в дампах

HMODULE DH_Load_PE_as_datafile(__in LPWSTR PE_File_path); //загружает PE файл в память процесса для исследвоания

PIMAGE_SECTION_HEADER DH_MAP_Get_main_protection_section_point(__in HMODULE hmModule); //найти главную секцию защиты. возвращает указатель на секцию по выделенному hmModule

////////////////////////////////// P * A * T * C * H //////////////////////////////

UINT DH_PATCH_Add_new_vmp_execute_address_patch(
	SERVICE_ADDRES Critical_address_array[], //ссылки на массив адресов для патчинга
	SERVICE_ARRAY_ID Critical_address_relative_addrpool_ID_array[], //ссылки на массив привязки адресов для патчинга - AddrPool
	WCHAR* owner_filename, //имя файла владельца (опционально)
	size_t i_Containt_handler_ArrayID, //ссылка на порядковый ID в HANHELD Containt_handler[VM_HANDLES_MAX_COUNT];
	//SERVICE_ADDRES Parent_subroutine_address, //адрес функции (саба) в которой находится патч (например для PROFILE_TYPE_VM это HANHELD.HANDELD_Primary_Addres) (опционально)
	SERVICE_TYPE Addres_asm_instruction_type, //тип адресов по асм инструкции (ex: mov eax, [rax]; xor al, [rdx])
	SERVICE_TYPE Addres_owner_type_of_profile //обладатель адресов (PATCH_TYPE_OWNERS_ENUMS)
	);//добавляет заготовку нового патча - для выполняемого кода внутри/снаружи VM

UINT DH_PATCH_Add_new_vmp_DATA_address_patch(
	SERVICE_ADDRES Patch_start_coverage_address, //начальный адрес покрытия патча
	size_t Size_Of_patch_data, //размер покрываемых данных
	WCHAR* owner_filename, //имя файла владельца (опционально)
	SERVICE_TYPE Addres_owner_type_of_profile //обладатель адресов (PATCH_TYPE_OWNERS_ENUMS)
	);//добавляет заготовку нового патча - для данных VM

TCHAR* __fastcall DH_VM_PATCH_OWNERS_type_to_string(
	SERVICE_TYPE owner_type //тип PATCH_TYPE_OWNERS_ENUMS
	); //преобразовывает тип PATCH_TYPE_OWNERS_ENUMS в строку


pPATCH_SUPPORTING_REGION_INI_STACK __fastcall DH_MAP_Release_patch_region_config(
	); //реализовывает загруженную конфигурацию по регионам (профиль - PROFILE_TYPE_PATCH_REGIONS_INFO_INI). Обратная функа = DH_MAP_Close_patch_region_config

bool __fastcall DH_MAP_Close_patch_region_config(
	pPATCH_SUPPORTING_REGION_INI_STACK pPatch_stack
	); //закрывает конфигурацию, реализованную функцией DH_MAP_Release_patch_region_config

size_t __fastcall DH_MAP_Do_APPLY_NA_STACK(
	pPATCH_SUPPORTING_REGION_INI_STACK pPatch_stack
	); //Применяет N/A стек для совершения процесса патчинга

void __fastcall DH_MAP_Do_APPLY_NA_STACK__Mark_Patch_MAP(
	SERVICE_ADDRES pDesd_addr, //адрес марикировки
	size_t size_write_data //длина мариковки (не более длины статического внутреннего буфера)
	); //производный DH_MAP_Do_APPLY_NA_STACK. Марикирует patch map

void __fastcall DH_MAP_Do_APPLY_NA_STACK__Mark_Patch_MAP(
	SERVICE_ADDRES pDesd_addr, //адрес марикировки
	size_t size_write_data //длина мариковки (не более длины статического внутреннего буфера)
	); //производный DH_MAP_Do_APPLY_NA_STACK. Марикирует patch map

void __fastcall DH_MAP_Do_APPLY_NA_STACK__ROLL_BACK_Mark_Patch_MAP(
	SERVICE_ADDRES pDesd_addr, //адрес марикировки
	size_t size_write_data //длина мариковки (не более длины статического внутреннего буфера)
	); //производный DH_MAP_Do_APPLY_NA_STACK. ДЕМарикирует patch map. Обратная функа DH_MAP_Do_APPLY_NA_STACK__Mark_Patch_MAP

SERVICE_TYPE __fastcall DH_MAP_Do_APPLY_NA_STACK__Get_BEST_Patch_on_TYPEDEFs_asm(
	__in __out pPATCH_INFO_STACK pPatchArrive, //входной поток инфостека Patch
	__in pVM_AsmAddr_POOL_STACK pVMAddrPoolArrive, //привязанный VM adrr pool
	__in pVM_INFO_STACK pVM_Info_Arrive, //привязанный VM стек
	__in XEDPARSE* pXEDParse //XED
	); //производный DH_MAP_Do_APPLY_NA_STACK. Производит методы патчинга непосредственно инструкций, отвечающих за тип хендла

__forceinline SERVICE_ARRAY_ID __fastcall DH_MAP_Do_APPLY_NA_STACK__Find_TargetAdd_in_AddressPool(
	SERVICE_ADDRES TargetTypedefAddr, //адрес для поиска
	pVM_hanheld_addres_pool pAddrPool_ReqHANHELD //указатель на готовый массив VM_hanheld_addres_pool - берется из SERVICE_ARRAY_ID i_Containt_handler_ArrayID
	); //производный DH_MAP_Do_APPLY_NA_STACK. Отвечает за поиск critical адреса опрееляющего тип "островка" в пуле адресов текущего хендла. Возвращает указатель array ID

bool __fastcall DH_MAP_Init_PATCH_strategy(
	pPATCH_SUPPORTING_REGION_INI_STACK pPatch_stack
	); //инициализирует таблицы патчинга.

pPATCH_SUPPORTING_VMPX_PROFILE_INI_STACK __fastcall DH_MAP_Do_INIT_Arbitary_FILTERs(
	__in 	WCHAR *VMPX_profile_name, //версия VMP - прописная цифра (ex: 2)
	__in	SERVICE_TYPE VMP_Version //версия VMP 2 или 3
	); //инициализирует фильтры перехвата для требуемой версии VMP. Возвращает заполненную структуру

__forceinline pPATCH_SUPPORTING_VMPX_PROFILE_INI_STACK __fastcall DH_MAP_Predict_VMPX_PROFILE_filter(
	__in pPATCH_SUPPORTING_VMPX_PROFILE_INI_STACK pFilterRAWStack //сырой стек профиля фильтра
	);//окончательно преобразовывает и предсказывает стек фильтра с учетом меток и контрольных символов

__forceinline pPATCH_SUPPORTING_VMPX_PROFILE_INI __fastcall DH_MAP_Do_APPLY_NA_STACK__Get_Arbitary_Filter(
	__in __out pPATCH_INFO_STACK pPatchArrive, //ссылка на патч
	__in pVM_INFO_STACK pVM_Info_Arrive //ссылка на VM стек
	); //производный DH_MAP_Do_APPLY_NA_STACK. Отвечает связывает тип хендла с соответсвующим arbitary-фильтром


SERVICE_COUNT __fastcall DH_MAP_Do_APPLY_NA_STACK__Construct_Retail_PATCH_byte_tables(
	__in pPATCH_HELD_GROUP pHELDGroup, //ссылка на текущий юнит патчинга
	__in pVM_INFO_STACK pVM_Info_Arrive, //ссылка на VM
	__in pPATCH_SUPPORTING_VMPX_PROFILE_INI pArbitaryFilterUnit, //ссылка на примененный фильтр
	__in pPATCH_SUPPORTING_REGION_INI_STACK pSupportRegion, //ссылка на регион
	__in XEDPARSE* pXEDParse //XED
	); //производный DH_MAP_Do_APPLY_NA_STACK. Выполняет окончательное построение ArbitaryFilter и строит таблицу байт


SERVICE_COUNT DH_MAP_Do_APPLY_NA_STACK__WritePM_Arbitary_Filter(
	__in pPATCH_INFO_STACK pWritePatch
	); ////производный DH_MAP_Do_APPLY_NA_STACK. Выполняет последннюю фазу - ЗАПИСЬ байт в адресное простратсво процесса

/*
SERVICE_COUNT __fastcall DH_MAP_Do_APPLY_NA_STACK__Remap_and_refresh_asm_pool(
	__in __out BYTE* pAsmPool, //буфер с асм инстуркциями
	__in size_t PoolSize, //размер пула
	__in SERVICE_ADDRES Source_CIP, //начальный CIP-адрес источника
	__in SERVICE_ADDRES Source_VA, //Virtual Addres источника
	__in SERVICE_ADDRES Desd_CIP, //начальный CIP-адрес НАЗНАЧЕНИЯ
	__in XEDPARSE* pXEDParse //XED
	);*/

SERVICE_COUNT __fastcall DH_MAP_Do_APPLY_NA_STACK__Remap_and_refresh_asm_pool(
	//__in __out BYTE* pAsmPool,  //буфер с асм инстуркциями
	__in SERVICE_COUNT AsmCount, //количество инструкций в буфере
	__in PDISASM pFILL_Dissasm, //изначально заполненная структура DISASM
	__in XEDPARSE* pFILL_XEDParse //изначально заполненная структура XED
	);//производный DH_MAP_Do_APPLY_NA_STACK. Выполняет ремаппинг и обновление displace-инструкций

SERVICE_COUNT __fastcall DH_MAP_Take_patch_list_snapshot(
	__in __out pPATCH_SNAPSHOT pPatch_Snapshot //snapshot список примененных патчей
	);//получает snapshot-список online-патчей

pPATCH_SNAPSHOT __fastcall DH_MAP_AntiRewrite_Sentinel(
	__in pPATCH_SNAPSHOT pSnapList, //полный список снапшотов
	__in SERVICE_COUNT* pSnapListCount, //ссылка на количество элементов в списке (ACCEPT SnapListCount > 0). Если найдено различие, возвращает в ссылке сколько осталось перебрать (INC, ++) до конца списка с первого попавшегося
	__out SERVICE_COUNT* pAllRewritesSnapsCOUNT //общее количество снапов с перезаписями
	); //AntiRewrite - часовой. Возвращает ссылку на первым измененный снапшот из списка с последующим инкерментом (INC, ++) значения

SERVICE_COUNT __fastcall DH_MAP_AntiRewrite_Sentinel__compare_two_dumps(
	__in PBYTE Dump1, //ссылка на дамп №1
	__in PBYTE Dump2, //ссылка на дамп №2
	__in SERVICE_COUNT DumpCount //размер проверяемой области
	//__in __out SERVICE_ADDRES* pF1rstDiff_Addr, //принимает начальный адрес. Корректирует его в соотверствии с первым найденным различием
	//__in __out SERVICE_COUNT* pF1rstDiff_Count //принимает ссылку на количество различных байт. Записывает значение ПРИ первом найденном различии
	);  //производная от DH_MAP_AntiRewrite_Sentinel. Сравнивает два дампа с одним размером. Возвращает количество различных байт до первого сходства

 TCHAR* __fastcall DH_PATCH_ERROR_type_to_string(
	 __in SERVICE_ERROR Err_type
	 ); //конвертирует тип ошибки патчинга в строковый вариант В соотверствии с enum PATCH_ERROR_STATUS

   TCHAR* __fastcall DH_PATCH_ReWRITE_type_to_string(
	   __in SERVICE_ERROR Rewrite_type
	   ); //конвертирует тип ошибки ПЕРЕЗАПИСИ (ReWrite) в строковый вариант В соотверствии с enum PATCH_MAP_REWRITE_DISPLACE_TARGET

   char* __fastcall DH_MAP_Determine_identical_protection_section(
	   __in HMODULE PE_File_1, //ссылка на сравниваемый файл №1
	   __in HMODULE PE_File_2, //ссылка на сравниваемый файл №2
	   __out pMAP_IDENTICAL_SECTIONS_STACK pIdentSections //ссылка на стек из данных по одинаковым секциям
	); //определить общую секцию, наиболее подходящую под секцию защиты из двух PE-файлов. При положительном результате возвращает ссылку на имя защищенной секции в PE-струтуре IMAGE_SECTION_HADER

   char* __fastcall DH_MAP_Verify_and_set_protection_section(
	   __in char* pSource_Section_name, //желаемое имя секции
	   __in pMAP_IDENTICAL_SECTIONS_STACK pIdentSections //
	); //выполняет проверку заданной пользователем секции в списке доступных к сравнению секцию. Инициализирует данные для поиска. При положительном результате возвращает ссылку на имя защищенной секции в PE-струтуре IMAGE_SECTION_HADER