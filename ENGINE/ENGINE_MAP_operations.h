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

#define MAP_RETURN_COMPARE_EQ_FILES -1 //����� ���������
#define MAP_RETURN_COMPARE_TOO_MANY_DIFFERECES -2 //������� �������� ���������� ��������

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
	PATCH_NOT_APPLIED, //���� "not applied" (disabled)
	PATCH_ONLY_NOP, //������ ��"NOP"��� ������� ���������� 
	PATCH_NOP_AND_JMP_TO_INTRECEPTOR_with_RETURN, //��"NOP"��� ������� ���������� � ��������� ������ � ����������� � ����������� ������������
	PATCH_NOP_AND_JMP_TO_INTRECEPTOR__NO_RETURN //��"NOP"��� ������� ���������� � ��������� ������ � ����������� ��� ����������� � ���� ����������
	};

enum PATCH_TYPE_METHOD_INTRECEPT_RELEASE_ENUMS {
	PATCH_NOP_SLED_AND_NEXT_JUMP_CONST = 1, //"NOP" SLED target-���������� � ������������� ���������� JMP const ��� ������������
	PATCH_NOP_SLED_AND_PREV_JUMP_CONST, //"NOP" SLED target-���������� � ������������� ����������� JMP const ��� ������������
	PATCH_DISPLACE_OTHERS_ASM, //���������� ���������� � target-��� ��� jmp �� �����������
	PATCH_COMBINE__NOP_SLED_and_DISPLACE_OTHERS_ASM_TOO_CLOSE_NEXT_JMP //���������� ���������� � target-��� ��� NOP � ����������� �������������� jmp �� �����������
	};

enum PATCH_TYPE_OWNERS_ENUMS {
	//executable
	PATCH_OWNER_VM_INSIDE_EXECUTE = 1, //��� - ����������� ��� ������ VM
	PATCH_OWNER_VM_OUTSIDE_EXECUTE, //��� - ����������� ��� �� ��������� ��������� VM
	//coverage addreses
	PATCH_OWNER_VM_COVERAGE, //��� - ������ �������� ������� VM
	//data
	PATCH_OWNER_VM_SUPPLY15_DATA, //��� - ��������� �1,5 (������� �������)
	PATCH_OWNER_VM_PCODE_DATA, //��� - ����� p-code
	PATCH_OWNER_VM_CONSTANT_DATA //��� - ���������, ������������ VM
};

enum PATCH_ERROR_STATUS {
	//executable
	PATCH_ERROR_SUCCESS = 0, //��� ������
	PATCH_ERROR_VM_STACK_ABSENT, //���������� ��������� VM stack
	PATCH_ERROR_ADDR_POOL_ABSENT, //���������� ��������� Addr pool
	PATCH_ERROR_TARGET_ADR_in_ADDR_POOL_BAD, //�� ��������� ������� Target Addr � ���� ������� ������ VM
	PATCH_ERROR_PATCH_METHOD_NOT_ASSIGNED, //��� ����������� ��������� ���������� ������ ��������/��������� ���� ��� target address
	PATCH_ERROR_TARG_ADDR_NOT_VERIFY, //target address �� ������ ����������� (ReadProcessMemory)
	PATCH_ERROR_ARBITARY_FILTER_NOT_ASSINGED, //�� ������� ��������� arbitary filter
	PATCH_ERROR_CANNOT_CONSTRUCT_RETAIL_ARBITARY_FILTER, //�� ������� �������������� ������������� arbitary filter
};

enum PATCH_MAP_REWRITE_DISPLACE_TARGET {
	//executable
	PATCH_REWRITE_TARGET_is_VM_ORIGINAL_TYPE_ASM = 1, //������������ ������������ ������� ���������� ������ VM
	PATCH_REWRITE_TARGET_is_VM_INSIDE_ASM, //������������ ����� ������ ���������� ������ VM �� ������������ DH
	PATCH_REWRITE_TARGET_is_MY_HOOK, //���������� ������ ����
	PATCH_REWRITE_TARGET_is_VM_OUTSIDE_ASM, //������������ ����� ������ ���������� ��� VM
	//coverage addreses
	PATCH_REWRITE_TARGET_is_VM_COVERAGE, //������������ ������ �������� ������� VM
	//data
	PATCH_REWRITE_TARGET_is_VM_SUPPLY15_DATA, //������������ ��������� �1,5 (������� �������)
	PATCH_REWRITE_TARGET_is_VM_PCODE_DATA, //������������ ����� p-code
	PATCH_REWRITE_TARGET_is_VM_CONSTANT_DATA //������������ ���������, ������������ VM
};

// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& code definitions &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
#define codedef_set_pathcing_intreception_type(release_type) pIteraion_PatchArrive->target_addres__assign_path_method = (release_type); last_pathing_method = (release_type)
#define codedef_no_key pOPEN_BRANCH = NULL

