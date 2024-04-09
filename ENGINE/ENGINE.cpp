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
#include <psapi.h>
#include "ENGINE.h"
#include "ENGINE_SPIRUN7_Communication.h"
#include "ENGINE_Symbol_avanpost.h"
#include "ENGINE_MAP_operations.h"
#include "ENGINE_VM_operations.h"
#include "ENGINE_Profile_operations.h"

// ******************************************************************************************************************
// ENGINE - замещаемые операторы. блок инициализации-закрытия процесса. Глобальный HIVE для остальных модулей ENGINE
// ******************************************************************************************************************



//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ h i v e $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ //
pDH_ProcInfo DH_Hive;
 tHIVE HUNT_HIVE;
 bool HUNT_HIVE_ACTIVE;
 HUNT_Profiles HUNT_PROFILE_HIVE;
 //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ h i v e $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//

 //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ p a r s e  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ //
PARSE_ERROR Error_info;

HMODULE m_LibNtdll;
PROCESS_BASIC_INFORMATION pbi;

HIVE_AA_DATA_Block AA_intelligence;


PZWQSI pZwQuerySystemInformation;	
PZWQIP pZwQueryInformationProcess;

HANDLE DH_Current_process_fast_handle;
unsigned long DH_Current_process_fast_ID;



  UINT64 DH_GetProcess(__out UINT64 *pProcessBufferPointer)
{
	//pProcessBufferPointer = NULL;
	
	 unsigned long aProcesses[1024], cbNeeded, cProcesses;
	 memset(aProcesses,0,sizeof(aProcesses));

    unsigned long i;
	HANDLE hProcess;
	unsigned long Success_Access=NULL;
	HMODULE hMods[1024];
	 memset(hMods,0,sizeof(hMods));

	
   DH_Current_process_fast_handle = ::GetCurrentProcess();
   DH_Current_process_fast_ID = ::GetCurrentProcessId();
	 
	// if (DAT->NumberOfPhysicalPages == 0xA)
		// return true;

    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
    {
        return false;
    }
	cProcesses = cbNeeded / sizeof(DWORD);
	*pProcessBufferPointer = (UINT64)::VirtualAlloc(NULL,cProcesses*sizeof(EnumProcessInfo)+0x1000,MEM_COMMIT|MEM_COMMIT,PAGE_READWRITE);
	//pBuffer_OUT = (UINT64) malloc((cbNeeded*sizeof(DH_ProcInfo))+0x100);
	if(*pProcessBufferPointer == NULL  )
	{
		return false;
	}
	
	hProcess = (HANDLE) -1;
	DWORD processID;
	// TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
	DH_Hive = (pDH_ProcInfo) *pProcessBufferPointer;
    // Get a handle to the process.

	 //NATIVE//
	 // Получаем адрес функции ZwQuerySystemInformation из ntdll.dll
	


	if(m_LibNtdll == NULL) { m_LibNtdll = LoadLibraryEx(L"ntdll.dll",NULL,NULL); }
	 pZwQuerySystemInformation = (PZWQSI)GetProcAddress(m_LibNtdll, "ZwQuerySystemInformation");
	if(pZwQuerySystemInformation == NULL) { return 0; }
	 pZwQueryInformationProcess = (PZWQIP)GetProcAddress(m_LibNtdll, "ZwQueryInformationProcess");
	if(pZwQueryInformationProcess == NULL) { return 0; }

	// Подбираем размер буффера
	PVOID buffer_LOCAL = NULL;
	ULONG buflen = 0x8000;
	ULONG retlen = 0;
	LONG ret = 0;
	do
	{
		buffer_LOCAL = VirtualAllocEx(DH_Current_process_fast_handle, NULL,buflen, MEM_COMMIT, PAGE_READWRITE);
		if(buffer_LOCAL == 0) { return 0; }

		ret = pZwQuerySystemInformation(SYSTEM_PROCESS_THREAD_INFO_CLASS, buffer_LOCAL, buflen, &retlen);

		if(ret == STATUS_INFO_LENGTH_MISMATCH)
		{
			VirtualFreeEx(DH_Current_process_fast_handle,buffer_LOCAL, 0, MEM_RELEASE);
			buflen = buflen * 2;
		}
		else if(ret != STATUS_SUCCESS)
		{
			VirtualFreeEx(DH_Current_process_fast_handle, buffer_LOCAL, 0, MEM_RELEASE);
			return 0;
		}
	}
	while(ret == STATUS_INFO_LENGTH_MISMATCH);

		AA_intelligence.A_A = GetProcAddress(m_LibNtdll, "DbgUiRemoteBreakin");
	if(AA_intelligence.A_A == 0) { return NULL; }

	 i = cProcesses;

	 do
	 {
    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i] ); // - 0x01000L
	
	DH_Hive->ProcessID = aProcesses[i];
	
    // Get the process name
    if (NULL != hProcess )
    {
        HMODULE hMod;
        DWORD cbNeeded;
		
		 if ( EnumProcessModules( hProcess, hMods, sizeof(hMods), &cbNeeded) )
        {
			DH_Hive->Main_HMODULE = hMods[0];
			DH_Hive->ModuleCount = cbNeeded / sizeof(HMODULE);
			GetModuleBaseName( hProcess, DH_Hive->Main_HMODULE, (LPWSTR) &DH_Hive->ProcessName[0], sizeof(DH_Hive->ProcessName)/sizeof(TCHAR) );
			GetModuleInformation(hProcess, DH_Hive->Main_HMODULE,&DH_Hive->Ext_Module_Information,sizeof(MODULEINFO));
			GetModuleFileNameEx(hProcess, DH_Hive->Main_HMODULE,&DH_Hive->Process_FilePath[0],MAX_PATH);
			
			//StrCpyW(&DH_Hive->ProcessName[0],TEXT("<unknown>"));

					//FIND PROC
		PSYSTEM_PROCESS_INFORMATION pProc = (PSYSTEM_PROCESS_INFORMATION)buffer_LOCAL;
				bool pidfound = false;
					while(1)
					{
						if((unsigned long)pProc->UniqueProcessId == DH_Hive->ProcessID)
						{ pidfound = true; break; }
						if(pProc->NextEntryOffset == 0) { break; }
						else { pProc = PSYSTEM_PROCESS_INFORMATION((BYTE*)pProc + pProc->NextEntryOffset); }
					}
					memcpy (&DH_Hive->Thread0, &pProc->Threads[0].ClientId, sizeof(CLIENT_ID));
//					HANDLE th = 0;
	//				th = OpenThread(READ_CONTROL,false,pProc->Threads[0].ClientId.UniqueThread);
		//			CloseHandle(th);
					if(pidfound)
					{
						
						//GET CRITICAL INFO - PROCESS DATA
						DH_Hive->ThreadCount=pProc->NumberOfThreads;
						DH_Hive->ParentProcessID=(unsigned long)pProc->InheritedFromUniqueProcessId;
						DH_Hive->HandleCount=pProc->HandleCount;
						//memcpy(&DH_Hive->IOCounters,&pProc->IoCounters,sizeof(IO_COUNTERS));

										UINT Path_Full_lenght_and_div_pointer = wcslen(&DH_Hive->Process_FilePath[0]);
										wcscpy(&buffer[0],&DH_Hive->Process_FilePath[0]);
										do
										{
											if((buffer[Path_Full_lenght_and_div_pointer] == 0x5C)) // \ = 0x5C
											{
												buffer[++Path_Full_lenght_and_div_pointer] = 0x0L;
												break;
											}
										}while(Path_Full_lenght_and_div_pointer--);
										if ((signed long)Path_Full_lenght_and_div_pointer > 0)
										{
										//steam_api64.dll
										wcscat(&buffer[Path_Full_lenght_and_div_pointer],_T("steam_api64.dll\0"));
											if (GetFileAttributes(&buffer[0]) == INVALID_FILE_ATTRIBUTES)
												DH_Hive->Digital_distribution_client_present.is_steam_api64_present = false;
											else
												DH_Hive->Digital_distribution_client_present.is_steam_api64_present = true;

										//steamclient64.dll
										buffer[Path_Full_lenght_and_div_pointer] = 0x0L;
										wcscat(&buffer[Path_Full_lenght_and_div_pointer],_T("steamclient64.dll\0"));
											if (GetFileAttributes(&buffer[0]) == INVALID_FILE_ATTRIBUTES)
												DH_Hive->Digital_distribution_client_present.is_steamclient64_present = false;
											else
												DH_Hive->Digital_distribution_client_present.is_steamclient64_present = true;

										// Core\ActivationUI.exe
										buffer[Path_Full_lenght_and_div_pointer] = 0x0L;
										wcscat(&buffer[Path_Full_lenght_and_div_pointer],_T("Core\\ActivationUI.exe\0"));
											if (GetFileAttributes(&buffer[0]) == INVALID_FILE_ATTRIBUTES)
												DH_Hive->Digital_distribution_client_present.is_Core_ActivationUI = false;
											else
												DH_Hive->Digital_distribution_client_present.is_Core_ActivationUI = true;

										// Core\Activation.dll
										buffer[Path_Full_lenght_and_div_pointer] = 0x0L;
										wcscat(&buffer[Path_Full_lenght_and_div_pointer],_T("Core\\Activation.dll\0"));
											if (GetFileAttributes(&buffer[0]) == INVALID_FILE_ATTRIBUTES)
												DH_Hive->Digital_distribution_client_present.is_core_activation = false;
											else
												DH_Hive->Digital_distribution_client_present.is_core_activation = true;

										// 3dmgame.dll
										buffer[Path_Full_lenght_and_div_pointer] = 0x0L;
										wcscat(&buffer[Path_Full_lenght_and_div_pointer],_T("3dmgame.dll\0"));
											if (GetFileAttributes(&buffer[0]) == INVALID_FILE_ATTRIBUTES)
												DH_Hive->Digital_distribution_client_present.is_3DM_dll = false;
											else
												DH_Hive->Digital_distribution_client_present.is_3DM_dll = true;

										// 3DMGAME.ini
										buffer[Path_Full_lenght_and_div_pointer] = 0x0L;
										wcscat(&buffer[Path_Full_lenght_and_div_pointer],_T("3DMGAME.ini\0"));
											if (GetFileAttributes(&buffer[0]) == INVALID_FILE_ATTRIBUTES)
												DH_Hive->Digital_distribution_client_present.is_3DM_ini = false;
											else
												DH_Hive->Digital_distribution_client_present.is_3DM_ini = true;

										//ItsAMe_Origin.dll
										buffer[Path_Full_lenght_and_div_pointer] = 0x0L;
										wcscat(&buffer[Path_Full_lenght_and_div_pointer],_T("ItsAMe_Origin.dll\0"));
											if (GetFileAttributes(&buffer[0]) == INVALID_FILE_ATTRIBUTES)
												DH_Hive->Digital_distribution_client_present.is_ItsAMe_Origin_dll = false;
											else
												DH_Hive->Digital_distribution_client_present.is_ItsAMe_Origin_dll = true;

										//CPY.ini
										buffer[Path_Full_lenght_and_div_pointer] = 0x0L;
										wcscat(&buffer[Path_Full_lenght_and_div_pointer],_T("ItsAMe_Origin.dll\0"));
											if (GetFileAttributes(&buffer[0]) == INVALID_FILE_ATTRIBUTES)
												DH_Hive->Digital_distribution_client_present.is_CPY_ini = false;
											else
												DH_Hive->Digital_distribution_client_present.is_CPY_ini = true;	
										}
						
					}//pidfound
								if (pZwQueryInformationProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), NULL) == STATUS_SUCCESS)
								{
									DH_Hive->PEB_BaseAddress = pbi.PebBaseAddress;
								}
							DH_Hive->eXT_IMAGE_Section_Count = DH_get_quick_module_address64_accessory(hProcess,DH_Hive->Main_HMODULE,(UINT64)DH_Hive->Ext_Module_Information.EntryPoint,(TCHAR*)&DH_Hive->eXT_EP_Section_Name[0]);

				//GET INFO - A.A. & DDC
						
						for ( UINT64 iii =1; iii < (cbNeeded / sizeof(HMODULE)); iii++ )
							{
									if ( GetModuleBaseName( hProcess, hMods[iii], &buffer[0],
															  sizeof(buffer) / sizeof(TCHAR)));
									{
										if(!_wcsicmp(&buffer[0],L"NTDLL.DLL"))
										{*(UINT64*)&DH_Hive->AA_Probe.A_A = (UINT64)hMods[iii]+((UINT64)AA_intelligence.A_A-(UINT64)m_LibNtdll);
										    DH_Hive->Ntdll = hMods[iii];
											DH_Hive->AA_Probe.AA_STATE = DH_Get_AA_Process_state((LPCVOID)DH_Hive->AA_Probe.A_A,hProcess,&DH_Hive->AA_Probe.AA_Replaced_count); }
										else //not ntdll
										{
											
												if(!_wcsicmp(&buffer[0],L"3dmgame.dll")) { DH_Hive->Digital_distribution_client_present.LOADED_3DM_dll=hMods[iii];} else
													{ if(!_wcsicmp(&buffer[0],L"ItsAMe_Origin.dll")) { DH_Hive->Digital_distribution_client_present.LOADED_ItsAMe_Origin_dll=hMods[iii];} else
														{ if(!_wcsicmp(&buffer[0],L"steam_api64.dll")) { DH_Hive->Digital_distribution_client_present.LOADED_steam_api64_dll=hMods[iii];} else
															{ if(!_wcsicmp(&buffer[0],L"steamclient64.dll")) { DH_Hive->Digital_distribution_client_present.LOADED_steam_api64_dll=hMods[iii];} else
																{if(!_wcsicmp(&buffer[0],L"Activation.dll")) { DH_Hive->Digital_distribution_client_present.LOADED_core_activation_dll=hMods[iii];} else
																	{if(!_wcsicmp(&buffer[0],L"kernel32.dll")) { DH_Hive->Kernel32=hMods[iii];} else
																		{if(!_wcsicmp(&buffer[0],L"kernelbase.dll")) { DH_Hive->Kernelbase=hMods[iii];}

																		}
																	}
																}
															}
														}
													}
												
										}
									}//GetModuleBaseName
							}//for end
							
						 } //EnumProcessModules
								Success_Access++;
								DH_Hive++;
	CloseHandle( hProcess );
	}//NULL != hProcess 

    // Print the process name and identifier.

   // _tprintf( TEXT("%s  (PID: %u)\n"), szProcessName, processID );
