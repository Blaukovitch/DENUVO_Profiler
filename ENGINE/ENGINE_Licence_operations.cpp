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
#include "ENGINE_Licence_operations.h"
#include "ENGINE_SPIRUN7_Communication.h"
#include "undocumented.h"

UINT DH_Licence__Get_RAW_table_data_from_current_profile( __in pT80U_Raw_licence_data pRAW_table_data_struct)
{
	
	//::GetThreadContext(HUNT_HIVE.Process_HIVE.ThreadCount[0]
	if (HUNT_HIVE.tOpenProcHANDLE == NULL && pRAW_table_data_struct == NULL)
		return NULL;

	memset(pRAW_table_data_struct,NULL, sizeof(T80U_Raw_licence_data));

	ULONG temp_reg_data_1;
	ULONG temp_reg_data_2;
	ULONG temp_reg_data_3;
	ULONG temp_reg_data_4;
	ULONG64 Hash_value;
	// >>>>>>>>>>>>>> CPUID >>>>>>>>>>>>>> 
	__asm{
		mov eax, 1
		CPUID
		lea r15, [temp_reg_data_1]
		mov [r15], eax
		lea r15, [temp_reg_data_2]
		mov [r15], ebx
		lea r15, [temp_reg_data_3]
		mov [r15], ecx
		lea r15, [temp_reg_data_4]
		mov [r15], edx
	}
	pRAW_table_data_struct->CPUID_stack.EAX_1.reg_EAX = temp_reg_data_1;
	pRAW_table_data_struct->CPUID_stack.EAX_1.reg_EBX = temp_reg_data_2;
	pRAW_table_data_struct->CPUID_stack.EAX_1.reg_ECX = temp_reg_data_3;
	pRAW_table_data_struct->CPUID_stack.EAX_1.reg_EDX = temp_reg_data_4;
	//////////////////////////////////////////// 0x80000002
		__asm{
		mov eax, 0x80000002
		CPUID
		lea r15, [temp_reg_data_1]
		mov [r15], eax
		lea r15, [temp_reg_data_2]
		mov [r15], ebx
		lea r15, [temp_reg_data_3]
		mov [r15], ecx
		lea r15, [temp_reg_data_4]
		mov [r15], edx
	}
	pRAW_table_data_struct->CPUID_stack.EAX_80000002.reg_EAX = temp_reg_data_1;
	pRAW_table_data_struct->CPUID_stack.EAX_80000002.reg_EBX = temp_reg_data_2;
	pRAW_table_data_struct->CPUID_stack.EAX_80000002.reg_ECX = temp_reg_data_3;
	pRAW_table_data_struct->CPUID_stack.EAX_80000002.reg_EDX = temp_reg_data_4;
		//////////////////////////////////////////// 0x80000003
		__asm{
		mov eax, 0x80000003
		CPUID
		lea r15, [temp_reg_data_1]
		mov [r15], eax
		lea r15, [temp_reg_data_2]
		mov [r15], ebx
		lea r15, [temp_reg_data_3]
		mov [r15], ecx
		lea r15, [temp_reg_data_4]
		mov [r15], edx
	}
	pRAW_table_data_struct->CPUID_stack.EAX_80000003.reg_EAX = temp_reg_data_1;
	pRAW_table_data_struct->CPUID_stack.EAX_80000003.reg_EBX = temp_reg_data_2;
	pRAW_table_data_struct->CPUID_stack.EAX_80000003.reg_ECX = temp_reg_data_3;
	pRAW_table_data_struct->CPUID_stack.EAX_80000003.reg_EDX = temp_reg_data_4;
		//////////////////////////////////////////// 0x80000004
		__asm{
		mov eax, 0x80000004
		CPUID
		lea r15, [temp_reg_data_1]
		mov [r15], eax
		lea r15, [temp_reg_data_2]
		mov [r15], ebx
		lea r15, [temp_reg_data_3]
		mov [r15], ecx
		lea r15, [temp_reg_data_4]
		mov [r15], edx
	}
	pRAW_table_data_struct->CPUID_stack.EAX_80000004.reg_EAX = temp_reg_data_1;
	pRAW_table_data_struct->CPUID_stack.EAX_80000004.reg_EBX = temp_reg_data_2;
	pRAW_table_data_struct->CPUID_stack.EAX_80000004.reg_ECX = temp_reg_data_3;
	pRAW_table_data_struct->CPUID_stack.EAX_80000004.reg_EDX = temp_reg_data_4;

	__asm{
		 xor rsi,rsi //esi - hash value
		 mov edi, 0x80000002
		 mov r15, 0x80000005
repeat_cpuid_hash:
		 mov eax,edi
		 CPUID
		 add eax,ebx
		 add eax,ecx
		 add eax,edx
		 add esi,eax
		 add edi, 1 //inc 
		 cmp edi, r15d //max id
		 jne repeat_cpuid_hash
		 lea r15, [Hash_value]
		 mov [r15], rsi
	asm_end_hash:
	}
	pRAW_table_data_struct->CPUID_stack.HASH = Hash_value;

		// >>>>>>>>>>>>>> KUSER_SHARED_DATA >>>>>>>>>>>>>> 
	pRAW_table_data_struct->KUSER_SHARED_DATA_stack.RVA_CryptoExponent = (PVOID)(MM_SHARED_USER_DATA_VA + defKUSER_CryptoExponen_offset);
	pRAW_table_data_struct->KUSER_SHARED_DATA_stack.RVA_NtMajorVersion = (PVOID)(MM_SHARED_USER_DATA_VA + defKUSER_NtMajorVersion_offset);
	pRAW_table_data_struct->KUSER_SHARED_DATA_stack.RVA_NtMinorVersion = (PVOID)(MM_SHARED_USER_DATA_VA + defKUSER_NtMinorVersion_offset);
	pRAW_table_data_struct->KUSER_SHARED_DATA_stack.RVA_NtSystemRoot = (PVOID)(MM_SHARED_USER_DATA_VA + defKUSER_NtSystemRoot_offset);
	pRAW_table_data_struct->KUSER_SHARED_DATA_stack.RVA_NumberOfPhysicalPages = (PVOID)(MM_SHARED_USER_DATA_VA + defKUSER_NumberOfPhysicalPages_offset);
	pRAW_table_data_struct->KUSER_SHARED_DATA_stack.RVA_ProcessorFeatures = (PVOID)(MM_SHARED_USER_DATA_VA + defKUSER_ProcessorFeatures_offset);
	pRAW_table_data_struct->KUSER_SHARED_DATA_stack.RVA_TimeSlip = (PVOID)(MM_SHARED_USER_DATA_VA + defKUSER_TimeSlip_offset);


	if (DH_VerifyProcessAddres(HUNT_HIVE.tOpenProcHANDLE,(LPVOID)MM_SHARED_USER_DATA_VA) != NULL)
	{
		PKUSER_SHARED_DATA Target_proc_KUSERSHAREDDATA_snapshot = (PKUSER_SHARED_DATA)malloc (sizeof(KUSER_SHARED_DATA));
		DH_ReadProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE, (LPVOID)MM_SHARED_USER_DATA_VA,sizeof(KUSER_SHARED_DATA),Target_proc_KUSERSHAREDDATA_snapshot);

		pRAW_table_data_struct->KUSER_SHARED_DATA_stack.NtMajorVersion = Target_proc_KUSERSHAREDDATA_snapshot->NtMajorVersion;
		pRAW_table_data_struct->KUSER_SHARED_DATA_stack.NtMinorVersion = Target_proc_KUSERSHAREDDATA_snapshot->NtMinorVersion;
		pRAW_table_data_struct->KUSER_SHARED_DATA_stack.NumberOfPhysicalPages = Target_proc_KUSERSHAREDDATA_snapshot->NumberOfPhysicalPages;
		memcpy(&pRAW_table_data_struct->KUSER_SHARED_DATA_stack.NtSystemRoot[0], &Target_proc_KUSERSHAREDDATA_snapshot->NtSystemRoot[0], sizeof(Target_proc_KUSERSHAREDDATA_snapshot->NtSystemRoot));
		memcpy(&pRAW_table_data_struct->KUSER_SHARED_DATA_stack.ProcessorFeatures[0], &Target_proc_KUSERSHAREDDATA_snapshot->ProcessorFeatures[0], sizeof(Target_proc_KUSERSHAREDDATA_snapshot->ProcessorFeatures));

		pRAW_table_data_struct->KUSER_SHARED_DATA_stack.TimeSlip = Target_proc_KUSERSHAREDDATA_snapshot->TimeSlip;
		pRAW_table_data_struct->KUSER_SHARED_DATA_stack.CryptoExponent = Target_proc_KUSERSHAREDDATA_snapshot->CryptoExponent;

		__asm
		{
			lea rbx, [Target_proc_KUSERSHAREDDATA_snapshot] //hash KUSER
			mov rbx, [rbx]
			xor rcx, rcx
			mov ecx,dword ptr ds:[rbx+0x274]
			add ecx,dword ptr ds:[rbx+0x278]
			add ecx,dword ptr ds:[rbx+0x27C]
			add ecx,dword ptr ds:[rbx+0x280]
			add ecx,dword ptr ds:[rbx+0x284]
			add ecx,dword ptr ds:[rbx+0x30]
			add ecx,dword ptr ds:[rbx+0x34]
			add ecx,dword ptr ds:[rbx+0x38]
			add ecx,dword ptr ds:[rbx+0x3C]
			add ecx,dword ptr ds:[rbx+0x40]
			add ecx,dword ptr ds:[rbx+0x44]
			add ecx,dword ptr ds:[rbx+0x48]
			add ecx,dword ptr ds:[rbx+0x4C]
			add ecx,dword ptr ds:[rbx+0x26C]
			add ecx,dword ptr ds:[rbx+0x270]
			add ecx,dword ptr ds:[rbx+0x2E8]
			lea rbx, [Hash_value]
			mov [rbx], rcx
		}
		pRAW_table_data_struct->KUSER_SHARED_DATA_stack.HASH = Hash_value;
		free(Target_proc_KUSERSHAREDDATA_snapshot);
	}


	// >>>>>>>>>>>>>> SYSTEM_LIBS x3 >>>>>>>>>>>>>> 
	//if (DH_VerifyProcessAddres(HUNT_HIVE.tOpenProcHANDLE, (LPVOID)HUNT_HIVE.Process_HIVE.Kernel32) != NULL)
		DH_Licence__Get_Lib_components(HUNT_HIVE.tOpenProcHANDLE,HUNT_HIVE.Process_HIVE.Kernel32,&pRAW_table_data_struct->IDD_system_dll_1);

		DH_Licence__Get_Lib_components(HUNT_HIVE.tOpenProcHANDLE,HUNT_HIVE.Process_HIVE.Ntdll,&pRAW_table_data_struct->IDD_system_dll_2);

	//if (DH_VerifyProcessAddres(HUNT_HIVE.tOpenProcHANDLE, (LPVOID)HUNT_HIVE.Process_HIVE.Kernelbase) != NULL)
		DH_Licence__Get_Lib_components(HUNT_HIVE.tOpenProcHANDLE,HUNT_HIVE.Process_HIVE.Kernelbase,&pRAW_table_data_struct->IDD_system_dll_3);

		// >>>>>>>>>>>>>> Process enviroment Block (PEB) >>>>>>>>>>>>>> 
	
	if (DH_VerifyProcessAddres(HUNT_HIVE.tOpenProcHANDLE, (LPVOID)HUNT_HIVE.Process_HIVE.PEB_BaseAddress) != NULL)
	{
		register pwPEB TargetrProcPeb = (pwPEB)malloc(sizeof(wPEB));
		DH_ReadProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE, (LPVOID)HUNT_HIVE.Process_HIVE.PEB_BaseAddress, sizeof(wPEB), TargetrProcPeb);
		pRAW_table_data_struct->PEB_stack.RVA_OSMajorVersion = (PVOID((UINT64)HUNT_HIVE.Process_HIVE.PEB_BaseAddress + defIPEB__OSMajorVersion_Value_offset));
		pRAW_table_data_struct->PEB_stack.RVA_OSMinorVersion = (PVOID((UINT64)HUNT_HIVE.Process_HIVE.PEB_BaseAddress + defIPEB__OSMinorVersion_Value_offset));
		pRAW_table_data_struct->PEB_stack.RVA_ImageSubSystemMajorVersion = (PVOID((UINT64)HUNT_HIVE.Process_HIVE.PEB_BaseAddress + defIPEB__ImageSubSystemMajorVersion_Value_offset));
		pRAW_table_data_struct->PEB_stack.RVA_ImageSubSystemMinorVersion = (PVOID((UINT64)HUNT_HIVE.Process_HIVE.PEB_BaseAddress + defIPEB__ImageSubSystemMinorVersion_Value_offset));
		pRAW_table_data_struct->PEB_stack.RVA_NumberOfProcessors = (PVOID((UINT64)HUNT_HIVE.Process_HIVE.PEB_BaseAddress + defIPEB__NumberOfProcessors_Value_offset));
		pRAW_table_data_struct->PEB_stack.RVA_OSPlatformId_short = (PVOID((UINT64)HUNT_HIVE.Process_HIVE.PEB_BaseAddress + defIPEB__OSPlatformId_SHORT_Value_offset));

		pRAW_table_data_struct->PEB_stack.OSMajorVersion = *(unsigned long*)((ULONG64)TargetrProcPeb + defIPEB__OSMajorVersion_Value_offset);
		pRAW_table_data_struct->PEB_stack.OSMinorVersion = *(unsigned long*)((ULONG64)TargetrProcPeb + defIPEB__OSMinorVersion_Value_offset);
		pRAW_table_data_struct->PEB_stack.ImageSubSystemMajorVersion = *(unsigned long*)((ULONG64)TargetrProcPeb + defIPEB__ImageSubSystemMajorVersion_Value_offset);
		pRAW_table_data_struct->PEB_stack.ImageSubSystemMinorVersion = *(unsigned long*)((ULONG64)TargetrProcPeb + defIPEB__ImageSubSystemMinorVersion_Value_offset);
		pRAW_table_data_struct->PEB_stack.NumberOfProcessors = *(unsigned long*)((ULONG64)TargetrProcPeb + defIPEB__NumberOfProcessors_Value_offset);
		pRAW_table_data_struct->PEB_stack.OSPlatformId_short = *(unsigned short*)((ULONG64)TargetrProcPeb + defIPEB__OSPlatformId_SHORT_Value_offset);

		__asm
		{
			lea rdi, [TargetrProcPeb]
			mov rdi, [rdi]
			xor rbx, rbx
			mov ebx,dword ptr ds:[rdi+0x118]
			add ebx,dword ptr ds:[rdi+0x11C]
			add ebx,dword ptr ds:[rdi+0x12C]
			add ebx,dword ptr ds:[rdi+0x130]
			add ebx,dword ptr ds:[rdi+0xB8]
			lea rdi, [Hash_value]
			mov [rdi], rbx
		}
		pRAW_table_data_struct->PEB_stack.HASH = Hash_value;

		free(TargetrProcPeb);	
	}
}

