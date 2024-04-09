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

//IJ_P DH_Inject_SPIRUN7;

 BYTE ASM_MOV_Register_RCX[2] = {0x48, 0xB9};
 BYTE ASM_CALL_CONST = 0xE8;
 BYTE ASM_NOP = 0x90;
 BYTE ASM_CALL_QWORD_PTR_DS[3] = {0xFF, 0x14, 0x25};
 BYTE ASM_RETN = 0xC3;
 BYTE ASM_ADD_RSP_30[] = {0x48, 0x83, 0xC4, 0x30};

 BYTE Inject_EXECUTE_CODE[] = {0x90, //nop
								0x48, 0xB9, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, //mov rcx, 111111111111111
								0x48, 0xB8, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, //mov rax, 2222222222222222
								0x48, 0xC7, 0xC3, 0x60, 0x00, 0x00, 0x00,//mov rbx, 60 - SIZEOF struct
								0x48, 0x29, 0xDC, //sub rsp, rbx
								0xFF, 0xD0, //call rax
								0x48, 0x01, 0xDC, //add rsp, rbx
								0xC3 //ret
 };

bool DH_SPIRUN_Injectx64(__in HANDLE Target_Process)
{
	UINT64 length_Pointer;
if(!(Target_Process) && (Target_Process != HUNT_HIVE.tOpenProcHANDLE)){return false;}

HUNT_HIVE.Injection_info.pLibRemote = VirtualAllocEx(Target_Process, NULL,0x10000,
                                MEM_COMMIT|MEM_COMMIT, PAGE_EXECUTE_READWRITE);
unsigned long err_code = GetLastError();
if(!HUNT_HIVE.Injection_info.pLibRemote){return false;}
HUNT_HIVE.Injection_info.x_DLL_Path_Alloc_Adress = HUNT_HIVE.Injection_info.pLibRemote;

			//******* PATH *********
SIZE_T WritenBytes=NULL;
length_Pointer = GetModuleFileName(GetModuleHandle(NULL),(LPWSTR)&Byte_buff[0],MAX_PATH);
length_Pointer = wcsleave_path__cut_file_name((TCHAR*)&Byte_buff[0]);
wcscat((LPWSTR)&Byte_buff[0],SPIRUN7_LIB_NAME); length_Pointer+=sizeof(SPIRUN7_LIB_NAME);

//***** Thread start *******
	//path + AND (arrears)
	*(UINT64*)&HUNT_HIVE.Injection_info.x_Start_Thread_Addr = (((UINT64)HUNT_HIVE.Injection_info.pLibRemote + length_Pointer + SAFE_CODE_OFFSET))&-16; //give THREAD START ADDRESS
length_Pointer=(UINT64)HUNT_HIVE.Injection_info.x_Start_Thread_Addr - (UINT64)HUNT_HIVE.Injection_info.pLibRemote;

if (!WriteProcessMemory(Target_Process,(LPVOID)HUNT_HIVE.Injection_info.pLibRemote,&Byte_buff[0],length_Pointer,&WritenBytes)) {return false;}

	//******* Get KERNEL32.LoadLibrary *********
HMODULE VA_kernel32 = GetModuleHandle(_T("KERNEL32.DLL"));
FARPROC RVA_LoadLibraryW = GetProcAddress(VA_kernel32,"LoadLibraryW");
*(UINT64*)&HUNT_HIVE.Injection_info.Operand_Load_LibraryW_RVA = (UINT64)HUNT_HIVE.Process_HIVE.Kernel32 + ((UINT64)RVA_LoadLibraryW - (UINT64)VA_kernel32);
//*(UINT64*)&DH_Inject_SPIRUN7.A_Address_of_QWORD_LoadLibraryW_Operand = (UINT64)HUNT_HIVE.pLibRemote+(UINT64)length_Pointer;

		


	//**************SET REGISTERS OPERANDS**********
memcpy(&Inject_EXECUTE_CODE[RCX_OPERAND_OFFSET], &HUNT_HIVE.Injection_info.x_DLL_Path_Alloc_Adress,sizeof(HUNT_HIVE.Injection_info.x_DLL_Path_Alloc_Adress)); //as Path TCHAR start -pointer
memcpy(&Inject_EXECUTE_CODE[RAX_OPERAND_OFFSET], &HUNT_HIVE.Injection_info.Operand_Load_LibraryW_RVA,sizeof(HUNT_HIVE.Injection_info.Operand_Load_LibraryW_RVA)); //as Path TCHAR start -pointer

if (!WriteProcessMemory(Target_Process,(LPVOID)((UINT64)HUNT_HIVE.Injection_info.pLibRemote+length_Pointer),&Inject_EXECUTE_CODE[0],sizeof(Inject_EXECUTE_CODE),&WritenBytes)) {return false;}

HUNT_HIVE.Injection_info.A_Create_Thread_handle = CreateRemoteThreadEx(Target_Process,NULL,NULL, (LPTHREAD_START_ROUTINE)HUNT_HIVE.Injection_info.x_Start_Thread_Addr,NULL,NULL,NULL, &HUNT_HIVE.Injection_info.A_Create_Thread_ID);
	/*
	hThr = CreateRemoteThreadEx(hProcess, NULL, 0,
                                (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("user32.dll"), "MessageBoxA"),
                                0, NULL,
                                "%x%s%8d%x",
                                NULL, "Hello world", "Caption", MB_ICONEXCLAMATION | MB_YESNOCANCEL);
								*/
if(!HUNT_HIVE.Injection_info.A_Create_Thread_handle) {return false;}
//::VirtualFreeEx(Target_Process,(LPVOID)HUNT_HIVE.Injection_info.pLibRemote,0x10000, MEM_RELEASE);
	return true;
}

