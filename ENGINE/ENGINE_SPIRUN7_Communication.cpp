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
#include "ENGINE_SPIRUN7_Communication.h"
#include "ENGINE.h"

// ******************************************************************************************************************
// ENGINE_SPIRUN7_Communication.cpp - контакты с target-процессом
// ******************************************************************************************************************


 CANAL My_Canal;

 IMAGE_DOS_HEADER Temp_Image_DOS_header;
	IMAGE_NT_HEADERS Temp_Image_NT_headers;
	IMAGE_SECTION_HEADER Temp_Image_Section[SECTION_MAX];

static bool DH_DO_Canal_connect()
{

  if (!CreatePipe(&My_Canal.newstdin,&My_Canal.write_stdin,&My_Canal.sa,0)){return false;}
   if (!CreatePipe(&My_Canal.read_stdout,&My_Canal.newstdout,&My_Canal.sa,0)){return false;}
  // CreateRemoteThreadEx(GetCurrentProcess(),NULL,NULL, (LPTHREAD_START_ROUTINE)Communication_THREAD,(LPVOID)this,NULL,NULL, &My_Canal.COMMUNICATE_Th_ID);
 //  CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)Communication_THREAD,(LPVOID)this,NULL,&My_Canal.COMMUNICATE_Th_ID);
return true;
}

static void WINAPI Communication_THREAD(__in LPVOID pParam)
{
	//reinterpret_cast<this>(pParam);
}

unsigned long __fastcall DH_get_quick_module_address64_accessory(__in HANDLE hTarget_proc, __in HMODULE Research_Module, __in UINT64 Demand_RVA_Adress, __out TCHAR* Address_Owner_name) //return IMAGE__Section_count
{
	SIZE_T Nbs=0;
	UINT64 LO_Border, HI_Border;

 PIMAGE_DOS_HEADER dosHeader;        //Pointer to DOS Header
    PIMAGE_NT_HEADERS ntHeader;         //Pointer to NT Header
    IMAGE_FILE_HEADER header;           //Pointer to image file header of NT Header 
    IMAGE_OPTIONAL_HEADER opHeader;     //Optional Header of PE files present in NT Header structure
    PIMAGE_SECTION_HEADER pSecHeader, pSecHeader_V;   //Section Header or Section Table Header
	

	ReadProcessMemory(hTarget_proc,Research_Module,(LPVOID)&Temp_Image_DOS_header,sizeof(Temp_Image_DOS_header),&Nbs);
				    ntHeader = (PIMAGE_NT_HEADERS)((UINT64)Research_Module + Temp_Image_DOS_header.e_lfanew); //TRANSLATE TO REMOTE VIRTUAL
		ReadProcessMemory(hTarget_proc,(LPCVOID)ntHeader,(LPVOID)&Temp_Image_NT_headers,sizeof(Temp_Image_NT_headers),&Nbs);
					//pSecHeader = IMAGE_FIRST_SECTION(ntHeader);
					pSecHeader_V = (PIMAGE_SECTION_HEADER)((ULONG_PTR)(ntHeader) + FIELD_OFFSET(IMAGE_NT_HEADERS, OptionalHeader) + Temp_Image_NT_headers.FileHeader.SizeOfOptionalHeader); //TRANSLATE TO REMOTE VIRTUAL
			ReadProcessMemory(hTarget_proc,(LPCVOID)(pSecHeader_V),(LPVOID)&Temp_Image_Section[0],sizeof(Temp_Image_Section),&Nbs);
				pSecHeader = &Temp_Image_Section[0]; //TRANSLATE TO REAL
	
		

	UINT SecCount = Temp_Image_NT_headers.FileHeader.NumberOfSections;

	LO_Border = ((UINT64)Research_Module + Temp_Image_NT_headers.OptionalHeader.BaseOfCode);

		do
		{
			HI_Border = LO_Border + pSecHeader->Misc.VirtualSize;
				if ((Demand_RVA_Adress < HI_Border) && (Demand_RVA_Adress >= LO_Border))
					{
						quick_ASCII_to_UNICODE(&pSecHeader->Name[0],Address_Owner_name);
						return Temp_Image_NT_headers.FileHeader.NumberOfSections;
					}
				LO_Border = HI_Border;
		pSecHeader++;
		}while(SecCount--);
	return 0;
}


bool DH_VerifyProcessAddres (__in HANDLE Target_Process, __in LPVOID Verify_Addres)
{
	BYTE stack_buffer[MAX_READ_BYTES];

	memset(&stack_buffer[0],0,MAX_READ_BYTES);

	ULONG BytesRead = 0;

	if (!::ReadProcessMemory(Target_Process,Verify_Addres,&stack_buffer[0],MAX_READ_BYTES,(SIZE_T*)&BytesRead))
	{UINT EA = ::GetLastError();
		return false;}
	return true;
}

bool DH_ReadProcessAddres_DATA(__in HANDLE Target_Process, __in LPVOID Des_addres, __in size_t Size_of_read_content, __out LPVOID Out_buffer)
{
	SIZE_T BytesRead = 0;

	if(Size_of_read_content == 0)
		return NULL;

	return ::ReadProcessMemory(Target_Process,Des_addres,Out_buffer,Size_of_read_content,(SIZE_T*)&BytesRead);
}