//	OutputDebugString(&DH_Hive->ProcessName[0]);
    // Release the handle to the process.

	//unsigned long BB =cProcesses*sizeof(EnumProcessInfo)+0x1000;
	//unsigned long AA =sizeof(EnumProcessInfo);
	


	 }while(i--);

	VirtualFreeEx(DH_Current_process_fast_handle,buffer_LOCAL, 0, MEM_RELEASE);
	DH_Hive = (pDH_ProcInfo) *pProcessBufferPointer;
/*
	// Копаемся в результатах, ищем нужный процесс
	PSYSTEM_PROCESSES pProc = (PSYSTEM_PROCESSES)buffer;
	bool pidfound = false;
	while(1)
	{
		if(pProc->ProcessId == pid)
		{ pidfound = true; break; }
		if(pProc->NextEntryDelta == 0) { break; }
		else { pProc = PSYSTEM_PROCESSES((BYTE*)pProc + pProc->NextEntryDelta); }
	}

	// Если нужный пид найден - возвращаем тиды массивом
	if(pidfound)
	{
		if(pProc->ThreadCount < maxtids) // Если все влезает - возвращаем все
		{
			for(unsigned int t = 0; t < pProc->ThreadCount; t++)
			{ tids[t] = pProc->Threads[t].ClientId.UniqueThread; }
			return pProc->ThreadCount;
		}
		else // Если не влезает, возвращаем то, что влезло
		{
			for(unsigned int t = 0; t < maxtids; t++)
			{ tids[t] = pProc->Threads[t].ClientId.UniqueThread; }
			return maxtids;
		}
	}
	*/

