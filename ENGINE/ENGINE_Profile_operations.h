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
//������ �� ����� ����� �������
#define PROFILE_READFILE_NAME L"DH_Profile.txt" //��� �����, ������������ � ���������� 
#define PROFILE_READFILE_CONTENT L"This folder containt profiles for DH!" //����� � ��

//���������� �������
//#define PROFILE_VM_nfo_CONTAINT_FILE_NAME L"VM.spr" //��� ����� ������ VM
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

#define PROFILE_IMPORT_nfo_CONTAINT_FILE_NAME L"IMPORT.spr" //��� ����� ������ ������� ��������

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

//�������
#define PROFILE_READ_ALL -1 //������ ��� ������ � �����
#define PROFILE_FIRST_FILE -1 //������ ������ ����� � ����������

//����������� ���������� ���-�� ������ ������ ����������
#define MAX_CONTAINTER_FILES 1000



//������� ����
#define A_RETURN_ORIGINAL_PROFLE_PATH HUNT_HIVE.PROFILE_PATH[HUNT_HIVE.PROFILE_PATH_WCSLEN] = (WCHAR)L'\0'
#define A_RETURN_ORIGINAL_VMPX_PROFLE_PATH HUNT_PROFILE_HIVE.VMPX_PROFILE_PATH[HUNT_PROFILE_HIVE.VMPX_PROFILE_PATH_WCSLEN] = (WCHAR)L'\0'
#define A_RETURN_ORIGINAL_PROFLE_PATH_MT_copy(tstr_0ref) tstr_0ref[HUNT_HIVE.PROFILE_PATH_WCSLEN] = (WCHAR)L'\0'
#define A_RETURN_ORIGINAL_TEMPORARY_PATH(end) TMP_Profle_Folder[end] = (WCHAR)L'\0'
#define A_INIT_BUFFER_PROFILE_STRING wcscpy(&HUNT_HIVE.PROFILE_PATH[0],Profile_path); wcscat(&HUNT_HIVE.PROFILE_PATH[0],Target_Process->ProcessName); wcscat(&HUNT_HIVE.PROFILE_PATH[0],L"\\\0")
#define A_INIT_TEMP_BUFFER_STRING wcscpy(&TMP_Profle_Folder[0],Profile_path); wcscat(&TMP_Profle_Folder[0],Target_Process->ProcessName)

//��� �������
#define PROFILE_TYPE_COMMON_NFO 0 //��� - ����� ����
#define PROFILE_TYPE_VM 1 //��� - ����������� ������
#define PROFILE_TYPE_IMPORT 2 //��� - ������ �������
#define PROFILE_TYPE_PATCH__NOT_APPLIED_FLAG 3 //��� - ������ �������� ������� � ������ "not_appied"
#define PROFILE_TYPE_PATCH_ONLINE 4 //��� - ������ ������������ �������� �������
#define PROFILE_TYPE_LIC_DATA_DUMP 5//��� - ������� ������ ��� ��������(PEB, IMAGE_DATA_DIRECTORY, KUSER...)
#define PROFILE_TYPE_VM_ADDRES_POOL 6 //��� - ��� ������� ������� ����������� ������
#define PROFILE_TYPE_TRACE_VMP2_LOG 7 //��� - ��� ������ ������ �� vmp2
#define PROFILE_TYPE_PATCH_REGIONS_INFO_INI 8 //��� - ��������������� INI-���� -> ���������� � �������� ������, ��������������� ��� ��������
#define PROFILE_TYPE_PATCH_FILTER_ASM_STACK_INI 9 //��� - ��������������� INI-���� -> ���� ���������� �� �������� (��� ��������)
#define PROFILE_TYPE_VM_VMPX_CONTEXT_INI 10 //��� - ��������������� INI-���� -> ���� ���������� ����������� ������ (��� ������)
#define PROFILE_TYPE_VM_BILLET 11 //��� - ��������� (����� ����������) ��� PROFILE_TYPE_VM. �������� ������ � VM_ENTER ��� ������� ��������� � ��������� ����������� ����������� ������. ������������ ��� �������������
#define PROFILE_TYPE_PIRATE_MAP 12 //��� - ��������� �����(tmp) �� ���������� �������

//extern WCHAR CURRENT_Profile[MAX_PATH*2]; //������ �������
//extern ULONG CURRENT_Profile_len; //����� �������

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
extern bool DH_InitProfiles_Engine_Path( __out LPWSTR Profile_path); //������������� ����� �������� (true/false)
extern bool DH_Is_HAVE_hunt_Profile( __in pDH_ProcInfo Target_Process,  __in LPWSTR Profile_path); //��������� ���������� �� ������� (true/false)
extern ULONG DH_SetCurrentHuntProfile( __in pDH_ProcInfo Target_Process,  __in  LPWSTR Profile_path); //������������� ������� ������� (return ����� WCHAR ������)
extern ULONG DH_GetCurrentHuntProfile( __out LPWSTR Profile_path); //���������� ������� ������� (���������� ����� ������)
extern bool DH_CreateNewHuntProfile( __in pDH_ProcInfo Target_Process,  __in LPWSTR Profile_path); //������� ����� hunt ������� (true/false)
extern void DH_Profile_Verify_Folders(__in UINT PROFILE_x_TYPE);  //��������� �����, ������� �����������
extern bool DH_Write_Current_hunt_profile_CONTAINER_savedlg(__in UINT PROFILE_x_TYPE, //��� ����������
	__in BYTE* write_buff, //����� ������
	__in size_t num_bytes_to_write //���-�� ���� ������  sizeof(��������� ����������)
		);//save CFileDlg dlg - ��� ���������� �����

extern PVOID DH_Read_Current_hunt_profile_CONTAINER(
	__in UINT PROFILE_x_TYPE, //��� ����������
	__in WCHAR* PROFILE_x_CONTAINT_FILE_NAME, //��� ����� ���������� (
	__in size_t num_bytes_of_read, //���-�� ���� ��� ������  (PROFILE_READ_ALL ��� �����)
	__out PULONG Return_Readed_Bytes //���-��, ������� ���� ������� ���������
	);//������ ���������� ����������. (���������� ����� � ������� ������)

bool DH_Write_Current_hunt_profile_CONTAINER(
	__in UINT PROFILE_x_TYPE, //��� ����������
	__in WCHAR* PROFILE_x_CONTAINT_FILE_NAME,  //��� ����� ����������
	__in BYTE* write_buff, //����� ������
	__in size_t num_bytes_to_write, //���-�� ���� ������  sizeof(��������� ����������)
	__out PULONG Return_Writed_Bytes);  //���������� ������ HIVE ������� � �����

extern bool DH_Test_Current_hunt_profile_CONTAINER(
	__in UINT PROFILE_x_TYPE, //��� ����������
	__in WCHAR* PROFILE_x_CONTAINT_FILE_NAME //��� ����� ����������
	); //�������� ���������� ����������. (true-false)

extern SERVICE_COUNT DH_Get_Current_hunt_profile_ALL_FILE_CONTAINERS(
	__in UINT PROFILE_x_TYPE, //��� ����������
	__out DH_ENUM_FILES_of_PROFILES File_names_buffer[] //��� ����� ���������� (��������� malloc)
	);//��������� ���� ��� ������. (���������� ���-�� ������)

extern bool DH_Profile_Containter_name_convert_extension_to_new(
	__in WCHAR* PROFILE_x_CONTAINT_FILE_NAME, //��� ����������
	__in SERVICE_TYPE New_PROFILE_x_TYPE, //����� ���
	__in SERVICE_TYPE Current_PROFILE_x_TYPE //������ ���
	); //������������ ���������� ���������� � �������������� �������� �����

extern bool DH_LOCAL_is_filename_containt_profile_extension(__in UINT PROFILE_x_TYPE,__in WCHAR* PROFILE_x_CONTAINT_FILE_NAME); //��� ����� �������� ��������� ���������� �����?

extern bool DH_Profile_Create_Service_File(
	__in SERVICE_TYPE PROFILE_x_TYPE, //�� �������
	__in TCHAR* File_full_patch //������ ���� � �����
	);  //�������� �������� ���������� ����� (��������� ������ ����)

extern PVOID DH_Profile_PARSE_Arbitary_FILTER_CONFIG(
	__in TCHAR* pRaw_VMPX_Profiler, //��������� �� ����������� ����� �������
	__in size_t Raw_size //������ ������
	); //������ �������� vp_x �������


extern PVOID DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG(
	__in TCHAR* pRaw_VMPX_Profiler, //��������� �� ����������� ����� �������
	__in size_t Raw_size, //������ ������
	__in TCHAR* pINIFilename //��� INI �����
	);//������ �������� vp_x ���������

extern SERVICE_COUNT DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__GetSectionKeys(
	__in TCHAR* pBranchName, //��� ������ (�����)
	__in LPVOID pResultUnitTypeStack, //�������� ���� ����� ������
	__in TCHAR* pINIFilename //��� INI �����
	);//����������� DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG. ���������� ���� ������ �� ��������� ������ (�����).


