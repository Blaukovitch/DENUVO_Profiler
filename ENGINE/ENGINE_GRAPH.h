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
#include "ENGINE_Profile_operations.h"
#include "ENGINE_VM_operations.h"

//------------------ define data ------------------ 
#define SPIRUN7_LIB_NAME _T("xa.dll")
#define SPIRUN7_FUNC_intrecept_main "fnxa" //главная функа init
#define SPIRUN7_DATA_VMP2_INTRECEPT_TABLE "XA_VMP2_INTRECEPT_TABLE"
#define SPIRUN7_DATA_VMP2_ORIGINAL_TABLE "XA_VMP2_ORIGINAL_TABLE"
#define SPIRUN7_DATA_VMP2_HANHELD_ENTER_TRACE_LOG_Addres "XA_VM_HANHELD_ENTER_TRACE_LOG_Addres"
#define SPIRUN7_DATA_VMP2_HANHELD_ENTER_TRACE_LOG_flow_pointer "XA_VM_HANHELD_ENTER_TRACE_LOG_flow_pointer"

#define SPIRUN7_HANHELD_ID_dump_pointer_type WORD
#define SPIRUN7_HANHELD_ID_dump_LP_pointer LPWORD
#define SPIRUN7_HANHELD_ID_dump_sizeof sizeof(SPIRUN7_HANHELD_ID_dump_pointer_type)


#define SAFE_CODE_OFFSET 0x100
#define SIZEOF_MOV_RCX 10

#define VM_HANDLES_SPIRUN7_LIMIT 262

#define RCX_OPERAND_OFFSET 3
#define RAX_OPERAND_OFFSET 13

#define SPURUN7_DUMP_AREA_SIZE 0x100000 //размер поля дампа

#define SPURUN7_LIMIT_GUI_LIST_SHOW_LATEST_HANHELDS 200 //GUI ClistControl - предел по показу последних вызванных hanhelds

#define SPIRUN7_MAX_HANHELD_COMPARED_SHOW 10 //сколько записывать хендлов в кластера при сравнении (до первого дифферента/после первого дифферента)

//def
/*
typedef struct SPIRUN7_Inject_Pool
{
	LPCVOID x_DLL_Path_Alloc_Adress;
	LPCVOID x_Start_Thread_Addr;
	LPCVOID Operand_Load_LibraryW_RVA;
	HANDLE A_Create_Thread_handle;
	ULONG A_Create_Thread_ID;
}IJ_P,*pIJ_P;
*/
//++++++++++++++++++++++ structs ++++++++++++++++++++++

typedef struct SPIRUN7_GUI_TRACE_LOG_LIST_SHOW_INFORMATION
{
	//общее количество
	SERVICE_COUNT Total_HANHELD_calls_count; //результирующий счетчик кол-ва вызовов в регионе дампа
	SERVICE_COUNT Total_PCODE_shift; //результирующий счетчик по сдвигу ленту p-code (в байтах)
	signed long	  Current_RBP_different; //текущий дифферент по регистру-указателю на V-stack (стек данных)
	//последние вызовы
	SERVICE_COUNT Last__Fill_HANHELDs_count; //кол-во заполненных указателей (на случай когда < SPURUN7_LIMIT_GUI_LIST_SHOW_LATEST_HANHELDS)
	pHANHELD pLast__Called_HANHELD[SPURUN7_LIMIT_GUI_LIST_SHOW_LATEST_HANHELDS]; //указатели на pVM_INFO_STACK.HANHELD
}SPIERUN7_INFO_CLUSTER_TRACE_LIST,*pSPIERUN7_INFO_CLUSTER_TRACE_LIST; //кластер собранной информации TRACE LOG vmp2 (ранжировка от 0 -> SPURUN7_LIMIT_GUI_LIST_SHOW_LATEST_HANHELDS = ранний -> поздний вызов)

typedef struct SPIRUN7_GUI_POPULAR_LIST_SHOW_INFORMATION
{
	SERVICE_COUNT Fill_HANHELDs_count; //кол-во заполненных указателей (на случай когда < VM_HANDLES_MAX_COUNT)
	pHANHELD pPopular_HANHELD[VM_HANDLES_MAX_COUNT]; //указатели на pVM_INFO_STACK.HANHELD
	SERVICE_COUNT pPopular_HANHELD_calls_count[VM_HANDLES_MAX_COUNT]; //счетчик кол-ва вызовов
}SPIERUN7_INFO_CLUSTER_POPULAR_LIST,*pSPIERUN7_INFO_CLUSTER_POPULAR_LIST; //кластер собранной информации POPULAR TRACE LIST HANHELDs(ранжировка от 0 -> VM_HANDLES_MAX_COUNT = наибольшее кол-во вызовов -> наименьшее)


