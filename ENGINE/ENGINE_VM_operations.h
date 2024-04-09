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

#include "ENGINE_VM_CONTEXT.h"
#include "ENGINE_Profile_operations.h"
#include "ENGINE_SPIRUN7_Communication.h"

#include "../BeaEngine/headers/BeaEngine.h" //BEA dissasm engine


#include "../XED/XEDParse.h"

#include "emmintrin.h"

//------------------ define data ------------------ 
#define VM_HANDLES_MAX_COUNT 260 //300

#define MAX_SUPPLYES 500
#define LIMIT_HANDLES_ON_SUPPLY 256

#define MAX_ASM_INSTUCTION_POOL 450 //количество асм инструкций в одном юните пула

#define MAX_COMMENTS_LEN 100 //макс. длинна комметария

#define MAX_DEEP_ASM_IN_HANHELD 200 //максимальная глубина (кол-во асм инструкций) для исследования "островка" (если не найден выход)
#define AVERAGE_DEEP_ASM_IN_HANHELD (MAX_DEEP_ASM_IN_HANHELD/2)-1 //"средняя" глубина (кол-во асм инструкций) для исследования "островка" (если не найден выход)
#define MAX_DEEP_ASM_JUMPOUTS_HANHELD  MAX_DEEP_ASM_IN_HANHELD/8 //max КОЛ-ВО условных втвлений (JO, JS, JNS, JG...) в островке
#define MAX_DEEP_CRYPTO_ASM_IN_HANHELD 10 //максимальная глубина (кол-во асм инструкций) от ссылки с использованием Supply15 до конца "островка"

#define MAX_HANHELD_TYPE_DETERMINE_CRITICAL_ADDRESES 10 //макс. кол-во критических адресов(для определения типа), определеямых в одном островке

#define MAX_IMMEDIANT_FROM_SHIFT_CALC 16 //макс. значение иммедианты асм инструкции для рассчёта сдвига

#define MAX_FOUND_REGION_IN_SEQ 20 //максимально допустимое количество найденных цепочек при исследовании всех VM_ENTER в указанном регионе

#define MAX_VM_ENTERs_FOR_ONE_CONTEXT 100 //максимально допустимое VM_ENTER, которые используют один и тот-же контекст

#define MNEMONIC_DWORD_add 0x20646461
#define MNEMONIC_DWORD_sub 0x20627573
#define MNEMONIC_DWORD_inc 0x20636E69
#define MNEMONIC_DWORD_dec 0x20636564
#define MNEMONIC_DWORD_not 0x20746F6E
#define MNEMONIC_DWORD_neg 0x2067656E
#define MNEMONIC_DWORD_rol 0x206C6F72
#define MNEMONIC_DWORD_ror 0x20726F72

// Supply 15 - DECODE/ENCODE instructions from table
#define S__SERVICE_NODECODE 0
#define S__INC 1
#define S__DEC 2
#define S__XOR 3
#define S__NOT 4
#define S__NEG 5
#define S__ADD 6
#define S__SUB 7
#define S__BSWAP 8
#define S__ROL 9
#define S__ROR 10

static WCHAR TS__INC[] = L"INC";
static WCHAR TS__DEC[] = L"DEC";
static WCHAR TS__XOR[] = L"XOR";
static WCHAR TS__NOT[] = L"NOT";
static WCHAR TS__NEG[] = L"NEG";
static WCHAR TS__ADD[] = L"ADD";
static WCHAR TS__SUB[] = L"SUB";
static WCHAR TS__BSWAP[] = L"BSWAP";
static WCHAR TS__ROL[] = L"ROL";
static WCHAR TS__ROR[] = L"ROR";


// VM type
#define VM_TYPE_VMP2 2
#define VM_TYPE_VMP3 3
#define VM_TYPE_VMX_D4 4
#define VM_TYPE_VMX_D5 5
#define VM_TYPE_ANOTHER 1
#define VM_TYPE_UNKNOWN VM_TYPE_ANOTHER

// VM type
#define TS_VM_TYPE_VMP2 L"VMP 2.x"
#define TS_VM_TYPE_VMP3 L"VMP 3.x"
#define TS_VM_TYPE_D4 L"Denuvo 4.x"
#define TS_VM_TYPE_D5 L"Denuvo 5.x"
#define TS_VM_TYPE_ANOTHER L"not determine"
#define TS_VM_TYPE_UNKNOWN VM_TYPE_ANOTHER

/////////////////asm instruct OPCODE//////////////////
#define ASM_LEA 0x0000008d
#define ASM_RDTSC 0x00000f31
#define ASM_CPUID 0xfa2 //cpuid

#define ASM_MOV_READ_reg8_reg64 0x8a //mov al, [rax]
#define ASM_MOV_READ_reg_reg64 0x8b //mov ax, [rax]; mov rax, [rax]
#define ASM_MOV_WRITE_reg64_reg32 0x89 //mov [rax], edx
#define ASM_MOVS_byte 0xA4 //movsb byte ptr [rdi], byte ptr [rsi] (rep/repe)
#define ASM_MOVS_dword 0xA5 //movsd dwrod ptr [rdi], dwrod ptr [rsi] (rep/repe)
#define ASM_MOV_WRITE_reg64_reg8 0x88 //mov byte ptr [rax], dl

#define ASM_XOR_dword_stack_reg 0x31 //xor dword ptr [rsp], eax
#define ASM_XOR_r8_r64 0x32 //xor al, [rdx]

#define ASM_ADD_8bit 0x83
#define ASM_SUB_8bit ASM_ADD_8bit
#define ASM_ADD_32bit 0x81
#define ASM_SUB_32bit ASM_ADD_32bit
#define ASM_ADD_mem 0x0000001
#define ASM_ADD_mem_byte 0x0000000

#define ASM_INC 0xff //INC REG_64
#define ASM_DEC ASM_INC //DEC REG_64

#define ASM_SHRD 0xfad //shrd eax, edx, cl
#define ASM_SHLD 0xfa5 //shld eax, edx, cl

#define ASM_DIV_mem 0xf7 //DIV

#define ASM_AND_mem 0x21

#define ASM_branch_JO	0x0f80
#define ASM_branch_JNO	0x0f81
#define ASM_branch_JB	0x0f82
#define ASM_branch_JAE	0x0f83
#define ASM_branch_JE	0x0f84
#define ASM_branch_JNE	0x0f85
#define ASM_branch_JBE	0x0f86
#define ASM_branch_JA	0x0f87
#define ASM_branch_JS	0x0f88
#define ASM_branch_JNS	0x0f89
#define ASM_branch_JP	0x0f8A
#define ASM_branch_JNP	0x0f8B
#define ASM_branch_JL	0x0f8C
#define ASM_branch_JGE	0x0f8D
#define ASM_branch_JLE	0x0f8E
#define ASM_branch_JG	0x0f8F

#define ASM_RET_0xBytes 0xc2 //ret with STACK pointer shift
#define ASM_RET 0xc3 //ret
#define ASM_JMP_CALL_reg 0xff //jmp & call
#define ASM_JMP_CONST 0xe9 //JMP 0x111111
#define ASM_JMP_CONST_SHORT 0xeb //JMP 0x111111
// asm instruct - JUMPOUTs
#define ASM_JA 0xf87
#define ASM_JG 0xf8f
#define ASM_CALL_OPERAND 0xe8 //call $+(near)
// ASM - LEA
#define ASM_LEA_A 0x8d

// -- supply15 decrypt asm instructions --
#define ASM_BSWAP_REG_rax_eax_r8 0xfc8
#define ASM_BSWAP_REG_rcx_ecx_r9 0xfc9
#define ASM_BSWAP_REG_rdx_edx_r10 0xfca
#define ASM_BSWAP_REG_rbx_ebx_r11 0xfcb
#define ASM_BSWAP_REG_rsp_esp_r12 0xfcc
#define ASM_BSWAP_REG_rbp_ebp_r13 0xfcd
#define ASM_BSWAP_REG_rsi_esi_r14 0xfce
#define ASM_BSWAP_REG_rdi_edi_r15 0xfcf
//#define ASM_ADD_8bit 0x83
//#define ASM_SUB_8bit ASM_ADD_8bit
//#define ASM_ADD_32bit 0x81
//#define ASM_SUB_32bit ASM_ADD_32bit
//#define ASM_INC 0xff //INC REG_64
//#define ASM_DEC ASM_INC //DEC REG_64
#define ASM_NEG_REG 0x0f7
#define ASM_NOT_REG ASM_NEG_REG
#define ASM_XOR_REG_qword ASM_ADD_32bit
#define ASM_XOR_REG_short ASM_ADD_8bit
#define ASM_NEG_ROL 0x0d1
#define ASM_NEG_ROR 0x0c1
//POP x64 registers (default VM_EXIT)
#define ASM_POP_RAX_r8 0x58
#define ASM_POP_RBX_r9 0x5B
#define ASM_POP_RCX 0x59
#define ASM_POP_RDX_r10 0x5A
#define ASM_POP_RBP_r13 0x5D
#define ASM_POP_RSI_r14 0x5E
#define ASM_POP_RDI_r15 0x5F


//register sizes
#define REGSIZE_8 8
#define REGSIZE_16 16
#define REGSIZE_32 32
#define REGSIZE_64 64

#define NEAR_JMPS_TRICK 40
#define SAFE_SKIP_CONST 5

#define WAITED_ADDR -1

#define def_VMPX_vmpx_2_stack_size sizeof(VMPX_VM_PROFILE_INFO_STACK) + (sizeof(HANHELD[VM_HANDLES_MAX_COUNT])) + sizeof(VM_hanheld_addres_pool[VM_HANDLES_MAX_COUNT])
#define def_VMPX_vmpx_2_HANHELD_offset sizeof(VMPX_VM_PROFILE_INFO_STACK)
#define def_VMPX_vmpx_2_instr_pool_offset sizeof(VMPX_VM_PROFILE_INFO_STACK) + (sizeof(HANHELD[VM_HANDLES_MAX_COUNT]))

#define HANHELD_IDENTIFY_CRITICAL_LOW_BORDER 5
#define APPROX_SAFE_ASM_INSTR_COUNT 16 //действительное расстояние по одной инструкции между двумя соседними адресами

#define FOUND_ASSUME_IN_ALL_HANHELDS -1 //найденный адрес применяектся ко всем хендлам (Supply15)

//#define def_VMPX_vmpx_2_instr_pool_size sizeof(VMPX_VM_PROFILE_INFO_STACK) + (sizeof(HANHELD)*VM_HANDLES_MAX_COUNT)

