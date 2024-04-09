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
#include "ENGINE_VM_operations.h"
#include "ENGINE_VM_CONTEXT.h"
#include "ENGINE_Profile_operations.h"
#include "ENGINE_Symbol_avanpost.h"

// ******************************************************************************************************************
// ENGINE_VM_operations.cpp - операции с виртуальной машиной
// ******************************************************************************************************************

#define codedef_create_thread_shedule \
		*fphThread = ::CreateRemoteThreadEx(DH_Current_process_fast_handle,NULL,NULL, (LPTHREAD_START_ROUTINE)DH_VM_Supply15_Shedule_threadwork_MT,pThreadWorkStack,NULL,NULL, fpTID);\
				if(*fphThread != NULL) \
					{fphThread++;fpTID++; succes_created_threads++;}

#define codedef_switch_mnemonic_arithmetic__save_immediat_return_result(immediata, digital_mnemonic, pOperand)\
		 switch(digital_mnemonic){\
		 case MNEMONIC_DWORD_add: {*pOperand = immediata; return S__ADD; break;}\
		 case MNEMONIC_DWORD_sub: {*pOperand = immediata; return S__SUB; break;}\
		 case MNEMONIC_DWORD_inc: {return S__INC; break;}\
		 case MNEMONIC_DWORD_dec: {return S__DEC; break;}}

#define codedef_switch_mnemonic_logical__save_immediat_return_result(immediata, digital_mnemonic, pOperand)\
		 switch(digital_mnemonic)\
		 {\
		 case MNEMONIC_DWORD_not: {return S__NOT; break;}\
		 case MNEMONIC_DWORD_neg: {return S__NEG; break;}}

#define codedef_switch_mnemonic_shift_rotate__save_immediat_return_result(immediata, digital_mnemonic, pOperand)\
		 switch(digital_mnemonic)\
		 {\
		 case MNEMONIC_DWORD_ror: {*pOperand = immediata; return S__ROR; break;}\
		 case MNEMONIC_DWORD_rol: {*pOperand = immediata; return S__ROL; break;}}
/*
case S__INC: {*Desd_out_Addres=*Desd_out_Addres+1; break;}
		case S__DEC: {*Desd_out_Addres=*Desd_out_Addres-1; break;}
		case S__XOR: {*Desd_out_Addres^=Decode_operand__optional; break;}
		case S__NOT: {*Desd_out_Addres=~*Desd_out_Addres; break;}
		case S__NEG: {*Desd_out_Addres=-*Desd_out_Addres; break;}
		case S__ADD: {*Desd_out_Addres+=Decode_operand__optional; break;}
		case S__SUB: {*Desd_out_Addres-=Decode_operand__optional; break;}
		case S__BSWAP: {*Desd_out_Addres = _byteswap_uint64(*Desd_out_Addres); break;} //reverse(*Desd_out_Addres)
		case S__ROL: {
				__asm {
					mov r15, Desd_out_Addres
					mov r14, [r15]
					mov rcx, [Decode_operand__optional]
					rol r14, cl
					mov [r15], r14
					 } 
				break;}
		case S__ROR: {
				__asm {
					lea r15, [Desd_out_Addres]
					mov r14, [r15]
					mov rcx, [Decode_operand__optional]
					ror r14, cl
					mov [r15], r14
					 } ; break;}
		}//end switch
		*/

UINT DH_VM_Get_current_profile_data_first(pVM_INFO_STACK VM_Stack)
{
	ULONG Return_Read_bytes = NULL; 
	PVOID alloc_tmp = 0;
	
//	alloc_tmp = DH_Read_Current_hunt_profile_CONTAINER(PROFILE_VM_nfo_CONTAINT_FILE_NAME,PROFILE_READ_ALL,&Return_Read_bytes);
	if(alloc_tmp == NULL)
		return false;
	//UINT64 a = (UINT64)alloc_tmp;

	memcpy(VM_Stack,alloc_tmp,sizeof(DH_VM_information_struct));
	free(alloc_tmp);

	
	return true;
}

UINT DH_Fill_Supply15_HANHELDs_table_for_current_profile(__in pVM_INFO_STACK VM_Stack_Info_set, __in HANHELD HANHELD_Array_Addres[], __in __out pVM_AsmAddr_POOL_STACK VM_addr_pool_stack_set)
{
	if(VM_Stack_Info_set->handles_count == NULL)
		return NULL;
	
	register size_t handles_count = VM_Stack_Info_set->handles_count;
	register SERVICE_ADDRES Supply15_Start_addres = (ULONG64)VM_Stack_Info_set->r12_supply_15_start_address;

	if(handles_count > VM_HANDLES_MAX_COUNT)
		handles_count = VM_HANDLES_MAX_COUNT;

	PDISASM Dissasm_struct = (PDISASM)malloc(sizeof(DISASM));

	size_t i = 0;
	size_t verifed_addr_count=0;
	handles_count--;
	
	////////////// выделение региона с виртуальной машиной /////////////
	register REGION_COPY Target_Section_all_VM;

	 SERVICE_ADDRES MainModule_ImageBase;
	 PIMAGE_SECTION_HEADER pProtSection = DH_get_main_protection_section_params(&MainModule_ImageBase);

	 if (pProtSection == NULL)
		 return NULL;

	 Target_Section_all_VM.region_start_addres = pProtSection->VirtualAddress + MainModule_ImageBase; //SERVICE_ADDRES Region_start_addres = DH_get_mem_region_from_addres(HUNT_HIVE.tOpenProcHANDLE,Hanheld_start_addr,&region_size);
	 Target_Section_all_VM.region_size = pProtSection->SizeOfRawData;

	 
	 Target_Section_all_VM.allocated_pool = (LPBYTE)malloc(Target_Section_all_VM.region_size); //скопированный регион с вм
	 Target_Section_all_VM.type_of_region = REGION_TYPE_ALL_SECTION_INCLUDE_VM;
	 
	 	if (!DH_ReadProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,(LPVOID)Target_Section_all_VM.region_start_addres,Target_Section_all_VM.region_size,(LPVOID)Target_Section_all_VM.allocated_pool))
			return HANHELD_TYPE_DETERMINE_ERROR;
	 //////////////
	

	try //exept
	{
								// F I L L		M A I N
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		BYTE instruction_pool[INSTRUCT_LENGTH];
		do
		{
			if (DH_VerifyProcessAddres(HUNT_HIVE.tOpenProcHANDLE,(PVOID)Supply15_Start_addres))
			{
				verifed_addr_count++; //veryfi count

				HANHELD_Array_Addres[i].Cell_Addres = (CELL_ADDRES)Supply15_Start_addres; //supply addres
				HANHELD_Array_Addres[i].is_valid_HANHELD_address = DH_ReadProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,(LPVOID)Supply15_Start_addres,sizeof(HANHELD_Array_Addres[i].HANDELD_Primary_Addres),(LPVOID)&HANHELD_Array_Addres[i].HANDELD_Primary_Addres);
			
				DH_DEcode_current_supply15_addres(VM_Stack_Info_set->r12_table_DEcode_instructions,VM_Stack_Info_set->r12_table_DEcode_instructions_operand,&HANHELD_Array_Addres[i].HANDELD_Primary_Addres); //DECODE

//				HANHELD_Array_Addres[i].HANDELD_Primary_Addres+=0x00007FD521D0000; //dll_fix
				////////////	set disasm struct	//////////////
				DH_ReadProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,(LPVOID)HANHELD_Array_Addres[i].HANDELD_Primary_Addres,sizeof(instruction_pool),(LPVOID)&instruction_pool[0]);

			
				Dissasm_struct->EIP = (size_t)&instruction_pool[0];
				Dissasm_struct->Archi=64; //64 bit
				Dissasm_struct->VirtualAddr = HANHELD_Array_Addres[i].HANDELD_Primary_Addres; //handler start addr
				Disasm(Dissasm_struct); //disasm!!!
			
				memset(Dissasm_struct,0, sizeof(Dissasm_struct)); //set 0
				memcpy(&HANHELD_Array_Addres[i].hanheld_dissasm_first_insruction[0], &Dissasm_struct->CompleteInstr,sizeof(HANHELD_Array_Addres[i].hanheld_dissasm_first_insruction));

				
				

				HANHELD_Array_Addres[i].HANDELD_type = DH_Get_HANHELD_type(&HANHELD_Array_Addres[i], &Target_Section_all_VM, &VM_addr_pool_stack_set->VM_HANHELD_addr_pool[i]); /// determine HANHELD type

				Supply15_Start_addres=Supply15_Start_addres+sizeof(HANHELD_ADDRES); //inc
			}
		}while(i++ != handles_count);

		//retail dublicate check for all fill adresses
		DH_Mark_out_Duplicate_HANHELDs_in_Supply15(VM_Stack_Info_set);


		////////////////////////// D U M P I N G  TABLE STATE //////////////////////////
		//Dumping supply hanheld tables (crypted)
		DH_ReadProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,(LPVOID)VM_Stack_Info_set->r12_supply_15_start_address,VM_Stack_Info_set->handles_count * sizeof(CELL_ADDRES),&VM_Stack_Info_set->vmp2_original_hanhelds_table_crypted[0]);

		//copy crypted to decrypted block
		memcpy(&VM_Stack_Info_set->vmp2_original_hanhelds_table_decrypted[0], &VM_Stack_Info_set->vmp2_original_hanhelds_table_crypted[0], VM_Stack_Info_set->handles_count * sizeof(CELL_ADDRES));

		//групповой DECODE
		DH_DEcode_supply15_table(VM_Stack_Info_set->r12_table_DEcode_instructions,VM_Stack_Info_set->r12_table_DEcode_instructions_operand,&VM_Stack_Info_set->vmp2_original_hanhelds_table_decrypted[0],VM_HANDLES_MAX_COUNT);
			
		free(Dissasm_struct);
		free(Target_Section_all_VM.allocated_pool);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	catch(...) //exept
	{
		free(Dissasm_struct);
		free(Target_Section_all_VM.allocated_pool);
		return verifed_addr_count;
	}
	return verifed_addr_count;
}



 void __fastcall  DH_DEcode_current_supply15_addres(__in UINT Decode_instruction_s_type, __in ULONG64 Decode_operand__optional, __in __out CELL_ADDRES Desd_out_Addres)
{
	// PRIMARY - DECODE ALGORITHM
	/*
#define S__SERVICE_NODECODE 0
#define S__INC 1
#define S__DEC 2
#define S__XOR 3
#define S__NOT 4
#define S__NEG 5
#define S__ADD 6
#define S__SUB 7
#define S__BSWAP 8
	*/
	switch(Decode_instruction_s_type)
	{
	case S__SERVICE_NODECODE: break;
	case S__INC: {*Desd_out_Addres=*Desd_out_Addres+1; break;}
	case S__DEC: {*Desd_out_Addres=*Desd_out_Addres-1; break;}
	case S__XOR: {*Desd_out_Addres^=Decode_operand__optional; break;}
	case S__NOT: {*Desd_out_Addres=~*Desd_out_Addres; break;}
	case S__NEG: {*Desd_out_Addres=-*Desd_out_Addres; break;}
	case S__ADD: {*Desd_out_Addres+=Decode_operand__optional; break;}
	case S__SUB: {*Desd_out_Addres-=Decode_operand__optional; break;}
	case S__BSWAP: {*Desd_out_Addres = _byteswap_uint64(*Desd_out_Addres); break;} //reverse(*Desd_out_Addres)
	case S__ROL: {
			__asm {
				mov r15, Desd_out_Addres
				mov r14, [r15]
				mov rcx, [Decode_operand__optional]
				rol r14, cl
				mov [r15], r14
				 } 
			break;}
	case S__ROR: {
			__asm {
				mov r15, Desd_out_Addres
				mov r14, [r15]
				mov rcx, [Decode_operand__optional]
				ror r14, cl
				mov [r15], r14
				 } ; break;}

	//case S__SHL: {*Desd_out_Addres = *Desd_out_Addres << Decode_operand__optional; break;}
	//case S__SHR: {*Desd_out_Addres = *Desd_out_Addres >> Decode_operand__optional; break;}
	}
}
 
  void __fastcall  DH_Crypt_current_supply15_addres(__in UINT Decode_instruction_s_type, __in ULONG64 Decode_operand__optional, __in __out CELL_ADDRES Desd_out_Addres)
{

//обратная операция
	switch(Decode_instruction_s_type)
	{
	case S__SERVICE_NODECODE: break;
	case S__INC: {*Desd_out_Addres=*Desd_out_Addres-1; break;}
	case S__DEC: {*Desd_out_Addres=*Desd_out_Addres+1; break;}
	case S__XOR: {*Desd_out_Addres^=Decode_operand__optional; break;}
	case S__NOT: {*Desd_out_Addres=~*Desd_out_Addres; break;}
	case S__NEG: {*Desd_out_Addres=-*Desd_out_Addres; break;}
	case S__ADD: {*Desd_out_Addres-=Decode_operand__optional; break;}
	case S__SUB: {*Desd_out_Addres+=Decode_operand__optional; break;}
	case S__BSWAP: {*Desd_out_Addres = _byteswap_uint64(*Desd_out_Addres); break;} //reverse(*Desd_out_Addres)
	case S__ROL: {
			__asm {
				mov r15, Desd_out_Addres
				mov r14, [r15]
				mov rcx, [Decode_operand__optional]
				ror r14, cl
				mov [r15], r14
				 } 
			break;}
	case S__ROR: {
			__asm {
				lea r15, [Desd_out_Addres]
				mov r14, [r15]
				mov rcx, [Decode_operand__optional]
				rol r14, cl
				mov [r15], r14
				 } ; break;}

	//case S__SHL: {*Desd_out_Addres = *Desd_out_Addres << Decode_operand__optional; break;}
	//case S__SHR: {*Desd_out_Addres = *Desd_out_Addres >> Decode_operand__optional; break;}
	}
}

//DH_DEcode_current_supply15_addres for GROUP
 void __fastcall  DH_DEcode_supply15_table(__in UINT Decode_instruction_s_type, __in ULONG64 Decode_operand__optional, __in __out CELL_ADDRES Desd_out_Addres, __in size_t sizeof_table)
{
	//прямая операция групповая
	do
	{
		switch(Decode_instruction_s_type)
		{
		case S__SERVICE_NODECODE: break;
		case S__INC: {*Desd_out_Addres=*Desd_out_Addres+1; break;}
		case S__DEC: {*Desd_out_Addres=*Desd_out_Addres-1; break;}
		case S__XOR: {*Desd_out_Addres^=Decode_operand__optional; break;}
		case S__NOT: {*Desd_out_Addres=~*Desd_out_Addres; break;}
		case S__NEG: {*Desd_out_Addres=-*Desd_out_Addres; break;}
		case S__ADD: {*Desd_out_Addres+=Decode_operand__optional; break;}
		case S__SUB: {*Desd_out_Addres-=Decode_operand__optional; break;}
		case S__BSWAP: {*Desd_out_Addres = _byteswap_uint64(*Desd_out_Addres); break;} //reverse(*Desd_out_Addres)
		case S__ROL: {
				__asm {
					mov r15, Desd_out_Addres
					mov r14, [r15]
					mov rcx, [Decode_operand__optional]
					rol r14, cl
					mov [r15], r14
					 } 
				break;}
		case S__ROR: {
				__asm {
					lea r15, [Desd_out_Addres]
					mov r14, [r15]
					mov rcx, [Decode_operand__optional]
					ror r14, cl
					mov [r15], r14
					 } ; break;}
		}//end switch
		Desd_out_Addres++;
	}while(sizeof_table--);
}