static BYTE MAP_mark_buffer[10000];
static BYTE MAP_mark_ROLL_BACK_buffer[10000] = {NULL};
//++++++++++++++++++++++ structs ++++++++++++++++++++++
typedef struct DH_Dump_Cell_service_struct
{
	SERVICE_ADDRES Cell_mem_jmp; //������ ��� �������� ������ � ���������� ������
	//OPERATIONAL_OPERAND_CELL_ADDRES Operand_Address; //������ ��� �������� ������ � ���������� ������
	DWORD Operand_Address; //������ ��� �������� ������ � ���������� ������
}DUMP_CELL_STRUCT, *pDUMP_CELL_STRUCT; //��������� ��������� �������� ������ � �������


// �������� ���������� ����� ����������� �������� ���� ������
typedef struct DH_Files_different_result
{
ULONG64 Start_diff_Address; //����� ������� �������������� ����� � �����
ULONG Diff_Bytes_Count; //���-�� ��������� ������ (������ � ������)
LPBYTE Diff_bytes_file_1_DUMP_offset; //�������� ����������� ������ ���� � ����� ����� ��� ����� �1
LPBYTE Diff_bytes_file_2_DUMP_offset; //�������� ����������� ������ ���� � ����� ����� ��� ����� �2
}SET_FILES_COMPARE_EVENT, *pSET_FILES_COMPARE_EVENT;

/*
typedef struct DH_Files_compare_results
{
	SERVICE_COUNT compare_event_table_count; //���-�� ������ ��������� � ������� �����������

	ULONGLONG   File_1_ImageBase; //ImageBase ������ �� ������ (������ ���� �����)
	IMAGE_SECTION_HEADER File_1_protection_section_header;//������ ������ ������ �� ������ (������ ���� �����)

	bool Option_Align_to_multiple_DWORD_QWORD; //����������� ���-�� ��������� ���� �� ������� DWORD - QWORD, ���� ������ (��������, ���� 3 ��� 7)
}HEADER_FILES_COMPARE_RESULTS, *pHEADER_FILES_COMPARE_RESULTS; //���������� ��������� ������
*/

// ++++++++++ structs P A T C H ++++++++++++++++
typedef struct DH_PATCH_target_information_struct
{
	SERVICE_ADDRES target_addres; //������ ������ ���������� ������� ��� �������� (ex: � ����� ��������)
	SERVICE_TYPE   target_addres__assign_path_method; //����������� ����� �������� ��� ������� ������ PATCH_TYPE_ENUMS (ex: � ����� ��������)
	SERVICE_ARRAY_ID Containt_HELD_GROUP_relative_to_AddrPool_ID; //�������� ������ �������� � ��������� ����

	//critical patch
	SERVICE_ADDRES patch__HOOK_set_addres; //��������� ����� ����
	SERVICE_ADDRES patch__desdonation_FILTER_addres; //��������� ����� "������������"
	SERVICE_ADDRES patch__return_addres_for_FILTER; //����� �������� ����� ��������� "������������"
	SERVICE_ADDRES patch__original_bytes_addres; //��������� ����� ���������� ����

	//������������ �����
	BYTE patch_BYTES_original_target_instruction_bytes[MAX_ASM_SAVE_BYTES]; //������������ ����������
	SERVICE_COUNT patch_COUNT_original_target_instruction_bytes; //������� ����������� ���� ������������ ����������

	BYTE patch_BYTES_original_displace_JMP_intreceptor_instruction_bytes[MAX_ASM_SAVE_BYTES]; //��� ���������� ����������� "JMP intrecept"
	SERVICE_COUNT patch_COUNT_original_displace_JMP_intreceptor_instruction_bytes; //������� ����������� ���� ����������� "JMP intrecept"

		//(PATCH_OWNER_VM_SUPPLY15_DATA, PATCH_OWNER_VM_PCODE_DATA, PATCH_OWNER_VM_CONSTANT_DATA - ����� � �������)//
	BYTE patch_BYTES_original_ANY_DATA_bytes[MAX_ANY_DATA_SAVE_BYTES]; //������������ ����� � �������� PATCH_OWNER_VM_SUPPLY15_DATA, PATCH_OWNER_VM_PCODE_DATA, PATCH_OWNER_VM_CONSTANT_DATA
	SERVICE_COUNT patch_COUNT_original_ANY_DATA_bytes; //���������� ������ � �������

	//���� �����
	BYTE patch_BYTES_revoke_target_instruction_bytes[MAX_ASM_SAVE_BYTES]; //�������������� ����� ������������ ����������

	BYTE patch_BYTES_HOOK_bytes[MAX_ASM_SAVE_BYTES]; //��� �� ������-�����������
	SERVICE_COUNT patch_COUNT_HOOK_instruction_bytes; //������� ���� ���� �� ������

	BYTE patch_BYTES__FILTER_instruction_bytes[MAX_INTRECEPTOR_BYTES]; //��� ���������� ������������-�������!!!
	SERVICE_COUNT patch_COUNT__FILTER_instruction_bytes; //������� ���� ������������-�������!!!
		//(PATCH_OWNER_VM_SUPPLY15_DATA, PATCH_OWNER_VM_PCODE_DATA, PATCH_OWNER_VM_CONSTANT_DATA - ����� � �������)//
	BYTE patch_BYTES_revoke_ANY_DATA_bytes[MAX_ANY_DATA_SAVE_BYTES]; //��������� ����� � �������� PATCH_OWNER_VM_SUPPLY15_DATA, PATCH_OWNER_VM_PCODE_DATA, PATCH_OWNER_VM_CONSTANT_DATA
	

	//��������� REWRITE
	SERVICE_TYPE Detected_rewrite_abilites[MAX_REWRITE_SITES]; //���� �������� ���������� ����, ���� �����������
	SERVICE_ADDRES Rewrite_addres[MAX_REWRITE_SITES]; //��������� ����� ���������� 
	SERVICE_COUNT Bytes_rewrite[MAX_REWRITE_SITES]; //���� ������������
}PATCH_HELD_GROUP, *pPATCH_HELD_GROUP;

