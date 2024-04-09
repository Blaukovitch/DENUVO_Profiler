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


#define MAX_SYMBOL_STACK_SIZE 500
#define def_TYPEDEF_INREMENTAL_cpu_REGISTERS_size 16
enum TYPEDEF_INREMENTAL_cpu_REGISTERS //регистры CPU и их производные. Повторяют перечислитель BeaEngine - ARGUMENTS_TYPE
{
	INCREMENTAL_CPU_RAX = 1,
	INCREMENTAL_CPU_RCX,
	INCREMENTAL_CPU_RDX,
	INCREMENTAL_CPU_RBX,
	INCREMENTAL_CPU_RSP,
	INCREMENTAL_CPU_RBP,
	INCREMENTAL_CPU_RSI,
	INCREMENTAL_CPU_RDI,

	INCREMENTAL_CPU_R8,
	INCREMENTAL_CPU_R9,
	INCREMENTAL_CPU_R10,
	INCREMENTAL_CPU_R11,
	INCREMENTAL_CPU_R12,
	INCREMENTAL_CPU_R13,
	INCREMENTAL_CPU_R14,
	INCREMENTAL_CPU_R15,
};

//VM disasm size command
#define VM_MAX_DISAASM_SIZE 16

// <RAX>
#define TS_CPU_REG_RAX_ascii "rax"
#define TS_CPU_REG_EAX_ascii "eax"
#define TS_CPU_REG_AX_ascii "ax"
#define TS_CPU_REG_AL_ascii "al"
// <RBX>
#define TS_CPU_REG_RBX_ascii "rbx"
#define TS_CPU_REG_EBX_ascii "ebx"
#define TS_CPU_REG_BX_ascii "bx"
#define TS_CPU_REG_BL_ascii "bl"
// <RCX>
#define TS_CPU_REG_RCX_ascii "rcx"
#define TS_CPU_REG_ECX_ascii "ecx"
#define TS_CPU_REG_CX_ascii "cx"
#define TS_CPU_REG_CL_ascii "cl"
// <RDX>
#define TS_CPU_REG_RDX_ascii "rdx"
#define TS_CPU_REG_EDX_ascii "edx"
#define TS_CPU_REG_DX_ascii "dx"
#define TS_CPU_REG_DL_ascii "dl"
// <R8>
#define TS_CPU_REG_R8_ascii "r8"
#define TS_CPU_REG_R8d_ascii "r8d"
#define TS_CPU_REG_R8w_ascii "r8w"
#define TS_CPU_REG_R8b_ascii "r8b"
// <R9>
#define TS_CPU_REG_R9_ascii "r9"
#define TS_CPU_REG_R9d_ascii "r9d"
#define TS_CPU_REG_R9w_ascii "r9w"
#define TS_CPU_REG_R9b_ascii "r9b"
// <R10>
#define TS_CPU_REG_R10_ascii "r10"
#define TS_CPU_REG_R10d_ascii "r10d"
#define TS_CPU_REG_R10w_ascii "r10w"
#define TS_CPU_REG_R10b_ascii "r10b"
// <R11>
#define TS_CPU_REG_R11_ascii "r11"
#define TS_CPU_REG_R11d_ascii "r11d"
#define TS_CPU_REG_R11w_ascii "r11w"
#define TS_CPU_REG_R11b_ascii "r11b"
// <R12>
#define TS_CPU_REG_R12_ascii "r12"
#define TS_CPU_REG_R12d_ascii "r12d"
#define TS_CPU_REG_R12w_ascii "r12w"
#define TS_CPU_REG_R12b_ascii "r12b"
// <R13>
#define TS_CPU_REG_R13_ascii "r13"
#define TS_CPU_REG_R13d_ascii "r13d"
#define TS_CPU_REG_R13w_ascii "r13w"
#define TS_CPU_REG_R13b_ascii "r13b"
// <R14>
#define TS_CPU_REG_R14_ascii "r14"
#define TS_CPU_REG_R14d_ascii "r14d"
#define TS_CPU_REG_R14w_ascii "r14w"
#define TS_CPU_REG_R14b_ascii "r14b"
// <R15>
#define TS_CPU_REG_R15_ascii "r15"
#define TS_CPU_REG_R15d_ascii "r15d"
#define TS_CPU_REG_R15w_ascii "r15w"
#define TS_CPU_REG_R15b_ascii "r15b"
// <RSI>
#define TS_CPU_REG_RSI_ascii "rsi"
#define TS_CPU_REG_ESI_ascii "esi"
#define TS_CPU_REG_SI_ascii "si"
#define TS_CPU_REG_SIL_ascii "sil"
// <RDI>
#define TS_CPU_REG_RDI_ascii "rdi"
#define TS_CPU_REG_EDI_ascii "edi"
#define TS_CPU_REG_DI_ascii "di"
#define TS_CPU_REG_DIL_ascii "dil"
// <RBP>
#define TS_CPU_REG_RBP_ascii "rbp"
#define TS_CPU_REG_EBP_ascii "ebp"
#define TS_CPU_REG_BP_ascii "bp"
#define TS_CPU_REG_BPL_ascii "bpl"
// <RSP>
#define TS_CPU_REG_RSP_ascii "rsp"
#define TS_CPU_REG_ESP_ascii "esp"
#define TS_CPU_REG_SP_ascii "sp"
#define TS_CPU_REG_SPL_ascii "spl"