void __fastcall  DH_Crypt_supply15_table(__in UINT Decode_instruction_s_type, __in ULONG64 Decode_operand__optional, __in __out CELL_ADDRES Desd_out_Addres, __in size_t sizeof_table)
{
//обратная операция групповая
	do
	{
		switch(Decode_instruction_s_type)
		{
		case S__SERVICE_NODECODE: break;
		case S__INC: {*Desd_out_Addres=*Desd_out_Addres-1; break;}
		case S__DEC: {*Desd_out_Addres=*Desd_out_Addres+1; break;}
		case S__XOR: {*Desd_out_Addres^=Decode_operand__optional; break;}
		case S__NOT: {*Desd_out_Addres=~*Desd_out_Addres; break;}
		case S__NEG: {*Desd_out_Addres=-*Desd_out_Addres; break;}
		case S__ADD: {*Desd_out_Addres-=Decode_operand__optional; break;}
		case S__SUB: {*Desd_out_Addres+=Decode_operand__optional; break;}
		case S__BSWAP: {*Desd_out_Addres = _byteswap_uint64(*Desd_out_Addres); break;} //reverse(*Desd_out_Addres)
		case S__ROL: {
				__asm {
					mov r15, Desd_out_Addres
					mov r14, [r15]
					mov rcx, [Decode_operand__optional]
					ror r14, cl
					mov [r15], r14
					 } 
				break;}
		case S__ROR: {
				__asm {
					lea r15, [Desd_out_Addres]
					mov r14, [r15]
					mov rcx, [Decode_operand__optional]
					rol r14, cl
					mov [r15], r14
					 } ; break;}

		}//end switch
		Desd_out_Addres++;
	}while(sizeof_table--);
}

  /////////////////////////////////// G E T		H A N H E L D		T Y P E

  HANHELD_TYPE __fastcall DH_Get_HANHELD_type (pHANHELD Research_hanheld, __in pREGION_COPY Req_all_VM_insruction_region, __in pVM_hanheld_addres_pool Req_VM_hanheld_instruction_addr_pool) 
	// __in HANHELD_ADDRES Hanheld_start_addr, __in HANHELD_TYPE_CRITICAL_DETERMINE_ADDRES pHanheld_critical_determine_addres_array[], __in pREGION_COPY Req_all_VM_insruction_region, __in HANHELD_JUMPOUTS_GRAPH Graph_table[])
 {

	 if((Research_hanheld == NULL) || (Research_hanheld->critical_jumpouts_array == NULL) || (Research_hanheld->determine_type_addres_array == NULL) || (Req_all_VM_insruction_region == NULL))
		 return HANHELD_TYPE_DETERMINE_ERROR;
	
	HANHELD_ADDRES Hanheld_start_addr = Research_hanheld->HANDELD_Primary_Addres;


//	if (!DH_VerifyProcessAddres(HUNT_HIVE.tOpenProcHANDLE,(PVOID)Hanheld_start_addr))
	 size_t curr_size_of_crit_addr_array=0;
	 register SERVICE_FLOW_POINTER fpInstruction_pool; 
	register PDISASM Dissasm_struct = (PDISASM)malloc(sizeof(DISASM)); //disasm
		Dissasm_struct->Archi=64; //64 bit

	if ((Hanheld_start_addr - Req_all_VM_insruction_region->region_start_addres) <= (Req_all_VM_insruction_region->region_size))
		fpInstruction_pool = (Hanheld_start_addr-Req_all_VM_insruction_region->region_start_addres) + Req_all_VM_insruction_region->allocated_pool; //плавающий указатель по скопированному региону (секции) с вм
	register size_t asm_instruct_size;
	register SERVICE_ADDRES Safe_max_bound =  (SERVICE_ADDRES)Req_all_VM_insruction_region->allocated_pool + Req_all_VM_insruction_region->region_size;
	register LPBYTE instruction_pool = Req_all_VM_insruction_region->allocated_pool; //QUICK ACCESS
	register HANHELD_TYPE FOUND_htype=NULL; //текущий найденный тип

	signed long RBP_shift = NULL;
	signed long PCODE_shift = NULL;
	
	Req_VM_hanheld_instruction_addr_pool->HANHELD_asm_instruction_count = NULL;


		signed long crit_addr_i = MAX_HANHELD_TYPE_DETERMINE_CRITICAL_ADDRESES-1;
		codedef_get_upper_table_first_free_cell(crit_addr_i);
			if ((signed long)crit_addr_i < NULL)
				return HANHELD_TYPE_DETERMINE_ERROR; //MAX limit!!!

	register size_t i = MAX_DEEP_ASM_IN_HANHELD;
//MAIN CYCLE - ENUM HANHELD INSTRUCTIONS
	 do
	 {
		 if((ULONG64)fpInstruction_pool >= Safe_max_bound || (ULONG64)fpInstruction_pool < (ULONG64)instruction_pool )
			 break;

		 		Dissasm_struct->EIP = (size_t)fpInstruction_pool;
				Dissasm_struct->VirtualAddr = Hanheld_start_addr; //handler start addr
				asm_instruct_size = Disasm(Dissasm_struct); //disasm!!!

				Req_VM_hanheld_instruction_addr_pool->asm_instruction_addres[Req_VM_hanheld_instruction_addr_pool->HANHELD_asm_instruction_count++] = (ULONG64)Hanheld_start_addr;//save current asm instruction addr
//VARIATOR START  /////////////////////////////////////
#ifdef DEBUG

	//	if (Dissasm_struct->VirtualAddr == 0x000000014170f829)// 0x1426faaad)// 0x1417277a9 //0x1417a3851 //0x141837359) -14191978B
//		{Beep(0,0);}
#endif
				switch(Dissasm_struct->Instruction.Opcode)
				{
					//%%%%%%% MISCELLANEOUS_INSTRUCTION (LEA) %%%%%%%%%%//
				case ASM_LEA: {
					if(Dissasm_struct->Instruction.Category != (GENERAL_PURPOSE_INSTRUCTION|MISCELLANEOUS_INSTRUCTION)) //control filter!
						break;
					if((Dissasm_struct->Argument1.ArgType == 0x20021000) && (Dissasm_struct->Argument2.ArgType == 0x44000000)) //arg.1
						//	{FOUND_htype = HANHELD_TYPE_VM_ENTER;} // - codedef_imm_return(HANHELD_TYPE_VM_ENTER)  //new vm enter
							{codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_VM_ENTER);}
					
					if((Dissasm_struct->Argument1.ArgType == 0x20020040) && (Dissasm_struct->Argument2.ArgType == 0x40000000) && (Dissasm_struct->Argument1.ArgSize == REGSIZE_64) && (Dissasm_struct->Argument2.Memory.Scale == NULL)) //arg.1
							{PCODE_shift += Dissasm_struct->Argument2.Memory.Displacement;} // - codedef_imm_return(HANHELD_TYPE_VM_ENTER)
							break; } //lea rsi, [rsi+x]
						//%%%%%%% SYSTEM (RDTSC, CPUID) %%%%%%%%%%//
				case ASM_RDTSC: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_RDTSC); break;} //RDTSC, but check continue (can be: HANHELD_TYPE_VM_EXIT)
				case ASM_CPUID: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_CPUID); break;} //cpuid - codedef_imm_return(HANHELD_TYPE_CPUID)
						//%%%%%%% DATA_TRANSFER (MOV) %%%%%%%%%%//
				case ASM_MOV_READ_reg8_reg64: {		
							if(Dissasm_struct->Instruction.Category != (GENERAL_PURPOSE_INSTRUCTION|DATA_TRANSFER)) //control filter!
									break;
							if((Dissasm_struct->Argument1.ArgType == 0x20020001) && (Dissasm_struct->Argument2.ArgType == 0x40000000) && (Dissasm_struct->Argument1.ArgSize = REGSIZE_8) && (Dissasm_struct->Argument2.Memory.BaseRegister == REG2) && (Dissasm_struct->Argument2.Memory.IndexRegister == NULL) && (Dissasm_struct->Argument2.SegmentReg == DSReg)) //mov al, [rdx] - DS
								{codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_reg8_memreg64_DS); break;} // - codedef_imm_return(HANHELD_TYPE_MOV_reg8_memreg64)

								if((Dissasm_struct->Argument1.ArgType == 0x20020001) && (Dissasm_struct->Argument2.ArgType == 0x40000000) && (Dissasm_struct->Argument1.ArgSize = REGSIZE_8) && (Dissasm_struct->Argument2.Memory.BaseRegister == REG2) && (Dissasm_struct->Argument2.Memory.IndexRegister == NULL) && (Dissasm_struct->Argument2.SegmentReg == SSReg)) //mov al, [rdx] - SS
									{codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_reg8_memreg64_SS); break;} // - codedef_imm_return(HANHELD_TYPE_MOV_reg8_memreg64)

								if((Dissasm_struct->Argument1.ArgType == 0x20020001) && (Dissasm_struct->Argument2.ArgType == 0x40000000) && (Dissasm_struct->Argument1.ArgSize = REGSIZE_8) && (Dissasm_struct->Argument2.Memory.BaseRegister == REG0) && (Dissasm_struct->Argument2.Memory.IndexRegister == 0x00000080) && (Dissasm_struct->Argument2.SegmentReg == DSReg)) //mov al, [rdx+rax] - DS
									{codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_reg8_struct); break;} 
					break;} //mov al, [rdx]
					  //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&//
				case ASM_MOV_READ_reg_reg64: {
						if(Dissasm_struct->Instruction.Category != (GENERAL_PURPOSE_INSTRUCTION|DATA_TRANSFER)) //control filter!
									break;

							//if((Dissasm_struct->Argument1.ArgType == 0x20020001) && (Dissasm_struct->Argument2.ArgType == 0x40000000) && (Dissasm_struct->Argument2.Memory.BaseRegister == REG0) && (Dissasm_struct->Argument2.Memory.IndexRegister == NULL) && (Dissasm_struct->Argument2.SegmentReg == DSReg)) //args - DS
								if((Dissasm_struct->Argument1.ArgType == 0x20020001) && (Dissasm_struct->Argument2.ArgType == 0x40000000) && (Dissasm_struct->Argument2.Memory.BaseRegister == REG0) && (Dissasm_struct->Argument2.Memory.IndexRegister == NULL)) //args - SS
								{
									switch(Dissasm_struct->Argument2.SegmentReg)
									{
									case (DSReg):
										{
											switch(Dissasm_struct->Argument1.ArgSize) //size of arg.1
											{
											case REGSIZE_8: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_reg8_memreg64_DS); break;} //mov al, [rax]? - codedef_imm_return
											case REGSIZE_16: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_reg16_memreg64_DS); break;} //mov ax, [rax] - codedef_imm_return
											case REGSIZE_32: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_reg32_memreg64_DS); break;} //mov eax, [rax] - codedef_imm_return
											case REGSIZE_64: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_reg64_memreg64_DS); break;} //mov rax, [rax] - codedef_imm_return
											}//switch ArgSize
										break;} //case DS

									case (SSReg): 
										{
											switch(Dissasm_struct->Argument1.ArgSize) //size of arg.1
											{
											case REGSIZE_8: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_reg8_memreg64_SS); break;} //mov al, [rax]? - codedef_imm_return
											case REGSIZE_16: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_reg16_memreg64_SS); break;} //mov ax, [rax] - codedef_imm_return
											case REGSIZE_32: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_reg32_memreg64_SS); break;} //mov eax, [rax] - codedef_imm_return
											case REGSIZE_64: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_reg64_memreg64_SS); break;} //mov rax, [rax] - codedef_imm_return
											}//switch ArgSize
										break;} //case SS

									case (GSReg): 
										{
											switch(Dissasm_struct->Argument1.ArgSize) //size of arg.1
											{
											case REGSIZE_8: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_reg8_memreg64_GS); break;} //mov al, [rax]? - codedef_imm_return
											case REGSIZE_16: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_reg16_memreg64_GS); break;} //mov ax, [rax] - codedef_imm_return
											case REGSIZE_32: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_reg32_memreg64_GS); break;} //mov eax, [rax] - codedef_imm_return
											case REGSIZE_64: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_reg64_memreg64_GS); break;} //mov rax, [rax] - codedef_imm_return
											}//switch ArgSize
										break;} //case GS

									}//switch SegmentReg
								}// if (coommon)

								if( ((Dissasm_struct->Argument1.ArgType == 0x20020004) || (Dissasm_struct->Argument1.ArgType == 0x20020001)) && (Dissasm_struct->Argument2.ArgType == 0x40000000) && (Dissasm_struct->Argument2.Memory.BaseRegister == REG0) && (Dissasm_struct->Argument2.Memory.IndexRegister == 0x000080))
								{
										switch(Dissasm_struct->Argument1.ArgSize) //size of arg.1
											{
											case REGSIZE_8: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_reg8_struct); break;} //mov dl, word ptr ds[rax + rdi] - mov al, word ptr ds[rax + rdi]
											case REGSIZE_16: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_reg16_struct); break;} //mov dx, word ptr ds[rax + rdi] - mov ax, word ptr ds[rax + rdi]
											case REGSIZE_32: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_reg32_struct); break;} //mov edx, word ptr ds[rax + rdi] - mov eax, word ptr ds[rax + rdi]
											case REGSIZE_64: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_reg64_struct); break;} //mov rdx, word ptr ds[rax + rdi] - mov rax, word ptr ds[rax + rdi]
											}//switch ArgSize
									
								break;}// if (coommon)

									if((Dissasm_struct->Argument1.ArgType == 0x20020001) && (Dissasm_struct->Argument2.ArgType == 0x40000000) && (Dissasm_struct->Argument2.Memory.BaseRegister == REG6) && (Dissasm_struct->Argument2.Memory.IndexRegister == 0) && (Dissasm_struct->Argument1.SegmentReg = DSReg))
								{
										switch(Dissasm_struct->Argument1.ArgSize) //size of arg.1
											{
											case REGSIZE_64: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_read_qword_PCODE); break;} //mov rax, qword ptr [rsi]
											case REGSIZE_32: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_read_dword_PCODE); break;} //mov eax, dword ptr [rsi]
											case REGSIZE_16: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_read_word_PCODE); break;} //mov ax, word ptr [rsi]
											}//switch ArgSize
									
								break;}// if (coommon)

					break;}
					//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&//
				case ASM_MOV_WRITE_reg64_reg32: { 
						if(Dissasm_struct->Instruction.Category != (GENERAL_PURPOSE_INSTRUCTION|DATA_TRANSFER)) //control filter!
									break;
						//Argument2.ArgType == 0x20020004
						//Argument2.ArgSize == REGSIZE_32 
						if((Dissasm_struct->Argument1.ArgType == 0x40000000) && (Dissasm_struct->Argument1.Memory.BaseRegister != 0x00000020) && (Dissasm_struct->Argument2.ArgType == 0x20020004) && (Dissasm_struct->Argument1.Memory.IndexRegister == NULL) && (Dissasm_struct->Argument1.Memory.Displacement == NULL))
						  {
							  switch(Dissasm_struct->Argument1.SegmentReg)
									{
							  			case (DSReg): 
										{
											switch(Dissasm_struct->Argument2.ArgSize) //size of arg.1
											{
											case REGSIZE_8: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_memreg64_reg8_DS); break;} //mov [rax], dl - codedef_imm_return (не совпадает с основным!)
											case REGSIZE_16: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_memreg64_reg16_DS); break;} //mov [rax], dx - codedef_imm_return
											case REGSIZE_32: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_memreg64_reg32_DS); break;} //mov [rax], edx - codedef_imm_return
											case REGSIZE_64: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_memreg64_reg64_DS); break;} //mov [rax], rdx - codedef_imm_return
											}//switch ArgSize
										break;}//case DSReg

										case (SSReg): 
										{
											switch(Dissasm_struct->Argument2.ArgSize) //size of arg.1
											{
											case REGSIZE_8: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_memreg64_reg8_SS); break;} //mov [rax], dl - codedef_imm_return (не совпадает с основным!)
											case REGSIZE_16: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_memreg64_reg16_SS); break;} //mov [rax], dx - codedef_imm_return
											case REGSIZE_32: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_memreg64_reg32_SS); break;} //mov [rax], edx - codedef_imm_return
											case REGSIZE_64: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_memreg64_reg64_SS); break;} //mov [rax], rdx - codedef_imm_return
											}//switch ArgSize
										break;}//case SSReg

										case (GSReg): 
										{
											switch(Dissasm_struct->Argument2.ArgSize) //size of arg.1
											{
											case REGSIZE_8: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_memreg64_reg8_GS); break;} //mov [rax], dl -  (не совпадает с основным!)
											case REGSIZE_16: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_memreg64_reg16_GS); break;} //mov [rax], dx 
											case REGSIZE_32: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_memreg64_reg32_GS); break;} //mov [rax], edx
											case REGSIZE_64: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_memreg64_reg64_GS); break;} //mov [rax], rdx 
											}//switch ArgSize
										break;}//case GSReg

									 }//end switch SegmentReg
						 break;}//if (common)
						
						//mov dword ptr [rax + rdi], edx
						//arg1 Memory	{BaseRegister=0x00000001 IndexRegister=0x00000080} arg2 = 	ArgType	0x20020004	int 	ArgSize	0x00000020	int
						if((Dissasm_struct->Argument1.ArgType == 0x40000000) && (Dissasm_struct->Argument2.ArgType == 0x20020004) && (Dissasm_struct->Argument1.Memory.IndexRegister == 0x80) && (Dissasm_struct->Argument1.ArgSize = REGSIZE_64))
						{
							switch (Dissasm_struct->Argument2.ArgSize)
								{
										case REGSIZE_8: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_struct_reg8); break;} //mov dword ptr [rax + rdi], dl
										case REGSIZE_16: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_struct_reg16); break;} //mov dword ptr [rax + rdi], 
										case REGSIZE_32: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_struct_reg32); break;} //mov dword ptr [rax + rdi],
										case REGSIZE_64: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_struct_reg64); break;} //mov dword ptr [rax + rdi],
								}//end swtich Dissasm_struct->Argument3.ArgSize
						;break;} //if (common#2)

					break;}
				case ASM_MOV_WRITE_reg64_reg8: //mov byte ptr [rax], dl
					{
							if(Dissasm_struct->Instruction.Category != (GENERAL_PURPOSE_INSTRUCTION|DATA_TRANSFER)) //control filter!
									break;

								if((Dissasm_struct->Argument1.ArgType == 0x40000000) && (Dissasm_struct->Argument1.Memory.BaseRegister == 0x00000001) && (Dissasm_struct->Argument1.Memory.IndexRegister == NULL) && (Dissasm_struct->Argument1.SegmentReg == DSReg)&& (Dissasm_struct->Argument2.ArgType == 0x20020004))								  
								  {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_memreg64_reg8_DS); break;}

								if((Dissasm_struct->Argument1.ArgType == 0x40000000) && (Dissasm_struct->Argument1.Memory.BaseRegister == 0x00000001) && (Dissasm_struct->Argument1.Memory.IndexRegister == NULL) && (Dissasm_struct->Argument1.SegmentReg == SSReg)&& (Dissasm_struct->Argument2.ArgType == 0x20020004))								  
								  {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_memreg64_reg8_SS); break;}

								if((Dissasm_struct->Argument1.ArgType == 0x40000000) && (Dissasm_struct->Argument1.Memory.BaseRegister == 0x00000001) && (Dissasm_struct->Argument1.Memory.IndexRegister == 0x00000080) && (Dissasm_struct->Argument1.SegmentReg == DSReg) && (Dissasm_struct->Argument2.ArgType == 0x20020004) && (Dissasm_struct->Argument2.ArgSize == 0x8))
								  {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_MOV_struct_reg8); break;} //mov byte ptr [rax + rdx], dl
						break;
					}
												/*
				case ASM_MOVS_byte: {
						if(Dissasm_struct->Instruction.Category != (GENERAL_PURPOSE_INSTRUCTION|STRING_INSTRUCTION)) //control filter!
									break;
						if((Dissasm_struct->Argument1.ArgType == 0x40000000) && (Dissasm_struct->Argument2.ArgType == 0x40000000))
								{FOUND_htype = HANHELD_TYPE_MOVS_byte; break;} //codedef_imm_return
									}
				case HANHELD_TYPE_MOVS_dword: {
						if(Dissasm_struct->Instruction.Category != (GENERAL_PURPOSE_INSTRUCTION|STRING_INSTRUCTION)) //control filter!
									break;
						if((Dissasm_struct->Argument1.ArgType == 0x40000000) && (Dissasm_struct->Argument2.ArgType == 0x40000000))
								{FOUND_htype = HANHELD_TYPE_MOVS_dword; break;} //codedef_imm_return
									}
									*/
							//%%%%%%% LOGICAL (XOR..) %%%%%%%%%%//
				case ASM_XOR_r8_r64: {
						if(Dissasm_struct->Instruction.Category != (GENERAL_PURPOSE_INSTRUCTION|LOGICAL_INSTRUCTION)) //control filter!
									break;

						if((Dissasm_struct->Argument1.ArgType == 0x20020001) && (Dissasm_struct->Argument2.ArgType == 0x40000000) && (Dissasm_struct->Argument1.ArgSize == REGSIZE_8) && (Dissasm_struct->Argument1.Memory.IndexRegister == NULL) && (Dissasm_struct->Argument2.SegmentReg == DSReg))
									{codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_CRC_CHECKL); break;} //codedef_imm_return
					break;}
				case ASM_XOR_dword_stack_reg:
					{
						if(Dissasm_struct->Instruction.Category != (GENERAL_PURPOSE_INSTRUCTION|LOGICAL_INSTRUCTION)) //control filter!
									break;

						if((Dissasm_struct->Argument1.ArgType == 0x40000000) && (Dissasm_struct->Argument2.ArgType == 0x20020001) && (Dissasm_struct->Argument1.Memory.IndexRegister == NULL))
									{codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_XOR_STACK_RSP_DWORD); break;}
					break;}
				case ASM_AND_mem:
					{
						if(Dissasm_struct->Instruction.Category != (GENERAL_PURPOSE_INSTRUCTION|LOGICAL_INSTRUCTION)) //control filter!
									break;
						
						if((Dissasm_struct->Argument1.ArgType == 0x40000000) && (Dissasm_struct->Argument2.ArgType == 0x20020001) && (Dissasm_struct->Argument1.Memory.Displacement == 8))
									{
										switch(Dissasm_struct->Argument2.ArgSize)
										{
										case REGSIZE_64: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_AND_qword_stack); break;} //and word ptr [rbp + 8], rax
										case REGSIZE_32: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_AND_dword_stack); break;} //and word ptr [rbp + 8], eax
										case REGSIZE_16: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_AND_word_stack); break;} //and word ptr [rbp + 8], ax
										}
									}
					break;}

						//%%%%%%% ARITHMETIC_INSTRUCTION (ADD/SUB/INC/DEC) %%%%%%%%%%//
				case ASM_ADD_8bit: { //ASM_SUB == ASM_ADD !!!!!!!! low number
						if(Dissasm_struct->Instruction.Category != (GENERAL_PURPOSE_INSTRUCTION|ARITHMETIC_INSTRUCTION)) //control filter!
									break;

						if((Dissasm_struct->Argument1.ArgType == (REGISTER_TYPE|GENERAL_REG|REG5)) && (Dissasm_struct->Argument2.ArgType == (CONSTANT_TYPE|ABSOLUTE_)) && (Dissasm_struct->Argument1.ArgSize == REGSIZE_64) && (Dissasm_struct->Argument2.ArgSize == REGSIZE_8))// add/sub RBP
									{  DH_VM_Output_8_bit_number_arithmetic_result_from_asm_instruction_mnemonic(Dissasm_struct->Instruction.Mnemonic,(signed char)Dissasm_struct->Instruction.Immediat, &RBP_shift);
										break;} //codedef_imm_return

						if((Dissasm_struct->Argument1.ArgType == (REGISTER_TYPE|GENERAL_REG|REG6)) && (Dissasm_struct->Argument2.ArgType == (CONSTANT_TYPE|ABSOLUTE_)) && (Dissasm_struct->Argument1.ArgSize == REGSIZE_64) && (Dissasm_struct->Argument2.ArgSize == REGSIZE_8))// add/sub RSI
									{DH_VM_Output_8_bit_number_arithmetic_result_from_asm_instruction_mnemonic(Dissasm_struct->Instruction.Mnemonic,(signed char)Dissasm_struct->Instruction.Immediat, &PCODE_shift); break;} //codedef_imm_return

							  break;}
								   
				case ASM_ADD_32bit:{ //ASM_SUB == ASM_ADD !!!!!!!! long number
					if(Dissasm_struct->Instruction.Category != (GENERAL_PURPOSE_INSTRUCTION|ARITHMETIC_INSTRUCTION)) //control filter!
									break;

								if((Dissasm_struct->Argument1.ArgType == (REGISTER_TYPE|GENERAL_REG|REG5)) && (Dissasm_struct->Argument2.ArgType == (CONSTANT_TYPE|ABSOLUTE_)) && (Dissasm_struct->Argument1.ArgSize == REGSIZE_64) && (Dissasm_struct->Argument2.ArgSize == REGSIZE_32))// add/sub RBP
									{  DH_VM_Output_32_bit_number_arithmetic_result_from_asm_instruction_mnemonic(Dissasm_struct->Instruction.Mnemonic,(signed long)Dissasm_struct->Instruction.Immediat, &RBP_shift);
										break;} //codedef_imm_return

								if((Dissasm_struct->Argument1.ArgType == (REGISTER_TYPE|GENERAL_REG|REG6)) && (Dissasm_struct->Argument2.ArgType == (CONSTANT_TYPE|ABSOLUTE_)) && (Dissasm_struct->Argument1.ArgSize == REGSIZE_64) && (Dissasm_struct->Argument2.ArgSize == REGSIZE_32))// add/sub RSI
									{DH_VM_Output_32_bit_number_arithmetic_result_from_asm_instruction_mnemonic(Dissasm_struct->Instruction.Mnemonic,(signed long)Dissasm_struct->Instruction.Immediat, &PCODE_shift); break;} //codedef_imm_return
								   break;}

				case ASM_ADD_mem:
					{
						codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|ARITHMETIC_INSTRUCTION);

						if((Dissasm_struct->Argument1.ArgType == (MEMORY_TYPE)) && (Dissasm_struct->Argument1.Memory.BaseRegister == REG5) && (Dissasm_struct->Argument2.ArgType == (REGISTER_TYPE|GENERAL_REG|REG0)) && (Dissasm_struct->Argument1.Memory.Displacement = 0x8)) // add ptr [rbp + 8], eax; ax; rax
							{
								switch(Dissasm_struct->Argument2.ArgSize)
								{
									case  REGSIZE_64: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_ADD_qword_stack); break;}
									case  REGSIZE_32: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_ADD_dword_stack); break;}
									case  REGSIZE_16: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_ADD_word_stack); break;}
								}//end sub-switch
						
							}//end if
					break;}
				case ASM_ADD_mem_byte:
					{
						codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|ARITHMETIC_INSTRUCTION);

						if((Dissasm_struct->Argument1.ArgType == (MEMORY_TYPE)) && (Dissasm_struct->Argument1.Memory.BaseRegister == REG5) && (Dissasm_struct->Argument2.ArgType == (REGISTER_TYPE|GENERAL_REG|REG0)) && (Dissasm_struct->Argument1.Memory.Displacement = 0x8) && (Dissasm_struct->Argument2.ArgSize = REGSIZE_8))// add byte ptr [rbp + 8], al
							{codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_ADD_byte_stack); break;}
					break;}
				case ASM_DIV_mem:
					{
							if(Dissasm_struct->Instruction.Category != (GENERAL_PURPOSE_INSTRUCTION|ARITHMETIC_INSTRUCTION)) //control filter!
									break;
							
							if((Dissasm_struct->Argument1.ArgType == (REGISTER_TYPE|GENERAL_REG|5)) && (Dissasm_struct->Argument2.ArgType == (MEMORY_TYPE)) && (Dissasm_struct->Argument1.ArgSize == REGSIZE_64) && (Dissasm_struct->Argument2.Memory.Displacement = 0x10))// add/sub RSI
							{codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_DIV_qword_stack); break;}
			

						if((Dissasm_struct->Argument1.ArgType == (REGISTER_TYPE|GENERAL_REG|5)) && (Dissasm_struct->Argument2.ArgType == (MEMORY_TYPE)) && (Dissasm_struct->Argument1.ArgSize == REGSIZE_32) && (Dissasm_struct->Argument2.Memory.Displacement = 0xC))// add/sub RSI
							{codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_DIV_dword_stack); break;}
					break;}
							  /*
				case ASM_INC: //ASM_INC == ASM_DEC !!!!!!!!
					{
					if(Dissasm_struct->Instruction.Category != (GENERAL_PURPOSE_INSTRUCTION|ARITHMETIC_INSTRUCTION)) //control filter!
									break;
					}*/
						//%%%%%%% SHIFT_ROTATE (shrd/shld) %%%%%%%%%%//		   
				case ASM_SHRD: 
					{
						if(Dissasm_struct->Instruction.Category != (GENERAL_PURPOSE_INSTRUCTION|SHIFT_ROTATE)) //control filter!
									break;
						
						if((Dissasm_struct->Argument1.ArgType == (REGISTER_TYPE|GENERAL_REG|REG0)) && (Dissasm_struct->Argument2.ArgType == (REGISTER_TYPE|GENERAL_REG|REG2)) && (Dissasm_struct->Argument3.ArgType == (REGISTER_TYPE|GENERAL_REG|REG1)))// 
							{
								switch((Dissasm_struct->Argument1.ArgSize))
								{
									case  REGSIZE_64: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_SHRD_64); break;}
									case  REGSIZE_32: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_SHRD_32); break;}
									case  REGSIZE_16: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_SHRD_16); break;}
								}//end case Dissasm_struct->Argument1.ArgSize)
							}//end common if
					break;}

				case ASM_SHLD: 
					{
						if(Dissasm_struct->Instruction.Category != (GENERAL_PURPOSE_INSTRUCTION|SHIFT_ROTATE)) //control filter!
									break;

					if((Dissasm_struct->Argument1.ArgType == (REGISTER_TYPE|GENERAL_REG|REG0)) && (Dissasm_struct->Argument2.ArgType == (REGISTER_TYPE|GENERAL_REG|REG2)) && (Dissasm_struct->Argument3.ArgType == (REGISTER_TYPE|GENERAL_REG|REG1)))// 
							{
								switch((Dissasm_struct->Argument1.ArgSize))
								{
									case  REGSIZE_64: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_SHLD_64); break;}
									case  REGSIZE_32: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_SHLD_32); break;}
									case  REGSIZE_16: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_SHLD_16); break;}
								}//end case Dissasm_struct->Argument1.ArgSize)
							}//end common if

					break;}
						//%%%%%%% CONTROL_TRANSFER (RET & JMP & CALL) %%%%%%%%%%//
				case ASM_RET: {codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_VM_EXIT); codedef_set_FLOW_CONTROL_marker(FLOW_CONTROL_MARKER_RET); codedef_set_condition_for_abort_variator_cycle; break;} //codedef_imm_return(HANHELD_TYPE_VM_EXIT);
				case ASM_JMP_CALL_reg: {

					if(Dissasm_struct->Instruction.Category == (GENERAL_PURPOSE_INSTRUCTION|ARITHMETIC_INSTRUCTION)) //case ASM_DEC & ASM_INC !!!!!!!!!!!!!!!
					{
						if((Dissasm_struct->Argument1.ArgType == (REGISTER_TYPE|GENERAL_REG|REG5)) && (Dissasm_struct->Argument1.ArgSize == REGSIZE_64) && (Dissasm_struct->Argument2.ArgType == NO_ARGUMENT))// inc RBP
						{DH_VM_Output_32_bit_number_arithmetic_result_from_asm_instruction_mnemonic(Dissasm_struct->Instruction.Mnemonic,(signed long)Dissasm_struct->Instruction.Immediat, &RBP_shift); break;}

						if((Dissasm_struct->Argument1.ArgType == (REGISTER_TYPE|GENERAL_REG|REG6)) && (Dissasm_struct->Argument1.ArgSize == REGSIZE_64) && (Dissasm_struct->Argument2.ArgType == NO_ARGUMENT))// inc RSI
						{DH_VM_Output_32_bit_number_arithmetic_result_from_asm_instruction_mnemonic(Dissasm_struct->Instruction.Mnemonic,(signed long)Dissasm_struct->Instruction.Immediat, &PCODE_shift); break;}					
						// continue ASM_JMP_CALL_reg
					}
					
					if(Dissasm_struct->Instruction.Category != (GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER)) //control filter!
						break;
								if (Dissasm_struct->Instruction.BranchType == JmpType) //select JMP reg (END of HANHELD!!! TYPE = REACH_HANHELD_END) 
									{codedef_condition_set_FOUND_type_on_indefinite_situation(HANHELD_TYPE_REACH_HANHELD_END); codedef_set_FLOW_CONTROL_marker(FLOW_CONTROL_MARKER_JUMP_REG); codedef_set_condition_for_abort_variator_cycle;} //	{codedef_set_HANHELD_shift_nubmers; codedef_conditional_imm_return__when_found_type; codedef_imm_return(HANHELD_TYPE_REACH_HANHELD_END);} //end - check FOUND_htype variable
								if (Dissasm_struct->Instruction.BranchType == CallType) //call reg64
									{codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_CALL);} //codedef_imm_return
								if (Dissasm_struct->Instruction.BranchType == RetType) //ret (spec condition)
									{codedef_set_FOUND_type_with_save_addres(HANHELD_TYPE_VM_EXIT); codedef_set_FLOW_CONTROL_marker(FLOW_CONTROL_MARKER_RET); codedef_set_condition_for_abort_variator_cycle;} //codedef_imm_return();//EXIT VM - END !!!!!!!!!!!!!!!!!!!
					break;} //jmp reg & call reg
				case ASM_JMP_CONST: {
					if (Dissasm_struct->Instruction.AddrValue != NULL) //jmp CONST_address
							{codedef_calculate_hanheld_jmp_continue; codedef_set_FLOW_CONTROL_marker(FLOW_CONTROL_MARKER_JUMP_CONST); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); asm_instruct_size = NULL;} //(!) DISABLE standart "Hanheld_start_addr & fpInstruction_pool" branch calculate
					break;} //jmps inside hanheld
				// ************ JUMPOUTs ************ //
				case ASM_branch_JO: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}
				case ASM_branch_JNO: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}
				case ASM_branch_JB:  {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}
				case ASM_branch_JAE: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}
				case ASM_branch_JE: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}
				case ASM_branch_JNE: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}
				case ASM_branch_JBE: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}
				case ASM_branch_JA: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}
				case ASM_branch_JS: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}
				case ASM_branch_JNS:{codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}
				case ASM_branch_JP: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}
				case ASM_branch_JNP: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}
				case ASM_branch_JL: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}
				case ASM_branch_JGE: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}
				case ASM_branch_JLE: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}
				case ASM_branch_JG: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}

				default:
					{
						if (Dissasm_struct->Instruction.Category != (GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER))
								break;
						if ((Dissasm_struct->Instruction.BranchType == JmpType) || (Dissasm_struct->Instruction.BranchType == CallType) || (Dissasm_struct->Instruction.BranchType == RetType))
								break;

//						DH_Get_HANHELD_type(Hanheld_start_addr, pHanheld_critical_determine_addres_array,Req_all_VM_insruction_region, Graph_table);
					}
				}

				Hanheld_start_addr+=asm_instruct_size; fpInstruction_pool=(LPVOID)((ULONG64)fpInstruction_pool + (ULONG64)asm_instruct_size);
				/*
				register HANHELD_TYPE tmp_htype = DH_VARIATOR(Dissasm_struct, &Hanheld_start_addr, &fpInstruction_pool, asm_instruct_size,Region_start_addres,(ULONG64)instruction_pool);
				if (tmp_htype != NULL) //в каких случаях продолжаем перебор дальше
				{
					if(tmp_htype == HANHELD_TYPE_DETERMINE_ERROR)
						break;

					if (tmp_htype == HANHELD_TYPE_CALL)
						{FOUND_htype = tmp_htype; break;}

					if (tmp_htype == HANHELD_TYPE_VM_EXIT)
						{FOUND_htype = tmp_htype; break;}

					if (tmp_htype == HANHELD_TYPE_VM_ENTER)
						{FOUND_htype = tmp_htype; break;}

					if (tmp_htype == HANHELD_TYPE_XOR_byte_from_addres)
						 {FOUND_htype = tmp_htype; break;}

					if (tmp_htype == HANHELD_TYPE_MOV_reg_memreg)
						{FOUND_htype = tmp_htype;}

					if (tmp_htype == HANHELD_TYPE_MOV_memreg_reg)
						{FOUND_htype = tmp_htype;}

				FOUND_htype = tmp_htype;
				}
				*/
	 }
	while(i--);