bool __inline DH_Licence__Get_Lib_components (__in HANDLE Targ_proc, __in HMODULE Remote_module, __in pIMAGE_DATA_DIRECTORY_licence_stack Lib_component)
{
	BYTE* PE_header = (BYTE*)malloc(LIC_MAX_PE_HEADER_SIZE);

	PIMAGE_DOS_HEADER dosHeader;        //Pointer to DOS Header
   register PIMAGE_NT_HEADERS ntHeader;         //Pointer to NT Header
    IMAGE_FILE_HEADER header;           //Pointer to image file header of NT Header 
    IMAGE_OPTIONAL_HEADER opHeader;     //Optional Header of PE files present in NT Header structure



	if (DH_ReadProcessAddres_DATA(Targ_proc, (LPVOID)Remote_module,LIC_MAX_PE_HEADER_SIZE,PE_header) == NULL)
		{free(PE_header);return false;}

	dosHeader = (PIMAGE_DOS_HEADER)PE_header;

	if(dosHeader->e_lfanew > LIC_MAX_PE_HEADER_SIZE)
		{free(PE_header);return false;}

	ntHeader = PIMAGE_NT_HEADERS64(dosHeader->e_lfanew + (ULONG64)dosHeader);
	
	//lib components RVA load
	register SERVICE_ADDRES Data_IMAGE_DIRECTORY_Remote_addr = ((SERVICE_ADDRES)Remote_module + dosHeader->e_lfanew)+defIMAGEDIRECTORY_Data_Directory_of_NtHeader_offset;

	Lib_component->RVA_ExportDirectoryVA  = (PVOID)(Data_IMAGE_DIRECTORY_Remote_addr + defIMAGEDIRECTORY_Export_Data_Directory__RVA_offset);
	Lib_component->RVA_ExportDirectorySize  = (PVOID)(Data_IMAGE_DIRECTORY_Remote_addr + defIMAGEDIRECTORY_Export_Data_Directory__Value_offset);
	Lib_component->RVA_ImportDirectoryVA  = (PVOID)(Data_IMAGE_DIRECTORY_Remote_addr + defIMAGEDIRECTORY_Import_Data_Directory__RVA_offset);
	Lib_component->RVA_ImportDirectorySize  = (PVOID)(Data_IMAGE_DIRECTORY_Remote_addr + defIMAGEDIRECTORY_Import_Data_Directory__Value_offset);
	Lib_component->RVA_ResourseDirectoryVA  = (PVOID)(Data_IMAGE_DIRECTORY_Remote_addr + defIMAGEDIRECTORY_Resource_Data_Directory__RVA_offset);
	Lib_component->RVA_ResourseDirectorySize  = (PVOID)(Data_IMAGE_DIRECTORY_Remote_addr + defIMAGEDIRECTORY_Resource_Data_Directory__Value_offset);
	Lib_component->RVA_ExceptionDirectorySize  = (PVOID)(Data_IMAGE_DIRECTORY_Remote_addr + defIMAGEDIRECTORY_Exception_Data_Directory__Value_offset);
	Lib_component->RVA_ExceptionDirectoryVA  = (PVOID)(Data_IMAGE_DIRECTORY_Remote_addr + defIMAGEDIRECTORY_Exception_Data_Directory__RVA_offset);
	Lib_component->RVA_RelocationDirectoryVA  = (PVOID)(Data_IMAGE_DIRECTORY_Remote_addr + defIMAGEDIRECTORY_Relocation_Data_Directory__RVA_offset);
	Lib_component->RVA_RelocationDirectorySize  = (PVOID)(Data_IMAGE_DIRECTORY_Remote_addr + defIMAGEDIRECTORY_Relocation_Data_Directory__Value_offset);
	Lib_component->RVA_DebugDirectoryVA  = (PVOID)(Data_IMAGE_DIRECTORY_Remote_addr + defIMAGEDIRECTORY_Debug_Data_Directory__RVA_offset);
	Lib_component->RVA_DebugDirectorySize  = (PVOID)(Data_IMAGE_DIRECTORY_Remote_addr + defIMAGEDIRECTORY_Debug_Data_Directory__Value_offset);

	//load lib VA/Size data
	Lib_component->ExportDirectoryVA = ntHeader->OptionalHeader.DataDirectory[0].VirtualAddress;
	Lib_component->ExportDirectorySize = ntHeader->OptionalHeader.DataDirectory[0].Size;
	Lib_component->ImportDirectoryVA = ntHeader->OptionalHeader.DataDirectory[1].VirtualAddress;
	Lib_component->ImportDirectorySize = ntHeader->OptionalHeader.DataDirectory[1].Size;
	Lib_component->ResourseDirectoryVA = ntHeader->OptionalHeader.DataDirectory[2].VirtualAddress;
	Lib_component->ResourseDirectorySize = ntHeader->OptionalHeader.DataDirectory[2].Size;
	Lib_component->ExceptionDirectoryVA = ntHeader->OptionalHeader.DataDirectory[3].VirtualAddress;
	Lib_component->ExceptionDirectorySize = ntHeader->OptionalHeader.DataDirectory[3].Size;
	Lib_component->RelocationDirectoryVA = ntHeader->OptionalHeader.DataDirectory[5].VirtualAddress;
	Lib_component->RelocationDirectorySize = ntHeader->OptionalHeader.DataDirectory[5].Size;
	Lib_component->DebugDirectoryVA = ntHeader->OptionalHeader.DataDirectory[6].VirtualAddress;
	Lib_component->DebugDirectorySize = ntHeader->OptionalHeader.DataDirectory[6].Size;

	DWORD Local_Hash = 
		Lib_component->ExportDirectoryVA + Lib_component->ExportDirectorySize + 
		Lib_component->ImportDirectoryVA + Lib_component->ImportDirectorySize +
		Lib_component->ResourseDirectoryVA + Lib_component->ResourseDirectorySize +
		Lib_component->ExceptionDirectoryVA + Lib_component->ExceptionDirectorySize +
		Lib_component->RelocationDirectoryVA + Lib_component->RelocationDirectorySize +
		Lib_component->DebugDirectoryVA + Lib_component->DebugDirectorySize;

	//calculate hash
	__asm
	{
		xor rax, rax
		xor rdx, rdx
		lea rsi, [PE_header]
		mov rsi, [rsi]
		mov eax,dword ptr ds:[rsi+0x3C]
		add rsi,rax
		add rsi, 0x88
		add edx,dword ptr ds:[rsi]
		add edx,dword ptr ds:[rsi+0x4]
		add edx,dword ptr ds:[rsi+0x8]
		add edx,dword ptr ds:[rsi+0xC]
		add edx,dword ptr ds:[rsi+0x10]
		add edx,dword ptr ds:[rsi+0x14]
		add edx,dword ptr ds:[rsi+0x18]
		add edx,dword ptr ds:[rsi+0x1C]
		add edx,dword ptr ds:[rsi+0x28]
		add edx,dword ptr ds:[rsi+0x2C]
		add edx,dword ptr ds:[rsi+0x30]
		add edx,dword ptr ds:[rsi+0x34]
		lea rsi, [dosHeader] //dosHeader == HASH value (2use variable)
		mov [rsi], rdx
	}
	Lib_component->HASH = (UINT64)dosHeader; ///dosHeader == HASH value (2use variable)!

	free(PE_header);
	return true;
}