//#define HANHELD_TYPE_ENUMS_COUNT 70
//HANHELD_TYPE
/*
enum HANHELD_TYPE_ENUMS {HANHELD_TYPE_DETERMINE_ERROR = 0, HANHELD_TYPE_VM_ENTER, HANHELD_TYPE_SWITCH_ENTER, HANHELD_TYPE_VM_EXIT, HANHELD_TYPE_CALL, HANHELD_TYPE_RDTSC, HANHELD_TYPE_CPUID,
	HANHELD_TYPE_XOR_stack_reg,
	HANHELD_TYPE_XOR_byte_from_memreg64, 
	//mov DS
	HANHELD_TYPE_MOV_reg8_memreg64_DS, //MOV AL, [RDX]
	HANHELD_TYPE_MOV_reg16_memreg64_DS, //MOV AX, [RAX]
	HANHELD_TYPE_MOV_reg32_memreg64_DS, //MOV EAX, [RAX]
	HANHELD_TYPE_MOV_reg64_memreg64_DS, //MOV RAX, [RAX]
	HANHELD_TYPE_MOV_memreg64_reg8_DS, //mov dword ptr [rax], dl
	HANHELD_TYPE_MOV_memreg64_reg16_DS, //mov dword ptr [rax], dx
	HANHELD_TYPE_MOV_memreg64_reg32_DS, //mov dword ptr [rax], edx
	HANHELD_TYPE_MOV_memreg64_reg64_DS, //mov dword ptr [rax], rdx

	HANHELD_TYPE_MOV_read_qword_PCODE, //mov rax, qword ptr [rsi]
	HANHELD_TYPE_MOV_read_dword_PCODE, //mov eax, qword ptr [rsi]
	HANHELD_TYPE_MOV_read_word_PCODE, //mov ax, qword ptr [rsi]

	// mov SS
	HANHELD_TYPE_MOV_reg8_memreg64_SS, //MOV AL, [RDX]
	HANHELD_TYPE_MOV_reg16_memreg64_SS, //MOV AX, [RAX]
	HANHELD_TYPE_MOV_reg32_memreg64_SS, //MOV EAX, [RAX]
	HANHELD_TYPE_MOV_reg64_memreg64_SS, //MOV RAX, [RAX]
	HANHELD_TYPE_MOV_memreg64_reg8_SS, //mov dword ptr [rax], dl
	HANHELD_TYPE_MOV_memreg64_reg16_SS, //mov dword ptr [rax], dx
	HANHELD_TYPE_MOV_memreg64_reg32_SS, //mov dword ptr [rax], edx
	HANHELD_TYPE_MOV_memreg64_reg64_SS, //mov dword ptr [rax], rdx
	//mov GS
	HANHELD_TYPE_MOV_reg8_memreg64_GS, //MOV AL, [RDX]
	HANHELD_TYPE_MOV_reg16_memreg64_GS, //MOV AX, [RAX]
	HANHELD_TYPE_MOV_reg32_memreg64_GS, //MOV EAX, [RAX]
	HANHELD_TYPE_MOV_reg64_memreg64_GS, //MOV RAX, [RAX]
	HANHELD_TYPE_MOV_memreg64_reg8_GS, //mov dword ptr [rax], dl
	HANHELD_TYPE_MOV_memreg64_reg16_GS, //mov dword ptr [rax], dx
	HANHELD_TYPE_MOV_memreg64_reg32_GS, //mov dword ptr [rax], edx
	HANHELD_TYPE_MOV_memreg64_reg64_GS, //mov dword ptr [rax], rdx

	HANHELD_TYPE_MOV_struct_reg8, //mov dword ptr [rax + rdi], dl
	HANHELD_TYPE_MOV_struct_reg16, //mov dword ptr [rax + rdi], dx
	HANHELD_TYPE_MOV_struct_reg32, //mov dword ptr [rax + rdi], edx
	HANHELD_TYPE_MOV_struct_reg64, //mov dword ptr [rax + rdi], rdx
	HANHELD_TYPE_MOV_reg8_struct, //mov dl, byte ptr [rax + rdi]
	HANHELD_TYPE_MOV_reg16_struct, //mov al, word ptr [rax + rdi]
	HANHELD_TYPE_MOV_reg32_struct, //mov edx, dword ptr [rax + rdi]
	HANHELD_TYPE_MOV_reg64_struct, //mov rdx, qword ptr [rax + rdi]

	//SHIFT_ROT
	HANHELD_TYPE_SHRD_64, //shrd rax, rdx, cl
	HANHELD_TYPE_SHLD_64, //shrd rax, rdx, cl
	HANHELD_TYPE_SHRD_32, //shrd eax, edx, cl
	HANHELD_TYPE_SHLD_32, //shld eax, edx, cl
	HANHELD_TYPE_SHRD_16, //shrd ax, dx, cl
	HANHELD_TYPE_SHLD_16, //shld ax, dx, cl

	//DIV
	HANHELD_TYPE_DIV_qword_stack, //div qword ptr [rbp + 0x10]
	HANHELD_TYPE_DIV_dword_stack, //div dword ptr [rbp + 0xC]

	//ADD
	HANHELD_TYPE_ADD_qword_stack, //add dword ptr [rbp + 8], rax
	HANHELD_TYPE_ADD_dword_stack, //add dword ptr [rbp + 8], eax
	HANHELD_TYPE_ADD_word_stack, //add dword ptr [rbp + 8], ax
	HANHELD_TYPE_ADD_byte_stack, //add byte ptr [rbp + 8], al

	//AND
	HANHELD_TYPE_and_qword_stack, //and dword ptr [rbp + 8], rax
	HANHELD_TYPE_and_dword_stack, //and dword ptr [rbp + 8], eax
	HANHELD_TYPE_and_word_stack, //and dword ptr [rbp + 8], ax
	HANHELD_TYPE_and_byte_stack, //and dword ptr [rbp + 8], ax

	HANHELD_TYPE_MOVS_byte, //MOVSB - movzx eax, byte ptr [rsi], mov [rbp], rax
	HANHELD_TYPE_MOVS_dword, //MOVSD


	//special SERVICE
	HANHELD_TYPE_HANHELD,

	HANHELD_TYPE_REACH_HANHELD_END = -1
};*/
enum VMPX_CONTEXT_ERRORS
{
	VMPX_ERROR_SUPPLY15_MISS_READ = -7, //ОШИБКА ПРИ ЧТЕНИИ ТАБЛИЦЫ SUPPLY15
	VMPX_ERROR_SUPPLY15_VALID_CRYPTO_METHOD_NOT_FOUND, //ОШИБКА ПРИ ОПРЕДЕЛЕНИИ КРИПТОМЕТОДА ДЛЯ SUPPLY15 - РАСШИФРОВАННЫЕ АДРЕСА НЕ ЧИТАЮТСЯ
	VMPX_ERROR_SUPPLY15_VMPX_CONTEXT_NOT_READY, //ОШИБКА ПРИ ОПРЕДЕЛЕНИИ ТИПА ХЕНДЛОВ "ОСТРОВКОВ" - КОНТЕКСТ НЕ ГОТОВ
	VMPX_ERROR_BAD_PROFILE_WRITE, //ОШИБКА ПРИ ЗАПИСИ ФАЙЛА ПРОФИЛЯ
	VMPX_ERROR_HANHELD_LOW_IDENTIFY_PERCENT //ОШИБКА - КРАЙНЕ МАЛЫЙ ПРОЦЕНТ РАСПОЗНАННЫХ ПРИМИТИВОВ (< define HANHELD_IDENTIFY_CRITICAL_LOW_BORDER)
};

enum JUMPOUT_FUNCTION_TYPE_ENUMS {JUMPOUT_TYPE_BRANCH_DOUBLE = 1, JUMPOUT_TYPE_CYCLE, JUMPOUT_TYPE_COMPARE};

enum asm_instruction_unconditional_FLOW_CONTROL_MARKER {FLOW_CONTROL_MARKER_JUMP_CONST = 1, FLOW_CONTROL_MARKER_JUMP_REG, FLOW_CONTROL_MARKER_RET, FLOW_CONTROL_MARKER_RETURN_ADDR};

enum VMPX_CONTEXT_TYPE_ASMS
{
	VMPX_CONTEXT___END = 0,
	VMPX_CONTEXT_ASM_PREDICT_WAIT_1 = 1,
	VMPX_CONTEXT_ASM_PREDICT_WAIT_2,
	VMPX_CONTEXT_ASM_PREDICT_WAIT_3,
	VMPX_CONTEXT_ASM_PREDICT_WAIT_4,
	VMPX_CONTEXT_ASM_CRITICAL,
	VMPX_CONTEXT_ASM_AFTER_1,
	VMPX_CONTEXT_ASM_AFTER_2,
	VMPX_CONTEXT_ASM_STATIC,
	VMPX_CONTEXT_ASM_EXTRACT,
	VMPX_CONTEXT_ASM_EXTRACT_VALUE_LINK
};

enum VMPX_CONTEXT_COMBINE_TYPE_ASMS
{
	VMPX_CONTEXT_COMBINE_STATIC__NO_CS = 1, //СТАТИЧЕСКАЯ + НЕТ СИМВОЛОВ В СТРОКЕ
	VMPX_CONTEXT_COMBINE_STATIC__CS_PRESENT, //СТАТИЧЕСКАЯ + КОНТРОЛЬНЫЙ СИМВОЛ

	VMPX_CONTEXT_COMBINE_DYNAMIC__NO_CS, //ДИНАМИЧЕСКАЯ + НЕТ СИМВОЛОВ В СТРОКЕ
	VMPX_CONTEXT_COMBINE_DYNAMIC__CS_PRESENT //ДИНАМИЧЕСКАЯ + КОНТРОЛЬНЫЙ СИМВОЛ
};

enum CONTROL_SYMBOL_SIZE__REGISTER
{
	CS_REGISTER_TYPEF_SIZE_QWORD = 1, //8 BYTES
	CS_REGISTER_TYPEF_SIZE_DWORD, //4 BYTES
	CS_REGISTER_TYPEF_SIZE_WORD, //2 BYTES
	CS_REGISTER_TYPEF_SIZE_BYTE //1 BYTE
};

enum TYPEDEF_INCREMENTAL_STANDY_MODE
{
	INREMENTAL_LOCK = 0, //НЕ производить инкрементацию. Заблокировано!  (по дефолту)
	INREMENTAL_EXECUTE, ////икремент взведен 
	INREMENTAL_RESET, //сбросить инкремент на начальное значение
};

enum DH_VM_FOUND_TYPE
{
	VM_FOUND_SUPPLY15 = 1, //ХРАНИЛИЩЕ №1,5
	VM_FOUND_SUPPLY15_CELL, //ЯЧЕЙКА ХРАНИЛИЩА №1,5
	VM_FOUND_ADDRES_POOL_PRIMARY_ASSEMBLY, //АДРЕС, ЗАПИСАННЫЙ В ПУЛЕ. ТОЧНОЕ СОВПАДЕНИЕ
	VM_FOUND_ADDRES_POOL_MID_ASSEMBLY, //АДРЕС, ЗАПИСАННЫЙ В ПУЛЕ. ПОПАДАЕТ В СЕРЕДИНУ ИНСТРУКЦИИ
	VM_FOUND_TYPEDEF_HANHELD //ТИП ХЕНДЛА
};

enum VM_Object_text_ref_TYPE
{
	VM_OBJECT_TYPE_REF__Addres_type = 1,
	VM_OBJECT_TYPE_REF__Hanheld_type
};


// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& code definitions &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
#define codedef_imm_return(ret_code) free(Dissasm_struct); return(ret_code)
#define codedef_set_condition_for_abort_variator_cycle	i=NULL
#define codedef_set_condition_for_abort_variator_SUBcycle	ii=NULL
#define codedef_set_condition_for_one_step_variator_cycle	i=1
#define codedef_convert_proc_rva_to_my_flow_pointer(ta, rs, al) (((ta)-(rs)) + (al)) //плавающий указатель по скопированному региону (секции) с вм
#define codedef_conditional_imm_return__when_found_type if(FOUND_htype != NULL) {free(Dissasm_struct); return (FOUND_htype);}
#define codedef_calculate_hanheld_jmp_continue fpInstruction_pool = (PVOID)((Dissasm_struct->Instruction.AddrValue-Req_all_VM_insruction_region->region_start_addres)+instruction_pool);   Hanheld_start_addr = Dissasm_struct->Instruction.AddrValue
#define codedef_set_HANHELD_shift_nubmers Research_hanheld->PCODE_Shift = PCODE_shift; Research_hanheld->stack_RBP_Shift = RBP_shift
#define codedef_category_filter(x) if(Dissasm_struct->Instruction.Category != (x)) break
#define codedef_quick_category_filter(x) if(quick_instruction_category != (x)) break
#define codedef_set_jumpout_table_element 	if (Research_hanheld->critical_jumpouts_array_count != MAX_DEEP_ASM_JUMPOUTS_HANHELD) {Research_hanheld->critical_jumpouts_array[Research_hanheld->critical_jumpouts_array_count].jumpout_type_as_BeaEngine_BRANCH_TYPE = Dissasm_struct->Instruction.BranchType; Research_hanheld->critical_jumpouts_array[Research_hanheld->critical_jumpouts_array_count].GRAPH_INIT_asm_instruction = Hanheld_start_addr; Research_hanheld->critical_jumpouts_array[Research_hanheld->critical_jumpouts_array_count].GRAPH_START = Dissasm_struct->Instruction.AddrValue; Research_hanheld->critical_jumpouts_array_count++;}
#define codedef_set_FLOW_CONTROL_marker(X_TYPE) 	Req_VM_hanheld_instruction_addr_pool->asm_instruction_unconditional_FLOW_CONTROL_MARKER[Req_VM_hanheld_instruction_addr_pool->HANHELD_asm_instruction_count-1] = X_TYPE
#define codedef_set_FLOW_CONTROL_RETURN_WAYPOINT(WAYPOINT_ADDRESS) 	Req_VM_hanheld_instruction_addr_pool->asm_instruction_unconditional_FLOW_CONTROL_MARKER_dest_addres[Req_VM_hanheld_instruction_addr_pool->HANHELD_asm_instruction_count-1] = (WAYPOINT_ADDRESS)
#define codedef_set_and_INCREMENT_FLOW_CONTROL_marker(X_TYPE) 	Req_VM_hanheld_instruction_addr_pool->asm_instruction_unconditional_FLOW_CONTROL_MARKER[Req_VM_hanheld_instruction_addr_pool->HANHELD_asm_instruction_count++] = X_TYPE
#define codedef_get_upper_table_first_free_cell(crit_i)	do{if (Research_hanheld->determine_type_addres_array[(crit_i)] == NULL) break;}while((crit_i)--)
#define codedef_set_FOUND_type_with_save_addres(ftype) FOUND_htype = (ftype); Research_hanheld->determine_type_addres_array[(crit_addr_i)] = Dissasm_struct->VirtualAddr;  Research_hanheld->determine_type_addres_array__HANHELD_in_AddrPool_ID[(crit_addr_i)] = Req_VM_hanheld_instruction_addr_pool->HANHELD_asm_instruction_count;  Research_hanheld->HANHELD_type_asm_disasm_simple_info.Category = Dissasm_struct->Instruction.Category; Research_hanheld->HANHELD_type_asm_disasm_simple_info.Opcode = Dissasm_struct->Instruction.Opcode; memcpy(&Research_hanheld->HANHELD_type_asm_disasm_simple_info.CompleteInstr[0], &Dissasm_struct->CompleteInstr[0], INSTRUCT_LENGTH) //crit_addr_i use
#define codedef_save_instruction_addres_only Research_hanheld->determine_type_addres_array[(crit_addr_i)] = Dissasm_struct->VirtualAddr; Research_hanheld->determine_type_addres_array__HANHELD_in_AddrPool_ID[(crit_addr_i)] = Req_VM_hanheld_instruction_addr_pool->HANHELD_asm_instruction_count
#define codedef_condition_set_FOUND_type_on_indefinite_situation(ftype) if(FOUND_htype == NULL) FOUND_htype = ftype
#define codedef_condition_set_GRAPH_circuit_closing(desd_addr) if(Req_VM_hanheld_instruction_addr_pool->asm_instruction_addres[asm_pool_instruction_count] == (desd_addr)) {Research_hanheld->critical_jumpouts_array[i_jouts].GRAPH_END = (desd_addr); codedef_condition_set_JUMPOUT_function_type_primary(JUMPOUT_TYPE_CYCLE); break;}
#define codedef_condition_set_JUMPOUT_function_type(jumpout_func_type_enum) Research_hanheld->critical_jumpouts_array[i_jouts-1].jumpout_function_as_VMProtect_definition = (jumpout_func_type_enum)
#define codedef_condition_set_JUMPOUT_function_type_primary(jumpout_func_type_enum) Research_hanheld->critical_jumpouts_array[i_jouts].jumpout_function_as_VMProtect_definition = (jumpout_func_type_enum)
#define codedef_set_critical_determine_address_array_count(crit_i) ((crit_i)) = MAX_HANHELD_TYPE_DETERMINE_CRITICAL_ADDRESES-1; do{if (Research_hanheld->determine_type_addres_array[((crit_i))] == NULL) break;}while((crit_i)--); Research_hanheld->determine_address_array_count = MAX_HANHELD_TYPE_DETERMINE_CRITICAL_ADDRESES - (++(crit_i))
#define codedef_JUMPOUTs_scan_fast__return ((Research_hanheld->critical_jumpouts_array_count - Old_critical_jumpouts_array_count))
#define codedef_get_upper_table_first_not_definition_cell(jumpout_i) ULONG f=NULL; do{if (HANHELD_ADDRES Hanheld_start_addr = Research_hanheld->critical_jumpouts_array[f].jumpout_function_as_VMProtect_definition == NULL) break; f++;}while((jumpout_i)--); jumpout_i = f
#define codedef_set_jumpout_func_as_vmprot_def(x_type) Research_hanheld->critical_jumpouts_array[i_jumpouts].jumpout_function_as_VMProtect_definition = (x_type)
#define codedef_set_GRAPH_circuit_closing(desd_addr) Research_hanheld->critical_jumpouts_array[i_jumpouts].GRAPH_END = (desd_addr)
#define codedef_check_JMP_desdonation_address_include_on_addr_pool(true_codition_execute_code) 	ii_jouts = Req_VM_hanheld_instruction_addr_pool->HANHELD_asm_instruction_count; do { if (Req_VM_hanheld_instruction_addr_pool->asm_instruction_addres[ii_jouts] == Hanheld_start_addr) {true_codition_execute_code break;} }while(ii_jouts--);
#define codedef_set_previously_found_type(previously_type) Research_hanheld->HANDELD_previously_type = (previously_type)
#define codedef_check_type_state_and_set_previously_found_type(previously_type) Research_hanheld->HANDELD_previously_type = (previously_type)
#define codedef_save_seq_addr_from_ESP(struct_member) {fpSeqESP->struct_member = Hanheld_start_addr; fpSeqESP->Total_found_addreses++; fpSelectedScanTaskESP++; CurrentSelectedScanTask = *fpSelectedScanTaskESP;}
#define codedef_save_seq_extract_addr_from_ESP__no_task_change {fpSeqESP->EXTRACT_addres = Hanheld_start_addr; fpSeqESP->EXTRACT_extracted_value = Dissasm_struct->Instruction.AddrValue;}
#define codedef_save_seq_extract_value_link_addr_from_ESP__no_task_change fpSeqESP->EXTRACT_VALUE_LINK_address = Hanheld_start_addr
#define codedef_save_seq_extract_value_link_left_side_Bea_type fpSeqESP->Bea_EXTRACT_VALUE_LINK__left_side_type = Dissasm_struct->Argument1.ArgType; fpSeqESP->Bea_EXTRACT_VALUE_LINK__left_Arg_size = Dissasm_struct->Argument1.ArgSize; fpSeqESP->Bea_EXTRACT_VALUE_LINK__asm_size = asm_instruct_size
#define codedef_set_wait_mode_seq_addr_from_ESP(struct_member) fpSeqESP->struct_member = WAITED_ADDR
#define codedef_imm_return_with_clearset {memset(fpSeqESP, 0, sizeof(VM_VMPX_PROFILE_FOUND_SEQUENCE)); return NULL;}
#define codedef_virtual_freeex__seq_stack(_region_start) ::VirtualFreeEx(DH_Current_process_fast_handle, (_region_start), 0, MEM_RELEASE)
#define codedef_LOCK_VMPX_taskwork LOCK_VMPX_intrepret_taskwork = true
#define codedef_inc_current_taskwork *(fpTaskWorkSearcher_stack) = *(fpTaskWorkSearcher_stack) + 1
#define codedef_hanheld_set_PREV_TYPE_with_check_collisions_and_taskwork_endlist(X_type) if(**fpTaskWorkSearcher_stack == VMPX_CONTEXT___END)\
																				{\
																					if(fpContaint_handler->HANDELD_previously_type == NULL)\
																						{fpContaint_handler->pVMPX_profile_reference = pVMPX_req_context; fpContaint_handler->HANDELD_previously_type__VMPX_asm_count = pVMPX_req_context->Assembly_count; fpContaint_handler->HANDELD_previously_type = (X_type); *fpDTAA_critical_addres = Hanheld_start_addr; fpContaint_handler->determine_address_array_count = 1; break;} \
																					else\
																						{\
																							if(fpContaint_handler->HANDELD_previously_type__VMPX_asm_count <= pVMPX_req_context->Assembly_count)\
																								{fpContaint_handler->pVMPX_profile_reference = pVMPX_req_context; fpContaint_handler->HANDELD_previously_type__VMPX_asm_count = pVMPX_req_context->Assembly_count; fpContaint_handler->HANDELD_previously_type = (X_type); *fpDTAA_critical_addres = Hanheld_start_addr; fpContaint_handler->determine_address_array_count = 1; break;}\
																						}}else{\
																						*fpLastCritical_Corrector_stack = Hanheld_start_addr;}
#define codedef_hanheld_set_PREV_TYPE_with_check_collisions_and_taskwork_endlist__after(X_type) if(**fpTaskWorkSearcher_stack == VMPX_CONTEXT___END)\
																				{\
																					if(fpContaint_handler->HANDELD_previously_type == NULL)\
																						{fpContaint_handler->pVMPX_profile_reference = pVMPX_req_context; fpContaint_handler->HANDELD_previously_type__VMPX_asm_count = pVMPX_req_context->Assembly_count; fpContaint_handler->HANDELD_previously_type = (X_type); *fpDTAA_critical_addres = *fpLastCritical_Corrector_stack; fpContaint_handler->determine_address_array_count = 1; break;} \
																					else\
																						{\
																							if(fpContaint_handler->HANDELD_previously_type__VMPX_asm_count <= pVMPX_req_context->Assembly_count)\
																								{fpContaint_handler->pVMPX_profile_reference = pVMPX_req_context; fpContaint_handler->HANDELD_previously_type__VMPX_asm_count = pVMPX_req_context->Assembly_count; fpContaint_handler->HANDELD_previously_type = (X_type); *fpDTAA_critical_addres = *fpLastCritical_Corrector_stack; fpContaint_handler->determine_address_array_count = 1; break;}\
																						}}
#define codedef_save_EXTRACT_VA_and_AddValue fpContaint_handler->EXTRACT_addres = Hanheld_start_addr; fpContaint_handler->EXTRACT_extracted_value = Dissasm_struct->Instruction.AddrValue
#define codedef_save_EXTRACT_LINK_VA fpContaint_handler->EXTRACT_VALUE_LINK_address = Hanheld_start_addr
#define codedef_set_FLOW_CONTROL_RETURN_WAYPOINT_ne 	*fpAddPool__FCM_dest_addres = Dissasm_struct->Instruction.AddrValue; *fpAddPool__FCM = Dissasm_struct->Instruction.BranchType;
#define codedef_set_jumpout_table_element_ne if(fpContaint_handler->critical_jumpouts_array_count <= MAX_DEEP_ASM_JUMPOUTS_HANHELD) {fpJumpOutGraph->jumpout_type_as_BeaEngine_BRANCH_TYPE = Dissasm_struct->Instruction.BranchType;  fpJumpOutGraph->GRAPH_INIT_asm_instruction = Hanheld_start_addr;  fpJumpOutGraph->GRAPH_START = Dissasm_struct->Instruction.AddrValue; fpContaint_handler->critical_jumpouts_array_count++; fpJumpOutGraph++;}
#define codedef_condition_set_JUMPOUT_function_type_primary_ne(jumpout_func_type_enum) fpResGRAPH->jumpout_function_as_VMProtect_definition = (jumpout_func_type_enum)
#define codedef_condition_set_GRAPH_circuit_closing_ne(desd_addr) fpEnumAddpool++; if(*fpEnumAddpool != (desd_addr)) continue; {fpResGRAPH->GRAPH_END = (desd_addr); codedef_condition_set_JUMPOUT_function_type_primary_ne(JUMPOUT_TYPE_CYCLE); break;}
#define codedef_set_FLOW_CONTROL_marker_ne(X_TYPE) 	*fpAddPool__FCM = X_TYPE
#define codedef_check_JMP_desdonation_address_include_on_addr_pool_ne(VMPROTDEF_X_TYPE) do {fpEnumAddpool++; if(*fpEnumAddpool != JMPOperandAddr) continue; fpCurrentResearchedJumpOutGraph->GRAPH_END = JMPOperandAddr; fpCurrentResearchedJumpOutGraph->jumpout_function_as_VMProtect_definition = (VMPROTDEF_X_TYPE); codedef_set_condition_for_abort_variator_SUBcycle; break; }while(ii_jouts--)
#define codedef_fill_determine_critical_addres_array if(fpContaint_handler->determine_address_array_count <= MAX_HANHELD_TYPE_DETERMINE_CRITICAL_ADDRESES){*fpDTAA_critical_addres = Hanheld_start_addr; fpContaint_handler->determine_address_array_count++;}
#define codedef_Accept_MT_Research_workflow(ERROR) *fpErr_seq_state = (ERROR); *fpResult_VM_Context = pNew_VMPX_profile; ::InterlockedIncrement(&pThreadWorkStack->TA_Context_save_count);
#define codedef_Success_found_target_object_inc(X_type) 	pFoundResultStruct->pVM_Success_found_VM_profile = pVM_Profile; *fpFoundHanheld = pVM_hanheld; *fpFound_target_type = (X_type); fpFoundHanheld++; fpFound_target_type++; pFoundResultStruct->Found_hanheld_stack_count++; ::InterlockedIncrement(&pThreadWorkStack->TA_Total_HANHELDS__in_VM_Profiles_found_count);//inc
#define codedef_Success_found_target_object_sup15(X_type, HOW_HANHELDS_ASSUME) 	pFoundResultStruct->pVM_Success_found_VM_profile = pVM_Profile; *fpFound_target_type = (X_type); pFoundResultStruct->Found_hanheld_stack_count = (HOW_HANHELDS_ASSUME); ::InterlockedIncrement(&pThreadWorkStack->TA_Total_VM_Profiles_found_count);