return Success_Access;
}

  HANDLE DH_OpenDENUVO(__in TCHAR* LoadPath, __in pDH_ProcInfo tProcess_Info) //получает первоначальную инофрмацию о целевом процессе и возращает хендл процесса
  {
	   SIZE_T NBR_opn=NULL;
	 if(tProcess_Info == NULL)
	 {
		 return 0;
	 }
	 else
	 {
		 HUNT_HIVE.tis_active = true;
		memcpy(&HUNT_HIVE.Process_HIVE,tProcess_Info,sizeof(DH_ProcInfo));
		//PROCESS_ALL_ACCESS 
	HUNT_HIVE.tOpenProcHANDLE = OpenProcess(PROCESS_ALL_ACCESS , FALSE, HUNT_HIVE.Process_HIVE.ProcessID ); // - 0x01000L
		if(!HUNT_HIVE.tOpenProcHANDLE)
			return 0;
		// GET PROFILE PATH
		GetModuleFileName(GetModuleHandle(NULL),&HUNT_HIVE.PROFILE_PATH[0],MAX_PATH);
		unsigned long modPoint = wcsleave_path__cut_file_name(&HUNT_HIVE.PROFILE_PATH[0]);
//		wcscpy(&HUNT_HIVE.VMPX_PROFILE_PATH[0], &HUNT_HIVE.PROFILE_PATH[0]);

		wcscat(&HUNT_HIVE.PROFILE_PATH[0],PROFILE_FOLDER_NAME);
		wcscat(&HUNT_HIVE.PROFILE_PATH[0],&HUNT_HIVE.Process_HIVE.ProcessName[0]);
		wcscat(&HUNT_HIVE.PROFILE_PATH[0],_T("\\"));
		HUNT_HIVE.PROFILE_PATH_WCSLEN = wcslen(&HUNT_HIVE.PROFILE_PATH[0]);

	//	wcscat(&HUNT_HIVE.VMPX_PROFILE_PATH[0],PROFILE_TRACE_VMPX_PROFILER_FOLDER_NAME);
	//	HUNT_HIVE.VMPX_PROFILE_PATH_WCSLEN = wcslen(&HUNT_HIVE.VMPX_PROFILE_PATH[0]);

		if (!ReadProcessMemory(HUNT_HIVE.tOpenProcHANDLE,HUNT_HIVE.Process_HIVE.Main_HMODULE,&HUNT_HIVE.Image_DOS_header,sizeof(HUNT_HIVE.Image_DOS_header),&NBR_opn)) {CloseHandle(HUNT_HIVE.tOpenProcHANDLE); return false;}
		
		PIMAGE_NT_HEADERS pNT_Head = (PIMAGE_NT_HEADERS)((UINT64)HUNT_HIVE.Process_HIVE.Main_HMODULE + HUNT_HIVE.Image_DOS_header.e_lfanew);
		ReadProcessMemory(HUNT_HIVE.tOpenProcHANDLE,(LPCVOID)(pNT_Head),&HUNT_HIVE.Image_NT_headers,sizeof(HUNT_HIVE.Image_NT_headers),&NBR_opn);

		PIMAGE_SECTION_HEADER pSecHeader_vpp = (PIMAGE_SECTION_HEADER)((ULONG_PTR)(pNT_Head) + FIELD_OFFSET(IMAGE_NT_HEADERS, OptionalHeader)	+ HUNT_HIVE.Image_NT_headers.FileHeader.SizeOfOptionalHeader); //TRANSLATE TO REMOTE VIRTUAL

		ReadProcessMemory(HUNT_HIVE.tOpenProcHANDLE,(LPCVOID)(pSecHeader_vpp),&HUNT_HIVE.Image_Section[0],(sizeof(IMAGE_SECTION_HEADER)*HUNT_HIVE.Image_NT_headers.FileHeader.NumberOfSections),&NBR_opn);

		//init symbol avanpost

		//HUNT_HIVE.Symbol_avanpost_stack.region_size = sizeof(DH_APOST_symbol_stack) * MAX_SYMBOL_STACK_SIZE;
	//	HUNT_HIVE.Symbol_avanpost_stack.type_of_region = REGION_TYPE_SYMBOL_AVANPOST_STRUCT;
//		HUNT_HIVE.Symbol_avanpost_stack.allocated_pool = (LPBYTE)::VirtualAlloc(NULL, HUNT_HIVE.Symbol_avanpost_stack.region_size,MEM_COMMIT, PAGE_READWRITE);
		//ADDNS
		//pDH_APOST_symbol_stack pTmpSymbol = (pDH_APOST_symbol_stack)malloc(sizeof(DH_APOST_symbol_stack));
		//memset(&pTmpSymbol,NULL, sizeof(DH_APOST_symbol_stack));
		//DH_APOST_ADDNS(

		
		HUNT_HIVE_ACTIVE = true;
		return HUNT_HIVE.tOpenProcHANDLE;
	 }
	return 0;
  }

  bool DH_CloseDENUVO(HANDLE ActiveProcess) //закрывает все действия
  {
	  if(HUNT_HIVE.tOpenProcHANDLE != ActiveProcess)//нужно ли??????
		  return false; 
	  
	  //close/free stuff
	  if (HUNT_HIVE.Injection_info.A_Create_Thread_handle != NULL)
		  ::TerminateThread(HUNT_HIVE.Injection_info.A_Create_Thread_handle,-1);

	  if (HUNT_HIVE.Injection_info.pLibRemote != NULL)
		  ::VirtualFreeEx(HUNT_HIVE.tOpenProcHANDLE, (LPVOID)HUNT_HIVE.Injection_info.pLibRemote, 0, MEM_RELEASE);

	  if (HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region.allocated_pool != NULL)
		  ::VirtualFreeEx(DH_Current_process_fast_handle, (LPVOID)HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region.allocated_pool, 0, MEM_RELEASE);

	  if (HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region.region_start_addres != NULL)
		  ::VirtualFreeEx(HUNT_HIVE.tOpenProcHANDLE, (LPVOID)HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region.region_start_addres, 0, MEM_RELEASE);

	  if (HUNT_HIVE.Symbol_avanpost_stack.allocated_pool != NULL)
		  ::VirtualFreeEx(DH_Current_process_fast_handle, (LPVOID)HUNT_HIVE.Symbol_avanpost_stack.allocated_pool, 0, MEM_RELEASE);

	  if (HUNT_HIVE.PATCH_Support_region_stack.allocated_pool != NULL)
		  //::VirtualFreeEx(DH_Current_process_fast_handle, (LPVOID)HUNT_HIVE.PATCH_Support_region_stack.allocated_pool, HUNT_HIVE.PATCH_Support_region_stack.region_size, MEM_RELEASE);
		  free(HUNT_HIVE.PATCH_Support_region_stack.allocated_pool);

	  if(HUNT_HIVE.PATCH_ArbitaryFilter_VMP3new.allocated_pool != NULL)
		  ::VirtualFreeEx(DH_Current_process_fast_handle, HUNT_HIVE.PATCH_ArbitaryFilter_VMP3new.allocated_pool, 0,MEM_RELEASE); //free(HUNT_HIVE.PATCH_ArbitaryFilter_VMP2.allocated_pool);

	  if(HUNT_HIVE.PATCH_ArbitaryFilter_VMP2.allocated_pool != NULL)
		  ::VirtualFreeEx(DH_Current_process_fast_handle, HUNT_HIVE.PATCH_ArbitaryFilter_VMP2.allocated_pool, 0,MEM_RELEASE); //free(HUNT_HIVE.PATCH_ArbitaryFilter_VMP2.allocated_pool);

	  if(HUNT_HIVE.VMPX_Context_VMP2.allocated_pool != NULL)
		  ::VirtualFreeEx(DH_Current_process_fast_handle, HUNT_HIVE.VMPX_Context_VMP2.allocated_pool, 0,MEM_RELEASE);

	  if(HUNT_HIVE.VMPX_Context_VMP3new.allocated_pool != NULL)
		  ::VirtualFreeEx(DH_Current_process_fast_handle, HUNT_HIVE.VMPX_Context_VMP3new.allocated_pool,0,MEM_RELEASE);

	   if(HUNT_HIVE.VMPX_Context_VMX_D4new.allocated_pool != NULL)
		  ::VirtualFreeEx(DH_Current_process_fast_handle, HUNT_HIVE.VMPX_Context_VMX_D4new.allocated_pool, 0,MEM_RELEASE);

	//retail end
	HUNT_HIVE_ACTIVE = false;
	  CloseHandle(ActiveProcess);
	  memset((void*)&HUNT_HIVE,0, sizeof(tHIVE));
	  return true;
  }

  bool DH_Get_AA_Process_state(__in LPCVOID NTDLL_AA_RVA_ex, __in HANDLE Research_Process, __out UINT* Replaced_Count)
  {
	  SIZE_T NBR=NULL;
	
	  LARGE_INTEGER held_file_size;
		if(m_LibNtdll == NULL) { m_LibNtdll = LoadLibraryEx(L"ntdll.dll",NULL,NULL); }
			if(AA_intelligence.A_A == NULL) { AA_intelligence.A_A = GetProcAddress(m_LibNtdll, "DbgUiRemoteBreakin"); }
			if(AA_intelligence.A_A == NULL) {return false; }
		if(AA_intelligence.AA_offset == NULL) {*(UINT64*)&AA_intelligence.AA_offset = (UINT64)AA_intelligence.A_A - (UINT64)m_LibNtdll; }
			
		/*
		if (!GetWindowsDirectory(&buffer[0], MAX_PATH)) {return false;}
			wcscat(&buffer[0],_T("\\system32\\ntdll.dll"));
			*/
	if(AA_intelligence.is_AA_info_received == false)
	{
		/*
		unsigned long numofread;
		HANDLE CP = GetCurrentProcess();

		GetModuleFileNameEx(CP,m_LibNtdll,&buffer[0],MAX_PATH);
		HANDLE held_ntdll = CreateFile(&buffer[0],GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		if(held_ntdll == NULL) {return false; }
		GetFileSizeEx(held_ntdll,&held_file_size);
		LPVOID help_alloc_ntdll = VirtualAllocEx(CP,NULL,held_file_size.QuadPart+0x1000,MEM_COMMIT|MEM_COMMIT,PAGE_READWRITE);
		if(help_alloc_ntdll == NULL) {return false; }
		if(!ReadFile(held_ntdll,help_alloc_ntdll,held_file_size.QuadPart,&numofread,false)) {return false;}

		memcpy(AA_intelligence.AA_native_dump,(VOID*)((UINT64)help_alloc_ntdll + (UINT64)AA_intelligence.AA_offset),FUNC_length_HI);

		CloseHandle(held_ntdll);
		VirtualFreeEx(CP,help_alloc_ntdll,held_file_size.QuadPart+0x1000,MEM_RELEASE);
		*/

		memcpy(AA_intelligence.AA_native_dump,(VOID*)AA_intelligence.A_A,FUNC_length_HI);
		AA_intelligence.is_AA_info_received = true;
	}
	

	  if (!ReadProcessMemory(Research_Process,NTDLL_AA_RVA_ex,&Byte_buff[0],MAX_PATH,&NBR))
		 {return false;}
	  
	  UINT ggwp = CHECK_AA_BORDER;
	  Replaced_Count = NULL;
	  do{
		  if(memcmp(&Byte_buff[ggwp],&AA_intelligence.AA_native_dump[ggwp],1))
		  {
			  Replaced_Count++;
		  }
	  }while(ggwp--);
	  
	  if(!Replaced_Count) {return false;}

	  return true;
  };

  void __fastcall itoa_16_fast(ULONG64 val, TCHAR* buf)
{
        TCHAR *p;                /* pointer to traverse string */
        TCHAR *firstdig;         /* pointer to first digit */
        TCHAR temp;              /* temp char */
        UINT64 digval;        /* value of digit */

        p = buf;

		/*
        if (is_neg) {
         
            *p++ = '-';
            val = (unsigned long)(-(long)val);
        }*/

        firstdig = p;           /* save pointer to first digit */

        do {
            digval = (unsigned) (val % 16);
            val /= 16;       /* get next digit */

            /* convert to ascii and store */
            if (digval > 9)
                *p++ = (char) (digval - 10 + 'a');  /* a letter */
            else
                *p++ = (char) (digval + '0');       /* a digit */
        } while (val > 0);

        /* We now have the digit of the number in the buffer, but in reverse
           order.  Thus we reverse them now. */

        *p-- = '\0';            /* terminate string; p points to last digit */

        do {
            temp = *p;
            *p = *firstdig;
            *firstdig = temp;   /* swap *p and *firstdig */
            --p;
            ++firstdig;         /* advance to next two digits */
        } while (firstdig < p); /* repeat until halfway */
}