SERVICE_COUNT __fastcall DH_WriteProcessAddres_DATA(__in HANDLE Target_Process, __in LPVOID Des_addres, __in size_t Size_of_write_content, __out LPVOID Write_buffer)
{
	size_t BytesWrite = 0;
	
	MEMORY_BASIC_INFORMATION ProcBasInfo;
	DWORD PAGE_protect_ERW = PAGE_EXECUTE_READWRITE;
	::VirtualQueryEx(Target_Process,Des_addres,&ProcBasInfo, sizeof(MEMORY_BASIC_INFORMATION));
	
	::VirtualProtectEx(Target_Process,Des_addres,Size_of_write_content,PAGE_protect_ERW, &ProcBasInfo.AllocationProtect);

	bool ret_status = ::WriteProcessMemory(Target_Process,Des_addres,Write_buffer,Size_of_write_content,(SIZE_T*)&BytesWrite);

	::VirtualProtectEx(Target_Process,Des_addres,Size_of_write_content,ProcBasInfo.AllocationProtect, &PAGE_protect_ERW);

	return ret_status;
}

void __fastcall DH_WriteProcessAddres_DATA_nocheck(__in LPVOID Des_addres, __in size_t Size_of_write_content, __out LPVOID Write_buffer)
{
	size_t BytesWrite;

	::WriteProcessMemory(HUNT_HIVE.tOpenProcHANDLE,Des_addres,Write_buffer,Size_of_write_content,(SIZE_T*)&BytesWrite);
}

DWORD __fastcall DH_get_PE_DUMP_image_size(__in HANDLE Target_Process, HMODULE PE_Module)
{
	PIMAGE_DOS_HEADER dosHeader;        //Pointer to DOS Header
    PIMAGE_NT_HEADERS ntHeader;         //Pointer to NT Header
    IMAGE_FILE_HEADER header;           //Pointer to image file header of NT Header 
    IMAGE_OPTIONAL_HEADER opHeader;     //Optional Header of PE files present in NT Header structure

	if (Target_Process == (HANDLE)COMMUNICATE_CURRENT_PROCESS)
	{
		dosHeader = (PIMAGE_DOS_HEADER)PE_Module;

		if(dosHeader->e_magic != MAGIC_DOS_HEADER_BYTES)
			return NULL;

		ntHeader = (PIMAGE_NT_HEADERS)((UINT64)PE_Module + dosHeader->e_lfanew);
		return ntHeader->OptionalHeader.SizeOfImage - 0x100;
	}
	return 0;
}

PIMAGE_DATA_DIRECTORY __fastcall DH_get_PE_DUMP_DATA_IMAGE_DIRECTORY_start(__in HANDLE Target_Process, HMODULE PE_Module)
{
	PIMAGE_DOS_HEADER dosHeader;        //Pointer to DOS Header
    PIMAGE_NT_HEADERS ntHeader;         //Pointer to NT Header
    IMAGE_FILE_HEADER header;           //Pointer to image file header of NT Header 
    IMAGE_OPTIONAL_HEADER opHeader;     //Optional Header of PE files present in NT Header structure

	IMAGE_DOS_HEADER str_dosHeader;        //Pointer to DOS Header
    IMAGE_NT_HEADERS str_ntHeader;         //Pointer to NT Header

	if (Target_Process == (HANDLE)COMMUNICATE_CURRENT_PROCESS)
	{
		
		dosHeader = (PIMAGE_DOS_HEADER)PE_Module;

		if(dosHeader->e_magic != MAGIC_DOS_HEADER_BYTES)
			return NULL;

		ntHeader = (PIMAGE_NT_HEADERS)((UINT64)PE_Module + dosHeader->e_lfanew);
		return ntHeader->OptionalHeader.DataDirectory;
	}
	else
	{
		if (DH_ReadProcessAddres_DATA(Target_Process, PE_Module, sizeof (IMAGE_DOS_HEADER), &str_dosHeader) != NULL)
			return 0;

		ntHeader = (PIMAGE_NT_HEADERS)((UINT64)PE_Module + dosHeader->e_lfanew);

		//if (DH_ReadProcessAddres_DATA(Target_Process, PE_Module, sizeof (IMAGE_NT_HEADERS), &str_ntHeader) != NULL)
			//return 0;
	
//		return *(ULONG64)ntHeader + (ULONG64)((ULONG64)&str_ntHeader.OptionalHeader.DataDirectory[0] - (ULONG64)&str_ntHeader);
	}
	return 0;
}

SERVICE_ADDRES __fastcall DH_get_mem_region_from_addres(__in HANDLE Target_Process, __in SERVICE_ADDRES RVA_Addres, __out SIZE_T* pRegionSize)
{
	 MEMORY_BASIC_INFORMATION MEM_BASIC;
	 memset(&MEM_BASIC,0, sizeof(MEM_BASIC));

	 if (!::VirtualQueryEx(Target_Process,(LPVOID)RVA_Addres, &MEM_BASIC,sizeof(MEMORY_BASIC_INFORMATION64)))
		 return false;
	 
	 *pRegionSize = MEM_BASIC.RegionSize;
	 return (ULONG64)MEM_BASIC.BaseAddress;
}

PIMAGE_SECTION_HEADER DH_get_main_protection_section_params(__in ULONGLONG* pMainModule)
{
	PIMAGE_SECTION_HEADER pSecHeader_vpp = HUNT_HIVE.Image_Section;

	signed long i =  HUNT_HIVE.Image_NT_headers.FileHeader.NumberOfSections;

	if(i < 2)
		return NULL;
	i--; //не учитывать последнюю секцию

	PIMAGE_SECTION_HEADER Section_point = (PIMAGE_SECTION_HEADER)pSecHeader_vpp;
	do
	{
		
		if(Section_point->SizeOfRawData < pSecHeader_vpp->SizeOfRawData)
			Section_point = pSecHeader_vpp;

		pSecHeader_vpp++;
	}while(--i);

*pMainModule = HUNT_HIVE.Image_NT_headers.OptionalHeader.ImageBase;
return Section_point;
}