//++++++++++++++++++++++ structs ++++++++++++++++++++++
#pragma pack()
typedef struct DH_VM_HANDLER_ALL_JUMPOUTS_information_struct
{
	SERVICE_ADDRES GRAPH_INIT_asm_instruction; //непосредственный адрес jumpouts (адрес инструкции инициализации графопостроителя)
	SERVICE_ADDRES GRAPH_START; //адрес назначения условного прыжка (по операнду) - НАЧАЛО ГРАФА
	SERVICE_ADDRES GRAPH_END; //ИНСТРУКЦИЯ СМЫКАНИЯ - КОНЕЦ ГРАФА (NULL - ГРАФ НЕ СОМКНУЛСЯ!)
	SERVICE_COUNT  GRAPH_asm_COUNTs; //кол-во инструкций в графе
	SERVICE_TYPE jumpout_type_as_BeaEngine_BRANCH_TYPE; //тип прыжка (BRANCH_TYPE из BeaEngine.h)
	SERVICE_TYPE jumpout_function_as_VMProtect_definition; //функция прыжка (JUMPOUT_FUNCTION_TYPE_ENUMS)
/*enum BRANCH_TYPE

  JO = 1,
  JC = 2,
  JE = 3,
  JA = 4,
  JS = 5,
  JP = 6,
  JL = 7,
  JG = 8,
  JB = 2,       /* JC == JB *//*
  JECXZ = 10,
//////////
  JNO = -1,
  JNC = -2,
  JNE = -3,
  JNA = -4,
  JNS = -5,
  JNP = -6,
  JNL = -7,
  JNG = -8,
  JNB = -2      /* JNC == JNB */

}HANHELD_JUMPOUTS_GRAPH, *pHANHELD_JUMPOUTS_GRAPH;

typedef struct DH_VM_DETERMINE_TYPE_INSTRUCTION_ASM_REPORT
{
	Int32 Category; //категория BeaEngine
	Int32 Opcode; //операционный код BeaEngine
	char CompleteInstr[INSTRUCT_LENGTH]; //мнемоника инструкции определяющей тип хендла BeaEngine
}HANHELD_DETERMINE_TYPE_ASM_INSTRUCTION_REPORT,*pHANHELD_DETERMINE_TYPE_ASM_INSTRUCTION_REPORT; //информация об инструкции - as BeaEngine

typedef struct VM_VMPX_PROFILE_INI_Struct
{
	WCHAR Predict_STATIC_assembly[MAX_PATH]; //предсказываемая статическая инструкция (для быстрого поиска заданной цепочки байт в регионе)
	WCHAR Predict_wait_assembly1[MAX_PATH]; //предсказываемая инструкция №1 перед появлением главной
	WCHAR Predict_wait_assembly2[MAX_PATH]; //предсказываемая инструкция №2 перед появлением главной
	WCHAR Predict_wait_assembly3[MAX_PATH]; //предсказываемая инструкция №3 перед появлением главной
	WCHAR Predict_wait_assembly4[MAX_PATH]; //предсказываемая инструкция №4 перед появлением главной
	WCHAR Critial_assembly[MAX_PATH]; //критическая инструкция
	WCHAR After_critical_assebly1[MAX_PATH]; //после критической №1
	WCHAR After_critical_assebly2[MAX_PATH]; //после критической №2
	WCHAR Out_EXTRACT[MAX_PATH]; //инструкция с вытаскиваемой переменной
	WCHAR Out_EXTRACT_VALUE_LINK[MAX_PATH]; //инструкция, которая имеет ссылку и использует вытаскиваемую переменную

	//ДЛИНА СТРОК ИНСТРУКЦИЙ WCHAR... (wcslen)
	SERVICE_COUNT Predict_STATIC_assembly_len;
	SERVICE_COUNT Predict_wait_assembly1_len;
	SERVICE_COUNT Predict_wait_assembly2_len;
	SERVICE_COUNT Predict_wait_assembly3_len;
	SERVICE_COUNT Predict_wait_assembly4_len;
		SERVICE_COUNT Critial_assembly_len;
	SERVICE_COUNT After_critical_assebly1_len;
	SERVICE_COUNT After_critical_assebly2_len;
	SERVICE_COUNT Out_EXTRACT_len;
	SERVICE_COUNT Out_EXTRACT_VALUE_LINK_len;


	SERVICE_COUNT Predict_wait_count; //количество загруженных Predict_wait
	SERVICE_COUNT After_count; //количество загруженных After

	//ассемблированные инструкции
	XEDPARSE xed_Predict_STATIC;
	XEDPARSE xed_Predict_wait_assembly1;
	XEDPARSE xed_Predict_wait_assembly2;
	XEDPARSE xed_Predict_wait_assembly3;
	XEDPARSE xed_Predict_wait_assembly4;
		XEDPARSE xed_Critial_assembly;
	XEDPARSE xed_After_critical_assebly1;
	XEDPARSE xed_After_critical_assebly2;
		XEDPARSE xed_Out_EXTRACT;
		XEDPARSE xed_Out_EXTRACT_VALUE_LINK;

	//дизассемблированная инструкция
	DISASM bea_Predict_STATIC;
	DISASM bea_Critial_assembly;
	DISASM bea_Out_EXTRACT;
	DISASM bea_Out_EXTRACT_VALUE_LINK;
	
	//ФЛАГ СТАТИЧЕСКОЙ ИНСТРУКЦИИ (одинаковые байты с любым адресом cip/rip)
	bool is_static_Predict_STATIC_assembly; //предсказываемая статическая инструкция (для быстрого поиска заданной цепочки байт в регионе)
	bool is_static_Predict_wait_assembly1; //предсказываемая инструкция №1 перед появлением главной
	bool is_static_Predict_wait_assembly2; //предсказываемая инструкция №2 перед появлением главной
	bool is_static_Predict_wait_assembly3; //предсказываемая инструкция №3 перед появлением главной
	bool is_static_Predict_wait_assembly4; //предсказываемая инструкция №4 перед появлением главной
	bool is_static_Critial_assembly; //критическая инструкция
	bool is_static_After_critical_assebly1; //после критической №1
	bool is_static_After_critical_assebly2; //после критической №2
	bool is_static_Out_EXTRACT; //инструкция с вытаскиваемой переменной
	bool is_static_Out_EXTRACT_VALUE_LINK; //инструкция, которая имеет ссылку и использует вытаскиваемую переменную

	//КОНТРОЛЬТНЫЕ СИМВОЛЫ
	SERVICE_TYPE Predict_wait_STATIC_constrol_symbol;	//контрольный символ (если присутствует)
	SERVICE_TYPE Predict_wait_1_control_symbol;			//контрольный символ (если присутствует)
	SERVICE_TYPE Predict_wait_2_control_symbol;			//контрольный символ (если присутствует)
	SERVICE_TYPE Predict_wait_3_control_symbol;			//контрольный символ (если присутствует)
	SERVICE_TYPE Predict_wait_4_control_symbol;			//контрольный символ (если присутствует)
	SERVICE_TYPE Critical_control_symbol;				//контрольный символ (если присутствует)
	SERVICE_TYPE After_1_control_symbol;				//контрольный символ (если присутствует)
	SERVICE_TYPE After_2_control_symbol;				//контрольный символ (если присутствует)
	SERVICE_TYPE Out_EXTRACT_control_symbol;			//контрольный символ (если присутствует)
	SERVICE_TYPE Out_EXTRACT_VALUE_LINK_control_symbol; //контрольный символ (если присутствует)

	//размер рассчитывается в байтах от начала строки!!!!!!!
	//*pCSPresent_site = (save_original_asm_size - AsmInstruction_size)*sizeof(TCHAR);
	size_t First_control_symbol_offset__Predict_wait_STATIC_constrol_symbol; //контрольный символ (если присутствует)
	size_t First_control_symbol_offset__Predict_wait_1_control_symbol; //контрольный символ (если присутствует)
	size_t First_control_symbol_offset__Predict_wait_2_control_symbol; //контрольный символ (если присутствует)
	size_t First_control_symbol_offset__Predict_wait_3_control_symbol; //контрольный символ (если присутствует)
	size_t First_control_symbol_offset__Predict_wait_4_control_symbol; //контрольный символ (если присутствует)
	size_t First_control_symbol_offset__Critical_control_symbol;		//контрольный символ (если присутствует)
	size_t First_control_symbol_offset__After_1_control_symbol;		//контрольный символ (если присутствует)
	size_t First_control_symbol_offset__After_2_control_symbol;		//контрольный символ (если присутствует)
	size_t First_control_symbol_offset__Out_EXTRACT_control_symbol; //контрольный символ (если присутствует)
	size_t First_control_symbol_offset__Out_EXTRACT_VALUE_LINK_control_symbol; //контрольный символ (если присутствует)

	//КОМБИНИРОВАНЫЙ ТИП ИНСТРУКЦИИ (ДЛЯ БЫСТРОГО ДОСТУПА) = ФЛАГ СТАТИЧЕСКОЙ ИНСТРУКЦИИ + КОНТРОЛЬТНЫЕ СИМВОЛЫ
	SERVICE_TYPE Quick_combine_type__Predict_STATIC; //VMPX_CONTEXT_COMBINE_TYPE_ASMS
	SERVICE_TYPE Quick_combine_type__Predict_wait_1; //VMPX_CONTEXT_COMBINE_TYPE_ASMS
	SERVICE_TYPE Quick_combine_type__Predict_wait_2; //VMPX_CONTEXT_COMBINE_TYPE_ASMS
	SERVICE_TYPE Quick_combine_type__Predict_wait_3; //VMPX_CONTEXT_COMBINE_TYPE_ASMS
	SERVICE_TYPE Quick_combine_type__Predict_wait_4; //VMPX_CONTEXT_COMBINE_TYPE_ASMS
	SERVICE_TYPE Quick_combine_type__Critical_control; //VMPX_CONTEXT_COMBINE_TYPE_ASMS
	SERVICE_TYPE Quick_combine_type__After_1; //VMPX_CONTEXT_COMBINE_TYPE_ASMS
	SERVICE_TYPE Quick_combine_type__After_2; //VMPX_CONTEXT_COMBINE_TYPE_ASMS
	SERVICE_TYPE Quick_combine_type__Out_EXTRACT; //VMPX_CONTEXT_COMBINE_TYPE_ASMS
	SERVICE_TYPE Quick_combine_type__Out_EXTRACT_VALUE_LINK; //VMPX_CONTEXT_COMBINE_TYPE_ASMS


	//размерность cpu-регистра для контрольного символа PROFILE_VMPX_CONTROL_SYMBOL_CPU_REGISTER
	SERVICE_TYPE CS_REGISTER__size_type__Predict_STATIC; //enum CONTROL_SYMBOL_SIZE__REGISTER
	SERVICE_TYPE CS_REGISTER__size_type__Predict_wait_1; //enum CONTROL_SYMBOL_SIZE__REGISTER
	SERVICE_TYPE CS_REGISTER__size_type__Predict_wait_2; //enum CONTROL_SYMBOL_SIZE__REGISTER
	SERVICE_TYPE CS_REGISTER__size_type__Predict_wait_3; //enum CONTROL_SYMBOL_SIZE__REGISTER
	SERVICE_TYPE CS_REGISTER__size_type__Predict_wait_4; //enum CONTROL_SYMBOL_SIZE__REGISTER
	SERVICE_TYPE CS_REGISTER__size_type__Critical_control; //enum CONTROL_SYMBOL_SIZE__REGISTER
	SERVICE_TYPE CS_REGISTER__size_type__After_1; //enum CONTROL_SYMBOL_SIZE__REGISTER
	SERVICE_TYPE CS_REGISTER__size_type__After_2; //enum CONTROL_SYMBOL_SIZE__REGISTER
	SERVICE_TYPE CS_REGISTER__size_type__Out_EXTRACT; //enum CONTROL_SYMBOL_SIZE__REGISTER
	SERVICE_TYPE CS_REGISTER__size_type__Out_EXTRACT_VALUE_LINK; //enum CONTROL_SYMBOL_SIZE__REGISTER
	
	//инкремент подставляемых данных в контрольные символы (регистры)
	SERVICE_TYPE INCREMENT_STANDBY_TYPE; //значение инкрементации для подставляемых контрольных символов. Определяется TYPEDEF_INCREMENTAL_STANDY_MODE. по дефолту - ЗАБЛОКИРОВАН!
	SERVICE_ID   INCREMENT_ID; // идентефикатор последнего подставляемого элемента

	//ПЛАН ПОИСКА ИНСТРУКЦИЙ (перечисление VMPX_CONTEXT_TYPE_ASMS)
	SERVICE_TYPE tAsmSearchTask_stack[12]; //стек задач по поиску инструкций - исключая Out_EXTRACT и STATIC
	SERVICE_COUNT cAsmSearchTask_stack_count; //количество задач по поиску инструкций

	SERVICE_COUNT Assembly_count; //КОЛ-ВО инструкций общее

}VM_VMPX_PROFILE_INI, *pVM_VMPX_PROFILE_INI;

