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
#include "ENGINE_SPIRUN7.h"
#include "ENGINE.h"
#include "PROFILER_ENGINE.h"
#include "ENGINE_Profile_operations.h"
#include "ENGINE_VM_operations.h"
#include "ENGINE_MAP_operations.h"
#include "ENGINE_Symbol_avanpost.h"

// ******************************************************************************************************************
// ENGINE_Profile_operations.cpp - любые операции с текущем профилем
// ******************************************************************************************************************
/*/
template <typename  case_id, char* input, char* qword_cpu_reg, char* dword_cpu_reg, char* word_cpu_reg, char* byte_cpu_reg> class T_instrict_registers 
{
		case case_id:
						{
							switch(Set_size_typedef_for_CS_REGISTER)
							{
								case CS_REGISTER_TYPEF_SIZE_QWORD:{input = qword_cpu_reg; break;}
								case CS_REGISTER_TYPEF_SIZE_DWORD:{input = dword_cpu_reg; break;}
								case CS_REGISTER_TYPEF_SIZE_WORD:{input = word_cpu_reg; break;}
								case CS_REGISTER_TYPEF_SIZE_BYTE:{input = byte_cpu_reg; break;}
							}
						break;}
}*/

/* OLD
#define codedef_register_switcher(case_id, input, qword_cpu_reg, dword_cpu_reg, word_cpu_reg, byte_cpu_reg)\
					case case_id:\
						{\
							switch(Set_size_typedef_for_CS_REGISTER)\
							{\
								case CS_REGISTER_TYPEF_SIZE_QWORD:{input = qword_cpu_reg; break;}\
								case CS_REGISTER_TYPEF_SIZE_DWORD:{input = dword_cpu_reg; break;}\
								case CS_REGISTER_TYPEF_SIZE_WORD:{input = word_cpu_reg; break;}\
								case CS_REGISTER_TYPEF_SIZE_BYTE:{input = byte_cpu_reg; break;}\
							}\
						break;}	
			*/	
#define codedef_register_switcher(case_id, input, qword_cpu_reg, dword_cpu_reg, word_cpu_reg, byte_cpu_reg)\
							switch(Set_size_typedef_for_CS_REGISTER)\
							{\
								case CS_REGISTER_TYPEF_SIZE_QWORD:{input = qword_cpu_reg; break;}\
								case CS_REGISTER_TYPEF_SIZE_DWORD:{input = dword_cpu_reg; break;}\
								case CS_REGISTER_TYPEF_SIZE_WORD:{input = word_cpu_reg; break;}\
								case CS_REGISTER_TYPEF_SIZE_BYTE:{input = byte_cpu_reg; break;}\
								}\
										if(Inset_register)\
											{\
												quick_UNICODE_to_ASCII_with_insert(pAsmInstruction, pXED_parse_asm->instr, fpAsmInstruction, input);\
												if (XEDParseAssemble(pXED_parse_asm) != XEDPARSE_ERROR)\
												{\
											\
														if(!memcmp(pInstruction_pool, &pXED_parse_asm->dest[0], pXED_parse_asm->dest_size))\
															{*pCS_latest_instead_CPU_Register = case_id; return true;}\
												}\
											}

#define codedef_register_switcher_no_write(case_id, input, qword_cpu_reg, dword_cpu_reg, word_cpu_reg, byte_cpu_reg)\
							switch(Set_size_typedef_for_CS_REGISTER)\
							{\
								case CS_REGISTER_TYPEF_SIZE_QWORD:{input = qword_cpu_reg; break;}\
								case CS_REGISTER_TYPEF_SIZE_DWORD:{input = dword_cpu_reg; break;}\
								case CS_REGISTER_TYPEF_SIZE_WORD:{input = word_cpu_reg; break;}\
								case CS_REGISTER_TYPEF_SIZE_BYTE:{input = byte_cpu_reg; break;}\
								}\
										if(Inset_register)\
											{\
												quick_UNICODE_to_ASCII_with_insert(pAsmInstruction, pXED_parse_asm->instr, fpAsmInstruction, input);\
												if (XEDParseAssemble(pXED_parse_asm) != XEDPARSE_ERROR)\
												{\
											\
														if(!memcmp(pInstruction_pool, &pXED_parse_asm->dest[0], pXED_parse_asm->dest_size))\
															return true;\
												}\
											}\


 WCHAR DataBuffer[] = PROFILE_READFILE_CONTENT;

 // T M P   P R O F I L E
 WCHAR TMP_Profle_Folder[MAX_PATH*2];

 // P R O F I L E PATH
 //WCHAR HUNT_HIVE.PROFILE_PATH[MAX_PATH*2];
 //ULONG HUNT_HIVE.PROFILE_PATH_WCSLEN;

bool DH_InitProfiles_Engine_Path(LPWSTR Profile_path) //инициализация профилей
{
	memset(&TMP_Profle_Folder[0],0,sizeof(TMP_Profle_Folder));
	::GetModuleFileNameW(::GetModuleHandleW(NULL),&TMP_Profle_Folder[0],MAX_PATH);
	long a = wcslen(TMP_Profle_Folder);
	do
	{
		if(TMP_Profle_Folder[a] == _T('\\'))
		{
			a++;
			TMP_Profle_Folder[a]=0;
			break;
		}
	}while(a--);
	memcpy(Profile_path,&TMP_Profle_Folder[0],a*sizeof(WCHAR));
	wcscat(Profile_path,PROFILE_FOLDER_NAME);
	wcscat(&TMP_Profle_Folder[0],PROFILE_FOLDER_NAME);

	DWORD Path_Attr=0;
	Path_Attr = ::GetFileAttributesW(Profile_path);
	if (Path_Attr == FILE_ATTRIBUTE_DIRECTORY) //папка существует?
		return true;

 if (!::CreateDirectory(Profile_path,NULL))
	 return false;
 
 wcscat(TMP_Profle_Folder,PROFILE_READFILE_NAME);

 //CREATE PROFILE Readme file////////////////////////////////////////////////////////
  HANDLE hFile = CreateFile(TMP_Profle_Folder,                // name of the write
                       GENERIC_WRITE,          // open for writing
                       0,                      // do not share
                       NULL,                   // default security
                       CREATE_NEW,             // create new file only
                       FILE_ATTRIBUTE_NORMAL,  // normal file
                       NULL);
   if (hFile == INVALID_HANDLE_VALUE) 
     return false;

   DWORD dwBytesToWrite = wcslen(DataBuffer) * sizeof(WCHAR);
   DWORD dwBytesWritten = 0;
  bool bErrorFlag = WriteFile( 
                    hFile,           // open file handle
                    DataBuffer,      // start of data to write
                    dwBytesToWrite,  // number of bytes to write
                    &dwBytesWritten, // number of bytes that were written
                    NULL);            // no overlapped structure
  CloseHandle(hFile);
   if (FALSE == bErrorFlag)
        return false;
   
return true;
}
bool DH_Is_HAVE_hunt_Profile(pDH_ProcInfo Target_Process, LPWSTR Profile_path)  //имеется ли профиль
{
	Target_Process->ProcessName;

	DH_Profile_Verify_Folders(PROFILE_TYPE_VM);
	DH_Profile_Verify_Folders(PROFILE_TYPE_PATCH__NOT_APPLIED_FLAG);
	DH_Profile_Verify_Folders(PROFILE_TYPE_LIC_DATA_DUMP);
	DH_Profile_Verify_Folders(PROFILE_TYPE_TRACE_VMP2_LOG);
	DH_Profile_Verify_Folders(PROFILE_TYPE_PATCH_REGIONS_INFO_INI);
	DH_Profile_Verify_Folders(PROFILE_TYPE_VM_BILLET);
	DH_Profile_Verify_Folders(PROFILE_TYPE_PIRATE_MAP);

	A_INIT_TEMP_BUFFER_STRING;
	DWORD Path_Attr_Have = ::GetFileAttributesW(&TMP_Profle_Folder[0]);
	if (Path_Attr_Have == FILE_ATTRIBUTE_DIRECTORY) //папка существует?
		return true;
	
	return false;
}

ULONG DH_SetCurrentHuntProfile(pDH_ProcInfo Target_Process, LPWSTR Profile_path) //установить HIVE профиль
{
	A_INIT_BUFFER_PROFILE_STRING;
	HUNT_HIVE.PROFILE_PATH_WCSLEN = wcslen(&HUNT_HIVE.PROFILE_PATH[0]);
	
	DWORD Path_Attr_Have = ::GetFileAttributesW(&HUNT_HIVE.PROFILE_PATH[0]);
	if (Path_Attr_Have == FILE_ATTRIBUTE_DIRECTORY) //папка существует?
		return false;

	return HUNT_HIVE.PROFILE_PATH_WCSLEN;
}

ULONG DH_GetCurrentHuntProfile(LPWSTR Profile_path)  //вернуть HIVE профиль
{
	wcscpy(Profile_path, &HUNT_HIVE.PROFILE_PATH[0]);
		return HUNT_HIVE.PROFILE_PATH_WCSLEN;
}