//ENDS MAIN CYCLE - ENUM HANHELD INSTRUCTIONS
//	codedef_set_previously_found_type(HANHELD_TYPE_DETERMINE_ERROR);
exit_type:
	codedef_set_previously_found_type(FOUND_htype);
	SERVICE_COUNT new_jouts = DH_VM_Scan_JUMPOUTs_Init(Research_hanheld, Req_all_VM_insruction_region, Req_VM_hanheld_instruction_addr_pool, Dissasm_struct); //scan jumpouts

	//if(FOUND_htype != NULL)
		//{*pHanheld_critical_determine_addres_array = Hanheld_start_addr; return FOUND_htype;}

	//>>>>>>>>>>>>>> калькуляция и окончательная отчетность <<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	codedef_set_HANHELD_shift_nubmers; //окончательное количество сдвигов

	codedef_set_critical_determine_address_array_count(crit_addr_i); //окончательно количество значений в массиве критических адресов, определяющих тип "островка"

	codedef_conditional_imm_return__when_found_type(FOUND_htype); //условие возврата при полностью пройденной итерации

	codedef_imm_return(HANHELD_TYPE_DETERMINE_ERROR);	//тип "островка" не определен
 }
   
 /*
 __inline HANHELD_TYPE DH_VARIATOR(__in PDISASM Read_Dissasm_struct, __in HANHELD_ADDRES* pHanheld_start_addr, __in SERVICE_FLOW_POINTER* pfpInstruction_pool, __in size_t Asm_instruct_size, __in SERVICE_ADDRES Region_start_addr, __in SERVICE_ADDRES Instruct_POOL)
 {
		switch (Read_Dissasm_struct->Instruction.Category)
			{
			case GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER: //ALL jmps-calls
				{
					////////////////////////////////////////////////////////
					//jmps-calls swtich 
					switch(Read_Dissasm_struct->Instruction.BranchType)
					{
						case JmpType: //JMP
						{
							if (Read_Dissasm_struct->Instruction.AddrValue != NULL) //jmp address
							{*pfpInstruction_pool = (PVOID)((Read_Dissasm_struct->Instruction.AddrValue-Region_start_addr)+Instruct_POOL);   *pHanheld_start_addr = Read_Dissasm_struct->Instruction.AddrValue;}
							else //jmp register
							{return HANHELD_TYPE_DETERMINE_ERROR;}
							break;
						}
						case CallType:
						{
							return HANHELD_TYPE_CALL;
							break;
						}
						case RetType:
						{
							return HANHELD_TYPE_VM_EXIT;
							break;
						}
					}
					////////////////////////////////////////////////////////
				break;
				}
				
			//-------------------------------------------------------//
			case GENERAL_PURPOSE_INSTRUCTION|DATA_TRANSFER: //ALL mov
				{
					if(Read_Dissasm_struct->Instruction.Opcode == ASM_MOV) //ASM_MOV
					{
						if(Read_Dissasm_struct->Argument1.ArgType^(REGISTER_TYPE|GENERAL_REG)==NULL) //mov reg,..
						{
							if(Read_Dissasm_struct->Argument2.ArgType^(MEMORY_TYPE)==NULL)  //mov reg, [reg]
							{return HANHELD_TYPE_MOV_reg_memreg;}
						break;
						}

						if(Read_Dissasm_struct->Argument1.ArgType^(MEMORY_TYPE)==NULL) //mov [reg],..
						{
							if(Read_Dissasm_struct->Argument2.ArgType^(REGISTER_TYPE|GENERAL_REG)==NULL)  //mov [reg], reg
							{return HANHELD_TYPE_MOV_memreg_reg;}
						break;
						}
					} //ASM_MOV
					
				}
			//-------------------------------------------------------//
			case GENERAL_PURPOSE_INSTRUCTION|MISCELLANEOUS_INSTRUCTION:
				{
					if(Read_Dissasm_struct->Instruction.Opcode == ASM_LEA) //LEA
					{
						if(Read_Dissasm_struct->Argument1.ArgType^(REGISTER_TYPE|GENERAL_REG|REG12)==NULL) //mov reg,..
						{
							if(Read_Dissasm_struct->Argument2.ArgType^(RELATIVE_|MEMORY_TYPE)==NULL)
								return HANHELD_TYPE_VM_ENTER;
						}
						
					}//LEA
					break;
				}
			//-------------------------------------------------------//
			case SYSTEM_INSTRUCTION: //ALL rdtsc
				{
					*pHanheld_start_addr+=Asm_instruct_size; *pfpInstruction_pool=(LPVOID)((ULONG64)*pfpInstruction_pool + (ULONG64)Asm_instruct_size);
					return HANHELD_TYPE_RDTSC;
					break;
				}
			//-------------------------------------------------------//
			case GENERAL_PURPOSE_INSTRUCTION|LOGICAL_INSTRUCTION: //xor 
				{
					*pHanheld_start_addr+=Asm_instruct_size; *pfpInstruction_pool=(LPVOID)((ULONG64)*pfpInstruction_pool + (ULONG64)Asm_instruct_size);
					if ((Read_Dissasm_struct->Argument2.ArgType^MEMORY_TYPE)==NULL)
						return HANHELD_TYPE_XOR_byte_from_addres;
					break;
				}
//-------------------------------------------------------//
			default:*pHanheld_start_addr+=Asm_instruct_size; *pfpInstruction_pool=(LPVOID)((ULONG64)*pfpInstruction_pool + (ULONG64)Asm_instruct_size); break;
			}
		return NULL;
 }
 */

 SERVICE_COUNT DH_Mark_out_Duplicate_HANHELDs_in_Supply15(__in __out __in pVM_INFO_STACK VM_Stack_Info_set)
 {
	 //VM_Stack_Info_set->Containt_handler[0].
	 register size_t i = VM_Stack_Info_set->handles_count;
	 register size_t ii;
	 if (VM_Stack_Info_set->handles_count == NULL)
			return NULL;

	 if (VM_Stack_Info_set->Containt_handler[i].is_duplicate_hanheld_in_supply15 == true)
			return NULL;

	 register HANHELD_ADDRES Current_research_HANHELD_addr;
	 register SERVICE_COUNT num_of_dublicate = 0;
	 do
	 {
		 if (VM_Stack_Info_set->Containt_handler[i].is_duplicate_hanheld_in_supply15 == false) //is not dublicate
		 {
			 Current_research_HANHELD_addr = VM_Stack_Info_set->Containt_handler[i].HANDELD_Primary_Addres;
			 ii = VM_Stack_Info_set->handles_count;
				do
				{
					if((Current_research_HANHELD_addr == VM_Stack_Info_set->Containt_handler[ii].HANDELD_Primary_Addres) && (i != ii))
						{VM_Stack_Info_set->Containt_handler[ii].is_duplicate_hanheld_in_supply15 = true; num_of_dublicate++;} //dublicate adress found
				}while(ii--); //secondary cycle
		 }
	 }while(i--); //main cycle

	 VM_Stack_Info_set->duplicate_handles_count = num_of_dublicate; //COPY retail count
	 return num_of_dublicate;
 }

 // ЖЖЖЖЖЖЖЖЖЖЖЖЖЖЖЖЖЖ		SCAN JUMPOUTs			ЖЖЖЖЖЖЖЖЖЖЖЖЖЖЖЖЖЖ
 
 ULONG __fastcall DH_VM_Scan_JUMPOUTs_Init(pHANHELD Research_hanheld, __in pREGION_COPY Req_all_VM_insruction_region, __in pVM_hanheld_addres_pool Req_VM_hanheld_instruction_addr_pool, __in PDISASM Dissasm_struct)
 {
	 if(Research_hanheld->critical_jumpouts_array_count == NULL)
		 return NULL;

	 register SERVICE_COUNT asm_pool_instruction_count = Req_VM_hanheld_instruction_addr_pool->HANHELD_asm_instruction_count;
	 if(asm_pool_instruction_count == NULL)
		 return false;
	 
	 register SERVICE_ADDRES jumpout_desd_addres; //адрес назначения прыжка

	register SERVICE_COUNT i_jouts = NULL; //= MAX_DEEP_ASM_JUMPOUTS_HANHELD; - primary i
	register SERVICE_COUNT i_cycle = MAX_DEEP_ASM_JUMPOUTS_HANHELD; //slave i

	ULONG new_jouts = NULL;
 

	 do
	 {
		 jumpout_desd_addres = Research_hanheld->critical_jumpouts_array[i_jouts].GRAPH_START; //адрес назначения (адрес начала графа)
		 asm_pool_instruction_count = Req_VM_hanheld_instruction_addr_pool->HANHELD_asm_instruction_count;
		
			 do 
			 {codedef_condition_set_GRAPH_circuit_closing(jumpout_desd_addres);} while(asm_pool_instruction_count--); //для сворачивания графа do{}while циклов

			 

		if(Research_hanheld->critical_jumpouts_array[i_jouts].jumpout_function_as_VMProtect_definition != JUMPOUT_TYPE_CYCLE)
			new_jouts = DH_VM_Scan_JUMPOUTs__fast(Research_hanheld, Req_all_VM_insruction_region, Req_VM_hanheld_instruction_addr_pool, Dissasm_struct, i_jouts);

				if(Research_hanheld->critical_jumpouts_array[i_jouts].GRAPH_INIT_asm_instruction == NULL) break; //BREAK CYCLE condition
		
				i_jouts++;	//primary i inc
	 }while(i_cycle--); //slave cycle i decrement 
	 //num of found jumpouts

 }// DH_VM_Scan_JUMPOUTs (main) end


__inline ULONG __fastcall DH_VM_Scan_JUMPOUTs__fast(__in pHANHELD Research_hanheld, __in pREGION_COPY Req_all_VM_insruction_region, __in pVM_hanheld_addres_pool Req_VM_hanheld_instruction_addr_pool, __in PDISASM Dissasm_struct, __in SERVICE_COUNT i_jumpouts)
 {
	if (Research_hanheld->critical_jumpouts_array[i_jumpouts].GRAPH_INIT_asm_instruction == NULL) //ключевое условие работы функции
		return NULL;

		signed long crit_addr_i = MAX_HANHELD_TYPE_DETERMINE_CRITICAL_ADDRESES-1; //второе условие работы функции - предел по массиву
		codedef_get_upper_table_first_free_cell(crit_addr_i);
			if ((signed long)crit_addr_i < NULL)
				return HANHELD_TYPE_DETERMINE_ERROR; //MAX limit!!!
			
	HANHELD_ADDRES Hanheld_start_addr = Research_hanheld->critical_jumpouts_array[i_jumpouts].GRAPH_START; //исследуемая ветка исходя из i_jumpouts

	 register SERVICE_FLOW_POINTER fpInstruction_pool; 
		Dissasm_struct->Archi=64; //64 bit

	if ((Hanheld_start_addr - Req_all_VM_insruction_region->region_start_addres) <= (Req_all_VM_insruction_region->region_size))
		fpInstruction_pool = (Hanheld_start_addr-Req_all_VM_insruction_region->region_start_addres) + Req_all_VM_insruction_region->allocated_pool; //плавающий указатель по скопированному региону (секции) с вм
	register size_t asm_instruct_size;
	register size_t Safe_max_bound =  (ULONG64)Req_all_VM_insruction_region->allocated_pool + Req_all_VM_insruction_region->region_size;
	register LPBYTE instruction_pool = Req_all_VM_insruction_region->allocated_pool; //QUICK ACCESS
	
	SERVICE_COUNT Old_critical_jumpouts_array_count = Research_hanheld->critical_jumpouts_array_count;

	register size_t i = MAX_DEEP_ASM_IN_HANHELD;
//MAIN CYCLE #2 - ENUM HANHELD INSTRUCTIONS
	 do
	 {
		 if((ULONG64)fpInstruction_pool >= Safe_max_bound || (ULONG64)fpInstruction_pool < (ULONG64)instruction_pool )
			 break;

		 		Dissasm_struct->EIP = (size_t)fpInstruction_pool;
				Dissasm_struct->VirtualAddr = Hanheld_start_addr; //handler start addr
				asm_instruct_size = Disasm(Dissasm_struct); //disasm #2!!!

				Req_VM_hanheld_instruction_addr_pool->asm_instruction_addres[Req_VM_hanheld_instruction_addr_pool->HANHELD_asm_instruction_count++] = (ULONG64)Hanheld_start_addr;//save current asm instruction addr

				register ULONG ii_jouts; //для цикла перебора адресов из пула в codedef_check_JMP_desdonation_address_include_on_addr_pool
				
				
				if (Research_hanheld->HANDELD_previously_type != NULL) //critical determine type instruction CHECK
					{
						if ((Research_hanheld->HANHELD_type_asm_disasm_simple_info.Category != NULL) && (Research_hanheld->HANHELD_type_asm_disasm_simple_info.Opcode == Dissasm_struct->Instruction.Opcode) && (Research_hanheld->HANHELD_type_asm_disasm_simple_info.Category == Dissasm_struct->Instruction.Category) && (!strcmp(&Research_hanheld->HANHELD_type_asm_disasm_simple_info.CompleteInstr[0], &Dissasm_struct->CompleteInstr[0]))) //главное условие совпадения инструкции в другой ветке
							{codedef_save_instruction_addres_only;}
					} //end critical instr check
//modified VARIATOR START  /////////////////////////////////////
				switch(Dissasm_struct->Instruction.Opcode)
				{
						//%%%%%%% CONTROL_TRANSFER (RET & JMP & CALL) %%%%%%%%%%//
				case ASM_RET: {codedef_set_FLOW_CONTROL_marker(FLOW_CONTROL_MARKER_RET); codedef_set_condition_for_abort_variator_cycle; break;} 
				case ASM_JMP_CALL_reg: {
					codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER);

					if (Dissasm_struct->Instruction.BranchType == JmpType) //select JMP reg (END of HANHELD!!! TYPE = REACH_HANHELD_END) 
									{codedef_set_FLOW_CONTROL_marker(FLOW_CONTROL_MARKER_JUMP_REG); codedef_set_condition_for_abort_variator_cycle;} //	{codedef_set_HANHELD_shift_nubmers; codedef_conditional_imm_return__when_found_type; codedef_imm_return(HANHELD_TYPE_REACH_HANHELD_END);} //end - check FOUND_htype variable
								if (Dissasm_struct->Instruction.BranchType == RetType) //ret (spec condition)
									{codedef_set_FLOW_CONTROL_marker(FLOW_CONTROL_MARKER_RET); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_condition_for_abort_variator_cycle;} //codedef_imm_return();//EXIT VM - END !!!!!!!!!!!!!!!!!!!
					break;} //jmp reg & call reg
				case ASM_JMP_CONST: {
					if (Dissasm_struct->Instruction.AddrValue != NULL) //jmp CONST_address
							{
							codedef_calculate_hanheld_jmp_continue;
							codedef_set_FLOW_CONTROL_marker(FLOW_CONTROL_MARKER_JUMP_CONST);
							codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue);
							codedef_check_JMP_desdonation_address_include_on_addr_pool(codedef_set_jumpout_func_as_vmprot_def(JUMPOUT_TYPE_BRANCH_DOUBLE); codedef_set_GRAPH_circuit_closing(Hanheld_start_addr); codedef_set_condition_for_abort_variator_cycle;) 
							/*
							ii_jouts = Req_VM_hanheld_instruction_addr_pool->HANHELD_asm_instruction_count; 
								do 
								{ if (Req_VM_hanheld_instruction_addr_pool->asm_instruction_addres[ii_jouts] == Hanheld_start_addr) 
									{
									codedef_set_jumpout_func_as_vmprot_def(JUMPOUT_TYPE_BRANCH_DOUBLE); codedef_set_GRAPH_circuit_closing(Hanheld_start_addr); codedef_set_condition_for_abort_variator_cycle; break;
									} 
								}while(ii_jouts--);
								*/
							asm_instruct_size = NULL;
							} //(!) DISABLE standart "Hanheld_start_addr & fpInstruction_pool" branch calculate
					break;} //jmps inside hanheld
					// ************ JUMPOUTs ************ //
				case ASM_branch_JO: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}
				case ASM_branch_JNO: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}
				case ASM_branch_JB:  {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}
				case ASM_branch_JAE: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}
				case ASM_branch_JE: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}
				case ASM_branch_JNE: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}
				case ASM_branch_JBE: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}
				case ASM_branch_JA: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}
				case ASM_branch_JS: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}
				case ASM_branch_JNS:{codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}
				case ASM_branch_JP: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}
				case ASM_branch_JNP: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}
				case ASM_branch_JL: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}
				case ASM_branch_JGE: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}
				case ASM_branch_JLE: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}
				case ASM_branch_JG: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(Dissasm_struct->Instruction.AddrValue); codedef_set_jumpout_table_element break;}

				default:
					{
						if (Dissasm_struct->Instruction.Category != (GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER))
								break;
						if ((Dissasm_struct->Instruction.BranchType == JmpType) || (Dissasm_struct->Instruction.BranchType == CallType) || (Dissasm_struct->Instruction.BranchType == RetType))
								break;

					}
				}

				Hanheld_start_addr+=asm_instruct_size; fpInstruction_pool=(LPVOID)((ULONG64)fpInstruction_pool + (ULONG64)asm_instruct_size);
	 }
	while(i--);
//ENDS MAIN CYCLE - ENUM HANHELD INSTRUCTIONS
exit_jumpouts:

	//>>>>>>>>>>>>>> калькуляция и окончательная отчетность <<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	codedef_set_critical_determine_address_array_count(crit_addr_i); //окончательно количество значений в массиве критических адресов, определяющих тип "островка"

	codedef_set_jumpout_func_as_vmprot_def(JUMPOUT_TYPE_BRANCH_DOUBLE);	//Research_hanheld->critical_jumpouts_array[jumpout_addr_i].jumpout_function_as_VMProtect_definition = JUMPOUT_TYPE_BRANCH_DOUBLE;
	return codedef_JUMPOUTs_scan_fast__return; 
 }//end "DH_VM_Scan_JUMPOUTs__fast"



 __inline signed long DH_VM_Output_8_bit_number_arithmetic_result_from_asm_instruction_mnemonic(__in char ArgMnemonic[], __in signed char iImmediat, __in signed long *input_number)
 {

	 if ((iImmediat > MAX_IMMEDIANT_FROM_SHIFT_CALC) && ((iImmediat < -MAX_IMMEDIANT_FROM_SHIFT_CALC)))
		 return NULL;

	 DWORD digital_mnemonic = *(LPDWORD)ArgMnemonic; //lo_case

		 switch(digital_mnemonic)
		 {
		 case MNEMONIC_DWORD_add: {return *input_number+=iImmediat; break;}
		 case MNEMONIC_DWORD_sub: {return *input_number-=iImmediat; break;}
		 case MNEMONIC_DWORD_inc: {return *input_number=*input_number + 1; break;}
		 case MNEMONIC_DWORD_dec: {return *input_number=*input_number - 1; break;}
		 }
	 
return NULL;
 }

  __inline signed long DH_VM_Output_32_bit_number_arithmetic_result_from_asm_instruction_mnemonic(__in char ArgMnemonic[], __in signed long iImmediat, __in signed long *input_number)
 {

	 if ((iImmediat > MAX_IMMEDIANT_FROM_SHIFT_CALC) && ((iImmediat < -MAX_IMMEDIANT_FROM_SHIFT_CALC)))
		 return NULL;

	 DWORD digital_mnemonic = *(LPDWORD)ArgMnemonic; //lo_case

		 switch(digital_mnemonic)
		 {
		 case MNEMONIC_DWORD_add: {return *input_number+=iImmediat; break;}
		 case MNEMONIC_DWORD_sub: {return *input_number-=iImmediat; break;}
		 case MNEMONIC_DWORD_inc: {return *input_number=*input_number + 1; break;}
		 case MNEMONIC_DWORD_dec: {return *input_number=*input_number - 1; break;}
		 }
	 
return NULL;
 }

  


//unsigned long DH_VM_Get_NEW_RSP_Enters (__in pREGION_COPY Req_all_VM_insruction_region, __out HANHELD VM_starts_array, __out unsigned long* Array_size)
 unsigned long DH_VM_Get_NEW_RSP_Enters ()
{
 ////////////// выделение региона с виртуальной машиной /////////////
	register REGION_COPY Target_Section_all_VM;
	//SGW3.exe
	 Target_Section_all_VM.region_start_addres = 0x145652000; 
	 Target_Section_all_VM.region_size = 0x00000005C2F000;
	 Target_Section_all_VM.allocated_pool = (LPBYTE)malloc(Target_Section_all_VM.region_size); //скопированный регион с вм
	 Target_Section_all_VM.type_of_region = REGION_TYPE_ALL_SECTION_INCLUDE_VM;
	 
	 	if (!DH_ReadProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,(LPVOID)Target_Section_all_VM.region_start_addres,Target_Section_all_VM.region_size,(LPVOID)Target_Section_all_VM.allocated_pool))
			return HANHELD_TYPE_DETERMINE_ERROR;
