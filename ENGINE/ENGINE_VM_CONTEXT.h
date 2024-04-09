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

//------------------ define data ------------------ 

 #define HANHELD_TYPE_ENUMS_COUNT 110 //КОЛИЧЕСТВО ТИПОВ "ОСТРОВКОВ"
// >>>>>>>>>>>>>>>> HANHELD_TYPE >>>>>>>>>>>>>> //
enum HANHELD_TYPE_ENUMS {HANHELD_TYPE_DETERMINE_ERROR = 0, HANHELD_TYPE_VM_ENTER, HANHELD_TYPE_SWITCH_ENTER, HANHELD_TYPE_VM_EXIT, HANHELD_TYPE_CALL, HANHELD_TYPE_RDTSC, HANHELD_TYPE_CPUID,
	HANHELD_TYPE_CRC_CHECKL,
	HANHELD_TYPE_XOR_STACK_RSP_DWORD, 
	HANHELD_TYPE_POPFQ, //RESTORE EFL (QWORD)
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
	HANHELD_TYPE_MOV_read_byte_PCODE,

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
	//read/write variables
	HANHELD_TYPE_MOV_struct_reg8, //mov dword ptr [rax + rdi], dl
	HANHELD_TYPE_MOV_struct_reg16, //mov dword ptr [rax + rdi], dx
	HANHELD_TYPE_MOV_struct_reg32, //mov dword ptr [rax + rdi], edx
	HANHELD_TYPE_MOV_struct_reg64, //mov dword ptr [rax + rdi], rdx
	HANHELD_TYPE_MOV_reg8_struct, //mov dl, byte ptr [rax + rdi]
	HANHELD_TYPE_MOV_reg16_struct, //mov al, word ptr [rax + rdi]
	HANHELD_TYPE_MOV_reg32_struct, //mov edx, dword ptr [rax + rdi]
	HANHELD_TYPE_MOV_reg64_struct, //mov rdx, qword ptr [rax + rdi]

	//PUSH_VStack
	HANHELD_TYPE_PUSH_VStack_qword,
	HANHELD_TYPE_PUSH_VStack_dword,
	HANHELD_TYPE_PUSH_VStack_word,
	HANHELD_TYPE_PUSH_VStack_byte,
	//POP_VStack
	HANHELD_TYPE_POP_VStack_qword,
	HANHELD_TYPE_POP_VStack_dword,
	HANHELD_TYPE_POP_VStack_word,
	HANHELD_TYPE_POP_VStack_byte,

	//SHIFT_ROT
	HANHELD_TYPE_SHRD_64, //shrd rax, rdx, cl
	HANHELD_TYPE_SHLD_64, //shrd rax, rdx, cl
	HANHELD_TYPE_SHRD_32, //shrd eax, edx, cl
	HANHELD_TYPE_SHLD_32, //shld eax, edx, cl
	HANHELD_TYPE_SHRD_16, //shrd ax, dx, cl
	HANHELD_TYPE_SHLD_16, //shld ax, dx, cl
	HANHELD_TYPE_SHIFT_R_reg64, //right
	HANHELD_TYPE_SHIFT_R_reg32,
	HANHELD_TYPE_SHIFT_R_reg16,
	HANHELD_TYPE_SHIFT_R_reg8,
	HANHELD_TYPE_SHIFT_L_reg64, //left
	HANHELD_TYPE_SHIFT_L_reg32,
	HANHELD_TYPE_SHIFT_L_reg16,
	HANHELD_TYPE_SHIFT_L_reg8,

	//DIV - INTEGER DIV
	HANHELD_TYPE_DIV_qword_stack, //div qword ptr [rbp + 0x10]
	HANHELD_TYPE_DIV_dword_stack, //div dword ptr [rbp + 0xC]
	HANHELD_TYPE_DIV_reg64, //div rcx
	HANHELD_TYPE_DIV_reg32, 
	HANHELD_TYPE_DIV_reg16,
	HANHELD_TYPE_DIV_reg8,
	HANHELD_TYPE_Int_DIV_qword_stack, //idiv
	HANHELD_TYPE_Int_DIV_dword_stack, //idiv
	HANHELD_TYPE_Int_DIV_reg64, //idiv
	HANHELD_TYPE_Int_DIV_reg32,
	HANHELD_TYPE_Int_DIV_reg16,
	HANHELD_TYPE_Int_DIV_reg8,

	//MUL - INTEGER MUL
	HANHELD_TYPE_MUL_reg64,
	HANHELD_TYPE_MUL_reg32,
	HANHELD_TYPE_MUL_reg16,
	HANHELD_TYPE_MUL_reg8,
	HANHELD_TYPE_INTEGER_MUL_reg64,
	HANHELD_TYPE_INTEGER_MUL_reg32,
	HANHELD_TYPE_INTEGER_MUL_reg16,
	HANHELD_TYPE_INTEGER_MUL_reg8,