void __fastcall itoa_16_fast_full_DWORD(ULONG val, TCHAR* buf)
{
        TCHAR *p;                /* pointer to traverse string */
        TCHAR *firstdig;         /* pointer to first digit */
        TCHAR temp;              /* temp char */
        UINT digval;        /* value of digit */

        p = buf;

		/*
        if (is_neg) {
         
            *p++ = '-';
            val = (unsigned long)(-(long)val);
        }*/

        firstdig = p;           /* save pointer to first digit */
		ULONG i = sizeof(DWORD)*2;

        do {
            digval = (unsigned) (val % 16);
            val /= 16;       /* get next digit */

            /* convert to ascii and store */
            if (digval > 9)
                *p++ = (char) (digval - 10 + 'a');  /* a letter */
            else
                *p++ = (char) (digval + '0');       /* a digit */
        } while (i--);

        /* We now have the digit of the number in the buffer, but in reverse
           order.  Thus we reverse them now. */
		p--;
        *p-- = 0;            /* terminate string; p points to last digit */

        do {
            temp = *p;
            *p = *firstdig;
            *firstdig = temp;   /* swap *p and *firstdig */
            --p;
            ++firstdig;         /* advance to next two digits */
        } while (firstdig < p); /* repeat until halfway */
}

   
  void __fastcall itoa_10_fast(ULONG64 val, TCHAR* buf)
{
        TCHAR *p;                /* pointer to traverse string */
        TCHAR *firstdig;         /* pointer to first digit */
        TCHAR temp;              /* temp char */
        UINT64 digval;        /* value of digit */

        p = buf;

		/*
        if (is_neg) {
         
            *p++ = '-';
            val = (unsigned long)(-(long)val);
        }*/

        firstdig = p;           /* save pointer to first digit */

        do {
            digval = (unsigned) (val % 10);
            val /= 10;       /* get next digit */

            /* convert to ascii and store */
            if (digval > 9)
                *p++ = (char) (digval - 10 + 'a');  /* a letter */
            else
                *p++ = (char) (digval + '0');       /* a digit */
        } while (val > 0);

        /* We now have the digit of the number in the buffer, but in reverse
           order.  Thus we reverse them now. */

        *p-- = '\0';            /* terminate string; p points to last digit */

        do {
            temp = *p;
            *p = *firstdig;
            *firstdig = temp;   /* swap *p and *firstdig */
            --p;
            ++firstdig;         /* advance to next two digits */
        } while (firstdig < p); /* repeat until halfway */
}

  unsigned long __fastcall wcsleave_path__cut_file_name(__in __out TCHAR* Ops_String)
  {
	  SIZE_T HowWStrLen = wcslen(Ops_String);
	  Ops_String+=HowWStrLen;
	  do{
		  if(!memcmp(Ops_String,_T("\\"),sizeof(_T("\\"))/sizeof(TCHAR)))
		  {
			  memcpy(++Ops_String,_T("\0\0"),sizeof(_T("\0\0")));
			  return HowWStrLen*sizeof(TCHAR);
		  }
		  Ops_String--;
	  }while(HowWStrLen--);
return 0;
  }