typedef struct DH_SPIRUN7_TRACELOG_VMP2_INFO_STACK_information_struct //информация о трейс-логе VMP2
{
	WCHAR vmp2_name[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN]; //имя виртуальной машины
	SERVICE_HASH HANHELD_ID_hash; //хэш сдампленных ID хендлов
	SPIERUN7_INFO_CLUSTER_TRACE_LIST TraceList_vmp2; //кластер собранной информации TRACE LOG vmp2 (ранжировка от 0 -> SPURUN7_LIMIT_GUI_LIST_SHOW_LATEST_HANHELDS = ранний -> поздний вызов)
	SPIERUN7_INFO_CLUSTER_POPULAR_LIST TracePopular_vmp2; //кластер собранной информации POPULAR TRACE LIST HANHELDs(ранжировка от 0 -> VM_HANDLES_MAX_COUNT = наибольшее кол-во вызовов -> наименьшее)
	SERVICE_COUNT ofHANHELD_ID_dump_size; //размер дампа
	SERVICE_OFFSET ofHANHELD_ID_dump; //оффсет дампа (должен быть под этой структурой)
}SPIRUN7_TRACELOG_VMP2_INFO_STACK, *pSPIRUN7_TRACELOG_VMP2_INFO_STACK;


typedef struct DH_SPIRUN7_TRACELOG_Cmp_record //запись об одном компоненте для сравнения
{
	HANHELD_ADDRES Hanhled_addr; //адрес хендла
	WCHAR* Type_string; //ссылка на строку с типом
	SERVICE_COUNT How_count; //кол-во повторений на данный момент
}DH_SPIRUN7_TRACELOG_HANHELD_COMPARE_RECORD, *pDH_SPIRUN7_TRACELOG_HANHELD_COMPARE_RECORD;

typedef struct DH_SPIRUN7_TRACELOG_VMP2_COMPARE_INFO_CLUSTER //кластер информации о сравнении двух трейсологов vmp2
{
	SERVICE_COUNT Equal_hahelds_until_first_different; //кол-во одинаковых хендлов до 1-первого дифферента
	bool Is_compares_have_equal_size; //дампы идеют одинаковый размер данных для сравнения??
	
	DH_SPIRUN7_TRACELOG_HANHELD_COMPARE_RECORD Equal_hahelds[SPIRUN7_MAX_HANHELD_COMPARED_SHOW]; //одинаковые хендлы до первого дифферента
	SERVICE_COUNT Equal_hahelds_count;

	DH_SPIRUN7_TRACELOG_HANHELD_COMPARE_RECORD Different_hahelds_1[SPIRUN7_MAX_HANHELD_COMPARED_SHOW]; //разные хендлы для входного контейнера 1
	SERVICE_COUNT Different_hahelds_1_count;

	DH_SPIRUN7_TRACELOG_HANHELD_COMPARE_RECORD Different_hahelds_2[SPIRUN7_MAX_HANHELD_COMPARED_SHOW]; //разные хендлы для входного контейнера 2
	SERVICE_COUNT Different_hahelds_2_count;
}SPIRUN7_TRACELOG_VMP2_COMPARE_INFO_CLUSTER, *pSPIRUN7_TRACELOG_VMP2_COMPARE_INFO_CLUSTER;

extern BYTE ASM_MOV_Register_RCX[2];
extern BYTE ASM_CALL_CONST;
extern BYTE ASM_NOP;
extern BYTE ASM_CALL_QWORD_PTR_DS[3];
extern BYTE ASM_RETN;
extern BYTE ASM_ADD_RSP_30[];
extern BYTE Inject_EXECUTE_CODE[];


 
// $$$$$$$$$$$$$$$$$$$$$$$$$ enums $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
enum INTRECEPT_ONBJECT_ENUM {IOBG_VMP2_SUPPLY15_TRACE_LOGGER = 1, IOBG_VMP3_HANHELD_COLLECTOR, IOBG_BETWEEN_VMs_PROBE};

enum INTRECEPT_COMMAND_ENUM {ICMD_VMP2_APLLY_INTRECEPT_SUPPLY15 = 1, ICMD_VMP2_RESTORE_ORIGINAL_SUPPLY15};

 // ------------------------- SUBs ------------------------- 
extern bool DH_SPIRUN_Injectx64(__in HANDLE Target_Process); //Открывает процедуру ВНЕДРЕНИЯ SPIRUN7_LIB_NAME
extern bool DH_SPIRUN_Close_Injectx64(__in HANDLE Target_Process); //завершает процедуру ВНЕДРЕНИЯ SPIRUN7_LIB_NAME
extern bool DH_SPIRUN_Init(__in __out pSPIRUN7_BLOCK pSPIRUN7_dll_info);//инициализация SPIRUN7
extern pVM_INFO_STACK DH_SPIRUN_Init_assign_vm_profile(__in TCHAR* assign_vm_name);//доп. инициализация перехватываемого профиля vm
extern bool DH_SPIRUN_Intrecept_Commander_Tower(
	__in SERVICE_TYPE Intrecept_object, //тип объекта перехвата от INTRECEPT_ONBJECT_ENUM
	__in SERVICE_TYPE Intrecept_command, //команда перехвата от INTRECEPT_COMMAND_ENUM
	__in pVM_INFO_STACK Selected_vmp_stack //vmp стек
	); //командный узел по перехвату

