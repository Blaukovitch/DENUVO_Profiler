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

//------------------ define data ------------------ 
#define MAX_READ_BYTES 64
#define COMMUNICATE_CURRENT_PROCESS -1
#define MAGIC_DOS_HEADER_BYTES 0x5A4D



//++++++++++++++++++++++ structs ++++++++++++++++++++++
	typedef struct DH_PIPE_CANAL
{
	HANDLE newstdin,newstdout,read_stdout,write_stdin;
	SECURITY_ATTRIBUTES sa;
	ULONG COMMUNICATE_Th_ID;
}CANAL,*pCANAL;
    extern CANAL My_Canal;


   
//********** DH Communication func ********** //
bool DH_DO_Canal_connect();
static void WINAPI Communication_THREAD(__in LPVOID pParam);
extern unsigned long __fastcall DH_get_quick_module_address64_accessory(__in HANDLE hTarget_proc, __in HMODULE Research_Module, __in UINT64 Demand_RVA_Adress, __out TCHAR* Address_Owner_name); //return IMAGE__Section_count
bool DH_VerifyProcessAddres (__in HANDLE Target_Process, __in LPVOID Verify_Addres); //проверить существование адреса (true/false)
bool DH_ReadProcessAddres_DATA(__in HANDLE Target_Process, __in LPVOID Des_addres, __in size_t Size_of_read_content, __out LPVOID Out_buffer); //прочитать данные по указанному адресу процесса (true/false)
SERVICE_COUNT __fastcall DH_WriteProcessAddres_DATA(__in HANDLE Target_Process, __in LPVOID Des_addres, __in size_t Size_of_write_content, __out LPVOID Write_buffer);//записать данные по указанному адресу процесса (true/false)
void __fastcall DH_WriteProcessAddres_DATA_nocheck(__in LPVOID Des_addres, __in size_t Size_of_write_content, __out LPVOID Write_buffer); //записать данные по указанному адресу процесса - без VirutalQueryEx и VirtualProtect (таки быстрее)
DWORD __fastcall DH_get_PE_DUMP_image_size(__in HANDLE Target_Process, HMODULE PE_Module); //возвращает image_size указанного PE модуля в памяти
PIMAGE_DATA_DIRECTORY __fastcall DH_get_PE_DUMP_DATA_IMAGE_DIRECTORY_start(__in HANDLE Target_Process, HMODULE PE_Module); //возвращает PIMAGE_DATA_DIRECTORY указанного PE модуля в памяти
SERVICE_ADDRES __fastcall DH_get_mem_region_from_addres(__in HANDLE Target_Process, __in SERVICE_ADDRES RVA_Addres, __out SIZE_T* pRegionSize); //возвращает начало региона по требуемому адресу + его размер
PIMAGE_SECTION_HEADER DH_get_main_protection_section_params(__in ULONGLONG* pMainModule); //возвращает парметры главной секции защиты