__inline void  __fastcall quick_ASCII_to_UNICODE (__in BYTE* Source_str, __out TCHAR* Dest_str) //преобразование строки ASCII to UNICODE
{
	UINT a = strlen((char*)Source_str);
	if (a > MAX_PATH)
		return;

	memset(Dest_str,0,a*sizeof(TCHAR));
	do
	{
		memcpy(Dest_str++,Source_str++,1);
	}while(a--);
	memcpy(Dest_str,"\0\0",2);
}

__inline void  __fastcall quick_UNICODE_to_ASCII (__in TCHAR* Source_str, __out char* Dest_str) //преобразование строки UNICODE to ASCII
{
	register size_t a = wcslen(Source_str);
	//if (a > MAX_PATH)
		//return;

	//memset(Dest_str,0,a);
	do
	{
		memcpy(Dest_str++,Source_str++,1);
	}while(a--);
	memcpy(Dest_str,"\0\0",2);
}

__inline void  __fastcall quick_UNICODE_to_ASCII_with_ltoa16 (__in TCHAR* Source_str, __out char* Dest_str, __in TCHAR* pSource_ControlSymbol, __in SERVICE_ADDRES val) //преобразование строки UNICODE to ASCII + ltoa с 16 радиксом
{
		register size_t a = wcslen(Source_str);
	//if (a > MAX_PATH)
		//return;

	//memset(Dest_str,0,a);
	do
	{
		memcpy(Dest_str++,Source_str++,1);
		if(Source_str != pSource_ControlSymbol)
			continue;
						{//START LTOA
						 char *p;                /* pointer to traverse string */
						char *firstdig;         /* pointer to first digit */
						char temp;              /* temp char */
						ULONG64 digval;        /* value of digit */

						p = Dest_str;

						/*
						if (is_neg) {
         
							*p++ = '-';
							val = (unsigned long)(-(long)val);
						}*/

						firstdig = p;           /* save pointer to first digit */

						do {
							digval = (unsigned) (val % 16);
							val /= 16;       /* get next digit */

							/* convert to ascii and store */
							if (digval > 9)
								*p++ = (char) (digval - 10 + 'a');  /* a letter */
							else
								*p++ = (char) (digval + '0');       /* a digit */
						} while (val > 0);

						/* We now have the digit of the number in the buffer, but in reverse
						   order.  Thus we reverse them now. */
						char* save_leave = p;

						*p-- = '\0';            /* terminate string; p points to last digit */
						
						do {
							temp = *p;
							*p = *firstdig;
							*firstdig = temp;   /* swap *p and *firstdig */
							--p;
							++firstdig;         /* advance to next two digits */
						} while (firstdig < p); /* repeat until halfway */

						//copy_leave
						a-=CONTROL_SYMBOL_resistance_constant;
						Source_str++; Source_str++;//CONTROL_SYMBOL_resistance_constant
						pSource_ControlSymbol++; pSource_ControlSymbol++; //CONTROL_SYMBOL_resistance_constant
						if(a > 0)
						{
								do
								{
								memcpy(save_leave++,Source_str++,1);
								}while(--a);
						*save_leave = '\0';
						}//end if
					break;
					}//END LTOA

	}while(--a);
	//memcpy(Dest_str,"\0\0",2);
}
  