bool DH_SPIRUN_Close_Injectx64(__in HANDLE Target_Process)
{
	
	return false;
}

bool DH_SPIRUN_Init(__in __out pSPIRUN7_BLOCK pSPIRUN7_dll_info)
{

	if (!HUNT_HIVE.SPIRUN7_Info.SPIRUN7_HMODULE) //init critical info in SPIRUN7 and DH struct
	{
		HUNT_HIVE.SPIRUN7_Info.SPIRUN7_HMODULE  =	(SERVICE_ADDRES)DH_GetModuleHANDLE(SPIRUN7_LIB_NAME); 
			if (!HUNT_HIVE.SPIRUN7_Info.SPIRUN7_HMODULE)
			{return false;}
			
			//get loaded SPIRUN7.dll path
			TCHAR* modpath_buff = (TCHAR*)malloc(MAX_PATH*sizeof(TCHAR));
		

				if (::GetModuleFileNameExW(HUNT_HIVE.tOpenProcHANDLE,(HMODULE)HUNT_HIVE.SPIRUN7_Info.SPIRUN7_HMODULE,&modpath_buff[0],MAX_PATH) == NULL)
					{free(modpath_buff); return false;}
						
			//getfilesize control
			HANDLE hFile = ::CreateFile(&modpath_buff[0],GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
				if(hFile == INVALID_HANDLE_VALUE )
					{free(modpath_buff); return false;}

				LARGE_INTEGER fsize;
				memset(&fsize,0, sizeof(LARGE_INTEGER));
				::GetFileSizeEx(hFile, &fsize);
				CloseHandle(hFile);

				if((fsize.HighPart != NULL) && (fsize.LowPart > 0x1400))
					{free(modpath_buff); return false;}
			//load local pool
			register HMODULE hSPIRUN7_target = ::LoadLibraryExW(&modpath_buff[0],NULL,DONT_RESOLVE_DLL_REFERENCES);
				if (hSPIRUN7_target == NULL)
					{free(modpath_buff); return false;}
			free(modpath_buff);

			//Get local proc addresses
			FARPROC fp_mySPIRUN7_Intrecept_main = 	::GetProcAddress(hSPIRUN7_target, SPIRUN7_FUNC_intrecept_main);
			FARPROC fp_mySPIRUN7_Intrecept_supply15_table_cell = ::GetProcAddress(hSPIRUN7_target, SPIRUN7_DATA_VMP2_INTRECEPT_TABLE);
			FARPROC fp_mySPIRUN7_Original_supply15_table_cell =	::GetProcAddress(hSPIRUN7_target, SPIRUN7_DATA_VMP2_ORIGINAL_TABLE);
			FARPROC fp_mySPIRUN7_HANHELD_ENTER_TRACE_LOG_addres_cell =	::GetProcAddress(hSPIRUN7_target, SPIRUN7_DATA_VMP2_HANHELD_ENTER_TRACE_LOG_Addres);
			FARPROC fp_mySPIRUN7_HANHELD_ENTER_TRACE_LOG_flow_pointer_cell =	::GetProcAddress(hSPIRUN7_target, SPIRUN7_DATA_VMP2_HANHELD_ENTER_TRACE_LOG_flow_pointer);

			if((fp_mySPIRUN7_Intrecept_main == NULL) && (fp_mySPIRUN7_Intrecept_supply15_table_cell == NULL) && (fp_mySPIRUN7_Original_supply15_table_cell == NULL) && (fp_mySPIRUN7_HANHELD_ENTER_TRACE_LOG_addres_cell == NULL) && (fp_mySPIRUN7_HANHELD_ENTER_TRACE_LOG_flow_pointer_cell == NULL))
				{FreeLibrary(hSPIRUN7_target); return false;}

			//calculating remote real addreses
				HUNT_HIVE.SPIRUN7_Info.SPIRUN7_main_vmp2_init_intrecept_in_targetproc = HUNT_HIVE.SPIRUN7_Info.SPIRUN7_HMODULE + ((ULONG64)fp_mySPIRUN7_Intrecept_main - (ULONG64)hSPIRUN7_target);

				HUNT_HIVE.SPIRUN7_Info.remote_vmp2_intrecept_table =  HUNT_HIVE.SPIRUN7_Info.SPIRUN7_HMODULE + ((ULONG64)fp_mySPIRUN7_Intrecept_supply15_table_cell - (ULONG64)hSPIRUN7_target);
				HUNT_HIVE.SPIRUN7_Info.remote_vmp2_original_table =  HUNT_HIVE.SPIRUN7_Info.SPIRUN7_HMODULE + ((ULONG64)fp_mySPIRUN7_Original_supply15_table_cell - (ULONG64)hSPIRUN7_target);

				HUNT_HIVE.SPIRUN7_Info.remote_HANHELD_ID_enter_trace_log_addres =  HUNT_HIVE.SPIRUN7_Info.SPIRUN7_HMODULE + ((ULONG64)fp_mySPIRUN7_HANHELD_ENTER_TRACE_LOG_addres_cell - (ULONG64)hSPIRUN7_target);
				HUNT_HIVE.SPIRUN7_Info.remote_HANHELD_ID_fp_addres =  HUNT_HIVE.SPIRUN7_Info.SPIRUN7_HMODULE + ((ULONG64)fp_mySPIRUN7_HANHELD_ENTER_TRACE_LOG_flow_pointer_cell - (ULONG64)hSPIRUN7_target);
		
		FreeLibrary(hSPIRUN7_target);
		
	}

	//remote allocate regions on log dumping
	if(!HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region.region_start_addres) 
	{
		HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region.type_of_region = REGION_TYPE_SPIRUN7_DUMP_AREA;
		HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region.region_size = SPURUN7_DUMP_AREA_SIZE;
		HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region.allocated_pool = NULL;
		HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region.region_start_addres = (SERVICE_ADDRES)::VirtualAllocEx(HUNT_HIVE.tOpenProcHANDLE,NULL,HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region.region_size, MEM_COMMIT|MEM_COMMIT,PAGE_READWRITE);
		HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region.allocated_pool = (LPBYTE)::VirtualAllocEx(DH_Current_process_fast_handle,NULL,HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region.region_size, MEM_COMMIT|MEM_COMMIT,PAGE_READWRITE);
	}


	//set remote_HANHELD_ID_enter_trace_log_addres & remote_HANHELD_ID_fp_addres
		if(HUNT_HIVE.SPIRUN7_Info.Restore_flag == false)
		{
			::DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,(LPVOID)HUNT_HIVE.SPIRUN7_Info.remote_HANHELD_ID_enter_trace_log_addres, sizeof(SERVICE_ADDRES), &HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region.region_start_addres);
			DH_SPIRUN_Set_initial_HANHELD_ID_dumping_region(); //::DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,(LPVOID)HUNT_HIVE.SPIRUN7_Info.remote_HANHELD_ID_fp_addres, sizeof(SERVICE_ADDRES), &HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region.region_start_addres);
		}

	memcpy(pSPIRUN7_dll_info,&HUNT_HIVE.SPIRUN7_Info,sizeof(SPIRUN7_BLOCK));
return true;
}