PVOID DH_Read_Current_hunt_profile_CONTAINER(__in UINT PROFILE_x_TYPE, __in WCHAR* PROFILE_x_CONTAINT_FILE_NAME, __in size_t num_bytes_of_read, __out PULONG Return_Readed_Bytes)  //считывает данные HIVE профиля с типом
{
	//reinterpret_cast<byte*>(&DECODED_raw_id)
	WIN32_FIND_DATAW FileFDAT;
	memset((void*)&FileFDAT,0,sizeof(WIN32_FIND_DATAW));
	HANDLE FindHandle = 0;

	bool in_filename_ext_preset = DH_LOCAL_is_filename_containt_profile_extension(PROFILE_x_TYPE, PROFILE_x_CONTAINT_FILE_NAME);

	register LPCWSTR ScanFileName;
	switch(PROFILE_x_TYPE)
	{
	case PROFILE_TYPE_VM: 
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_VM_nfo_FOLDER_NAME);

		if(in_filename_ext_preset == false)
			wcscat(PROFILE_x_CONTAINT_FILE_NAME, PROFILE_VM_nfo_EXTENTION_CRITICAL);

		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_x_CONTAINT_FILE_NAME);
		/*
			if(PROFILE_x_CONTAINT_FILE_NAME == (WCHAR*)PROFILE_FIRST_FILE)
				FindHandle = FindFirstFile(&HUNT_HIVE.PROFILE_PATH[0],&FileFDAT);
				if (FindHandle == INVALID_HANDLE_VALUE)
					goto error;
				CloseHandle(FindHandle);
				*/
			codedef_set_default_ops_filename(&HUNT_HIVE.PROFILE_PATH[0]);
		break;
		}

	case PROFILE_TYPE_PATCH__NOT_APPLIED_FLAG: 
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_PATCH_nfo_FOLDER_NAME);
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_PATCH_nfo__NOT_APPLIED_FOLDER_NAME);

		if(in_filename_ext_preset == false)
			wcscat(PROFILE_x_CONTAINT_FILE_NAME, PROFILE_PATCH_nfo_EXTENTION_CRITICAL);

		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_x_CONTAINT_FILE_NAME);
		/*
			if(PROFILE_x_CONTAINT_FILE_NAME == (WCHAR*)PROFILE_FIRST_FILE)
				FindHandle = FindFirstFile(&HUNT_HIVE.PROFILE_PATH[0],&FileFDAT);
				if (FindHandle == INVALID_HANDLE_VALUE)
					goto error;
				CloseHandle(FindHandle);
				*/
		codedef_set_default_ops_filename(&HUNT_HIVE.PROFILE_PATH[0]);
		break;
		}
	
	case PROFILE_TYPE_PATCH_ONLINE: 
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_PATCH_nfo_FOLDER_NAME);

		if(in_filename_ext_preset == false)
			wcscat(PROFILE_x_CONTAINT_FILE_NAME, PROFILE_PATCH_nfo_EXTENTION_CRITICAL);

		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_x_CONTAINT_FILE_NAME);
		/*
			if(PROFILE_x_CONTAINT_FILE_NAME == (WCHAR*)PROFILE_FIRST_FILE)
				FindHandle = FindFirstFile(&HUNT_HIVE.PROFILE_PATH[0],&FileFDAT);
				if (FindHandle == INVALID_HANDLE_VALUE)
					goto error;
				CloseHandle(FindHandle);
				*/
		codedef_set_default_ops_filename(&HUNT_HIVE.PROFILE_PATH[0]);
		break;
		}

	case PROFILE_TYPE_LIC_DATA_DUMP: 
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_LIC_DATA_DUMP_nfo_FOLDER_NAME);
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_x_CONTAINT_FILE_NAME);
		codedef_set_default_ops_filename(&HUNT_HIVE.PROFILE_PATH[0]);
		break;
		}

	case PROFILE_TYPE_VM_ADDRES_POOL:
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_VM_nfo_FOLDER_NAME);

		if(in_filename_ext_preset == false)
			wcscat(PROFILE_x_CONTAINT_FILE_NAME, PROFILE_VM_addres_pool_EXTENTION_CRITICAL);

		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_x_CONTAINT_FILE_NAME);
		codedef_set_default_ops_filename(&HUNT_HIVE.PROFILE_PATH[0]);
			break;
		}

		case PROFILE_TYPE_TRACE_VMP2_LOG:
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_TRACE_VMP2_LOG_nfo_FOLDER_NAME);

		if(in_filename_ext_preset == false)
			wcscat(PROFILE_x_CONTAINT_FILE_NAME, PROFILE_TRACE_VMP2_LOG_nfo_EXTENTION_CRITICAL);

		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_x_CONTAINT_FILE_NAME);
		codedef_set_default_ops_filename(&HUNT_HIVE.PROFILE_PATH[0]);
			break;
		}

		case PROFILE_TYPE_PATCH_REGIONS_INFO_INI:
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_PATCH_nfo_FOLDER_NAME);
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_PATCH_nfo_CONFIG_FILE_REGION_INI);
		codedef_set_default_ops_filename(&HUNT_HIVE.PROFILE_PATH[0]);
			break;
		}
		case PROFILE_TYPE_PATCH_FILTER_ASM_STACK_INI:
		{
	
			if(in_filename_ext_preset == false)
			wcscat(PROFILE_x_CONTAINT_FILE_NAME, PROFILE_TRACE_VMPX_EXTENTION_CRITICAL);

		wcscat(&HUNT_PROFILE_HIVE.VMPX_PROFILE_PATH[0], PROFILE_x_CONTAINT_FILE_NAME);
		codedef_set_default_ops_filename(&HUNT_PROFILE_HIVE.VMPX_PROFILE_PATH[0]);
			break;
		}
		case PROFILE_TYPE_VM_VMPX_CONTEXT_INI:
		{
			if(in_filename_ext_preset == false)
			wcscat(PROFILE_x_CONTAINT_FILE_NAME, PROFILE_VM_VMPX_EXTENTION_CRITICAL);

		wcscat(&HUNT_PROFILE_HIVE.VMPX_PROFILE_PATH[0], PROFILE_x_CONTAINT_FILE_NAME);
		codedef_set_default_ops_filename(&HUNT_PROFILE_HIVE.VMPX_PROFILE_PATH[0]);
			break;
		}

		case PROFILE_TYPE_VM_BILLET:
		{
			if(in_filename_ext_preset == false)
			wcscat(PROFILE_x_CONTAINT_FILE_NAME, PROFILE_VM_VMPX_EXTENTION_CRITICAL);

		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_VM_nfo_FOLDER_NAME);
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_VM_BILLET_FOLDER_NAME); 
		codedef_set_default_ops_filename(&HUNT_HIVE.PROFILE_PATH[0]);
			break;
		}

		case PROFILE_TYPE_PIRATE_MAP:
		{
			if (in_filename_ext_preset == false)
			wcscat(PROFILE_x_CONTAINT_FILE_NAME, PROFILE_PIRATE_MAP_nfo_EXTENTION_CRITICAL);

		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_PIRATE_MAP_nfo_FOLDER_NAME);
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_x_CONTAINT_FILE_NAME);
		codedef_set_default_ops_filename(&HUNT_HIVE.PROFILE_PATH[0]);
			break;
		}
	default: {__assume(0); return false;}
	}
	

	if(::GetFileAttributesW(ScanFileName) == (DWORD)INVALID_HANDLE_VALUE)
			{
				*Return_Readed_Bytes = NULL;
				A_RETURN_ORIGINAL_PROFLE_PATH;
				A_RETURN_ORIGINAL_VMPX_PROFLE_PATH;
				return false;
			}

	 HANDLE hFile = CreateFile(ScanFileName,                // name of the write
                       GENERIC_READ,          // 
                       0,                      // do not share
                       NULL,                   // default security
                       OPEN_EXISTING,             // 
                       FILE_ATTRIBUTE_NORMAL,  // normal file
                       NULL);
		   if (hFile == INVALID_HANDLE_VALUE) 
		   {
				*Return_Readed_Bytes = NULL;
				A_RETURN_ORIGINAL_PROFLE_PATH;
				A_RETURN_ORIGINAL_VMPX_PROFLE_PATH;
				return false;
		   }

   
   if(num_bytes_of_read == PROFILE_READ_ALL)
   {ULONG sizlimits = 0x100000;num_bytes_of_read =  ::GetFileSize(hFile,&sizlimits);}

  // if(num_bytes_of_read < sizeof(PROFILER_VM))
   //return false;

  PVOID allocate = malloc(num_bytes_of_read+1); //allocate mem
  //PVOID allocate = ::VirtualAlloc(NULL, num_bytes_of_read, MEM_COMMIT, PAGE_READWRITE);

   bool bErrorFlag  = ReadFile(hFile,allocate,num_bytes_of_read,Return_Readed_Bytes,NULL);
   ::CloseHandle(hFile);
      if (bErrorFlag == false)
         	{
				*Return_Readed_Bytes = NULL;
				A_RETURN_ORIGINAL_PROFLE_PATH;
				A_RETURN_ORIGINAL_VMPX_PROFLE_PATH;
				return false;
			}

	  //update critical struct data /////////////////////////////
	  switch(PROFILE_x_TYPE)
	{
	case PROFILE_TYPE_PATCH__NOT_APPLIED_FLAG: 
		{
	//		pPATCH_INFO_STACK patch_info_allocate = (pPATCH_INFO_STACK)allocate;
	//		patch_info_allocate->Containt_HELD_GROUP[0] = pPATCH_HELD_GROUP(ULONG64(allocate) + sizeof(PATCH_INFO_STACK));
		break;
		}
	case PROFILE_TYPE_PATCH_ONLINE: 
		{
		//	pPATCH_INFO_STACK patch_info_allocate = (pPATCH_INFO_STACK)allocate;
		//	patch_info_allocate->Containt_HELD_GROUP[0] = pPATCH_HELD_GROUP(ULONG64(allocate) + sizeof(PATCH_INFO_STACK));
		break;
		}
	case PROFILE_TYPE_PATCH_REGIONS_INFO_INI:
		{
			TCHAR ValueInfo[MAX_PATH];
			pPATCH_SUPPORTING_REGION_INI_STACK patch_info_allocate = (pPATCH_SUPPORTING_REGION_INI_STACK)allocate;
			// -------------- read VMP_Section
			memset(&ValueInfo,0, sizeof(QWORD)*2);
			::GetPrivateProfileString(PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section,
				PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_name_key,
				NULL,
				&patch_info_allocate->VMP_SectionName[0],
				MAX_PATH,
				&HUNT_HIVE.PROFILE_PATH[0]);

			memset(&ValueInfo,0, sizeof(QWORD)*2);
			::GetPrivateProfileString(PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section,
				PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_size_key,
				TS_DEF_NULL,
				&ValueInfo[0],
				MAX_PATH,
				&HUNT_HIVE.PROFILE_PATH[0]);

			if(wcsicmp(PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_Auto_Alloc_value, &ValueInfo[0]))
					//patch_info_allocate->VMP_Section_size = wtou64_16_fast__NO_SWAP_BYTES(&ValueInfo[0]);
					patch_info_allocate->VMP_Section_size = _wcstoui64(&ValueInfo[0],NULL,16);
			else
					patch_info_allocate->VMP_Section_size = PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_Auto_Alloc_default;

			memset(&ValueInfo,0, sizeof(QWORD)*2);
			::GetPrivateProfileString(PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section,
				PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_address_key,
				TS_DEF_NULL,
				&ValueInfo[0],
				MAX_PATH,
				&HUNT_HIVE.PROFILE_PATH[0]);

			if(wcsicmp(PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_Auto_Alloc_value, &ValueInfo[0]))
					patch_info_allocate->VMP_Section_start_address = _wcstoui64(&ValueInfo[0],NULL,16);
			else
					patch_info_allocate->VMP_Section_start_address = PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_Auto_Alloc_default;
			
			// -------------- read VMP_Section_original_copy
			memset(&ValueInfo,0, sizeof(QWORD)*2);
			::GetPrivateProfileString(PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_original_copy,
				PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_original_copy_start_address_key,
				TS_DEF_NULL,
				&ValueInfo[0],
				MAX_PATH,
				&HUNT_HIVE.PROFILE_PATH[0]);

			if(wcsicmp(PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_Auto_Alloc_value, &ValueInfo[0]))
			{patch_info_allocate->Region_VMP_original_copy_start = _wcstoui64(&ValueInfo[0],NULL,16); patch_info_allocate->is_Region_VMP_original_copy_auto_alloc = false;}
			else
			{patch_info_allocate->is_Region_VMP_original_copy_auto_alloc = true; patch_info_allocate->Region_VMP_original_copy_start = PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_Auto_Alloc_default;}

			memset(&ValueInfo,0, sizeof(QWORD)*2);
			::GetPrivateProfileString(PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_original_copy,
				PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_original_copy_end_address_key,
				TS_DEF_NULL,
				&ValueInfo[0],
				MAX_PATH,
				&HUNT_HIVE.PROFILE_PATH[0]);

			if(wcsicmp(PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_Auto_Alloc_value, &ValueInfo[0]))
					patch_info_allocate->Region_VMP_original_copy_end = _wcstoui64(&ValueInfo[0],NULL,16);
			else
					patch_info_allocate->Region_VMP_original_copy_end = PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_Auto_Alloc_default;

			
			memset(&ValueInfo,0, sizeof(QWORD)*2);
			::GetPrivateProfileString(PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_original_copy,
				PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_original_copy_is_req_memcpy_original_section_flag_key,
				TS_DEF_NULL,
				&ValueInfo[0],
				MAX_PATH,
				&HUNT_HIVE.PROFILE_PATH[0]);

			size_t temp_f = wtou64_16_fast(&ValueInfo[0]);
			if(temp_f > TRUE) //wtou64_16_fast FILTER
				temp_f = false;

			patch_info_allocate->is_Region_VMP_original_copy_REQ_MEMCPY = (bool)temp_f;
			
			// -------------- read VMP_patch_map
			memset(&ValueInfo,0, sizeof(QWORD));
			::GetPrivateProfileString(PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_patch_map,
				PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_patch_map_start_address_key,
				TS_DEF_NULL,
				&ValueInfo[0],
				MAX_PATH,
				&HUNT_HIVE.PROFILE_PATH[0]);

			if(wcsicmp(PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_Auto_Alloc_value, &ValueInfo[0]))
				{patch_info_allocate->Region_VMP_patch_map_start = _wcstoui64(&ValueInfo[0],NULL,16); patch_info_allocate->is_Region_VMP_patch_map_auto_alloc = false;}
			else
				{patch_info_allocate->is_Region_VMP_patch_map_auto_alloc = true; patch_info_allocate->Region_VMP_patch_map_start = PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_Auto_Alloc_default;}

			memset(&ValueInfo,0, sizeof(QWORD)*2);
			::GetPrivateProfileString(PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_patch_map,
				PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_patch_map_end_address_key,
				TS_DEF_NULL,
				&ValueInfo[0],
				MAX_PATH,
				&HUNT_HIVE.PROFILE_PATH[0]);

			if(wcsicmp(PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_Auto_Alloc_value, &ValueInfo[0]))
					patch_info_allocate->Region_VMP_patch_map_end = _wcstoui64(&ValueInfo[0],NULL,16);
			else
					patch_info_allocate->Region_VMP_patch_map_end = PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_Auto_Alloc_default;

			memset(&ValueInfo,0, sizeof(QWORD)*2);
			::GetPrivateProfileString(PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_patch_map,
				PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_patch_map_lockmarker_key,
				TS_DEF_NULL,
				&ValueInfo[0],
				MAX_PATH,
				&HUNT_HIVE.PROFILE_PATH[0]);

			if(wcsicmp(PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_Auto_Alloc_value, &ValueInfo[0]))
					patch_info_allocate->Region_VMP_patch_map_LOCK_byte = (BYTE)wtou64_16_fast(&ValueInfo[0]);
			else
					patch_info_allocate->Region_VMP_patch_map_LOCK_byte = PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_Auto_Alloc_default;

			// -------------- read DH_hooks_and_filters
			memset(&ValueInfo,0, sizeof(QWORD)*2);
			::GetPrivateProfileString(PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_DH_hooks_and_filters,
				PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_DH_hooks_and_filters_start_address_key,
				TS_DEF_NULL,
				&ValueInfo[0],
				MAX_PATH,
				&HUNT_HIVE.PROFILE_PATH[0]);

			if(wcsicmp(PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_Auto_Alloc_value, &ValueInfo[0]))
				{patch_info_allocate->Region_VMP_hook_and_filter_start = _wcstoui64(&ValueInfo[0],NULL,16); patch_info_allocate->is_Region_VMP_hook_and_filter_auto_alloc = false;}
			else
				{patch_info_allocate->is_Region_VMP_hook_and_filter_auto_alloc = true; patch_info_allocate->Region_VMP_hook_and_filter_start = PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_Auto_Alloc_default;}

			memset(&ValueInfo,0, sizeof(QWORD)*2);
			::GetPrivateProfileString(PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_DH_hooks_and_filters,
				PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_DH_hooks_and_filters_end_address_key,
				TS_DEF_NULL,
				&ValueInfo[0],
				MAX_PATH,
				&HUNT_HIVE.PROFILE_PATH[0]);

			if(wcsicmp(PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_Auto_Alloc_value, &ValueInfo[0]))
					patch_info_allocate->Region_VMP_hook_and_filter_end = _wcstoui64(&ValueInfo[0],NULL,16);
			else
					patch_info_allocate->Region_VMP_hook_and_filter_end = PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_Auto_Alloc_default;
	
			memset(&ValueInfo,0, sizeof(QWORD)*2);
			::GetPrivateProfileString(PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_DH_hooks_and_filters,
				PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_DH_hooks_and_filters_float_pointer_key,
				TS_DEF_NULL,
				&ValueInfo[0],
				MAX_PATH,
				&HUNT_HIVE.PROFILE_PATH[0]);

			if(wcsicmp(PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_Auto_Alloc_value, &ValueInfo[0]))
					patch_info_allocate->Region_VMP_float_pointer = _wcstoui64(&ValueInfo[0],NULL,16);
			else
					patch_info_allocate->Region_VMP_float_pointer = PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_Auto_Alloc_default;
			break;
		}//end PROFILE_TYPE_PATCH_REGIONS_INFO_INI
			case PROFILE_TYPE_PATCH_FILTER_ASM_STACK_INI:
		{
				allocate = DH_Profile_PARSE_Arbitary_FILTER_CONFIG((TCHAR*)allocate, *Return_Readed_Bytes);
			break;
		}//end PROFILE_TYPE_PATCH_FILTER_ASM_STACK_INI
		case PROFILE_TYPE_VM_VMPX_CONTEXT_INI:
		{
				allocate = DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG((TCHAR*)allocate, *Return_Readed_Bytes, (TCHAR*)ScanFileName);
			break;
		}//end PROFILE_TYPE_VM_VMPX_CONTEXT_INI
	 }//end SWITCH
	  // ends of update /////////////////////////////////////////

	  A_RETURN_ORIGINAL_PROFLE_PATH;
	  A_RETURN_ORIGINAL_VMPX_PROFLE_PATH;
	  return allocate;
    	
}

bool DH_Write_Current_hunt_profile_CONTAINER(__in UINT PROFILE_x_TYPE, __in WCHAR* PROFILE_x_CONTAINT_FILE_NAME, __in BYTE* write_buff, __in size_t num_bytes_to_write, __out PULONG Return_Writed_Bytes)  //записывает данные HIVE профиля с типом
{
	if(num_bytes_to_write == NULL)
		return false;

	register bool in_filename_ext_preset = DH_LOCAL_is_filename_containt_profile_extension(PROFILE_x_TYPE, PROFILE_x_CONTAINT_FILE_NAME);

	switch(PROFILE_x_TYPE)
	{
	case PROFILE_TYPE_VM: 
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_VM_nfo_FOLDER_NAME);
		if (in_filename_ext_preset == false)
			wcscat(PROFILE_x_CONTAINT_FILE_NAME, PROFILE_VM_nfo_EXTENTION_CRITICAL);
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_x_CONTAINT_FILE_NAME);
			break;
		}
	case PROFILE_TYPE_PATCH_ONLINE:
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_PATCH_nfo_FOLDER_NAME);
		if (in_filename_ext_preset == false)
			wcscat(PROFILE_x_CONTAINT_FILE_NAME, PROFILE_PATCH_nfo_EXTENTION_CRITICAL);
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_x_CONTAINT_FILE_NAME);
			break;
		}

	case PROFILE_TYPE_PATCH__NOT_APPLIED_FLAG:
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_PATCH_nfo_FOLDER_NAME);
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_PATCH_nfo__NOT_APPLIED_FOLDER_NAME); 
		if (in_filename_ext_preset == false)
			wcscat(PROFILE_x_CONTAINT_FILE_NAME, PROFILE_PATCH_nfo_EXTENTION_CRITICAL);
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_x_CONTAINT_FILE_NAME);
			break;
		}
	case PROFILE_TYPE_VM_ADDRES_POOL:
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_VM_nfo_FOLDER_NAME);
		if (in_filename_ext_preset == false)
			wcscat(PROFILE_x_CONTAINT_FILE_NAME, PROFILE_VM_addres_pool_EXTENTION_CRITICAL);
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_x_CONTAINT_FILE_NAME);
			break;
		}
	case PROFILE_TYPE_TRACE_VMP2_LOG:
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_TRACE_VMP2_LOG_nfo_FOLDER_NAME);
		if (in_filename_ext_preset == false)
			wcscat(PROFILE_x_CONTAINT_FILE_NAME, PROFILE_TRACE_VMP2_LOG_nfo_EXTENTION_CRITICAL);
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_x_CONTAINT_FILE_NAME);
			break;
		}
	case PROFILE_TYPE_PATCH_REGIONS_INFO_INI:
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_PATCH_nfo_FOLDER_NAME);
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_PATCH_nfo_CONFIG_FILE_REGION_INI);
			break;
		}
		case PROFILE_TYPE_VM_BILLET:
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_VM_nfo_FOLDER_NAME);
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_VM_BILLET_FOLDER_NAME); 
		if (in_filename_ext_preset == false)
			wcscat(PROFILE_x_CONTAINT_FILE_NAME, PROFILE_VM_BILLET_EXTENTION_CRITICAL);
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_x_CONTAINT_FILE_NAME);
			break;
		}

		case PROFILE_TYPE_PIRATE_MAP:
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_PIRATE_MAP_nfo_FOLDER_NAME);
		if (in_filename_ext_preset == false)
			wcscat(PROFILE_x_CONTAINT_FILE_NAME, PROFILE_PIRATE_MAP_nfo_EXTENTION_CRITICAL);
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_x_CONTAINT_FILE_NAME);
			break;
		}
	}

		register HANDLE hFile = CreateFile(&HUNT_HIVE.PROFILE_PATH[0],                // name of the write
                       GENERIC_WRITE,          // 
                       FILE_SHARE_WRITE,       // WRITE
                       NULL,                   // default security
                       CREATE_ALWAYS,             // NEW
                       FILE_ATTRIBUTE_NORMAL,  // normal file
                       NULL);
		 if (hFile == INVALID_HANDLE_VALUE) //try rewrite
		 			goto error1;
		

		bool bErrorFlag =  ::WriteFile(hFile,write_buff,num_bytes_to_write,Return_Writed_Bytes,NULL);

     ::CloseHandle(hFile);
      if (bErrorFlag == false)
         goto error1;

	  A_RETURN_ORIGINAL_PROFLE_PATH;
	  return true;

error1:
   A_RETURN_ORIGINAL_PROFLE_PATH;
	return false;
}


bool DH_CreateNewHuntProfile(__in pDH_ProcInfo Target_Process, __in LPWSTR Profile_path)  //создать профиль
{
	A_INIT_TEMP_BUFFER_STRING; 
	wcscat(&TMP_Profle_Folder[0], L"\\");
	UINT str_len = wcslen(&TMP_Profle_Folder[0]);
	::CreateDirectoryW(&TMP_Profle_Folder[0], NULL); //GENERATE MAIN DIRECTORY of PROFILE

	DWORD Path_Attr_Have = ::GetFileAttributesW(&TMP_Profle_Folder[0]);
	if (Path_Attr_Have != FILE_ATTRIBUTE_DIRECTORY) //папка существует?
		return false;

	A_RETURN_ORIGINAL_TEMPORARY_PATH(str_len);
	wcscat(&TMP_Profle_Folder[0], PROFILE_VM_nfo_FOLDER_NAME);
	::CreateDirectoryW(&TMP_Profle_Folder[0], NULL); //generate VM folder
	

	return true;
}

SERVICE_COUNT DH_Get_Current_hunt_profile_ALL_FILE_CONTAINERS(__in UINT PROFILE_x_TYPE,__out DH_ENUM_FILES_of_PROFILES File_names_buffer[])  //перечислить все файлы профиля
{
	register WIN32_FIND_DATAW FileFDAT;
	memset((void*)&FileFDAT,0,sizeof(WIN32_FIND_DATAW));
	HANDLE FindHandle = 0;

	switch(PROFILE_x_TYPE)
	{
	case PROFILE_TYPE_VM: 
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_VM_nfo_FOLDER_NAME); 
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_VM_nfo_EXTE_FIND);
			//goto FILE_FOUND;
			break;
		}
	case PROFILE_TYPE_PATCH__NOT_APPLIED_FLAG:
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_PATCH_nfo_FOLDER_NAME); 
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_PATCH_nfo__NOT_APPLIED_FOLDER_NAME); 
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_PATCH_nfo_EXTE_FIND);
		//goto FILE_FOUND;
		break;
		}
	case PROFILE_TYPE_LIC_DATA_DUMP:
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_LIC_DATA_DUMP_nfo_FOLDER_NAME); 
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_LIC_DATA_DUMP_nfo_EXTE_FIND);
		//goto FILE_FOUND;
		break;
		}
	case PROFILE_TYPE_VM_ADDRES_POOL: 
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_VM_nfo_FOLDER_NAME); 
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_VM_addres_pool_EXTE_FIND);
			//goto FILE_FOUND;
			break;
		}
	case PROFILE_TYPE_TRACE_VMP2_LOG:
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_TRACE_VMP2_LOG_nfo_FOLDER_NAME);
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_TRACE_VMP2_LOG_nfo_EXTE_FIND);
			break;
		}
	case PROFILE_TYPE_PATCH_REGIONS_INFO_INI:
		{
			return NULL; //n/a for config .ini file
		}

	case PROFILE_TYPE_VM_BILLET:
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_VM_nfo_FOLDER_NAME); 
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_VM_BILLET_FOLDER_NAME);
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_VM_BILLET_EXTE_FIND);
			break;
		}
	case PROFILE_TYPE_PIRATE_MAP:
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_PIRATE_MAP_nfo_FOLDER_NAME);
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_PIRATE_MAP_nfo_EXTE_FIND);
			break;
		}

	default:
		{return  NULL;}
	}
	//return  NULL

	//new ingine fileparser
	size_t Number_of_valid_files = 0;
	register bool isbreakcycle = false;
	register pDH_ENUM_FILES_of_PROFILES fpEnumFilesStack = &File_names_buffer[0];
		do
		{
			FindHandle = FindFirstFile(&HUNT_HIVE.PROFILE_PATH[0],&FileFDAT);
					if (FindHandle == INVALID_HANDLE_VALUE)
							{isbreakcycle = true; break;}

				do
				{
					if(wcslen(FileFDAT.cFileName) > PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN)
							continue;

					wcscpy(&fpEnumFilesStack->FileName[0],&FileFDAT.cFileName[0]);
					Number_of_valid_files++; //inc
					fpEnumFilesStack++; //inc
				}while(FindNextFile(FindHandle,&FileFDAT) == 1); //find next cycle
				isbreakcycle = true;
				break;
		}while(isbreakcycle == false); //fakecycle
		::FindClose(FindHandle);
		/* old fileparser
FILE_FOUND:
	//сначала перечисляем количество файлов 
	size_t Number_of_valid_files = 0;
		FindHandle = FindFirstFile(&HUNT_HIVE.PROFILE_PATH[0],&FileFDAT);
				if (FindHandle == INVALID_HANDLE_VALUE)
					goto error_DH_Get_Current_hunt_profile_ALL_FILE_CONTAINERS;

			size_t i = wcslen(FileFDAT.cFileName);
					if(i > PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN)
						goto skip_file;
						
						memcpy(&File_names_buffer[Number_of_valid_files].FileName[0],&FileFDAT.cFileName[0],sizeof(FILE_ENUM_NAMES));
						Number_of_valid_files++;
	skip_file:
				if (FindNextFile(FindHandle,&FileFDAT) == 1)
				{
					i = wcslen(FileFDAT.cFileName);
					if(i > PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN)
						goto skip_file;
						memcpy(&File_names_buffer[Number_of_valid_files].FileName[0],&FileFDAT.cFileName[0],sizeof(FILE_ENUM_NAMES));
						Number_of_valid_files++;
				}
				else
				{
					goto end_enum;
				}
				goto skip_file;
				*/
		/*	
	pDH_ENUM_FILES_of_PROFILES Allocate_Pointer = (pDH_ENUM_FILES_of_PROFILES)malloc((Number_of_valid_files * sizeof(DH_ENUM_FILES_of_PROFILES))*2);
			
	
	//окончательно формируем список
	*File_names_buffer = Allocate_Pointer;
		memset((void*)&FileFDAT,0,sizeof(WIN32_FIND_DATAW));
		FindHandle = FindFirstFile(&HUNT_HIVE.PROFILE_PATH[0],&FileFDAT);
			if (FindHandle == INVALID_HANDLE_VALUE)
					goto error_DH_Get_Current_hunt_profile_ALL_FILE_CONTAINERS;
			
			i = wcslen(FileFDAT.cFileName);
		//	FileFDAT.cFileName[i-sizeof(PROFILE_VM_nfo_EXTE)] = (WCHAR)L"\0"; // - РАСШИРЕНИЕ
			memcpy(&Allocate_Pointer->FileName[0], &FileFDAT.cFileName[0],sizeof(DH_ENUM_FILES_of_PROFILES));
			
			Allocate_Pointer+=sizeof(DH_ENUM_FILES_of_PROFILES);

				if (FindNextFile(FindHandle,&FileFDAT) == 1)
				{
				memcpy(&Allocate_Pointer->FileName[0], &FileFDAT.cFileName[0],sizeof(DH_ENUM_FILES_of_PROFILES));
				Allocate_Pointer+=sizeof(DH_ENUM_FILES_of_PROFILES);
				}
				*/