__inline void  __fastcall quick_UNICODE_to_ASCII_with_insert(__in TCHAR* Source_str, __out char* Dest_str, __in TCHAR* pSource_ControlSymbol, __in const char* pInsert) //преобразование строки UNICODE to ASCII + вставка строки
{
		register size_t a = wcslen(Source_str);
		size_t b = strlen(pInsert);
	//if (a > MAX_PATH || b > MAX_PATH)
		//return;

	//memset(Dest_str,0,a);
	do
	{
		memcpy(Dest_str++,Source_str++,1);
		if(Source_str != pSource_ControlSymbol)
			continue;
					
	
						memcpy(Dest_str, pInsert, b);
						Dest_str+=b;
						

						//copy_leave
						a-=CONTROL_SYMBOL_resistance_constant;
						Source_str++; Source_str++;//CONTROL_SYMBOL_resistance_constant
						pSource_ControlSymbol++; pSource_ControlSymbol++; //CONTROL_SYMBOL_resistance_constant
						if(a > 0)
						{
								do
								{
								memcpy(Dest_str++,Source_str++,1);
								}while(--a);
						*Dest_str = '\0';
						}//end if
					break;
				

	}while(--a);
	//memcpy(Dest_str,"\0\0",2);
}

__inline void  quick_wscpy_UNICODE_to_UNICODE_set_num_filename(__in WCHAR* pSource, __out WCHAR* pDesd, __in size_t Source_len, __in size_t IncFileNumber) //копирование UNICODE в UNICODE без расширения файла
{
	static const WCHAR name_div = L'_';
	register TCHAR *p;   /* pointer to traverse string */
	do
	{
		memcpy(pDesd++,pSource++,sizeof(WCHAR));
		if(*pSource != L'.')
			continue;
			memcpy(pDesd++, &name_div, sizeof(WCHAR));

			
						{
						
								register TCHAR *firstdig;         /* pointer to first digit */
								TCHAR temp;              /* temp char */
								register size_t digval;        /* value of digit */

								p = pDesd;


								firstdig = p;           /* save pointer to first digit */
								register size_t i = sizeof(DWORD)*2;

								do {
									digval = (unsigned) (IncFileNumber % 10);
									IncFileNumber /= 10;       /* get next digit */

									/* convert to ascii and store */
									if (digval > 9)
										*p++ = (WCHAR) (digval - 10 + 'a');  /* a letter */
									else
										*p++ = (WCHAR) (digval + '0');       /* a digit */
								} while (IncFileNumber > 0);

								/* We now have the digit of the number in the buffer, but in reverse
								   order.  Thus we reverse them now. */
								*p-- = 0;            /* terminate string; p points to last digit */

								do {
									temp = *p;
									*p = *firstdig;
									*firstdig = temp;   /* swap *p and *firstdig */
									--p;
									++firstdig;         /* advance to next two digits */
								} while (firstdig < p); /* repeat until halfway */


						}

					break;
	}while(--Source_len);
}

UINT64 __fastcall wtou64_16_fast(TCHAR* src) //
{
	byte tmp_buff[64];
	
  static const byte byte_alphabet[] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,1,2,3,4,5,6,7,8,9,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xA,0xB,0xC,0xD,0xE,0xF,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xA, 0xB,0xC,0xD, 0xE, 0xF};
LPWSTR next_src=src++;
register size_t size = wcslen(src);
		if(size == NULL)
			return NULL;

memset(&tmp_buff[0],0,sizeof(tmp_buff));
byte* dst = &tmp_buff[0];

  while (--size)
  {
         *dst++ = (byte_alphabet[*src]) | (byte_alphabet[*next_src]<< 4);
   src+=2;
   next_src+=2;            
  };
  UINT64 AA = *(UINT64*)&tmp_buff[0];
  htonl(AA);
  return *(UINT64*)&tmp_buff[0];
              

	
	wcstol(&buffer[0],(wchar_t**)&buffer[0],16);
	return 0;
}

UINT64 __fastcall wtou64_16_fast__NO_SWAP_BYTES(TCHAR* src) //no byteswap
{
	byte tmp_buff[64];
	
  static const byte byte_alphabet[] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,1,2,3,4,5,6,7,8,9,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xA,0xB,0xC,0xD,0xE,0xF,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xA, 0xB,0xC,0xD, 0xE, 0xF};

register size_t size = (wcslen(src));
		if(size == NULL)
			return NULL;