SERVICE_COUNT DH_Licence__Write_dump_into_target_process ( __in pT80U_Raw_licence_data Write_DUMP)
{
	SERVICE_COUNT true_answers_count = NULL;
	//IMAGE_DATA_DIRECTORY write 10x3 = 30 ¯ÚÛÍ
	//lib #1 
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_1.RVA_DebugDirectorySize, sizeof(Write_DUMP->IDD_system_dll_1.DebugDirectorySize),&Write_DUMP->IDD_system_dll_1.DebugDirectorySize);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_1.RVA_DebugDirectoryVA, sizeof(Write_DUMP->IDD_system_dll_1.DebugDirectoryVA),&Write_DUMP->IDD_system_dll_1.DebugDirectoryVA);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_1.RVA_ExportDirectorySize, sizeof(Write_DUMP->IDD_system_dll_1.ExportDirectorySize),&Write_DUMP->IDD_system_dll_1.ExportDirectorySize);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_1.RVA_ExportDirectoryVA, sizeof(Write_DUMP->IDD_system_dll_1.ExportDirectoryVA),&Write_DUMP->IDD_system_dll_1.ExportDirectoryVA);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_1.RVA_ImportDirectorySize, sizeof(Write_DUMP->IDD_system_dll_1.ImportDirectorySize),&Write_DUMP->IDD_system_dll_1.ImportDirectorySize);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_1.RVA_ImportDirectoryVA, sizeof(Write_DUMP->IDD_system_dll_1.ImportDirectoryVA),&Write_DUMP->IDD_system_dll_1.ImportDirectoryVA);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_1.RVA_ExceptionDirectorySize, sizeof(Write_DUMP->IDD_system_dll_1.ExceptionDirectorySize),&Write_DUMP->IDD_system_dll_1.ExceptionDirectorySize);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_1.RVA_ExceptionDirectoryVA, sizeof(Write_DUMP->IDD_system_dll_1.ExceptionDirectoryVA),&Write_DUMP->IDD_system_dll_1.ExceptionDirectoryVA);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_1.RVA_RelocationDirectorySize, sizeof(Write_DUMP->IDD_system_dll_1.RelocationDirectorySize),&Write_DUMP->IDD_system_dll_1.RelocationDirectorySize);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_1.RVA_RelocationDirectoryVA, sizeof(Write_DUMP->IDD_system_dll_1.RelocationDirectoryVA),&Write_DUMP->IDD_system_dll_1.RelocationDirectoryVA);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_1.RVA_ResourseDirectorySize, sizeof(Write_DUMP->IDD_system_dll_1.ResourseDirectorySize),&Write_DUMP->IDD_system_dll_1.ResourseDirectorySize);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_1.RVA_ResourseDirectoryVA, sizeof(Write_DUMP->IDD_system_dll_1.ResourseDirectoryVA),&Write_DUMP->IDD_system_dll_1.ResourseDirectoryVA);
	//lib #2
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_2.RVA_DebugDirectorySize, sizeof(Write_DUMP->IDD_system_dll_2.DebugDirectorySize),&Write_DUMP->IDD_system_dll_2.DebugDirectorySize);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_2.RVA_DebugDirectoryVA, sizeof(Write_DUMP->IDD_system_dll_2.DebugDirectoryVA),&Write_DUMP->IDD_system_dll_2.DebugDirectoryVA);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_2.RVA_ExportDirectorySize, sizeof(Write_DUMP->IDD_system_dll_2.ExportDirectorySize),&Write_DUMP->IDD_system_dll_2.ExportDirectorySize);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_2.RVA_ExportDirectoryVA, sizeof(Write_DUMP->IDD_system_dll_2.ExportDirectoryVA),&Write_DUMP->IDD_system_dll_2.ExportDirectoryVA);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_2.RVA_ImportDirectorySize, sizeof(Write_DUMP->IDD_system_dll_2.ImportDirectorySize),&Write_DUMP->IDD_system_dll_2.ImportDirectorySize);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_2.RVA_ImportDirectoryVA, sizeof(Write_DUMP->IDD_system_dll_2.ImportDirectoryVA),&Write_DUMP->IDD_system_dll_2.ImportDirectoryVA);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_2.RVA_ExceptionDirectorySize, sizeof(Write_DUMP->IDD_system_dll_2.ExceptionDirectorySize),&Write_DUMP->IDD_system_dll_2.ExceptionDirectorySize);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_2.RVA_ExceptionDirectoryVA, sizeof(Write_DUMP->IDD_system_dll_2.ExceptionDirectoryVA),&Write_DUMP->IDD_system_dll_2.ExceptionDirectoryVA);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_2.RVA_RelocationDirectorySize, sizeof(Write_DUMP->IDD_system_dll_2.RelocationDirectorySize),&Write_DUMP->IDD_system_dll_2.RelocationDirectorySize);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_2.RVA_RelocationDirectoryVA, sizeof(Write_DUMP->IDD_system_dll_2.RelocationDirectoryVA),&Write_DUMP->IDD_system_dll_2.RelocationDirectoryVA);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_2.RVA_ResourseDirectorySize, sizeof(Write_DUMP->IDD_system_dll_2.ResourseDirectorySize),&Write_DUMP->IDD_system_dll_2.ResourseDirectorySize);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_2.RVA_ResourseDirectoryVA, sizeof(Write_DUMP->IDD_system_dll_2.ResourseDirectoryVA),&Write_DUMP->IDD_system_dll_2.ResourseDirectoryVA);
	 //lib #3
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_3.RVA_DebugDirectorySize, sizeof(Write_DUMP->IDD_system_dll_3.DebugDirectorySize),&Write_DUMP->IDD_system_dll_3.DebugDirectorySize);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_3.RVA_DebugDirectoryVA, sizeof(Write_DUMP->IDD_system_dll_3.DebugDirectoryVA),&Write_DUMP->IDD_system_dll_3.DebugDirectoryVA);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_3.RVA_ExportDirectorySize, sizeof(Write_DUMP->IDD_system_dll_3.ExportDirectorySize),&Write_DUMP->IDD_system_dll_3.ExportDirectorySize);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_3.RVA_ExportDirectoryVA, sizeof(Write_DUMP->IDD_system_dll_3.ExportDirectoryVA),&Write_DUMP->IDD_system_dll_3.ExportDirectoryVA);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_3.RVA_ImportDirectorySize, sizeof(Write_DUMP->IDD_system_dll_3.ImportDirectorySize),&Write_DUMP->IDD_system_dll_3.ImportDirectorySize);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_3.RVA_ImportDirectoryVA, sizeof(Write_DUMP->IDD_system_dll_3.ImportDirectoryVA),&Write_DUMP->IDD_system_dll_3.ImportDirectoryVA);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_3.RVA_ExceptionDirectorySize, sizeof(Write_DUMP->IDD_system_dll_3.ExceptionDirectorySize),&Write_DUMP->IDD_system_dll_3.ExceptionDirectorySize);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_3.RVA_ExceptionDirectoryVA, sizeof(Write_DUMP->IDD_system_dll_3.ExceptionDirectoryVA),&Write_DUMP->IDD_system_dll_3.ExceptionDirectoryVA);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_3.RVA_RelocationDirectorySize, sizeof(Write_DUMP->IDD_system_dll_3.RelocationDirectorySize),&Write_DUMP->IDD_system_dll_3.RelocationDirectorySize);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_3.RVA_RelocationDirectoryVA, sizeof(Write_DUMP->IDD_system_dll_3.RelocationDirectoryVA),&Write_DUMP->IDD_system_dll_3.RelocationDirectoryVA);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_3.RVA_ResourseDirectorySize, sizeof(Write_DUMP->IDD_system_dll_3.ResourseDirectorySize),&Write_DUMP->IDD_system_dll_3.ResourseDirectorySize);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->IDD_system_dll_3.RVA_ResourseDirectoryVA, sizeof(Write_DUMP->IDD_system_dll_3.ResourseDirectoryVA),&Write_DUMP->IDD_system_dll_3.ResourseDirectoryVA);
	 
	 //KUSER_SHARED_DATA - 7 ÿ“” 
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->KUSER_SHARED_DATA_stack.RVA_NtMajorVersion, sizeof(Write_DUMP->KUSER_SHARED_DATA_stack.NtMajorVersion),&Write_DUMP->KUSER_SHARED_DATA_stack.NtMajorVersion);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->KUSER_SHARED_DATA_stack.RVA_NtMinorVersion, sizeof(Write_DUMP->KUSER_SHARED_DATA_stack.NtMinorVersion),&Write_DUMP->KUSER_SHARED_DATA_stack.NtMinorVersion);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->KUSER_SHARED_DATA_stack.RVA_NtSystemRoot, sizeof(Write_DUMP->KUSER_SHARED_DATA_stack.NtSystemRoot),&Write_DUMP->KUSER_SHARED_DATA_stack.NtSystemRoot[0]);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->KUSER_SHARED_DATA_stack.RVA_NumberOfPhysicalPages, sizeof(Write_DUMP->KUSER_SHARED_DATA_stack.NumberOfPhysicalPages),&Write_DUMP->KUSER_SHARED_DATA_stack.NumberOfPhysicalPages);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->KUSER_SHARED_DATA_stack.RVA_ProcessorFeatures, sizeof(Write_DUMP->KUSER_SHARED_DATA_stack.ProcessorFeatures),&Write_DUMP->KUSER_SHARED_DATA_stack.ProcessorFeatures);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->KUSER_SHARED_DATA_stack.RVA_TimeSlip, sizeof(Write_DUMP->KUSER_SHARED_DATA_stack.TimeSlip),&Write_DUMP->KUSER_SHARED_DATA_stack.TimeSlip);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->KUSER_SHARED_DATA_stack.RVA_CryptoExponent, sizeof(Write_DUMP->KUSER_SHARED_DATA_stack.CryptoExponent),&Write_DUMP->KUSER_SHARED_DATA_stack.CryptoExponent);

	 //PEB - 6 ÿ“” 
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->PEB_stack.RVA_OSMajorVersion, sizeof(Write_DUMP->PEB_stack.OSMajorVersion),&Write_DUMP->PEB_stack.OSMajorVersion);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->PEB_stack.RVA_OSMinorVersion, sizeof(Write_DUMP->PEB_stack.OSMinorVersion),&Write_DUMP->PEB_stack.OSMinorVersion);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->PEB_stack.RVA_NumberOfProcessors, sizeof(Write_DUMP->PEB_stack.NumberOfProcessors),&Write_DUMP->PEB_stack.NumberOfProcessors);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->PEB_stack.RVA_ImageSubSystemMajorVersion, sizeof(Write_DUMP->PEB_stack.ImageSubSystemMajorVersion),&Write_DUMP->PEB_stack.ImageSubSystemMajorVersion);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->PEB_stack.RVA_ImageSubSystemMinorVersion, sizeof(Write_DUMP->PEB_stack.ImageSubSystemMinorVersion),&Write_DUMP->PEB_stack.ImageSubSystemMinorVersion);
	 true_answers_count += DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Write_DUMP->PEB_stack.RVA_OSPlatformId_short, sizeof(Write_DUMP->PEB_stack.OSPlatformId_short),&Write_DUMP->PEB_stack.OSPlatformId_short);

	 
	 return true_answers_count; //30+7+6 = (0x31)
}