typedef struct PATCH_Struct
{
	WCHAR name[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN]; //��� patch-�����
	SERVICE_TYPE owner_as_profile__of_HELD_GROUP;//�������� ������ ������� �� ����� ������� (PATCH_TYPE_OWNERS_ENUMS)
	WCHAR owner_name[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN]; //��� ����� ��������� patch-�����

	PATCH_HELD_GROUP Containt_HELD_GROUP[MAX_HANHELD_TYPE_DETERMINE_CRITICAL_ADDRESES]; //������ ��������
	SERVICE_COUNT HELD_GROUP_Fill_count; //���������� ����������� HELD_GROUP
	//held common info
	SERVICE_TYPE type_of_target_addres;//��� ������ (ex: �� HANHELD_TYPE_ENUMS)
	SERVICE_ADDRES parent_subroutine_address; //����� ������� (����) � ������� ��������� ���� (�������� ��� PROFILE_TYPE_VM ��� HANHELD.HANDELD_Primary_Addres)

	SERVICE_ARRAY_ID i_Containt_handler_ArrayID; //������ �� HANHELD Containt_handler[VM_HANDLES_MAX_COUNT] � ����������� ����� VM

	bool is_not_applied; //���� "not applied"
	bool is_new_added; //���� "������ ��� ��������"
	SERVICE_ERROR last_error_status; //���������� ������ ��������
}PATCH_INFO_STACK, *pPATCH_INFO_STACK;


typedef struct PATCH_SUPPORTING_REGION_INI_Struct
{
	WCHAR VMP_SectionName[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN]; //��� ������ vmp
	SERVICE_ADDRES VMP_Section_start_address; //����� ������ (RVA)
	size_t VMP_Section_size; //������ ������ (� ������)
	bool is_VMP_section_validate; //������������� ��������� VMP Section?

	SERVICE_ADDRES Region_VMP_original_copy_start; //����� ������ ������� - ����������� ����� vmp ������
	SERVICE_ADDRES Region_VMP_original_copy_end; //����� ����� ������� - ����������� ����� vmp ������
	bool is_Region_VMP_original_copy_auto_alloc; //���� ��������� ������� Region_VMP_original_copy_auto
	bool is_Region_VMP_original_copy_REQ_MEMCPY; //��������� ����������� ���������� VMP_Section � Region_VMP_original_copy?
	bool is_Region_VMP_original_copy_validate; //������������� ��������� Region_VMP_original_copy?

	SERVICE_ADDRES Region_VMP_patch_map_start; //����� ������ ������� - ����� �������� ������
	SERVICE_ADDRES Region_VMP_patch_map_end; //����� ����� ������� - ����� �������� ������
	BYTE		   Region_VMP_patch_map_LOCK_byte; //�������� LOCK ����� (�� ������� 0xFF)
	bool is_Region_VMP_patch_map_auto_alloc; //���� ��������� ������� Region_VMP_patch_map?
	bool is_Region_VMP_patch_map_validate; //������������� ��������� Region_vmp_patch_map?

	SERVICE_ADDRES Region_VMP_hook_and_filter_start; //����� ������ ������� - ��������� ��� hook's � �������� vmp
	SERVICE_ADDRES Region_VMP_hook_and_filter_end; //����� ����� ������� - ��������� ��� hook's � �������� vmp
	SERVICE_ADDRES Region_VMP_float_pointer; //��������� ��������� �� ��������� �����
	bool is_Region_VMP_hook_and_filter_auto_alloc; //���� ��������� ������� Region_VMP_hook_and_filter?
	bool is_Region_VMP_hook_and_filter_validate; //������������� ��������� Region_VMP_hook_and_filter?

	size_t All_Support_regions_retail_size; //��������� ������ ���� �������������� �������� ������
}PATCH_SUPPORTING_REGION_INI_STACK, *pPATCH_SUPPORTING_REGION_INI_STACK;