end_enum:
				A_RETURN_ORIGINAL_PROFLE_PATH;
return Number_of_valid_files;

error_DH_Get_Current_hunt_profile_ALL_FILE_CONTAINERS:
	A_RETURN_ORIGINAL_PROFLE_PATH;

return 0;
}

bool DH_Test_Current_hunt_profile_CONTAINER(__in UINT PROFILE_x_TYPE,__in WCHAR* PROFILE_x_CONTAINT_FILE_NAME) //проверка указанного контейнера. (true-false)
{
	ULONG Retn_bytes=0;
	PVOID KPointer = DH_Read_Current_hunt_profile_CONTAINER(PROFILE_x_TYPE,PROFILE_x_CONTAINT_FILE_NAME,PROFILE_READ_ALL,&Retn_bytes);
	
	if(KPointer == NULL)
		{return false;}
	else
		{free(KPointer);return true;}
}

bool DH_LOCAL_is_filename_containt_profile_extension(__in UINT PROFILE_x_TYPE,__in WCHAR* PROFILE_x_CONTAINT_FILE_NAME)
{
	register size_t FileName_len = wcslen(PROFILE_x_CONTAINT_FILE_NAME);
	if(FileName_len == NULL)
		return false;

	WCHAR* Checked_Profile_ext;
	register size_t Checked_Profile_ext_SIZE;

	switch(PROFILE_x_TYPE)
	{
	case PROFILE_TYPE_VM: {Checked_Profile_ext = PROFILE_VM_nfo_EXTENTION_CRITICAL; Checked_Profile_ext_SIZE = wcslen(PROFILE_VM_nfo_EXTENTION_CRITICAL); break;}
	case PROFILE_TYPE_PATCH__NOT_APPLIED_FLAG: {Checked_Profile_ext = PROFILE_PATCH_nfo_EXTENTION_CRITICAL; Checked_Profile_ext_SIZE = wcslen(PROFILE_PATCH_nfo_EXTENTION_CRITICAL); break;}
	case PROFILE_TYPE_PATCH_ONLINE: {Checked_Profile_ext = PROFILE_PATCH_nfo_EXTENTION_CRITICAL; Checked_Profile_ext_SIZE = wcslen(PROFILE_PATCH_nfo_EXTENTION_CRITICAL); break;}
	case PROFILE_TYPE_TRACE_VMP2_LOG: {Checked_Profile_ext = PROFILE_TRACE_VMP2_LOG_nfo_EXTENTION_CRITICAL; Checked_Profile_ext_SIZE = wcslen(PROFILE_TRACE_VMP2_LOG_nfo_EXTENTION_CRITICAL); break;}
	case PROFILE_TYPE_VM_ADDRES_POOL: {Checked_Profile_ext = PROFILE_VM_addres_pool_EXTENTION_CRITICAL; Checked_Profile_ext_SIZE = wcslen(PROFILE_VM_addres_pool_EXTENTION_CRITICAL); break;}
	case PROFILE_TYPE_PATCH_FILTER_ASM_STACK_INI: {Checked_Profile_ext = PROFILE_TRACE_VMPX_EXTENTION_CRITICAL; Checked_Profile_ext_SIZE = wcslen(PROFILE_TRACE_VMPX_EXTENTION_CRITICAL); break;}
	case PROFILE_TYPE_VM_VMPX_CONTEXT_INI: {Checked_Profile_ext = PROFILE_VM_VMPX_EXTENTION_CRITICAL; Checked_Profile_ext_SIZE = wcslen(PROFILE_VM_VMPX_EXTENTION_CRITICAL); break;}
	case PROFILE_TYPE_VM_BILLET: {Checked_Profile_ext = PROFILE_VM_BILLET_EXTENTION_CRITICAL; Checked_Profile_ext_SIZE = wcslen(PROFILE_VM_BILLET_EXTENTION_CRITICAL); break;}
	case PROFILE_TYPE_PIRATE_MAP:{Checked_Profile_ext = PROFILE_PIRATE_MAP_nfo_EXTENTION_CRITICAL; Checked_Profile_ext_SIZE = wcslen(PROFILE_PIRATE_MAP_nfo_EXTENTION_CRITICAL); break;}
	default: return false;
	}

	if(Checked_Profile_ext_SIZE > FileName_len)
		return false;
	
	if(!_wcsicmp(&PROFILE_x_CONTAINT_FILE_NAME[FileName_len-Checked_Profile_ext_SIZE],Checked_Profile_ext))
		return true;

	return false;
}

bool DH_Profile_Containter_name_convert_extension_to_new(__in WCHAR* PROFILE_x_CONTAINT_FILE_NAME, __in SERVICE_TYPE New_PROFILE_x_TYPE, __in SERVICE_TYPE Current_PROFILE_x_TYPE)
{
	register size_t str_len = wcslen(PROFILE_x_CONTAINT_FILE_NAME);

	register size_t Checked_Profile_ext_SIZE;
	WCHAR* Checked_Profile_ext;

	switch(Current_PROFILE_x_TYPE)
	{
		case PROFILE_TYPE_VM: {Checked_Profile_ext_SIZE = wcslen(PROFILE_VM_nfo_EXTENTION_CRITICAL); break;}
		case PROFILE_TYPE_PATCH__NOT_APPLIED_FLAG: {Checked_Profile_ext_SIZE = wcslen(PROFILE_PATCH_nfo_EXTENTION_CRITICAL); break;}
		case PROFILE_TYPE_PATCH_ONLINE: {Checked_Profile_ext_SIZE = wcslen(PROFILE_PATCH_nfo_EXTENTION_CRITICAL); break;}
		case PROFILE_TYPE_TRACE_VMP2_LOG: {Checked_Profile_ext_SIZE = wcslen(PROFILE_TRACE_VMP2_LOG_nfo_EXTENTION_CRITICAL); break;}
		case PROFILE_TYPE_VM_ADDRES_POOL: {Checked_Profile_ext_SIZE = wcslen(PROFILE_VM_addres_pool_EXTENTION_CRITICAL); break;}
	}
	register WCHAR* pNameExt = PROFILE_x_CONTAINT_FILE_NAME+str_len-Checked_Profile_ext_SIZE;

	switch(New_PROFILE_x_TYPE)
	{
	case PROFILE_TYPE_VM: {Checked_Profile_ext = PROFILE_VM_nfo_EXTENTION_CRITICAL; Checked_Profile_ext_SIZE = wcslen(PROFILE_VM_nfo_EXTENTION_CRITICAL); break;}
	case PROFILE_TYPE_PATCH__NOT_APPLIED_FLAG: {Checked_Profile_ext = PROFILE_PATCH_nfo_EXTENTION_CRITICAL; Checked_Profile_ext_SIZE = wcslen(PROFILE_PATCH_nfo_EXTENTION_CRITICAL); break;}
	case PROFILE_TYPE_PATCH_ONLINE: {Checked_Profile_ext = PROFILE_PATCH_nfo_EXTENTION_CRITICAL; Checked_Profile_ext_SIZE = wcslen(PROFILE_PATCH_nfo_EXTENTION_CRITICAL); break;}
	case PROFILE_TYPE_TRACE_VMP2_LOG: {Checked_Profile_ext = PROFILE_TRACE_VMP2_LOG_nfo_EXTENTION_CRITICAL; Checked_Profile_ext_SIZE = wcslen(PROFILE_TRACE_VMP2_LOG_nfo_EXTENTION_CRITICAL); break;}
	case PROFILE_TYPE_VM_ADDRES_POOL: {Checked_Profile_ext = PROFILE_VM_addres_pool_EXTENTION_CRITICAL; Checked_Profile_ext_SIZE = wcslen(PROFILE_VM_addres_pool_EXTENTION_CRITICAL); break;}
	}
	
	//Checked_Profile_ext_SIZE++; //ICNLUDE ZERO
	memcpy(pNameExt, Checked_Profile_ext, Checked_Profile_ext_SIZE * sizeof(WCHAR));
	*(pNameExt+(Checked_Profile_ext_SIZE)) = '\0';
}


void DH_Profile_Verify_Folders(__in UINT PROFILE_x_TYPE)  //проверить папки, создать недостающие
{
	
	switch(PROFILE_x_TYPE)
	{
	case PROFILE_TYPE_VM: 
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_VM_nfo_FOLDER_NAME); 
			//goto FILE_FOUND;
			break;
		}
	case PROFILE_TYPE_PATCH__NOT_APPLIED_FLAG:
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_PATCH_nfo_FOLDER_NAME);

		if (::GetFileAttributesW(&HUNT_HIVE.PROFILE_PATH[0]) == (DWORD)INVALID_HANDLE_VALUE)
			::CreateDirectoryW(&HUNT_HIVE.PROFILE_PATH[0], NULL);

		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_PATCH_nfo__NOT_APPLIED_FOLDER_NAME); 
		//goto FILE_FOUND;
		break;
		}
	case PROFILE_TYPE_LIC_DATA_DUMP:
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_LIC_DATA_DUMP_nfo_FOLDER_NAME); 
		//goto FILE_FOUND;
		break;
		}

		case PROFILE_TYPE_TRACE_VMP2_LOG:
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_TRACE_VMP2_LOG_nfo_FOLDER_NAME);
			break;
		}

		case PROFILE_TYPE_PATCH_REGIONS_INFO_INI: //FILE!!!!!!!!!!!!!!!!!!!!!!!!!
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_PATCH_nfo_FOLDER_NAME);
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_PATCH_nfo_CONFIG_FILE_REGION_INI);
			DH_Profile_Create_Service_File(PROFILE_TYPE_PATCH_REGIONS_INFO_INI, &HUNT_HIVE.PROFILE_PATH[0]);
		A_RETURN_ORIGINAL_PROFLE_PATH;
		return;
			break;
		} //END CASE PROFILE_TYPE_PATCH_REGIONS_INFO_INI

		case PROFILE_TYPE_VM_BILLET:
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_VM_nfo_FOLDER_NAME);
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_VM_BILLET_FOLDER_NAME); 
			break;
		}

		case PROFILE_TYPE_PIRATE_MAP:
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_PIRATE_MAP_nfo_FOLDER_NAME);
			break;
		}

	default:
		{return;}
	}
	if (::GetFileAttributesW(&HUNT_HIVE.PROFILE_PATH[0]) == (DWORD)INVALID_HANDLE_VALUE)
			::CreateDirectoryW(&HUNT_HIVE.PROFILE_PATH[0], NULL);
	
	A_RETURN_ORIGINAL_PROFLE_PATH;
}

bool DH_Profile_Create_Service_File(__in SERVICE_TYPE PROFILE_x_TYPE, __in TCHAR* File_full_patch)  //создание типового служебного файла
{
	switch (PROFILE_x_TYPE)
	{
	case PROFILE_TYPE_PATCH_REGIONS_INFO_INI: 
		{
				if (::GetFileAttributesW(File_full_patch) == (DWORD)INVALID_HANDLE_VALUE)
							{
								 HANDLE hFile = CreateFile(File_full_patch,                // name of the write
								   GENERIC_WRITE,          // 
								   FILE_SHARE_WRITE,       // WRITE
								   NULL,                   // default security
								   CREATE_NEW,             // NEW
								   FILE_ATTRIBUTE_NORMAL,  // normal file
								   NULL);

										if (hFile == INVALID_HANDLE_VALUE) 
											{return false;}

										::CloseHandle(hFile);

									WritePrivateProfileString (PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section, 
								 PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_name_key, 
								 TS_DEF_NOTEXT, 
                                 File_full_patch); //vmp section info - name

									WritePrivateProfileString (PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section, 
								 PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_address_key, 
								 TS_DEF_NOTEXT, 
                                 File_full_patch); //vmp section info - address

									WritePrivateProfileString (PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section, 
								 PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_size_key, 
								 TS_DEF_NOTEXT, 
                                 File_full_patch); //vmp section info - size

								WritePrivateProfileString (PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_original_copy, 
								 PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_original_copy_start_address_key, 
								 TS_DEF_NOTEXT, 
                                 File_full_patch); //vmp section original copy - start address

								WritePrivateProfileString (PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_original_copy, 
								 PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_original_copy_end_address_key, 
								 TS_DEF_NOTEXT, 
                                 File_full_patch); //vmp section original copy - end address

								WritePrivateProfileString (PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_original_copy, 
								 PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_original_copy_is_req_memcpy_original_section_flag_key, 
								 TS_DEF_NULL, 
                                 File_full_patch); //vmp section original copy - req memcpy

									WritePrivateProfileString (PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_patch_map, 
								 PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_patch_map_start_address_key, 
								 TS_DEF_NOTEXT, 
                                 File_full_patch); //vmp section patch map - start address

									WritePrivateProfileString (PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_patch_map, 
								 PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_patch_map_end_address_key, 
								 TS_DEF_NOTEXT, 
                                 File_full_patch); //vmp section patch map - end address

								WritePrivateProfileString (PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_patch_map, 
								 PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_patch_map_lockmarker_key, 
								 PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_VMP_section_patch_map_lockmarker_value, 
                                 File_full_patch); //vmp section patch map -lock marker

											WritePrivateProfileString (PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_DH_hooks_and_filters, 
								 PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_DH_hooks_and_filters_start_address_key, 
								 TS_DEF_NOTEXT, 
                                 File_full_patch); //vmp hooks & filters - start addres

											WritePrivateProfileString (PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_DH_hooks_and_filters, 
								 PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_DH_hooks_and_filters_end_address_key, 
								 TS_DEF_NOTEXT, 
                                 File_full_patch); //vmp hooks & filters - end addres

											WritePrivateProfileString (PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_DH_hooks_and_filters, 
								 PROFILE_SERVICE_CONFIG_FILE_REGION_INI_SECTION_DH_hooks_and_filters_float_pointer_key, 
								 TS_DEF_NOTEXT, 
                                 File_full_patch); //vmp hooks & filters - float pointer
							}
			} //end case PROFILE_TYPE_PATCH_REGIONS_INFO_INI
	}//end switch
}

bool DH_Write_Current_hunt_profile_CONTAINER_savedlg(__in UINT PROFILE_x_TYPE, //тип контейнера
	__in BYTE* write_buff, //буфер записи
	__in size_t num_bytes_to_write //кол-во байт записи  sizeof(струкрура контейнера)
		)
{

	switch(PROFILE_x_TYPE)
	{
	case PROFILE_TYPE_VM: 
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_VM_nfo_FOLDER_NAME);
			break;
		}
	case PROFILE_TYPE_PATCH_ONLINE:
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_PATCH_nfo_FOLDER_NAME);
			break;
		}

	case PROFILE_TYPE_PATCH__NOT_APPLIED_FLAG:
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_PATCH_nfo_FOLDER_NAME);
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_PATCH_nfo__NOT_APPLIED_FOLDER_NAME); 
			break;
		}
	case PROFILE_TYPE_LIC_DATA_DUMP:
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_LIC_DATA_DUMP_nfo_FOLDER_NAME); 
		break;
		}

	case PROFILE_TYPE_PATCH_REGIONS_INFO_INI:
		{
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_PATCH_nfo_FOLDER_NAME);
		wcscat(&HUNT_HIVE.PROFILE_PATH[0], PROFILE_PATCH_nfo_CONFIG_FILE_REGION_INI);
			break;
		}

	}
		CFileDialog fileDialog(false,NULL,_T("dump.licdmp"),NULL,_T("Licence raw dumps (*.licdmp)|*.licdmp|"));
		fileDialog.m_ofn.lpstrInitialDir = &HUNT_HIVE.PROFILE_PATH[0];
		int result = fileDialog.DoModal();

		if (result != NULL)
		{
				CString path= fileDialog.GetPathName();
				if (::GetFileAttributesW(path) != (DWORD)INVALID_HANDLE_VALUE)
					::DeleteFileW(path);

			  HANDLE hFile = CreateFile(path,                // name of the write
                       GENERIC_WRITE,          // open for writing
                       0,                      // do not share
                       NULL,                   // default security
                       CREATE_NEW,             // create new file only
                       FILE_ATTRIBUTE_NORMAL,  // normal file
                       NULL);
				 if (hFile == INVALID_HANDLE_VALUE) 
				 {
					 A_RETURN_ORIGINAL_PROFLE_PATH;
					 path.ReleaseBuffer(0);
					 return false;
				 }

							 DWORD dwBytesWritten =0;

			 bool bErrorFlag = WriteFile( 
                    hFile,           // open file handle
                    write_buff,      // start of data to write
                    num_bytes_to_write,  // number of bytes to write
                    &dwBytesWritten, // number of bytes that were written
                    NULL);            // no overlapped structure
			CloseHandle(hFile);

				path.ReleaseBuffer(0);
		}

		A_RETURN_ORIGINAL_PROFLE_PATH;
	return result;
}


