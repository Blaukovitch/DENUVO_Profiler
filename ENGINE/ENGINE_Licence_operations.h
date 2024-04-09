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
#include "ENGINE_Profile_operations.h"
#include "KI_USER.h"
#include "W_PEB.h"
//3rd pary code
#include "../ThirdParty/Origin/3__EA_Origin_STUB.h"
#include "../ThirdParty/Origin/3__EA_Origin_Licence.h"

//------------------ define data ------------------ 
#define MM_SHARED_USER_DATA_VA      0x7FFE0000
#define USER_SHARED_DATA ((KUSER_SHARED_DATA * const)MM_SHARED_USER_DATA_VA)

#define LIC_MAX_PE_HEADER_SIZE 0x1000

#define MAX_REPEAT_Check_KUSER_SYSTEM_TIME_CYCLE 3
#define MAX_Sleep_time_in_check_KUSER_SYSTEM_TIME_CYCLE 2

#define defKUSER_KSYSTEMTIME1_addres ((ULONG64)MM_SHARED_USER_DATA_VA + 8)
#define defKUSER_KSYSTEMTIME2_addres ((ULONG64)MM_SHARED_USER_DATA_VA + 20)
#define defKUSER_CryptoExponen_offset 0x023C
#define defKUSER_NtMajorVersion_offset 0x026C
#define defKUSER_NtMinorVersion_offset 0x0270
#define defKUSER_ProcessorFeatures_offset 0x0274
#define defKUSER_TimeSlip_offset 0x02BC
#define defKUSER_NumberOfPhysicalPages_offset 0x02E8
#define defKUSER_NtSystemRoot_offset 0x030

#define defIMAGEDIRECTORY_Data_Directory_of_NtHeader_offset 0x88
#define defIMAGEDIRECTORY_Export_Data_Directory__RVA_offset 0x0
#define defIMAGEDIRECTORY_Export_Data_Directory__Value_offset 0x4
#define defIMAGEDIRECTORY_Import_Data_Directory__RVA_offset 0x8
#define defIMAGEDIRECTORY_Import_Data_Directory__Value_offset 0xC
#define defIMAGEDIRECTORY_Resource_Data_Directory__RVA_offset 0x10
#define defIMAGEDIRECTORY_Resource_Data_Directory__Value_offset 0x14
#define defIMAGEDIRECTORY_Exception_Data_Directory__RVA_offset 0x18
#define defIMAGEDIRECTORY_Exception_Data_Directory__Value_offset 0x1C
#define defIMAGEDIRECTORY_Relocation_Data_Directory__RVA_offset 0x28
#define defIMAGEDIRECTORY_Relocation_Data_Directory__Value_offset 0x2C
#define defIMAGEDIRECTORY_Debug_Data_Directory__RVA_offset 0x30
#define defIMAGEDIRECTORY_Debug_Data_Directory__Value_offset 0x34

#define defIPEB__OSMajorVersion_Value_offset 0x118
#define defIPEB__OSMinorVersion_Value_offset 0x11C
#define defIPEB__ImageSubSystemMajorVersion_Value_offset 0x12C
#define defIPEB__ImageSubSystemMinorVersion_Value_offset 0x130
#define defIPEB__NumberOfProcessors_Value_offset 0xB8
#define defIPEB__OSPlatformId_SHORT_Value_offset 0x122

/*
где хранит лицензии денуво который навешан на играх в origin?
Steam здесь: c:\Program Files\Steam\userdata\<SteamID>\<AppID>\dbdata
Uplay здесь: c:\ProgramData\dbdata\<ID>\<Other_ID>.dbdata
Origin здесь: c:\ProgramData\Electronic Arts\EA Services\License\
*/
#define STD_DEF_DIR_RET L"."
#define STD_DEF_DIR_RET_a L".."

#define LICENCE_DEF_PATH__EA_ORIGIN L"C:\\ProgramData\\Electronic Arts\\EA Services\\License\\"
#define LICENCE__EA_ORIGIN_DLC_LIC_EXTENTION_CRITICAL L".dlf"
#define LICENCE__EA_ORIGIN_DLC_LIC_EXTE_FIND L"*.dlf"
#define LICENCE_FULL_DEF_PATH_EXTE_FIND_EA_ORIGIN L"C:\\ProgramData\\Electronic Arts\\EA Services\\License\\*.dlf"