/*
extern bool DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Normalize_and_assembly(
	__in TCHAR* pAsmInstruction, //�������� ����������
	__in size_t AsmInstruction_size, //����� ������ ���������� (����� ��� �������� CS)
	__in XEDPARSE* pXED_parse_asm, //��������� �� ��������� XEDPARSE
	__out SERVICE_TYPE* pSetCS_type, //��������� �� ���������� ���� ������������ ������� (���� ����������� - �� > NULL)
	__out size_t*		pSetCS_first_symbol, //��������� �� ���������� ������� ������� ������������ �������
	__out bool*	pSetStaticFlag //��������� �� ���� static/dynamic
	);//����������� DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG. ��������� ����� ����������� �������� � ������ ��� ����������, �� ������, � ����� ���������� ��������� XEDPARSE
	*/


extern bool DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Normalize_and_assembly(
	__in SERVICE_TYPE Ident_normalize, //��� ��������������� ������� (�������� VMPX_CONTEXT_TYPE_ASMS)
	__in size_t AsmInstruction_size, // now! ����� ������ ����������(����� ��� �������� CS)
	__out LPVOID pResultUnitTypeStack // ������ �� VM_VMPX_PROFILE_INI
	);//����������� DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG. ��������� ����� ����������� �������� � ������ ��� ����������, �� ������, � ����� ���������� ��������� XEDPARSE � ������������ ����������� ������

extern bool DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Release_normalize_instruction(
	__in __out LPVOID pResultUnitTypeStack, // ������ �� ����������� VM_VMPX_PROFILE_INI
	__in SERVICE_TYPE Ident_normalize, //��� ��������������� ������� (�������� VMPX_CONTEXT_TYPE_ASMS)
	__in PVOID pInstruction_pool, //������ �� ��� ���������� (��� ���������)
	__in PVOID pSeqStack //������ �� ���� ������� - ��� ���������������� ����������
	//__in SERVICE_TYPE Asm_operation //����������� �������� �������� ������������ - �c������������� ��� ������������������ (XED_PARSE__ASSEMBLY - BEA_ENGINE__DISSASM)
	);//����������� DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG. ������������ ��������� ���������� � ������� ���������� ����������� �������.

extern bool DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Release_normalize_instruction__NO_WRITE(
	__in __out LPVOID pResultUnitTypeStack, 
	__in SERVICE_TYPE Ident_normalize, 
	__in PVOID pInstruction_pool
	); //��-�� ����� ��� � DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Release_normalize_instruction. ������ ��� �������� ������ ������.

////////////////////////////////// [M] [T] //////////////////////////////

extern bool __fastcall DH_Profile_Shedule_threadwork__Write(
	__in PVOID pThreadWorkStack_pointer, //�������� ��������� �� ����
	__in SERVICE_TYPE StackType //��� ����� (������������ PROFILE_MT_STACKWORK_TYPE)
	); //������������� �������� ������ ����� ThreadWork.

static DWORD WINAPI DH_PROFILE_Shedule_WRITE_threadwork_MT(
	LPVOID Param
	); //�������� ������� DH_Profile_Shedule_threadwork__Write. ���������� ���������� ����� �� ������ �����������

extern bool __inline DH_Write_Current_hunt_profile_CONTAINER_MT(
	__in UINT PROFILE_x_TYPE, //��� ����������
	__in WCHAR* PROFILE_x_CONTAINT_FILE_NAME,  //��� ����� ����������
	__in BYTE* write_buff, //����� ������
	__in size_t num_bytes_to_write, //���-�� ���� ������  sizeof(��������� ����������)
	__in TCHAR* pLocalHUNTPROFILEPATCH_copy //��������� ����� HUNT_HIVE.PROFILE_PATCH ������ ��� ���������� ����������� ����������� ��������
	);//���������� ������ HIVE ������� � ����� MT. ������������� �����������!
extern PVOID __inline DH_Read_Current_hunt_profile_CONTAINER__readall_MT(
	__in ULONG PROFILE_x_TYPE, //��� ����������
	__in WCHAR* PROFILE_x_CONTAINT_FILE_NAME, //��� ����� ����������
	__in TCHAR* pLocalHUNTPROFILEPATCH_copy //��������� ����� HUNT_HIVE.PROFILE_PATCH ������ ��� ���������� ����������� ����������� ��������
	);  //��������� ������ HIVE ������� - �� ������������� ��� ������ ������ � ���������������� ������!!! ������������� �����������!