pVMPX_VM_PROFILE_INFO_STACK DH_SPIRUN_Init_assign_vm_profile(__in TCHAR* assign_vm_name)
{
	ULONG64 reta = NULL;
	
	SERVICE_ADDRES test_addr;

  pVMPX_VM_PROFILE_INFO_STACK Selected_Stack = (pVMPX_VM_PROFILE_INFO_STACK)DH_Read_Current_hunt_profile_CONTAINER(PROFILE_TYPE_VM, assign_vm_name,PROFILE_READ_ALL,(PULONG)&reta);

  if(Selected_Stack == NULL)
	  return NULL;

  //услови€ корректной работы
  if((Selected_Stack->VMPX_version != VM_TYPE_VMP2) && (Selected_Stack->supply_15_start_address != NULL) && (Selected_Stack->handles_count <= LIMIT_HANDLES_ON_SUPPLY))
	{free(Selected_Stack); return NULL;}

  if (!DH_ReadProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Selected_Stack->supply_15_start_address,sizeof(CELL_ADDRES),&test_addr))
		{free(Selected_Stack); return NULL;}

  DH_DEcode_current_supply15_addres(Selected_Stack->table_DEcode_instruction,Selected_Stack->table_DEcode_instruction_operand,&test_addr);

  if (!DH_ReadProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,(LPVOID)test_addr,sizeof(CELL_ADDRES),&reta))
		{free(Selected_Stack); return NULL;}

//получаем сформированную таблицу перехватчиков SPIRUN7  
DH_ReadProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,(LPVOID)HUNT_HIVE.SPIRUN7_Info.remote_vmp2_intrecept_table,sizeof(CELL_ADDRES)*VM_HANDLES_MAX_COUNT,&Selected_Stack->spirun7_hanhelds_table_decrypted[0]);

//копируем в crypt-блок
memcpy(&Selected_Stack->spirun7_hanhelds_table_crypted[0], &Selected_Stack->spirun7_hanhelds_table_decrypted[0], sizeof(Selected_Stack->spirun7_hanhelds_table_crypted));

//проводим криптооперации дл€ таблицы перехватчиков (готовим к подмене) в скопированном блоке
DH_Crypt_supply15_table(Selected_Stack->table_DEcode_instruction,Selected_Stack->table_DEcode_instruction_operand, &Selected_Stack->spirun7_hanhelds_table_crypted[0], VM_HANDLES_MAX_COUNT);

//заполн€ем декодированную таблицу в массиве SPURIN7.XA_VMP2_ORIGINAL_TABLE
DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,(LPVOID)HUNT_HIVE.SPIRUN7_Info.remote_vmp2_original_table,sizeof(Selected_Stack->spirun7_hanhelds_table_crypted), &Selected_Stack->original_hanhelds_table_decrypted[0]);

 // free(Selected_Stack);
return Selected_Stack;
}


bool DH_SPIRUN_Intrecept_Commander_Tower(__in SERVICE_TYPE Intrecept_object, __in SERVICE_TYPE Intrecept_command, __in pVMPX_VM_PROFILE_INFO_STACK Selected_vmp_stack)
{
	if(Selected_vmp_stack == NULL)
		return false;
	
	bool return_state = NULL;

	//OBJECT SWTICHER
	switch(Intrecept_object)
	{
	case IOBG_VMP2_SUPPLY15_TRACE_LOGGER:
		{
				switch(Intrecept_command)
				{
				case ICMD_VMP2_APLLY_INTRECEPT_SUPPLY15: {return_state = DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Selected_vmp_stack->supply_15_start_address, LIMIT_HANDLES_ON_SUPPLY*sizeof(HANHELD_ADDRES), &Selected_vmp_stack->spirun7_hanhelds_table_crypted[0]); break;}
				case ICMD_VMP2_RESTORE_ORIGINAL_SUPPLY15: {return_state = DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,Selected_vmp_stack->supply_15_start_address, LIMIT_HANDLES_ON_SUPPLY*sizeof(HANHELD_ADDRES), &Selected_vmp_stack->original_hanhelds_table_crypted[0]); break;}
				}//end Intrecept command switch
		break;} //end case IOBG_VMP2_SUPPLY15_TRACE_LOGGER

	case IOBG_VMP3_HANHELD_COLLECTOR:
	case IOBG_BETWEEN_VMs_PROBE:
	}//end OBJECT SWTICHER

	return return_state;
}