typedef struct VM_VMPX_PROFILE_FOUND_SEQUENCE_Struct
{
	//найденные адреса требуемых инструкций
	SERVICE_ADDRES Predict_STATIC_addres; //адрес Predict_STATIC_assembly
	SERVICE_ADDRES Predict_wait_assembly1_addres;
	SERVICE_ADDRES Predict_wait_assembly2_addres;
	SERVICE_ADDRES Predict_wait_assembly3_addres;
	SERVICE_ADDRES Predict_wait_assembly4_addres;
	SERVICE_ADDRES Critial_assembly_addres; //критический адрес
	SERVICE_ADDRES After_critical_assebly1_addres;
	SERVICE_ADDRES After_critical_assebly2_addres;
	SERVICE_ADDRES EXTRACT_addres; //непосредвенный адрес инструкции _EXTRACET
	SERVICE_ADDRES EXTRACT_extracted_value; //извлеченный операнд из этого адреса _EXTRACT
	SERVICE_ADDRES EXTRACT_VALUE_LINK_address; //адрес инструкции, которая ссылается на извлеченное значение EXTRACT_VALUE_LINK
	Int32 Bea_EXTRACT_VALUE_LINK__left_side_type; //тип левой стороны инструкции (регистр, область памяти и тд), которая ссылается на VALUE_LINK. Bea Engine type.
	size_t Bea_EXTRACT_VALUE_LINK__left_Arg_size; //размер операнда левой стороны, которая ссылается на VALUE_LINK.
	size_t Bea_EXTRACT_VALUE_LINK__asm_size; //общий размер инструкции для последующего перескока

	bool isDuplicateContext; //является ли EXTRACT_extracted_value повторяющимся по отношению к остальному стеку (НЕ УНИКАЛЬНЫЙ)
	
	//конрольный символ - регистр CPU (если требуется)
	SERVICE_ID CS_Predict_wait1__CPU_REGISTER;
	SERVICE_ID CS_Predict_wait2__CPU_REGISTER;
	SERVICE_ID CS_Predict_wait3__CPU_REGISTER;
	SERVICE_ID CS_Predict_wait4__CPU_REGISTER;
	SERVICE_ID CS_Critical__CPU_REGISTER;
	SERVICE_ID CS_After1__CPU_REGISTER;
	SERVICE_ID CS_After2__CPU_REGISTER;
	SERVICE_ID CS_EXTRACT__CPU_REGISTER; //запоминаем регистр CPU, который используется в _EXTRACT
	SERVICE_ID CS_EXTRACT_VALUE_LINK__CPU_REGISTER; //запоминаем регистр CPU, который используется в EXTRACT_VALUE_LINK

	SERVICE_COUNT Researched_VM_hanhelds; //количество hanhelds, которые достоверно удалось обнаружить статическим путём
	SERVICE_COUNT Total_found_addreses; //общее количество найденных адресов

	signed long Stack_RSP_Different; //операции резервирования-освобождения стека: количество байт
}VM_VMPX_PROFILE_FOUND_SEQUENCE, *pVM_VMPX_PROFILE_FOUND_SEQUENCE;

#define MAX_STACK_SEQ 10000
typedef struct VM_VMPX_PROFILE_FOUND_SEQUENCE_STACK_Struct //найденная цепочка
{
	VM_VMPX_PROFILE_FOUND_SEQUENCE Found_Seq[MAX_STACK_SEQ]; //стек найденых цепочек
	pVM_VMPX_PROFILE_FOUND_SEQUENCE fpSeqStackESP; //указатель на вершину стека Found_Seq
	SERVICE_COUNT Found_sequences; //количество найденных цепочек
	SERVICE_COUNT Duplicate_from_EXTRACT; //дублируются исходя из одинаковых значений EXTRACT_extracted_value
	SERVICE_TYPE VMPX_Version; //vmpx версия профиля (VMP2, VMP3...)
	TARGET_PROC_REGION_COPY IncludeVMRegion; //параметры выделенного региона с вируальной машиной
	IMAGE_SECTION_HEADER RegionSectionInfo; //информация о секции PE-файла, к которой принадлежит регион (если доступно)
}VM_VMPX_PROFILE_FOUND_SEQUENCE_STACK, *pVM_VMPX_PROFILE_FOUND_SEQUENCE_STACK;

typedef struct DH_VM_HANDLER_information_struct
{
CELL_ADDRES Cell_Addres; //адрес ячейки, в которой хранится зашифрованный HANDELD_Primary_Addres (только для VMP2)
HANHELD_ADDRES HANDELD_Primary_Addres; // непосредственный адрес хендла
bool is_valid_HANHELD_address; // правильный адрес?
bool is_duplicate_hanheld_in_supply15; //адрес дублирован, уже встречался ранее? (только для VMP2)

//стек по _EXTRACT... 
SERVICE_ADDRES EXTRACT_addres; //непосредвенный адрес инструкции _EXTRACET
SERVICE_ADDRES EXTRACT_extracted_value; //извлеченный операнд из этого адреса _EXTRACT
SERVICE_ADDRES EXTRACT_VALUE_LINK_address; //адрес инструкции, которая ссылается на извлеченное значение EXTRACT_VALUE_LINK

HANHELD_TYPE HANDELD_type; //тип островка (ОКОНЧАТЕЛЬНО ОПРЕДЕЛЕННЫЙ)
HANHELD_TYPE HANDELD_previously_type; //тип островка ПРЕДВАРИТЕЛЬНЫЙ (для функции DH_VM_Scan_JUMPOUTs_Init)
HANHELD_TYPE HANDELD_previously_type__VMPX_asm_count; //количество инструкций, с помощью которых был определен тип островка (проверка в случае перезаписи)
HANHELD_DETERMINE_TYPE_ASM_INSTRUCTION_REPORT HANHELD_type_asm_disasm_simple_info; //краткая информация о диссасемблированной инструкции, определяющей тип островка (BeaEngine)
pVM_VMPX_PROFILE_INI pVMPX_profile_reference; //ссылка на VMPX_профиль виртуальной машины (обновляемая)

signed long PCODE_Shift; //сдвиг по ленте
signed long stack_RBP_Shift; //сдвиг по стеку RBP/EBP

char hanheld_dissasm_first_insruction[INSTRUCT_LENGTH]; //дисассемблированная команда

HANHELD_TYPE_CRITICAL_DETERMINE_ADDRES determine_type_addres_array[MAX_HANHELD_TYPE_DETERMINE_CRITICAL_ADDRESES]; //критические адреса, по которым определяется тип островка
SERVICE_ARRAY_ID determine_type_addres_array__HANHELD_in_AddrPool_ID[MAX_HANHELD_TYPE_DETERMINE_CRITICAL_ADDRESES]; //привязка критических адресов к пулу адресов хендла (какая позиция в массиве)
SERVICE_COUNT determine_address_array_count; //кол-во элементов в determine_type_addres_array
HANHELD_TYPE_CRITICAL_DETERMINE_ADDRES* fpCritical_addres;// fp на последний свободный determine_type_addres_array

HANHELD_JUMPOUTS_GRAPH critical_jumpouts_array[MAX_DEEP_ASM_JUMPOUTS_HANHELD]; //графы хендла
SERVICE_COUNT critical_jumpouts_array_count; //кол-во элементов в critical_jumpouts_array
HANHELD_JUMPOUTS_GRAPH* fpCritical_jumpouts; //fp на последний свободный critical_jumpouts_array

SERVICE_COUNT AsmInstructionAddressPool_size;// кол-во адресов в одном пуле

}HANHELD, *pHANHELD; //хендлер VM

typedef struct DH_VMPX_VM_information_struct //НОВЫЙ УНИВЕРСАЛЬНЫЙ ПРОФИЛЬ (КОНТЕКСТА) ВИРТУАЛЬНОЙ МАШИНЫ
{
//common info. Описатели заготовки. Общая информация
WCHAR vm_name[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN]; //имя виртуальной машины
WCHAR contact_PIRATE_MAP_PROFILE_name[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN]; //имя профиля PIRATE_MAP-карты для связи
WCHAR contact_VMPX_PROFILE_name[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN]; //имя VMPX-профиля
SERVICE_COUNT handles_count; //достоверно установленное количество вирутальных островков (VMP2 - 256, VMP - > 1000)
SERVICE_COUNT approx_scan_handles_count; //предполагаемое количество вирутальных островков, найденное при multi-сканировании
SERVICE_COUNT identeficated_handles_count; //количество распознанных (с окончательно присвоенным типом) островков от handles_count
SERVICE_TYPE VMPX_version; //версия VMPX. Дублируется в VM_VMPX_PROFILE_FOUND_SEQUENCE
bool is_dll_addres_type; //динамический адрес (DLL?, RELOCATION?). требуется ли пересчет адресов. для динамических адресов SERVICE_ADDRES как SERVICE_OFFSET
bool is_vm_billet; //is billet || VMPX_VM_Profile ?
signed long Stack_RSP_Different; //операции резервирования-освобождения стека: количество байт

//информация о регионе(секции), в котором расположен контекст
IMAGE_SECTION_HEADER RegionSectionInfo; //информация о секции PE-файла, к которой принадлежит регион (если доступно)

//first operation info - SEQUENCE
VM_VMPX_PROFILE_FOUND_SEQUENCE VM_ENTER_Seq_Stack; //стек найденных адресов цепочек VM_ENTER, согласно указанному VMPX-профилю

//if present supply15. Если присутсвует supply15, работа с контекстом через него.
SERVICE_COUNT duplicate_handles_count; //кол-во дублированных хендлов в таблице (кол-во уникальных хендлов = handles_count - duplicate_handles_count)
PVOID supply_15_start_address; //начальный адрес таблицы хендлов (default: r12 - VMP 2) 
OPERATIONAL_ASM_INSTRUCTION table_DEcode_instruction; //инструкция декодирования #1
OPERATIONAL_OPERAND_CELL_ADDRES table_DEcode_instruction_operand; //операнд для деокдирования #1 (add, sub) * если требуется 
OPERATIONAL_ASM_INSTRUCTION table_DEcode_instruction_2; //инструкция декодирования #2
OPERATIONAL_OPERAND_CELL_ADDRES table_DEcode_instruction_operand_2; //операнд для деокдирования #2 (add, sub) * если требуется

//Информация для spirun7.
SERVICE_ADDRES VM_enter_first_asm_addres[MAX_VM_ENTERs_FOR_ONE_CONTEXT]; //адрес входа в VM_ENTER  для одного контекста
SERVICE_COUNT VM_enter_first_asm_addres_count; //количество элементов в массиве VM_enter_first_asm_addres
HANHELD_ADDRES original_hanhelds_table_crypted[VM_HANDLES_MAX_COUNT]; //оригинальный дамп таблицы hanhelds - покриптованный (для быстрого копирования)
HANHELD_ADDRES original_hanhelds_table_decrypted[VM_HANDLES_MAX_COUNT]; //оригинальный дамп таблицы hanhelds - с открытыми адресами (для быстрого копирования)
HANHELD_ADDRES spirun7_hanhelds_table_crypted[VM_HANDLES_MAX_COUNT]; //spirun7 дамп таблицы hanhelds - покриптованный (для быстрого копирования)
HANHELD_ADDRES spirun7_hanhelds_table_decrypted[VM_HANDLES_MAX_COUNT]; //spirun7 дамп таблицы hanhelds - с открытыми адресами (для быстрого копирования)

SERVICE_OFFSET ofs_1Containt_handler; //оффсет для первого HANHELD Containt_handler[], следуемого за VMPX_VM_PROFILE_INFO_STACK
SERVICE_OFFSET ofs_VM_hanheld_addres_pool; //оффсет VM_hanheld_addres_pool, следуемого за HANHELD Containt_handler[], который теперь после VMPX_VM_PROFILE_INFO_STACK :)
size_t retail_VM_profile_stack_size; //РАЗМЕР ВСЕГО СТЕКА, С УЧЁТОМ  Containt_handler[] + VM_hanheld_addres_pool[]
}VMPX_VM_PROFILE_INFO_STACK, *pVMPX_VM_PROFILE_INFO_STACK;
//HANHELD Containt_handler[]; //"островки" - расшифрованы
//VM_hanheld_addres_pool[]; //пулы адресов