__forceinline PVOID DH_Profile_PARSE_Arbitary_FILTER_CONFIG(__in TCHAR* pRaw_VMPX_Profiler, __in size_t Raw_size)
{
		//register pPATCH_SUPPORTING_VMPX_PROFILE_INI_STACK pResult_stack = (pPATCH_SUPPORTING_VMPX_PROFILE_INI_STACK)malloc(sizeof(PATCH_SUPPORTING_VMPX_PROFILE_INI_STACK)); //allocate stack
			register pPATCH_SUPPORTING_VMPX_PROFILE_INI_STACK pResult_stack = (pPATCH_SUPPORTING_VMPX_PROFILE_INI_STACK)::VirtualAlloc(NULL,sizeof(PATCH_SUPPORTING_VMPX_PROFILE_INI_STACK),MEM_COMMIT,PAGE_READWRITE);
		if(pResult_stack == NULL)
			return NULL;

		Raw_size/=sizeof(TCHAR); //correct cycle iterations
		TCHAR* fpRaw_VMPX_Profiler = pRaw_VMPX_Profiler;
		register pPATCH_SUPPORTING_VMPX_PROFILE_INI_STACK fpResult_stack = pResult_stack;

		Error_info.error_TYPE = ERROR_SUCCESS;
		Error_info.Line_error = 1;
		Error_info.Col_error = 1;

		TCHAR* pOPEN_BRANCH = NULL;
		TCHAR* pASM = pRaw_VMPX_Profiler;
		pPATCH_SUPPORTING_VMPX_PROFILE_INI VMPX_Profile = &pResult_stack->VMPX_HANHELD_TYPE_ENUMS[0];
		TCHAR* pASM_Stack = pResult_stack->VMPX_HANHELD_TYPE_ENUMS[0].Assembly_instruction1;
		VMPX_Profile->Assembly_count = NULL;
		pResult_stack->Loaded_HANHELDS_FILTERS = NULL;
		size_t* fpFirstSymbol = &VMPX_Profile->First_control_symbol_offset[0];
		bool* fpControlBymbolPresent = &VMPX_Profile->Control_symbols_is_present[0];
	
		VMPX_Profile--;
		
		bool is_comment = false;
		bool is_nkey = false;

			do
			{
	
					switch(*fpRaw_VMPX_Profiler++)
					{
					case FILTER_DEFINE_CONTROL_SYMBOL: 
						{
							if(is_comment == true)
								break;

							*fpControlBymbolPresent = true;
							if (*fpFirstSymbol == NULL)
								*fpFirstSymbol = (SERVICE_ADDRES)fpRaw_VMPX_Profiler - (SERVICE_ADDRES)pASM;
							//CONTROL SYMBOL TYPE
							
									switch(*fpRaw_VMPX_Profiler)
									{
									case FILTER_SUBDEFINE_CONTROL_SYMBOL_RETURN: {VMPX_Profile->is_request_return_to_original_code = true; break;}
									}//CS swtich end
									
						break;}
					case FILTER_DEFINE_BRANCH_OPEN_SYMBOL: 
						{
							if(((SERVICE_ADDRES)fpRaw_VMPX_Profiler - (SERVICE_ADDRES)pASM)  == sizeof(TCHAR)) //is string start?
								{pOPEN_BRANCH = fpRaw_VMPX_Profiler; is_nkey = false;} //SAVE first position
							else
								{pOPEN_BRANCH = NULL; is_nkey = true;}

						break;}

					case FILTER_DEFINE_BRANCH_CLOSE_SYMBOL: 
						{
							if(is_nkey == true) //filter for ex MOV R15B, [R15]
								break;

							if(pOPEN_BRANCH == NULL) //error! - close symbol present, BUT no open symbol
								{Error_info.error_TYPE = PARSE_ERROR_NO_OPEN; goto error_ret;}
							*(fpRaw_VMPX_Profiler-1) = L'\0';

								SERVICE_TYPE Key_Type_i = DH_VM_HANHELD_string_To_type(pOPEN_BRANCH);

									if(Key_Type_i == NULL) //error! - close symbol present, BUT no open symbol
										{Error_info.error_TYPE = PARSE_ERROR_UNSUPPORTED_KEY; goto error_ret;}
					
								pResult_stack->KeyType[Key_Type_i] = Key_Type_i; //assinged key type (as HANHELD TYPE)
							
								VMPX_Profile = &pResult_stack->VMPX_HANHELD_TYPE_ENUMS[Key_Type_i]; //HANHELD TYPE = QIUCK ACCESS TABLE
								pResult_stack->Loaded_HANHELDS_FILTERS++;
								pASM_Stack = &VMPX_Profile->Assembly_instruction1[0];
								VMPX_Profile->Assembly_count = NULL;
								fpFirstSymbol = &VMPX_Profile->First_control_symbol_offset[0];
								fpControlBymbolPresent = &VMPX_Profile->Control_symbols_is_present[0];
						break;}

					case FILTER_DEFINE_COMMENT_SYMBOL: {is_comment = true; break;}
					case FILTER_DEFINE_END_STRING_SYMBOL: 
						{
							if(pOPEN_BRANCH == NULL && pASM != NULL && is_comment == false) //is string != key?
								{memcpy(pASM_Stack, pASM, (SERVICE_ADDRES)fpRaw_VMPX_Profiler - (SERVICE_ADDRES)pASM); *(TCHAR*)((SERVICE_ADDRES)pASM_Stack+((SERVICE_ADDRES)fpRaw_VMPX_Profiler - (SERVICE_ADDRES)pASM)-sizeof(TCHAR)-sizeof(TCHAR)) = L'\0'; pASM_Stack+=sizeof(pResult_stack->VMPX_HANHELD_TYPE_ENUMS[0].Assembly_instruction1)/sizeof(TCHAR); VMPX_Profile->Assembly_count++;}

							Error_info.Line_error++; //next line
							Error_info.Col_error = 1; //set entry column
							codedef_no_key; //NO KEY!
							pASM = fpRaw_VMPX_Profiler; //NEW STRING predefinition as ASM
							is_comment = false;

							fpFirstSymbol++;
							fpControlBymbolPresent++;
						break;}
					case FILTER_DEFINE_LABEL_SYMBOL:
						{
							if(is_comment == true)
								break;

							TCHAR Label_source[4];
							memset(&Label_source[0], NULL, sizeof(Label_source));
							memcpy(&Label_source[0],fpRaw_VMPX_Profiler, sizeof(TCHAR));
							
								SERVICE_ARRAY_ID saLabelID = wcstoul(&Label_source[0],NULL,16);
										if(saLabelID > 10)
											//{goto error_ret;}
										{
											::VirtualFree(pResult_stack,sizeof(PATCH_SUPPORTING_VMPX_PROFILE_INI_STACK),MEM_RELEASE);
											free(pRaw_VMPX_Profiler);
											return NULL;
										}//if bad convert -> array size is bad

							VMPX_Profile->Label[saLabelID] = VMPX_Profile->Assembly_count; //save current asm stack pointer
							
							//update pointer - skip label
								if (*(fpRaw_VMPX_Profiler+1) == ' ')
									pASM = (fpRaw_VMPX_Profiler+2); 
								else
									pASM = (fpRaw_VMPX_Profiler+1);
						break;}
					}//end switch

				Error_info.Col_error++;
			}while(--Raw_size); //main cycle +++++++++++++++++++

		free(pRaw_VMPX_Profiler);
	return pResult_stack;

error_ret:
::VirtualFree(pResult_stack,sizeof(PATCH_SUPPORTING_VMPX_PROFILE_INI_STACK),MEM_RELEASE);
free(pRaw_VMPX_Profiler);
return NULL;
}



__forceinline PVOID DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG(__in TCHAR* pRaw_VMPX_Profiler, __in size_t Raw_size, __in TCHAR* pINIFilename)
{
		register pVM_VMPX_PROFILE_INI_STACK pResult_stack = (pVM_VMPX_PROFILE_INI_STACK)::VirtualAlloc(NULL,sizeof(VM_VMPX_PROFILE_INI_STACK),MEM_COMMIT,PAGE_READWRITE);

		if(pResult_stack == NULL)
			return NULL;

		Raw_size/=sizeof(TCHAR); //correct cycle iterations
		TCHAR* fpRaw_VMPX_Profiler = pRaw_VMPX_Profiler;
		register pVM_VMPX_PROFILE_INI_STACK fpResult_stack = pResult_stack;

		Error_info.error_TYPE = ERROR_SUCCESS;
		Error_info.Line_error = 1;
		Error_info.Col_error = 1;

		TCHAR* pOPEN_BRANCH = NULL;
		TCHAR* pASM = pRaw_VMPX_Profiler;
		pVM_VMPX_PROFILE_INI VMPX_Profile = &pResult_stack->VMPX_HANHELD_TYPE_ENUMS[0];
		//TCHAR* pASM_Stack = pResult_stack->VMPX_HANHELD_TYPE_ENUMS[0].Assembly_instruction1;
		VMPX_Profile->Assembly_count = NULL;
		pResult_stack->Loaded_HANHELDS_FILTERS = NULL;
	
		VMPX_Profile--;
		
		bool is_comment = false;
		bool is_nkey = false;

			do
			{
	
					switch(*fpRaw_VMPX_Profiler++)
					{
					case FILTER_DEFINE_BRANCH_OPEN_SYMBOL: 
						{
							if (is_comment == true) //filter #1
								break;

							if(((SERVICE_ADDRES)fpRaw_VMPX_Profiler - (SERVICE_ADDRES)pASM)  == sizeof(TCHAR)) //is string start?
								{pOPEN_BRANCH = fpRaw_VMPX_Profiler; is_nkey = false;} //SAVE first position
							else
								{pOPEN_BRANCH = NULL; is_nkey = true;}

						break;}

					case FILTER_DEFINE_BRANCH_CLOSE_SYMBOL: 
						{
							if(is_nkey == true) //filter for ex MOV R15B, [R15]
								break;
							if (is_comment == true) //filter #1
								break;

							if(pOPEN_BRANCH == NULL) //error! - close symbol present, BUT no open symbol
								{Error_info.error_TYPE = PARSE_ERROR_NO_OPEN; ::VirtualFree(pResult_stack,sizeof(PATCH_SUPPORTING_VMPX_PROFILE_INI_STACK),MEM_RELEASE); free(pRaw_VMPX_Profiler); return NULL;}
							*(fpRaw_VMPX_Profiler-1) = L'\0';

								SERVICE_TYPE Key_Type_i = DH_VM_HANHELD_string_To_type(pOPEN_BRANCH);

									if((signed long)Key_Type_i <= NULL) //error! - close symbol present, BUT no open symbol
										{Error_info.error_TYPE = PARSE_ERROR_UNSUPPORTED_KEY; ::VirtualFree(pResult_stack,sizeof(PATCH_SUPPORTING_VMPX_PROFILE_INI_STACK),MEM_RELEASE); free(pRaw_VMPX_Profiler); return NULL;}
					
								pResult_stack->KeyType[Key_Type_i] = Key_Type_i; //assinged key type (as HANHELD TYPE !!!!!!!!!!!!!!!!!!!!!!)
							
								VMPX_Profile = &pResult_stack->VMPX_HANHELD_TYPE_ENUMS[Key_Type_i]; //HANHELD TYPE = QIUCK ACCESS TABLE
								pResult_stack->Loaded_HANHELDS_FILTERS++;
								VMPX_Profile->Assembly_count = NULL;

								DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__GetSectionKeys(pOPEN_BRANCH, VMPX_Profile, pINIFilename); // get keys value
						break;}

					case FILTER_DEFINE_COMMENT_SYMBOL: {is_comment = true; break;}
					case FILTER_DEFINE_END_STRING_SYMBOL: 
						{
							//if(pOPEN_BRANCH == NULL && pASM != NULL && is_comment == false) //is string != key?
								//{memcpy(pASM_Stack, pASM, (SERVICE_ADDRES)fpRaw_VMPX_Profiler - (SERVICE_ADDRES)pASM); *(TCHAR*)((SERVICE_ADDRES)pASM_Stack+((SERVICE_ADDRES)fpRaw_VMPX_Profiler - (SERVICE_ADDRES)pASM)-sizeof(TCHAR)-sizeof(TCHAR)) = L'\0'; pASM_Stack+=sizeof(pResult_stack->VMPX_HANHELD_TYPE_ENUMS[0].Assembly_instruction1)/sizeof(TCHAR); VMPX_Profile->Assembly_count++;}

							Error_info.Line_error++; //next line
							Error_info.Col_error = 1; //set entry column
							codedef_no_key; //NO KEY!
							pASM = fpRaw_VMPX_Profiler; //NEW STRING predefinition as ASM
							is_comment = false;
						break;}
					}//end switch

				Error_info.Col_error++;
			}while(--Raw_size); //main cycle +++++++++++++++++++

			//set inital values from TaskWork_stack
			
			Raw_size = HANHELD_TYPE_ENUMS_COUNT;
			SERVICE_TYPE** fpSetTWS_null_fp = &pResult_stack->TaskWorkSearcher_stack__NULL_fp[0];
			SERVICE_COUNT* fpSetTWS_initial_count = (SERVICE_COUNT*)&pResult_stack->TaskWorkSearcher_count_initial_count[0];
			VMPX_Profile = &pResult_stack->VMPX_HANHELD_TYPE_ENUMS[0];
			do
			{
				*fpSetTWS_null_fp = &VMPX_Profile->tAsmSearchTask_stack[0];
				*fpSetTWS_initial_count = VMPX_Profile->cAsmSearchTask_stack_count;

				fpSetTWS_null_fp++; 
				fpSetTWS_initial_count++;
				VMPX_Profile++;
			}while(--Raw_size);

		free(pRaw_VMPX_Profiler);
	return pResult_stack;


}