bool DH_Licence_Check_KUSER_SYSTEM_TIME_update_status_from_current_profile (void)
{
	register unsigned long target_KSYSTEMTIME_tick1 = NULL;
	register unsigned long target_KSYSTEMTIME_tick2  = NULL;

	if (DH_ReadProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE, (LPVOID)defKUSER_KSYSTEMTIME1_addres,sizeof(target_KSYSTEMTIME_tick1),&target_KSYSTEMTIME_tick1) == NULL)
			return false;

	//ULONG i = MAX_REPEAT_Check_KUSER_SYSTEM_TIME_CYCLE;
	
		DH_ReadProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE, (LPVOID)defKUSER_KSYSTEMTIME1_addres,sizeof(target_KSYSTEMTIME_tick2),&target_KSYSTEMTIME_tick2);
		SleepEx(MAX_Sleep_time_in_check_KUSER_SYSTEM_TIME_CYCLE, false);
		
		//if (target_KSYSTEMTIME_tick1 == target_KSYSTEMTIME_tick2)
			//return false;

		DH_ReadProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE, (LPVOID)defKUSER_KSYSTEMTIME1_addres,sizeof(target_KSYSTEMTIME_tick1),&target_KSYSTEMTIME_tick1);
		SleepEx(MAX_Sleep_time_in_check_KUSER_SYSTEM_TIME_CYCLE, false);

		if (target_KSYSTEMTIME_tick1 == target_KSYSTEMTIME_tick2)
			return false;