bool DH_SPIRUN_Sentry_for_HANHELD_ID_dumping_region(__in pVMPX_VM_PROFILE_INFO_STACK Selected_vmp_stack, __in __out pSPIERUN7_INFO_CLUSTER_TRACE_LIST TraceListCluster, __in __out pSPIERUN7_INFO_CLUSTER_POPULAR_LIST PopularListCluster, __in __out TARGET_PROC_REGION_COPY* HANHELD_ID_dumping_region)
{
	SERVICE_FLOW_POINTER Old_fp = HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region.flow_pointer_to_allocated_pool;
	bool err_control = DH_ReadProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,(LPVOID)HUNT_HIVE.SPIRUN7_Info.remote_HANHELD_ID_fp_addres,sizeof(SERVICE_FLOW_POINTER), &HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region.flow_pointer_to_allocated_pool);
	if (!err_control)
		return false;

	if(HUNT_HIVE.SPIRUN7_Info.Restore_flag == false)
	{
		if ((Old_fp != NULL) && (Old_fp == HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region.flow_pointer_to_allocated_pool))
			return false; //обновление не требуетс€!
	}
	else //restore mode!
		{HUNT_HIVE.SPIRUN7_Info.Restore_flag = false; DH_SPIRUN_Set_initial_HANHELD_ID_dumping_region;} //restore mode fired. Only one iteration enable!

	//pSPIERUN7_INFO_CLUSTER_POPULAR_LIST PopularListCluster_temp = (pSPIERUN7_INFO_CLUSTER_POPULAR_LIST)malloc(sizeof(SPIERUN7_INFO_CLUSTER_POPULAR_LIST));
//	memset(PopularListCluster_temp,NULL, sizeof(SPIERUN7_INFO_CLUSTER_POPULAR_LIST));

	if (((SERVICE_ADDRES)HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region.flow_pointer_to_allocated_pool < HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region.region_start_addres) && ((SERVICE_ADDRES)HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region.flow_pointer_to_allocated_pool > (HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region.region_start_addres + HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region.region_size))) //”—Ћќ¬»≈ попадани€ float pointer в границы региона
		return false;

	if(HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region.allocated_pool == NULL)
		return false;

	if (Selected_vmp_stack->handles_count == 0)
		return false;
	unsigned long handles_count;
	if (Selected_vmp_stack->handles_count > VM_HANDLES_MAX_COUNT)
		handles_count = VM_HANDLES_MAX_COUNT;
	else
		handles_count = Selected_vmp_stack->handles_count;
	//чтение дампа трейс лога
	size_t ReadDataSize = (SERVICE_ADDRES)HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region.flow_pointer_to_allocated_pool - HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region.region_start_addres;
	if (ReadDataSize > HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region.region_size)
			ReadDataSize = HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region.region_size;
	 err_control =	DH_ReadProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,(LPVOID)HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region.region_start_addres, ReadDataSize, HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region.allocated_pool);
		if (!err_control)
			return false;

	//обнуление кластеров
		memset(TraceListCluster, NULL, sizeof(SPIERUN7_INFO_CLUSTER_TRACE_LIST));
		memset(PopularListCluster, NULL, sizeof(SPIERUN7_INFO_CLUSTER_POPULAR_LIST));

		//вычисление кол-ва элементов в дампе
	register SERVICE_COUNT i_dump = ReadDataSize / SPIRUN7_HANHELD_ID_dump_sizeof;
	register SERVICE_COUNT i_support; //дл€ перечислени€ элементов во вложенных циклах

	register SPIRUN7_HANHELD_ID_dump_LP_pointer fpHANHELD_ID_dump_cycle = (SPIRUN7_HANHELD_ID_dump_LP_pointer)HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region.allocated_pool;
	register SPIRUN7_HANHELD_ID_dump_pointer_type HANHELD_ID; //извлекаемый идентефикатор hanheld'а (равен пор€дковому номеру в массиве)
	do
	{
		HANHELD_ID = *fpHANHELD_ID_dump_cycle++;
		if (HANHELD_ID > VM_HANDLES_SPIRUN7_LIMIT) 
		continue;

		pHANHELD fpHANHELD = (pHANHELD)((SERVICE_ADDRES)Selected_vmp_stack + Selected_vmp_stack->ofs_1Containt_handler);
		if(i_dump < SPURUN7_LIMIT_GUI_LIST_SHOW_LATEST_HANHELDS) //остал*ось менее или равно SPURUN7_LIMIT_GUI_LIST_SHOW_LATEST_HANHELDS итераций дл€ записи последних вызовов
			{TraceListCluster->pLast__Called_HANHELD[i_dump] = &fpHANHELD[HANHELD_ID]; TraceListCluster->Last__Fill_HANHELDs_count++;}
		
		//скопировать ссылку на блок HANHELD (попул€рность) - исключа€ дублирующейс€ адреса хендлов в €чейках Supply15
		if((PopularListCluster->pPopular_HANHELD[HANHELD_ID] == NULL) && (fpHANHELD[HANHELD_ID].is_duplicate_hanheld_in_supply15 == false))
			PopularListCluster->pPopular_HANHELD[HANHELD_ID] = &fpHANHELD[HANHELD_ID];

		//калькул€ци€ счетчика попул€рности с учетом дублирующихс€ адресов хендлов в €чейках Supply15
		if (fpHANHELD[HANHELD_ID].is_duplicate_hanheld_in_supply15 == false)
			PopularListCluster->pPopular_HANHELD_calls_count[HANHELD_ID]++; //инкремент пр€мой €чейки
		else
		{
			i_support = handles_count; //DH_Mark_out_Duplicate_HANHELDs_in_Supply15 помечает дубликаты в верхнем диапазоне
			HANHELD_ADDRES double_hanheld = fpHANHELD[HANHELD_ID].HANDELD_Primary_Addres;
			register pHANHELD pScanHANHELD = &fpHANHELD[handles_count];
			unsigned long a = 0;
			do
			{
				if((double_hanheld == pScanHANHELD->HANDELD_Primary_Addres) && (pScanHANHELD->is_duplicate_hanheld_in_supply15 == false))
						{PopularListCluster->pPopular_HANHELD_calls_count[i_support]++; break;} //перенос инкремента с дублирующейс€ €чейки в основную

					pScanHANHELD--;
					a++;
			}while(i_support--);
		}//end else if

		//калькул€ци€ report счетчиков (PCODE, RBP, total calls)
		TraceListCluster->Total_PCODE_shift += fpHANHELD[HANHELD_ID].PCODE_Shift;
		TraceListCluster->Current_RBP_different += fpHANHELD[HANHELD_ID].stack_RBP_Shift;
		TraceListCluster->Total_HANHELD_calls_count++;
	
	}while(i_dump--);

	//окончательное построение списка попул€рности учитыва€ результаты pPopular_HANHELD_calls_count

	i_dump = VM_HANDLES_MAX_COUNT; //дл€ PopularListCluster_temp
		/*
	pHANHELD*		pPLCluster_HANHELD_temp = &PopularListCluster_temp->pPopular_HANHELD[0];
	SERVICE_COUNT*	pPLCluster_HANHELD_calls_count_temp = &PopularListCluster_temp->pPopular_HANHELD_calls_count[0];
	register SERVICE_COUNT Current_MIN_cont = 1;
	SERVICE_COUNT Next_planned_MIN_cont = 2;

	pPLCluster_HANHELD_temp--;
	pPLCluster_HANHELD_calls_count_temp--;
	do
	{
		pPLCluster_HANHELD_temp++;
		pPLCluster_HANHELD_calls_count_temp++;
		if (*pPLCluster_HANHELD_temp == NULL)
			continue;
		
		if (Current_MIN_cont)
		
	}while(i_dump--);
	*/