LPWSTR next_src= src+size-2;
LPWSTR prew_src = src+size-1;
memset(&tmp_buff[0],0,sizeof(tmp_buff));
byte* dst = &tmp_buff[0];

  while (src <= prew_src)
  {
         *dst++ = (byte_alphabet[*prew_src]) | (byte_alphabet[*next_src]<< 4);
   prew_src-=2;
   next_src-=2;            
  };
  
  return *(UINT64*)&tmp_buff[0];
}

void bytes_to_TCHAR_string(BYTE src_byte, TCHAR* desd_string) //out in dig_buffer
{
			DWORD val=(DWORD)src_byte;
			
		TCHAR *p;                /* pointer to traverse string */
        TCHAR *firstdig;         /* pointer to first digit */
        TCHAR temp;              /* temp char */

		 p = desd_string;
		 
         unsigned long digval;  
  
		
        unsigned int f=0;


    firstdig = p;           /* save pointer to first digit */

      do {
            digval = (unsigned) (val % 16);
            val /= 16;       /* get next digit */

            /* convert to ascii and store */
            if (digval > 9)
                *p++ = (char) (digval - 10 + 'A');  /* a letter */
            else
                *p++ = (char) (digval + '0');       /* a digit */
        } while (val > 0);

      

        /* We now have the digit of the number in the buffer, but in reverse
           order.  Thus we reverse them now. */

	if(p<&desd_string[2])
	{
	*p++='0';
	}
	


	*p--='\0';



        do {
            temp = *p;
            *p = *firstdig;
            *firstdig = temp;   /* swap *p and *firstdig */
            --p;
            ++firstdig;         /* advance to next two digits */
        } while (firstdig < p); /* repeat until halfway */

}

 long reverse(long i) {
       // HD, Figure 7-1
    i = (i & 0x5555555555555555L) << 1 | (i >> 1) & 0x5555555555555555L;
    i = (i & 0x3333333333333333L) << 2 | (i >> 2) & 0x3333333333333333L;
    i = (i & 0x0f0f0f0f0f0f0f0fL) << 4 | (i >> 4) & 0x0f0f0f0f0f0f0f0fL;
    i = (i & 0x00ff00ff00ff00ffL) << 8 | (i >> 8) & 0x00ff00ff00ff00ffL;
    i = (i << 48) | ((i & 0xffff0000L) << 16) |
        ((i >> 16) & 0xffff0000L) | (i >> 48);
    return i;
  }

 /*
 bool CloseProcessMainThread(DWORD dwProcID)
{
  DWORD dwMainThreadID = 0;
  ULONGLONG ullMinCreateTime = MAXULONGLONG;

  HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
  if (hThreadSnap != INVALID_HANDLE_VALUE) {
    THREADENTRY32 th32;
    th32.dwSize = sizeof(THREADENTRY32);
    BOOL bOK = TRUE;
    for (bOK = Thread32First(hThreadSnap, &th32); bOK;
         bOK = Thread32Next(hThreadSnap, &th32)) {
      if (th32.th32OwnerProcessID == dwProcID) {
        HANDLE hThread = OpenThread(THREAD_QUERY_INFORMATION,
                                    TRUE, th32.th32ThreadID);
        if (hThread) {
          FILETIME afTimes[4] = {0};
          if (GetThreadTimes(hThread,
                             &afTimes[0], &afTimes[1], &afTimes[2], &afTimes[3])) {
            ULONGLONG ullTest = MAKEULONGLONG(afTimes[0].dwLowDateTime,
                                              afTimes[0].dwHighDateTime);
            if (ullTest && ullTest < ullMinCreateTime) {
              ullMinCreateTime = ullTest;
              dwMainThreadID = th32.th32ThreadID; // let it be main... :)
            }
          }
          CloseHandle(hThread);
        }
      }
    }
#ifndef UNDER_CE
    CloseHandle(hThreadSnap);
#else
    CloseToolhelp32Snapshot(hThreadSnap);
#endif
  }

  if (dwMainThreadID) {
    PostThreadMessage(dwMainThreadID, WM_QUIT, 0, 0); // close your eyes...
  }

  return (0 != dwMainThreadID);
}
*/

   HMODULE DH_GetModuleHANDLE(__in  TCHAR* pModuleFileName)
{
HMODULE hMods[1024];
DWORD cbNeeded = NULL;
TCHAR* modname = (TCHAR*)malloc(sizeof(TCHAR)*MAX_PATH); //TCHAR ModName[MAX_PATH]

		 if ( EnumProcessModules(HUNT_HIVE.tOpenProcHANDLE, hMods, sizeof(hMods), &cbNeeded) )
        {
			unsigned long icount = cbNeeded / sizeof(HMODULE);
			do
			{
				GetModuleBaseName( HUNT_HIVE.tOpenProcHANDLE, hMods[icount], modname, sizeof(TCHAR)*MAX_PATH);

				if(!_wcsicmp(modname,pModuleFileName))
				{free(modname); return hMods[icount];}

			}while(icount--);
		}

		 free(modname);
return NULL;
}


SERVICE_HASH __forceinline DH_Calc_area_HASH(LPDWORD Area_start, size_t area_size)
{
	register SERVICE_HASH local_Hash = NULL;

	size_t f = area_size % sizeof(DWORD);
	area_size = area_size / sizeof(DWORD);
	do
	{
		local_Hash+=*(Area_start++);
	}while(area_size--);

	switch(f) //f - остаток от деления на DWORD
	{
	case 1: {local_Hash = local_Hash + *(LPBYTE)(Area_start); break;}
	case 2: {local_Hash = local_Hash + *(LPWORD)(Area_start); break;}
	case 3: {local_Hash = local_Hash + *(LPWORD)(Area_start); Area_start = Area_start + sizeof(WORD); local_Hash = local_Hash + *(LPBYTE)(Area_start); break;}
	}

return local_Hash;
}

 SERVICE_COUNT DH_Get_Target_PE_Section(PIMAGE_SECTION_HEADER* pSectionList)
 {
	 if(HUNT_HIVE_ACTIVE == false)
		 return NULL;
	 *pSectionList = &HUNT_HIVE.Image_Section[0];
return HUNT_HIVE.Image_NT_headers.FileHeader.NumberOfSections;
 }

 pDH_ProcInfo DH_Get_Target_Process_information(void)
 {
	 if(HUNT_HIVE_ACTIVE == false)
		 return 0;
return &HUNT_HIVE.Process_HIVE;
 }


