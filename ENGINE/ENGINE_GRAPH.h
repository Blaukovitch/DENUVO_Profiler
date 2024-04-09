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
#define SPIRUN7_FUNC_intrecept_main "fnxa" //������� ����� init
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

#define SPURUN7_DUMP_AREA_SIZE 0x100000 //������ ���� �����

#define SPURUN7_LIMIT_GUI_LIST_SHOW_LATEST_HANHELDS 200 //GUI ClistControl - ������ �� ������ ��������� ��������� hanhelds

#define SPIRUN7_MAX_HANHELD_COMPARED_SHOW 10 //������� ���������� ������� � �������� ��� ��������� (�� ������� ����������/����� ������� ����������)

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
	//����� ����������
	SERVICE_COUNT Total_HANHELD_calls_count; //�������������� ������� ���-�� ������� � ������� �����
	SERVICE_COUNT Total_PCODE_shift; //�������������� ������� �� ������ ����� p-code (� ������)
	signed long	  Current_RBP_different; //������� ��������� �� ��������-��������� �� V-stack (���� ������)
	//��������� ������
	SERVICE_COUNT Last__Fill_HANHELDs_count; //���-�� ����������� ���������� (�� ������ ����� < SPURUN7_LIMIT_GUI_LIST_SHOW_LATEST_HANHELDS)
	pHANHELD pLast__Called_HANHELD[SPURUN7_LIMIT_GUI_LIST_SHOW_LATEST_HANHELDS]; //��������� �� pVM_INFO_STACK.HANHELD
}SPIERUN7_INFO_CLUSTER_TRACE_LIST,*pSPIERUN7_INFO_CLUSTER_TRACE_LIST; //������� ��������� ���������� TRACE LOG vmp2 (���������� �� 0 -> SPURUN7_LIMIT_GUI_LIST_SHOW_LATEST_HANHELDS = ������ -> ������� �����)

typedef struct SPIRUN7_GUI_POPULAR_LIST_SHOW_INFORMATION
{
	SERVICE_COUNT Fill_HANHELDs_count; //���-�� ����������� ���������� (�� ������ ����� < VM_HANDLES_MAX_COUNT)
	pHANHELD pPopular_HANHELD[VM_HANDLES_MAX_COUNT]; //��������� �� pVM_INFO_STACK.HANHELD
	SERVICE_COUNT pPopular_HANHELD_calls_count[VM_HANDLES_MAX_COUNT]; //������� ���-�� �������
}SPIERUN7_INFO_CLUSTER_POPULAR_LIST,*pSPIERUN7_INFO_CLUSTER_POPULAR_LIST; //������� ��������� ���������� POPULAR TRACE LIST HANHELDs(���������� �� 0 -> VM_HANDLES_MAX_COUNT = ���������� ���-�� ������� -> ����������)


typedef struct DH_SPIRUN7_TRACELOG_VMP2_INFO_STACK_information_struct //���������� � �����-���� VMP2
{
	WCHAR vmp2_name[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN]; //��� ����������� ������
	SERVICE_HASH HANHELD_ID_hash; //��� ����������� ID �������
	SPIERUN7_INFO_CLUSTER_TRACE_LIST TraceList_vmp2; //������� ��������� ���������� TRACE LOG vmp2 (���������� �� 0 -> SPURUN7_LIMIT_GUI_LIST_SHOW_LATEST_HANHELDS = ������ -> ������� �����)
	SPIERUN7_INFO_CLUSTER_POPULAR_LIST TracePopular_vmp2; //������� ��������� ���������� POPULAR TRACE LIST HANHELDs(���������� �� 0 -> VM_HANDLES_MAX_COUNT = ���������� ���-�� ������� -> ����������)
	SERVICE_COUNT ofHANHELD_ID_dump_size; //������ �����
	SERVICE_OFFSET ofHANHELD_ID_dump; //������ ����� (������ ���� ��� ���� ����������)
}SPIRUN7_TRACELOG_VMP2_INFO_STACK, *pSPIRUN7_TRACELOG_VMP2_INFO_STACK;


typedef struct DH_SPIRUN7_TRACELOG_Cmp_record //������ �� ����� ���������� ��� ���������
{
	HANHELD_ADDRES Hanhled_addr; //����� ������
	WCHAR* Type_string; //������ �� ������ � �����
	SERVICE_COUNT How_count; //���-�� ���������� �� ������ ������
}DH_SPIRUN7_TRACELOG_HANHELD_COMPARE_RECORD, *pDH_SPIRUN7_TRACELOG_HANHELD_COMPARE_RECORD;