typedef struct DH_VM_information_struct //информация о таблице вирутальной машины
{
WCHAR name[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN]; //имя виртуальной машины
SERVICE_COUNT handles_count; //количество вирутальных островков (256)
SERVICE_COUNT duplicate_handles_count; //кол-во дублированных хендлов в таблице (кол-во уникальных хендлов = handles_count - duplicate_handles_count)
ULONG vm_version; //версия VM
//ex (lords of fallen): 000000014FE791EA - 0x000000014FE789F2 = 0x7F8 - 2040
PVOID r12_supply_15_start_address; //начальный адрес таблицы хендлов (r12) 
//ex (lords of fallen): start 0x000000014FE789F2 / ends in 000000014FE791EA
OPERATIONAL_ASM_INSTRUCTION r12_table_DEcode_instructions; //инструкции декодирования #1
//ULONG r12_table_ENcode_instructions; //инструкции кодирования таблицы #1
OPERATIONAL_OPERAND_CELL_ADDRES r12_table_DEcode_instructions_operand; //операнд для деокдирования #1 (add, sub) * если требуется 

OPERATIONAL_ASM_INSTRUCTION r12_table_DEcode_instructions_2; //инструкции декодирования #2
OPERATIONAL_OPERAND_CELL_ADDRES r12_table_DEcode_instructions_operand_2; //операнд для деокдирования #2 (add, sub) * если требуется

HANHELD Containt_handler[VM_HANDLES_MAX_COUNT]; //"островки" - расшифрованы
//spirun7
SERVICE_ADDRES vmp2_original_hanhelds_table_crypted[VM_HANDLES_MAX_COUNT]; //оригинальный дамп таблицы hanhelds - покриптованный (для быстрого копирования)
SERVICE_ADDRES vmp2_original_hanhelds_table_decrypted[VM_HANDLES_MAX_COUNT]; //оригинальный дамп таблицы hanhelds - с открытыми адресами (для быстрого копирования)
SERVICE_ADDRES spirun7_hanhelds_table_crypted[VM_HANDLES_MAX_COUNT]; //spirun7 дамп таблицы hanhelds - покриптованный (для быстрого копирования)
SERVICE_ADDRES spirun7_hanhelds_table_decrypted[VM_HANDLES_MAX_COUNT]; //spirun7 дамп таблицы hanhelds - с открытыми адресами (для быстрого копирования)

WCHAR comment[MAX_COMMENTS_LEN]; //текст комментариев
}VM_INFO_STACK, *pVM_INFO_STACK; //old VM_INFO stack

//пул адресов
typedef struct DH_VM_instruction_pool_hanheld
{
	SERVICE_COUNT HANHELD_asm_instruction_count; //кол-во адресов инструкций в пуле для каждого хендла
	SERVICE_ADDRES asm_instruction_addres[MAX_ASM_INSTUCTION_POOL];//адреса инструкций VM одного хендла
	BYTE asm_instruction_unconditional_FLOW_CONTROL_MARKER[MAX_ASM_INSTUCTION_POOL];//маркеры инструкций с безусловными переходами VM одного хендла (JUMP const, JMP reg64, RET)
	SERVICE_ADDRES asm_instruction_unconditional_FLOW_CONTROL_MARKER_dest_addres[MAX_ASM_INSTUCTION_POOL];//адреса назначения прыжка для маркеров с безусловными переходами

	//текущие указатели на свободные блоки
	LPBYTE fpAddPool__FCM; //fp по asm_instruction_unconditional_FLOW_CONTROL_MARKER
	SERVICE_ADDRES* fpAddPool__FCM_dest_addres; //fp по asm_instruction_unconditional_FLOW_CONTROL_MARKER_dest_addres
	SERVICE_ADDRES* fpAddPool__asm_addres; //fp по asm_instruction_addres
}VM_hanheld_addres_pool, *pVM_hanheld_addres_pool;

typedef struct DH_VM_instruction_pool_struct //информация о таблице вирутальной машины
{
//WCHAR name[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN]; //имя виртуальной машины
//REGION_COPY AsmInstructionAddressPool;// пул адресов всех инструкций этого контекста VM
//SERVICE_COUNT max_asm_instruction_pool_size;//макс кол-во  == MAX_ASM_INSTUCTION_POOL
VM_hanheld_addres_pool VM_HANHELD_addr_pool[VM_HANDLES_MAX_COUNT]; //пулы адресов инструкций хендлов
}VM_AsmAddr_POOL_STACK, *pVM_AsmAddr_POOL_STACK;


typedef struct DH_VM_container_info //заголовок. Информация о контейнере
{	SERVICE_COUNT VM_information_records_count; //ко-во записей стеков виртуальных машин
}DH_VM_info_header, *pDH_VM_info_header;

typedef struct DH_VM_MANUAL_input //базовая информация для VM профилей, создаваемых вручную
{
	SERVICE_TYPE VMPX_version;
	TCHAR VM_name[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN];
	PVOID supply_15_start_address; //начальный адрес таблицы хендлов (default: r12 - VMP 2) 
	OPERATIONAL_ASM_INSTRUCTION table_DEcode_instruction; //инструкция декодирования #1
	OPERATIONAL_OPERAND_CELL_ADDRES table_DEcode_instruction_operand; //операнд для деокдирования #1 (add, sub) * если требуется 
	SERVICE_COUNT handles_count;
	signed long Stack_RSP_Different;
}VM_Manual_input, *pVM_Manual_input;


#define MAX_THREADS 100 //максимальное количество потоков
typedef struct VM_STRUCT_THREADWORK_SEQ_SCAN
{
	//threadwork
	DWORD Shedule_ThreadID[MAX_STACK_SEQ]; // == VM_VMPX_PROFILE_FOUND_SEQUENCE Found_Seq[MAX_STACK_SEQ]; //ЗАПЛАНИРОВАННЫЕ ID ПО ЦЕПОЧКМ
	SERVICE_ERROR Err_seq_state[MAX_STACK_SEQ]; //СТАТУС ОШИБОК
	pVMPX_VM_PROFILE_INFO_STACK		pVM_Context_save[MAX_STACK_SEQ]; //созданные контексты виртуальной машины
	volatile SERVICE_COUNT TA_Context_save_count; //количество контекстов для сохранения
	//pVM_AsmAddr_POOL_STACK			pVM_Address_pool[MAX_STACK_SEQ]; //пул адресов виртуальной машины с привязкой к контекстам - обьеденен в одно целое с VM Context stack
	//TARGET_PROC_REGION_COPY pRegion_context_result[MAX_STACK_SEQ]; //созданные контексты виртуальной машины
	//TARGET_PROC_REGION_COPY pRegion_address_pool_result[MAX_STACK_SEQ]; //пул адресов виртуальной машины с привязкой к контекстам

	//СИНХРОННЫЕ ДАННЫЕ
	HANDLE hThread[MAX_THREADS]; // СОХРАНЕННЫЕ ХЕНДЛЫ ПОТОКОВ
	DWORD ThreadID[MAX_THREADS]; //СОХРАНЕННЫЕ ID ПОТОКОВ

	//
	SERVICE_COUNT Found_sequences; //количество найденных цепочек (брать из VM_VMPX_PROFILE_FOUND_SEQUENCE_STACK)
	

	pVM_VMPX_PROFILE_FOUND_SEQUENCE_STACK pSeq_live_pointer_stack; //указатель на стек цепочек
	bool Ready_flag; //флаг готовности планировщика shedule. Взводится при полностью заполненной таблице HANDLE hThread и DWORD ThreadID
	SERVICE_COUNT How_thread_has_been_created; //количество потоков, которое было изначально(!) создано
	volatile SERVICE_COUNT TA_Active_Threads_count; //количество активных потоков, задействованных сейчас для исследования (требуется для планировщика задач). Переменная volatile
}VM_THREADWORK, *pVM_THREADWORK; //планировщик задач для SEQ_SCAN

#define MAX_FOUND_HANHELDS 10000
typedef struct VM_OBJECT_FOUND_HANHELDS
{
	pVMPX_VM_PROFILE_INFO_STACK pVM_Success_found_VM_profile; //ссылка на успешно найденные VM контейнер
	pHANHELD pFoundHanheld[MAX_FOUND_HANHELDS]; //ссылки на найденные хендлы в нём
	SERVICE_TYPE Found_target_type[MAX_FOUND_HANHELDS];//тип найденных данных  в хендлах (согласно DH_VM_FOUND_TYPE)
	SERVICE_COUNT Found_hanheld_stack_count; //количество найденных хендлов в стеке
}DH_VM_FOUND_RESULT, *pDH_VM_FOUND_RESULT;


typedef struct VM_STRUCT_THREADWORK_OBJECT_SEARCH
{
	//threadwork _OBJECT_SEARCH
	DWORD Shedule_ThreadID[MAX_CONTAINTER_FILES]; //ЗАПЛАНИРОВАННЫЕ ID ПО ЦЕПОЧКМ
	SERVICE_ERROR Err_seq_state[MAX_CONTAINTER_FILES]; //СТАТУС ОШИБОК

	DH_VM_FOUND_RESULT VM_Found_result[MAX_CONTAINTER_FILES]; //результат поиска выраженный в найденных VM профилях и их хендлах
	pDH_ENUM_FILES_of_PROFILES pEnumFilesStack_start; //ОБЯЗАТЕЛЬНАЯ ссылка на начало стека pDH_ENUM_FILES_of_PROFILES
	SERVICE_COUNT EnumFilesStack_COUNT; //количество указателей в стеке pEnumFilesStack_start

	volatile SERVICE_COUNT TA_Total_VM_Profiles_found_count; //ОБЩЕЕ количество найденных VM ПРОФИЛЕЙ (контейнеров)
	volatile SERVICE_COUNT TA_Total_HANHELDS__in_VM_Profiles_found_count; //ОБЩЕЕ количество найденных хендлов во всех VM ПРОФИЛЯХ (контейнерах)

	//СИНХРОННЫЕ ДАННЫЕ
	HANDLE hThread[MAX_THREADS]; // СОХРАНЕННЫЕ ХЕНДЛЫ ПОТОКОВ
	DWORD ThreadID[MAX_THREADS]; //СОХРАНЕННЫЕ ID ПОТОКОВ

	TCHAR* pObject_text_ref; //ссылка на изначальный обьект поиска
	size_t pObject_text_addres_wcstoul; //преобразованное число из [ссылка на изначальный обьект поиска] - если преобразование возможно по цели запроса.
	SERVICE_TYPE pObject_text_ref_TYPE; //тип объекта (согласно VM_Object_text_ref_TYPE)
	bool Ready_flag; //флаг готовности планировщика shedule. Взводится при полностью заполненной таблице HANDLE hThread и DWORD ThreadID
	SERVICE_COUNT How_thread_has_been_created; //количество потоков, которое было изначально(!) создано
	volatile SERVICE_COUNT TA_Active_Threads_count; //количество активных потоков, задействованных сейчас для исследования (требуется для планировщика задач). Переменная volatile
}VM_THREADWORK__OBJECT_SEARCH, *pVM_THREADWORK__OBJECT_SEARCH; //планировщик задач для _OBJECT_SEARCH

typedef struct VM_VMPX_PROFILE_INI_STACK_Struct
{
	SERVICE_TYPE KeyType[HANHELD_TYPE_ENUMS_COUNT];
	VM_VMPX_PROFILE_INI VMPX_HANHELD_TYPE_ENUMS[HANHELD_TYPE_ENUMS_COUNT];
	SERVICE_COUNT Loaded_HANHELDS_FILTERS; //кол-во загруженных фильтров в PATCH_SUPPORTING_VMPX_PROFILE_INI

	//подготовленные данные для диспетчера по поиску инструкций
	 SERVICE_TYPE* TaskWorkSearcher_stack__NULL_fp[HANHELD_TYPE_ENUMS_COUNT]; //инициализированные значения fp-null
	 SERVICE_COUNT  TaskWorkSearcher_count_initial_count[HANHELD_TYPE_ENUMS_COUNT]; //инициализированные значения fp-null
}VM_VMPX_PROFILE_INI_STACK, *pVM_VMPX_PROFILE_INI_STACK;

typedef struct PROGRESS_MULTI_SCAN_INFO
	{
		SERVICE_COUNT Total_Bytes; //всего байт
		SERVICE_COUNT Bytes_leave; //байт осталося (в декременте)
		SERVICE_COUNT Found_VM_ENTERs; //найденные входы в VM
		SERVICE_COUNT Found_VM_HANHELDs; //найденные VM примитивы
		SERVICE_COUNT Found_VM_CONTEXTs; //найденные VM контексты исходя из VM_VMPX_PROFILE_FOUND_SEQUENCE_Struct.EXTRACT_extracted_value (кол-во контекстов = Found_VM_ENTERs - результат DH_VM_Scan_VirtualMachine_Contexts__Mark_Duplicate_contexts_from_EXTRACT)
		SERVICE_COUNT Researched_VM_HANHELDs; //исследованные VM контексты (полученная информация по VM_VMPX_PROFILE_FOUND_SEQUENCE_Struct.EXTRACT_extracted_value с применением соотвествующих методов для каждого типа виртуальной машины).
		SERVICE_COUNT Identeficated_VM_HANHELDs; //идентифицируемые VM контексты (напрямую завязана с предыдущей переменной Researched_VM_HANHELDs).
	}PROGRESS_MULTI_SCAN_INFO_STACK,*pPROGRESS_MULTI_SCAN_INFO_STACK; //информация о прогрессе в выполняемом сканировании mutli (для ОДНОГО РЕГИОНА)