////////////////////////////////////////////////////////////////////////

////////////// выделение памяти для таблицы найденных инструкций /////////////
#define max_vm_enters 100000
		UINT64* found_data = (UINT64*)malloc(sizeof (UINT64) * max_vm_enters);
//////////////////////////////////////////////////////////////////////////////

		UINT64 HI_Border = Target_Section_all_VM.region_start_addres + Target_Section_all_VM.region_size;
	 register SERVICE_FLOW_POINTER fpInstruction_pool = Target_Section_all_VM.allocated_pool;
	 register UINT64 Ordinal_section = 0x145652000;
	 register UINT64 asm_instruct_size;

	PDISASM Dissasm_struct = (PDISASM)malloc(sizeof(DISASM));


//VARIATOR START  /////////////////////////////////////
		//lea rsp, [rsp - 0x420]
	UINT64 a = 0;
Dissasm_struct->Archi=64; //64 bit
				do
				{
				Dissasm_struct->EIP = (size_t)fpInstruction_pool;
				Dissasm_struct->VirtualAddr = Ordinal_section;
				asm_instruct_size = Disasm(Dissasm_struct); //disasm!!!

					switch(Dissasm_struct->Instruction.Opcode)
					{
					case ASM_LEA_A:
						{
							if(Dissasm_struct->Instruction.Category != (GENERAL_PURPOSE_INSTRUCTION|MISCELLANEOUS_INSTRUCTION)) //control filter!
								break;
							if (Dissasm_struct->Argument1.ArgType == 0x20020010 && Dissasm_struct->Argument2.Memory.BaseRegister == 0x00000010)
									a++;
						break;
						}
					}
				Ordinal_section += asm_instruct_size;
				fpInstruction_pool = (PVOID)((UINT64)fpInstruction_pool + (UINT64)asm_instruct_size);
				}while( Ordinal_section < HI_Border);
				
///////////free////////////////
free(Target_Section_all_VM.allocated_pool);
free(found_data);
}

 TCHAR* __fastcall DH_VM_HANHELD_type_to_string(HANHELD_TYPE HANHELD_type)
 {
 switch(HANHELD_type)
			{
			case HANHELD_TYPE_VM_ENTER: {return TS_VM__HANHELD_TYPE_VM_ENTER; break;}
			case HANHELD_TYPE_SWITCH_ENTER: {return TS_VM__HANHELD_TYPE_SWITCH_ENTER; break;}
			case HANHELD_TYPE_VM_EXIT: {return TS_VM__HANHELD_TYPE_VM_EXIT; break;}
			case HANHELD_TYPE_CALL: {return TS_VM__HANHELD_TYPE_CALL; break;}
			case HANHELD_TYPE_RDTSC: {return TS_VM__HANHELD_TYPE_RDTSC; break;}
			case HANHELD_TYPE_CPUID: {return TS_VM__HANHELD_TYPE_CPUID; break;}
			case HANHELD_TYPE_CRC_CHECKL: {return TS_VM__HANHELD_TYPE_CRC_CHECKL; break;} //XOR al,[reg]
			case HANHELD_TYPE_XOR_STACK_RSP_DWORD: {return TS_VM__HANHELD_TYPE_XOR_STACK_RSP_DWORD; break;}
													 //MOV DS
			case HANHELD_TYPE_MOV_reg8_memreg64_DS: {return TS_VM__HANHELD_TYPE_MOV_reg8_memreg64_DS; break;} //MOV al, DS:[rdx]
			case HANHELD_TYPE_MOV_reg16_memreg64_DS: {return TS_VM__HANHELD_TYPE_MOV_reg16_memreg64_DS; break;} //MOV ax, DS:[rax]
			case HANHELD_TYPE_MOV_reg32_memreg64_DS: {return TS_VM__HANHELD_TYPE_MOV_reg32_memreg64_DS; break;} //MOV eax, DS:[rax]
			case HANHELD_TYPE_MOV_reg64_memreg64_DS: {return TS_VM__HANHELD_TYPE_MOV_reg64_memreg64_DS; break;} //MOV rax, DS:[rax]
			case HANHELD_TYPE_MOV_memreg64_reg8_DS:	 {return TS_VM__HANHELD_TYPE_MOV_memreg64_reg8_DS; break;} //MOV DS:[rax], dl
			case HANHELD_TYPE_MOV_memreg64_reg16_DS: {return TS_VM__HANHELD_TYPE_MOV_memreg64_reg16_DS; break;} //MOV DS:[rax], dx
			case HANHELD_TYPE_MOV_memreg64_reg32_DS: {return TS_VM__HANHELD_TYPE_MOV_memreg64_reg32_DS; break;} //MOV DS:[rax], edx
			case HANHELD_TYPE_MOV_memreg64_reg64_DS: {return TS_VM__HANHELD_TYPE_MOV_memreg64_reg64_DS; break;} //MOV DS:[rax], rdx
													 //лента p-code
			case HANHELD_TYPE_MOV_read_qword_PCODE: {return TS_VM__HANHELD_TYPE_MOV_read_qword_PCODE; break;}
			case HANHELD_TYPE_MOV_read_dword_PCODE: {return TS_VM__HANHELD_TYPE_MOV_read_dword_PCODE; break;}
			case HANHELD_TYPE_MOV_read_word_PCODE: {return TS_VM__HANHELD_TYPE_MOV_read_word_PCODE; break;}
			case HANHELD_TYPE_MOV_read_byte_PCODE: {return TS_VM__HANHELD_TYPE_MOV_read_byte_PCODE; break; }

													 //MOV SS
			case HANHELD_TYPE_MOV_reg8_memreg64_SS: {return TS_VM__HANHELD_TYPE_MOV_reg8_memreg64_SS; break;}
			case HANHELD_TYPE_MOV_reg16_memreg64_SS: {return TS_VM__HANHELD_TYPE_MOV_reg16_memreg64_SS; break;}
			case HANHELD_TYPE_MOV_reg32_memreg64_SS: {return TS_VM__HANHELD_TYPE_MOV_reg32_memreg64_SS; break;}
			case HANHELD_TYPE_MOV_reg64_memreg64_SS: {return TS_VM__HANHELD_TYPE_MOV_reg64_memreg64_SS; break;}
			case HANHELD_TYPE_MOV_memreg64_reg8_SS: {return TS_VM__HANHELD_TYPE_MOV_memreg64_reg8_SS; break;}
			case HANHELD_TYPE_MOV_memreg64_reg16_SS: {return TS_VM__HANHELD_TYPE_MOV_memreg64_reg16_SS; break;}
			case HANHELD_TYPE_MOV_memreg64_reg32_SS: {return TS_VM__HANHELD_TYPE_MOV_memreg64_reg32_SS; break;}
			case HANHELD_TYPE_MOV_memreg64_reg64_SS: {return TS_VM__HANHELD_TYPE_MOV_memreg64_reg64_SS; break;}
													 //MOV GS
			case HANHELD_TYPE_MOV_reg8_memreg64_GS: {return TS_VM__HANHELD_TYPE_MOV_reg8_memreg64_GS; break;}
			case HANHELD_TYPE_MOV_reg16_memreg64_GS: {return TS_VM__HANHELD_TYPE_MOV_reg16_memreg64_GS; break;}
			case HANHELD_TYPE_MOV_reg32_memreg64_GS: {return TS_VM__HANHELD_TYPE_MOV_reg32_memreg64_GS; break;}
			case HANHELD_TYPE_MOV_reg64_memreg64_GS: {return TS_VM__HANHELD_TYPE_MOV_reg64_memreg64_GS; break;}
			case HANHELD_TYPE_MOV_memreg64_reg8_GS: {return TS_VM__HANHELD_TYPE_MOV_memreg64_reg8_GS; break;}
			case HANHELD_TYPE_MOV_memreg64_reg16_GS: {return TS_VM__HANHELD_TYPE_MOV_memreg64_reg16_GS; break; }
			case HANHELD_TYPE_MOV_memreg64_reg32_GS: {return TS_VM__HANHELD_TYPE_MOV_memreg64_reg32_GS; break; }
			case HANHELD_TYPE_MOV_memreg64_reg64_GS: {return TS_VM__HANHELD_TYPE_MOV_memreg64_reg64_GS; break; }
													 //shift/rot
			case HANHELD_TYPE_SHRD_64: {return TS_VM__HANHELD_TYPE_SHRD_64; break;}
			case HANHELD_TYPE_SHLD_64: {return TS_VM__HANHELD_TYPE_SHLD_64; break;}
			case HANHELD_TYPE_SHRD_32: {return TS_VM__HANHELD_TYPE_SHRD_32; break;}
			case HANHELD_TYPE_SHLD_32: {return TS_VM__HANHELD_TYPE_SHLD_32; break;}
			case HANHELD_TYPE_SHRD_16: {return TS_VM__HANHELD_TYPE_SHRD_16; break;}
			case HANHELD_TYPE_SHLD_16: {return TS_VM__HANHELD_TYPE_SHLD_16; break;}
			case HANHELD_TYPE_SHIFT_R_reg64: {return TS_VM__HANHELD_TYPE_SHIFT_R_reg64; break; }
			case HANHELD_TYPE_SHIFT_R_reg32: {return TS_VM__HANHELD_TYPE_SHIFT_R_reg32; break; }
			case HANHELD_TYPE_SHIFT_R_reg16: {return TS_VM__HANHELD_TYPE_SHIFT_R_reg16; break; }
			case HANHELD_TYPE_SHIFT_R_reg8: {return TS_VM__HANHELD_TYPE_SHIFT_R_reg8; break; }
			case HANHELD_TYPE_SHIFT_L_reg64: {return TS_VM__HANHELD_TYPE_SHIFT_L_reg64; break; }
			case HANHELD_TYPE_SHIFT_L_reg32: {return TS_VM__HANHELD_TYPE_SHIFT_L_reg32; break; }
			case HANHELD_TYPE_SHIFT_L_reg16: {return TS_VM__HANHELD_TYPE_SHIFT_L_reg16; break; }
			case HANHELD_TYPE_SHIFT_L_reg8: {return TS_VM__HANHELD_TYPE_SHIFT_L_reg8; break; }
											//read/write VM variables
			case HANHELD_TYPE_MOV_struct_reg8: {return TS_VM__HANHELD_TYPE_MOV_struct_reg8; break;}
			case HANHELD_TYPE_MOV_struct_reg16: {return TS_VM__HANHELD_TYPE_MOV_struct_reg16; break;}
			case HANHELD_TYPE_MOV_struct_reg32: {return TS_VM__HANHELD_TYPE_MOV_struct_reg32; break;}
			case HANHELD_TYPE_MOV_struct_reg64: {return TS_VM__HANHELD_TYPE_MOV_struct_reg64; break;}
			case HANHELD_TYPE_MOV_reg8_struct: {return TS_VM__HANHELD_TYPE_MOV_reg8_struct; break;}
			case HANHELD_TYPE_MOV_reg16_struct: {return TS_VM__HANHELD_TYPE_MOV_reg16_struct; break;}
			case HANHELD_TYPE_MOV_reg32_struct: {return TS_VM__HANHELD_TYPE_MOV_reg32_struct; break;}
			case HANHELD_TYPE_MOV_reg64_struct: {return TS_VM__HANHELD_TYPE_MOV_reg64_struct; break;}
												//PUSH/POP VStack
			case HANHELD_TYPE_PUSH_VStack_qword: {return TS_VM__HANHELD_TYPE_PUSH_VStack_qword; break; }
			case HANHELD_TYPE_PUSH_VStack_dword: {return TS_VM__HANHELD_TYPE_PUSH_VStack_dword; break; }
			case HANHELD_TYPE_PUSH_VStack_word: {return TS_VM__HANHELD_TYPE_PUSH_VStack_word; break; }
			case HANHELD_TYPE_PUSH_VStack_byte: {return TS_VM__HANHELD_TYPE_PUSH_VStack_byte; break; }
			case HANHELD_TYPE_POP_VStack_qword: {return TS_VM__HANHELD_TYPE_POP_VStack_qword; break; }
			case HANHELD_TYPE_POP_VStack_dword: {return TS_VM__HANHELD_TYPE_POP_VStack_dword; break; }
			case HANHELD_TYPE_POP_VStack_word: {return TS_VM__HANHELD_TYPE_POP_VStack_word; break; }
			case HANHELD_TYPE_POP_VStack_byte: {return TS_VM__HANHELD_TYPE_POP_VStack_byte; break; }
													//add
			case HANHELD_TYPE_ADD_qword_stack: {return TS_VM__HANHELD_TYPE_ADD_qword_stack; break;}
			case HANHELD_TYPE_ADD_dword_stack: {return TS_VM__HANHELD_TYPE_ADD_dword_stack; break;}
			case HANHELD_TYPE_ADD_word_stack: {return TS_VM__HANHELD_TYPE_ADD_word_stack; break;}
			case HANHELD_TYPE_ADD_byte_stack: {return TS_VM__HANHELD_TYPE_ADD_byte_stack; break;}
													//div
			case HANHELD_TYPE_DIV_qword_stack: {return TS_VM__HANHELD_TYPE_DIV_qword_stack; break;}
			case HANHELD_TYPE_DIV_dword_stack: {return TS_VM__HANHELD_TYPE_DIV_dword_stack; break;}
			case HANHELD_TYPE_DIV_reg64: {return TS_VM__HANHELD_TYPE_DIV_reg64; break; }
			case HANHELD_TYPE_DIV_reg32: {return TS_VM__HANHELD_TYPE_DIV_reg32; break; }
			case HANHELD_TYPE_DIV_reg16: {return TS_VM__HANHELD_TYPE_DIV_reg16; break; }
			case HANHELD_TYPE_DIV_reg8: {return TS_VM__HANHELD_TYPE_DIV_reg8; break; }
												//integer div
			case HANHELD_TYPE_Int_DIV_qword_stack: {return TS_VM__HANHELD_TYPE_Int_DIV_qword_stack; break; }
			case HANHELD_TYPE_Int_DIV_dword_stack: {return TS_VM__HANHELD_TYPE_Int_DIV_dword_stack; break; }
			case HANHELD_TYPE_Int_DIV_reg64: {return TS_VM__HANHELD_TYPE_Int_DIV_reg64; break; }
			case HANHELD_TYPE_Int_DIV_reg32: {return TS_VM__HANHELD_TYPE_Int_DIV_reg32; break; }
			case HANHELD_TYPE_Int_DIV_reg16: {return TS_VM__HANHELD_TYPE_Int_DIV_reg16; break; }
			case HANHELD_TYPE_Int_DIV_reg8: {return TS_VM__HANHELD_TYPE_Int_DIV_reg8; break; }
													//mul
			case HANHELD_TYPE_MUL_reg64: {return TS_VM__HANHELD_TYPE_MUL_reg64; break; }
			case HANHELD_TYPE_MUL_reg32: {return TS_VM__HANHELD_TYPE_MUL_reg32; break; }
			case HANHELD_TYPE_MUL_reg16: {return TS_VM__HANHELD_TYPE_MUL_reg16; break; }
			case HANHELD_TYPE_MUL_reg8: {return TS_VM__HANHELD_TYPE_MUL_reg8; break; }
													//integer mul
			case HANHELD_TYPE_INTEGER_MUL_reg64: {return TS_VM__HANHELD_TYPE_INTEGER_MUL_reg64; break; }
			case HANHELD_TYPE_INTEGER_MUL_reg32: {return TS_VM__HANHELD_TYPE_INTEGER_MUL_reg32; break; }
			case HANHELD_TYPE_INTEGER_MUL_reg16: {return TS_VM__HANHELD_TYPE_INTEGER_MUL_reg16; break; }
			case HANHELD_TYPE_INTEGER_MUL_reg8: {return TS_VM__HANHELD_TYPE_INTEGER_MUL_reg8; break; }
													//and
			case HANHELD_TYPE_AND_qword_stack: {return TS_VM__HANHELD_TYPE_AND_qword_stack; break;}
			case HANHELD_TYPE_AND_dword_stack: {return TS_VM__HANHELD_TYPE_AND_dword_stack; break;}
			case HANHELD_TYPE_AND_word_stack: {return TS_VM__HANHELD_TYPE_AND_word_stack; break;}
			case HANHELD_TYPE_AND_byte_stack: {return TS_VM__HANHELD_TYPE_AND_byte_stack; break;}
			case HANHELD_TYPE_not_and_reg64: {return TS_VM__HANHELD_TYPE_not_and_reg64; break; }
			case HANHELD_TYPE_not_and_reg32: {return TS_VM__HANHELD_TYPE_not_and_reg32; break; }
			case HANHELD_TYPE_not_and_reg16: {return TS_VM__HANHELD_TYPE_not_and_reg16; break; }
			case HANHELD_TYPE_not_and_reg8: {return TS_VM__HANHELD_TYPE_not_and_reg8; break; }
											  //repne movs
			case HANHELD_TYPE_MOVS_byte: {return TS_VM__HANHELD_TYPE_MOVS_byte; break;}
			case HANHELD_TYPE_MOVS_dword: {return TS_VM__HANHELD_TYPE_MOVS_dword; break;}
										  //GET_RND func
			case HANHELD_TYPE_GET_RND_const_qword: {return TS_VM__HANHELD_TYPE_GET_RND_const_qword; break; }
			case HANHELD_TYPE_GET_RND_const_dword: {return TS_VM__HANHELD_TYPE_GET_RND_const_dword; break; }
			case HANHELD_TYPE_GET_RND_const_word: {return TS_VM__HANHELD_TYPE_GET_RND_const_word; break; }
			case HANHELD_TYPE_GET_RND_const_byte: {return TS_VM__HANHELD_TYPE_GET_RND_const_byte; break; }
			}
 return NULL;
 }


 HANHELD_TYPE __fastcall DH_VM_HANHELD_string_To_type(TCHAR* HANHELD_type)
 {
		if (!wcsicmp(HANHELD_type, TS_VM__SERVICE_TYPE__hanheld))
			return HANHELD_TYPE_HANHELD;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_VM_ENTER))
			return HANHELD_TYPE_VM_ENTER;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_SWITCH_ENTER))
			return HANHELD_TYPE_SWITCH_ENTER;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_VM_EXIT))
			return HANHELD_TYPE_VM_EXIT;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_CALL))
			return HANHELD_TYPE_CALL;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_RDTSC))
			return HANHELD_TYPE_RDTSC;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_CPUID))
			return HANHELD_TYPE_CPUID;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_CRC_CHECKL))
			return HANHELD_TYPE_CRC_CHECKL;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_XOR_STACK_RSP_DWORD))
			return HANHELD_TYPE_XOR_STACK_RSP_DWORD;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_POPFQ))
			return HANHELD_TYPE_POPFQ;
										 //MOV DS
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_reg8_memreg64_DS))
			return HANHELD_TYPE_MOV_reg8_memreg64_DS;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_reg16_memreg64_DS))
			return HANHELD_TYPE_MOV_reg16_memreg64_DS;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_reg32_memreg64_DS))
			return HANHELD_TYPE_MOV_reg32_memreg64_DS;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_reg64_memreg64_DS))
			return HANHELD_TYPE_MOV_reg64_memreg64_DS;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_memreg64_reg8_DS))
			return HANHELD_TYPE_MOV_memreg64_reg8_DS;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_memreg64_reg16_DS))
			return HANHELD_TYPE_MOV_memreg64_reg16_DS;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_memreg64_reg32_DS))
			return HANHELD_TYPE_MOV_memreg64_reg32_DS;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_memreg64_reg64_DS))
			return HANHELD_TYPE_MOV_memreg64_reg64_DS;

								//чтение p-code
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_read_qword_PCODE))
			return HANHELD_TYPE_MOV_read_qword_PCODE;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_read_dword_PCODE))
			return HANHELD_TYPE_MOV_read_dword_PCODE;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_read_word_PCODE))
			return HANHELD_TYPE_MOV_read_word_PCODE;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_read_byte_PCODE))
			return HANHELD_TYPE_MOV_read_byte_PCODE;

		//чтение-запись в Stack Segment
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_reg8_memreg64_SS))
			return HANHELD_TYPE_MOV_reg8_memreg64_SS;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_reg16_memreg64_SS))
			return HANHELD_TYPE_MOV_reg16_memreg64_SS;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_reg32_memreg64_SS))
			return HANHELD_TYPE_MOV_reg32_memreg64_SS;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_reg64_memreg64_SS))
			return HANHELD_TYPE_MOV_reg64_memreg64_SS;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_memreg64_reg8_SS))
			return HANHELD_TYPE_MOV_memreg64_reg8_SS;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_memreg64_reg16_SS))
			return HANHELD_TYPE_MOV_memreg64_reg16_SS;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_memreg64_reg32_SS))
			return HANHELD_TYPE_MOV_memreg64_reg32_SS;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_memreg64_reg64_SS))
			return HANHELD_TYPE_MOV_memreg64_reg64_SS;

		//чтение-запись в General Segment
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_reg8_memreg64_GS))
			return HANHELD_TYPE_MOV_reg8_memreg64_GS;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_reg16_memreg64_GS))
			return HANHELD_TYPE_MOV_reg16_memreg64_GS;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_reg32_memreg64_GS))
			return HANHELD_TYPE_MOV_reg32_memreg64_GS;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_reg64_memreg64_GS))
			return HANHELD_TYPE_MOV_reg64_memreg64_GS;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_memreg64_reg8_GS))
			return HANHELD_TYPE_MOV_memreg64_reg8_GS;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_memreg64_reg16_GS))
			return HANHELD_TYPE_MOV_memreg64_reg16_GS;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_memreg64_reg32_GS))
			return HANHELD_TYPE_MOV_memreg64_reg32_GS;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_memreg64_reg64_GS))
			return HANHELD_TYPE_MOV_memreg64_reg64_GS;

		//логические операции - shift/rot
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_SHRD_64))
			return HANHELD_TYPE_SHRD_64;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_SHLD_64))
			return HANHELD_TYPE_SHLD_64;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_SHRD_32))
			return HANHELD_TYPE_SHRD_32;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_SHLD_32))
			return HANHELD_TYPE_SHLD_32;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_SHRD_16))
			return HANHELD_TYPE_SHRD_16;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_SHLD_16))
			return HANHELD_TYPE_SHLD_16;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_SHIFT_R_reg64))
			return HANHELD_TYPE_SHIFT_R_reg64;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_SHIFT_R_reg32))
			return HANHELD_TYPE_SHIFT_R_reg32;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_SHIFT_R_reg16))
			return HANHELD_TYPE_SHIFT_R_reg16;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_SHIFT_R_reg8))
			return HANHELD_TYPE_SHIFT_R_reg8;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_SHIFT_L_reg64))
			return HANHELD_TYPE_SHIFT_L_reg64;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_SHIFT_L_reg32))
			return HANHELD_TYPE_SHIFT_L_reg32;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_SHIFT_L_reg16))
			return HANHELD_TYPE_SHIFT_L_reg16;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_SHIFT_L_reg8))
			return HANHELD_TYPE_SHIFT_L_reg8;

		// чтение / запись переменных VM
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_struct_reg8))
			return HANHELD_TYPE_MOV_struct_reg8;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_struct_reg16))
			return HANHELD_TYPE_MOV_struct_reg16;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_struct_reg32))
			return HANHELD_TYPE_MOV_struct_reg32;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_struct_reg64))
			return HANHELD_TYPE_MOV_struct_reg64;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_reg8_struct))
			return HANHELD_TYPE_MOV_reg8_struct;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_reg16_struct))
			return HANHELD_TYPE_MOV_reg16_struct;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_reg32_struct))
			return HANHELD_TYPE_MOV_reg32_struct;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOV_reg64_struct))
			return HANHELD_TYPE_MOV_reg64_struct;

		//операции с VStack
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_PUSH_VStack_qword))
			return HANHELD_TYPE_PUSH_VStack_qword;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_PUSH_VStack_dword))
			return HANHELD_TYPE_PUSH_VStack_dword;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_PUSH_VStack_word))
			return HANHELD_TYPE_PUSH_VStack_word;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_PUSH_VStack_byte))
			return HANHELD_TYPE_PUSH_VStack_byte;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_POP_VStack_qword))
			return HANHELD_TYPE_POP_VStack_qword;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_POP_VStack_dword))
			return HANHELD_TYPE_POP_VStack_dword;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_POP_VStack_word))
			return HANHELD_TYPE_POP_VStack_word;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_POP_VStack_byte))
			return HANHELD_TYPE_POP_VStack_byte;

		//арифметические операции - сложение (стек)
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_ADD_qword_stack))
			return HANHELD_TYPE_ADD_qword_stack;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_ADD_dword_stack))
			return HANHELD_TYPE_ADD_dword_stack;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_ADD_word_stack))
			return HANHELD_TYPE_ADD_word_stack;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_ADD_byte_stack))
			return HANHELD_TYPE_ADD_byte_stack;

		//арифметические операции - деление с остатком
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_DIV_qword_stack))
			return HANHELD_TYPE_DIV_qword_stack;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_DIV_dword_stack))
			return HANHELD_TYPE_DIV_dword_stack;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_DIV_reg64))
			return HANHELD_TYPE_DIV_reg64;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_DIV_reg32))
			return HANHELD_TYPE_DIV_reg32;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_DIV_reg16))
			return HANHELD_TYPE_DIV_reg16;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_DIV_reg8))
			return HANHELD_TYPE_DIV_reg8;

		//арифметические операции - целочисленное деление
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_Int_DIV_qword_stack))
			return HANHELD_TYPE_Int_DIV_qword_stack;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_Int_DIV_dword_stack))
			return HANHELD_TYPE_Int_DIV_dword_stack;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_Int_DIV_reg64))
			return HANHELD_TYPE_Int_DIV_reg64;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_Int_DIV_reg32))
			return HANHELD_TYPE_Int_DIV_reg32;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_Int_DIV_reg16))
			return HANHELD_TYPE_Int_DIV_reg16;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_Int_DIV_reg8))
			return HANHELD_TYPE_Int_DIV_reg8;

		//арифметические операции - умножение
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MUL_reg64))
			return HANHELD_TYPE_MUL_reg64;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MUL_reg32))
			return HANHELD_TYPE_MUL_reg32;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MUL_reg16))
			return HANHELD_TYPE_MUL_reg16;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MUL_reg8))
			return HANHELD_TYPE_MUL_reg8;

		//арифметические операции - целочисленное умножение
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_INTEGER_MUL_reg64))
			return HANHELD_TYPE_INTEGER_MUL_reg64;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_INTEGER_MUL_reg32))
			return HANHELD_TYPE_INTEGER_MUL_reg32;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_INTEGER_MUL_reg16))
			return HANHELD_TYPE_INTEGER_MUL_reg16;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_INTEGER_MUL_reg8))
			return HANHELD_TYPE_INTEGER_MUL_reg8;

		//логические операции - И (&)
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_AND_qword_stack))
			return HANHELD_TYPE_AND_qword_stack;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_AND_dword_stack))
			return HANHELD_TYPE_AND_dword_stack;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_AND_word_stack))
			return HANHELD_TYPE_AND_word_stack;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_AND_byte_stack))
			return HANHELD_TYPE_AND_byte_stack;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_not_and_reg64))
			return HANHELD_TYPE_not_and_reg64;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_not_and_reg32))
			return HANHELD_TYPE_not_and_reg32;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_not_and_reg16))
			return HANHELD_TYPE_not_and_reg16;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_not_and_reg8))
			return HANHELD_TYPE_not_and_reg8;

		//операции копирования больших объемов данных
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOVS_byte))
			return HANHELD_TYPE_MOVS_byte;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_MOVS_dword))
			return HANHELD_TYPE_MOVS_dword;
		//GET_RND func
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_GET_RND_const_qword))
				return HANHELD_TYPE_GET_RND_const_qword;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_GET_RND_const_dword))
			return HANHELD_TYPE_GET_RND_const_dword;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_GET_RND_const_word))
			return HANHELD_TYPE_GET_RND_const_word;
		if (!wcsicmp(HANHELD_type, TS_VM__HANHELD_TYPE_GET_RND_const_byte))
			return HANHELD_TYPE_GET_RND_const_byte;
 return NULL;
 
 }

 size_t DH_VM_Dissasm_instruction(SERVICE_ADDRES src_addr, char* pOut)
 {
	register PDISASM pDissasm_struct = (PDISASM)malloc(sizeof(DISASM));
	register size_t asm_instruct_size = NULL;
	BYTE asm_src[INSTRUCT_LENGTH];

	if(DH_ReadProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,(LPVOID)src_addr,INSTRUCT_LENGTH,&asm_src[0]))
	{
			pDissasm_struct->Archi = 64;
	 		pDissasm_struct->EIP = (size_t)&asm_src[0];
			pDissasm_struct->VirtualAddr = src_addr; //handler start addr
			asm_instruct_size = Disasm(pDissasm_struct); //disasm!!!
			
			strcpy(pOut, &pDissasm_struct->CompleteInstr[0]);
	}

	free(pDissasm_struct);
	return asm_instruct_size;
 }


 /// A	R	B	I	T	A	R	Y /*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 pVM_VMPX_PROFILE_INI_STACK __fastcall DH_VM_Do_INIT_Arbitary_VMPX_context(__in 	WCHAR *VMPX_profile_name, __in	SERVICE_TYPE VMP_Version)
 {
	 size_t reta = 0;
	register pVM_VMPX_PROFILE_INI_STACK pRaw_VMPX_Profiler = (pVM_VMPX_PROFILE_INI_STACK)DH_Read_Current_hunt_profile_CONTAINER(PROFILE_TYPE_VM_VMPX_CONTEXT_INI, VMPX_profile_name,PROFILE_READ_ALL,(PULONG)&reta); //read x_profiler

	if(pRaw_VMPX_Profiler == NULL)
		return NULL;

	//ajust filter as active VMP version
	switch(VMP_Version)
	{
	case VM_TYPE_VMP2: 
		{
			if(HUNT_HIVE.VMPX_Context_VMP2.allocated_pool == NULL)
			{
			HUNT_HIVE.VMPX_Context_VMP2.allocated_pool = (LPBYTE)pRaw_VMPX_Profiler;
			HUNT_HIVE.VMPX_Context_VMP2.region_size = sizeof(VM_VMPX_PROFILE_INI_STACK);
			HUNT_HIVE.VMPX_Context_VMP2.type_of_region = REGION_TYPE_VMPX_CONTEXT_FILTER_STRUCT;
			}
			else
			{
				::VirtualFreeEx(DH_Current_process_fast_handle, HUNT_HIVE.VMPX_Context_VMP2.allocated_pool, HUNT_HIVE.VMPX_Context_VMP2.region_size,MEM_RELEASE); //free(HUNT_HIVE.PATCH_ArbitaryFilter_VMP2.allocated_pool);
				HUNT_HIVE.VMPX_Context_VMP2.allocated_pool = (LPBYTE)pRaw_VMPX_Profiler;
			}
		break;
		} //END CASE 2
	case VM_TYPE_VMP3:
		{
			if(HUNT_HIVE.VMPX_Context_VMP3new.allocated_pool == NULL)
			{
			HUNT_HIVE.VMPX_Context_VMP3new.allocated_pool = (LPBYTE)pRaw_VMPX_Profiler;
			HUNT_HIVE.VMPX_Context_VMP3new.region_size = sizeof(VM_VMPX_PROFILE_INI_STACK);
			HUNT_HIVE.VMPX_Context_VMP3new.type_of_region = REGION_TYPE_VMPX_CONTEXT_FILTER_STRUCT; 
			}
			else
			{
				::VirtualFreeEx(DH_Current_process_fast_handle, HUNT_HIVE.VMPX_Context_VMP3new.allocated_pool, HUNT_HIVE.VMPX_Context_VMP3new.region_size,MEM_RELEASE);
				HUNT_HIVE.VMPX_Context_VMP3new.allocated_pool = (LPBYTE)pRaw_VMPX_Profiler;
			}
		break;} //END CASE 3
	case VM_TYPE_VMX_D4:
		{
		if(HUNT_HIVE.VMPX_Context_VMX_D4new.allocated_pool == NULL)
			{
			HUNT_HIVE.VMPX_Context_VMX_D4new.allocated_pool = (LPBYTE)pRaw_VMPX_Profiler;
			HUNT_HIVE.VMPX_Context_VMX_D4new.region_size = sizeof(VM_VMPX_PROFILE_INI_STACK);
			HUNT_HIVE.VMPX_Context_VMX_D4new.type_of_region = REGION_TYPE_VMPX_CONTEXT_FILTER_STRUCT; 
			}
			else
			{
				::VirtualFreeEx(DH_Current_process_fast_handle, HUNT_HIVE.VMPX_Context_VMX_D4new.allocated_pool, HUNT_HIVE.VMPX_Context_VMX_D4new.region_size,MEM_RELEASE);
				HUNT_HIVE.VMPX_Context_VMX_D4new.allocated_pool = (LPBYTE)pRaw_VMPX_Profiler;
			}
		break;}//end case 4
	default: {free(pRaw_VMPX_Profiler); return NULL;} //not support variants
	}//end version swtich
return pRaw_VMPX_Profiler;
 }