#define LICENCE_DEF_PATH__VALVE_STEAM L"C:\\Program Files (x86)\\Steam\\userdata\\"
#define LICENCE_DEF_SUBPATH__VALVE_STEAM_EXT_FIND L"\\dbdata\\*"
#define LICENCE_DEF_SUBPATH__VALVE_STEAM L"\\dbdata"
#define LICENCE__VALVE_STEAM_DLC_LIC_EXTENTION_CRITICAL L""
#define LICENCE__VALVE_STEAM_DLC_LIC_EXTE_FIND L"*"
//#define LICENCE_FULL_DEF_PATH_EXTE_FIND_EA_ORIGIN L"C:\\ProgramData\\Electronic Arts\\EA Services\\License\\*.dlf"


#define MAX_XML_DATA_BYTESIZE 700000
#define MAX_XML_VALUE_BYTESIZE MAX_XML_DATA_BYTESIZE/10
#define MAX_SECRET_DWORD_TABLE 1000
#define MAX_SECRET_DWORD_ASCII_TABLE MAX_SECRET_DWORD_TABLE*4

#define ENGLISH_ALPHABET_COUNT 26
// >>>>>>>>>>>>>>>>>>>> e n u m <<<<<<<<<<<<<<<<<<<<<<<<<<<
enum eDLC_TYPE
{
	LICENCE_DLC_EA_ORIGIN = 1,
	LICENCE_DLC_VAVLE_STEAM
};

//++++++++++++++++++++++ structs consist of ++++++++++++++++++++++
typedef struct DH_CPUID_REGISTER_enumerator
{
	unsigned long unsigned long reg_EAX;
	unsigned long unsigned long reg_EBX;
	unsigned long unsigned long reg_ECX;
	unsigned long unsigned long reg_EDX;
}REGISTER_enumerator, *pREGISTER_enumerator;

typedef struct DH_CPUID_lic_block
{
	REGISTER_enumerator EAX_1; //Additional Information / Feature Information
	REGISTER_enumerator EAX_80000002; //Processor Brand String
	REGISTER_enumerator EAX_80000003; //Processor Brand String продолжение
	REGISTER_enumerator EAX_80000004; //Processor Brand String окончаение
	UINT64 HASH; //Hash
}CPUID_licence_stack, *pCPUID_licence_stack;

typedef struct DH_KUSER_SHARED_DATA_lic_block
{
	//aviable data
	WCHAR NtSystemRoot[MAX_PATH]; // в 99% случаев "C:\Windows" в unicode 
	ULONG NtMajorVersion; //Major-значение (1 - для Windows 7)
    ULONG NtMinorVersion; //Minor-значение (6 - для Windows 7)
	UCHAR ProcessorFeatures[64]; 
	ULONG NumberOfPhysicalPages; 
	//n/a, but access
	 ULONG CryptoExponent;
	 ULONG TimeSlip;

	 //ADDRESES
	 PVOID RVA_NtSystemRoot;
	 PVOID RVA_NtMajorVersion;
	 PVOID RVA_NtMinorVersion;
	 PVOID RVA_ProcessorFeatures;
	 PVOID RVA_NumberOfPhysicalPages;
	 PVOID RVA_CryptoExponent;
	 PVOID RVA_TimeSlip;

	 UINT64 HASH; //Hash
}KUSER_SHARED_DATA_licence_stack, *pKUSER_SHARED_DATA_licence_stack;

typedef struct DH_IMAGE_DATA_DIRECTORY_lic_block
{
	WCHAR Service_DllName[MAX_PATH]; //service use! for DH!
	HMODULE Service_Module_handle; //HMODULE from hive process
	//critical IMAGE_DATA_DIRECTORY data
	ULONG ExportDirectoryVA;
	ULONG ExportDirectorySize;

	ULONG ImportDirectoryVA;
	ULONG ImportDirectorySize;

	ULONG ResourseDirectoryVA;
	ULONG ResourseDirectorySize;

	ULONG ExceptionDirectoryVA;
	ULONG ExceptionDirectorySize;

	ULONG RelocationDirectoryVA;
	ULONG RelocationDirectorySize;

	ULONG DebugDirectoryVA;
	ULONG DebugDirectorySize;

	//ADDRESES
	 PVOID RVA_ExportDirectoryVA;
	 PVOID RVA_ExportDirectorySize;

	 PVOID RVA_ImportDirectoryVA;
	 PVOID RVA_ImportDirectorySize;

	 PVOID RVA_ResourseDirectoryVA;
	 PVOID RVA_ResourseDirectorySize;

	 PVOID RVA_ExceptionDirectoryVA;
	 PVOID RVA_ExceptionDirectorySize;
	 
	 PVOID RVA_RelocationDirectoryVA;
	 PVOID RVA_RelocationDirectorySize;

	 PVOID RVA_DebugDirectoryVA;
	 PVOID RVA_DebugDirectorySize;

	 SERVICE_HASH HASH; //Hash
}IMAGE_DATA_DIRECTORY_licence_stack, *pIMAGE_DATA_DIRECTORY_licence_stack;