typedef struct VMPX_PROFILE_FORCED_INIT_ASM_Struct //обязательн иницилиализируемые инструкции профиля
{
	bool Force_init_STATIC; //обязательная инициализация PROFILE_SERVICE_CONFIG_FILE_VMPX_CONTEXT_INI_DH_Predict_STATIC_key
	bool Force_all_Predict_wait; //обязательная инициализация PROFILE_SERVICE_CONFIG_FILE_VMPX_CONTEXT_INI_DH_Predict_key_x
	bool Force_all_After; //обязательная инициализация PROFILE_SERVICE_CONFIG_FILE_VMPX_CONTEXT_INI_DH_After_key_x
	bool Force_Critical; //обязательная инициализация PROFILE_SERVICE_CONFIG_FILE_VMPX_CONTEXT_INI_DH_Critical_key
	bool Force_EXTRACT; //обязательная инициализация PROFILE_SERVICE_CONFIG_FILE_VMPX_CONTEXT_INI_DH_EXTRARCT_key (_EXTRACT)
}VMPX_PROFILE_FORCED_INIT_ASM, *pVMPX_PROFILE_FORCED_INIT_ASM;

// >>> new engine VMPX_STACK >>>
typedef struct DH_VMPX_billet_information_struct //заготовка (биллет) виртуальной машины
{
//common info. Описатели заготовки. Общая информация
WCHAR vm_name[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN]; //имя виртуальной машины
WCHAR contact_PIRATE_MAP_PROFILE_name[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN]; //имя профиля PIRATE_MAP-карты для связи
WCHAR contact_VMPX_PROFILE_name[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN]; //имя VMPX-профиля
SERVICE_COUNT handles_count; //достоверно установленное количество вирутальных островков (VMP2 - 256, VMP - > 1000)
SERVICE_COUNT approx_scan_handles_count; //предполагаемое количество вирутальных островков, найденное при multi-сканировании
SERVICE_TYPE VMPX_version; //версия VMPX. Дублируется в VM_VMPX_PROFILE_FOUND_SEQUENCE
bool is_dll_addres_type; //динамический адрес (DLL?, RELOCATION?). требуется ли пересчет адресов. для динамических адресов SERVICE_ADDRES как SERVICE_OFFSET
bool is_vm_billet; //is billet || VMPX_VM_Profile ?

//first operation info - SEQUENCE
VM_VMPX_PROFILE_FOUND_SEQUENCE VM_ENTER_Seq_Stack; //стек найденных адресов цепочек VM_ENTER, согласно указанному VMPX-профилю

//if present supply15. Если присутсвует supply15, работа с контекстом через него.
SERVICE_COUNT duplicate_handles_count; //кол-во дублированных хендлов в таблице (кол-во уникальных хендлов = handles_count - duplicate_handles_count)
PVOID supply_15_start_address; //начальный адрес таблицы хендлов (default: r12 - VMP 2) 
OPERATIONAL_ASM_INSTRUCTION r12_table_DEcode_instructions; //инструкции декодирования #1
OPERATIONAL_OPERAND_CELL_ADDRES r12_table_DEcode_instructions_operand; //операнд для деокдирования #1 (add, sub) * если требуется 
OPERATIONAL_ASM_INSTRUCTION r12_table_DEcode_instructions_2; //инструкции декодирования #2
OPERATIONAL_OPERAND_CELL_ADDRES r12_table_DEcode_instructions_operand_2; //операнд для деокдирования #2 (add, sub) * если требуется

//Информация для spirun7.
SERVICE_ADDRES VM_enter_first_asm_addres[MAX_VM_ENTERs_FOR_ONE_CONTEXT]; //адрес входа в VM_ENTER  для одного контекста
SERVICE_COUNT VM_enter_first_asm_addres_count; //количество элементов в массиве VM_enter_first_asm_addres
CELL_ADDRES original_hanhelds_table_crypted[VM_HANDLES_MAX_COUNT]; //оригинальный дамп таблицы hanhelds - покриптованный (для быстрого копирования)
CELL_ADDRES original_hanhelds_table_decrypted[VM_HANDLES_MAX_COUNT]; //оригинальный дамп таблицы hanhelds - с открытыми адресами (для быстрого копирования)
CELL_ADDRES spirun7_hanhelds_table_crypted[VM_HANDLES_MAX_COUNT]; //spirun7 дамп таблицы hanhelds - покриптованный (для быстрого копирования)
CELL_ADDRES spirun7_hanhelds_table_decrypted[VM_HANDLES_MAX_COUNT]; //spirun7 дамп таблицы hanhelds - с открытыми адресами (для быстрого копирования)

HANHELD Containt_handler[]; //"островки" - расшифрованы
}VMPX_BILLET_INFO_STACK, *pVMPX_BILLET_INFO_STACK;

//********** DH VM func ********** //
extern UINT DH_VM_Get_current_profile_data_first(pVM_INFO_STACK VM_Stack); //чтение данных VM из текущего профиля. Возвращает кол-во записей стеков виртуальных машин и заполняет стек первой VM в профиле
extern UINT DH_Fill_Supply15_HANHELDs_table_for_current_profile(__in pVM_INFO_STACK VM_Stack_Info_set, __in HANHELD HANHELD_Array_Addres[], __in __out pVM_AsmAddr_POOL_STACK VM_addr_pool_stack_set); //заполняет "островки" для VMP2 по известнолму адресу supply15

extern void __fastcall DH_DEcode_current_supply15_addres(
	__in UINT Decode_instruction_s_type, //константа-команда S_TYPE декодирования
	 __in ULONG64 Decode_operand__optional, //операнд для декодирования (опционально. требуется не для всех инстуркций)
	__in __out CELL_ADDRES Desd_out_Addres //указанный адрес (декодирование значение кладется туда же)
	); //PRIMARY - декодирует адрес указанной инструкнцией

void __fastcall  DH_Crypt_current_supply15_addres(
	__in UINT Decode_instruction_s_type, //константа-команда S_TYPE декодирования
	__in ULONG64 Decode_operand__optional, //операнд для декодирования (опционально. требуется не для всех инстуркций)
	__in __out CELL_ADDRES Desd_out_Addres//указанный адрес (декодирование значение кладется туда же)
	); //BACK - криптует адрес указанной инструкции

extern void __fastcall  DH_DEcode_supply15_table(
	__in UINT Decode_instruction_s_type, //константа-команда S_TYPE декодирования
	__in ULONG64 Decode_operand__optional, //операнд для декодирования (опционально. требуется не для всех инстуркций)
	__in __out CELL_ADDRES Desd_out_Addres, //указанный адрес (декодирование значение кладется туда же) 
	__in size_t sizeof_table //колво элементов в таблице
	); //PRIMARY GROUP- декодирует группы адресов указанной инструкнцией

extern void __fastcall  DH_Crypt_supply15_table(
	__in UINT Decode_instruction_s_type, //константа-команда S_TYPE декодирования
	__in ULONG64 Decode_operand__optional, //операнд для декодирования (опционально. требуется не для всех инстуркций)
	__in __out CELL_ADDRES Desd_out_Addres, //указанный адрес (декодирование значение кладется туда же) 
	__in size_t sizeof_table //колво элементов в таблице
	); //BACK GROUP- криптует группы адресов указанной инструкнцией

HANHELD_TYPE __fastcall DH_Get_HANHELD_type (pHANHELD Research_hanheld, //исследуемый hanheld
	__in pREGION_COPY Req_all_VM_insruction_region, //регион с перенесенными данными (инструкциями) вм
	__in pVM_hanheld_addres_pool Req_VM_hanheld_instruction_addr_pool //стек под сохраненение найденных адресов
	);
	/*
	__in HANHELD_ADDRES Hanheld_start_addr, //требуемый rva адрес
	__in HANHELD_TYPE_CRITICAL_DETERMINE_ADDRES pHanheld_critical_determine_addres_array[], //массив критических адресов, определяющих тип хендла
		__in HANHELD_JUMPOUTS_GRAPH Graph_table[] //регион с перенесенными данными (инструкциями) вм
	*/
extern ULONG __fastcall DH_VM_Scan_JUMPOUTs_Init(pHANHELD Research_hanheld, //исследуемый hanheld
	__in pREGION_COPY Req_all_VM_insruction_region, //регион с перенесенными данными (инструкциями) вм
	__in pVM_hanheld_addres_pool Req_VM_hanheld_instruction_addr_pool, //стек под сохраненение найденных адресов
	__in PDISASM Dissasm_struct //выделенная структура (должна быть выделенна заблоговременно - для исключения накладных расходов на malloc/free
	); //функция-родитель. принимает решение по стратегии исследования ветвлений (jumpouts). Исследует ветвления в хендле (островке). Возвращает количество найденных новых ветвлений

extern ULONG __fastcall DH_VM_Scan_JUMPOUTs__fast(__in pHANHELD Research_hanheld,
	__in pREGION_COPY Req_all_VM_insruction_region, 
	__in pVM_hanheld_addres_pool Req_VM_hanheld_instruction_addr_pool,
	__in PDISASM Dissasm_struct, //выделенная структура (должна быть выделенна заблоговременно - для исключения накладных расходов на malloc/free
	__in SERVICE_COUNT i_jumpouts //текущее i в jumpouts массиве
	); //дочерняя функция DH_VM_Scan_JUMPOUTs_Init. выполняет решение по ускоренному исследованию ветвлений (собираются адреса инструкций в ветвлениях, быстрый поиск критических инструкций по уже определенным данным). Возвращает количество найденных новых ветвлений

__inline HANHELD_TYPE DH_VARIATOR(__in PDISASM Read_Dissasm_struct, __in CELL_ADDRES pHanheld_start_addr, __in SERVICE_FLOW_POINTER* pfpInstruction_pool, __in size_t Asm_instruct_size, __in SERVICE_ADDRES Region_start_addr, __in SERVICE_ADDRES Instruct_POOL); //ВАРИАТОР для асм инструкций (для DH_Get_HANHELD_type)

 extern TCHAR* __fastcall DH_VM_HANHELD_type_to_string(
	 HANHELD_TYPE HANHELD_type //тип HANHELD
	 );//конвертирует тип в соотвествующую строку

extern HANHELD_TYPE __fastcall DH_VM_HANHELD_string_To_type(
	TCHAR* HANHELD_type
	); //обратная функа DH_VM_HANHELD_type_to_string. конвертирует строку в соответсвующий тип

extern size_t DH_VM_Dissasm_instruction(
	SERVICE_ADDRES src_addr, //адрес начала инструкции
	char* pOut //буфер вывода мнемоники инстуркции (ASCIIZ !!!)
	); //диссамблирует одну инструкцию по заданному адресу в target-процессе. возвращает размер инструкции

extern pVM_VMPX_PROFILE_INI_STACK __fastcall DH_VM_Do_INIT_Arbitary_VMPX_context(
	__in 	WCHAR *VMPX_profile_name, //имя файла
	__in	SERVICE_TYPE VMP_Version //версия (2 или 3)
	); //инициализирует конфигурацию контекста виртуальной машины для требуемой версии VMP. Возвращает заполненную структуру

extern pVM_VMPX_PROFILE_FOUND_SEQUENCE_STACK __fastcall DH_VM_Scan_VirtualMachine_Contexts(
	__in pREGION_COPY pIncludeVMRegion,
	//инициализация региона выполняется вызывающей функцией
	//	__in SERVICE_ADDRES StartAddres_search_area, //начало сканируемой области с исполняемым кодом
//	__in size_t AreaSize, //размер области
	__in	SERVICE_TYPE VMP_Version, //версия VMP
	__out pPROGRESS_MULTI_SCAN_INFO_STACK pProgressNfo //ссылка на информацию о прогрессе выполнения
	); //производит сканирование заданной области на предмет наличия контекстов виртуальной машины. Возвращает указатель на заполненную структуру VM_VMPX_PROFILE_FOUND_SEQUENCE_STACK

extern pVM_VMPX_PROFILE_INI __fastcall DH_VM_Scan_VirtualMachine_Contexts__Prepeare_Selected_VM_CONTEXT(
	__in	SERVICE_TYPE VMP_Version, //версия VMP
	__in	SERVICE_TYPE Init_HANHELD_Type, //инициализируемый тип контекста (равнозначен HANHELD_TYPE_ENUMS)
	__in	pVMPX_PROFILE_FORCED_INIT_ASM pReq_init_asm_table //таблица обязательно инициализируемых асм инструкций (заполненная структура VMPX_PROFILE_FORCED_INIT_ASM принудительно инициализируемых нструкций)
	);//производная DH_VM_Scan_VirtualMachine_Contexts. Подготавливает необходимые структуры данных для последующей процедуры поиска

extern SERVICE_TYPE __fastcall DH_VM_Scan_VirtualMachine_Contexts__Research_Found_Sequence(
	__in DISASM* Dissasm_struct, //заполненная(!) структуа для дизассемблера
	TARGET_PROC_REGION_COPY* Req_all_VM_insruction_region,  //требуемый регион с присуствующим контекстом VM
	__in pVM_VMPX_PROFILE_INI pVMPX_req_context, //исследуемый контекст
	__out pVM_VMPX_PROFILE_FOUND_SEQUENCE_STACK pScanSequenceStack //указатель для заполнения стека данными
	);//производная DH_VM_Scan_VirtualMachine_Contexts. При нахождении статической цепочки выполняет дальнейшее дизассемблиролвание со следующей асм инструкции и поиск асм инструкций, указанных в юните VM контекста