__forceinline SERVICE_COUNT DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__GetSectionKeys(__in TCHAR* pBranchName, __in LPVOID pResultUnitTypeStack, __in TCHAR* pINIFilename)
{
	pVM_VMPX_PROFILE_INI pCastResultUnitTypeStack = reinterpret_cast<pVM_VMPX_PROFILE_INI>(pResultUnitTypeStack);
	register SERVICE_COUNT Result_count = NULL; 
	register SERVICE_COUNT api_ret = NULL;

	register SERVICE_COUNT Predict_wait_count = NULL;
	register SERVICE_COUNT After_count = NULL;

	pCastResultUnitTypeStack->cAsmSearchTask_stack_count = NULL;
	register SERVICE_TYPE* fpAsmSearchTask_stack_ESP = &pCastResultUnitTypeStack->tAsmSearchTask_stack[0]; //первая задача по поиску 

	//PREDICT STATIC (1 CYCLE)
	api_ret = ::GetPrivateProfileString(pBranchName,
				PROFILE_SERVICE_CONFIG_FILE_VMPX_CONTEXT_INI_DH_Predict_STATIC_key,
				NULL,
				&pCastResultUnitTypeStack->Predict_STATIC_assembly[0],
				MAX_PATH,
				pINIFilename);

	if(api_ret != NULL)
	{
	Result_count++;
	//DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Normalize_and_assembly(&pCastResultUnitTypeStack->Predict_STATIC_assembly[0], api_ret,&pCastResultUnitTypeStack->xed_Predict_STATIC, &pCastResultUnitTypeStack->Predict_wait_STATIC_constrol_symbol, &pCastResultUnitTypeStack->First_control_symbol_offset__Predict_wait_STATIC_constrol_symbol, &pCastResultUnitTypeStack->is_static_Predict_STATIC_assembly);
	pCastResultUnitTypeStack->Predict_STATIC_assembly_len = api_ret;
	DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Normalize_and_assembly(VMPX_CONTEXT_ASM_STATIC, api_ret , pCastResultUnitTypeStack);
		//*fpAsmSearchTask_stack_ESP++ = VMPX_CONTEXT_ASM_STATIC; //по дефолту ВСЕГДА ищется самой первой
	}
	//PREDICT WAIT (4 CYCLES)
	api_ret = ::GetPrivateProfileString(pBranchName,
				PROFILE_SERVICE_CONFIG_FILE_VMPX_CONTEXT_INI_DH_Predict_key_1,
				NULL,
				&pCastResultUnitTypeStack->Predict_wait_assembly1[0],
				MAX_PATH,
				pINIFilename);

	if(api_ret != NULL)
	{
	Result_count++;
	Predict_wait_count++;
	pCastResultUnitTypeStack->Predict_wait_assembly1_len = api_ret;
	if (DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Normalize_and_assembly(VMPX_CONTEXT_ASM_PREDICT_WAIT_1, api_ret , pCastResultUnitTypeStack) == true)
		{*fpAsmSearchTask_stack_ESP++ = VMPX_CONTEXT_ASM_PREDICT_WAIT_1; pCastResultUnitTypeStack->cAsmSearchTask_stack_count++;} //добавить в планировщик
	}

	api_ret = ::GetPrivateProfileString(pBranchName,
				PROFILE_SERVICE_CONFIG_FILE_VMPX_CONTEXT_INI_DH_Predict_key_2,
				NULL,
				&pCastResultUnitTypeStack->Predict_wait_assembly2[0],
				MAX_PATH,
				pINIFilename);

	if(api_ret != NULL)
	{
	Result_count++;
	Predict_wait_count++;
	pCastResultUnitTypeStack->Predict_wait_assembly2_len = api_ret;
	if (DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Normalize_and_assembly(VMPX_CONTEXT_ASM_PREDICT_WAIT_2, api_ret , pCastResultUnitTypeStack) == true)
		{*fpAsmSearchTask_stack_ESP++ = VMPX_CONTEXT_ASM_PREDICT_WAIT_2; pCastResultUnitTypeStack->cAsmSearchTask_stack_count++;} //добавить в планировщик
	}

	api_ret = ::GetPrivateProfileString(pBranchName,
					PROFILE_SERVICE_CONFIG_FILE_VMPX_CONTEXT_INI_DH_Predict_key_3,
					NULL,
					&pCastResultUnitTypeStack->Predict_wait_assembly3[0],
					MAX_PATH,
					pINIFilename);

	if(api_ret != NULL)
	{
	Result_count++;
	Predict_wait_count++;
	pCastResultUnitTypeStack->Predict_wait_assembly3_len = api_ret;
	if (DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Normalize_and_assembly(VMPX_CONTEXT_ASM_PREDICT_WAIT_3, api_ret , pCastResultUnitTypeStack) == true)
		{*fpAsmSearchTask_stack_ESP++ = VMPX_CONTEXT_ASM_PREDICT_WAIT_3; pCastResultUnitTypeStack->cAsmSearchTask_stack_count++;} //добавить в планировщик
	}

	api_ret = ::GetPrivateProfileString(pBranchName,
						PROFILE_SERVICE_CONFIG_FILE_VMPX_CONTEXT_INI_DH_Predict_key_4,
						NULL,
						&pCastResultUnitTypeStack->Predict_wait_assembly4[0],
						MAX_PATH,
						pINIFilename);

	if(api_ret != NULL)
	{
	Result_count++;
	Predict_wait_count++;
	pCastResultUnitTypeStack->Predict_wait_assembly4_len = api_ret;
	if (DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Normalize_and_assembly(VMPX_CONTEXT_ASM_PREDICT_WAIT_4, api_ret , pCastResultUnitTypeStack) == true)
			{*fpAsmSearchTask_stack_ESP++ = VMPX_CONTEXT_ASM_PREDICT_WAIT_4; pCastResultUnitTypeStack->cAsmSearchTask_stack_count++;} //добавить в планировщик
	}

	//CRITICAL
	api_ret = ::GetPrivateProfileString(pBranchName,
						PROFILE_SERVICE_CONFIG_FILE_VMPX_CONTEXT_INI_DH_Critical_key,
						NULL,
						&pCastResultUnitTypeStack->Critial_assembly[0],
						MAX_PATH,
						pINIFilename);

	if(api_ret != NULL)
	{
	Result_count++;
	pCastResultUnitTypeStack->Critial_assembly_len = api_ret;
	if (DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Normalize_and_assembly(VMPX_CONTEXT_ASM_CRITICAL, api_ret , pCastResultUnitTypeStack) == true)
			{*fpAsmSearchTask_stack_ESP++ = VMPX_CONTEXT_ASM_CRITICAL; pCastResultUnitTypeStack->cAsmSearchTask_stack_count++;} //добавить в планировщик
	}

	//AFTER CRITICAL (2 CYCLES)
	api_ret = ::GetPrivateProfileString(pBranchName,
						PROFILE_SERVICE_CONFIG_FILE_VMPX_CONTEXT_INI_DH_After_key_1,
						NULL,
						&pCastResultUnitTypeStack->After_critical_assebly1[0],
						MAX_PATH,
						pINIFilename);

	if(api_ret != NULL)
	{
	Result_count++; 
	After_count++;
	pCastResultUnitTypeStack->After_critical_assebly1_len = api_ret;
	if (DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Normalize_and_assembly(VMPX_CONTEXT_ASM_AFTER_1, api_ret , pCastResultUnitTypeStack) == true)
			{*fpAsmSearchTask_stack_ESP++ = VMPX_CONTEXT_ASM_AFTER_1; pCastResultUnitTypeStack->cAsmSearchTask_stack_count++;} //добавить в планировщик
	}


	api_ret = ::GetPrivateProfileString(pBranchName,
						PROFILE_SERVICE_CONFIG_FILE_VMPX_CONTEXT_INI_DH_After_key_2,
						NULL,
						&pCastResultUnitTypeStack->After_critical_assebly2[0],
						MAX_PATH,
						pINIFilename);

	if(api_ret != NULL)
	{
	Result_count++; 
	After_count++;
	pCastResultUnitTypeStack->After_critical_assebly2_len = api_ret;
	if (DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Normalize_and_assembly(VMPX_CONTEXT_ASM_AFTER_2, api_ret , pCastResultUnitTypeStack) == true)
		{*fpAsmSearchTask_stack_ESP++ = VMPX_CONTEXT_ASM_AFTER_2; pCastResultUnitTypeStack->cAsmSearchTask_stack_count++;} //добавить в планировщик
	}

	api_ret = ::GetPrivateProfileString(pBranchName,
						PROFILE_SERVICE_CONFIG_FILE_VMPX_CONTEXT_INI_DH_EXTRARCT_key,
						NULL,
						&pCastResultUnitTypeStack->Out_EXTRACT[0],
						MAX_PATH,
						pINIFilename);

	if(api_ret != NULL)
	{
	Result_count++; 
	After_count++;
	pCastResultUnitTypeStack->Out_EXTRACT_len = api_ret;
	DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Normalize_and_assembly(VMPX_CONTEXT_ASM_EXTRACT, api_ret ,pCastResultUnitTypeStack);
	}


	api_ret = ::GetPrivateProfileString(pBranchName,
						PROFILE_SERVICE_CONFIG_FILE_VMPX_CONTEXT_INI_DH_EXTRARCT_VALUE_LINK_key,
						NULL,
						&pCastResultUnitTypeStack->Out_EXTRACT_VALUE_LINK[0],
						MAX_PATH,
						pINIFilename);

	if(api_ret != NULL)
	{
	Result_count++; 
	After_count++;
	pCastResultUnitTypeStack->Out_EXTRACT_VALUE_LINK_len = api_ret;
	DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Normalize_and_assembly(VMPX_CONTEXT_ASM_EXTRACT_VALUE_LINK, api_ret ,pCastResultUnitTypeStack);
	}

	*fpAsmSearchTask_stack_ESP = VMPX_CONTEXT___END; //SET END

pCastResultUnitTypeStack->Predict_wait_count = Predict_wait_count;
pCastResultUnitTypeStack->After_count = After_count;
pCastResultUnitTypeStack->Assembly_count = Result_count;
return Result_count;
}

//__forceinline bool DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Normalize_and_assembly(__in TCHAR* pAsmInstruction, __in size_t AsmInstruction_size, __in XEDPARSE* pXED_parse_asm, __out SERVICE_TYPE* pSetCS_type, __out size_t* pSetCS_first_symbol, __out bool*	pSetStaticFlag)
__forceinline bool DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Normalize_and_assembly(__in SERVICE_TYPE Ident_normalize, __in size_t AsmInstruction_size, __out LPVOID pResultUnitTypeStack)
{
	pVM_VMPX_PROFILE_INI pCastResultUnitTypeStack = reinterpret_cast<pVM_VMPX_PROFILE_INI>(pResultUnitTypeStack);
	TCHAR* pAsmInstruction; //указатель на строку инструкции
	XEDPARSE* pXED_parse_asm; //указатель на структуру xedparse
	bool* pIsStatic; //указатель на флаг static/dynamic
	SERVICE_TYPE* pCSPresent; //указатель на контрольный символ
	size_t* pCSPresent_site; //указатель на место присутсвия контрольного символа
	SERVICE_TYPE*  pQuick_combine_type; //указатель на тип быстрого определения асм
	SERVICE_TYPE*  pSet_size_typedef_for_CS_REGISTER; //указатель на тип размерности регистра (для контрольного символа PROFILE_VMPX_CONTROL_SYMBOL_CPU_REGISTER)

	//TYPEDEF SWITCHER
	switch(Ident_normalize) //какую асм нормализируем
	{
		case VMPX_CONTEXT_ASM_STATIC:
		{
			pAsmInstruction = &pCastResultUnitTypeStack->Predict_STATIC_assembly[0];
			pXED_parse_asm = &pCastResultUnitTypeStack->xed_Predict_STATIC;
			pIsStatic = &pCastResultUnitTypeStack->is_static_Predict_STATIC_assembly;
			pCSPresent = &pCastResultUnitTypeStack->Predict_wait_STATIC_constrol_symbol;
			pCSPresent_site = &pCastResultUnitTypeStack->First_control_symbol_offset__Predict_wait_STATIC_constrol_symbol;
			pQuick_combine_type = &pCastResultUnitTypeStack->Quick_combine_type__Predict_STATIC;
			pSet_size_typedef_for_CS_REGISTER = &pCastResultUnitTypeStack->CS_REGISTER__size_type__Predict_STATIC;
			break;
		}//end VMPX_CONTEXT_ASM_STATIC

		//predict_wait (cycle)
	case VMPX_CONTEXT_ASM_PREDICT_WAIT_1:
		{
			pAsmInstruction = &pCastResultUnitTypeStack->Predict_wait_assembly1[0];
			pXED_parse_asm = &pCastResultUnitTypeStack->xed_Predict_wait_assembly1;
			pIsStatic = &pCastResultUnitTypeStack->is_static_Predict_wait_assembly1;
			pCSPresent = &pCastResultUnitTypeStack->Predict_wait_1_control_symbol;
			pCSPresent_site = &pCastResultUnitTypeStack->First_control_symbol_offset__Predict_wait_1_control_symbol;
			pQuick_combine_type = &pCastResultUnitTypeStack->Quick_combine_type__Predict_wait_1;
			pSet_size_typedef_for_CS_REGISTER = &pCastResultUnitTypeStack->CS_REGISTER__size_type__Predict_wait_1;
			break;
		}//end VMPX_CONTEXT_ASM_PREDICT_WAIT_1

		case VMPX_CONTEXT_ASM_PREDICT_WAIT_2:
		{
			pAsmInstruction = &pCastResultUnitTypeStack->Predict_wait_assembly2[0];
			pXED_parse_asm = &pCastResultUnitTypeStack->xed_Predict_wait_assembly2;
			pIsStatic = &pCastResultUnitTypeStack->is_static_Predict_wait_assembly2;
			pCSPresent = &pCastResultUnitTypeStack->Predict_wait_2_control_symbol;
			pCSPresent_site = &pCastResultUnitTypeStack->First_control_symbol_offset__Predict_wait_2_control_symbol;
			pQuick_combine_type = &pCastResultUnitTypeStack->Quick_combine_type__Predict_wait_2;
			pSet_size_typedef_for_CS_REGISTER = &pCastResultUnitTypeStack->CS_REGISTER__size_type__Predict_wait_2;
			break;
		}//end VMPX_CONTEXT_ASM_PREDICT_WAIT_2

		case VMPX_CONTEXT_ASM_PREDICT_WAIT_3:
		{
			pAsmInstruction = &pCastResultUnitTypeStack->Predict_wait_assembly3[0];
			pXED_parse_asm = &pCastResultUnitTypeStack->xed_Predict_wait_assembly3;
			pIsStatic = &pCastResultUnitTypeStack->is_static_Predict_wait_assembly3;
			pCSPresent = &pCastResultUnitTypeStack->Predict_wait_3_control_symbol;
			pCSPresent_site = &pCastResultUnitTypeStack->First_control_symbol_offset__Predict_wait_3_control_symbol;
			pQuick_combine_type = &pCastResultUnitTypeStack->Quick_combine_type__Predict_wait_3;
			pSet_size_typedef_for_CS_REGISTER = &pCastResultUnitTypeStack->CS_REGISTER__size_type__Predict_wait_3;
			break;
		}//end VMPX_CONTEXT_ASM_PREDICT_WAIT_3

		case VMPX_CONTEXT_ASM_PREDICT_WAIT_4:
		{
			pAsmInstruction = &pCastResultUnitTypeStack->Predict_wait_assembly4[0];
			pXED_parse_asm = &pCastResultUnitTypeStack->xed_Predict_wait_assembly4;
			pIsStatic = &pCastResultUnitTypeStack->is_static_Predict_wait_assembly4;
			pCSPresent = &pCastResultUnitTypeStack->Predict_wait_4_control_symbol;
			pCSPresent_site = &pCastResultUnitTypeStack->First_control_symbol_offset__Predict_wait_4_control_symbol;
			pQuick_combine_type = &pCastResultUnitTypeStack->Quick_combine_type__Predict_wait_4;
			pSet_size_typedef_for_CS_REGISTER = &pCastResultUnitTypeStack->CS_REGISTER__size_type__Predict_wait_4;
			break;
		}//end VMPX_CONTEXT_ASM_PREDICT_WAIT_4

		case VMPX_CONTEXT_ASM_CRITICAL:
		{
			pAsmInstruction = &pCastResultUnitTypeStack->Critial_assembly[0];
			pXED_parse_asm = &pCastResultUnitTypeStack->xed_Critial_assembly;
			pIsStatic = &pCastResultUnitTypeStack->is_static_Critial_assembly;
			pCSPresent = &pCastResultUnitTypeStack->Critical_control_symbol;
			pCSPresent_site = &pCastResultUnitTypeStack->First_control_symbol_offset__Critical_control_symbol;
			pQuick_combine_type = &pCastResultUnitTypeStack->Quick_combine_type__Critical_control;
			pSet_size_typedef_for_CS_REGISTER = &pCastResultUnitTypeStack->CS_REGISTER__size_type__Critical_control;
			break;
		}//end VMPX_CONTEXT_ASM_CRITICAL

		case VMPX_CONTEXT_ASM_AFTER_1:
		{
			pAsmInstruction = &pCastResultUnitTypeStack->After_critical_assebly1[0];
			pXED_parse_asm = &pCastResultUnitTypeStack->xed_After_critical_assebly1;
			pIsStatic = &pCastResultUnitTypeStack->is_static_After_critical_assebly1;
			pCSPresent = &pCastResultUnitTypeStack->After_1_control_symbol;
			pCSPresent_site = &pCastResultUnitTypeStack->First_control_symbol_offset__After_1_control_symbol;
			pQuick_combine_type = &pCastResultUnitTypeStack->Quick_combine_type__After_1;
			pSet_size_typedef_for_CS_REGISTER = &pCastResultUnitTypeStack->CS_REGISTER__size_type__After_1;
			break;
		}//end VMPX_CONTEXT_ASM_AFTER_1

		case VMPX_CONTEXT_ASM_AFTER_2:
		{
			pAsmInstruction = &pCastResultUnitTypeStack->After_critical_assebly2[0];
			pXED_parse_asm = &pCastResultUnitTypeStack->xed_After_critical_assebly2;
			pIsStatic = &pCastResultUnitTypeStack->is_static_After_critical_assebly2;
			pCSPresent = &pCastResultUnitTypeStack->After_2_control_symbol;
			pCSPresent_site = &pCastResultUnitTypeStack->First_control_symbol_offset__After_2_control_symbol;
			pQuick_combine_type = &pCastResultUnitTypeStack->Quick_combine_type__After_2;
			pSet_size_typedef_for_CS_REGISTER = &pCastResultUnitTypeStack->CS_REGISTER__size_type__After_2;
			break;
		}//end VMPX_CONTEXT_ASM_AFTER_2

		case VMPX_CONTEXT_ASM_EXTRACT:
			{
				pAsmInstruction = &pCastResultUnitTypeStack->Out_EXTRACT[0];
				pXED_parse_asm = &pCastResultUnitTypeStack->xed_Out_EXTRACT;
				pIsStatic = &pCastResultUnitTypeStack->is_static_Out_EXTRACT;
				pCSPresent = &pCastResultUnitTypeStack->Out_EXTRACT_control_symbol;
				pCSPresent_site = &pCastResultUnitTypeStack->First_control_symbol_offset__Out_EXTRACT_control_symbol;
				pQuick_combine_type = &pCastResultUnitTypeStack->Quick_combine_type__Out_EXTRACT;
				pSet_size_typedef_for_CS_REGISTER = &pCastResultUnitTypeStack->CS_REGISTER__size_type__Out_EXTRACT;
				break;
			}//end VMPX_CONTEXT_ASM_EXTRACT

		case VMPX_CONTEXT_ASM_EXTRACT_VALUE_LINK:
			{
				pAsmInstruction = &pCastResultUnitTypeStack->Out_EXTRACT_VALUE_LINK[0];
				pXED_parse_asm = &pCastResultUnitTypeStack->xed_Out_EXTRACT_VALUE_LINK;
				pIsStatic = &pCastResultUnitTypeStack->is_static_Out_EXTRACT_VALUE_LINK;
				pCSPresent = &pCastResultUnitTypeStack->Out_EXTRACT_VALUE_LINK_control_symbol;
				pCSPresent_site = &pCastResultUnitTypeStack->First_control_symbol_offset__Out_EXTRACT_VALUE_LINK_control_symbol;
				pQuick_combine_type = &pCastResultUnitTypeStack->Quick_combine_type__Out_EXTRACT_VALUE_LINK;
				pSet_size_typedef_for_CS_REGISTER = &pCastResultUnitTypeStack->CS_REGISTER__size_type__Out_EXTRACT_VALUE_LINK;
				break;
			}//end VMPX_CONTEXT_ASM_EXTRACT_VALUE_LINK
	}

	register TCHAR* fpParseAsmCSymbol = pAsmInstruction;
	register size_t save_original_asm_size = AsmInstruction_size;
	unsigned char prev_instr[XEDPARSE_MAXASMSIZE]; //for static
	char fix_static_instr[XEDPARSE_MAXBUFSIZE]; //скорректированная инструкция для статической проверки

	*pCSPresent = NULL;
	*pQuick_combine_type = NULL;
	pXED_parse_asm->x64 = true;
	pXED_parse_asm->cip = PREDICT_ADDR_CONST; //!=PREDICT_ADDR_DISTANCE;
	do
	{
		if(*fpParseAsmCSymbol++ != PROFILE_VMPX_CONTROL_SYMBOL)
			continue;
		//CS present
			switch(*fpParseAsmCSymbol)
			{
			case PROFILE_VMPX_CONTROL_SYMBOL_HIMSELF_ADDR:
				{
				*pCSPresent = PROFILE_VMPX_CONTROL_SYMBOL_HIMSELF_ADDRES; 
				*pCSPresent_site = (save_original_asm_size - AsmInstruction_size)*sizeof(TCHAR);
				//TCHAR* f = &pAsmInstruction[*pSetCS_first_symbol];
				quick_UNICODE_to_ASCII_with_ltoa16(pAsmInstruction,pXED_parse_asm->instr, fpParseAsmCSymbol-1, PREDICT_ADDR_CONST);
				quick_UNICODE_to_ASCII_with_ltoa16(pAsmInstruction, &fix_static_instr[0], fpParseAsmCSymbol-1, PREDICT_ADDR_DISTANCE);
				break;}//end PROFILE_VMPX_CONTROL_SYMBOL_HIMSELF_ADDR

				case PROFILE_VMPX_CONTROL_SYMBOL_EXTRACT_ADDR:
				{
				*pCSPresent = PROFILE_VMPX_CONTROL_SYMBOL_EXTRACT_ADDR; 
				*pCSPresent_site = (save_original_asm_size - AsmInstruction_size)*sizeof(TCHAR);
				//TCHAR* f = &pAsmInstruction[*pSetCS_first_symbol];
				quick_UNICODE_to_ASCII_with_ltoa16(pAsmInstruction,pXED_parse_asm->instr, fpParseAsmCSymbol-1, PREDICT_ADDR_CONST);
				quick_UNICODE_to_ASCII_with_ltoa16(pAsmInstruction, &fix_static_instr[0], fpParseAsmCSymbol-1, PROFILE_VMPX_CONTROL_SYMBOL_EXTRACT_ADDR_constant);
				break;}//end PROFILE_VMPX_CONTROL_SYMBOL_HIMSELF_ADDR

				case PROFILE_VMPX_CONTROL_SYMBOL_CPU_REGISTER:
				{
				*pCSPresent = PROFILE_VMPX_CONTROL_SYMBOL_CPU_REGISTER; 
				*pCSPresent_site = (save_original_asm_size - AsmInstruction_size)*sizeof(TCHAR);

					// --- detect register size --- //
					quick_UNICODE_to_ASCII_with_insert(pAsmInstruction, pXED_parse_asm->instr, fpParseAsmCSymbol-1, PROFILE_VMPX_CONTROL_SYMBOL_CPU_REGISTER_QWORD_default_ascii);
					if (XEDParseAssemble(pXED_parse_asm) == XEDPARSE_ERROR) //bad QWORD
						{
							quick_UNICODE_to_ASCII_with_insert(pAsmInstruction, pXED_parse_asm->instr, fpParseAsmCSymbol-1, PROFILE_VMPX_CONTROL_SYMBOL_CPU_REGISTER_DWORD_default_ascii);
							if (XEDParseAssemble(pXED_parse_asm) == XEDPARSE_ERROR) //bad QWORD
							{
								quick_UNICODE_to_ASCII_with_insert(pAsmInstruction, pXED_parse_asm->instr, fpParseAsmCSymbol-1, PROFILE_VMPX_CONTROL_SYMBOL_CPU_REGISTER_WORD_default_ascii);
								if (XEDParseAssemble(pXED_parse_asm) == XEDPARSE_ERROR) //bad QWORD
								{
										quick_UNICODE_to_ASCII_with_insert(pAsmInstruction, pXED_parse_asm->instr, fpParseAsmCSymbol-1, PROFILE_VMPX_CONTROL_SYMBOL_CPU_REGISTER_BYTE_default_ascii);
										if (XEDParseAssemble(pXED_parse_asm) == XEDPARSE_ERROR) //bad QWORD
										{
											*pSet_size_typedef_for_CS_REGISTER = NULL;
										}//end BYTE CHECK
										else
										*pSet_size_typedef_for_CS_REGISTER = CS_REGISTER_TYPEF_SIZE_BYTE;
								}//end WORD CHECK
								else
								*pSet_size_typedef_for_CS_REGISTER = CS_REGISTER_TYPEF_SIZE_WORD;
							}//end DWORD CHECK
							else
							*pSet_size_typedef_for_CS_REGISTER = CS_REGISTER_TYPEF_SIZE_QWORD;	
						}//end QWORD CHECK
					else
						*pSet_size_typedef_for_CS_REGISTER = CS_REGISTER_TYPEF_SIZE_QWORD;
				
				
				}//end PROFILE_VMPX_CONTROL_SYMBOL_HIMSELF_ADDR

			}//end switch
				if (--AsmInstruction_size == NULL)
						break;
	}while(--AsmInstruction_size);

	
	if(*pCSPresent == NULL) //CS is absent - copy now
		quick_UNICODE_to_ASCII(pAsmInstruction, &pXED_parse_asm->instr[0]);

		if (XEDParseAssemble(pXED_parse_asm) == XEDPARSE_ERROR) //XED parse +++++++++++++++++++++++++
						return false;

		//check for static/dynamic ////////////
		AsmInstruction_size = pXED_parse_asm->dest_size;
		memcpy(&prev_instr[0], &pXED_parse_asm->dest[0], AsmInstruction_size);
		pXED_parse_asm->cip = PREDICT_ADDR_DISTANCE;
		pXED_parse_asm->dest_size = NULL;

		if(*pCSPresent != NULL && *pCSPresent != PROFILE_VMPX_CONTROL_SYMBOL_CPU_REGISTER) //CS is present - correct assembly string
			strcpy(&pXED_parse_asm->instr[0], &fix_static_instr[0]);
		

			if (XEDParseAssemble(pXED_parse_asm) == XEDPARSE_ERROR) //XED parse +++++++++++++++++++++++++
						return false;

		if((pXED_parse_asm->dest_size == AsmInstruction_size) && !memcmp(&prev_instr[0], &pXED_parse_asm->dest[0], AsmInstruction_size))
			*pIsStatic = true;
		else
			*pIsStatic = false;
		//end static/dynamic check ////////////


			// ------- set quick access type // ------- 
			switch(*pIsStatic)
			{
				//static
			case true:
				{
					if(*pCSPresent == NULL)
						*pQuick_combine_type = VMPX_CONTEXT_COMBINE_STATIC__NO_CS;
					else
						*pQuick_combine_type = VMPX_CONTEXT_COMBINE_STATIC__CS_PRESENT;
				break;}
				//dynamic
			case false:
				{
					if(*pCSPresent == NULL)
						*pQuick_combine_type = VMPX_CONTEXT_COMBINE_DYNAMIC__NO_CS;
					else
						*pQuick_combine_type = VMPX_CONTEXT_COMBINE_DYNAMIC__CS_PRESENT;
				break;}
			}// ------- end // ------- // ------- 

	// ********** EXTENDED IDENTEFICATION  ****************************
	switch(Ident_normalize) //какую асм нормализируем
	{
			case VMPX_CONTEXT_ASM_EXTRACT:
			{
				if(pCastResultUnitTypeStack->Out_EXTRACT_len != NULL)
				{
					//PROFILE_VMPX_CONTROL_SYMBOL_EXTRACT_ADDR_constant
					pCastResultUnitTypeStack->bea_Out_EXTRACT.Archi = 64;
					pCastResultUnitTypeStack->bea_Out_EXTRACT.VirtualAddr = pXED_parse_asm->cip;
					pCastResultUnitTypeStack->bea_Out_EXTRACT.EIP = (UIntPtr)&pXED_parse_asm->dest[0];
					pCastResultUnitTypeStack->bea_Out_EXTRACT.Instruction.Opcode = NULL;
					Disasm(&pCastResultUnitTypeStack->bea_Out_EXTRACT);

					if(pCastResultUnitTypeStack->bea_Out_EXTRACT.Instruction.AddrValue != PROFILE_VMPX_CONTROL_SYMBOL_EXTRACT_ADDR_constant)
						return false;
				}// END IF (pCastResultUnitTypeStack->Out_EXTRACT_len != NULL)
			break;}//end case VMPX_CONTEXT_ASM_EXTRACT

			case VMPX_CONTEXT_ASM_EXTRACT_VALUE_LINK:
			{
				if(pCastResultUnitTypeStack->Out_EXTRACT_VALUE_LINK_len != NULL)
				{
					pCastResultUnitTypeStack->bea_Out_EXTRACT_VALUE_LINK.Archi = 64;
					pCastResultUnitTypeStack->bea_Out_EXTRACT_VALUE_LINK.VirtualAddr = pXED_parse_asm->cip;
					pCastResultUnitTypeStack->bea_Out_EXTRACT_VALUE_LINK.EIP = (UIntPtr)&pXED_parse_asm->dest[0];
					pCastResultUnitTypeStack->bea_Out_EXTRACT_VALUE_LINK.Instruction.Opcode = NULL;
					Disasm(&pCastResultUnitTypeStack->bea_Out_EXTRACT_VALUE_LINK);
				}// END IF (pCastResultUnitTypeStack->Out_EXTRACT_VALUE_LINK_len != NULL)
			break;}//end case VMPX_CONTEXT_ASM_EXTRACT
	}//end switch(Ident_normalize)

	return true;
}