typedef struct DH_SPIRUN7_TRACELOG_VMP2_COMPARE_INFO_CLUSTER //������� ���������� � ��������� ���� ����������� vmp2
{
	SERVICE_COUNT Equal_hahelds_until_first_different; //���-�� ���������� ������� �� 1-������� ����������
	bool Is_compares_have_equal_size; //����� ����� ���������� ������ ������ ��� ���������??
	
	DH_SPIRUN7_TRACELOG_HANHELD_COMPARE_RECORD Equal_hahelds[SPIRUN7_MAX_HANHELD_COMPARED_SHOW]; //���������� ������ �� ������� ����������
	SERVICE_COUNT Equal_hahelds_count;

	DH_SPIRUN7_TRACELOG_HANHELD_COMPARE_RECORD Different_hahelds_1[SPIRUN7_MAX_HANHELD_COMPARED_SHOW]; //������ ������ ��� �������� ���������� 1
	SERVICE_COUNT Different_hahelds_1_count;

	DH_SPIRUN7_TRACELOG_HANHELD_COMPARE_RECORD Different_hahelds_2[SPIRUN7_MAX_HANHELD_COMPARED_SHOW]; //������ ������ ��� �������� ���������� 2
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
extern bool DH_SPIRUN_Injectx64(__in HANDLE Target_Process); //��������� ��������� ��������� SPIRUN7_LIB_NAME
extern bool DH_SPIRUN_Close_Injectx64(__in HANDLE Target_Process); //��������� ��������� ��������� SPIRUN7_LIB_NAME
extern bool DH_SPIRUN_Init(__in __out pSPIRUN7_BLOCK pSPIRUN7_dll_info);//������������� SPIRUN7
extern pVM_INFO_STACK DH_SPIRUN_Init_assign_vm_profile(__in TCHAR* assign_vm_name);//���. ������������� ���������������� ������� vm
extern bool DH_SPIRUN_Intrecept_Commander_Tower(
	__in SERVICE_TYPE Intrecept_object, //��� ������� ��������� �� INTRECEPT_ONBJECT_ENUM
	__in SERVICE_TYPE Intrecept_command, //������� ��������� �� INTRECEPT_COMMAND_ENUM
	__in pVM_INFO_STACK Selected_vmp_stack //vmp ����
	); //��������� ���� �� ���������

bool DH_SPIRUN_Sentry_for_HANHELD_ID_dumping_region(
	__in pVM_INFO_STACK Selected_vmp_stack, //��������� ������� ����������� ������
	__in __out pSPIERUN7_INFO_CLUSTER_TRACE_LIST TraceListCluster, //��������� �� ������� ������ �������� ����������: �������������� ���-�� � ��������� ������
	__in __out pSPIERUN7_INFO_CLUSTER_POPULAR_LIST PopularListCluster,//��������� �� ������� ������ ���������� � ���������� ������� HANHELD
	__in __out TARGET_PROC_REGION_COPY* HANHELD_ID_dumping_region //���������� � ��������� � �������
	); //����������� � ���������� �� ���������� ����� HANHELD_ID_dumping_region

bool DH_SPIRUN_Set_restore_mode(
	__in bool Restore_flag_state, //��������� ����� "Restore"
	 __in TCHAR* Save_sel_VM_name //��� ������������ ������� ����������� ������
	); //������������� ����� �������������� ��������� tracemode

bool DH_SPIRUN_Get_restore_mode(
	__in TCHAR* Save_sel_VM_name //����������������� ������� VM
	); //���������� ����� ��������� tracemode

extern pSPIRUN7_TRACELOG_VMP2_INFO_STACK DH_PROFILE_SPIRUN7_Pack_save_profile_vmp2(
	__in pSPIERUN7_INFO_CLUSTER_TRACE_LIST pCluster_tracelist_tracep, //����� �������, ��������� 200 �������
	__in pSPIERUN7_INFO_CLUSTER_POPULAR_LIST pCluster_popular_list_tracep,  //������� ���������� ������
	__in TARGET_PROC_REGION_COPY* pHANHELD_ID_dumping_region, //������ � ������ HAHHELD IDs
	__in WCHAR* vmname, //��� ����������� vmp2
	__in __out SIZE_T* ByteOuts //�������������� ������ ����������� ���������
	); //����������� ������ ��� ������� "trace log vmp2"

void DH_SPIRUN_Set_initial_HANHELD_ID_dumping_region(); //��������� ��������� ��� ������ ����� HANHELD_ID_dumping_region

ULONG64 DH_PROFILE_SPIRUN7_compare_vmp2_containers(
	__in pSPIRUN7_TRACELOG_VMP2_INFO_STACK Compare1, //������������ ���� �1
	__in pSPIRUN7_TRACELOG_VMP2_INFO_STACK Compare2, //������������ ���� �2
	__out pSPIRUN7_TRACELOG_VMP2_COMPARE_INFO_CLUSTER Compare_Results //���������� ���������
	);

SERVICE_TYPE DH_PROFILE_SPIRUN7_TRANSLATE_VMP2_TRACE_LOG_HANHELD_ID(
	__in SPIRUN7_HANHELD_ID_dump_pointer_type Hanheld_ID_Log, //������������ HANHELD_ID
	__in pVM_INFO_STACK VMP2_Info_Stack, //������� ����������� ������
	__out pDH_SPIRUN7_TRACELOG_HANHELD_COMPARE_RECORD Record_compare_cluster //������� ��� ������������ ����������
	); //����������� HAHNHELD_ID � �������������� ������� � ������ ������������ ������� ����������� ������


HANHELD_ADDRES DH_PROFILE_SPIRUN7_TRANSLATE_VMP2_TRACE_LOG_HANHELD_ID_Address_only(
	__in SPIRUN7_HANHELD_ID_dump_pointer_type Hanheld_ID_Log, 
	__in pVM_INFO_STACK VMP2_Info_Stack
	);//����������� HAHNHELD_ID � ���������� ����� ������ � ������ ������������ ������� ����������� ������


SERVICE_COUNT DH_PROFILE_SPIRUN7_Get_Corrent_count_from_cluster(
	__in SPIRUN7_HANHELD_ID_dump_pointer_type Hanheld_ID_Log, 
	__in pVM_INFO_STACK VMP2_Info_Stack, 
	__in pSPIERUN7_INFO_CLUSTER_POPULAR_LIST Cluster
	); //���������� ���������� ���������� SERVICE_COUNT �� �������� � ������ ������������� ������� � vmp2 �����