// pVM_VMPX_PROFILE_FOUND_SEQUENCE_STACK __fastcall DH_VM_Scan_VirtualMachine_Contexts(__in SERVICE_ADDRES StartAddres_search_area, __in size_t AreaSize, __in	SERVICE_TYPE VMP_Version, __out pPROGRESS_MULTI_SCAN_INFO_STACK pProgressNfo)
 pVM_VMPX_PROFILE_FOUND_SEQUENCE_STACK __fastcall DH_VM_Scan_VirtualMachine_Contexts(__in pREGION_COPY pIncludeVMRegion, __in	SERVICE_TYPE VMP_Version, __out pPROGRESS_MULTI_SCAN_INFO_STACK pProgressNfo)
 {
	pVM_VMPX_PROFILE_FOUND_SEQUENCE_STACK pFoundSeqStack = (pVM_VMPX_PROFILE_FOUND_SEQUENCE_STACK)::VirtualAllocEx(DH_Current_process_fast_handle, NULL, sizeof(VM_VMPX_PROFILE_FOUND_SEQUENCE_STACK), MEM_COMMIT, PAGE_READWRITE);
	 
	if(pFoundSeqStack == NULL) //filter #1
			return NULL;

	 if(pIncludeVMRegion->allocated_pool == NULL) //filter #2
			{codedef_virtual_freeex__seq_stack(pFoundSeqStack); return NULL;}

	 pVM_VMPX_PROFILE_INI fpVMP_ENTER_context;
	 pVM_VMPX_PROFILE_INI fpVMP_hanheld_context;
	
	 
	 VMPX_PROFILE_FORCED_INIT_ASM Init_asm_table;
	 Init_asm_table.Force_all_After=true;
	 Init_asm_table.Force_all_Predict_wait=true;
	 Init_asm_table.Force_Critical=true;
	 Init_asm_table.Force_init_STATIC=true;
	 Init_asm_table.Force_EXTRACT=true;
	 
	 fpVMP_ENTER_context =			DH_VM_Scan_VirtualMachine_Contexts__Prepeare_Selected_VM_CONTEXT(VMP_Version, HANHELD_TYPE_VM_ENTER, &Init_asm_table);
	 fpVMP_hanheld_context =	DH_VM_Scan_VirtualMachine_Contexts__Prepeare_Selected_VM_CONTEXT(VMP_Version, HANHELD_TYPE_HANHELD, &Init_asm_table);
	

	 if (fpVMP_ENTER_context == NULL || fpVMP_hanheld_context == NULL)
		{::VirtualFreeEx(DH_Current_process_fast_handle, pIncludeVMRegion->allocated_pool, pIncludeVMRegion->region_size, MEM_RELEASE); codedef_virtual_freeex__seq_stack(pFoundSeqStack);  return false;}
	 
	 pFoundSeqStack->VMPX_Version = VMP_Version;
	// SERVICE_ADDRES* pUniqueStack_reserved = (SERVICE_ADDRES*)malloc(sizeof(SERVICE_ADDRES)*MAX_STACK_SEQ);
	 //main multi-scan cycle
	 pFoundSeqStack->fpSeqStackESP = &pFoundSeqStack->Found_Seq[0];
	 register SERVICE_ADDRES fpStatic = (SERVICE_ADDRES)&fpVMP_ENTER_context->xed_Predict_STATIC.dest[0];
	 SERVICE_ADDRES fpStatic_hanheld = (SERVICE_ADDRES)&fpVMP_hanheld_context->xed_Predict_STATIC.dest[0];
	 register LPBYTE fpSequence_VirtualAddr = (LPBYTE)pIncludeVMRegion->region_start_addres;
	 register LPBYTE fpSequence_my_alloc_pool = pIncludeVMRegion->allocated_pool;
	 register const size_t instruction_size = fpVMP_ENTER_context->xed_Predict_STATIC.dest_size;
	 const size_t instruction_size_hanheld = fpVMP_hanheld_context->xed_Predict_STATIC.dest_size;
	 fpSequence_VirtualAddr--;
	 fpSequence_my_alloc_pool--;
	 DISASM Dissasm_struct;
	 Dissasm_struct.Archi = 64;
	 size_t AreaSize = pIncludeVMRegion->region_size;
	 AreaSize-=7; //correct size
	 do
	 {
		 fpSequence_VirtualAddr++; //remote eip/cip
		 fpSequence_my_alloc_pool++; //my pool pointer


		 if (memcmp((const void*)fpSequence_my_alloc_pool, (const void*)fpStatic, instruction_size))
			{
				if (memcmp((const void*)fpSequence_my_alloc_pool, (const void*)fpStatic_hanheld, instruction_size_hanheld))
					continue;
				// ********* __hanheld ********** //
				Dissasm_struct.EIP = (UIntPtr)fpSequence_my_alloc_pool + instruction_size;
				Dissasm_struct.VirtualAddr = (UInt64)fpSequence_VirtualAddr  + instruction_size;
					switch (DH_VM_Scan_VirtualMachine_Contexts__Research_Found_Sequence__without_save(&Dissasm_struct, pIncludeVMRegion, fpVMP_hanheld_context))
					{
						case TRUE: {pProgressNfo->Found_VM_HANHELDs++; break;}
					}//end func switch
					fpSequence_my_alloc_pool+= instruction_size + SAFE_SKIP_CONST; //skip
					fpSequence_VirtualAddr+= instruction_size + SAFE_SKIP_CONST; //skip
					AreaSize-= (instruction_size + SAFE_SKIP_CONST);
					continue;
			}//END IF for vm_enter
			
				// ********* VM_ENTER ********** //
			//static Sequence found! Dissasm now NEXT ASM!!!!
			 pFoundSeqStack->fpSeqStackESP->Predict_STATIC_addres = (SERVICE_ADDRES)fpSequence_VirtualAddr;
			Dissasm_struct.EIP = (UIntPtr)fpSequence_my_alloc_pool + instruction_size;
			Dissasm_struct.VirtualAddr = (UInt64)fpSequence_VirtualAddr  + instruction_size;
			switch (DH_VM_Scan_VirtualMachine_Contexts__Research_Found_Sequence(&Dissasm_struct, pIncludeVMRegion, fpVMP_ENTER_context, pFoundSeqStack))
			{
				case TRUE: {pProgressNfo->Found_VM_ENTERs++; break;}
			}//end func switch
			fpSequence_my_alloc_pool+= instruction_size + SAFE_SKIP_CONST; //skip
			fpSequence_VirtualAddr+= instruction_size + SAFE_SKIP_CONST; //skip
			AreaSize-= (instruction_size + SAFE_SKIP_CONST);
		pProgressNfo->Bytes_leave = AreaSize;
		//SleepEx(255, false);
	 }while(--AreaSize != NULL); //main cycle - переборка цепочки байт
	
	 if(pFoundSeqStack->Found_sequences != NULL)
		 pProgressNfo->Found_VM_CONTEXTs += pFoundSeqStack->Found_sequences - DH_VM_Scan_VirtualMachine_Contexts__Mark_Duplicate_contexts_from_EXTRACT(pFoundSeqStack);

//::VirtualFreeEx(DH_Current_process_fast_handle, IncludeVMRegion.allocated_pool, IncludeVMRegion.region_size, MEM_RELEASE);
//free(pUniqueStack);
return pFoundSeqStack;
 }

pVM_VMPX_PROFILE_INI __fastcall DH_VM_Scan_VirtualMachine_Contexts__Prepeare_Selected_VM_CONTEXT(__in	SERVICE_TYPE VMP_Version, SERVICE_TYPE Init_HANHELD_Type, __in	pVMPX_PROFILE_FORCED_INIT_ASM pReq_init_asm_table)
{
	// ------- ЗАКОММЕНТИРОВАННЫЙ КОД ПЕРЕЛОЖЕН В DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Normalize_and_assembly (!!!) ///////////////

	register pVM_VMPX_PROFILE_INI_STACK pRestoreVMP_Profile_Stack;
	//SELECT VMPX Profile
	switch(VMP_Version)
	{
	case VM_TYPE_VMP3:{pRestoreVMP_Profile_Stack = (pVM_VMPX_PROFILE_INI_STACK)HUNT_HIVE.VMPX_Context_VMP3new.allocated_pool; break;}
	case VM_TYPE_VMP2:{pRestoreVMP_Profile_Stack = (pVM_VMPX_PROFILE_INI_STACK)HUNT_HIVE.VMPX_Context_VMP2.allocated_pool; break;}
	case VM_TYPE_VMX_D4:{pRestoreVMP_Profile_Stack = (pVM_VMPX_PROFILE_INI_STACK)HUNT_HIVE.VMPX_Context_VMX_D4new.allocated_pool; break;}
	default: {return NULL;}
	}

		// ** FILTERS **/
	if(pRestoreVMP_Profile_Stack == NULL)
		return NULL;

	if(Init_HANHELD_Type > HANHELD_TYPE_ENUMS_COUNT)
		return NULL;

	//find init context
	register pVM_VMPX_PROFILE_INI pVM_Sel_init_CONTEXT = &pRestoreVMP_Profile_Stack->VMPX_HANHELD_TYPE_ENUMS[Init_HANHELD_Type];	//select STATIC instruction
	
	//** verify STATIC **//
		//	XEDPARSE xed_assembly;
			//xed_assembly.x64 = true;

	if (pVM_Sel_init_CONTEXT->Assembly_count == NULL)
		return NULL; //error - context is empty

		//*******************//
		if (pReq_init_asm_table->Force_init_STATIC == true) // INIT - STATIC
		{
			if(pVM_Sel_init_CONTEXT->Predict_STATIC_assembly_len == NULL)
				return NULL; //error - absent STATIC VM_ENTER

			//pVM_Sel_init_CONTEXT->xed_Predict_STATIC.x64 = true;
			
				if(pVM_Sel_init_CONTEXT->is_static_Predict_STATIC_assembly == false)
					return NULL; //error - STATIC asm is not static!!!
		
		}//end if Force_init_STATIC
		
		//*******************//
		if (pReq_init_asm_table->Force_all_Predict_wait == true) // INIT - All Predict_wait
		{
			/*
			pVM_Sel_init_CONTEXT->xed_Predict_wait_assembly1.x64 = true;
			pVM_Sel_init_CONTEXT->xed_Predict_wait_assembly2.x64 = true;
			pVM_Sel_init_CONTEXT->xed_Predict_wait_assembly3.x64 = true;
			pVM_Sel_init_CONTEXT->xed_Predict_wait_assembly4.x64 = true;
			pVM_Sel_init_CONTEXT->xed_Predict_wait_assembly1.cip = PREDICT_ADDR_DISTANCE;
			pVM_Sel_init_CONTEXT->xed_Predict_wait_assembly2.cip = PREDICT_ADDR_DISTANCE;
			pVM_Sel_init_CONTEXT->xed_Predict_wait_assembly3.cip = PREDICT_ADDR_DISTANCE;
			pVM_Sel_init_CONTEXT->xed_Predict_wait_assembly4.cip = PREDICT_ADDR_DISTANCE;

				switch(pVM_Sel_init_CONTEXT->Predict_wait_count)
				{
				case 4:
					{

						quick_UNICODE_to_ASCII(&pVM_Sel_init_CONTEXT->Predict_wait_assembly4[0], &pVM_Sel_init_CONTEXT->xed_Predict_wait_assembly4.instr[0]);
							if (XEDParseAssemble(&pVM_Sel_init_CONTEXT->xed_Predict_wait_assembly4) == XEDPARSE_ERROR) //XED parse predict №1
								return NULL; //error -BAD XEDPARSE inside Predict_wait
					}
				case 3:
					{

						quick_UNICODE_to_ASCII(&pVM_Sel_init_CONTEXT->Predict_wait_assembly3[0], &pVM_Sel_init_CONTEXT->xed_Predict_wait_assembly3.instr[0]);
							if (XEDParseAssemble(&pVM_Sel_init_CONTEXT->xed_Predict_wait_assembly3) == XEDPARSE_ERROR) //XED parse predict №1
								return NULL; //error -BAD XEDPARSE inside Predict_wait 
					}
				case 2:
					{

						quick_UNICODE_to_ASCII(&pVM_Sel_init_CONTEXT->Predict_wait_assembly2[0], &pVM_Sel_init_CONTEXT->xed_Predict_wait_assembly2.instr[0]);
							if (XEDParseAssemble(&pVM_Sel_init_CONTEXT->xed_Predict_wait_assembly2) == XEDPARSE_ERROR) //XED parse predict №1
								return NULL; //error -BAD XEDPARSE inside Predict_wait 
					}
				case 1:
					{

						quick_UNICODE_to_ASCII(&pVM_Sel_init_CONTEXT->Predict_wait_assembly1[0], &pVM_Sel_init_CONTEXT->xed_Predict_wait_assembly1.instr[0]);
							if (XEDParseAssemble(&pVM_Sel_init_CONTEXT->xed_Predict_wait_assembly1) == XEDPARSE_ERROR) //XED parse predict №1
								return NULL; //error -BAD XEDPARSE inside Predict_wait 
					}
				}//end switch pVM_Sel_init_CONTEXT->Predict_wait_count
				*/
		}//end if Force_all_Predict_wait


		//*******************//
		if (pReq_init_asm_table->Force_Critical == true) // INIT - Critical
		{
			if(pVM_Sel_init_CONTEXT->Critial_assembly_len == NULL)
				return NULL;
			/*
			pVM_Sel_init_CONTEXT->xed_Critial_assembly.x64 = true;
			pVM_Sel_init_CONTEXT->xed_Critial_assembly.cip = PREDICT_ADDR_DISTANCE;

						pVM_Sel_init_CONTEXT->is_static_Critial_assembly = DH_VM_Scan_VirtualMachine_Contexts__is_static_assembly(&pVM_Sel_init_CONTEXT->Critial_assembly[0], &xed_assembly, &pVM_Sel_init_CONTEXT->xed_Critial_assembly);

						quick_UNICODE_to_ASCII(&pVM_Sel_init_CONTEXT->Critial_assembly[0], &pVM_Sel_init_CONTEXT->xed_Critial_assembly.instr[0]);
							if (XEDParseAssemble(&pVM_Sel_init_CONTEXT->xed_Critial_assembly) == XEDPARSE_ERROR) //XED parse crtitical №1
								return NULL; //error -BAD XEDPARSE inside critical 
					*/
		}//end if Force_Critical

				//*******************//
		if (pReq_init_asm_table->Force_all_After == true) // INIT - all_After
		{
			/*
			pVM_Sel_init_CONTEXT->xed_After_critical_assebly1.x64 = true;
			pVM_Sel_init_CONTEXT->xed_After_critical_assebly2.x64 = true;
			pVM_Sel_init_CONTEXT->xed_After_critical_assebly1.cip = PREDICT_ADDR_DISTANCE;
			pVM_Sel_init_CONTEXT->xed_After_critical_assebly2.cip = PREDICT_ADDR_DISTANCE;
			
				switch(pVM_Sel_init_CONTEXT->After_count)
				{
				case 2:
					{

						quick_UNICODE_to_ASCII(&pVM_Sel_init_CONTEXT->After_critical_assebly2[0], &pVM_Sel_init_CONTEXT->xed_After_critical_assebly2.instr[0]);
							if (XEDParseAssemble(&pVM_Sel_init_CONTEXT->xed_After_critical_assebly2) == XEDPARSE_ERROR) //XED parse after №1
								return NULL; //error -BAD XEDPARSE inside after 
					}
				case 1:
					{

						quick_UNICODE_to_ASCII(&pVM_Sel_init_CONTEXT->After_critical_assebly1[0], &pVM_Sel_init_CONTEXT->xed_After_critical_assebly1.instr[0]);
							if (XEDParseAssemble(&pVM_Sel_init_CONTEXT->xed_After_critical_assebly1) == XEDPARSE_ERROR) //XED parse after №1
								return NULL; //error -BAD XEDPARSE inside after 
					}
				}//end switch pVM_Sel_init_CONTEXT->After_count
				*/
		}//end if Force_Critical

		if (pReq_init_asm_table->Force_EXTRACT == true) // INIT - Force_EXTRACT
		{
			//if(pVM_Sel_init_CONTEXT->Out_EXTRACT_len == NULL && pVM_Sel_init_CONTEXT->First_control_symbol_offset__Out_EXTRACT_control_symbol == NULL)
			//return NULL;
		}

	if(pVM_Sel_init_CONTEXT->cAsmSearchTask_stack_count == NULL)
		return NULL;

return pVM_Sel_init_CONTEXT;
}