__forceinline bool DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Release_normalize_instruction(__in __out LPVOID pResultUnitTypeStack, __in SERVICE_TYPE Ident_normalize, __in PVOID pInstruction_pool, __in PVOID pSeqStack)
{
   register pVM_VMPX_PROFILE_INI pCastResultUnitTypeStack = reinterpret_cast<pVM_VMPX_PROFILE_INI>(pResultUnitTypeStack);
			pVM_VMPX_PROFILE_FOUND_SEQUENCE  pCastSeqStack= reinterpret_cast<pVM_VMPX_PROFILE_FOUND_SEQUENCE>(pSeqStack);
	TCHAR* pAsmInstruction; //указатель на строку инструкции
	XEDPARSE* pXED_parse_asm; //указатель на структуру xedparse
	DISASM* pBEA_dissasm; //указатель на структуру BEA EGINE
	bool IsStatic; //указатель на флаг static/dynamic
	SERVICE_TYPE CSPresent; //указатель на контрольный символ
	size_t CSPresent_site; //указатель на место присутсвия контрольного символа
	SERVICE_TYPE  Quick_combine_type; //указатель на тип быстрого определения асм
	SERVICE_TYPE Set_size_typedef_for_CS_REGISTER; //УКАЗАТЕЛЬ на размерность регистра
	
	SERVICE_ID* pCS_latest_instead_CPU_Register; //последний, успешно подставленный регистр CPU для контрольного символа 

	//inrement state
	/*switch(pCastResultUnitTypeStack->INCREMENT_STANDBY_TYPE) //режим инкремента подставляемых данных
	{
		case INREMENTAL_LOCK: {break;}
		case INREMENTAL_EXECUTE: {pCastResultUnitTypeStack->INCREMENT_STANDBY_TYPE++; break;}
		case INREMENTAL_RESET: {pCastResultUnitTypeStack->INCREMENT_STANDBY_TYPE = 1; break;}
	}//end switch incremental*/


	//TYPEDEF SWITCHER
	switch(Ident_normalize) //какую асм нормализируем
	{
		case VMPX_CONTEXT_ASM_STATIC:
		{
			pAsmInstruction = &pCastResultUnitTypeStack->Predict_STATIC_assembly[0];
			pXED_parse_asm = &pCastResultUnitTypeStack->xed_Predict_STATIC;
			pBEA_dissasm = &pCastResultUnitTypeStack->bea_Predict_STATIC;
			IsStatic = pCastResultUnitTypeStack->is_static_Predict_STATIC_assembly;
			CSPresent = pCastResultUnitTypeStack->Predict_wait_STATIC_constrol_symbol;
			CSPresent_site = pCastResultUnitTypeStack->First_control_symbol_offset__Predict_wait_STATIC_constrol_symbol;
			Quick_combine_type = pCastResultUnitTypeStack->Quick_combine_type__Predict_STATIC;
			Set_size_typedef_for_CS_REGISTER = pCastResultUnitTypeStack->CS_REGISTER__size_type__Predict_STATIC;
			break;
		}//end VMPX_CONTEXT_ASM_STATIC

		//predict_wait (cycle)
	case VMPX_CONTEXT_ASM_PREDICT_WAIT_1:
		{
			pAsmInstruction = &pCastResultUnitTypeStack->Predict_wait_assembly1[0];
			pXED_parse_asm = &pCastResultUnitTypeStack->xed_Predict_wait_assembly1;
			pBEA_dissasm = NULL;
			IsStatic = pCastResultUnitTypeStack->is_static_Predict_wait_assembly1;
			CSPresent = pCastResultUnitTypeStack->Predict_wait_1_control_symbol;
			CSPresent_site = pCastResultUnitTypeStack->First_control_symbol_offset__Predict_wait_1_control_symbol;
			Quick_combine_type = pCastResultUnitTypeStack->Quick_combine_type__Predict_wait_1;
			Set_size_typedef_for_CS_REGISTER = pCastResultUnitTypeStack->CS_REGISTER__size_type__Predict_wait_1;
			pCS_latest_instead_CPU_Register = &pCastSeqStack->CS_Predict_wait1__CPU_REGISTER;
			break;
		}//end VMPX_CONTEXT_ASM_PREDICT_WAIT_1

		case VMPX_CONTEXT_ASM_PREDICT_WAIT_2:
		{
			pAsmInstruction = &pCastResultUnitTypeStack->Predict_wait_assembly2[0];
			pXED_parse_asm = &pCastResultUnitTypeStack->xed_Predict_wait_assembly2;
			pBEA_dissasm = NULL;
			IsStatic = pCastResultUnitTypeStack->is_static_Predict_wait_assembly2;
			CSPresent = pCastResultUnitTypeStack->Predict_wait_2_control_symbol;
			CSPresent_site = pCastResultUnitTypeStack->First_control_symbol_offset__Predict_wait_2_control_symbol;
			Quick_combine_type = pCastResultUnitTypeStack->Quick_combine_type__Predict_wait_2;
			Set_size_typedef_for_CS_REGISTER = pCastResultUnitTypeStack->CS_REGISTER__size_type__Predict_wait_2;
			pCS_latest_instead_CPU_Register = &pCastSeqStack->CS_Predict_wait2__CPU_REGISTER;
			break;
		}//end VMPX_CONTEXT_ASM_PREDICT_WAIT_2

		case VMPX_CONTEXT_ASM_PREDICT_WAIT_3:
		{
			pAsmInstruction = &pCastResultUnitTypeStack->Predict_wait_assembly3[0];
			pXED_parse_asm = &pCastResultUnitTypeStack->xed_Predict_wait_assembly3;
			pBEA_dissasm = NULL;
			IsStatic = pCastResultUnitTypeStack->is_static_Predict_wait_assembly3;
			CSPresent = pCastResultUnitTypeStack->Predict_wait_3_control_symbol;
			CSPresent_site = pCastResultUnitTypeStack->First_control_symbol_offset__Predict_wait_3_control_symbol;
			Quick_combine_type = pCastResultUnitTypeStack->Quick_combine_type__Predict_wait_3;
			Set_size_typedef_for_CS_REGISTER = pCastResultUnitTypeStack->CS_REGISTER__size_type__Predict_wait_3;
			pCS_latest_instead_CPU_Register = &pCastSeqStack->CS_Predict_wait3__CPU_REGISTER;
			break;
		}//end VMPX_CONTEXT_ASM_PREDICT_WAIT_3

		case VMPX_CONTEXT_ASM_PREDICT_WAIT_4:
		{
			pAsmInstruction = &pCastResultUnitTypeStack->Predict_wait_assembly4[0];
			pXED_parse_asm = &pCastResultUnitTypeStack->xed_Predict_wait_assembly4;
			pBEA_dissasm = NULL;
			IsStatic = pCastResultUnitTypeStack->is_static_Predict_wait_assembly4;
			CSPresent = pCastResultUnitTypeStack->Predict_wait_4_control_symbol;
			CSPresent_site = pCastResultUnitTypeStack->First_control_symbol_offset__Predict_wait_4_control_symbol;
			Quick_combine_type = pCastResultUnitTypeStack->Quick_combine_type__Predict_wait_4;
			Set_size_typedef_for_CS_REGISTER = pCastResultUnitTypeStack->CS_REGISTER__size_type__Predict_wait_4;
			pCS_latest_instead_CPU_Register = &pCastSeqStack->CS_Predict_wait4__CPU_REGISTER;
			break;
		}//end VMPX_CONTEXT_ASM_PREDICT_WAIT_4

		case VMPX_CONTEXT_ASM_CRITICAL:
		{
			pAsmInstruction = &pCastResultUnitTypeStack->Critial_assembly[0];
			pXED_parse_asm = &pCastResultUnitTypeStack->xed_Critial_assembly;
			pBEA_dissasm = &pCastResultUnitTypeStack->bea_Critial_assembly;
			IsStatic = pCastResultUnitTypeStack->is_static_Critial_assembly;
			CSPresent = pCastResultUnitTypeStack->Critical_control_symbol;
			CSPresent_site = pCastResultUnitTypeStack->First_control_symbol_offset__Critical_control_symbol;
			Quick_combine_type = pCastResultUnitTypeStack->Quick_combine_type__Critical_control;
			Set_size_typedef_for_CS_REGISTER = pCastResultUnitTypeStack->CS_REGISTER__size_type__Critical_control;
			pCS_latest_instead_CPU_Register = &pCastSeqStack->CS_Critical__CPU_REGISTER;
			break;
		}//end VMPX_CONTEXT_ASM_CRITICAL

		case VMPX_CONTEXT_ASM_AFTER_1:
		{
			pAsmInstruction = &pCastResultUnitTypeStack->After_critical_assebly1[0];
			pXED_parse_asm = &pCastResultUnitTypeStack->xed_After_critical_assebly1;
			pBEA_dissasm = NULL;
			IsStatic = pCastResultUnitTypeStack->is_static_After_critical_assebly1;
			CSPresent = pCastResultUnitTypeStack->After_1_control_symbol;
			CSPresent_site = pCastResultUnitTypeStack->First_control_symbol_offset__After_1_control_symbol;
			Quick_combine_type = pCastResultUnitTypeStack->Quick_combine_type__After_1;
			Set_size_typedef_for_CS_REGISTER = pCastResultUnitTypeStack->CS_REGISTER__size_type__After_1;
			pCS_latest_instead_CPU_Register = &pCastSeqStack->CS_After1__CPU_REGISTER;
			break;
		}//end VMPX_CONTEXT_ASM_AFTER_1

		case VMPX_CONTEXT_ASM_AFTER_2:
		{
			pAsmInstruction = &pCastResultUnitTypeStack->After_critical_assebly2[0];
			pXED_parse_asm = &pCastResultUnitTypeStack->xed_After_critical_assebly2;
			pBEA_dissasm = NULL;
			IsStatic = pCastResultUnitTypeStack->is_static_After_critical_assebly2;
			CSPresent = pCastResultUnitTypeStack->After_2_control_symbol;
			CSPresent_site = pCastResultUnitTypeStack->First_control_symbol_offset__After_2_control_symbol;
			Quick_combine_type = pCastResultUnitTypeStack->Quick_combine_type__After_2;
			Set_size_typedef_for_CS_REGISTER = pCastResultUnitTypeStack->CS_REGISTER__size_type__After_2;
			pCS_latest_instead_CPU_Register = &pCastSeqStack->CS_After2__CPU_REGISTER;
			break;
		}//end VMPX_CONTEXT_ASM_AFTER_2

		case VMPX_CONTEXT_ASM_EXTRACT:
			{
				pAsmInstruction = &pCastResultUnitTypeStack->Out_EXTRACT[0];
				pXED_parse_asm = &pCastResultUnitTypeStack->xed_Out_EXTRACT;
				pBEA_dissasm = &pCastResultUnitTypeStack->bea_Out_EXTRACT;
				IsStatic = pCastResultUnitTypeStack->is_static_Out_EXTRACT;
				CSPresent = pCastResultUnitTypeStack->Out_EXTRACT_control_symbol;
				CSPresent_site = pCastResultUnitTypeStack->First_control_symbol_offset__Out_EXTRACT_control_symbol;
				Quick_combine_type = pCastResultUnitTypeStack->Quick_combine_type__Out_EXTRACT;
				Set_size_typedef_for_CS_REGISTER = pCastResultUnitTypeStack->CS_REGISTER__size_type__Out_EXTRACT;
				pCS_latest_instead_CPU_Register = &pCastSeqStack->CS_EXTRACT__CPU_REGISTER;
				break;
			}//end VMPX_CONTEXT_ASM_EXTRACT

			case VMPX_CONTEXT_ASM_EXTRACT_VALUE_LINK:
			{
				pAsmInstruction = &pCastResultUnitTypeStack->Out_EXTRACT_VALUE_LINK[0];
				pXED_parse_asm = &pCastResultUnitTypeStack->xed_Out_EXTRACT_VALUE_LINK;
				pBEA_dissasm = &pCastResultUnitTypeStack->bea_Out_EXTRACT_VALUE_LINK;
				IsStatic = &pCastResultUnitTypeStack->is_static_Out_EXTRACT_VALUE_LINK;
				CSPresent = pCastResultUnitTypeStack->Out_EXTRACT_VALUE_LINK_control_symbol;
				CSPresent_site = pCastResultUnitTypeStack->First_control_symbol_offset__Out_EXTRACT_VALUE_LINK_control_symbol;
				Quick_combine_type = pCastResultUnitTypeStack->Quick_combine_type__Out_EXTRACT_VALUE_LINK;
				Set_size_typedef_for_CS_REGISTER = pCastResultUnitTypeStack->CS_REGISTER__size_type__Out_EXTRACT_VALUE_LINK;
				pCS_latest_instead_CPU_Register = &pCastSeqStack->CS_EXTRACT_VALUE_LINK__CPU_REGISTER;
				break;
			}//end VMPX_CONTEXT_ASM_EXTRACT_VALUE_LINK
	}//end ASM restore by "Ident_normalize" switcher


	//restore CS site
	register TCHAR* fpAsmInstruction = (TCHAR*)((SERVICE_ADDRES)pAsmInstruction + CSPresent_site);

	//restore type of CS - normalize from XEDParse string
	switch(CSPresent)
	{
		case PROFILE_VMPX_CONTROL_SYMBOL_HIMSELF_ADDR:
			{
						quick_UNICODE_to_ASCII_with_ltoa16(pAsmInstruction,pXED_parse_asm->instr, fpAsmInstruction, pXED_parse_asm->cip);
								if (XEDParseAssemble(pXED_parse_asm) == XEDPARSE_ERROR) //XED parse +++++++++++++++++++++++++
									return false;

								if(!memcmp(pInstruction_pool, &pXED_parse_asm->dest[0], pXED_parse_asm->dest_size)) // retail compare!!!!!!!!!!!!!!!!!!!!!!
									return true;

			break;}//end case PROFILE_VMPX_CONTROL_SYMBOL_HIMSELF_ADDR

			case PROFILE_VMPX_CONTROL_SYMBOL_CPU_REGISTER:
			{
				register char* Inset_register = NULL;
				switch(pCastResultUnitTypeStack->INCREMENT_STANDBY_TYPE) //режим инкремента подставляемых данных
				{
					case INREMENTAL_EXECUTE: 
						{
						//		do
							//	{
								//		switch(i)
									//	{
											//COMMON 8
											codedef_register_switcher(INCREMENTAL_CPU_RAX, Inset_register, TS_CPU_REG_RAX_ascii, TS_CPU_REG_EAX_ascii, TS_CPU_REG_AX_ascii, TS_CPU_REG_AL_ascii);
											codedef_register_switcher(INCREMENTAL_CPU_RBX, Inset_register, TS_CPU_REG_RBX_ascii, TS_CPU_REG_EBX_ascii, TS_CPU_REG_BX_ascii, TS_CPU_REG_BL_ascii);
											codedef_register_switcher(INCREMENTAL_CPU_RCX, Inset_register, TS_CPU_REG_RCX_ascii, TS_CPU_REG_ECX_ascii, TS_CPU_REG_CX_ascii, TS_CPU_REG_CL_ascii);
											codedef_register_switcher(INCREMENTAL_CPU_RDX, Inset_register, TS_CPU_REG_RDX_ascii, TS_CPU_REG_EDX_ascii, TS_CPU_REG_DX_ascii, TS_CPU_REG_DL_ascii);
											codedef_register_switcher(INCREMENTAL_CPU_RBP, Inset_register, TS_CPU_REG_RBP_ascii, TS_CPU_REG_EBP_ascii, TS_CPU_REG_BP_ascii, TS_CPU_REG_BPL_ascii);
											codedef_register_switcher(INCREMENTAL_CPU_RSP, Inset_register, TS_CPU_REG_RSP_ascii, TS_CPU_REG_ESP_ascii, TS_CPU_REG_SP_ascii, TS_CPU_REG_SPL_ascii);
											codedef_register_switcher(INCREMENTAL_CPU_RSI, Inset_register, TS_CPU_REG_RSI_ascii, TS_CPU_REG_RSI_ascii, TS_CPU_REG_SI_ascii, TS_CPU_REG_SIL_ascii);
											codedef_register_switcher(INCREMENTAL_CPU_RDI, Inset_register, TS_CPU_REG_RDI_ascii, TS_CPU_REG_RDI_ascii, TS_CPU_REG_DI_ascii, TS_CPU_REG_DIL_ascii);

											//NUMERIC
											codedef_register_switcher(INCREMENTAL_CPU_R8, Inset_register, TS_CPU_REG_R8_ascii, TS_CPU_REG_R8d_ascii, TS_CPU_REG_R8w_ascii, TS_CPU_REG_R8b_ascii);
											codedef_register_switcher(INCREMENTAL_CPU_R9, Inset_register, TS_CPU_REG_R9_ascii, TS_CPU_REG_R9d_ascii, TS_CPU_REG_R9w_ascii, TS_CPU_REG_R9b_ascii);
											codedef_register_switcher(INCREMENTAL_CPU_R10, Inset_register, TS_CPU_REG_R10_ascii, TS_CPU_REG_R10d_ascii, TS_CPU_REG_R10w_ascii, TS_CPU_REG_R10b_ascii);
											codedef_register_switcher(INCREMENTAL_CPU_R11, Inset_register, TS_CPU_REG_R11_ascii, TS_CPU_REG_R11d_ascii, TS_CPU_REG_R11w_ascii, TS_CPU_REG_R11b_ascii);
											codedef_register_switcher(INCREMENTAL_CPU_R12, Inset_register, TS_CPU_REG_R12_ascii, TS_CPU_REG_R12d_ascii, TS_CPU_REG_R12w_ascii, TS_CPU_REG_R12b_ascii);
											codedef_register_switcher(INCREMENTAL_CPU_R13, Inset_register, TS_CPU_REG_R13_ascii, TS_CPU_REG_R13d_ascii, TS_CPU_REG_R13w_ascii, TS_CPU_REG_R13b_ascii);
											codedef_register_switcher(INCREMENTAL_CPU_R14, Inset_register, TS_CPU_REG_R14_ascii, TS_CPU_REG_R14d_ascii, TS_CPU_REG_R14w_ascii, TS_CPU_REG_R14b_ascii);
											codedef_register_switcher(INCREMENTAL_CPU_R15, Inset_register, TS_CPU_REG_R15_ascii, TS_CPU_REG_R15d_ascii, TS_CPU_REG_R15w_ascii, TS_CPU_REG_R15b_ascii);
									//	}//end Increment_id switcher
						//		}while(--i > 0);
							break;} //end case INREMENTAL_EXECUTE
				case INREMENTAL_LOCK: {
									
											switch(pCastResultUnitTypeStack->INCREMENT_ID)
											{
												//COMMON 8
												codedef_register_switcher(INCREMENTAL_CPU_RAX, Inset_register, TS_CPU_REG_RAX_ascii, TS_CPU_REG_EAX_ascii, TS_CPU_REG_AX_ascii, TS_CPU_REG_AL_ascii);
												codedef_register_switcher(INCREMENTAL_CPU_RBX, Inset_register, TS_CPU_REG_RBX_ascii, TS_CPU_REG_EBX_ascii, TS_CPU_REG_BX_ascii, TS_CPU_REG_BL_ascii);
												codedef_register_switcher(INCREMENTAL_CPU_RCX, Inset_register, TS_CPU_REG_RCX_ascii, TS_CPU_REG_ECX_ascii, TS_CPU_REG_CX_ascii, TS_CPU_REG_CL_ascii);
												codedef_register_switcher(INCREMENTAL_CPU_RDX, Inset_register, TS_CPU_REG_RDX_ascii, TS_CPU_REG_EDX_ascii, TS_CPU_REG_DX_ascii, TS_CPU_REG_DL_ascii);
												codedef_register_switcher(INCREMENTAL_CPU_RBP, Inset_register, TS_CPU_REG_RBP_ascii, TS_CPU_REG_EBP_ascii, TS_CPU_REG_BP_ascii, TS_CPU_REG_BPL_ascii);
												codedef_register_switcher(INCREMENTAL_CPU_RSP, Inset_register, TS_CPU_REG_RSP_ascii, TS_CPU_REG_ESP_ascii, TS_CPU_REG_SP_ascii, TS_CPU_REG_SPL_ascii);
												codedef_register_switcher(INCREMENTAL_CPU_RSI, Inset_register, TS_CPU_REG_RSI_ascii, TS_CPU_REG_RSI_ascii, TS_CPU_REG_SI_ascii, TS_CPU_REG_SIL_ascii);
												codedef_register_switcher(INCREMENTAL_CPU_RDI, Inset_register, TS_CPU_REG_RDI_ascii, TS_CPU_REG_RDI_ascii, TS_CPU_REG_DI_ascii, TS_CPU_REG_DIL_ascii);

												//NUMERIC
												codedef_register_switcher(INCREMENTAL_CPU_R8, Inset_register, TS_CPU_REG_R8_ascii, TS_CPU_REG_R8d_ascii, TS_CPU_REG_R8w_ascii, TS_CPU_REG_R8b_ascii);
												codedef_register_switcher(INCREMENTAL_CPU_R9, Inset_register, TS_CPU_REG_R9_ascii, TS_CPU_REG_R9d_ascii, TS_CPU_REG_R9w_ascii, TS_CPU_REG_R9b_ascii);
												codedef_register_switcher(INCREMENTAL_CPU_R10, Inset_register, TS_CPU_REG_R10_ascii, TS_CPU_REG_R10d_ascii, TS_CPU_REG_R10w_ascii, TS_CPU_REG_R10b_ascii);
												codedef_register_switcher(INCREMENTAL_CPU_R11, Inset_register, TS_CPU_REG_R11_ascii, TS_CPU_REG_R11d_ascii, TS_CPU_REG_R11w_ascii, TS_CPU_REG_R11b_ascii);
												codedef_register_switcher(INCREMENTAL_CPU_R12, Inset_register, TS_CPU_REG_R12_ascii, TS_CPU_REG_R12d_ascii, TS_CPU_REG_R12w_ascii, TS_CPU_REG_R12b_ascii);
												codedef_register_switcher(INCREMENTAL_CPU_R13, Inset_register, TS_CPU_REG_R13_ascii, TS_CPU_REG_R13d_ascii, TS_CPU_REG_R13w_ascii, TS_CPU_REG_R13b_ascii);
												codedef_register_switcher(INCREMENTAL_CPU_R14, Inset_register, TS_CPU_REG_R14_ascii, TS_CPU_REG_R14d_ascii, TS_CPU_REG_R14w_ascii, TS_CPU_REG_R14b_ascii);
												codedef_register_switcher(INCREMENTAL_CPU_R15, Inset_register, TS_CPU_REG_R15_ascii, TS_CPU_REG_R15d_ascii, TS_CPU_REG_R15w_ascii, TS_CPU_REG_R15b_ascii);
											}//end Increment_id switcher
												if(!Inset_register)
												{
													quick_UNICODE_to_ASCII_with_insert(pAsmInstruction, pXED_parse_asm->instr, fpAsmInstruction, Inset_register);
													if (XEDParseAssemble(pXED_parse_asm) == XEDPARSE_ERROR) //XED parse +++++++++++++++++++++++++
																return false;

															if(!memcmp(pInstruction_pool, &pXED_parse_asm->dest[0], pXED_parse_asm->dest_size)) // retail compare!!!!!!!!!!!!!!!!!!!!!!
																return true;
												}//end if(!Inset_register)
					
					break;}
				}//end switcher pCastResultUnitTypeStack->INCREMENT_STANDBY_TYPE 
			break;}
	}//end CS type switcher
return false;
}