extern bool __fastcall DH_VM_Scan_VirtualMachine_Contexts__is_static_assembly(
	__in WCHAR* pTestContextAsm, //входящая асм строка соотвествующего профиля VMPX из VM_VMPX_PROFILE_INI
	__in XEDPARSE* pxed_1, //структура XED №1 для сравнения
	__in XEDPARSE* pxed_2 //структура XED №2 для сравнения
	);//производная DH_VM_Scan_VirtualMachine_Contexts. Проводит тест на тип инструкции (статическая - true/динамическая - false). 

extern SERVICE_TYPE __fastcall DH_VM_Scan_VirtualMachine_Contexts__Research_Found_Sequence__without_save(
	__in DISASM* Dissasm_struct, //заполненная(!) структуа для дизассемблера
	TARGET_PROC_REGION_COPY* Req_all_VM_insruction_region,  //требуемый регион с присуствующим контекстом VM
	__in pVM_VMPX_PROFILE_INI pVMPX_req_context //исследуемый контекст
	);//производная DH_VM_Scan_VirtualMachine_Contexts. Копия DH_VM_Scan_VirtualMachine_Contexts__Research_Found_Sequence, за исключением сохранения найденных адресов. Используется только для подсчета элементов указанного контекста или подстверждения от найденной статической инструкции.

extern SERVICE_COUNT __fastcall DH_VM_Scan_VirtualMachine_Contexts__Mark_Duplicate_contexts_from_EXTRACT(
	__in pVM_VMPX_PROFILE_FOUND_SEQUENCE_STACK pSeqStack //заполненный стек найденных последовательностей
	);//производная DH_VM_Scan_VirtualMachine_Contexts. дублированные контексты. отмечает дублирующиеся адреса EXTRACT_extracted_value в структуре стека. Возвращает кол-во дублированных элементов и заносит самостоятельно их стек

extern pVM_THREADWORK __fastcall DH_VM_Supply15_Shedule_threadwork(
	__in pVM_VMPX_PROFILE_FOUND_SEQUENCE_STACK pSeqStack
	); //распределитель задач многопоточного сканирования контекста виртуальной машины по Supply15. Анализ хендлов (островков) с выводом результатов. Возвращает стек планировщика по выполнению многопоточного сканирования

extern bool __fastcall DH_VM_Supply15_Terminate_ALL_Shedule_threadworks(
	__in pVM_THREADWORK* ppThreadWorkStack, //двойной указатель на стек pVM_THREADWORK
	__in SERVICE_COUNT ThreadWork_stack_count //количество элементов в стеке
	); //корректно завершает многопоточное сканирование, если потоки остаются.

bool __fastcall DH_VM_ThreadWork_ClearSet(
	__in pVM_THREADWORK* ppThreadWorkStack, //двойной указатель на стек pVM_THREADWORK
	__in SERVICE_COUNT* pThreadWork_stack_count //указатель на количество элементов в стеке (обнуляется в процедуре по окончании)
	); // DH_VM_Supply15_Terminate_ALL_Shedule_threadworks + чистка стека

static DWORD WINAPI DH_VM_Supply15_Shedule_threadwork_MT(LPVOID Param); //дочерняя функция DH_VM_Supply15_Shedule_threadwork. Выполнение расписания стека задач по исследованию каждого контекста с созданием VMPX_профиля.

OPERATIONAL_ASM_INSTRUCTION __fastcall DH_VM_Supply15_Detect_crypto_method(
	__in pVM_VMPX_PROFILE_FOUND_SEQUENCE pSeqStack, //исследуемая цепочка с известным EXTRACT_VALUE_LINK_address
	__in pREGION_COPY Req_all_VM_insruction_region, //выделенный регион с виртуальной машиной
	__in DISASM* Dissasm_struct, //структура Bea Engine для быстрого доступа
	__out size_t* pCrypto_Operand //вывод константы операнда (если требуется, в зависимости от типа шифрования) по ссылке.
	); //автоматически определяет методы (инструкции) шифрования ячеек с адресами Supply 15.

void __fastcall  DH_VM_Supply15_DEcode_256_table(
	__in size_t Decode_instruction_s_type, //тип шифрования
	__in size_t Decode_operand__optional, //константа-операнд для метода шифрования (если требуется, в зависимости от типа шифрования)
	__in __out CELL_ADDRES First_table_cell //первый элемент в таблице хендлов "островков" Supply15
	);//скоростная расшифровка 256 ячеек Supply15. Перезапись ячеек расширофрованными значениями.

void _fastcall DH_VM_Supply15_Relocation_256_table(
	__in __out CELL_ADDRES First_table_cell//первый элемент в таблице хендлов "островков" Supply15
	);//пересчитывает RVA в таблице с учетом релоков. 

SERVICE_COUNT __fastcall  DH_VM_Supply15_Get_HANHELD_type_256_table(
	__in pVMPX_VM_PROFILE_INFO_STACK pVMPX_info_stack, //инфо-стек
	__in pREGION_COPY Req_all_VM_insruction_region, //требуемый регион с выделенными асм-инструкциями
	__in pVM_AsmAddr_POOL_STACK Req_VM_hanheld_instruction_addr_pool, //пул записываемых адресов для виртуальной машины
	__in DISASM* Dissasm_struct //структура Disasm для BeaEngine
	);//Определение типа 256 хендлов "островков" согласно загруженному контексту. Возвращает количество уникальных островков.

SERVICE_COUNT __fastcall DH_VM_Supply15_Mark_out_Duplicate_HANHELDs_256_table(
	__in __out __in pVMPX_VM_PROFILE_INFO_STACK pVM_Stack_Info_set //исследуемый VM профиля стек
	); //отмечает дублирующиеся адреса хендлов в структуре инфо_стека (только для VMP2).Возвращает кол-во дублированных элементов и заносит самостоятельно их инфо_стек

SERVICE_COUNT __fastcall DH_VMPX_VM_Scan_JUMPOUTs_Init(
	pHANHELD Research_hanheld, //ссылка на исследуемый "островок"
	__in pREGION_COPY Req_all_VM_insruction_region, //требуемый регион с выделенными асм-инструкциями
	__in pVM_hanheld_addres_pool Req_VM_hanheld_instruction_addr_pool, //пул записываемых адресов для виртуальной машины
	__in PDISASM Dissasm_struct //структура Disasm для BeaEngine
	); //инициирует исследование ветвлений хендла (JUMPouts) от условных переходов. возвращает количество новых найденных ветвлений

SERVICE_COUNT DH_VMPX_VM_Scan_JUMPOUTs__fast(
	__in HANHELD_JUMPOUTS_GRAPH* fpCurrentResearchedJumpOutGraph, //плавающий указатель на заполненный граф
	__in pREGION_COPY Req_all_VM_insruction_region, //требуемый регион с выделенными асм-инструкциями
	__in pVM_hanheld_addres_pool Req_VM_hanheld_instruction_addr_pool,  //пул записываемых адресов для виртуальной машины
	__in pHANHELD fpContaint_handler, //ссылка на исследуемый "островок"
	__in PDISASM Dissasm_struct //структура Disasm для BeaEngine
	); //производная DH_VMPX_VM_Scan_JUMPOUTs_Init. Производит индивидуальное исследование одного графа ветвления. Возвращает количество найденных новых ветвлений

bool __fastcall DH_VM_Seq_ClearSet
	(__in pVM_VMPX_PROFILE_FOUND_SEQUENCE_STACK* ppSeqStack,  //двойной указатель на стек pVM_VMPX_PROFILE_FOUND_SEQUENCE_STACK
	__in SERVICE_COUNT* pSequence_stack_count //указатель на количество элементов в стеке (обнуляется в процедуре по окончании)
	); //освобождает память в регионах, выделенную в качестве пула у нас для InludeVM. Окончательно очищает стек найденных цепочек

bool __fastcall DH_VM_Supply15_Calc_HANHELDs_quantitative_report__by_ThreadWork(
	__in pVM_THREADWORK* ppThreadStack, //указатель на начальный элемент pVM_THREADWORK
	__out SERVICE_COUNT* pTotal_Researched_HANHELDS_found, //указатель на переменную для сохранения значения всех найденных хендлов "островков"
	__out SERVICE_COUNT* pTotal_idendeficated_HANHELDS,  //указатель на переменную для сохранения значения всех индетефицированных хендлов "островков" (не может быть больше, чем pTotal_HANHELDS_found)
	__in SERVICE_COUNT Thread_stack_count //количество элементов pVM_THREADWORK
	); //числовой отчет по всем (найденным хендлам || успешно индентифицированным по типу хендлам) ссылаясь на созданные VM профили в планировщике Thread Work.

bool __fastcall DH_VMPX_VM_Profiles__Assign_Name_with_mark_crossref_in_Supply15__by_ThreadWork(
	__in __out pVM_THREADWORK* ppThreadStack, //указатель на начальный элемент pVM_THREADWORK
	__in SERVICE_COUNT Thread_stack_count, //количество элементов pVM_THREADWORK
	__out SERVICE_COUNT* pReport_WillBeCreated_VM_Profiles, //количество полных VM профилей, которые предполагается создать
	__out SERVICE_COUNT* pReport_WillBeCreated_VM_Billets //количество VM заготовок, которые предполагается создать
	); //назначает порядковые имена для файлов профиля согласно версии VMPX-контекста. 

SERVICE_ERROR __fastcall DH_VMPX_Create_VM_Profile_MANUAL(
	__in pVM_Manual_input pManual_VM_Info //вручную введенные данные
	);//создает один контекст виртуальной машины в ручном режиме. С учетом введенных внешних данных.

WCHAR* __fastcall DH_VM_ERROR_type_to_string(
	SERVICE_ERROR VMPX_Error //тип ошибки (соответсвует VMPX_CONTEXT_ERRORS)
	);//конвертирует ошибку в соотвествующую строку

pVM_THREADWORK__OBJECT_SEARCH __fastcall DH_VM_Search_Object_in_containters_MT(
	__in pDH_ENUM_FILES_of_PROFILES pEnumFiles, //ссылка на первый элемент стека pDH_ENUM_FILES_of_PROFILES
	__in SERVICE_COUNT enum_files_count, //количество данных в стеке pDH_ENUM_FILES_of_PROFILES
	__in const TCHAR* pObject_text_ref //текстовая ссылка на обьект поиска
	//bool Ony_GUI_selected_VM_profile //искать только в выделенном GUI контейнере (в первом аргументе подразуевается ссылка на него). Иначе - искать во всех контейнерах VM профилей.
	);//выполняет поиск заданного текста (конвертируется в адрес или служебный тип) в контейнерах VM профилей. МНОГОПОТОЧНАЯ ОПМТИЗАЦИЯ!

bool __fastcall DH_VM_ThreadWork_Object_found_ClearSet(
	__in pVM_THREADWORK__OBJECT_SEARCH pThreadWorkStack
	); //выполняет чистку стека OBJECT_SEARCH

WCHAR* __fastcall DH_VM_FOUND_RESULT_type_to_string(
	SERVICE_TYPE Found_type //установленный тип поискового запроса (согласно enum DH_VM_FOUND_TYPE)
	);//конвертирует установленный тип результата поискового запроса в соотвествующую строку

DWORD WINAPI DH_VM_Shedule_Search_Object_MT(LPVOID Param);

SERVICE_COUNT DH_Mark_out_Duplicate_HANHELDs_in_Supply15(__in __out __in pVM_INFO_STACK VM_Stack_Info_set); //отмечает дублирующиеся адреса хендлов в структуре инфо_стека (только для VMP2).Возвращает кол-во дублированных элементов и заносит самостоятельно их инфо_стек
signed long DH_VM_Output_8_bit_number_arithmetic_result_from_asm_instruction_mnemonic(__in char ArgMnemonic[], __in signed char iImmediat, __in signed long *input_number); //выдает конечный результат с учетом мнемоники add/sub/inc/dec выполняя окончательную операцию с 8 битным Immediat инструкции (по причине того, что add/sub и inc/dec имеют одинаковые опкоды)
signed long DH_VM_Output_32_bit_number_arithmetic_result_from_asm_instruction_mnemonic(__in char ArgMnemonic[], __in signed long iImmediat, __in signed long *input_number); //выдает конечный результат с учетом мнемоники add/sub/inc/dec выполняя окончательную операцию с 32 битным Immediat инструкции (по причине того, что add/sub и inc/dec имеют одинаковые опкоды)

extern  unsigned long DH_VM_Get_NEW_RSP_Enters();