bool DH_SPIRUN_Sentry_for_HANHELD_ID_dumping_region(
	__in pVM_INFO_STACK Selected_vmp_stack, //выбранный профиль виртуальной машины
	__in __out pSPIERUN7_INFO_CLUSTER_TRACE_LIST TraceListCluster, //указатель на кластер вывода ключевой информации: количественные хар-ки и последние вызовы
	__in __out pSPIERUN7_INFO_CLUSTER_POPULAR_LIST PopularListCluster,//указатель на кластер вывода информации о популярных вызовах HANHELD
	__in __out TARGET_PROC_REGION_COPY* HANHELD_ID_dumping_region //обновление в структуре о регионе
	); //наблюдатель и анализатор за состоянием дампа HANHELD_ID_dumping_region

bool DH_SPIRUN_Set_restore_mode(
	__in bool Restore_flag_state, //состояние флага "Restore"
	 __in TCHAR* Save_sel_VM_name //имя сохраняемого профиля виртуальной машины
	); //устанавливает режим восстановления состояния tracemode

bool DH_SPIRUN_Get_restore_mode(
	__in TCHAR* Save_sel_VM_name //восстанавливаемый профиль VM
	); //возвращает режим состояния tracemode

extern pSPIRUN7_TRACELOG_VMP2_INFO_STACK DH_PROFILE_SPIRUN7_Pack_save_profile_vmp2(
	__in pSPIERUN7_INFO_CLUSTER_TRACE_LIST pCluster_tracelist_tracep, //общий кластер, последние 200 вызовов
	__in pSPIERUN7_INFO_CLUSTER_POPULAR_LIST pCluster_popular_list_tracep,  //кластер популяного списка
	__in TARGET_PROC_REGION_COPY* pHANHELD_ID_dumping_region, //регион с дампом HAHHELD IDs
	__in WCHAR* vmname, //имя привязанной vmp2
	__in __out SIZE_T* ByteOuts //результирующий размер сохраняемой структуры
	); //упаковывает данные для профиля "trace log vmp2"

void DH_SPIRUN_Set_initial_HANHELD_ID_dumping_region(); //начальное состояние для нового дампа HANHELD_ID_dumping_region

ULONG64 DH_PROFILE_SPIRUN7_compare_vmp2_containers(
	__in pSPIRUN7_TRACELOG_VMP2_INFO_STACK Compare1, //сравниваемый дамп №1
	__in pSPIRUN7_TRACELOG_VMP2_INFO_STACK Compare2, //сравниваемый дамп №2
	__out pSPIRUN7_TRACELOG_VMP2_COMPARE_INFO_CLUSTER Compare_Results //результаты сравнения
	);

SERVICE_TYPE DH_PROFILE_SPIRUN7_TRANSLATE_VMP2_TRACE_LOG_HANHELD_ID(
	__in SPIRUN7_HANHELD_ID_dump_pointer_type Hanheld_ID_Log, //траслируемый HANHELD_ID
	__in pVM_INFO_STACK VMP2_Info_Stack, //профиль виртуальной машины
	__out pDH_SPIRUN7_TRACELOG_HANHELD_COMPARE_RECORD Record_compare_cluster //кластер для траслируемой информации
	); //транслирует HAHNHELD_ID в информационный кластер с учетом загруженного профиля виртуальной машины


HANHELD_ADDRES DH_PROFILE_SPIRUN7_TRANSLATE_VMP2_TRACE_LOG_HANHELD_ID_Address_only(
	__in SPIRUN7_HANHELD_ID_dump_pointer_type Hanheld_ID_Log, 
	__in pVM_INFO_STACK VMP2_Info_Stack
	);//транслирует HAHNHELD_ID и возвращает АДРЕС хендла с учетом загруженного профиля виртуальной машины


SERVICE_COUNT DH_PROFILE_SPIRUN7_Get_Corrent_count_from_cluster(
	__in SPIRUN7_HANHELD_ID_dump_pointer_type Hanheld_ID_Log, 
	__in pVM_INFO_STACK VMP2_Info_Stack, 
	__in pSPIERUN7_INFO_CLUSTER_POPULAR_LIST Cluster
	); //возвращает корректное количество SERVICE_COUNT из кластера с учетом дублирующихся адресов в vmp2 стеке