//TYPE OF PATCHES enums !!!!!!!!!!!!!!!
enum AVANPOST_SYMBOL_DEFINITIONS_OF_HUNT_TARGET {
	APOST_PEB = 1, //PEB addres
	APOST_MAIN_HMODULE, //HMODULE main
	APOST_KUSER_SHARED_DATA, //KUSER_SHARED_DATA

						//MAP operations//
	APOST_LO_VM_SECTION_BORDER, //LO граница VM секции
	APOST_HI_VM_SECTION_BORDER, //HI граница VM секции

	APOST_PATCH_MAP_LO_BORDER, //LO граница карты патча
	APOST_PATCH_MAP_HI_BORDER, //HI граница карты патча
	APOST_PATCH_MAP_vs_VM_SECTION_DIFFERENT, //дифферент между секцией VM и картой патча

	APOST_ORIGINAL_VM_SECTION_LO_BORDER, //LO граница оригинальной секции
	APOST_ORIGINAL_VM_SECTION_HI_BORDER, //HI граница оригинальной секции
	APOST_ORIGINAL_VM_SECTION_vs_VM_SECTION_DIFFERENT, //дифферент между оригинальной секцией VM и картой патча

	APOST_HOOK_AND_FILTERS_AREA_LO_BORDER, //LO граница кода хука и фильтров
	APOST_HOOK_AND_FILTERS_AREA_HI_BORDER, //HI граница кода хука и фильтров
	APOST_HOOK_AND_FILTERS_AREA_vs_VM_SECTION_DIFFERENT //дифферент между оригинальной секцией VM и кодом хуков и фильтров
	};

//++++++++++++++++++++++ structs ++++++++++++++++++++++

typedef struct DH_APOST_symbol_stack_approved
{
	SERVICE_TYPE Symbol_type_as_enum; //тип символа по AVANPOST_SYMBOL_DEFINITIONS_OF_HUNT_TARGET
	TCHAR Symbol_full_String[MAX_PATH]; //полная строка символа
	size_t Symbol_full_String_len; //длина полной строки символа

	TCHAR Symbol_SHORT_assign; //короткое обозначение символа (для vmp_x) по стандарту
	
	size_t Variable; //числовое значение символа
	TCHAR Variable_str[50]; //строковое значение символа
	size_t Variable_str_len; //длина строкового значения
}DH_APOST_symbol_stack, *pDH_APOST_symbol_stack;


//******************************* F U N C! ***************************************
extern  bool __fastcall DH_APOST_ADDNS(
	 __in pDH_APOST_symbol_stack pTmpSymbol_struct //временная структура для передачи информации о символе
	 ); //ДОБАВЛЯЕТ значение символа в стек