//	memcpy(PopularListCluster PopularListCluster_temp//
//	free(PopularListCluster_temp);

	//обновление данных региона
		memcpy(HANHELD_ID_dumping_region, &HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region, sizeof(TARGET_PROC_REGION_COPY));

return true;
}

bool DH_SPIRUN_Set_restore_mode(__in bool Restore_flag_state, __in TCHAR* Save_sel_VM_name)
{
	if (Save_sel_VM_name != NULL)
		memcpy(&HUNT_HIVE.SPIRUN7_Info.Assign_vmp2_name[0],Save_sel_VM_name,sizeof(HUNT_HIVE.SPIRUN7_Info.Assign_vmp2_name));
	else
		memset(&HUNT_HIVE.SPIRUN7_Info.Assign_vmp2_name[0],0, sizeof(HUNT_HIVE.SPIRUN7_Info.Assign_vmp2_name));

	return (HUNT_HIVE.SPIRUN7_Info.Restore_flag = Restore_flag_state);
}

bool DH_SPIRUN_Get_restore_mode(__in TCHAR* Save_sel_VM_name)
{
	memcpy(Save_sel_VM_name,&HUNT_HIVE.SPIRUN7_Info.Assign_vmp2_name[0],sizeof(HUNT_HIVE.SPIRUN7_Info.Assign_vmp2_name));
	return HUNT_HIVE.SPIRUN7_Info.Restore_flag;
}

void DH_SPIRUN_Set_initial_HANHELD_ID_dumping_region()
{
	::DH_WriteProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,(LPVOID)HUNT_HIVE.SPIRUN7_Info.remote_HANHELD_ID_fp_addres, sizeof(SERVICE_ADDRES), &HUNT_HIVE.SPIRUN7_Info.HANHELD_ID_dumping_region.region_start_addres);
}

