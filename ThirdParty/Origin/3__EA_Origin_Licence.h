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
//#include "3__EA_Origin_errors_stack.h"

//3rd pary code
#define dbea1__SIG_1 0x1825364E
#define dbea1__SIG_2 0xAD31EDD9

#define KEY_LEN 64
#define KEY_sick 10
#define DEF_CRYPTO_BLOCK_sick 10
#define end_base 61

#pragma pack(1)
typedef struct dlf_file_header
{
	BYTE b1;
	BYTE fake1;
	BYTE b3;
	BYTE b4;
	BYTE bytes[20];
	BYTE b5;
	BYTE b6;
	BYTE fake2;
	BYTE fake3;
	BYTE bytes2[37];
}DLF_head, *pDLF_head;


typedef struct input_arg1_table
{
	size_t block_size;
	LPDWORD pCryptoBlock;
	DWORD BuilCrTable[68];
}Arg1_crypto_info, *pArg1_crypto_info;

typedef struct X_TABLE_Info_DWORD
{
	unsigned short unk1; //00 01
	unsigned short table_dword_count; //кол-во DWORDs
}X_table_intro, *pX_table_intro;
#pragma pack()
//0000000141813C80 qtable_shift_byte_2 dq 80h dup(0)
//00000001415EF750 qtable_shift8_2 dq 80h dup(0)  
//0000000141910900 qtable_shift16  dq 80h dup(0)  
//0000000141A4BC00 qtable_shift16_2 dq 80h dup(0)  
//00000001414D1260 qtable_shift24_2 dq 80h dup(0)
//00000001419F3980 qtable_def_2    dq 20h dup(0)  
//00000001416FF870 qtable_shift_byte dq 80h dup(0)
//00000001418E0F00 qtable_shift8   dq 80h dup(0) 
//000000014155A4F0 qtable_shift24  dq 80h dup(0)
//0000000141793770 qtable_def      dq 20h dup(0) 


//составные функции DECRYPT
// ^^^^^^^^^^^^^^^^^^^^^^^ SUBs ^^^^^^^^^^^^^^^^^^^^^^^
bool __fastcall EA_Origin__Full_DECODE_stage(__in LPBYTE pSourceReadData, __in size_t SourceSize, __out char* pXML_Container,  __out char* pXML_Tag_value, __out LPDWORD pX_TABLE); //полный цикл декодирования (дефолтный)

extern size_t __fastcall EA_Origin__dbeadata_seek_signature1(__in LPBYTE pSource, __in size_t SourceSize); //сигнатура 1
extern size_t __fastcall EA_Origin__dbeadata_verify_HASH(__in DWORD* pSource, __in size_t SourceDWORDSize); //хэш до сигнатуры 1
extern size_t  EA_Origin__DLF_key_wick_decrypt(__in byte* pKEY_wick, __in LPBYTE pSource, __in size_t SourceDWORDSize, __in DWORD* pKEY_sick, __in BYTE* pKey_HOSE); //билд ключа
extern size_t __fastcall decrypt_XML(LPDWORD a1_pSick, size_t a2_unk, size_t a3_SourceSize, LPBYTE a4_pDecryptTable, LPDWORD pSource, LPBYTE pOut); //извлечение XML-контейнера
extern  __int64 qwtable(LPDWORD a1, size_t a2, LPDWORD pSource, LPBYTE pOut); //составная qwtable
extern size_t __fastcall FoundXMLTag(char* pSource, size_t SourceSize, const char* pTagName, char* pExtractedTagValue); //извлечение value из заданного тега
extern LPBYTE __fastcall Decrypt_SecretTable(char* pSource, size_t SourceSize, LPDWORD pLick, LPBYTE pResult); //декодирование секретной части