typedef struct PATCH_SUPPORTING_VMPX_PROFILE_INI_Struct
{
	WCHAR Assembly_instruction1[MAX_PATH]; //���������� 1
	WCHAR Assembly_instruction2[MAX_PATH]; //���������� 2
	WCHAR Assembly_instruction3[MAX_PATH]; //���������� 3
	WCHAR Assembly_instruction4[MAX_PATH]; //���������� 4
	WCHAR Assembly_instruction5[MAX_PATH]; //���������� 5
	WCHAR Assembly_instruction6[MAX_PATH]; //���������� 6
	WCHAR Assembly_instruction7[MAX_PATH]; //���������� 7
	WCHAR Assembly_instruction8[MAX_PATH]; //���������� 8
	WCHAR Assembly_instruction9[MAX_PATH]; //���������� 9
	WCHAR Assembly_instruction10[MAX_PATH]; //���������� 10
	WCHAR Assembly_instruction11[MAX_PATH]; //���������� 11
	WCHAR Assembly_instruction12[MAX_PATH]; //���������� 12
	WCHAR Assembly_instruction13[MAX_PATH]; //���������� 13
	WCHAR Assembly_instruction14[MAX_PATH]; //���������� 14

	WCHAR Assembly_instruction15[MAX_PATH]; //���������� 1
	WCHAR Assembly_instruction16[MAX_PATH]; //���������� 2
	WCHAR Assembly_instruction17[MAX_PATH]; //���������� 3
	WCHAR Assembly_instruction18[MAX_PATH]; //���������� 4
	WCHAR Assembly_instruction19[MAX_PATH]; //���������� 5
	WCHAR Assembly_instruction20[MAX_PATH]; //���������� 6
	WCHAR Assembly_instruction21[MAX_PATH]; //���������� 7
	WCHAR Assembly_instruction22[MAX_PATH]; //���������� 8
	WCHAR Assembly_instruction23[MAX_PATH]; //���������� 9
	WCHAR Assembly_instruction24[MAX_PATH]; //���������� 10
	WCHAR Assembly_instruction25[MAX_PATH]; //���������� 11
	WCHAR Assembly_instruction26[MAX_PATH]; //���������� 12
	WCHAR Assembly_instruction27[MAX_PATH]; //���������� 13
	WCHAR Assembly_instruction28[MAX_PATH]; //���������� 14
	SERVICE_COUNT Assembly_count; //���-�� ����������

	SERVICE_ARRAY_ID Label[10]; //����� ��� ������ ���������� (1-9)
	bool Control_symbols_is_present[28]; //�������/���������� ����������� �������� � ������
	size_t First_control_symbol_offset[28]; //�������� �� ������ ������ ������� ������������ ������� (�����)
	SERVICE_TYPE Predict_POST_BUILD_Control_symbol_type[28]; //����������� �������, ������������ �� ��������� ����� ������ (JMP �� ������ � ������� � ������������ ���)
	size_t Predict_POST_BUILD_asm_instruction_increment_size[28]; //���������� ������� ������� ���������� (��� JMP �� ������) � �����������
	bool is_request_return_to_original_code; //���������� �� ������ �������� � ������������ ���?
}PATCH_SUPPORTING_VMPX_PROFILE_INI, *pPATCH_SUPPORTING_VMPX_PROFILE_INI;

typedef struct PATCH_SUPPORTING_VMPX_PROFILE_INI_STACK_Struct
{
	SERVICE_TYPE KeyType[HANHELD_TYPE_ENUMS_COUNT];
	PATCH_SUPPORTING_VMPX_PROFILE_INI VMPX_HANHELD_TYPE_ENUMS[HANHELD_TYPE_ENUMS_COUNT];
	SERVICE_COUNT Loaded_HANHELDS_FILTERS; //���-�� ����������� �������� � PATCH_SUPPORTING_VMPX_PROFILE_INI
}PATCH_SUPPORTING_VMPX_PROFILE_INI_STACK, *pPATCH_SUPPORTING_VMPX_PROFILE_INI_STACK;