typedef struct DH_PEB_lic_block
{
  ULONG                   OSMajorVersion;
  ULONG                   OSMinorVersion;
  ULONG                   ImageSubSystemMajorVersion;
  ULONG                   ImageSubSystemMinorVersion;
  ULONG                   NumberOfProcessors;
  USHORT				  OSPlatformId_short;

  //ADDRESES
  PVOID RVA_OSMajorVersion;
  PVOID RVA_OSMinorVersion;
  PVOID RVA_ImageSubSystemMajorVersion;
  PVOID RVA_ImageSubSystemMinorVersion;
  PVOID RVA_NumberOfProcessors;
  PVOID RVA_OSPlatformId_short;

  UINT64 HASH; //Hash
}PEB_licence_stack, *pPEB_licence_stack;


typedef struct DH_DLC_infostack
{
	WIN32_FIND_DATAW FileFIND_DAT; //имя файла DLC
	SERVICE_TYPE DLC_TYPE; //согласно перечислителя eDLC_TYPE
	size_t ApproxVersion; //предполагаемая версия - для определения активного расшифровщика/декриптера
	TCHAR Path[MAX_PATH];
	bool isVerify; //проверен на соотвествие
}LIC_DLC_FILE, *pLIC_DLC_FILE;

typedef struct DH_DLC_infostack_info_struct
{
	LIC_DLC_FILE FileUnit[MAX_CONTAINTER_FILES]; //перечень файлов
	SERVICE_COUNT NumofFoundFiles; //общее кол-во найденных файлов
	//из них:
	SERVICE_COUNT EA_ORIGIN_licence; //Electronic Arts Origin
	SERVICE_COUNT Vavle_Steam_licence; //Vale Steam
}LIC_DLC_FileInfo_STACK, *pLIC_DLC_FileInfo_STACK; //стек по всем файлам DLC

typedef struct DH_DLC_DECRYPT_info_struct
{
	char XML_DATA[MAX_XML_DATA_BYTESIZE]; //XML-контейнер
	char XML_TagString[MAX_XML_VALUE_BYTESIZE]; //отдельно извлеченный value из тега
	size_t TagStringSize; //размер value

	DWORD X_TABLE[MAX_SECRET_DWORD_TABLE]; //таблица DWORDs
	char X_TABLE_ASCII[MAX_SECRET_DWORD_ASCII_TABLE]; //таблица DWORDs в char 
	SERVICE_COUNT X_TABLE_size; //её реальный размер
	SERVICE_COUNT X_TABLE_DWORDs_count; //кол-во DWORDs
	double X_TABLE_entropy; //энтропия таблицы
}DH_LICENCE_DECODE_DLC_FILE_DATA, *pDH_LICENCE_DECODE_DLC_FILE_DATA;

