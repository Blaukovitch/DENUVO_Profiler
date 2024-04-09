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


#define TS_IMPORT_CELL_A "cell"
#define TS_IMPORT_WINAPI_A "winapi"
//++++++++++++++++++++++ structs ++++++++++++++++++++++
typedef struct IMPORT_DECODE_METHOD
{
	OPERATIONAL_ASM_INSTRUCTION decode_instruction;
	OPERATIONAL_OPERAND_CELL_ADDRES decode_operand; //or offset
}DH_IMPORT_DECODE_METHOD, *pDH_IMPORT_DECODE_METHOD;

typedef struct IMPORT_RECORD
{
	SERVICE_OFFSET Import_Dll_name_offset; //��� ����������
	SERVICE_OFFSET Import_API_NAME_offset; //��� ������� �� ���� ����������
	CELL_ADDRES Import_CELL; //������ � ������� �������� �������������� �����
	FARPROC Import_last_save_adress; //����� �� ������ �����

	DH_IMPORT_DECODE_METHOD Import_decode_block; //���� ������������� �������

	SERVICE_COUNT Import_Xrefs; //������ �� ������ 

}DH_IMPORT_RECORD, *pDH_IMPORT_RECORD;


typedef struct IMPORT_STACK
{
}IMPORT_INFO_STACK, *pIMPORT_INFO_STACK;

//********** DH PROFILE func ********** //
extern ULONG DH_PickDumpFile_import(__in CString Dump_file_path, __out pIMPORT_INFO_STACK Out_import_stack); //��������� ���� ������� � ��������� ������ �������