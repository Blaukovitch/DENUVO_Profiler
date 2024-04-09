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

#define MAX_PIRATEMAPS_COUNT 20
/*

*/
//++++++++++++++++++++++ structs ++++++++++++++++++++++

typedef struct DH_PIRATEMAP_main // info stack unit
{
	WCHAR filename[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN];
	bool is_dll_addres_type; //������������ ����� (DLL?, RELOCATION?). ��������� �� �������� �������. ��� ������������ ������� SERVICE_ADDRES ��� SERVICE_OFFSET
	SERVICE_TYPE PirateMAP_addres_area_type; //��� ��������� ������� (�������� ������������� PIRATEMAP_MAP_TYPEDEF)
	IMAGE_SECTION_HEADER PE_Section_Header; // ������������� ����������� � ������ (��� ���� PMAP_TYPE_PE_IMAGE_SECTION)

	SERVICE_ADDRES PIRATEMAPPING_Region_start_addres; //��������� ����� ������� � �������� �������� PIRATE MAP (SERVICE_OFFSET � ������ ������������� ������)
	size_t PIRATEMAPPING_Region_size; //������ (� ������)  � �������� �������� PIRATE MAP 
}PIRATEMAP_PROFILE_MAIN_INFO, *pPIRATEMAP_PROFILE_MAIN_INFO;

typedef struct DH_PIRATEMAP_information_struct // *************& info stack
{
	PIRATEMAP_PROFILE_MAIN_INFO PMAP_Main_info; //������� ���������� � PIRATE MAP
	BYTE PirateMAP_area[];
}PIRATEMAP_PROFILE_INFO_STACK, *pPIRATEMAP_PROFILE_INFO_STACK;

typedef struct DH_PIRATEMAP_filelist_struct // filelist
{
	PIRATEMAP_PROFILE_MAIN_INFO PMAP_Main_info_stack[MAX_CONTAINTER_FILES]; //���� ������� ���������� � ���� ������ PIRATE MAP
	SERVICE_COUNT FileList_count; //���������� ��������� � ������
}PIRATEMAP_FILELIST_INFO, *pPIRATEMAP_FILELIST_INFO;

 
// $$$$$$$$$$$$$$$$$$$$$$$$$ enums $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
enum PIRATEMAP_MAP_TYPEDEF
{
	PMAP_TYPE_PE_IMAGE_SECTION = 1, //����������� ������ PE-�����
	PMAP_TYPE_DYNAMIC_ALLOC, //������ ���������� �����������
	PMAP_ANOTHER // ������
};

 // ------------------------- SUBs ------------------------- 

extern pPIRATEMAP_FILELIST_INFO DH_PIRATEMAP_Create_main_info_stack_cache(void); //�������� ��������� PIRATEMAP_PROFILE_MAIN_INFO ���� �����������, ������� �������� ������� ���������� � ���������� �����
extern pPIRATEMAP_PROFILE_MAIN_INFO __fastcall DH_PIRATEMAP_Found_target_addres_in_stack_cache(
	SERVICE_ADDRES Target_Addres, //����������� �����
	pPIRATEMAP_FILELIST_INFO pFill_main_info_stack_cache //������� ��� �� DH_PIRATEMAP_Create_main_info_stack_cache
	);//��������� ����� ������������ ����� �� ����������� �������. (���� ����� ���������� � ������, ���������� NULL, ����� - ��������� �� ���� ����)