__forceinline void  __fastcall DH_DISASM_to_XED_string(char* DISASM_str, char* XED_str)
{
	register size_t d = strlen(DISASM_str);
	if(d == NULL)
		return;

	//char* SaveLastPreOperand_symbol_position;
	char* firstsymbolsave = DISASM_str;
	do
	{
		//if(*DISASM_str == ' ')
			//SaveLastPreOperand_symbol_position = DISASM_str;

		if(*DISASM_str == 'h')
		{
			if(*(DISASM_str-1) >= '0' && *(DISASM_str-1) <= 'F')
				*DISASM_str = ' ';
		}

		DISASM_str++;
	}while(d--);

	strcpy(XED_str, firstsymbolsave);
}

__forceinline SERVICE_ID DH_Convert_Bea__to__DH_CPU_register(__in ULONG Bea_Register)
{
	switch(Bea_Register)
	{
		case REG0: {return INCREMENTAL_CPU_RAX;};
		case REG1: {return INCREMENTAL_CPU_RCX;};
		case REG2: {return INCREMENTAL_CPU_RDX;};
		case REG3: {return INCREMENTAL_CPU_RBX;};
		case REG4: {return INCREMENTAL_CPU_RSP;};
		case REG5: {return INCREMENTAL_CPU_RBP;};
		case REG6: {return INCREMENTAL_CPU_RSI;};
		case REG7: {return INCREMENTAL_CPU_RDI;};
		case REG8: {return INCREMENTAL_CPU_R8;};
		case REG9: {return INCREMENTAL_CPU_R9;};
		case REG10: {return INCREMENTAL_CPU_R10;};
		case REG11: {return INCREMENTAL_CPU_R11;};
		case REG12: {return INCREMENTAL_CPU_R12;};
		case REG13: {return INCREMENTAL_CPU_R13;};
		case REG14: {return INCREMENTAL_CPU_R14;};
		case REG15: {return INCREMENTAL_CPU_R15;};
	}//end switch
return NULL;
}

bool DH__INIT_DENUVO_PROFILER(void)
{
		GetModuleFileName(GetModuleHandle(NULL),&HUNT_PROFILE_HIVE.VMPX_PROFILE_PATH[0],MAX_PATH);
		unsigned long modPoint = wcsleave_path__cut_file_name(&HUNT_PROFILE_HIVE.VMPX_PROFILE_PATH[0]);
		wcscat(&HUNT_PROFILE_HIVE.VMPX_PROFILE_PATH[0],PROFILE_TRACE_VMPX_PROFILER_FOLDER_NAME);
		HUNT_PROFILE_HIVE.VMPX_PROFILE_PATH_WCSLEN = wcslen(&HUNT_PROFILE_HIVE.VMPX_PROFILE_PATH[0]);
return true;
}
bool DH__Install_HUNT_Profile(__in WCHAR* pProfileFileName, SERVICE_TYPE MainProfileType, SERVICE_TYPE SubType)
{
	register WCHAR* Dest_str_set;//строка приёмник
	register size_t* Dest_str_set_len; //длина строки приёмника
	DWORD reta= NULL;
	UINT Read_TYPE_PROFILE;
	size_t f = wcslen(pProfileFileName);

	if(f == 0)
		return false;

	switch(MainProfileType)
	{
		case DH_INSTALL_PROFILE__VMPX_Context: 
			{
				switch(SubType)
				{
				case VM_TYPE_VMP2: {Dest_str_set = &HUNT_PROFILE_HIVE.VMPX_Context_vmp2[0]; Dest_str_set_len = &HUNT_PROFILE_HIVE.VMPX_Context_vmp2_len; break;}
				case VM_TYPE_VMP3: {Dest_str_set = &HUNT_PROFILE_HIVE.VMPX_Context_vmp3new[0]; Dest_str_set_len = &HUNT_PROFILE_HIVE.VMPX_Context_vmp3new_len;break;}
				case VM_TYPE_VMX_D4: {Dest_str_set = &HUNT_PROFILE_HIVE.VMPX_Context_D4[0]; Dest_str_set_len = &HUNT_PROFILE_HIVE.VMPX_Context_D4_len; break;}
				case VM_TYPE_VMX_D5: {Dest_str_set = &HUNT_PROFILE_HIVE.VMPX_Context_D5[0]; Dest_str_set_len = &HUNT_PROFILE_HIVE.VMPX_Context_D5_len; break;}
				case VM_TYPE_ANOTHER: {Dest_str_set = &HUNT_PROFILE_HIVE.VMPX_Context_ANOTHER[0]; Dest_str_set_len = &HUNT_PROFILE_HIVE.VMPX_Context_ANOTHER_len; break;}
				default: {return false; break;}
				}//end SubType switch
				Read_TYPE_PROFILE = PROFILE_TYPE_VM_VMPX_CONTEXT_INI;
			break;}//end MAIN case DH_INSTALL_PROFILE__VMPX_Context

		case DH_INSTALL_PROFILE__PATH_Arbitary_filter: 
			{
				switch(SubType)
				{
				case VM_TYPE_VMP2: {Dest_str_set = &HUNT_PROFILE_HIVE.PATH_Arbitary_filter_vmp2[0]; break;}
				case VM_TYPE_VMP3: {Dest_str_set = &HUNT_PROFILE_HIVE.PATH_Arbitary_filter_vmp3new[0];break;}
				case VM_TYPE_VMX_D4: {Dest_str_set = &HUNT_PROFILE_HIVE.PATH_Arbitary_filter_D4[0]; break;}
				case VM_TYPE_VMX_D5: {Dest_str_set = &HUNT_PROFILE_HIVE.PATH_Arbitary_filter_D5[0]; break;}
				case VM_TYPE_ANOTHER: {Dest_str_set = &HUNT_PROFILE_HIVE.PATH_Arbitary_filter_ANOTHER[0]; break;}
				default: {return false; break;}
				}//end SubType switch
				Read_TYPE_PROFILE = PROFILE_TYPE_PATCH_FILTER_ASM_STACK_INI;
			break;} //end MAIN case DH_INSTALL_PROFILE__PATH_Arbitary_filter
		default: {return false;}
	}//end swithc MainProfileType


	register pVM_VMPX_PROFILE_INI_STACK pRaw_VMPX_Profiler = (pVM_VMPX_PROFILE_INI_STACK)DH_Read_Current_hunt_profile_CONTAINER(Read_TYPE_PROFILE, pProfileFileName,PROFILE_READ_ALL,(PULONG)&reta); //read x_profiler

				if(pRaw_VMPX_Profiler == NULL)
					return false;
		//SUCCESS!!!
		::VirtualFree(pRaw_VMPX_Profiler, 0, MEM_RELEASE); //free(pRaw_VMPX_Profiler);
		wcscpy(Dest_str_set, pProfileFileName);
		*Dest_str_set_len = f;
return true;
}


// ERRORS PARSE