__forceinline bool DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Release_normalize_instruction__NO_WRITE(__in __out LPVOID pResultUnitTypeStack, __in SERVICE_TYPE Ident_normalize, __in PVOID pInstruction_pool)
{
   register pVM_VMPX_PROFILE_INI pCastResultUnitTypeStack = reinterpret_cast<pVM_VMPX_PROFILE_INI>(pResultUnitTypeStack);
	TCHAR* pAsmInstruction; //указатель на строку инструкции
	XEDPARSE* pXED_parse_asm; //указатель на структуру xedparse
	DISASM* pBEA_dissasm; //указатель на структуру BEA EGINE
	bool IsStatic; //указатель на флаг static/dynamic
	SERVICE_TYPE CSPresent; //указатель на контрольный символ
	size_t CSPresent_site; //указатель на место присутсвия контрольного символа
	SERVICE_TYPE  Quick_combine_type; //указатель на тип быстрого определения асм
	SERVICE_TYPE Set_size_typedef_for_CS_REGISTER; //УКАЗАТЕЛЬ на размерность регистра
	
	//inrement state
	/*switch(pCastResultUnitTypeStack->INCREMENT_STANDBY_TYPE) //режим инкремента подставляемых данных
	{
		case INREMENTAL_LOCK: {break;}
		case INREMENTAL_EXECUTE: {pCastResultUnitTypeStack->INCREMENT_STANDBY_TYPE++; break;}
		case INREMENTAL_RESET: {pCastResultUnitTypeStack->INCREMENT_STANDBY_TYPE = 1; break;}
	}//end switch incremental*/


	//TYPEDEF SWITCHER
	switch(Ident_normalize) //какую асм нормализируем
	{
		case VMPX_CONTEXT_ASM_STATIC:
		{
			pAsmInstruction = &pCastResultUnitTypeStack->Predict_STATIC_assembly[0];
			pXED_parse_asm = &pCastResultUnitTypeStack->xed_Predict_STATIC;
			pBEA_dissasm = &pCastResultUnitTypeStack->bea_Predict_STATIC;
			IsStatic = pCastResultUnitTypeStack->is_static_Predict_STATIC_assembly;
			CSPresent = pCastResultUnitTypeStack->Predict_wait_STATIC_constrol_symbol;
			CSPresent_site = pCastResultUnitTypeStack->First_control_symbol_offset__Predict_wait_STATIC_constrol_symbol;
			Quick_combine_type = pCastResultUnitTypeStack->Quick_combine_type__Predict_STATIC;
			Set_size_typedef_for_CS_REGISTER = pCastResultUnitTypeStack->CS_REGISTER__size_type__Predict_STATIC;
			break;
		}//end VMPX_CONTEXT_ASM_STATIC

		//predict_wait (cycle)
	case VMPX_CONTEXT_ASM_PREDICT_WAIT_1:
		{
			pAsmInstruction = &pCastResultUnitTypeStack->Predict_wait_assembly1[0];
			pXED_parse_asm = &pCastResultUnitTypeStack->xed_Predict_wait_assembly1;
			pBEA_dissasm = NULL;
			IsStatic = pCastResultUnitTypeStack->is_static_Predict_wait_assembly1;
			CSPresent = pCastResultUnitTypeStack->Predict_wait_1_control_symbol;
			CSPresent_site = pCastResultUnitTypeStack->First_control_symbol_offset__Predict_wait_1_control_symbol;
			Quick_combine_type = pCastResultUnitTypeStack->Quick_combine_type__Predict_wait_1;
			Set_size_typedef_for_CS_REGISTER = pCastResultUnitTypeStack->CS_REGISTER__size_type__Predict_wait_1;
			break;
		}//end VMPX_CONTEXT_ASM_PREDICT_WAIT_1

		case VMPX_CONTEXT_ASM_PREDICT_WAIT_2:
		{
			pAsmInstruction = &pCastResultUnitTypeStack->Predict_wait_assembly2[0];
			pXED_parse_asm = &pCastResultUnitTypeStack->xed_Predict_wait_assembly2;
			pBEA_dissasm = NULL;
			IsStatic = pCastResultUnitTypeStack->is_static_Predict_wait_assembly2;
			CSPresent = pCastResultUnitTypeStack->Predict_wait_2_control_symbol;
			CSPresent_site = pCastResultUnitTypeStack->First_control_symbol_offset__Predict_wait_2_control_symbol;
			Quick_combine_type = pCastResultUnitTypeStack->Quick_combine_type__Predict_wait_2;
			Set_size_typedef_for_CS_REGISTER = pCastResultUnitTypeStack->CS_REGISTER__size_type__Predict_wait_2;
			break;
		}//end VMPX_CONTEXT_ASM_PREDICT_WAIT_2

		case VMPX_CONTEXT_ASM_PREDICT_WAIT_3:
		{
			pAsmInstruction = &pCastResultUnitTypeStack->Predict_wait_assembly3[0];
			pXED_parse_asm = &pCastResultUnitTypeStack->xed_Predict_wait_assembly3;
			pBEA_dissasm = NULL;
			IsStatic = pCastResultUnitTypeStack->is_static_Predict_wait_assembly3;
			CSPresent = pCastResultUnitTypeStack->Predict_wait_3_control_symbol;
			CSPresent_site = pCastResultUnitTypeStack->First_control_symbol_offset__Predict_wait_3_control_symbol;
			Quick_combine_type = pCastResultUnitTypeStack->Quick_combine_type__Predict_wait_3;
			Set_size_typedef_for_CS_REGISTER = pCastResultUnitTypeStack->CS_REGISTER__size_type__Predict_wait_3;
			break;
		}//end VMPX_CONTEXT_ASM_PREDICT_WAIT_3

		case VMPX_CONTEXT_ASM_PREDICT_WAIT_4:
		{
			pAsmInstruction = &pCastResultUnitTypeStack->Predict_wait_assembly4[0];
			pXED_parse_asm = &pCastResultUnitTypeStack->xed_Predict_wait_assembly4;
			pBEA_dissasm = NULL;
			IsStatic = pCastResultUnitTypeStack->is_static_Predict_wait_assembly4;
			CSPresent = pCastResultUnitTypeStack->Predict_wait_4_control_symbol;
			CSPresent_site = pCastResultUnitTypeStack->First_control_symbol_offset__Predict_wait_4_control_symbol;
			Quick_combine_type = pCastResultUnitTypeStack->Quick_combine_type__Predict_wait_4;
			Set_size_typedef_for_CS_REGISTER = pCastResultUnitTypeStack->CS_REGISTER__size_type__Predict_wait_4;
			break;
		}//end VMPX_CONTEXT_ASM_PREDICT_WAIT_4

		case VMPX_CONTEXT_ASM_CRITICAL:
		{
			pAsmInstruction = &pCastResultUnitTypeStack->Critial_assembly[0];
			pXED_parse_asm = &pCastResultUnitTypeStack->xed_Critial_assembly;
			pBEA_dissasm = &pCastResultUnitTypeStack->bea_Critial_assembly;
			IsStatic = pCastResultUnitTypeStack->is_static_Critial_assembly;
			CSPresent = pCastResultUnitTypeStack->Critical_control_symbol;
			CSPresent_site = pCastResultUnitTypeStack->First_control_symbol_offset__Critical_control_symbol;
			Quick_combine_type = pCastResultUnitTypeStack->Quick_combine_type__Critical_control;
			Set_size_typedef_for_CS_REGISTER = pCastResultUnitTypeStack->CS_REGISTER__size_type__Critical_control;
			break;
		}//end VMPX_CONTEXT_ASM_CRITICAL

		case VMPX_CONTEXT_ASM_AFTER_1:
		{
			pAsmInstruction = &pCastResultUnitTypeStack->After_critical_assebly1[0];
			pXED_parse_asm = &pCastResultUnitTypeStack->xed_After_critical_assebly1;
			pBEA_dissasm = NULL;
			IsStatic = pCastResultUnitTypeStack->is_static_After_critical_assebly1;
			CSPresent = pCastResultUnitTypeStack->After_1_control_symbol;
			CSPresent_site = pCastResultUnitTypeStack->First_control_symbol_offset__After_1_control_symbol;
			Quick_combine_type = pCastResultUnitTypeStack->Quick_combine_type__After_1;
			Set_size_typedef_for_CS_REGISTER = pCastResultUnitTypeStack->CS_REGISTER__size_type__After_1;
			break;
		}//end VMPX_CONTEXT_ASM_AFTER_1

		case VMPX_CONTEXT_ASM_AFTER_2:
		{
			pAsmInstruction = &pCastResultUnitTypeStack->After_critical_assebly2[0];
			pXED_parse_asm = &pCastResultUnitTypeStack->xed_After_critical_assebly2;
			pBEA_dissasm = NULL;
			IsStatic = pCastResultUnitTypeStack->is_static_After_critical_assebly2;
			CSPresent = pCastResultUnitTypeStack->After_2_control_symbol;
			CSPresent_site = pCastResultUnitTypeStack->First_control_symbol_offset__After_2_control_symbol;
			Quick_combine_type = pCastResultUnitTypeStack->Quick_combine_type__After_2;
			Set_size_typedef_for_CS_REGISTER = pCastResultUnitTypeStack->CS_REGISTER__size_type__After_2;
			break;
		}//end VMPX_CONTEXT_ASM_AFTER_2

		case VMPX_CONTEXT_ASM_EXTRACT:
			{
				pAsmInstruction = &pCastResultUnitTypeStack->Out_EXTRACT[0];
				pXED_parse_asm = &pCastResultUnitTypeStack->xed_Out_EXTRACT;
				pBEA_dissasm = &pCastResultUnitTypeStack->bea_Out_EXTRACT;
				IsStatic = pCastResultUnitTypeStack->is_static_Out_EXTRACT;
				CSPresent = pCastResultUnitTypeStack->Out_EXTRACT_control_symbol;
				CSPresent_site = pCastResultUnitTypeStack->First_control_symbol_offset__Out_EXTRACT_control_symbol;
				Quick_combine_type = pCastResultUnitTypeStack->Quick_combine_type__Out_EXTRACT;
				Set_size_typedef_for_CS_REGISTER = pCastResultUnitTypeStack->CS_REGISTER__size_type__Out_EXTRACT;
				break;
			}//end VMPX_CONTEXT_ASM_EXTRACT

			case VMPX_CONTEXT_ASM_EXTRACT_VALUE_LINK:
			{
				pAsmInstruction = &pCastResultUnitTypeStack->Out_EXTRACT_VALUE_LINK[0];
				pXED_parse_asm = &pCastResultUnitTypeStack->xed_Out_EXTRACT_VALUE_LINK;
				pBEA_dissasm = &pCastResultUnitTypeStack->bea_Out_EXTRACT_VALUE_LINK;
				IsStatic = &pCastResultUnitTypeStack->is_static_Out_EXTRACT_VALUE_LINK;
				CSPresent = pCastResultUnitTypeStack->Out_EXTRACT_VALUE_LINK_control_symbol;
				CSPresent_site = pCastResultUnitTypeStack->First_control_symbol_offset__Out_EXTRACT_VALUE_LINK_control_symbol;
				Quick_combine_type = pCastResultUnitTypeStack->Quick_combine_type__Out_EXTRACT_VALUE_LINK;
				Set_size_typedef_for_CS_REGISTER = pCastResultUnitTypeStack->CS_REGISTER__size_type__Out_EXTRACT_VALUE_LINK;
				break;
			}//end VMPX_CONTEXT_ASM_EXTRACT_VALUE_LINK
	}//end ASM restore by "Ident_normalize" switcher


	//restore CS site
	register TCHAR* fpAsmInstruction = (TCHAR*)((SERVICE_ADDRES)pAsmInstruction + CSPresent_site);

	//restore type of CS - normalize from XEDParse string
	switch(CSPresent)
	{
		case PROFILE_VMPX_CONTROL_SYMBOL_HIMSELF_ADDR:
			{
						quick_UNICODE_to_ASCII_with_ltoa16(pAsmInstruction,pXED_parse_asm->instr, fpAsmInstruction, pXED_parse_asm->cip);
								if (XEDParseAssemble(pXED_parse_asm) == XEDPARSE_ERROR) //XED parse +++++++++++++++++++++++++
									return false;

								if(!memcmp(pInstruction_pool, &pXED_parse_asm->dest[0], pXED_parse_asm->dest_size)) // retail compare!!!!!!!!!!!!!!!!!!!!!!
									return true;

			break;}//end case PROFILE_VMPX_CONTROL_SYMBOL_HIMSELF_ADDR

			case PROFILE_VMPX_CONTROL_SYMBOL_CPU_REGISTER:
			{
				register char* Inset_register = NULL;
				switch(pCastResultUnitTypeStack->INCREMENT_STANDBY_TYPE) //режим инкремента подставляемых данных
				{
					case INREMENTAL_EXECUTE: 
						{
							register size_t Increment_id = INCREMENTAL_CPU_RAX;
							register size_t i = def_TYPEDEF_INREMENTAL_cpu_REGISTERS_size;
							//	do
								//{
								//		switch(i)
									//	{
											//COMMON 8
											codedef_register_switcher_no_write(INCREMENTAL_CPU_RAX, Inset_register, TS_CPU_REG_RAX_ascii, TS_CPU_REG_EAX_ascii, TS_CPU_REG_AX_ascii, TS_CPU_REG_AL_ascii);
											codedef_register_switcher_no_write(INCREMENTAL_CPU_RBX, Inset_register, TS_CPU_REG_RBX_ascii, TS_CPU_REG_EBX_ascii, TS_CPU_REG_BX_ascii, TS_CPU_REG_BL_ascii);
											codedef_register_switcher_no_write(INCREMENTAL_CPU_RCX, Inset_register, TS_CPU_REG_RCX_ascii, TS_CPU_REG_ECX_ascii, TS_CPU_REG_CX_ascii, TS_CPU_REG_CL_ascii);
											codedef_register_switcher_no_write(INCREMENTAL_CPU_RDX, Inset_register, TS_CPU_REG_RDX_ascii, TS_CPU_REG_EDX_ascii, TS_CPU_REG_DX_ascii, TS_CPU_REG_DL_ascii);
											codedef_register_switcher_no_write(INCREMENTAL_CPU_RBP, Inset_register, TS_CPU_REG_RBP_ascii, TS_CPU_REG_EBP_ascii, TS_CPU_REG_BP_ascii, TS_CPU_REG_BPL_ascii);
											codedef_register_switcher_no_write(INCREMENTAL_CPU_RSP, Inset_register, TS_CPU_REG_RSP_ascii, TS_CPU_REG_ESP_ascii, TS_CPU_REG_SP_ascii, TS_CPU_REG_SPL_ascii);
											codedef_register_switcher_no_write(INCREMENTAL_CPU_RSI, Inset_register, TS_CPU_REG_RSI_ascii, TS_CPU_REG_RSI_ascii, TS_CPU_REG_SI_ascii, TS_CPU_REG_SIL_ascii);
											codedef_register_switcher_no_write(INCREMENTAL_CPU_RDI, Inset_register, TS_CPU_REG_RDI_ascii, TS_CPU_REG_RDI_ascii, TS_CPU_REG_DI_ascii, TS_CPU_REG_DIL_ascii);

											//NUMERIC
											codedef_register_switcher_no_write(INCREMENTAL_CPU_R8, Inset_register, TS_CPU_REG_R8_ascii, TS_CPU_REG_R8d_ascii, TS_CPU_REG_R8w_ascii, TS_CPU_REG_R8b_ascii);
											codedef_register_switcher_no_write(INCREMENTAL_CPU_R9, Inset_register, TS_CPU_REG_R9_ascii, TS_CPU_REG_R9d_ascii, TS_CPU_REG_R9w_ascii, TS_CPU_REG_R9b_ascii);
											codedef_register_switcher_no_write(INCREMENTAL_CPU_R10, Inset_register, TS_CPU_REG_R10_ascii, TS_CPU_REG_R10d_ascii, TS_CPU_REG_R10w_ascii, TS_CPU_REG_R10b_ascii);
											codedef_register_switcher_no_write(INCREMENTAL_CPU_R11, Inset_register, TS_CPU_REG_R11_ascii, TS_CPU_REG_R11d_ascii, TS_CPU_REG_R11w_ascii, TS_CPU_REG_R11b_ascii);
											codedef_register_switcher_no_write(INCREMENTAL_CPU_R12, Inset_register, TS_CPU_REG_R12_ascii, TS_CPU_REG_R12d_ascii, TS_CPU_REG_R12w_ascii, TS_CPU_REG_R12b_ascii);
											codedef_register_switcher_no_write(INCREMENTAL_CPU_R13, Inset_register, TS_CPU_REG_R13_ascii, TS_CPU_REG_R13d_ascii, TS_CPU_REG_R13w_ascii, TS_CPU_REG_R13b_ascii);
											codedef_register_switcher_no_write(INCREMENTAL_CPU_R14, Inset_register, TS_CPU_REG_R14_ascii, TS_CPU_REG_R14d_ascii, TS_CPU_REG_R14w_ascii, TS_CPU_REG_R14b_ascii);
											codedef_register_switcher_no_write(INCREMENTAL_CPU_R15, Inset_register, TS_CPU_REG_R15_ascii, TS_CPU_REG_R15d_ascii, TS_CPU_REG_R15w_ascii, TS_CPU_REG_R15b_ascii);
										//}//end Increment_id switcher
										
							//	}while(--i > 0);
							break;} //end case INREMENTAL_EXECUTE
				case INREMENTAL_LOCK: {
									
											switch(pCastResultUnitTypeStack->INCREMENT_ID)
											{
												//COMMON 8
												codedef_register_switcher_no_write(INCREMENTAL_CPU_RAX, Inset_register, TS_CPU_REG_RAX_ascii, TS_CPU_REG_EAX_ascii, TS_CPU_REG_AX_ascii, TS_CPU_REG_AL_ascii);
												codedef_register_switcher_no_write(INCREMENTAL_CPU_RBX, Inset_register, TS_CPU_REG_RBX_ascii, TS_CPU_REG_EBX_ascii, TS_CPU_REG_BX_ascii, TS_CPU_REG_BL_ascii);
												codedef_register_switcher_no_write(INCREMENTAL_CPU_RCX, Inset_register, TS_CPU_REG_RCX_ascii, TS_CPU_REG_ECX_ascii, TS_CPU_REG_CX_ascii, TS_CPU_REG_CL_ascii);
												codedef_register_switcher_no_write(INCREMENTAL_CPU_RDX, Inset_register, TS_CPU_REG_RDX_ascii, TS_CPU_REG_EDX_ascii, TS_CPU_REG_DX_ascii, TS_CPU_REG_DL_ascii);
												codedef_register_switcher_no_write(INCREMENTAL_CPU_RBP, Inset_register, TS_CPU_REG_RBP_ascii, TS_CPU_REG_EBP_ascii, TS_CPU_REG_BP_ascii, TS_CPU_REG_BPL_ascii);
												codedef_register_switcher_no_write(INCREMENTAL_CPU_RSP, Inset_register, TS_CPU_REG_RSP_ascii, TS_CPU_REG_ESP_ascii, TS_CPU_REG_SP_ascii, TS_CPU_REG_SPL_ascii);
												codedef_register_switcher_no_write(INCREMENTAL_CPU_RSI, Inset_register, TS_CPU_REG_RSI_ascii, TS_CPU_REG_RSI_ascii, TS_CPU_REG_SI_ascii, TS_CPU_REG_SIL_ascii);
												codedef_register_switcher_no_write(INCREMENTAL_CPU_RDI, Inset_register, TS_CPU_REG_RDI_ascii, TS_CPU_REG_RDI_ascii, TS_CPU_REG_DI_ascii, TS_CPU_REG_DIL_ascii);

												//NUMERIC
												codedef_register_switcher_no_write(INCREMENTAL_CPU_R8, Inset_register, TS_CPU_REG_R8_ascii, TS_CPU_REG_R8d_ascii, TS_CPU_REG_R8w_ascii, TS_CPU_REG_R8b_ascii);
												codedef_register_switcher_no_write(INCREMENTAL_CPU_R9, Inset_register, TS_CPU_REG_R9_ascii, TS_CPU_REG_R9d_ascii, TS_CPU_REG_R9w_ascii, TS_CPU_REG_R9b_ascii);
												codedef_register_switcher_no_write(INCREMENTAL_CPU_R10, Inset_register, TS_CPU_REG_R10_ascii, TS_CPU_REG_R10d_ascii, TS_CPU_REG_R10w_ascii, TS_CPU_REG_R10b_ascii);
												codedef_register_switcher_no_write(INCREMENTAL_CPU_R11, Inset_register, TS_CPU_REG_R11_ascii, TS_CPU_REG_R11d_ascii, TS_CPU_REG_R11w_ascii, TS_CPU_REG_R11b_ascii);
												codedef_register_switcher_no_write(INCREMENTAL_CPU_R12, Inset_register, TS_CPU_REG_R12_ascii, TS_CPU_REG_R12d_ascii, TS_CPU_REG_R12w_ascii, TS_CPU_REG_R12b_ascii);
												codedef_register_switcher_no_write(INCREMENTAL_CPU_R13, Inset_register, TS_CPU_REG_R13_ascii, TS_CPU_REG_R13d_ascii, TS_CPU_REG_R13w_ascii, TS_CPU_REG_R13b_ascii);
												codedef_register_switcher_no_write(INCREMENTAL_CPU_R14, Inset_register, TS_CPU_REG_R14_ascii, TS_CPU_REG_R14d_ascii, TS_CPU_REG_R14w_ascii, TS_CPU_REG_R14b_ascii);
												codedef_register_switcher_no_write(INCREMENTAL_CPU_R15, Inset_register, TS_CPU_REG_R15_ascii, TS_CPU_REG_R15d_ascii, TS_CPU_REG_R15w_ascii, TS_CPU_REG_R15b_ascii);
											}//end Increment_id switcher
												if(Inset_register != NULL)
												{
													quick_UNICODE_to_ASCII_with_insert(pAsmInstruction, pXED_parse_asm->instr, fpAsmInstruction, Inset_register);
													if (XEDParseAssemble(pXED_parse_asm) == XEDPARSE_ERROR) //XED parse +++++++++++++++++++++++++
																return false;

															if(!memcmp(pInstruction_pool, &pXED_parse_asm->dest[0], pXED_parse_asm->dest_size)) // retail compare!!!!!!!!!!!!!!!!!!!!!!
																return true;
												}//end if(!Inset_register)
					
					break;}
				}//end switcher pCastResultUnitTypeStack->INCREMENT_STANDBY_TYPE 
			break;}
	}//end CS type switcher
return false;
}