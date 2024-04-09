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
#include "ENGINE_Import_operations.h"


// ******************************************************************************************************************
// ENGINE_Import_operations.cpp - любые операции с таблицами импорта
// ******************************************************************************************************************

ULONG DH_PickDumpFile_import(__in CString Dump_file_path, __out pIMPORT_INFO_STACK Out_import_stack)
{
	if (::GetFileAttributesW(CT2CW(Dump_file_path)) == (DWORD)INVALID_HANDLE_VALUE)
		return NULL;

	 HANDLE hFile = CreateFile(&HUNT_HIVE.PROFILE_PATH[0],                // name of the write
                       GENERIC_READ,          // 
                       0,                      // do not share
                       NULL,                   // default security
                       OPEN_EXISTING,             // 
                       FILE_ATTRIBUTE_NORMAL,  // normal file
                       NULL);
   if (hFile == INVALID_HANDLE_VALUE) 
     goto error_PickDumpFile_import;

   
   ULONG sizlimits = 0x100000;
  ULONG num_bytes_of_read =  ::GetFileSize(hFile,&sizlimits);

   

  PVOID operaions_allocate = malloc(num_bytes_of_read); //allocate mem для операционного буфера
  ULONG Return_Readed_Bytes = 0;

   bool bErrorFlag  = ReadFile(hFile,operaions_allocate,num_bytes_of_read,&Return_Readed_Bytes,NULL);
   ::CloseHandle(hFile);
      if (bErrorFlag == false)
         goto error_PickDumpFile_import;

	  ULONG64 Bound_file_region_addres = (ULONG64)operaions_allocate + num_bytes_of_read;
	  SERVICE_COUNT Import_item_count;
//	  SERVICE_COUNT Import_item_count = Calculate_ALL_import_items(operaions_allocate,Bound_file_region_addres);

	  PVOID allocate_import_record_part = malloc(Import_item_count*sizeof(DH_IMPORT_RECORD)); //allocate mem для основной части под запись главных данных
	  PVOID allocate_import_wchar_text_part = malloc(num_bytes_of_read*sizeof(WCHAR)); //allocate mem для вторичной части - запись текстовых строк
	  
	  SERVICE_FLOW_POINTER Flow_pointer_IMPORT_DUMP_FILE = operaions_allocate; 
	  pDH_IMPORT_RECORD Flow_pointer_IMPORT_RECORD = (pDH_IMPORT_RECORD)allocate_import_record_part; //плавающий указатель для региона под запись основных данных
	  SERVICE_FLOW_POINTER Flow_pointer_WCHAR_RECORD = (pDH_IMPORT_RECORD)allocate_import_wchar_text_part; //плавающий указатель для региона под запись текстовых данных

	  ULONG a = Import_item_count;
	  
	  SERVICE_FLOW_POINTER Chk_Flow_Pointer; 
	  do
	  {
//		  Chk_Flow_Pointer = Calculate_NEXT_import_item(Flow_pointer_IMPORT_DUMP_FILE, Bound_file_region_addres);
		  if(Chk_Flow_Pointer == NULL)
			  break;


	  }while(a--);


error_PickDumpFile_import:
	  free(operaions_allocate);
	  return NULL;
}

ULONG Calculate_ALL_import_items(PVOID Start_region, ULONG64 End_of_region)
{

	return 0;
}

PVOID Calculate_NEXT_import_item(__in SERVICE_FLOW_POINTER Flow_pointer, __in  ULONG64 End_of_region, __out PVOID API_cell, __out PVOID API_last_addr, __out PVOID API_name)
{
	return 0;
}