SERVICE_TYPE __fastcall DH_VM_Scan_VirtualMachine_Contexts__Research_Found_Sequence(__in DISASM* Dissasm_struct, TARGET_PROC_REGION_COPY* Req_all_VM_insruction_region, __in pVM_VMPX_PROFILE_INI pVMPX_req_context, __out pVM_VMPX_PROFILE_FOUND_SEQUENCE_STACK pScanSequenceStack)
{
			 SERVICE_ADDRES Hanheld_start_addr =  Dissasm_struct->VirtualAddr; //extract VA
			 register PVOID fpInstruction_pool =  (PVOID)Dissasm_struct->EIP; //extract pointer in the allocated pool
			 register size_t i = AVERAGE_DEEP_ASM_IN_HANHELD; //how DEEP hanheld iterations
			 register size_t asm_instruct_size = NULL; //out asm size
			 register size_t Safe_max_bound =  (ULONG64)Req_all_VM_insruction_region->allocated_pool + Req_all_VM_insruction_region->region_size;
			 LPBYTE instruction_pool = Req_all_VM_insruction_region->allocated_pool;
			 register pVM_VMPX_PROFILE_FOUND_SEQUENCE fpSeqESP = pScanSequenceStack->fpSeqStackESP; //указатель на вершину стека (ESP) структуры VM_VMPX_PROFILE_FOUND_SEQUENCE
				 SERVICE_COUNT Extract_len = pVMPX_req_context->Out_EXTRACT_len; //длина _extract (если присуствует)
				 SERVICE_COUNT Extract_value_link_len = pVMPX_req_context->Out_EXTRACT_len; //длина _extract_value_link (если присуствует - имеет смысл при существующей _extract)
				 size_t quick_Extract_BEA_Opcode = pVMPX_req_context->bea_Out_EXTRACT.Instruction.Opcode; //опкод _extract (если присуствует)
				 size_t quick_Extract_BEA_Category = pVMPX_req_context->bea_Out_EXTRACT.Instruction.Category; //ктегория _extract (если присуствует)
				 size_t quick_Extract_value_link_BEA_Opcode = pVMPX_req_context->bea_Out_EXTRACT_VALUE_LINK.Instruction.Opcode; //опкод _extract_value_link (если присуствует)
				 size_t quick_Extract_value_link_BEA_Category = pVMPX_req_context->bea_Out_EXTRACT_VALUE_LINK.Instruction.Category; //ктегория _extract_value_link (если присуствует)
				 //ARGTYPE* pQuick_Extract_BEA_Arg1 = &pVMPX_req_context->bea_Out_EXTRACT.Argument1; //Arg1 _extract (если присуствует)
				 //ARGTYPE* pQuick_Extract_BEA_Arg2 = &pVMPX_req_context->bea_Out_EXTRACT.Argument2; //Arg1 _extract (если присуствует)
				 Int32* pQuick_Extract_BEA_Arg1_argtype = &pVMPX_req_context->bea_Out_EXTRACT.Argument1.ArgType; //Arg1 _extract (если присуствует) - исключая    char ArgMnemonic[64];
				 Int32* pQuick_Extract_BEA_Arg2_argtype = &pVMPX_req_context->bea_Out_EXTRACT.Argument2.ArgType; //Arg1 _extract (если присуствует) - исключая    char ArgMnemonic[64];
				 Int64* pQuick_Extract_BEA_Arg1_displacement = &pVMPX_req_context->bea_Out_EXTRACT.Argument1.Memory.Displacement; //Arg1 _extract (если присуствует) - displacement
				 Int64* pQuick_Extract_BEA_Arg2_displacement = &pVMPX_req_context->bea_Out_EXTRACT.Argument2.Memory.Displacement; //Arg1 _extract (если присуствует) - displacement
				 ARGTYPE* pQuick_Extract_value_link_BEA_Arg1 = &pVMPX_req_context->bea_Out_EXTRACT_VALUE_LINK.Argument1;
				 ARGTYPE* pQuick_Extract_value_link_BEA_Arg2 = &pVMPX_req_context->bea_Out_EXTRACT_VALUE_LINK.Argument2;
				 ARGTYPE* pQuick_Extract_value_link_BEA_Arg3 = &pVMPX_req_context->bea_Out_EXTRACT_VALUE_LINK.Argument3;
				 Int32 Quick_Extract_value_link_BEA_Arg2_MemDisplacement = pVMPX_req_context->bea_Out_EXTRACT_VALUE_LINK.Argument2.Memory.Displacement;
				 fpSeqESP->Stack_RSP_Different = NULL;


			 //enumerate found asm//
			 bool is_found_Predict1_asm = false;
			 bool is_found_Predict2_asm = false;
			 bool is_found_Predict3_asm = false;
			 bool is_found_Predict4_asm = false;
			 bool is_found_Critical_asm = false;
			 bool is_found_After1_asm = false;
			 bool is_found_After2_asm = false;

			 SERVICE_COUNT Switched_predict_wait_count = pVMPX_req_context->Predict_wait_count;
			 SERVICE_COUNT Switched_After_count = pVMPX_req_context->After_count;

			 //ВЫБОР ПЕРВОЙ СКАНИРУЕМОЙ ИНСТРУКЦИИ (VMPX_CONTEXT_TYPE_ASMS)
			 SERVICE_TYPE* fpSelectedScanTaskESP = &pVMPX_req_context->tAsmSearchTask_stack[0];
			 register SERVICE_TYPE CurrentSelectedScanTask = *fpSelectedScanTaskESP;
			 SERVICE_COUNT cSelectedScanTasks_count = pVMPX_req_context->cAsmSearchTask_stack_count;

			 pVMPX_req_context->INCREMENT_STANDBY_TYPE = INREMENTAL_EXECUTE; //set incremental mode for CONTROL SYMBOLs
			 do
			 {
				 if((ULONG64)fpInstruction_pool >= Safe_max_bound || (ULONG64)fpInstruction_pool < (ULONG64)instruction_pool )
					 break;

				Dissasm_struct->EIP = (UIntPtr)fpInstruction_pool;
				Dissasm_struct->VirtualAddr = Hanheld_start_addr; //handler start addr
				asm_instruct_size = Disasm(Dissasm_struct); //disasm!!!

		//		if(Hanheld_start_addr == 0x000000144C4EE84)
			//		Beep(0,0);
	
						//modified VARIATOR START  /////////////////////////////////////
				switch(Dissasm_struct->Instruction.Opcode)
				{
						//%%%%%%% CONTROL_TRANSFER (RET & JMP & CALL) %%%%%%%%%%//
				case ASM_RET_0xBytes: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_condition_for_abort_variator_cycle; break;}
				case ASM_RET: {codedef_set_condition_for_abort_variator_cycle; break;} 
				case ASM_JMP_CALL_reg: {
					codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER);

					if (Dissasm_struct->Instruction.BranchType == JmpType) //select JMP reg (END of HANHELD!!! TYPE = REACH_HANHELD_END) 
									{codedef_set_condition_for_abort_variator_cycle;} 
								if (Dissasm_struct->Instruction.BranchType == RetType) //ret (spec condition)
									{ codedef_set_condition_for_abort_variator_cycle;} //codedef_imm_return();//EXIT VM - END !!!!!!!!!!!!!!!!!!!
					break;} //jmp reg & call reg
				case ASM_JMP_CONST_SHORT:
				case ASM_JMP_CONST: {
					if (Dissasm_struct->Instruction.AddrValue != NULL) //jmp CONST_address
							{codedef_calculate_hanheld_jmp_continue;asm_instruct_size = NULL;}
							
					break;} 
				case ASM_CALL_OPERAND:{codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); if(Dissasm_struct->Instruction.AddrValue - Hanheld_start_addr < NEAR_JMPS_TRICK){codedef_calculate_hanheld_jmp_continue;asm_instruct_size = NULL;}break;}
									//jmps inside hanheld
					// ************ JUMPOUTs ************ //
				/*		
				case ASM_branch_JO: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); break;}
				case ASM_branch_JNO: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); break;}
				case ASM_branch_JB:  {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); break;}
				case ASM_branch_JAE: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); break;}
				case ASM_branch_JE: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); break;}
				case ASM_branch_JNE: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); break;}
				case ASM_branch_JBE: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); break;}
				case ASM_branch_JA: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); break;}
				case ASM_branch_JS: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); break;}
				case ASM_branch_JNS:{codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER);break;}
				case ASM_branch_JP: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER);  break;}
				case ASM_branch_JNP: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); break;}
				case ASM_branch_JL: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); break;}
				case ASM_branch_JGE: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER);  break;}
				case ASM_branch_JLE: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); break;}
				case ASM_branch_JG: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); break;}
				*/
				default:
					{
						if (Dissasm_struct->Instruction.Category == (GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER))
								break;
						if ((Dissasm_struct->Instruction.BranchType != NULL))
								break;
						
						// ____________ PARSE STEP-BYTE-STEP MODE _____________ //

			// *** task stack *** SWITCH
					switch(CurrentSelectedScanTask) //}//++++++++++++++ //}//++++++++++++++
						{
							case VMPX_CONTEXT_ASM_PREDICT_WAIT_1:
											// -- VMPX_CONTEXT_ASM_PREDICT_WAIT_1 -- //
							{
										// !МЕТОД СКАНИРОВАНИЯ ПОСЛЕДОВАТЕЛЬНОСТИ БАЙТ!
								switch(pVMPX_req_context->Quick_combine_type__Critical_control)
								{
								case VMPX_CONTEXT_COMBINE_STATIC__CS_PRESENT: //cтатическая - присутсвует контрольный символ
								case VMPX_CONTEXT_COMBINE_STATIC__NO_CS: //cтатическая - нет контрольных символов
									{
										if ((asm_instruct_size == pVMPX_req_context->xed_Predict_wait_assembly1.dest_size) && !memcmp(fpInstruction_pool, &pVMPX_req_context->xed_Predict_wait_assembly1.dest[0], asm_instruct_size))
												codedef_save_seq_addr_from_ESP(Predict_wait_assembly1_addres)//save addr
									break;}
								case VMPX_CONTEXT_COMBINE_DYNAMIC__NO_CS: //динамическая - нет контрольных символов
									{
										pVMPX_req_context->xed_Predict_wait_assembly1.cip = Hanheld_start_addr; // < коректируем cip >
										XEDParseAssemble(&pVMPX_req_context->xed_Predict_wait_assembly1); //<<< XEDParseAssemble >>>

										if ((asm_instruct_size == pVMPX_req_context->xed_Predict_wait_assembly1.dest_size) && !memcmp(fpInstruction_pool, &pVMPX_req_context->xed_Predict_wait_assembly1.dest[0], asm_instruct_size))
												codedef_save_seq_addr_from_ESP(Predict_wait_assembly1_addres)//save addr
									break;}
								case VMPX_CONTEXT_COMBINE_DYNAMIC__CS_PRESENT: //динамическая - присутсвует контрольный символ
									{
										pVMPX_req_context->xed_Predict_wait_assembly1.cip = Hanheld_start_addr; //AS arg = Hanheld_start_addr
										if (DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Release_normalize_instruction(pVMPX_req_context, VMPX_CONTEXT_ASM_PREDICT_WAIT_1, fpInstruction_pool, fpSeqESP) == true)
											codedef_save_seq_addr_from_ESP(Predict_wait_assembly1_addres)//save addr
									break;}
								}//end VMPX_CONTEXT_ASM_PREDICT_WAIT_1
							break;
							}//end VMPX_CONTEXT_ASM_PREDICT_WAIT_1


							case VMPX_CONTEXT_ASM_PREDICT_WAIT_2:
											// -- VMPX_CONTEXT_ASM_PREDICT_WAIT_2 -- //
							{
										// !МЕТОД СКАНИРОВАНИЯ ПОСЛЕДОВАТЕЛЬНОСТИ БАЙТ!
								switch(pVMPX_req_context->Quick_combine_type__Critical_control)
								{
								case VMPX_CONTEXT_COMBINE_STATIC__CS_PRESENT: //cтатическая - присутсвует контрольный символ
								case VMPX_CONTEXT_COMBINE_STATIC__NO_CS: //cтатическая - нет контрольных символов
									{
										if ((asm_instruct_size == pVMPX_req_context->xed_Predict_wait_assembly2.dest_size) && (!memcmp(fpInstruction_pool, &pVMPX_req_context->xed_Predict_wait_assembly2.dest[0], asm_instruct_size)))
												codedef_save_seq_addr_from_ESP(Predict_wait_assembly2_addres)//save addr
									break;}
								case VMPX_CONTEXT_COMBINE_DYNAMIC__NO_CS: //динамическая - нет контрольных символов
									{
										pVMPX_req_context->xed_Predict_wait_assembly2.cip = Hanheld_start_addr; // < коректируем cip >
										XEDParseAssemble(&pVMPX_req_context->xed_Predict_wait_assembly2); //<<< XEDParseAssemble >>>

										if ((asm_instruct_size == pVMPX_req_context->xed_Predict_wait_assembly2.dest_size) && (!memcmp(fpInstruction_pool, &pVMPX_req_context->xed_Predict_wait_assembly2.dest[0], asm_instruct_size)))
												codedef_save_seq_addr_from_ESP(Predict_wait_assembly2_addres)//save addr
									break;}
								case VMPX_CONTEXT_COMBINE_DYNAMIC__CS_PRESENT: //динамическая - присутсвует контрольный символ
									{
										pVMPX_req_context->xed_Predict_wait_assembly2.cip = Hanheld_start_addr; //AS arg = Hanheld_start_addr
										if (DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Release_normalize_instruction(pVMPX_req_context, VMPX_CONTEXT_ASM_PREDICT_WAIT_2, fpInstruction_pool, fpSeqESP) == true)
											codedef_save_seq_addr_from_ESP(Predict_wait_assembly2_addres)//save addr
									break;}
								}//end VMPX_CONTEXT_ASM_PREDICT_WAIT_2
							break;
							}//end VMPX_CONTEXT_ASM_PREDICT_WAIT_2


							case VMPX_CONTEXT_ASM_PREDICT_WAIT_3:
											// -- VMPX_CONTEXT_ASM_PREDICT_WAIT_3 -- //
							{
										// !МЕТОД СКАНИРОВАНИЯ ПОСЛЕДОВАТЕЛЬНОСТИ БАЙТ!
								switch(pVMPX_req_context->Quick_combine_type__Critical_control)
								{
								case VMPX_CONTEXT_COMBINE_STATIC__CS_PRESENT: //cтатическая - присутсвует контрольный символ
								case VMPX_CONTEXT_COMBINE_STATIC__NO_CS: //cтатическая - нет контрольных символов
									{
										if ((asm_instruct_size == pVMPX_req_context->xed_Predict_wait_assembly3.dest_size) && (!memcmp(fpInstruction_pool, &pVMPX_req_context->xed_Predict_wait_assembly3.dest[0], asm_instruct_size)))
												codedef_save_seq_addr_from_ESP(Predict_wait_assembly3_addres)//save addr
									break;}
								case VMPX_CONTEXT_COMBINE_DYNAMIC__NO_CS: //динамическая - нет контрольных символов
									{
										pVMPX_req_context->xed_Predict_wait_assembly3.cip = Hanheld_start_addr; // < коректируем cip >
										XEDParseAssemble(&pVMPX_req_context->xed_Predict_wait_assembly3); //<<< XEDParseAssemble >>>

										if ((asm_instruct_size == pVMPX_req_context->xed_Predict_wait_assembly3.dest_size) && (!memcmp(fpInstruction_pool, &pVMPX_req_context->xed_Predict_wait_assembly3.dest[0], asm_instruct_size)))
												codedef_save_seq_addr_from_ESP(Predict_wait_assembly3_addres);//save addr
									break;}
								case VMPX_CONTEXT_COMBINE_DYNAMIC__CS_PRESENT: //динамическая - присутсвует контрольный символ
									{
										pVMPX_req_context->xed_Predict_wait_assembly3.cip = Hanheld_start_addr; //AS arg = Hanheld_start_addr
										if (DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Release_normalize_instruction(pVMPX_req_context, VMPX_CONTEXT_ASM_PREDICT_WAIT_3, fpInstruction_pool, fpSeqESP) == true)
											codedef_save_seq_addr_from_ESP(Predict_wait_assembly3_addres);//save addr
									break;}
								}//end VMPX_CONTEXT_ASM_PREDICT_WAIT_3
							break;
							}//end VMPX_CONTEXT_ASM_PREDICT_WAIT_3


							case VMPX_CONTEXT_ASM_PREDICT_WAIT_4:
											// -- VMPX_CONTEXT_ASM_PREDICT_WAIT_4 -- //
							{
										// !МЕТОД СКАНИРОВАНИЯ ПОСЛЕДОВАТЕЛЬНОСТИ БАЙТ!
								switch(pVMPX_req_context->Quick_combine_type__Critical_control)
								{
								case VMPX_CONTEXT_COMBINE_STATIC__CS_PRESENT: //cтатическая - присутсвует контрольный символ
								case VMPX_CONTEXT_COMBINE_STATIC__NO_CS: //cтатическая - нет контрольных символов
									{
										if ((asm_instruct_size == pVMPX_req_context->xed_Predict_wait_assembly4.dest_size) && (!memcmp(fpInstruction_pool, &pVMPX_req_context->xed_Predict_wait_assembly4.dest[0], asm_instruct_size)))
												codedef_save_seq_addr_from_ESP(Predict_wait_assembly4_addres)//save addr
									break;}
								case VMPX_CONTEXT_COMBINE_DYNAMIC__NO_CS: //динамическая - нет контрольных символов
									{
										pVMPX_req_context->xed_Predict_wait_assembly4.cip = Hanheld_start_addr; // < коректируем cip >
										XEDParseAssemble(&pVMPX_req_context->xed_Predict_wait_assembly4); //<<< XEDParseAssemble >>>

										if ((asm_instruct_size == pVMPX_req_context->xed_Predict_wait_assembly4.dest_size) && (!memcmp(fpInstruction_pool, &pVMPX_req_context->xed_Predict_wait_assembly4.dest[0], asm_instruct_size)))
												codedef_save_seq_addr_from_ESP(Predict_wait_assembly4_addres);//save addr
									break;}
								case VMPX_CONTEXT_COMBINE_DYNAMIC__CS_PRESENT: //динамическая - присутсвует контрольный символ
									{
										pVMPX_req_context->xed_Predict_wait_assembly4.cip = Hanheld_start_addr; //AS arg = Hanheld_start_addr
										if (DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Release_normalize_instruction(pVMPX_req_context, VMPX_CONTEXT_ASM_PREDICT_WAIT_4, fpInstruction_pool, fpSeqESP) == true)
											codedef_save_seq_addr_from_ESP(Predict_wait_assembly4_addres);//save addr
									break;}
								}//end VMPX_CONTEXT_ASM_PREDICT_WAIT_4
							break;
							}//end VMPX_CONTEXT_ASM_PREDICT_WAIT_4


						case VMPX_CONTEXT_ASM_CRITICAL:
											// --__STATIC __-- //
							{
										// !МЕТОД СКАНИРОВАНИЯ ПОСЛЕДОВАТЕЛЬНОСТИ БАЙТ!
								switch(pVMPX_req_context->Quick_combine_type__Critical_control)
								{
								case VMPX_CONTEXT_COMBINE_STATIC__CS_PRESENT: //cтатическая - присутсвует контрольный символ
								case VMPX_CONTEXT_COMBINE_STATIC__NO_CS: //cтатическая - нет контрольных символов
									{
										if ((asm_instruct_size == pVMPX_req_context->xed_Critial_assembly.dest_size) && (!memcmp(fpInstruction_pool, &pVMPX_req_context->xed_Critial_assembly.dest[0], asm_instruct_size)))
												codedef_save_seq_addr_from_ESP(Critial_assembly_addres)//save addr
									break;}
								case VMPX_CONTEXT_COMBINE_DYNAMIC__NO_CS: //динамическая - нет контрольных символов
									{
										pVMPX_req_context->xed_Critial_assembly.cip = Hanheld_start_addr; // < коректируем cip >
										XEDParseAssemble(&pVMPX_req_context->xed_Critial_assembly); //<<< XEDParseAssemble >>>

										if ((asm_instruct_size == pVMPX_req_context->xed_Critial_assembly.dest_size) && (!memcmp(fpInstruction_pool, &pVMPX_req_context->xed_Critial_assembly.dest[0], asm_instruct_size)))
												codedef_save_seq_addr_from_ESP(Critial_assembly_addres);//save addr
									break;}
								case VMPX_CONTEXT_COMBINE_DYNAMIC__CS_PRESENT: //динамическая - присутсвует контрольный символ
									{
										pVMPX_req_context->xed_Critial_assembly.cip = Hanheld_start_addr; //AS arg = Hanheld_start_addr
										if (DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Release_normalize_instruction(pVMPX_req_context, VMPX_CONTEXT_ASM_CRITICAL, fpInstruction_pool, fpSeqESP) == true)
											codedef_save_seq_addr_from_ESP(Critial_assembly_addres);//save addr
									break;}
								}//end Quick_combine_type__Critical_control
							break;
							}//end VMPX_CONTEXT_ASM_CRITICAL


							case VMPX_CONTEXT_ASM_AFTER_1:
											// -- VMPX_CONTEXT_ASM_AFTER_1 -- //
							{
										// !МЕТОД СКАНИРОВАНИЯ ПОСЛЕДОВАТЕЛЬНОСТИ БАЙТ!
								switch(pVMPX_req_context->Quick_combine_type__Critical_control)
								{
								case VMPX_CONTEXT_COMBINE_STATIC__CS_PRESENT: //cтатическая - присутсвует контрольный символ
								case VMPX_CONTEXT_COMBINE_STATIC__NO_CS: //cтатическая - нет контрольных символов
									{
										if ((asm_instruct_size == pVMPX_req_context->xed_After_critical_assebly1.dest_size) && (!memcmp(fpInstruction_pool, &pVMPX_req_context->xed_After_critical_assebly1.dest[0], asm_instruct_size)))
												codedef_save_seq_addr_from_ESP(After_critical_assebly1_addres)//save addr
									break;}
								case VMPX_CONTEXT_COMBINE_DYNAMIC__NO_CS: //динамическая - нет контрольных символов
									{
										pVMPX_req_context->xed_After_critical_assebly1.cip = Hanheld_start_addr; // < коректируем cip >
										XEDParseAssemble(&pVMPX_req_context->xed_After_critical_assebly1); //<<< XEDParseAssemble >>>

										if ((asm_instruct_size == pVMPX_req_context->xed_After_critical_assebly1.dest_size) && (!memcmp(fpInstruction_pool, &pVMPX_req_context->xed_After_critical_assebly1.dest[0], asm_instruct_size)))
												codedef_save_seq_addr_from_ESP(After_critical_assebly1_addres);//save addr
									break;}
								case VMPX_CONTEXT_COMBINE_DYNAMIC__CS_PRESENT: //динамическая - присутсвует контрольный символ
									{
										pVMPX_req_context->xed_After_critical_assebly1.cip = Hanheld_start_addr; //AS arg = Hanheld_start_addr
										if (DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Release_normalize_instruction(pVMPX_req_context, VMPX_CONTEXT_ASM_AFTER_1, fpInstruction_pool, fpSeqESP) == true)
											codedef_save_seq_addr_from_ESP(After_critical_assebly1_addres);//save addr
									break;}
								}//end VMPX_CONTEXT_ASM_AFTER_1
							break;
							}//end VMPX_CONTEXT_ASM_AFTER_1


						case VMPX_CONTEXT_ASM_AFTER_2:
											// -- VMPX_CONTEXT_ASM_AFTER_2 -- //
							{
										// !МЕТОД СКАНИРОВАНИЯ ПОСЛЕДОВАТЕЛЬНОСТИ БАЙТ!
								switch(pVMPX_req_context->Quick_combine_type__Critical_control)
								{
								case VMPX_CONTEXT_COMBINE_STATIC__CS_PRESENT: //cтатическая - присутсвует контрольный символ
								case VMPX_CONTEXT_COMBINE_STATIC__NO_CS: //cтатическая - нет контрольных символов
									{
										if ((asm_instruct_size == pVMPX_req_context->xed_After_critical_assebly2.dest_size) && (!memcmp(fpInstruction_pool, &pVMPX_req_context->xed_After_critical_assebly2.dest[0], asm_instruct_size)))
												codedef_save_seq_addr_from_ESP(After_critical_assebly2_addres)//save addr
									break;}
								case VMPX_CONTEXT_COMBINE_DYNAMIC__NO_CS: //динамическая - нет контрольных символов
									{
										pVMPX_req_context->xed_After_critical_assebly2.cip = Hanheld_start_addr; // < коректируем cip >
										XEDParseAssemble(&pVMPX_req_context->xed_After_critical_assebly2); //<<< XEDParseAssemble >>>

										if ((asm_instruct_size == pVMPX_req_context->xed_After_critical_assebly2.dest_size) && (!memcmp(fpInstruction_pool, &pVMPX_req_context->xed_After_critical_assebly2.dest[0], asm_instruct_size)))
												codedef_save_seq_addr_from_ESP(After_critical_assebly2_addres);//save addr
									break;}
								case VMPX_CONTEXT_COMBINE_DYNAMIC__CS_PRESENT: //динамическая - присутсвует контрольный символ
									{
										pVMPX_req_context->xed_After_critical_assebly2.cip = Hanheld_start_addr; //AS arg = Hanheld_start_addr
										if (DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Release_normalize_instruction(pVMPX_req_context, VMPX_CONTEXT_ASM_AFTER_2, fpInstruction_pool, fpSeqESP) == true)
											codedef_save_seq_addr_from_ESP(After_critical_assebly2_addres);//save addr
									break;}
								}//end VMPX_CONTEXT_ASM_AFTER_2
							break;
							}//end VMPX_CONTEXT_ASM_AFTER_2
						default:
							{
							break;}


						}//++++++++++++++ end task switcher ++++++++++++++

							// *** PRIMARY CHECK _EXTRACT *** IF
							/*
							 SERVICE_COUNT Extract_len;
						 size_t quick_Extract_BEA_Opcode;
						 size_t quick_Extract_BEA_Category;
						 ARGTYPE* pQuick_Extract_BEA_Arg1;
						 ARGTYPE* pQuick_Extract_BEA_Arg2;*/
							if(Extract_len != NULL)
							{
								//ПРОВЕРЯЕМ НА _EXTRACT
								if ((Dissasm_struct->Instruction.Opcode == quick_Extract_BEA_Opcode) && (Dissasm_struct->Instruction.Category == quick_Extract_BEA_Category))
								{
									*pQuick_Extract_BEA_Arg1_displacement = Dissasm_struct->Argument1.Memory.Displacement; //copy Displacement - normal mismatch
									*pQuick_Extract_BEA_Arg2_displacement = Dissasm_struct->Argument2.Memory.Displacement; //copy Displacement - normal mismatch
									//if (!memcmp(&Dissasm_struct->Argument1, pQuick_Extract_BEA_Arg1, sizeof(ARGTYPE)) && !memcmp(&Dissasm_struct->Argument2, pQuick_Extract_BEA_Arg2, sizeof(ARGTYPE))) // all
									if (!memcmp(&Dissasm_struct->Argument1.ArgType, pQuick_Extract_BEA_Arg1_argtype, sizeof(ARGTYPE)-sizeof(Dissasm_struct->Argument1.ArgMnemonic)) && !memcmp(&Dissasm_struct->Argument2.ArgType, pQuick_Extract_BEA_Arg2_argtype, sizeof(ARGTYPE)-sizeof(Dissasm_struct->Argument1.ArgMnemonic))) //exclude mnemonic
											codedef_save_seq_extract_addr_from_ESP__no_task_change //fpSeqESP->EXTRACT_addres = Hanheld_start_addr; fpSeqESP->EXTRACT_extracted_value = Dissasm_struct->Instruction.AddrValue;
								}// end if (Dissasm_struct->Instruction.Opcode == quick_Extract_BEA_Opcode) && (Dissasm_struct->Instruction.Category == quick_Extract_BEA_Category)
								
								//ПРОВЕРЯЕМ НА _EXTRACT_VALUE_LEN - имеет смысл только при наличии _EXTRACT
								if(Extract_value_link_len != NULL)
								{
										if ((Dissasm_struct->Instruction.Opcode == quick_Extract_value_link_BEA_Opcode) && (Dissasm_struct->Instruction.Category == quick_Extract_value_link_BEA_Category) && (Dissasm_struct->Argument2.Memory.Displacement == Quick_Extract_value_link_BEA_Arg2_MemDisplacement))
										{
											// !МЕТОД СКАНИРОВАНИЯ ПОСЛЕДОВАТЕЛЬНОСТИ БАЙТ!
												switch(pVMPX_req_context->Quick_combine_type__Out_EXTRACT_VALUE_LINK)
												{
												case VMPX_CONTEXT_COMBINE_STATIC__CS_PRESENT: //cтатическая - присутсвует контрольный символ
													{
														pVMPX_req_context->xed_Out_EXTRACT_VALUE_LINK.cip = Hanheld_start_addr; //AS arg = Hanheld_start_addr
														if(DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Release_normalize_instruction(pVMPX_req_context, VMPX_CONTEXT_ASM_EXTRACT_VALUE_LINK, fpInstruction_pool, fpSeqESP) == true)
															{codedef_save_seq_extract_value_link_left_side_Bea_type; codedef_save_seq_extract_value_link_addr_from_ESP__no_task_change;}//save addr
													break;}
												case VMPX_CONTEXT_COMBINE_STATIC__NO_CS: //cтатическая - нет контрольных символов
													{
														if(!memcmp(&Dissasm_struct->Argument1, pQuick_Extract_value_link_BEA_Arg1, sizeof(Dissasm_struct->Argument1)) && (!memcmp(&Dissasm_struct->Argument2, pQuick_Extract_value_link_BEA_Arg2, sizeof(Dissasm_struct->Argument2))) && (!memcmp(&Dissasm_struct->Argument3, pQuick_Extract_value_link_BEA_Arg3, sizeof(Dissasm_struct->Argument3))))
															{codedef_save_seq_extract_value_link_left_side_Bea_type; codedef_save_seq_extract_value_link_addr_from_ESP__no_task_change;}
													break;}
												case VMPX_CONTEXT_COMBINE_DYNAMIC__NO_CS: //динамическая - нет контрольных символов
													{
														pVMPX_req_context->xed_Out_EXTRACT_VALUE_LINK.cip = Hanheld_start_addr; //AS arg = Hanheld_start_addr
														XEDParseAssemble(&pVMPX_req_context->xed_Out_EXTRACT_VALUE_LINK); //<<< XEDParseAssemble >>>

														if(!memcmp(fpInstruction_pool, &pVMPX_req_context->xed_Out_EXTRACT_VALUE_LINK.dest[0], asm_instruct_size))
															{codedef_save_seq_extract_value_link_left_side_Bea_type; codedef_save_seq_extract_value_link_addr_from_ESP__no_task_change;}
													break;}
												case VMPX_CONTEXT_COMBINE_DYNAMIC__CS_PRESENT: //динамическая - присутсвует контрольный символ
													{
														pVMPX_req_context->xed_Out_EXTRACT_VALUE_LINK.cip = Hanheld_start_addr; //AS arg = Hanheld_start_addr
															 if (DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Release_normalize_instruction(pVMPX_req_context, VMPX_CONTEXT_ASM_AFTER_1, fpInstruction_pool, fpSeqESP) == true)
																{codedef_save_seq_extract_value_link_left_side_Bea_type; codedef_save_seq_extract_value_link_addr_from_ESP__no_task_change;}//save addr
													break;}
												}//end switcher pVMPX_req_context->Quick_combine_type__Out_EXTRACT_VALUE_LINK
										}// end if (Dissasm_struct->Instruction.Opcode == quick_Extract_BEA_Opcode) && (Dissasm_struct->Instruction.Category == quick_Extract_BEA_Category)
								}//end if(Extract_value_link_len != NULL)
							}//end if (Extract_len != NULL)

							  //how bytes in stack reserved from VM (sub rsp, 0x??? --- add rsp, 0x???)
						if(Dissasm_struct->Instruction.Opcode == ASM_SUB_32bit)
						{
						codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|ARITHMETIC_INSTRUCTION);
			  
										if(Dissasm_struct->Argument1.ArgType == (REGISTER_TYPE|GENERAL_REG|REG4) &&  (Dissasm_struct->Argument2.ArgType == CONSTANT_TYPE|ABSOLUTE_) && (Dissasm_struct->Argument1.ArgSize == REGSIZE_64))
												{DH_VM_Output_32_bit_number_arithmetic_result_from_asm_instruction_mnemonic(Dissasm_struct->Instruction.Mnemonic,(signed long)Dissasm_struct->Instruction.Immediat, &fpSeqESP->Stack_RSP_Different);}
						break;}

				}//end CASE default

	
			}//end switch(Dissasm_struct->Instruction.Opcode)

			Hanheld_start_addr+=asm_instruct_size; fpInstruction_pool=(LPVOID)((ULONG64)fpInstruction_pool + (ULONG64)asm_instruct_size);
			}while(i--); //walk in instruction pool

		if (fpSeqESP->Total_found_addreses != NULL)
		{
			if (fpSeqESP->Critial_assembly_addres == NULL)
					codedef_imm_return_with_clearset

				fpSelectedScanTaskESP = &pVMPX_req_context->tAsmSearchTask_stack[0];
				register SERVICE_COUNT i = pVMPX_req_context->cAsmSearchTask_stack_count;
				fpSelectedScanTaskESP--;
				bool AllFound = true; //all found
				do
				{
					
					fpSelectedScanTaskESP++;
					switch(*fpSelectedScanTaskESP)
					{
						//PREDICT (4 CYCLES)
						case VMPX_CONTEXT_ASM_PREDICT_WAIT_1:
							{
								if (fpSeqESP->Predict_wait_assembly1_addres == NULL)
									codedef_imm_return_with_clearset
							break;} //end VMPX_CONTEXT_ASM_PREDICT_WAIT_1
						case VMPX_CONTEXT_ASM_PREDICT_WAIT_2:
							{
								if (fpSeqESP->Predict_wait_assembly1_addres == NULL)
									codedef_imm_return_with_clearset
							break;} //end VMPX_CONTEXT_ASM_PREDICT_WAIT_2
						case VMPX_CONTEXT_ASM_PREDICT_WAIT_3:
							{
								if (fpSeqESP->Predict_wait_assembly1_addres == NULL)
									codedef_imm_return_with_clearset
							break;} //end VMPX_CONTEXT_ASM_PREDICT_WAIT_3
						case VMPX_CONTEXT_ASM_PREDICT_WAIT_4:
							{
								if (fpSeqESP->Predict_wait_assembly4_addres == NULL)
									codedef_imm_return_with_clearset
							break;} //end VMPX_CONTEXT_ASM_PREDICT_WAIT_1

						//AFTER
						case VMPX_CONTEXT_ASM_AFTER_1:
							{
								if (fpSeqESP->After_critical_assebly1_addres == NULL)
									codedef_imm_return_with_clearset
							break;} //end VMPX_CONTEXT_ASM_AFTER_1
						case VMPX_CONTEXT_ASM_AFTER_2:
							{
								if (fpSeqESP->After_critical_assebly2_addres == NULL)
									codedef_imm_return_with_clearset
							break;} //end VMPX_CONTEXT_ASM_AFTER_1

					}//switch current task fpSelectedScanTaskESP
				}while(--i); //check all tasks
			pScanSequenceStack->fpSeqStackESP++;
			pScanSequenceStack->Found_sequences++;
			return TRUE;
		}//FILTER #enda
			