return true;
}

bool DH_Licence_EMULATE_KSYSTEM_tick_from_current_profile (void)
{
	
	//::CreateRemoteThread(::GetCurrentProcess(),NULL,NULL,&,this,NULL,
}


signed long DH_Licence_CPUID_Sentinel(pCPUID_licence_stack Substitution_cpuid, pCPUID_licence_stack Compare_you_original_cpuid)
{
	//pCPUID_licence_stack MyCPUID = (pCPUID_licence_stack)malloc(sizeof(CPUID_licence_stack));


	CONTEXT cThread0;
	memset(&cThread0, NULL, sizeof(CONTEXT));

	HANDLE hThread0 = NULL;
	hThread0 = ::OpenThread(THREAD_ALL_ACCESS, false, HUNT_HIVE.Process_HIVE.Thread0.UniqueThread);
	
		if(!hThread0)
			return -1;

	
//	DWORD s = ::SuspendThread(hThread0);
	DWORD errset = NULL;
	cThread0.ContextFlags = CONTEXT_FULL;
		if (::GetThreadContext(hThread0,&cThread0) == 1)
		{
			if((cThread0.Rax == Compare_you_original_cpuid->EAX_1.reg_EAX) && (cThread0.Rcx == Compare_you_original_cpuid->EAX_1.reg_ECX) && (cThread0.Rdx == Compare_you_original_cpuid->EAX_1.reg_EDX))
				{codedef_set_context_as_substitution_eax1; codedef_execute_set_context(hThread0);}
			else
			{
				if((cThread0.Rax == Compare_you_original_cpuid->EAX_80000002.reg_EAX) && (cThread0.Rbx == Compare_you_original_cpuid->EAX_80000002.reg_EBX) && (cThread0.Rcx == Compare_you_original_cpuid->EAX_80000002.reg_ECX) && (cThread0.Rdx == Compare_you_original_cpuid->EAX_80000002.reg_EDX))
					{codedef_set_context_as_substitution_eax80000002; codedef_execute_set_context(hThread0);}
				else
				{
					if((cThread0.Rax == Compare_you_original_cpuid->EAX_80000003.reg_EAX) && (cThread0.Rbx == Compare_you_original_cpuid->EAX_80000003.reg_EBX) && (cThread0.Rcx == Compare_you_original_cpuid->EAX_80000003.reg_ECX) && (cThread0.Rdx == Compare_you_original_cpuid->EAX_80000003.reg_EDX))
						{codedef_set_context_as_substitution_eax80000003; codedef_execute_set_context(hThread0);}
					else
					{
						if((cThread0.Rax == Compare_you_original_cpuid->EAX_80000004.reg_EAX) && (cThread0.Rbx == Compare_you_original_cpuid->EAX_80000004.reg_EBX) && (cThread0.Rcx == Compare_you_original_cpuid->EAX_80000004.reg_ECX) && (cThread0.Rdx == Compare_you_original_cpuid->EAX_80000004.reg_EDX))
							{codedef_set_context_as_substitution_eax80000004; codedef_execute_set_context(hThread0);}
					}//end else #80000003
				}//end else #80000002
			}//end else #1
			//errset = ::SetThreadContext(hThread0,&cThread0);
		}
		::ResumeThread(hThread0);
	::CloseHandle(hThread0);

	if (errset == 1)
		return 1;

return 0;
}