//<<<<<<<<<<<<<<< codedef <<<<<<<<<<<<<<<<<
#define codedef_set_context_as_substitution_eax1		cThread0.Rax = Substitution_cpuid->EAX_1.reg_EAX; cThread0.Rbx = Substitution_cpuid->EAX_1.reg_EBX; cThread0.Rcx = Substitution_cpuid->EAX_1.reg_ECX; cThread0.Rdx = Substitution_cpuid->EAX_1.reg_EDX
#define codedef_set_context_as_substitution_eax80000002 cThread0.Rax = Substitution_cpuid->EAX_80000002.reg_EAX; cThread0.Rbx = Substitution_cpuid->EAX_80000002.reg_EBX; cThread0.Rcx = Substitution_cpuid->EAX_80000002.reg_ECX; cThread0.Rdx = Substitution_cpuid->EAX_80000002.reg_EDX
#define codedef_set_context_as_substitution_eax80000003 cThread0.Rax = Substitution_cpuid->EAX_80000003.reg_EAX; cThread0.Rbx = Substitution_cpuid->EAX_80000003.reg_EBX; cThread0.Rcx = Substitution_cpuid->EAX_80000003.reg_ECX; cThread0.Rdx = Substitution_cpuid->EAX_80000003.reg_EDX
#define codedef_set_context_as_substitution_eax80000004 cThread0.Rax = Substitution_cpuid->EAX_80000004.reg_EAX; cThread0.Rbx = Substitution_cpuid->EAX_80000004.reg_EBX; cThread0.Rcx = Substitution_cpuid->EAX_80000003.reg_ECX; cThread0.Rdx = Substitution_cpuid->EAX_80000004.reg_EDX
#define codedef_execute_set_context(thandle) errset = ::SetThreadContext(thandle,&cThread0)
//++++++++++++++++++++++ structs ++++++++++++++++++++++
typedef struct DH_Licence_RAW_TABLE_DATA
{
CPUID_licence_stack CPUID_stack; //CPUID
KUSER_SHARED_DATA_licence_stack KUSER_SHARED_DATA_stack; //KUSER_SHARED_DATA
IMAGE_DATA_DIRECTORY_licence_stack IDD_system_dll_1; //IMAGE_DATA_DIRECTORY dll #1
IMAGE_DATA_DIRECTORY_licence_stack IDD_system_dll_2; //IMAGE_DATA_DIRECTORY dll #2
IMAGE_DATA_DIRECTORY_licence_stack IDD_system_dll_3; //IMAGE_DATA_DIRECTORY dll #3
PEB_licence_stack PEB_stack; // Process Enviroment Block (PEB)
}T80U_Raw_licence_data, *pT80U_Raw_licence_data;


//******************************* F U N C! ***************************************
UINT DH_Licence__Get_RAW_table_data_from_current_profile(__in pT80U_Raw_licence_data RAW_table_data_struct); //get RAW licence table data
bool __inline DH_Licence__Get_Lib_components (__in HANDLE Targ_proc, __in HMODULE Remote_module, __in pIMAGE_DATA_DIRECTORY_licence_stack Lib_component); //получает информацию о компонентах IMAGE_DATA_DIRECTORY
SERVICE_COUNT DH_Licence__Write_dump_into_target_process (__in pT80U_Raw_licence_data Write_DUMP); // запись текущего дампа в процесс (кроме CPUID)
bool DH_Licence_Check_KUSER_SYSTEM_TIME_update_status_from_current_profile (void); //состояние процесса обновления KSYSTEM_TIME в KUSER_SHARE_DATA
signed long DH_Licence_CPUID_Sentinel(
	pCPUID_licence_stack Substitution_cpuid, //заменяемый контекст
	pCPUID_licence_stack Compare_you_original_cpuid //контекст для сравнения
	); //CPUID Setninel следит за контектстом главного потока и подменяет данные регистров после вызова cpuid на данные из загруженного дампа
void DH_Licence_Get_my_original_CPUID_stack(pCPUID_licence_stack You_original_cpuid);//возвращает оригинальный стек cpuid
pLIC_DLC_FileInfo_STACK __fastcall DH_Licence_Get_DLC_licence_filestack(
	__in bool isGet_EAOrigin, //вытаскивать все лицензии EA origin
	__in bool isGet_VavleSteam //вытаскивать все лицензии Vavle Steam
	);//составляет список всех файлов лицензий DLC на компьютере

pDH_LICENCE_DECODE_DLC_FILE_DATA __fastcall DH_Licence_Decode_Licence_data_from_file(
	__in WCHAR* pDLCFileName, //имя файла
	__in SERVICE_TYPE LicFileType //тип файла-лицензии eDLC_TYPE
	); //Осуществляет декодирование файла-лицензии. Возвращает извлеченную таблицу с XML-контейнером и секретной X_TABLE

extern TCHAR* DH_Licence_Enum_patch_DISK_variants(
	__in __out TCHAR* pPatch //путь (директория) с первой включенной буквой для поиска
	); //перечисляет возможную пути лицензии. Возвращает первый найденный

extern double entropy(byte const* pSource, size_t size); //эентропия

BYTE* __fastcall DH_DENUVO_Native_Decrypt(char *Input, size_t Data_area_size, DWORD* Support);