	//ADD
	HANHELD_TYPE_ADD_qword_stack, //add dword ptr [rbp + 8], rax
	HANHELD_TYPE_ADD_dword_stack, //add dword ptr [rbp + 8], eax
	HANHELD_TYPE_ADD_word_stack, //add dword ptr [rbp + 8], ax
	HANHELD_TYPE_ADD_byte_stack, //add byte ptr [rbp + 8], al

	//AND
	HANHELD_TYPE_AND_qword_stack, //and dword ptr [rbp + 8], rax
	HANHELD_TYPE_AND_dword_stack, //and dword ptr [rbp + 8], eax
	HANHELD_TYPE_AND_word_stack, //and dword ptr [rbp + 8], ax
	HANHELD_TYPE_AND_byte_stack, //and dword ptr [rbp + 8], ax
	//NOT-AND
	HANHELD_TYPE_not_and_reg64, //NOT rax - NOT rdx - and rax, rdx
	HANHELD_TYPE_not_and_reg32,
	HANHELD_TYPE_not_and_reg16,
	HANHELD_TYPE_not_and_reg8,

	HANHELD_TYPE_MOVS_byte, //MOVSB - movzx eax, byte ptr [rsi], mov [rbp], rax
	HANHELD_TYPE_MOVS_dword, //MOVSD

	//GET_RND func
	HANHELD_TYPE_GET_RND_const_qword,
	HANHELD_TYPE_GET_RND_const_dword,
	HANHELD_TYPE_GET_RND_const_word,
	HANHELD_TYPE_GET_RND_const_byte,
	//special SERVICE !!!!!!!!!!!!!!!
	HANHELD_TYPE_HANHELD,

	HANHELD_TYPE_REACH_HANHELD_END = -1
};

//стратегические примитивы
#define TS_VM__SERVICE_TYPE__hanheld _T("__hanheld_unit")
#define TS_VM__HANHELD_TYPE_VM_ENTER _T("VM_ENTER")
#define TS_VM__HANHELD_TYPE_SWITCH_ENTER _T("VM_S_ENTER")
#define TS_VM__HANHELD_TYPE_VM_EXIT _T("VM_EXIT")

//второстепенные стратегические примитивы
#define TS_VM__HANHELD_TYPE_CALL _T("CALL")
#define TS_VM__HANHELD_TYPE_RDTSC _T("RDTSC")
#define TS_VM__HANHELD_TYPE_CPUID _T("CPUID")
#define TS_VM__HANHELD_TYPE_CRC_CHECKL _T("_CRC_CHECKL")
#define TS_VM__HANHELD_TYPE_XOR_STACK_RSP_DWORD _T("XOR_RSP_dword")
#define TS_VM__HANHELD_TYPE_POPFQ  _T("POPFQ")