void DH_Licence_Get_my_original_CPUID_stack(pCPUID_licence_stack You_original_cpuid)
{
	CPUID_licence_stack MyCPUID_local;
	memset(&MyCPUID_local, NULL, sizeof(MyCPUID_local));

	__asm
	{
		mov eax, 1
		cpuid
		lea r12, [MyCPUID_local.EAX_1.reg_EAX]
		mov [r12], rax
		lea r12, [MyCPUID_local.EAX_1.reg_EBX]
		mov [r12], rbx
		lea r12, [MyCPUID_local.EAX_1.reg_ECX]
		mov [r12], rcx
		lea r12, [MyCPUID_local.EAX_1.reg_EDX]
		mov [r12], rdx

		//80000002
		mov eax, 0x80000002
		cpuid
		lea r12, [MyCPUID_local.EAX_80000002.reg_EAX]
		mov [r12], rax
		lea r12, [MyCPUID_local.EAX_80000002.reg_EBX]
		mov [r12], rbx
		lea r12, [MyCPUID_local.EAX_80000002.reg_ECX]
		mov [r12], rcx
		lea r12, [MyCPUID_local.EAX_80000002.reg_EDX]
		mov [r12], rdx

		//80000003
		mov eax, 0x80000003
		cpuid
		lea r12, [MyCPUID_local.EAX_80000003.reg_EAX]
		mov [r12], rax
		lea r12, [MyCPUID_local.EAX_80000003.reg_EBX]
		mov [r12], rbx
		lea r12, [MyCPUID_local.EAX_80000003.reg_ECX]
		mov [r12], rcx
		lea r12, [MyCPUID_local.EAX_80000003.reg_EDX]
		mov [r12], rdx

		//80000004
		mov eax, 0x80000004
		cpuid
		lea r12, [MyCPUID_local.EAX_80000004.reg_EAX]
		mov [r12], rax
		lea r12, [MyCPUID_local.EAX_80000004.reg_EBX]
		mov [r12], rbx
		lea r12, [MyCPUID_local.EAX_80000004.reg_ECX]
		mov [r12], rcx
		lea r12, [MyCPUID_local.EAX_80000004.reg_EDX]
		mov [r12], rdx
	}
	memcpy(You_original_cpuid, &MyCPUID_local, sizeof(CPUID_licence_stack));
}