typedef struct PATCH_LIST_SNAPSHOT
{
	//���������������� ������
	SERVICE_ADDRES checkpoint_address1; //����� ������������ ���������� (PATCH_OWNER_VM_SUPPLY15_DATA, PATCH_OWNER_VM_PCODE_DATA, PATCH_OWNER_VM_CONSTANT_DATA - ������ �������)
	SERVICE_ADDRES checkpoint_address2; //����� ���� (������������ ������ ��� PATCH_OWNER_VM_OUTSIDE_EXECUTE � PATCH_OWNER_VM_INSIDE_EXECUTE)

	//��������
	SERVICE_COUNT  COUNT_of_original_bytes_in_checkpoint1; //���-�� ���� ��� �������� � ������� #1 (original == reworke)
	SERVICE_COUNT  COUNT_of_original_bytes_in_checkpoint2; //���-�� ���� ��� �������� � ������� #2 (original == reworke)
	SERVICE_COUNT  COUNT_all_rewrites_checkpoint1; //���-�� �������������� ���� #1
	SERVICE_COUNT  COUNT_all_rewrites_checkpoint2; //���-�� �������������� ���� #2

	//���� checkpoints
	SERVICE_TYPE TYPE_of_checkpoint1; //��� ������ ��������� #1 (enum PATCH_MAP_REWRITE_DISPLACE_TARGET)
	SERVICE_TYPE TYPE_of_checkpoint2; //��� ������ ��������� #1 (enum PATCH_MAP_REWRITE_DISPLACE_TARGET)

	//������������ �����
	BYTE original_BYTES_checkpoint1[MAX_ANY_DATA_SAVE_BYTES]; //������������ ����� checkpoint #1
	BYTE original_BYTES_checkpoint2[MAX_ANY_DATA_SAVE_BYTES]; //������������ ����� checkpoint #2

	//�������������� ���� �����
	BYTE reworke_BYTES_checkpoint1[MAX_ANY_DATA_SAVE_BYTES]; //���� ����� checkpoint #1
	BYTE reworke_BYTES_checkpoint2[MAX_ANY_DATA_SAVE_BYTES]; //���� ����� checkpoint #2
	
	//��������� ����
	WCHAR name[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN]; //��� patch-�����
	SERVICE_TYPE owner_as_profile__of_HELD_GROUP;//�������� ������ ������� �� ����� ������� (PATCH_TYPE_OWNERS_ENUMS)
	WCHAR owner_name[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN]; //��� ����� ��������� patch-�����

	//��������� REWRITE
	SERVICE_COUNT COUNT_Rewrite_struct; //���������� ��������� � ��������� REWRITE
	SERVICE_TYPE Detected_rewrite_abilites[MAX_REWRITE_SITES]; //���� �������� ���������� ����, ���� �����������
	SERVICE_ADDRES Rewrite_addres[MAX_REWRITE_SITES]; //��������� ����� ���������� 
	SERVICE_COUNT Bytes_rewrite[MAX_REWRITE_SITES]; //���� ������������
}PATCH_SNAPSHOT, *pPATCH_SNAPSHOT;

#define MAX_APPROX_SECTION_COUNT 150
typedef struct MAP_IDENTICAL_SECTIONS
{
	PIMAGE_SECTION_HEADER Protection_SECTION_1; //������ �� ������ ������ � ����� �1. ��������������� ��� ������ ������������
	PIMAGE_SECTION_HEADER Protection_SECTION_2; //������ �� ������ ������ � ����� �2. ��������������� ��� ������ ������������

	PIMAGE_SECTION_HEADER IDENTICAL_Section_vpp_1[MAX_APPROX_SECTION_COUNT]; //������ �� ����� ������ �� ������� �����
	PIMAGE_SECTION_HEADER IDENTICAL_Section_vpp_2[MAX_APPROX_SECTION_COUNT]; //������ �� ����� ������ �� ������� �����
	WORD NumOfSection_ALL; //����� ���������� ���������� ������
	ULONGLONG AnyFILE_1_2_IMAGEBASE; //ImageBase ������ �� ������. ������ ���� ���������!!!!!!!!!!!!

	//������ ���������////////////
	SERVICE_COUNT compare_event_table_count; //���-�� ������ ��������� � ������� �����������

	ULONGLONG   File_1_ImageBase; //ImageBase ������ �� ������ (������ ���� �����)
	IMAGE_SECTION_HEADER File_1_protection_section_header;//������ ������ ������ �� ������ (������ ���� �����)

	bool Option_Align_to_multiple_DWORD_QWORD; //����������� ���-�� ��������� ���� �� ������� DWORD - QWORD, ���� ������ (��������, ���� 3 ��� 7)
}MAP_IDENTICAL_SECTIONS_STACK, *pMAP_IDENTICAL_SECTIONS_STACK;

//******************************* F U N C! ***************************************
bool DH_File_change_binary_dump(LPWSTR Dest_file, //���� ����������
	LPWSTR Src_binary_dump, //�������� ����
	size_t Dest_file_raw_offset, //����� �������� �� ������ �����
	size_t Size_of_dump_data //������ ������ �����
	); //�������� ������ � �����

bool DH_File_fill_addres_cells(LPWSTR Dest_file, //���� ����������
	LPWSTR Src_binary_dump, //�������� ����
	UINT64 File_load_Base_Address //������� ����� �������� ����� (��� ������)
	); //��������� ��������� ������ �������� ��


pSET_FILES_COMPARE_EVENT __fastcall DH_PE_Files_compare(
	HMODULE PE_File_1, //���� PE_����� �1
	HMODULE PE_File_2, //���� PE_����� �2
	LPBYTE* ppSET_FILES_COMPARE_BYTES_buffer, //���������� ������ ��� ���� ������������� ����
	__in __out pMAP_IDENTICAL_SECTIONS_STACK pIdentSections //��������� ���������, ������������ � �������� ������� ��������� � ��������� ������ ������������ PE ������. ���-�� �������� ����� ���������
	); //���������� �����, �������� ������ ��� ������� ����������� ��������� � ���������� ��������� �� ��

__inline ULONG local_find_next_byte_differece_in_DUMP(
	__in LPBYTE *Current_file_DUMP_1_pointer, //������ ��������� �� ���� ����� �1
	__in LPBYTE *Current_file_DUMP_2_pointer, //������ ��������� �� ���� ����� �2
	__in __out SERVICE_COUNT* iteration_count //���-�� ���������� ��������
	); //������� ������ ��������� �������� � ������

HMODULE DH_Load_PE_as_datafile(__in LPWSTR PE_File_path); //��������� PE ���� � ������ �������� ��� ������������

PIMAGE_SECTION_HEADER DH_MAP_Get_main_protection_section_point(__in HMODULE hmModule); //����� ������� ������ ������. ���������� ��������� �� ������ �� ����������� hmModule

////////////////////////////////// P * A * T * C * H //////////////////////////////

UINT DH_PATCH_Add_new_vmp_execute_address_patch(
	SERVICE_ADDRES Critical_address_array[], //������ �� ������ ������� ��� ��������
	SERVICE_ARRAY_ID Critical_address_relative_addrpool_ID_array[], //������ �� ������ �������� ������� ��� �������� - AddrPool
	WCHAR* owner_filename, //��� ����� ��������� (�����������)
	size_t i_Containt_handler_ArrayID, //������ �� ���������� ID � HANHELD Containt_handler[VM_HANDLES_MAX_COUNT];
	//SERVICE_ADDRES Parent_subroutine_address, //����� ������� (����) � ������� ��������� ���� (�������� ��� PROFILE_TYPE_VM ��� HANHELD.HANDELD_Primary_Addres) (�����������)
	SERVICE_TYPE Addres_asm_instruction_type, //��� ������� �� ��� ���������� (ex: mov eax, [rax]; xor al, [rdx])
	SERVICE_TYPE Addres_owner_type_of_profile //���������� ������� (PATCH_TYPE_OWNERS_ENUMS)
	);//��������� ��������� ������ ����� - ��� ������������ ���� ������/������� VM

UINT DH_PATCH_Add_new_vmp_DATA_address_patch(
	SERVICE_ADDRES Patch_start_coverage_address, //��������� ����� �������� �����
	size_t Size_Of_patch_data, //������ ����������� ������
	WCHAR* owner_filename, //��� ����� ��������� (�����������)
	SERVICE_TYPE Addres_owner_type_of_profile //���������� ������� (PATCH_TYPE_OWNERS_ENUMS)
	);//��������� ��������� ������ ����� - ��� ������ VM

TCHAR* __fastcall DH_VM_PATCH_OWNERS_type_to_string(
	SERVICE_TYPE owner_type //��� PATCH_TYPE_OWNERS_ENUMS
	); //��������������� ��� PATCH_TYPE_OWNERS_ENUMS � ������


pPATCH_SUPPORTING_REGION_INI_STACK __fastcall DH_MAP_Release_patch_region_config(
	); //������������� ����������� ������������ �� �������� (������� - PROFILE_TYPE_PATCH_REGIONS_INFO_INI). �������� ����� = DH_MAP_Close_patch_region_config

bool __fastcall DH_MAP_Close_patch_region_config(
	pPATCH_SUPPORTING_REGION_INI_STACK pPatch_stack
	); //��������� ������������, ������������� �������� DH_MAP_Release_patch_region_config

size_t __fastcall DH_MAP_Do_APPLY_NA_STACK(
	pPATCH_SUPPORTING_REGION_INI_STACK pPatch_stack
	); //��������� N/A ���� ��� ���������� �������� ��������

void __fastcall DH_MAP_Do_APPLY_NA_STACK__Mark_Patch_MAP(
	SERVICE_ADDRES pDesd_addr, //����� �����������
	size_t size_write_data //����� ��������� (�� ����� ����� ������������ ����������� ������)
	); //����������� DH_MAP_Do_APPLY_NA_STACK. ���������� patch map

void __fastcall DH_MAP_Do_APPLY_NA_STACK__Mark_Patch_MAP(
	SERVICE_ADDRES pDesd_addr, //����� �����������
	size_t size_write_data //����� ��������� (�� ����� ����� ������������ ����������� ������)
	); //����������� DH_MAP_Do_APPLY_NA_STACK. ���������� patch map

void __fastcall DH_MAP_Do_APPLY_NA_STACK__ROLL_BACK_Mark_Patch_MAP(
	SERVICE_ADDRES pDesd_addr, //����� �����������
	size_t size_write_data //����� ��������� (�� ����� ����� ������������ ����������� ������)
	); //����������� DH_MAP_Do_APPLY_NA_STACK. ������������ patch map. �������� ����� DH_MAP_Do_APPLY_NA_STACK__Mark_Patch_MAP

SERVICE_TYPE __fastcall DH_MAP_Do_APPLY_NA_STACK__Get_BEST_Patch_on_TYPEDEFs_asm(
	__in __out pPATCH_INFO_STACK pPatchArrive, //������� ����� ��������� Patch
	__in pVM_AsmAddr_POOL_STACK pVMAddrPoolArrive, //����������� VM adrr pool
	__in pVM_INFO_STACK pVM_Info_Arrive, //����������� VM ����
	__in XEDPARSE* pXEDParse //XED
	); //����������� DH_MAP_Do_APPLY_NA_STACK. ���������� ������ �������� ��������������� ����������, ���������� �� ��� ������

__forceinline SERVICE_ARRAY_ID __fastcall DH_MAP_Do_APPLY_NA_STACK__Find_TargetAdd_in_AddressPool(
	SERVICE_ADDRES TargetTypedefAddr, //����� ��� ������
	pVM_hanheld_addres_pool pAddrPool_ReqHANHELD //��������� �� ������� ������ VM_hanheld_addres_pool - ������� �� SERVICE_ARRAY_ID i_Containt_handler_ArrayID
	); //����������� DH_MAP_Do_APPLY_NA_STACK. �������� �� ����� critical ������ ������������ ��� "��������" � ���� ������� �������� ������. ���������� ��������� array ID

bool __fastcall DH_MAP_Init_PATCH_strategy(
	pPATCH_SUPPORTING_REGION_INI_STACK pPatch_stack
	); //�������������� ������� ��������.

pPATCH_SUPPORTING_VMPX_PROFILE_INI_STACK __fastcall DH_MAP_Do_INIT_Arbitary_FILTERs(
	__in 	WCHAR *VMPX_profile_name, //������ VMP - ��������� ����� (ex: 2)
	__in	SERVICE_TYPE VMP_Version //������ VMP 2 ��� 3
	); //�������������� ������� ��������� ��� ��������� ������ VMP. ���������� ����������� ���������

__forceinline pPATCH_SUPPORTING_VMPX_PROFILE_INI_STACK __fastcall DH_MAP_Predict_VMPX_PROFILE_filter(
	__in pPATCH_SUPPORTING_VMPX_PROFILE_INI_STACK pFilterRAWStack //����� ���� ������� �������
	);//������������ ��������������� � ������������� ���� ������� � ������ ����� � ����������� ��������

__forceinline pPATCH_SUPPORTING_VMPX_PROFILE_INI __fastcall DH_MAP_Do_APPLY_NA_STACK__Get_Arbitary_Filter(
	__in __out pPATCH_INFO_STACK pPatchArrive, //������ �� ����
	__in pVM_INFO_STACK pVM_Info_Arrive //������ �� VM ����
	); //����������� DH_MAP_Do_APPLY_NA_STACK. �������� ��������� ��� ������ � �������������� arbitary-��������


SERVICE_COUNT __fastcall DH_MAP_Do_APPLY_NA_STACK__Construct_Retail_PATCH_byte_tables(
	__in pPATCH_HELD_GROUP pHELDGroup, //������ �� ������� ���� ��������
	__in pVM_INFO_STACK pVM_Info_Arrive, //������ �� VM
	__in pPATCH_SUPPORTING_VMPX_PROFILE_INI pArbitaryFilterUnit, //������ �� ����������� ������
	__in pPATCH_SUPPORTING_REGION_INI_STACK pSupportRegion, //������ �� ������
	__in XEDPARSE* pXEDParse //XED
	); //����������� DH_MAP_Do_APPLY_NA_STACK. ��������� ������������� ���������� ArbitaryFilter � ������ ������� ����


SERVICE_COUNT DH_MAP_Do_APPLY_NA_STACK__WritePM_Arbitary_Filter(
	__in pPATCH_INFO_STACK pWritePatch
	); ////����������� DH_MAP_Do_APPLY_NA_STACK. ��������� ���������� ���� - ������ ���� � �������� ����������� ��������

/*
SERVICE_COUNT __fastcall DH_MAP_Do_APPLY_NA_STACK__Remap_and_refresh_asm_pool(
	__in __out BYTE* pAsmPool, //����� � ��� ������������
	__in size_t PoolSize, //������ ����
	__in SERVICE_ADDRES Source_CIP, //��������� CIP-����� ���������
	__in SERVICE_ADDRES Source_VA, //Virtual Addres ���������
	__in SERVICE_ADDRES Desd_CIP, //��������� CIP-����� ����������
	__in XEDPARSE* pXEDParse //XED
	);*/

SERVICE_COUNT __fastcall DH_MAP_Do_APPLY_NA_STACK__Remap_and_refresh_asm_pool(
	//__in __out BYTE* pAsmPool,  //����� � ��� ������������
	__in SERVICE_COUNT AsmCount, //���������� ���������� � ������
	__in PDISASM pFILL_Dissasm, //���������� ����������� ��������� DISASM
	__in XEDPARSE* pFILL_XEDParse //���������� ����������� ��������� XED
	);//����������� DH_MAP_Do_APPLY_NA_STACK. ��������� ��������� � ���������� displace-����������

SERVICE_COUNT __fastcall DH_MAP_Take_patch_list_snapshot(
	__in __out pPATCH_SNAPSHOT pPatch_Snapshot //snapshot ������ ����������� ������
	);//�������� snapshot-������ online-������

pPATCH_SNAPSHOT __fastcall DH_MAP_AntiRewrite_Sentinel(
	__in pPATCH_SNAPSHOT pSnapList, //������ ������ ���������
	__in SERVICE_COUNT* pSnapListCount, //������ �� ���������� ��������� � ������ (ACCEPT SnapListCount > 0). ���� ������� ��������, ���������� � ������ ������� �������� ��������� (INC, ++) �� ����� ������ � ������� �����������
	__out SERVICE_COUNT* pAllRewritesSnapsCOUNT //����� ���������� ������ � ������������
	); //AntiRewrite - �������. ���������� ������ �� ������ ���������� ������� �� ������ � ����������� ����������� (INC, ++) ��������

SERVICE_COUNT __fastcall DH_MAP_AntiRewrite_Sentinel__compare_two_dumps(
	__in PBYTE Dump1, //������ �� ���� �1
	__in PBYTE Dump2, //������ �� ���� �2
	__in SERVICE_COUNT DumpCount //������ ����������� �������
	//__in __out SERVICE_ADDRES* pF1rstDiff_Addr, //��������� ��������� �����. ������������ ��� � ������������ � ������ ��������� ���������
	//__in __out SERVICE_COUNT* pF1rstDiff_Count //��������� ������ �� ���������� ��������� ����. ���������� �������� ��� ������ ��������� ��������
	);  //����������� �� DH_MAP_AntiRewrite_Sentinel. ���������� ��� ����� � ����� ��������. ���������� ���������� ��������� ���� �� ������� ��������

 TCHAR* __fastcall DH_PATCH_ERROR_type_to_string(
	 __in SERVICE_ERROR Err_type
	 ); //������������ ��� ������ �������� � ��������� ������� � ������������ � enum PATCH_ERROR_STATUS

   TCHAR* __fastcall DH_PATCH_ReWRITE_type_to_string(
	   __in SERVICE_ERROR Rewrite_type
	   ); //������������ ��� ������ ���������� (ReWrite) � ��������� ������� � ������������ � enum PATCH_MAP_REWRITE_DISPLACE_TARGET

   char* __fastcall DH_MAP_Determine_identical_protection_section(
	   __in HMODULE PE_File_1, //������ �� ������������ ���� �1
	   __in HMODULE PE_File_2, //������ �� ������������ ���� �2
	   __out pMAP_IDENTICAL_SECTIONS_STACK pIdentSections //������ �� ���� �� ������ �� ���������� �������
	); //���������� ����� ������, �������� ���������� ��� ������ ������ �� ���� PE-������. ��� ������������� ���������� ���������� ������ �� ��� ���������� ������ � PE-�������� IMAGE_SECTION_HADER

   char* __fastcall DH_MAP_Verify_and_set_protection_section(
	   __in char* pSource_Section_name, //�������� ��� ������
	   __in pMAP_IDENTICAL_SECTIONS_STACK pIdentSections //
	); //��������� �������� �������� ������������� ������ � ������ ��������� � ��������� ������. �������������� ������ ��� ������. ��� ������������� ���������� ���������� ������ �� ��� ���������� ������ � PE-�������� IMAGE_SECTION_HADER