return NULL;
}


bool __fastcall DH_VM_Scan_VirtualMachine_Contexts__is_static_assembly(__in WCHAR* pTestContextAsm, __in XEDPARSE* pxed_1, __in XEDPARSE* pxed_2)
{
	
			pxed_1->cip = PREDICT_ADDR_DISTANCE; // != PREDICT_ADDR_CONST
			quick_UNICODE_to_ASCII(pTestContextAsm, &pxed_1->instr[0]);
			if (XEDParseAssemble(pxed_1) == XEDPARSE_ERROR) //XED parse STATIC №1
								return false; //error -BAD XEDPARSE №1
			

			pxed_2->cip = PREDICT_ADDR_CONST;
			quick_UNICODE_to_ASCII(pTestContextAsm, &pxed_2->instr[0]);
			if (XEDParseAssemble(pxed_2) == XEDPARSE_ERROR) //XED parse STATIC №1
								return false; //error -BAD XEDPARSE №2

			if((pxed_2->dest_size != pxed_1->dest_size) && (!memcmp(&pxed_1->dest[0], &pxed_2->dest[0], pxed_1->dest_size)))
			return false; //DYMANIC

return true;//STATIC
}


SERVICE_TYPE __fastcall DH_VM_Scan_VirtualMachine_Contexts__Research_Found_Sequence__without_save(__in DISASM* Dissasm_struct, TARGET_PROC_REGION_COPY* Req_all_VM_insruction_region, __in pVM_VMPX_PROFILE_INI pVMPX_req_context)
{
			 SERVICE_ADDRES Hanheld_start_addr =  Dissasm_struct->VirtualAddr; //extract VA
			 register PVOID fpInstruction_pool =  (PVOID)Dissasm_struct->EIP; //extract pointer in the allocated pool
			 register size_t i = AVERAGE_DEEP_ASM_IN_HANHELD; //how DEEP hanheld iterations
			 register size_t asm_instruct_size = NULL; //out asm size
			 register size_t Safe_max_bound =  (ULONG64)Req_all_VM_insruction_region->allocated_pool + Req_all_VM_insruction_region->region_size;
			 LPBYTE instruction_pool = Req_all_VM_insruction_region->allocated_pool;

			 
			 //enumerate found asm//
			 bool is_found_Predict1_asm = false;
			 bool is_found_Predict2_asm = false;
			 bool is_found_Predict3_asm = false;
			 bool is_found_Predict4_asm = false;
			 bool is_found_Critical_asm = false;
			 bool is_found_After1_asm = false;
			 bool is_found_After2_asm = false;

			 SERVICE_COUNT Switched_predict_wait_count = pVMPX_req_context->Predict_wait_count;
			 SERVICE_COUNT Switched_After_count = pVMPX_req_context->After_count;

			 //ВЫБОР ПЕРВОЙ СКАНИРУЕМОЙ ИНСТРУКЦИИ (VMPX_CONTEXT_TYPE_ASMS)
			 SERVICE_TYPE* fpSelectedScanTaskESP = &pVMPX_req_context->tAsmSearchTask_stack[0];
			 register SERVICE_TYPE CurrentSelectedScanTask = *fpSelectedScanTaskESP;
			 SERVICE_COUNT cSelectedScanTasks_count = pVMPX_req_context->cAsmSearchTask_stack_count;

			 do
			 {
				 if((ULONG64)fpInstruction_pool >= Safe_max_bound || (ULONG64)fpInstruction_pool < (ULONG64)instruction_pool )
					 break;

				Dissasm_struct->EIP = (UIntPtr)fpInstruction_pool;
				Dissasm_struct->VirtualAddr = Hanheld_start_addr; //handler start addr
				asm_instruct_size = Disasm(Dissasm_struct); //disasm!!!

		//		if(Hanheld_start_addr == 0x000000144C4EE84)
			//		Beep(0,0);
	
						//modified VARIATOR START  /////////////////////////////////////
				switch(Dissasm_struct->Instruction.Opcode)
				{
						//%%%%%%% CONTROL_TRANSFER (RET & JMP & CALL) %%%%%%%%%%//
				case ASM_RET_0xBytes: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); codedef_set_condition_for_abort_variator_cycle; break;}
				case ASM_RET: {codedef_set_condition_for_abort_variator_cycle; break;} 
				case ASM_JMP_CALL_reg: {
					codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER);

					if (Dissasm_struct->Instruction.BranchType == JmpType) //select JMP reg (END of HANHELD!!! TYPE = REACH_HANHELD_END) 
									{codedef_set_condition_for_abort_variator_cycle;} 
								if (Dissasm_struct->Instruction.BranchType == RetType) //ret (spec condition)
									{ codedef_set_condition_for_abort_variator_cycle;} //codedef_imm_return();//EXIT VM - END !!!!!!!!!!!!!!!!!!!
					break;} //jmp reg & call reg
				case ASM_JMP_CONST_SHORT:
				case ASM_JMP_CONST: {
					if (Dissasm_struct->Instruction.AddrValue != NULL) //jmp CONST_address
							{codedef_calculate_hanheld_jmp_continue;asm_instruct_size = NULL;}
							
					break;} 
				case ASM_CALL_OPERAND:{codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); if(Dissasm_struct->Instruction.AddrValue - Hanheld_start_addr < NEAR_JMPS_TRICK){codedef_calculate_hanheld_jmp_continue;asm_instruct_size = NULL;}break;}
									//jmps inside hanheld
					// ************ JUMPOUTs ************ //
				/*		
				case ASM_branch_JO: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); break;}
				case ASM_branch_JNO: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); break;}
				case ASM_branch_JB:  {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); break;}
				case ASM_branch_JAE: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); break;}
				case ASM_branch_JE: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); break;}
				case ASM_branch_JNE: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); break;}
				case ASM_branch_JBE: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); break;}
				case ASM_branch_JA: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); break;}
				case ASM_branch_JS: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); break;}
				case ASM_branch_JNS:{codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER);break;}
				case ASM_branch_JP: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER);  break;}
				case ASM_branch_JNP: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); break;}
				case ASM_branch_JL: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); break;}
				case ASM_branch_JGE: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER);  break;}
				case ASM_branch_JLE: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); break;}
				case ASM_branch_JG: {codedef_category_filter(GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER); break;}
				*/
				default:
					{
						if (Dissasm_struct->Instruction.Category == (GENERAL_PURPOSE_INSTRUCTION|CONTROL_TRANSFER))
								break;
						if ((Dissasm_struct->Instruction.BranchType != NULL))
								break;
						
						// ____________ PARSE STEP-BYTE-STEP MODE _____________ //

			switch(CurrentSelectedScanTask) //}//++++++++++++++ //}//++++++++++++++
				{
					case VMPX_CONTEXT_ASM_PREDICT_WAIT_1:
									// -- VMPX_CONTEXT_ASM_PREDICT_WAIT_1 -- //
					{
								// !МЕТОД СКАНИРОВАНИЯ ПОСЛЕДОВАТЕЛЬНОСТИ БАЙТ!
						switch(pVMPX_req_context->Quick_combine_type__Critical_control)
						{
						case VMPX_CONTEXT_COMBINE_STATIC__CS_PRESENT: //cтатическая - присутсвует контрольный символ
						case VMPX_CONTEXT_COMBINE_STATIC__NO_CS: //cтатическая - нет контрольных символов
							{
								if ((asm_instruct_size == pVMPX_req_context->xed_Predict_wait_assembly1.dest_size) && !memcmp(fpInstruction_pool, &pVMPX_req_context->xed_Predict_wait_assembly1.dest[0], asm_instruct_size))
										is_found_Predict1_asm = true;
							break;}
						case VMPX_CONTEXT_COMBINE_DYNAMIC__NO_CS: //динамическая - нет контрольных символов
							{
								pVMPX_req_context->xed_Predict_wait_assembly1.cip = Hanheld_start_addr; // < коректируем cip >
								XEDParseAssemble(&pVMPX_req_context->xed_Predict_wait_assembly1); //<<< XEDParseAssemble >>>

								if ((asm_instruct_size == pVMPX_req_context->xed_Predict_wait_assembly1.dest_size) && !memcmp(fpInstruction_pool, &pVMPX_req_context->xed_Predict_wait_assembly1.dest[0], asm_instruct_size))
										is_found_Predict1_asm = true;
							break;}
						case VMPX_CONTEXT_COMBINE_DYNAMIC__CS_PRESENT: //динамическая - присутсвует контрольный символ
							{
								pVMPX_req_context->xed_Predict_wait_assembly1.cip = Hanheld_start_addr; //AS arg = Hanheld_start_addr
								if (DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Release_normalize_instruction__NO_WRITE(pVMPX_req_context, VMPX_CONTEXT_ASM_PREDICT_WAIT_1, fpInstruction_pool) == true)
									is_found_Predict1_asm = true;
							break;}
						}//end VMPX_CONTEXT_ASM_PREDICT_WAIT_1
					break;
					}//end VMPX_CONTEXT_ASM_PREDICT_WAIT_1


					case VMPX_CONTEXT_ASM_PREDICT_WAIT_2:
									// -- VMPX_CONTEXT_ASM_PREDICT_WAIT_2 -- //
					{
								// !МЕТОД СКАНИРОВАНИЯ ПОСЛЕДОВАТЕЛЬНОСТИ БАЙТ!
						switch(pVMPX_req_context->Quick_combine_type__Critical_control)
						{
						case VMPX_CONTEXT_COMBINE_STATIC__CS_PRESENT: //cтатическая - присутсвует контрольный символ
						case VMPX_CONTEXT_COMBINE_STATIC__NO_CS: //cтатическая - нет контрольных символов
							{
								if ((asm_instruct_size == pVMPX_req_context->xed_Predict_wait_assembly2.dest_size) && (!memcmp(fpInstruction_pool, &pVMPX_req_context->xed_Predict_wait_assembly2.dest[0], asm_instruct_size)))
										is_found_Predict2_asm = true;
							break;}
						case VMPX_CONTEXT_COMBINE_DYNAMIC__NO_CS: //динамическая - нет контрольных символов
							{
								pVMPX_req_context->xed_Predict_wait_assembly2.cip = Hanheld_start_addr; // < коректируем cip >
								XEDParseAssemble(&pVMPX_req_context->xed_Predict_wait_assembly2); //<<< XEDParseAssemble >>>

								if ((asm_instruct_size == pVMPX_req_context->xed_Predict_wait_assembly2.dest_size) && (!memcmp(fpInstruction_pool, &pVMPX_req_context->xed_Predict_wait_assembly2.dest[0], asm_instruct_size)))
										is_found_Predict2_asm = true;
							break;}
						case VMPX_CONTEXT_COMBINE_DYNAMIC__CS_PRESENT: //динамическая - присутсвует контрольный символ
							{
								pVMPX_req_context->xed_Predict_wait_assembly2.cip = Hanheld_start_addr; //AS arg = Hanheld_start_addr
								if (DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Release_normalize_instruction__NO_WRITE(pVMPX_req_context, VMPX_CONTEXT_ASM_PREDICT_WAIT_2, fpInstruction_pool) == true)
									is_found_Predict2_asm = true;
							break;}
						}//end VMPX_CONTEXT_ASM_PREDICT_WAIT_2
					break;
					}//end VMPX_CONTEXT_ASM_PREDICT_WAIT_2


					case VMPX_CONTEXT_ASM_PREDICT_WAIT_3:
									// -- VMPX_CONTEXT_ASM_PREDICT_WAIT_3 -- //
					{
								// !МЕТОД СКАНИРОВАНИЯ ПОСЛЕДОВАТЕЛЬНОСТИ БАЙТ!
						switch(pVMPX_req_context->Quick_combine_type__Critical_control)
						{
						case VMPX_CONTEXT_COMBINE_STATIC__CS_PRESENT: //cтатическая - присутсвует контрольный символ
						case VMPX_CONTEXT_COMBINE_STATIC__NO_CS: //cтатическая - нет контрольных символов
							{
								if ((asm_instruct_size == pVMPX_req_context->xed_Predict_wait_assembly3.dest_size) && (!memcmp(fpInstruction_pool, &pVMPX_req_context->xed_Predict_wait_assembly3.dest[0], asm_instruct_size)))
										is_found_Predict3_asm = true;
							break;}
						case VMPX_CONTEXT_COMBINE_DYNAMIC__NO_CS: //динамическая - нет контрольных символов
							{
								pVMPX_req_context->xed_Predict_wait_assembly3.cip = Hanheld_start_addr; // < коректируем cip >
								XEDParseAssemble(&pVMPX_req_context->xed_Predict_wait_assembly3); //<<< XEDParseAssemble >>>

								if ((asm_instruct_size == pVMPX_req_context->xed_Predict_wait_assembly3.dest_size) && (!memcmp(fpInstruction_pool, &pVMPX_req_context->xed_Predict_wait_assembly3.dest[0], asm_instruct_size)))
										is_found_Predict3_asm = true;
							break;}
						case VMPX_CONTEXT_COMBINE_DYNAMIC__CS_PRESENT: //динамическая - присутсвует контрольный символ
							{
								pVMPX_req_context->xed_Predict_wait_assembly3.cip = Hanheld_start_addr; //AS arg = Hanheld_start_addr
								if (DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Release_normalize_instruction__NO_WRITE(pVMPX_req_context, VMPX_CONTEXT_ASM_PREDICT_WAIT_3, fpInstruction_pool) == true)
									is_found_Predict3_asm = true;
							break;}
						}//end VMPX_CONTEXT_ASM_PREDICT_WAIT_3
					break;
					}//end VMPX_CONTEXT_ASM_PREDICT_WAIT_3


					case VMPX_CONTEXT_ASM_PREDICT_WAIT_4:
									// -- VMPX_CONTEXT_ASM_PREDICT_WAIT_4 -- //
					{
								// !МЕТОД СКАНИРОВАНИЯ ПОСЛЕДОВАТЕЛЬНОСТИ БАЙТ!
						switch(pVMPX_req_context->Quick_combine_type__Critical_control)
						{
						case VMPX_CONTEXT_COMBINE_STATIC__CS_PRESENT: //cтатическая - присутсвует контрольный символ
						case VMPX_CONTEXT_COMBINE_STATIC__NO_CS: //cтатическая - нет контрольных символов
							{
								if ((asm_instruct_size == pVMPX_req_context->xed_Predict_wait_assembly4.dest_size) && (!memcmp(fpInstruction_pool, &pVMPX_req_context->xed_Predict_wait_assembly4.dest[0], asm_instruct_size)))
										is_found_Predict4_asm = true;
							break;}
						case VMPX_CONTEXT_COMBINE_DYNAMIC__NO_CS: //динамическая - нет контрольных символов
							{
								pVMPX_req_context->xed_Predict_wait_assembly4.cip = Hanheld_start_addr; // < коректируем cip >
								XEDParseAssemble(&pVMPX_req_context->xed_Predict_wait_assembly4); //<<< XEDParseAssemble >>>

								if ((asm_instruct_size == pVMPX_req_context->xed_Predict_wait_assembly4.dest_size) && (!memcmp(fpInstruction_pool, &pVMPX_req_context->xed_Predict_wait_assembly4.dest[0], asm_instruct_size)))
										is_found_Predict4_asm = true;
							break;}
						case VMPX_CONTEXT_COMBINE_DYNAMIC__CS_PRESENT: //динамическая - присутсвует контрольный символ
							{
								pVMPX_req_context->xed_Predict_wait_assembly4.cip = Hanheld_start_addr; //AS arg = Hanheld_start_addr
								if (DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Release_normalize_instruction__NO_WRITE(pVMPX_req_context, VMPX_CONTEXT_ASM_PREDICT_WAIT_4, fpInstruction_pool) == true)
									is_found_Predict4_asm = true;
							break;}
						}//end VMPX_CONTEXT_ASM_PREDICT_WAIT_4
					break;
					}//end VMPX_CONTEXT_ASM_PREDICT_WAIT_4


				case VMPX_CONTEXT_ASM_CRITICAL:
									// --__STATIC __-- //
					{
								// !МЕТОД СКАНИРОВАНИЯ ПОСЛЕДОВАТЕЛЬНОСТИ БАЙТ!
						switch(pVMPX_req_context->Quick_combine_type__Critical_control)
						{
						case VMPX_CONTEXT_COMBINE_STATIC__CS_PRESENT: //cтатическая - присутсвует контрольный символ
						case VMPX_CONTEXT_COMBINE_STATIC__NO_CS: //cтатическая - нет контрольных символов
							{
								if ((asm_instruct_size == pVMPX_req_context->xed_Critial_assembly.dest_size) && (!memcmp(fpInstruction_pool, &pVMPX_req_context->xed_Critial_assembly.dest[0], asm_instruct_size)))
										is_found_Critical_asm = true;
							break;}
						case VMPX_CONTEXT_COMBINE_DYNAMIC__NO_CS: //динамическая - нет контрольных символов
							{
								pVMPX_req_context->xed_Critial_assembly.cip = Hanheld_start_addr; // < коректируем cip >
								XEDParseAssemble(&pVMPX_req_context->xed_Critial_assembly); //<<< XEDParseAssemble >>>

								if ((asm_instruct_size == pVMPX_req_context->xed_Critial_assembly.dest_size) && (!memcmp(fpInstruction_pool, &pVMPX_req_context->xed_Critial_assembly.dest[0], asm_instruct_size)))
										is_found_Critical_asm = true;
							break;}
						case VMPX_CONTEXT_COMBINE_DYNAMIC__CS_PRESENT: //динамическая - присутсвует контрольный символ
							{
								pVMPX_req_context->xed_Critial_assembly.cip = Hanheld_start_addr; //AS arg = Hanheld_start_addr
								if (DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Release_normalize_instruction__NO_WRITE(pVMPX_req_context, VMPX_CONTEXT_ASM_CRITICAL, fpInstruction_pool) == true)
										is_found_Critical_asm = true;
							break;}
						}//end Quick_combine_type__Critical_control
					break;
					}//end VMPX_CONTEXT_ASM_CRITICAL


					case VMPX_CONTEXT_ASM_AFTER_1:
									// -- VMPX_CONTEXT_ASM_AFTER_1 -- //
					{
								// !МЕТОД СКАНИРОВАНИЯ ПОСЛЕДОВАТЕЛЬНОСТИ БАЙТ!
						switch(pVMPX_req_context->Quick_combine_type__Critical_control)
						{
						case VMPX_CONTEXT_COMBINE_STATIC__CS_PRESENT: //cтатическая - присутсвует контрольный символ
						case VMPX_CONTEXT_COMBINE_STATIC__NO_CS: //cтатическая - нет контрольных символов
							{
								if ((asm_instruct_size == pVMPX_req_context->xed_After_critical_assebly1.dest_size) && (!memcmp(fpInstruction_pool, &pVMPX_req_context->xed_After_critical_assebly1.dest[0], asm_instruct_size)))
										is_found_After1_asm = true;
							break;}
						case VMPX_CONTEXT_COMBINE_DYNAMIC__NO_CS: //динамическая - нет контрольных символов
							{
								pVMPX_req_context->xed_After_critical_assebly1.cip = Hanheld_start_addr; // < коректируем cip >
								XEDParseAssemble(&pVMPX_req_context->xed_After_critical_assebly1); //<<< XEDParseAssemble >>>

								if ((asm_instruct_size == pVMPX_req_context->xed_After_critical_assebly1.dest_size) && (!memcmp(fpInstruction_pool, &pVMPX_req_context->xed_After_critical_assebly1.dest[0], asm_instruct_size)))
										is_found_After1_asm = true;
							break;}
						case VMPX_CONTEXT_COMBINE_DYNAMIC__CS_PRESENT: //динамическая - присутсвует контрольный символ
							{
								pVMPX_req_context->xed_After_critical_assebly1.cip = Hanheld_start_addr; //AS arg = Hanheld_start_addr
								if (DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Release_normalize_instruction__NO_WRITE(pVMPX_req_context, VMPX_CONTEXT_ASM_AFTER_1, fpInstruction_pool) == true)
										is_found_After1_asm = true;
							break;}
						}//end VMPX_CONTEXT_ASM_AFTER_1
					break;
					}//end VMPX_CONTEXT_ASM_AFTER_1


				case VMPX_CONTEXT_ASM_AFTER_2:
									// -- VMPX_CONTEXT_ASM_AFTER_2 -- //
					{
								// !МЕТОД СКАНИРОВАНИЯ ПОСЛЕДОВАТЕЛЬНОСТИ БАЙТ!
						switch(pVMPX_req_context->Quick_combine_type__Critical_control)
						{
						case VMPX_CONTEXT_COMBINE_STATIC__CS_PRESENT: //cтатическая - присутсвует контрольный символ
						case VMPX_CONTEXT_COMBINE_STATIC__NO_CS: //cтатическая - нет контрольных символов
							{
								if ((asm_instruct_size == pVMPX_req_context->xed_After_critical_assebly2.dest_size) && (!memcmp(fpInstruction_pool, &pVMPX_req_context->xed_After_critical_assebly2.dest[0], asm_instruct_size)))
												is_found_After2_asm = true;
							break;}
						case VMPX_CONTEXT_COMBINE_DYNAMIC__NO_CS: //динамическая - нет контрольных символов
							{
								pVMPX_req_context->xed_After_critical_assebly2.cip = Hanheld_start_addr; // < коректируем cip >
								XEDParseAssemble(&pVMPX_req_context->xed_After_critical_assebly2); //<<< XEDParseAssemble >>>

								if ((asm_instruct_size == pVMPX_req_context->xed_After_critical_assebly2.dest_size) && (!memcmp(fpInstruction_pool, &pVMPX_req_context->xed_After_critical_assebly2.dest[0], asm_instruct_size)))
											   is_found_After2_asm = true;
							break;}
						case VMPX_CONTEXT_COMBINE_DYNAMIC__CS_PRESENT: //динамическая - присутсвует контрольный символ
							{
								pVMPX_req_context->xed_After_critical_assebly2.cip = Hanheld_start_addr; //AS arg = Hanheld_start_addr
								if (DH_Profile_PARSE_Arbitary_VMPX_CONTEXT_CONFIG__Release_normalize_instruction__NO_WRITE(pVMPX_req_context, VMPX_CONTEXT_ASM_AFTER_2, fpInstruction_pool) == true)
									is_found_After2_asm = true;
							break;}
						}//end VMPX_CONTEXT_ASM_AFTER_2
					break;
					}//end VMPX_CONTEXT_ASM_AFTER_2
				default:
					{
					break;}

				}//++++++++++++++ end task switcher ++++++++++++++

					}//end CASE default
				}//end switch(Dissasm_struct->Instruction.Opcode)
			Hanheld_start_addr+=asm_instruct_size; fpInstruction_pool=(LPVOID)((ULONG64)fpInstruction_pool + (ULONG64)asm_instruct_size);
			}while(i--); //walk in instruction pool


			

				fpSelectedScanTaskESP = &pVMPX_req_context->tAsmSearchTask_stack[0];
				 i = pVMPX_req_context->cAsmSearchTask_stack_count;

				fpSelectedScanTaskESP--;
				bool AllFound = true; //all found
				do
				{
					
					fpSelectedScanTaskESP++;
					switch(*fpSelectedScanTaskESP)
					{
						//PREDICT (4 CYCLES)
						case VMPX_CONTEXT_ASM_PREDICT_WAIT_1:
							{
								if (is_found_Predict1_asm == false)
										return NULL;
							break;} //end VMPX_CONTEXT_ASM_PREDICT_WAIT_1
						case VMPX_CONTEXT_ASM_PREDICT_WAIT_2:
							{
								if (is_found_Predict2_asm == false)
									return NULL;
							break;} //end VMPX_CONTEXT_ASM_PREDICT_WAIT_2
						case VMPX_CONTEXT_ASM_PREDICT_WAIT_3:
							{
								if (is_found_Predict3_asm == false)
									return NULL;
							break;} //end VMPX_CONTEXT_ASM_PREDICT_WAIT_3
						case VMPX_CONTEXT_ASM_PREDICT_WAIT_4:
							{
								if (is_found_Predict4_asm == false)
									return NULL;
							break;} //end VMPX_CONTEXT_ASM_PREDICT_WAIT_1

							case VMPX_CONTEXT_ASM_CRITICAL:
							{
								if (is_found_Critical_asm == false)
									return NULL;
							break;} //end VMPX_CONTEXT_ASM_CRITICAL

						//AFTER
						case VMPX_CONTEXT_ASM_AFTER_1:
							{
								if (is_found_After1_asm == false)
									return NULL;
							break;} //end VMPX_CONTEXT_ASM_AFTER_1
						case VMPX_CONTEXT_ASM_AFTER_2:
							{
								if (is_found_After2_asm == false)
									return NULL;
							break;} //end VMPX_CONTEXT_ASM_AFTER_1

					}//switch current task fpSelectedScanTaskESP
				}while(--i); //check all tasks
			return TRUE;
		
}