byte table[]=
{
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00,
0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x00, 0x00, 0x00, 0x00, 0x3F,
0x00, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

BYTE* __fastcall DH_DENUVO_Native_Decrypt(char *Input, size_t Data_area_size, DWORD* Support)
{

	__asm rdtsc
  BYTE *result; // rax@2
  __int64 v4; // rdx@3
  int v5; // [sp+20h] [bp-48h]@9
  int v6; // [sp+20h] [bp-48h]@12
  int v7; // [sp+20h] [bp-48h]@15
  int v8; // [sp+20h] [bp-48h]@18
  unsigned int v9; // [sp+24h] [bp-44h]@9
  unsigned int v10; // [sp+28h] [bp-40h]@23
  int v11; // [sp+2Ch] [bp-3Ch]@18
  int v12; // [sp+30h] [bp-38h]@12
  int v13; // [sp+34h] [bp-34h]@21
  int v14; // [sp+40h] [bp-28h]@15
  char *v15; // [sp+70h] [bp+8h]@1
//  unsigned int v16; // [sp+78h] [bp+10h]@1
  DWORD *v17; // [sp+80h] [bp+18h]@1

  v17 = Support;
  v15 = Input;
  if (!(unsigned int)Data_area_size % 4 )
    result = 0i64;


    v4 = Data_area_size % 4;
    *Support = 3 * (Data_area_size / 4);
    if ( Input[Data_area_size - 1] == 0x3D )
      --*Support;
    if ( Input[Data_area_size - 2] == 0x3D )
      --*Support;

    result = (byte*) malloc(*Support);
    if ( !result )
		result = 0i64;
//	memset(result,0, Data_area_size);

      v5 = 0;
      v9 = 0;
      while ( v5 < Data_area_size )
      {
        if ( v15[v5] == 0x3D )
        {
          v12 = 0;
          v6 = v5 + 1;
        }
        else
        {
          v12 = table[v15[v5]];
          v6 = v5 + 1;
        }
        if ( v15[v6] == 0x3D )
        {
          v14 = 0;
          v7 = v6 + 1;
        }
        else
        {
          v14 = table[v15[v6]];
          v7 = v6 + 1;
        }
        if ( v15[v7] == 0x3D )
        {
          v11 = 0;
          v8 = v7 + 1;
        }
        else
        {
          v11 = table[v15[v7]];
          v8 = v7 + 1;
        }
        if ( v15[v8] == 0x3D )
        {
          v13 = 0;
          v5 = v8 + 1;
        }
        else
        {
          v13 = table[v15[v8]];
          v5 = v8 + 1;
        }
        v10 = (v14 << 12) + (v12 << 18) + v13 + (v11 << 6);
        if ( v9 < *Support )
          result[v9++] = v10 >> 16;
        if ( v9 < *Support )
          result[v9++] = v10 >> 8;
        if ( v9 < *Support )
          result[v9++] = v13 + ((BYTE)v11 << 6);
      }
    
  return result;
}


pLIC_DLC_FileInfo_STACK __fastcall DH_Licence_Get_DLC_licence_filestack(__in bool isGet_EAOrigin, __in bool isGet_VavleSteam)
{
	pLIC_DLC_FileInfo_STACK pResult = (pLIC_DLC_FileInfo_STACK)malloc(sizeof(LIC_DLC_FileInfo_STACK));
		if(!pResult) //filter #1
			return;

	//COMMON INIT STRUCT
	pResult->NumofFoundFiles = NULL;
	pResult->EA_ORIGIN_licence = NULL;
	pResult->Vavle_Steam_licence = NULL;
	register pLIC_DLC_FILE fpFileStackUnit = &pResult->FileUnit[0]; //fp

	//PREPEARE WINAPI DATA
	register WIN32_FIND_DATAW FileFDAT;
	memset((void*)&FileFDAT,0,sizeof(WIN32_FIND_DATAW));
	HANDLE FindHandle = 0;

	//	************************	 ea origin	********************** //
	//ÒÌ‡˜‡Î‡ ÔÂÂ˜ËÒÎˇÂÏ ÍÓÎË˜ÂÒÚ‚Ó Ù‡ÈÎÓ‚
	register bool isbreakcycle = false;

	if(isGet_EAOrigin == true)
	{
		do
		{
			FindHandle = FindFirstFile(LICENCE_FULL_DEF_PATH_EXTE_FIND_EA_ORIGIN,&FileFDAT);
			TCHAR LicDir[MAX_PATH + 0x20];
					if (FindHandle == INVALID_HANDLE_VALUE)
					{
						wcscpy(&LicDir[0], LICENCE_DEF_PATH__EA_ORIGIN);
						if(DH_Licence_Enum_patch_DISK_variants(&LicDir[0]) == NULL)
						{
							isbreakcycle = true;
							break;
						}
						wcscat(&LicDir[0], LICENCE__EA_ORIGIN_DLC_LIC_EXTE_FIND);
						register TCHAR* pLicDir_endptr0 = LicDir + wcslen(LicDir); //œ≈–¬€… –¿«ƒ≈À»“≈À‹
						FindHandle = FindFirstFile(LICENCE_FULL_DEF_PATH_EXTE_FIND_EA_ORIGIN, &FileFDAT);
						*pLicDir_endptr0 = L'\0';

						if (FindHandle == INVALID_HANDLE_VALUE)
						{
							isbreakcycle = true;
							break;
						}
					}
											

				do
				{
							fpFileStackUnit->DLC_TYPE = LICENCE_DLC_EA_ORIGIN; //ea origin
							fpFileStackUnit->ApproxVersion = 0;
							fpFileStackUnit->isVerify = true;
							wcscpy(&fpFileStackUnit->Path[0], &LicDir[0]);
							memcpy(fpFileStackUnit++,&FileFDAT,sizeof(WIN32_FIND_DATAW)); //copy data
							pResult->EA_ORIGIN_licence++; //inc

				}while(FindNextFile(FindHandle,&FileFDAT) == 1); //find next cycle
				isbreakcycle = true;
				break;
		}while(isbreakcycle == false); //fakecycle
		::FindClose(FindHandle);
		//::CloseHandle(FindHandle);
	}//end if (isGet_EAOrigin == true)

	if(isGet_VavleSteam == true)
	{
		isbreakcycle = false;
		memset((void*)&FileFDAT, 0, sizeof(WIN32_FIND_DATAW));
		register TCHAR* pLicDir = (TCHAR*)malloc(sizeof(TCHAR[MAX_PATH * 2]));
		wcscpy(pLicDir, LICENCE_DEF_PATH__VALVE_STEAM);
		register TCHAR* pLicDir_endptr = pLicDir + wcslen(LICENCE_DEF_PATH__VALVE_STEAM); //œ≈–¬€… –¿«ƒ≈À»“≈À‹ ¬≈–’Õ≈√Œ œ”“»
		do
		{
			if (::GetFileAttributes(pLicDir) == INVALID_FILE_ATTRIBUTES)
			{
				if (DH_Licence_Enum_patch_DISK_variants(pLicDir) == NULL)
				{
					free(pLicDir);
					isbreakcycle = true;
					break;
				}
			}
			//‚ÂÚÍ‡ ÔÂÂ˜ËÒÎÂÌËˇ Ù‡ÈÎÓ‚
			wcscat(pLicDir, LICENCE__VALVE_STEAM_DLC_LIC_EXTE_FIND);

					FindHandle = FindFirstFile(pLicDir, &FileFDAT);
					*pLicDir_endptr = L'\0';
					do
					{
						if (FileFDAT.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
						{
							if (wcscmp(FileFDAT.cFileName, STD_DEF_DIR_RET) == 0) //filter #1
								continue;
							if (wcscmp(FileFDAT.cFileName, STD_DEF_DIR_RET_a) == 0) //filter #2
								continue;

							//‰ËÂÍÚÓËˇ <SteamID> Ì‡È‰ÂÌ‡//
							wcscat(pLicDir, FileFDAT.cFileName);
							wcscat(pLicDir, L"\\");
							register TCHAR* pLicDir_SteamID_endptr = pLicDir + wcslen(pLicDir); //¬“Œ–Œ… –¿«ƒ≈À»“≈À‹
							wcscat(pLicDir, LICENCE__VALVE_STEAM_DLC_LIC_EXTE_FIND);

								//»Ÿ≈Ã œŒƒœ¿œ » <AppID>
								register HANDLE FindHandle_SteamID = FindFirstFile(pLicDir, &FileFDAT);
								*pLicDir_SteamID_endptr = L'\0';
								if (FindHandle_SteamID != INVALID_HANDLE_VALUE)
								{
									do
									{
										if (FileFDAT.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
										{
											if (wcscmp(FileFDAT.cFileName, STD_DEF_DIR_RET) == 0) //filter #1
												continue;
											if (wcscmp(FileFDAT.cFileName, STD_DEF_DIR_RET_a) == 0) //filter #2
												continue;

											wcscat(pLicDir, FileFDAT.cFileName);
											register TCHAR* pLicDir_SteamID_AppID_endptr = pLicDir + wcslen(pLicDir);
											wcscat(pLicDir, LICENCE_DEF_SUBPATH__VALVE_STEAM_EXT_FIND);
											//»Ÿ≈Ã œ¿œ ” LICENCE_DEF_SUBPATH__VALVE_STEAM ‚ ÚÂÍÛ˘ÂÏ <AppID>

											HANDLE FindHandle_SteamID_AppID_licence = FindFirstFile(pLicDir, &FileFDAT);
											*pLicDir_SteamID_AppID_endptr = L'\0';
											if (FindHandle_SteamID_AppID_licence != INVALID_HANDLE_VALUE)
											{
												do
												{
													if (FileFDAT.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) //ÔÓÔÛÒÍ‡ÂÏ ‰ËÂÍÚÓËË
														continue;
													//************ Ì‡¯ÎË Ù‡ÈÎ˚ ÎËˆÂÌÁËË *********** ///

													fpFileStackUnit->DLC_TYPE = LICENCE_DLC_VAVLE_STEAM; //VALVE Steam
													fpFileStackUnit->ApproxVersion = 0;
													fpFileStackUnit->isVerify = true;
													wcscpy(&fpFileStackUnit->Path[0], pLicDir);
													memcpy(fpFileStackUnit++, &FileFDAT, sizeof(WIN32_FIND_DATAW)); //copy data
													pResult->Vavle_Steam_licence++; //inc

													//*pLicDir_SteamID_AppID_endptr = L'\0';
												}while (FindNextFile(FindHandle_SteamID_AppID_licence, &FileFDAT) == 1); //find next cycle
												::FindClose(FindHandle_SteamID_AppID_licence);									
											}//if (FindHandle_SteamID != INVALID_HANDLE_VALUE)
											*pLicDir_SteamID_endptr = L'\0';
										}//is directory only <AppID>
									}while (FindNextFile(FindHandle_SteamID, &FileFDAT) == 1); //find next cycle
									::FindClose(FindHandle_SteamID);		
								}//»Ÿ≈Ã œŒƒœ¿œ » <AppID>

							*pLicDir_endptr = L'\0';
						}//is directory only <SteamID>
					} while (FindNextFile(FindHandle, &FileFDAT) == 1); //find next cycle
					::FindClose(FindHandle);
					free(pLicDir);
					isbreakcycle = true;
			break;
		}while (isbreakcycle == false); //fakecycle
	}
	pResult->NumofFoundFiles = pResult->EA_ORIGIN_licence + pResult->Vavle_Steam_licence;
return pResult;
}

__forceinline TCHAR* DH_Licence_Enum_patch_DISK_variants(__in __out TCHAR* pPatch)
{
	unsigned long alphabet_count = ENGLISH_ALPHABET_COUNT;
	if (*(pPatch + 1) != L':\\')
		return 0;
	*pPatch = 'A'; //disk A..Z
	*pPatch-=1;
	do
	{
		*pPatch += 1;
		if (::GetFileAttributes(pPatch) == INVALID_FILE_ATTRIBUTES)
			continue;
		return pPatch;
	} while (--alphabet_count);

return 0;
}

pDH_LICENCE_DECODE_DLC_FILE_DATA __fastcall DH_Licence_Decode_Licence_data_from_file(__in WCHAR* pDLCFileName, __in SERVICE_TYPE LicFileType)
{
	register WCHAR* pFullPath = (WCHAR*)malloc(MAX_PATH*sizeof(WCHAR));
	if(!pFullPath) //filter #1
		return NULL;

	//WCHAR FullPath[MAX_PATH];
	switch (LicFileType)
	{
		case LICENCE_DLC_EA_ORIGIN:
			{
				wcscpy(pFullPath, LICENCE_DEF_PATH__EA_ORIGIN);
				wcscat(pFullPath, pDLCFileName);
				break;
			};
	}//end typeswitcher

	/////////////////////////// FILEREAD ////////////////////////////////////////
	HANDLE hFile = CreateFile(pFullPath,                // name of the write
                       GENERIC_READ,          // 
                       0,                      // do not share
                       NULL,                   // default security
                       OPEN_EXISTING,             // 
                       FILE_ATTRIBUTE_NORMAL,  // normal file
                       NULL);
		if(hFile == INVALID_HANDLE_VALUE)
			{free(pFullPath); return NULL;}

		LARGE_INTEGER li;
		memset(&li, 0 ,sizeof(LARGE_INTEGER));
		::GetFileSizeEx(hFile, &li);

		LPBYTE pReadBuffer = (LPBYTE)malloc(li.LowPart+16);
		ULONG ByterRead= NULL;
		*pReadBuffer = (size_t)NULL;

		::SetFilePointer(hFile, 0, 0, FILE_BEGIN); 
		::ReadFile(hFile, pReadBuffer, li.LowPart, &ByterRead, NULL);

		CloseHandle(hFile);
		free(pFullPath);

		if(ByterRead == NULL)
			return;

		/////////////////////////// RESEARCH ////////////////////////////////////////
	register pDH_LICENCE_DECODE_DLC_FILE_DATA pResult = (pDH_LICENCE_DECODE_DLC_FILE_DATA)malloc(sizeof(DH_LICENCE_DECODE_DLC_FILE_DATA));
		if(!pResult)
			return 0;

		pResult->X_TABLE_size = NULL;
		pResult->X_TABLE_DWORDs_count = NULL;
		pResult->X_TABLE_entropy = NULL;

	switch (LicFileType)
	{
		case LICENCE_DLC_EA_ORIGIN:
			{
				char* pSeekXML = NULL;
				LPDWORD pSeekXTABLE = NULL;

				bool result = EA_Origin__Full_DECODE_stage(pReadBuffer, li.LowPart, &pResult->XML_DATA[0], &pResult->XML_TagString[0], &pResult->X_TABLE[0]);
				
				if (result) //full decrypt
				{
					pX_table_intro X_FirstDWORD = (pX_table_intro)&pResult->X_TABLE[0];
					pResult->X_TABLE_DWORDs_count = X_FirstDWORD->table_dword_count;
					pResult->X_TABLE_entropy = entropy((const byte*)&pResult->X_TABLE[1], pResult->X_TABLE_DWORDs_count * sizeof(DWORD));
				}
				break;
			};
	}//end typeswitcher

free(pReadBuffer); //free filebuffer
return pResult;
}

double entropy(byte const* pSource, size_t size) {
    static unsigned int freqs[256]={0};
	memset(&freqs[0], 0, sizeof(freqs));

    register unsigned int total=0;
    for (; --size; ++pSource,++total)
        ++freqs[*pSource];
    register double result=0;
    for (size_t i=0; i<256; ++i)
        if (freqs[i]) {
            double p=(double)freqs[i]/total;
            result-=p*log(p);
        }
    return result;
}