//чтение из Data Segment
#define TS_VM__HANHELD_TYPE_MOV_reg8_memreg64_DS _T("_READ_DS_BYTE")
#define TS_VM__HANHELD_TYPE_MOV_reg16_memreg64_DS _T("_READ_DS_WORD")
#define TS_VM__HANHELD_TYPE_MOV_reg32_memreg64_DS _T("_READ_DS_DWORD")
#define TS_VM__HANHELD_TYPE_MOV_reg64_memreg64_DS _T("_READ_DS_QWORD")
//запись в Data Segment
#define TS_VM__HANHELD_TYPE_MOV_memreg64_reg8_DS _T("_WRITE_DS_BYTE")
#define TS_VM__HANHELD_TYPE_MOV_memreg64_reg16_DS _T("_WRITE_DS_WORD")
#define TS_VM__HANHELD_TYPE_MOV_memreg64_reg32_DS _T("_WRITE_DS_DWORD")
#define TS_VM__HANHELD_TYPE_MOV_memreg64_reg64_DS _T("_WRITE_DS_QWORD")
//чтение p-code
#define TS_VM__HANHELD_TYPE_MOV_read_qword_PCODE _T("_READ_QWORD_pcode")
#define TS_VM__HANHELD_TYPE_MOV_read_dword_PCODE _T("_READ_DWORD_pcode")
#define TS_VM__HANHELD_TYPE_MOV_read_word_PCODE _T("_READ_WORD_pcode")
#define TS_VM__HANHELD_TYPE_MOV_read_byte_PCODE _T("_READ_BYTE_pcode")
//чтение-запись в Stack Segment
#define TS_VM__HANHELD_TYPE_MOV_reg8_memreg64_SS _T("_READ_ss_byte")
#define TS_VM__HANHELD_TYPE_MOV_reg16_memreg64_SS _T("_READ_ss_word")
#define TS_VM__HANHELD_TYPE_MOV_reg32_memreg64_SS _T("_READ_ss_dword")
#define TS_VM__HANHELD_TYPE_MOV_reg64_memreg64_SS _T("_READ_ss_qword")
#define TS_VM__HANHELD_TYPE_MOV_memreg64_reg8_SS _T("_WRITE_ss_byte")
#define TS_VM__HANHELD_TYPE_MOV_memreg64_reg16_SS _T("_WRITE_ss_word")
#define TS_VM__HANHELD_TYPE_MOV_memreg64_reg32_SS _T("_WRITE_ss_dword")
#define TS_VM__HANHELD_TYPE_MOV_memreg64_reg64_SS _T("_WRITE_ss_qword")
//чтение-запись в General Segment
#define TS_VM__HANHELD_TYPE_MOV_reg8_memreg64_GS _T("_READ_gs_byte")
#define TS_VM__HANHELD_TYPE_MOV_reg16_memreg64_GS _T("_READ_gs_word")
#define TS_VM__HANHELD_TYPE_MOV_reg32_memreg64_GS _T("_READ_gs_dword")
#define TS_VM__HANHELD_TYPE_MOV_reg64_memreg64_GS _T("_READ_gs_qword")
#define TS_VM__HANHELD_TYPE_MOV_memreg64_reg8_GS _T("_WRITE_gs_byte")
#define TS_VM__HANHELD_TYPE_MOV_memreg64_reg16_GS _T("_WRITE_gs_word")
#define TS_VM__HANHELD_TYPE_MOV_memreg64_reg32_GS _T("_WRITE_gs_dword")
#define TS_VM__HANHELD_TYPE_MOV_memreg64_reg64_GS _T("_WRITE_gs_qword")
//логические операции - shift/rot
#define TS_VM__HANHELD_TYPE_SHRD_64 _T("SHRD_qword")
#define TS_VM__HANHELD_TYPE_SHLD_64 _T("SHLD_qword")
#define TS_VM__HANHELD_TYPE_SHRD_32 _T("SHRD_dword")
#define TS_VM__HANHELD_TYPE_SHLD_32 _T("SHLD_dword")
#define TS_VM__HANHELD_TYPE_SHRD_16 _T("SHRD_word")
#define TS_VM__HANHELD_TYPE_SHLD_16 _T("SHLD_word")
#define TS_VM__HANHELD_TYPE_SHIFT_R_reg64 _T("SHR_qword")
#define TS_VM__HANHELD_TYPE_SHIFT_R_reg32 _T("SHR_dword")
#define TS_VM__HANHELD_TYPE_SHIFT_R_reg16 _T("SHR_word")
#define TS_VM__HANHELD_TYPE_SHIFT_R_reg8 _T("SHR_byte")
#define TS_VM__HANHELD_TYPE_SHIFT_L_reg64 _T("SHL_qword")
#define TS_VM__HANHELD_TYPE_SHIFT_L_reg32 _T("SHL_dword")
#define TS_VM__HANHELD_TYPE_SHIFT_L_reg16 _T("SHL_word")
#define TS_VM__HANHELD_TYPE_SHIFT_L_reg8  _T("SHL_byte")
//чтение/запись переменных VM
#define TS_VM__HANHELD_TYPE_MOV_struct_reg8 _T("_WRITE_VAR_BYTE")
#define TS_VM__HANHELD_TYPE_MOV_struct_reg16 _T("_WRITE_VAR_WORD")
#define TS_VM__HANHELD_TYPE_MOV_struct_reg32 _T("_WRITE_VAR_DWORD")
#define TS_VM__HANHELD_TYPE_MOV_struct_reg64 _T("_WRITE_VAR_QWORD")
#define TS_VM__HANHELD_TYPE_MOV_reg8_struct _T("_READ_VAR_BYTE")
#define TS_VM__HANHELD_TYPE_MOV_reg16_struct _T("_READ_VAR_WORD")
#define TS_VM__HANHELD_TYPE_MOV_reg32_struct _T("_READ_VAR_DWORD")
#define TS_VM__HANHELD_TYPE_MOV_reg64_struct _T("_READ_VAR_QWORD")
//операции с VStack
#define TS_VM__HANHELD_TYPE_PUSH_VStack_qword _T("_PUSH_VStack_qword")
#define TS_VM__HANHELD_TYPE_PUSH_VStack_dword _T("_PUSH_VStack_dword")
#define TS_VM__HANHELD_TYPE_PUSH_VStack_word _T("_PUSH_VStack_word")
#define TS_VM__HANHELD_TYPE_PUSH_VStack_byte _T("_PUSH_VStack_byte")
#define TS_VM__HANHELD_TYPE_POP_VStack_qword _T("_POP_VStack_qword")
#define TS_VM__HANHELD_TYPE_POP_VStack_dword _T("_POP_VStack_dword")
#define TS_VM__HANHELD_TYPE_POP_VStack_word _T("_POP_VStack_word")
#define TS_VM__HANHELD_TYPE_POP_VStack_byte _T("_POP_VStack_byte")
//арифметические операции - сложение (стек)
#define TS_VM__HANHELD_TYPE_ADD_qword_stack _T("ADD_qword_stack")
#define TS_VM__HANHELD_TYPE_ADD_dword_stack _T("ADD_dword_stack")
#define TS_VM__HANHELD_TYPE_ADD_word_stack _T("ADD_word_stack")
#define TS_VM__HANHELD_TYPE_ADD_byte_stack _T("ADD_byte_stack")
//арифметические операции - деление с остатком
#define TS_VM__HANHELD_TYPE_DIV_qword_stack _T("DIV_qword_stack")
#define TS_VM__HANHELD_TYPE_DIV_dword_stack _T("DIV_dword_stack")
#define TS_VM__HANHELD_TYPE_DIV_reg64 _T("DIV_qword")
#define TS_VM__HANHELD_TYPE_DIV_reg32 _T("DIV_dword")
#define TS_VM__HANHELD_TYPE_DIV_reg16 _T("DIV_word")
#define TS_VM__HANHELD_TYPE_DIV_reg8 _T("DIV_byte")
//арифметические операции - целочисленное деление
#define TS_VM__HANHELD_TYPE_Int_DIV_qword_stack _T("Int_DIV_qword_stack")
#define TS_VM__HANHELD_TYPE_Int_DIV_dword_stack _T("Int_DIV_dword_stack")
#define TS_VM__HANHELD_TYPE_Int_DIV_reg64 _T("Int_DIV_qword")
#define TS_VM__HANHELD_TYPE_Int_DIV_reg32 _T("Int_DIV_dword")
#define TS_VM__HANHELD_TYPE_Int_DIV_reg16 _T("Int_DIV_word")
#define TS_VM__HANHELD_TYPE_Int_DIV_reg8  _T("Int_DIV_byte")
//арифметические операции - умножение
#define TS_VM__HANHELD_TYPE_MUL_reg64 _T("MUL_qword")
#define TS_VM__HANHELD_TYPE_MUL_reg32 _T("MUL_dword")
#define TS_VM__HANHELD_TYPE_MUL_reg16 _T("MUL_word")
#define TS_VM__HANHELD_TYPE_MUL_reg8 _T("MUL_byte")
//арифметические операции - целочисленное умножение
#define TS_VM__HANHELD_TYPE_INTEGER_MUL_reg64 _T("Int_MUL_qword")
#define TS_VM__HANHELD_TYPE_INTEGER_MUL_reg32 _T("Int_MUL_dword")
#define TS_VM__HANHELD_TYPE_INTEGER_MUL_reg16 _T("Int_MUL_word")
#define TS_VM__HANHELD_TYPE_INTEGER_MUL_reg8 _T("Int_MUL_byte")
//логические операции - И (&)
#define TS_VM__HANHELD_TYPE_AND_qword_stack _T("AND_qword_stack")
#define TS_VM__HANHELD_TYPE_AND_dword_stack _T("AND_dword_stack")
#define TS_VM__HANHELD_TYPE_AND_word_stack _T("AND_word_stack")
#define TS_VM__HANHELD_TYPE_AND_byte_stack _T("AND_byte_stack")
#define TS_VM__HANHELD_TYPE_not_and_reg64 _T("NOT_AND_qword")
#define TS_VM__HANHELD_TYPE_not_and_reg32 _T("NOT_AND_dword")
#define TS_VM__HANHELD_TYPE_not_and_reg16 _T("NOT_AND_word")
#define TS_VM__HANHELD_TYPE_not_and_reg8 _T("NOT_AND_byte")
//операции копирования больших объемов данных
#define TS_VM__HANHELD_TYPE_MOVS_byte _T("rep_MOVSB_byte")
#define TS_VM__HANHELD_TYPE_MOVS_dword _T("rep_MOVSD_dword")
//операции получения RANDOM констант
#define TS_VM__HANHELD_TYPE_GET_RND_const_qword _T("_GET_RND_const_qword")
#define TS_VM__HANHELD_TYPE_GET_RND_const_dword _T("_GET_RND_const_dword")
#define TS_VM__HANHELD_TYPE_GET_RND_const_word  _T("_GET_RND_const_word")
#define TS_VM__HANHELD_TYPE_GET_RND_const_byte  _T("_GET_RND_const_byte")