//spirun7 profiler ///////////////////////////////////////////////////////// <<<<<<<<<<<<<<<<<<<<<<<<<<<<
pSPIRUN7_TRACELOG_VMP2_INFO_STACK DH_PROFILE_SPIRUN7_Pack_save_profile_vmp2(__in pSPIERUN7_INFO_CLUSTER_TRACE_LIST pCluster_tracelist_tracep, __in pSPIERUN7_INFO_CLUSTER_POPULAR_LIST pCluster_popular_list_tracep, __in TARGET_PROC_REGION_COPY* pHANHELD_ID_dumping_region, __in WCHAR* vmname, __in __out SIZE_T* ByteOuts)
{
	if ((SERVICE_ADDRES)pHANHELD_ID_dumping_region->flow_pointer_to_allocated_pool < (SERVICE_ADDRES)pHANHELD_ID_dumping_region->region_start_addres)
		return NULL;

	ULONG wcslen_vnname = wcslen(vmname);

	if(wcslen_vnname >PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN || wcslen_vnname == NULL)
		return NULL;

	register ULONG64 precision_dump_area_size = (SERVICE_ADDRES)pHANHELD_ID_dumping_region->flow_pointer_to_allocated_pool - (SERVICE_ADDRES)pHANHELD_ID_dumping_region->region_start_addres;

	size_t sizealloc = sizeof(SPIRUN7_TRACELOG_VMP2_INFO_STACK) + precision_dump_area_size;
	register pSPIRUN7_TRACELOG_VMP2_INFO_STACK pResult_pack_profiler = (pSPIRUN7_TRACELOG_VMP2_INFO_STACK)malloc(sizealloc);

	if(pResult_pack_profiler  == NULL)
		return NULL;



	memcpy(&pResult_pack_profiler->TraceList_vmp2, pCluster_tracelist_tracep,sizeof(SPIERUN7_INFO_CLUSTER_TRACE_LIST));
	memcpy(&pResult_pack_profiler->TracePopular_vmp2, pCluster_popular_list_tracep,sizeof(SPIERUN7_INFO_CLUSTER_POPULAR_LIST));

	//LPBYTE	pResult_pack_profiler = reinterpret_cast<LPBYTE>(pResult_pack_profiler);
	memcpy((void*)((SERVICE_ADDRES)pResult_pack_profiler + sizeof(SPIRUN7_TRACELOG_VMP2_INFO_STACK)),pHANHELD_ID_dumping_region->allocated_pool, precision_dump_area_size);

//	pSPIRUN7_TRACELOG_VMP2_INFO_STACK	pResult_pack_profiler = reinterpret_cast<pSPIRUN7_TRACELOG_VMP2_INFO_STACK>(pResult_pack_profiler);
	
	pResult_pack_profiler->ofHANHELD_ID_dump = sizeof(SPIRUN7_TRACELOG_VMP2_INFO_STACK);
	pResult_pack_profiler->ofHANHELD_ID_dump_size = precision_dump_area_size;

	memcpy(&pResult_pack_profiler->vmp2_name[0], vmname, (wcslen_vnname+1)*sizeof(TCHAR)); //— Ќ”Ћ≈ћ

	*ByteOuts = (SIZE_T)sizealloc;


	pResult_pack_profiler->HANHELD_ID_hash = DH_Calc_area_HASH((LPDWORD)pHANHELD_ID_dumping_region->allocated_pool, precision_dump_area_size);

	return pResult_pack_profiler;
}