SERVICE_COUNT __fastcall DH_VM_Scan_VirtualMachine_Contexts__Mark_Duplicate_contexts_from_EXTRACT(__in pVM_VMPX_PROFILE_FOUND_SEQUENCE_STACK pSeqStack)
{
	pSeqStack->fpSeqStackESP = &pSeqStack->Found_Seq[0]; //set Seq ESP NULL pointer
	register pVM_VMPX_PROFILE_FOUND_SEQUENCE fpSeqESP = pSeqStack->fpSeqStackESP;
	register pVM_VMPX_PROFILE_FOUND_SEQUENCE fpSeqESP_slave;
	register SERVICE_ADDRES CurrentUniqAddres;
	SERVICE_COUNT Duplicate_result = NULL; //результат - кол-во уникальных контекстов
	size_t FoundSeq_local = pSeqStack->Found_sequences;
	register size_t i = FoundSeq_local; //found seq
	register size_t ii;
	fpSeqESP--;
	do
	{
		//*fpUniqStack = fpSeqESP->EXTRACT_extracted_value; //save addr
		//fpUniqStack++;
		fpSeqESP++;
		CurrentUniqAddres = fpSeqESP->EXTRACT_extracted_value;
		if(CurrentUniqAddres == NULL)
			continue; //primary
		if(fpSeqESP->isDuplicateContext == true)
			continue; //primary

			ii = FoundSeq_local;
			fpSeqESP_slave = &pSeqStack->Found_Seq[0];
			fpSeqESP_slave--;
			do
			{
			fpSeqESP_slave++;

				if(i == ii) //eq position
					continue; //slave
				if (fpSeqESP_slave->EXTRACT_extracted_value != CurrentUniqAddres) //not eq addreses
					continue; //slave
				if(fpSeqESP_slave->isDuplicateContext == true)
					continue; //primary

				//found duplicate addres
				fpSeqESP_slave->isDuplicateContext = true;
				Duplicate_result++; //inc 
			}while(--ii); //slave cycle
	}while(--i);//main cycle

pSeqStack->Duplicate_from_EXTRACT = Duplicate_result; //save result
return Duplicate_result;
}


SERVICE_ERROR __fastcall DH_VMPX_Create_VM_Profile_MANUAL(__in pVM_Manual_input pManual_VM_Info)
{
	//поиск и выделение региона с виртуальной машиной
	PIMAGE_SECTION_HEADER pSection_header_ref;
	register pREGION_COPY Target_Section_all_VM = DH_SPIRUN7_Take_Region_Snapshot_by_addres((SERVICE_ADDRES)pManual_VM_Info->supply_15_start_address, REGION_TYPE_ALL_SECTION_INCLUDE_VM,&pSection_header_ref);

	if (Target_Section_all_VM == NULL)//фильтр №1
		return false;
	
	//выделение памяти под новый профиль
	register pVMPX_VM_PROFILE_INFO_STACK pNew_VMPX_profile = (pVMPX_VM_PROFILE_INFO_STACK)::VirtualAlloc(NULL, def_VMPX_vmpx_2_stack_size, MEM_COMMIT, PAGE_READWRITE);

	if (pNew_VMPX_profile == NULL) //фильтр №2
		{free(Target_Section_all_VM->allocated_pool); free(Target_Section_all_VM); return false;}

	register pVM_AsmAddr_POOL_STACK pNew_VMPX_profile__address_POOL = (pVM_AsmAddr_POOL_STACK)((SERVICE_ADDRES)pNew_VMPX_profile + def_VMPX_vmpx_2_instr_pool_offset);
	pNew_VMPX_profile->ofs_VM_hanheld_addres_pool = (SERVICE_ADDRES)pNew_VMPX_profile__address_POOL - (SERVICE_ADDRES)pNew_VMPX_profile;
	pNew_VMPX_profile->ofs_1Containt_handler = def_VMPX_vmpx_2_HANHELD_offset;
	pNew_VMPX_profile->retail_VM_profile_stack_size = def_VMPX_vmpx_2_stack_size;

	pNew_VMPX_profile->supply_15_start_address = (CELL_ADDRES)pManual_VM_Info->supply_15_start_address;
	pNew_VMPX_profile->VMPX_version = pManual_VM_Info->VMPX_version;
	pNew_VMPX_profile->Stack_RSP_Different = 0;
	pNew_VMPX_profile->table_DEcode_instruction = pManual_VM_Info->table_DEcode_instruction;
	pNew_VMPX_profile->table_DEcode_instruction_operand = pManual_VM_Info->table_DEcode_instruction_operand;
	pNew_VMPX_profile->Stack_RSP_Different = pManual_VM_Info->Stack_RSP_Different;
	if(pSection_header_ref != NULL)
		memcpy(&pNew_VMPX_profile->RegionSectionInfo, pSection_header_ref, sizeof(IMAGE_SECTION_HEADER));
	wcscpy(&pNew_VMPX_profile->vm_name[0], &pManual_VM_Info->VM_name[0]);

	//дамп закодированной таблицы хендлов supply15
	if ((pManual_VM_Info->handles_count == 0) || (pManual_VM_Info->handles_count > LIMIT_HANDLES_ON_SUPPLY))
		pManual_VM_Info->handles_count = LIMIT_HANDLES_ON_SUPPLY;
	if (!DH_ReadProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE, (LPVOID)pNew_VMPX_profile->supply_15_start_address, sizeof(HANHELD_ADDRES)*pManual_VM_Info->handles_count, (LPVOID)&pNew_VMPX_profile->original_hanhelds_table_crypted[0]))
		{free(Target_Section_all_VM->allocated_pool); free(Target_Section_all_VM); ::VirtualFree(pNew_VMPX_profile, def_VMPX_vmpx_2_stack_size, MEM_RELEASE); return VMPX_ERROR_SUPPLY15_MISS_READ;}
	//вторая копия для расшифровки адресов
	memcpy(&pNew_VMPX_profile->original_hanhelds_table_decrypted[0], &pNew_VMPX_profile->original_hanhelds_table_crypted[0], sizeof(pNew_VMPX_profile->original_hanhelds_table_crypted));

	//попытка расшифровки таблицы хендлов найденным методом шифрования
	if (pNew_VMPX_profile->table_DEcode_instruction)
		DH_VM_Supply15_DEcode_256_table(pNew_VMPX_profile->table_DEcode_instruction, pNew_VMPX_profile->table_DEcode_instruction_operand, &pNew_VMPX_profile->original_hanhelds_table_decrypted[0]);

	//имеем ли дело с релоками. восстановление RVA от ImageBase.
	if (HUNT_HIVE.Is_req_relocation)
		DH_VM_Supply15_Relocation_256_table(&pNew_VMPX_profile->original_hanhelds_table_decrypted[0]);

	//контрольное чтение расшифрованных данных из декодированной таблицы
	size_t Read_data;
	if (!DH_ReadProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE, (LPVOID)*&pNew_VMPX_profile->original_hanhelds_table_decrypted[0], sizeof(Read_data), (LPVOID)&Read_data))
		{free(Target_Section_all_VM->allocated_pool); free(Target_Section_all_VM); ::VirtualFree(pNew_VMPX_profile, def_VMPX_vmpx_2_stack_size, MEM_RELEASE); return VMPX_ERROR_SUPPLY15_VALID_CRYPTO_METHOD_NOT_FOUND;}

	//ИДЕНТИФИКАЦИЯ ХЕНДЛОВ
	DISASM Dissasm_struct;
	Dissasm_struct.Archi = 64;
	SERVICE_ERROR err_hanhelds_type = DH_VM_Supply15_Get_HANHELD_type_256_table(pNew_VMPX_profile, Target_Section_all_VM, pNew_VMPX_profile__address_POOL, &Dissasm_struct);
	if (err_hanhelds_type != NULL)
	{free(Target_Section_all_VM->allocated_pool); free(Target_Section_all_VM); ::VirtualFree(pNew_VMPX_profile, def_VMPX_vmpx_2_stack_size, MEM_RELEASE); return err_hanhelds_type;}


	//окончательное освобождение выделенной памяти
	free(Target_Section_all_VM->allocated_pool); 
	free(Target_Section_all_VM);

	TCHAR MT_PROFILE_PATH[MAX_PATH * 2];
	register TCHAR* pMT_PROFILE_PATH_0ptr = &MT_PROFILE_PATH[0];
	wcscpy(&MT_PROFILE_PATH[0], &HUNT_HIVE.PROFILE_PATH[0]);
	bool error_write = false;
	if ((pNew_VMPX_profile->is_vm_billet == false) && (pNew_VMPX_profile->identeficated_handles_count != NULL))
		error_write = DH_Write_Current_hunt_profile_CONTAINER_MT(PROFILE_TYPE_VM, &pNew_VMPX_profile->vm_name[0], (BYTE*)pNew_VMPX_profile, pNew_VMPX_profile->retail_VM_profile_stack_size, pMT_PROFILE_PATH_0ptr);
	else
		error_write = DH_Write_Current_hunt_profile_CONTAINER_MT(PROFILE_TYPE_VM_BILLET, &pNew_VMPX_profile->vm_name[0], (BYTE*)pNew_VMPX_profile, pNew_VMPX_profile->retail_VM_profile_stack_size, pMT_PROFILE_PATH_0ptr);

	::VirtualFree(pNew_VMPX_profile, def_VMPX_vmpx_2_stack_size, MEM_RELEASE);

	if (error_write == false)
		return VMPX_ERROR_BAD_PROFILE_WRITE;

	return 0;
}

WCHAR* __fastcall DH_VM_ERROR_type_to_string(SERVICE_ERROR VMPX_Error)
{
	switch (VMPX_Error)
	{
	case VMPX_ERROR_SUPPLY15_MISS_READ: {return L"SUPPLY15_MISS_READ"; break; }
	case VMPX_ERROR_SUPPLY15_VALID_CRYPTO_METHOD_NOT_FOUND: {return L"MISS_CRYPTO_METHOD"; break; }
	case VMPX_ERROR_SUPPLY15_VMPX_CONTEXT_NOT_READY: {return L"VMP_CONTEXT_NOT_READY"; break; }
	case VMPX_ERROR_BAD_PROFILE_WRITE: {return L"BAD_WRITE_FILE"; break; }
	case VMPX_ERROR_HANHELD_LOW_IDENTIFY_PERCENT: {return L"LOW_IDENTIFY_PERCENT"; break; }
	}
	return TS_DEF_NULL;
}

WCHAR* __fastcall DH_VM_FOUND_RESULT_type_to_string(SERVICE_TYPE Found_type)
{
	switch (Found_type)
	{
	case VM_FOUND_SUPPLY15: {return L"SUPPLY15"; break; }
	case VM_FOUND_SUPPLY15_CELL: {return L"SUPPLY15_CELL"; break; }
	case VM_FOUND_ADDRES_POOL_PRIMARY_ASSEMBLY: {return L"HANHELD_PRIMARY_ASM"; break; }
	case VM_FOUND_ADDRES_POOL_MID_ASSEMBLY: {return L"HANHELD_MID_ASM"; break; }
	case VM_FOUND_TYPEDEF_HANHELD: {return L"TYPEDEF_HANHELD"; break; }
	}
	return TS_DEF_NULL;
}