ULONG64 DH_PROFILE_SPIRUN7_compare_vmp2_containers(__in pSPIRUN7_TRACELOG_VMP2_INFO_STACK Compare1, __in pSPIRUN7_TRACELOG_VMP2_INFO_STACK Compare2, __out pSPIRUN7_TRACELOG_VMP2_COMPARE_INFO_CLUSTER Compare_Results)
{
	//if (Compare1 == NULL || Compare2 == NULL || Compare_Results == NULL) //filter - is bad pointers??
	//	return false; 

	if (Compare1->HANHELD_ID_hash == Compare2->HANHELD_ID_hash) //filter - is equal hashes???? quick compare
			return -1; //equal compare dumps

	if (wcsicmp(&Compare1->vmp2_name[0], &Compare2->vmp2_name[0])) //filter - is equal hashes???? quick compare
			return -2; //equal compare dumps
	
	DWORD reta;
	register pVMPX_VM_PROFILE_INFO_STACK pCompare_profile_Stack = (pVMPX_VM_PROFILE_INFO_STACK)DH_Read_Current_hunt_profile_CONTAINER(PROFILE_TYPE_VM, &Compare1->vmp2_name[0],PROFILE_READ_ALL,(PULONG)&reta);
	if(pCompare_profile_Stack == NULL)
			return -3;

	memset(Compare_Results,NULL, sizeof(SPIRUN7_TRACELOG_VMP2_COMPARE_INFO_CLUSTER));

	pSPIERUN7_INFO_CLUSTER_POPULAR_LIST pPopularListCluster = (pSPIERUN7_INFO_CLUSTER_POPULAR_LIST)malloc(sizeof(SPIERUN7_INFO_CLUSTER_POPULAR_LIST));
	memset(pPopularListCluster, NULL, sizeof(SPIERUN7_INFO_CLUSTER_POPULAR_LIST));


	register SPIRUN7_HANHELD_ID_dump_LP_pointer fpCompare1 = (SPIRUN7_HANHELD_ID_dump_LP_pointer)(sizeof(SPIRUN7_TRACELOG_VMP2_INFO_STACK) + (SERVICE_ADDRES)Compare1);//(SPIRUN7_HANHELD_ID_dump_LP_pointer)((Compare1->ofHANHELD_ID_dump) + Compare1);
	register SPIRUN7_HANHELD_ID_dump_LP_pointer fpCompare2 = (SPIRUN7_HANHELD_ID_dump_LP_pointer)(sizeof(SPIRUN7_TRACELOG_VMP2_INFO_STACK) + (SERVICE_ADDRES)Compare2); //(SPIRUN7_HANHELD_ID_dump_LP_pointer)((Compare2->ofHANHELD_ID_dump) + Compare2);

	register SERVICE_COUNT Num_of_i;

		if (Compare1->ofHANHELD_ID_dump_size == Compare2->ofHANHELD_ID_dump_size)
		{
			Compare_Results->Is_compares_have_equal_size = true;
			Num_of_i = Compare1->ofHANHELD_ID_dump_size;
		}
		else
		{
			Compare_Results->Is_compares_have_equal_size = false;
				if (Compare1->ofHANHELD_ID_dump_size > Compare2->ofHANHELD_ID_dump_size)
					Num_of_i = Compare2->ofHANHELD_ID_dump_size;
				else
					Num_of_i = Compare1->ofHANHELD_ID_dump_size;
		}
		Num_of_i/=sizeof(SPIRUN7_HANHELD_ID_dump_pointer_type); //TO real COUNT
		SERVICE_COUNT Save_start_Num_of_i = Num_of_i;
		pHANHELD fpHANHELD = (pHANHELD)((SERVICE_ADDRES)pCompare_profile_Stack + pCompare_profile_Stack->ofs_1Containt_handler);

		fpCompare1--;
		fpCompare2--;
	do //main cycle - search first different
	{
		fpCompare1++;
		fpCompare2++;

		if(*fpCompare1 <= VM_HANDLES_MAX_COUNT)
		{
			if (fpHANHELD[*fpCompare1].is_duplicate_hanheld_in_supply15 == false)
				pPopularListCluster->pPopular_HANHELD_calls_count[*fpCompare1]++; //инкремент пр€мой €чейки
			else
			{
				SERVICE_COUNT	i_support = VM_HANDLES_MAX_COUNT; //DH_Mark_out_Duplicate_HANHELDs_in_Supply15 помечает дубликаты в верхнем диапазоне
				HANHELD_ADDRES double_hanheld = fpHANHELD[*fpCompare1].HANDELD_Primary_Addres;
				register pHANHELD pScanHANHELD = &fpHANHELD[0];
				unsigned long a = 0;
				do
				{
					if((double_hanheld == pScanHANHELD->HANDELD_Primary_Addres) && (pScanHANHELD->is_duplicate_hanheld_in_supply15 == false))
							{pPopularListCluster->pPopular_HANHELD_calls_count[VM_HANDLES_MAX_COUNT-i_support]++; break;} //перенос инкремента с дублирующейс€ €чейки в основную

						pScanHANHELD++;
						a++;
				}while(i_support--);
			}//end else if
		}//critical filter  (*fpCompare1 <= VM_HANDLES_MAX_COUNT)

		if(*fpCompare1 == *fpCompare2)
			continue;
			if (DH_PROFILE_SPIRUN7_TRANSLATE_VMP2_TRACE_LOG_HANHELD_ID_Address_only(*fpCompare1,pCompare_profile_Stack) == DH_PROFILE_SPIRUN7_TRANSLATE_VMP2_TRACE_LOG_HANHELD_ID_Address_only(*fpCompare2,pCompare_profile_Stack))
				continue;
				{
				size_t ii;
				/*
				if(Num_of_i < SPIRUN7_MAX_HANHELD_COMPARED_SHOW) //set record limit
					ii = Num_of_i;
				else
					ii = SPIRUN7_MAX_HANHELD_COMPARED_SHOW;*/
				codedef_set_limits_condition_up(SPIRUN7_MAX_HANHELD_COMPARED_SHOW,ii,Num_of_i);
				SERVICE_COUNT save_ii = ii;
				/////////
				Compare_Results->Different_hahelds_1_count = ii;
				Compare_Results->Different_hahelds_2_count = ii;

				SPIRUN7_HANHELD_ID_dump_LP_pointer fpCompare_TMP = fpCompare1; //diff є1

						do //fill different cluster є1
						{
							DH_PROFILE_SPIRUN7_TRANSLATE_VMP2_TRACE_LOG_HANHELD_ID(*fpCompare_TMP,pCompare_profile_Stack, (pDH_SPIRUN7_TRACELOG_HANHELD_COMPARE_RECORD)&Compare_Results->Different_hahelds_1[ii]);
							Compare_Results->Different_hahelds_1[ii].How_count = DH_PROFILE_SPIRUN7_Get_Corrent_count_from_cluster(*fpCompare_TMP++,pCompare_profile_Stack,pPopularListCluster);
						}while(ii--); 
						

					fpCompare_TMP = fpCompare2; //diff є2
					ii = save_ii; //return original ii
						do //fill different cluster є2
						{
							DH_PROFILE_SPIRUN7_TRANSLATE_VMP2_TRACE_LOG_HANHELD_ID(*fpCompare_TMP,pCompare_profile_Stack, (pDH_SPIRUN7_TRACELOG_HANHELD_COMPARE_RECORD)&Compare_Results->Different_hahelds_2[ii]);
							Compare_Results->Different_hahelds_2[ii].How_count = DH_PROFILE_SPIRUN7_Get_Corrent_count_from_cluster(*fpCompare_TMP++,pCompare_profile_Stack,pPopularListCluster);
						}
						while(ii--); 

				codedef_set_limits_condition_up(SPIRUN7_MAX_HANHELD_COMPARED_SHOW,ii,Save_start_Num_of_i - Num_of_i);
				Compare_Results->Equal_hahelds_count = ii;

					fpCompare_TMP = fpCompare1--; //equal (+ down to first equal instruction)
						do //fill equal 
						{
							DH_PROFILE_SPIRUN7_TRANSLATE_VMP2_TRACE_LOG_HANHELD_ID(*fpCompare_TMP,pCompare_profile_Stack, (pDH_SPIRUN7_TRACELOG_HANHELD_COMPARE_RECORD)&Compare_Results->Equal_hahelds[ii]);
							Compare_Results->Equal_hahelds[ii].How_count = DH_PROFILE_SPIRUN7_Get_Corrent_count_from_cluster(*fpCompare_TMP--,pCompare_profile_Stack,pPopularListCluster);
						}
						while(ii--);
				
					Compare_Results->Equal_hahelds_until_first_different = Save_start_Num_of_i - Num_of_i;
				break;
				}
		
	}while(--Num_of_i);

free(pCompare_profile_Stack);
free(pPopularListCluster);
return NULL;
}

__forceinline SERVICE_TYPE DH_PROFILE_SPIRUN7_TRANSLATE_VMP2_TRACE_LOG_HANHELD_ID(__in SPIRUN7_HANHELD_ID_dump_pointer_type Hanheld_ID_Log, __in pVMPX_VM_PROFILE_INFO_STACK VMP2_Info_Stack, pDH_SPIRUN7_TRACELOG_HANHELD_COMPARE_RECORD Record_compare_cluster)
{
	if(Hanheld_ID_Log > VM_HANDLES_MAX_COUNT)
		return -1;

	pHANHELD fpHANHELD = (pHANHELD)((SERVICE_ADDRES)VMP2_Info_Stack + VMP2_Info_Stack->ofs_1Containt_handler);
	SERVICE_TYPE Hand_TYPE = fpHANHELD[Hanheld_ID_Log].HANDELD_type;

	Record_compare_cluster->Type_string = DH_VM_HANHELD_type_to_string(Hand_TYPE);
	Record_compare_cluster->Hanhled_addr = fpHANHELD[Hanheld_ID_Log].HANDELD_Primary_Addres;
	
return Hand_TYPE;
}

__forceinline HANHELD_ADDRES DH_PROFILE_SPIRUN7_TRANSLATE_VMP2_TRACE_LOG_HANHELD_ID_Address_only(__in SPIRUN7_HANHELD_ID_dump_pointer_type Hanheld_ID_Log, __in pVMPX_VM_PROFILE_INFO_STACK VMP2_Info_Stack)
{
	if(Hanheld_ID_Log > VM_HANDLES_MAX_COUNT)
		return -1;
	pHANHELD fpHANHELD = (pHANHELD)((SERVICE_ADDRES)VMP2_Info_Stack + VMP2_Info_Stack->ofs_1Containt_handler);

	return fpHANHELD[Hanheld_ID_Log].HANDELD_Primary_Addres;
}

__forceinline SERVICE_COUNT DH_PROFILE_SPIRUN7_Get_Corrent_count_from_cluster(__in SPIRUN7_HANHELD_ID_dump_pointer_type Hanheld_ID_Log, __in pVMPX_VM_PROFILE_INFO_STACK VMP2_Info_Stack, __in pSPIERUN7_INFO_CLUSTER_POPULAR_LIST Cluster)
{
	if(Hanheld_ID_Log > VM_HANDLES_MAX_COUNT)
		return NULL;

	pHANHELD fpHANHELD = (pHANHELD)((SERVICE_ADDRES)VMP2_Info_Stack + VMP2_Info_Stack->ofs_1Containt_handler);

	if(fpHANHELD[Hanheld_ID_Log].is_duplicate_hanheld_in_supply15 == false)
		return Cluster->pPopular_HANHELD_calls_count[Hanheld_ID_Log];
	else
	{
		register HANHELD_ADDRES Search_addres = fpHANHELD[Hanheld_ID_Log].HANDELD_Primary_Addres;
		register unsigned long sup_ii = VM_HANDLES_MAX_COUNT;
		do
		{
				if((Search_addres == fpHANHELD[sup_ii].HANDELD_Primary_Addres) && (fpHANHELD[sup_ii].is_duplicate_hanheld_in_supply15 == false))
					return Cluster->pPopular_HANHELD_calls_count[sup_ii];
		}while(sup_ii--);
	return NULL;
	}//end else
return NULL;
}

pREGION_COPY __fastcall DH_SPIRUN7_Take_Region_Snapshot_by_addres(__in SERVICE_ADDRES RegionAddres, __in SERVICE_TYPE RegionType, __out PIMAGE_SECTION_HEADER* pPESection_header)
{
	register REGION_COPY Target_Region;
	Target_Region.type_of_region = RegionType;
	Target_Region.flow_pointer_to_allocated_pool = NULL;
	*pPESection_header = NULL;

	register PIMAGE_SECTION_HEADER pSecHeader_vpp = HUNT_HIVE.Image_Section;
	register SERVICE_ADDRES MainModule = (SERVICE_ADDRES)HUNT_HIVE.Process_HIVE.Main_HMODULE;

	register unsigned long i = HUNT_HIVE.Image_NT_headers.FileHeader.NumberOfSections;

	//****** — јЌ»–ќ¬јЌ»≈ —≈ ÷»… PE-файла **************//
	pSecHeader_vpp--;
	do
	{
		pSecHeader_vpp++;
		SERVICE_ADDRES Lo_BORDER = pSecHeader_vpp->VirtualAddress + MainModule;
		SERVICE_ADDRES Hi_BORDER = pSecHeader_vpp->Misc.VirtualSize + Lo_BORDER;
		if ((Lo_BORDER <= RegionAddres) && (Hi_BORDER > RegionAddres))
		{
			Target_Region.region_start_addres = Lo_BORDER;
			Target_Region.region_size = pSecHeader_vpp->Misc.VirtualSize;
			Target_Region.allocated_pool = (LPBYTE)malloc(Target_Region.region_size);
			if (!DH_ReadProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE, (LPVOID)Target_Region.region_start_addres, Target_Region.region_size, (LPVOID)Target_Region.allocated_pool))
			{
				free(Target_Region.allocated_pool);
				return NULL;
			}
			pREGION_COPY pReturn_REGION = (pREGION_COPY)malloc(sizeof(REGION_COPY));
			memcpy(pReturn_REGION, &Target_Region, sizeof(REGION_COPY));
			*pPESection_header = pSecHeader_vpp;
			return pReturn_REGION;
		}	
	} while (--i);

	return NULL;
}