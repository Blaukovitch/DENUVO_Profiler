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
#include <psapi.h>

using namespace std;
#pragma comment (lib, "BeaEngine64.lib")

//------------------ define data ------------------ 
#define TS_DEF_NA L"n/a" //default for no aviable items
#define TS_DEF_NULL L"0" //text NULL
#define TS_DEF_NULL_PERCENT L"0%" //text NULL percent
#define TS_DEF_PERCENT L"%" //text percent
#define TS_DEF_PERCENT_ACSII "%" //text percent
#define TS_DEF_TRUE L"1" //text 1
#define TS_DEF_NOTEXT L"" //text FREE
#define TS_DEF_DIV_UNIT L" / "

#define PROFILE_FOLDER_NAME _T("v2\\")
#define PROFILE_TRACE_VMPX_PROFILER_FOLDER_NAME L"vx_p\\"

#define A_SET_ORIGINAL_PATH CURRENT_Profile[CURRENT_Profile_len] = (WCHAR)L'\0'

//GUI props
///////// VM props limits
#define G_ADDRES_MAX_LEN sizeof(ULONG64)*4
#define G_FILE_NAME_VM_PROFILE_MAX_LEN 8
#define G_HANDLES_COUNT 5
#define G_VM_COMMENTERIES_MAX_LEN 100*2
#define G_VM_DECODE_INSTUCTION_NAME_MAX_LEN 16

//common definitions
#define HANHELD_ADDRES ULONG64 //����� "��������"
#define HANHELD_TYPE ULONG //��� "��������"
#define HANHELD_TYPE_CRITICAL_DETERMINE_ADDRES ULONG64 //����������� ������ ������, ���������� �� ����������� ���� "��������"
#define CELL_ADDRES HANHELD_ADDRES*    //����� ������, � ������� �������� ������� ������ (����� "��������", ����� ��������� �������)
#define OPERATIONAL_OPERAND_CELL_ADDRES ULONG64 //����� ��������, ����������� � ������� ��������
#define OPERATIONAL_ASM_INSTRUCTION ULONG //����������, ���������� �� ������
#define SERVICE_ADDRES ULONG64 //������ �����
#define SERVICE_COUNT size_t //������� 
#define SERVICE_OFFSET ULONG64 //����� ����� ������ (������� �������� � �����)
#define SERVICE_TYPE ULONG //��� ������
#define SERVICE_FLOW_POINTER PVOID //��������� ���������
#define SERVICE_ERROR signed long //������������� ������ ����������
#define SERVICE_HASH ULONG64 //����
#define SERVICE_ARRAY_ID ULONG //������� � �������
#define SERVICE_ID size_t //���������� ������������ ������������� 

///////// VM UNICODE limits
#define G_ADDRES_STRING_UNICODE_MAX_LEN (G_ADDRES_MAX_LEN*2)+4

#define ERROR_MAX_ARRAY_LIMIT 1

//REGION_TYPE 
#define REGION_TYPE_ALL_SECTION_INCLUDE_VM 1
#define REGION_TYPE_SPIRUN7_DUMP_AREA 2
#define REGION_TYPE_SYMBOL_AVANPOST_STRUCT 3
#define REGION_TYPE_PATCH_SUPPORT_REGIONS_STRUCT 4
#define REGION_TYPE_PATCH_ARBITARY_FILTER_STRUCT 5
#define REGION_TYPE_VMPX_CONTEXT_FILTER_STRUCT 6
#define REGION_TYPE_VMPX_CONTEXT_STACK 7
#define REGION_TYPE_VMPX_ADDRESS_POOL 8

//X E D parse (!)
#define PREDICT_ADDR_CONST 0x01487BBAA0 //�����-��������� ��� ������������
#define PREDICT_ADDR_JMP_DESD_OFFSET 0x4C
#define PREDICT_ADDR_DISTANCE PREDICT_ADDR_JMP_DESD_OFFSET + PREDICT_ADDR_CONST
#define PREDICT_ADDR_SUPPLY15 0x000014191978B //�����-��������� ��� ������������ ��������� �1.5 (VMP2)

#define CONTROL_SYMBOL_resistance_constant 2 // '$' + 'any_char'

//�������� � ������������ �����
#define BEA_ENGINE__DISSASM 1 //������������������
#define XED_PARSE__ASSEMBLY 2 //���������

//++++++++++++++++++++++ structs ++++++++++++++++++++++
//////////////////////////////// D A T A    F O R M A T! \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

typedef struct _PROCESS_BASIC_INFORMATION
{
    LONG ExitStatus;
    PVOID PebBaseAddress;
    ULONG_PTR AffinityMask;
    LONG BasePriority;
    ULONG_PTR UniqueProcessId;
    ULONG_PTR ParentProcessId;
} PROCESS_BASIC_INFORMATION, *PPROCESS_BASIC_INFORMATION;
	typedef enum _PROCESSINFOCLASS {
			ProcessBasicInformation = 0
		} PROCESSINFOCLASS;

typedef LONG(NTAPI* PZWQSI)(UINT, PVOID, ULONG, PULONG);         // ��������� �� ZwQuerySystemInformation
typedef LONG(NTAPI* PZWQIP)(     // ��������� �� ZwQueryInformationProcess
    IN  HANDLE ProcessHandle,
    IN  PROCESSINFOCLASS ProcessInformationClass,
    OUT PVOID ProcessInformation,
    IN  ULONG ProcessInformationLength,
    OUT PULONG ReturnLength    OPTIONAL
    );

#ifdef _WIN64
#define MEMADR LONGLONG
#else
#define MEMADR DWORD
#endif

 #define SYSTEM_PROCESS_THREAD_INFO_CLASS 5
 #define STATUS_INFO_LENGTH_MISMATCH 0xC0000004L
 #define STATUS_BUFFER_OVERFLOW 0x80000005L
 #define STATUS_BUFFER_TOO_SMALL 0xC0000023L
 #define STATUS_SUCCESS  0x00000000L

#define FUNC_length_HI 0x1FF
#define CHECK_AA_BORDER 0x14

// ���������� � �������

typedef struct _CLIENT_ID
{
	MEMADR UniqueProcess;
	MEMADR UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

// ���������� � �������
/*
typedef struct _SYSTEM_THREADS
{
	LARGE_INTEGER KernelTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER CreateTime;
	ULONG         WaitTime;
	PVOID         StartAddress;
	CLIENT_ID     ClientId;
	LONG          Priority;
	LONG          BasePriority;
	ULONG         ContextSwitchCount;
	LONG          State;
	LONG          WaitReason;
} SYSTEM_THREADS, *PSYSTEM_THREAD;
*/
typedef PVOID KPRIORITY;
typedef struct _SYSTEM_THREAD_INFORMATION
{
    LARGE_INTEGER KernelTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER CreateTime;
    ULONG WaitTime;
    PVOID StartAddress;
    CLIENT_ID ClientId;
    KPRIORITY Priority;
    LONG BasePriority;
    ULONG ContextSwitches;
    ULONG ThreadState;
    ULONG WaitReason;
}
SYSTEM_THREAD_INFORMATION, *PSYSTEM_THREAD_INFORMATION;

struct VM_COUNTERS {
 unsigned long PeakVirtualSize;
   unsigned long VirtualSize;
   unsigned long PageFaultCount;
   unsigned long PeakWorkingSetSize;
   unsigned long WorkingSetSize;
   unsigned long QuotaPeakPagedPoolUsage;
   unsigned long QuotaPagedPoolUsage;
   unsigned long QuotaPeakNonPagedPoolUsage;
  unsigned long QuotaNonPagedPoolUsage;
  unsigned long PagefileUsage;
  unsigned long PeakPagefileUsage;
  };


// ���������� � ���������
/*
typedef struct _SYSTEM_PROCESSES
{
	ULONG          NextEntryDelta;
	ULONG          ThreadCount;
	ULONG          Reserved1[6];
	LARGE_INTEGER  CreateTime;
	LARGE_INTEGER  UserTime;
	LARGE_INTEGER  KernelTime;
	USHORT         ProcessNameLength;
	USHORT         ProcessNameMaxLength;
	PWSTR          ProcessNameBuffer;
	LONG           BasePriority;
#ifdef _WIN64
	ULONG		   hz1;
	ULONG          ProcessId;
	ULONG          hz22;
	ULONG          InheritedFromProcessId;
	ULONG          hz3, hz4, hz5;
#else
	ULONG          ProcessId;
	ULONG          InheritedFromProcessId;
#endif
	ULONG          HandleCount;
	ULONG          Reserved2[2];
	VM_COUNTERS    VmCounters;
	IO_COUNTERS    IoCounters;
	//SYSTEM_THREADS Threads[1];
	SYSTEM_THREAD_INFORMATION Threads[1];
} SYSTEM_PROCESSES, *PSYSTEM_PROCESSES;
*/
typedef struct _LSA_UNICODE_STRING {
  USHORT Length;
  USHORT MaximumLength;
  PWSTR  Buffer;
} LSA_UNICODE_STRING, *PLSA_UNICODE_STRING, UNICODE_STRING, *PUNICODE_STRING;

typedef struct _SYSTEM_PROCESS_INFORMATION
{
  ULONG NextEntryOffset;
    ULONG NumberOfThreads;
    LARGE_INTEGER WorkingSetPrivateSize; // since VISTA
    ULONG HardFaultCount; // since WIN7
    ULONG NumberOfThreadsHighWatermark; // since WIN7
    ULONGLONG CycleTime; // since WIN7
    LARGE_INTEGER CreateTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER KernelTime;
    UNICODE_STRING ImageName;
    KPRIORITY BasePriority;
    HANDLE UniqueProcessId;
    HANDLE InheritedFromUniqueProcessId;
    ULONG HandleCount;
    ULONG SessionId;
    ULONG_PTR UniqueProcessKey; // since VISTA (requires SystemExtendedProcessInformation)
    SIZE_T PeakVirtualSize;
    SIZE_T VirtualSize;
    ULONG PageFaultCount;
    SIZE_T PeakWorkingSetSize;
    SIZE_T WorkingSetSize;
    SIZE_T QuotaPeakPagedPoolUsage;
    SIZE_T QuotaPagedPoolUsage;
    SIZE_T QuotaPeakNonPagedPoolUsage;
    SIZE_T QuotaNonPagedPoolUsage;
    SIZE_T PagefileUsage;
    SIZE_T PeakPagefileUsage;
    SIZE_T PrivatePageCount;
    LARGE_INTEGER ReadOperationCount;
    LARGE_INTEGER WriteOperationCount;
    LARGE_INTEGER OtherOperationCount;
    LARGE_INTEGER ReadTransferCount;
    LARGE_INTEGER WriteTransferCount;
    LARGE_INTEGER OtherTransferCount;
    SYSTEM_THREAD_INFORMATION Threads[1];
}
SYSTEM_PROCESS_INFORMATION, *PSYSTEM_PROCESS_INFORMATION;

// PEB//
  typedef VOID (WINAPI *PPS_POST_PROCESS_INIT_ROUTINE)(VOID);


/*
	typedef struct _RTL_USER_PROCESS_PARAMETERS {
	  BYTE           Reserved1[16];
	  PVOID          Reserved2[10];
	  UNICODE_STRING ImagePathName;
	  UNICODE_STRING CommandLine;
	} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

		typedef struct _PEB_LDR_DATA {
		  BYTE       Reserved1[8];
		  PVOID      Reserved2[3];
		  LIST_ENTRY InMemoryOrderModuleList;
		} PEB_LDR_DATA, *PPEB_LDR_DATA;

			typedef struct _PEB_64 {
				BYTE Reserved1[2];
				BYTE BeingDebugged;
				BYTE Reserved2[21];
				PPEB_LDR_DATA LoaderData;
				PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
				BYTE Reserved3[520];
				PPS_POST_PROCESS_INIT_ROUTINE PostProcessInitRoutine;
				BYTE Reserved4[136];
				ULONG SessionId;
			} PEB;
			*/
						//*/*/*/*/*/*/*/*/*/ MY /*/*/*/*/*/*/*/*/*/*//
  	typedef struct TARGET_PROC_REGION_COPY
	{
		SERVICE_ADDRES region_start_addres; //RVA ������ ������� � target-��������
		SIZE_T region_size; //������ ����������� �������
		SERVICE_TYPE type_of_region; //ex: REGION_TYPE_ALL_SECTION_INCLUDE_VM
		LPBYTE allocated_pool; //����� ����������� ���� � ��� �� malloc
		SERVICE_FLOW_POINTER flow_pointer_to_allocated_pool; //��������� ��������� ��� ���� 
	}REGION_COPY, *pREGION_COPY;

typedef struct SPECIAL_Digital_distribution_client //
{
	// STEAM

	bool is_steam_api64_present; //steam_api64.dll
	bool is_steamclient64_present; //steamclient64.dll
	// EA Origin
	bool is_Core_ActivationUI; // Core\ActivationUI.exe
	bool is_core_activation; // Core\Activation.dll

	//*** CRACK TEAMS **//
		//static//
	bool is_3DM_dll; //3dmgame.dll
	bool is_3DM_ini; //3DMGAME.ini
	bool is_ItsAMe_Origin_dll; //ItsAMe_Origin.dll
	bool is_CPY_ini; //CPY.ini
		//loaded//
	HMODULE LOADED_3DM_dll;
	HMODULE LOADED_ItsAMe_Origin_dll;
	//STEAM
	HMODULE LOADED_steam_api64_dll;
	HMODULE LOADED_steamclient64_dll;
	//Origin
	HMODULE LOADED_core_activation_dll;
}DDC, *pDDC;

#define AA_TYPE_NOT_DEFINED 0
#define AA_TYPE_UNKNOWN 1
#define AA_TYPE_JMP_EXIT_PROC_CONTAINER 2
#define AA_TYPE_JMP_EXIT_PRIMARY 3
#define AA_TYPE_JMP_HOOK 4

typedef struct AA_Process
{
	bool AA_STATE; //������� ��������� A.A. (��������/����������-����.)
	bool AA_TYPE; //��� A.A.
	BYTE AA_Proc_DUMP[CHECK_AA_BORDER]; //���� ������� DbgUiRemoteBreakin
	UINT AA_Replaced_count; //���������� ������������ ����
	FARPROC A_A; //����� ntdll.DbgUiRemoteBreakin 
}AA_PROC, pAA_Proc;


typedef struct EnumProcessInfo
{
	TCHAR ProcessName[MAX_PATH]; //��� ��������
	TCHAR Process_FilePath[MAX_PATH];
	unsigned long ProcessID; //PID
	unsigned long ParentProcessID; //PID ��������
	unsigned long ThreadCount; //���-�� �������
	unsigned long ModuleCount; //���-�� ����������� ���������
	unsigned long HandleCount; //���-�� �������� �������
	PVOID PEB_BaseAddress; //PEB
	HMODULE Main_HMODULE; //������� �����
	HMODULE Kernel32; //KERNEL32 ��� LoadLibrary
	HMODULE Ntdll; //NTDLL ��� LoadLibrary
	HMODULE Kernelbase; //NTDLL ��� LoadLibrary
	//IO_COUNTERS IOCounters; //���-�� �������� �����-������
	MODULEINFO Ext_Module_Information; //����������� ������ � ������
	DDC Digital_distribution_client_present; //���� ���������� � DDC (Steam, Orgin) + cracks
	CLIENT_ID Thread0; //���� � ������� �����
	AA_Process AA_Probe; //���� � ������������ ������ �� Anti-Attach

	TCHAR    eXT_EP_Section_Name[IMAGE_SIZEOF_SHORT_NAME]; //��� ������ ����� �����
	unsigned long eXT_IMAGE_Section_Count; //���-�� ������ � ������
}DH_ProcInfo, *pDH_ProcInfo;


typedef struct HIVE_AA_DATA_Block
{
	bool is_AA_info_received;
	BYTE AA_native_dump[FUNC_length_HI];
	FARPROC A_A;
	FARPROC AA_offset;
}AA_BLOCK, *pAA_BLOCK;

typedef struct HIVE_Licence_Block
{
	DWORD Emulate_KSYSTEM_TIME_Thread_ID; //KSYSTEM_TIME emulate thread ID
}Lic_BLOCK, *pLic_BLOCK;

typedef struct HIVE_SPIRUN7
{
	bool Restore_flag; //���� �������������� (��� �������������� ������ - ���������� ������������� ��� ���������� ������)
	bool SPIRUN7_Inject_state; //SPIRUN7 inject state
	SERVICE_ADDRES SPIRUN7_HMODULE; //SPIRUN7 HMODULE

	TCHAR Assign_vmp2_name[G_FILE_NAME_VM_PROFILE_MAX_LEN]; //��� ���������� ������� vmp2
	
	SERVICE_ADDRES SPIRUN7_main_vmp2_init_intrecept_in_targetproc; //����� ������������� � ��������� HANHELD � ��������� target-��������

		///����������, �������� �������� � SPIRUN7 target-��������
	SERVICE_ADDRES remote_HANHELD_ID_enter_trace_log_addres; //����� ����������� ����� ������ ��� ����� ���������� � ���������� HANHELD � SPIRUN7
	SERVICE_ADDRES remote_HANHELD_ID_fp_addres; //����� ���������� ��������� HANHELD_ID_Alloc_addres � SPIRUN7 �� ����� ������

	SERVICE_ADDRES remote_vmp2_intrecept_table; //����� ������� ������������� � SPIRUN7
	SERVICE_ADDRES remote_vmp2_original_table; //����� ������� ������������ HANHELD � SPIRUN7
	  ////////////////////////////////////////////////////

	/////// ����������� supply 15 tables � DH ----------- ���������� � typedef struct DH_VM_information_struct
	/*
	HANHELD_ADDRES* pSupply15_Intrecept_Table__WITH_crypt; //������� ������������� � �����������
	SERVICE_COUNT Supply15_hanheld_count; //������� ��������� supply15
	HANHELD_ADDRES* pSupply15_Original_Table__no_crypt; //������� ������������ HANHELD ��� ����������
	HANHELD_ADDRES* pSupply15_Original_Table__WITH_crypt; //������� ������������ HANHELD ��� ����������
	////////////////////////////////////////////////////////*/

	TARGET_PROC_REGION_COPY HANHELD_ID_dumping_region; //���������� ������ ��� ���������� HANHELD ID

}SPIRUN7_BLOCK, *pSPIRUN7_BLOCK;

typedef struct HIVE_injection
{
	LPCVOID pLibRemote; //injection ���������� ������ ��� Inject
	LPCVOID x_DLL_Path_Alloc_Adress; //injection ���� � ���������� ��� 
	LPCVOID x_Start_Thread_Addr; //injection ����� ������� ���������� ������
	LPCVOID Operand_Load_LibraryW_RVA; //injection ����� WinAPI LoadLibraryW
	HANDLE A_Create_Thread_handle; //injection ����� ������
	ULONG A_Create_Thread_ID; //injection id �����
}Inject_BLOCK, *pInject_BLOCK;



#define SECTION_MAX 30
typedef struct Target_Found
{
	bool tis_active; //�������
	HANDLE tOpenProcHANDLE; //������� ����� ��������� ��������
	DH_ProcInfo Process_HIVE; //���������� � ��������
	TCHAR PROFILE_PATH[MAX_PATH*2]; //�������
	size_t PROFILE_PATH_WCSLEN; //����� ���� �������
	bool Is_req_relocation;//��������� �� �������� RVA � ������ �������

	Inject_BLOCK Injection_info; //���������� �� ����������� ��������� ������������ ����

	Lic_BLOCK Licence_Info; //���������� � ��������

	SPIRUN7_BLOCK SPIRUN7_Info; //���������� � ���������� ���������� SPIRUN7

	IMAGE_DOS_HEADER Image_DOS_header;
	IMAGE_NT_HEADERS64 Image_NT_headers;
	IMAGE_SECTION_HEADER Image_Section[SECTION_MAX];

	TARGET_PROC_REGION_COPY Symbol_avanpost_stack; //��������� �� ���������� ����
	SERVICE_COUNT Symbol_Count; //���������� ��������� � ���������� �����

	TARGET_PROC_REGION_COPY PATCH_Support_region_stack; //��������� �� ��������� �������� ��� ����� PATCH_SUPPORTING_REGION_INI_STACK
	TARGET_PROC_REGION_COPY PATCH_ArbitaryFilter_VMP2; //������ �������� ��� ������� VMP2
	TARGET_PROC_REGION_COPY PATCH_ArbitaryFilter_VMP3new; //������ �������� ��� ������� VMP3
	TARGET_PROC_REGION_COPY PATCH_ArbitaryFilter__VMX_D4new; //������ �������� ��� ������� DENUVO v4
	TARGET_PROC_REGION_COPY VMPX_Context_VMP2; //������� �� ���������� VMP2
	TARGET_PROC_REGION_COPY VMPX_Context_VMP3new; //������� �� ���������� VMP3
	TARGET_PROC_REGION_COPY VMPX_Context_VMX_D4new; //������� �� ���������� DENUVO v4
}tHIVE, *ptHIVE;

enum DH_ERROR_STACK_TYPE
{
	DH_ERROR__ERRORS_SUCCESS = 0,
	DH_ERROR__XED_PARSE_ERROR, //XED parse
	DH_ERROR__Bea_Engine_ERROR //Bea Engine
};

typedef struct PARSE_Struct
{
	ULONG Line_error; //line error
	ULONG Col_error; //column error
	SERVICE_TYPE error_TYPE; //error type by DH_ERROR_STACK_TYPE
	size_t error_SUB_TYPE; //object error (�� ������������� ������ �������)
}PARSE_ERROR,*pPARSE_ERROR;

#define MAX_ERRORS_SET 50
typedef struct PATCH_PARSE_Struct
{
	PARSE_ERROR Error_STACK[MAX_ERRORS_SET]; //���� ������
	SERVICE_FLOW_POINTER fpErrorStack_ESP; //��������� �� ������� �����
	SERVICE_COUNT Errors_sets; //����� ����������
}PATCH_PARSE_ERROR_stack,*pPATCH_PARSE_ERROR_stack;

extern PARSE_ERROR Error_info;

enum Install_profile_main_types
{
	DH_INSTALL_PROFILE__VMPX_Context = 1,
	DH_INSTALL_PROFILE__PATH_Arbitary_filter
};
typedef struct Assign_vmpx_profiles
{
	TCHAR VMPX_PROFILE_PATH[MAX_PATH*2]; //������� VMPX
	size_t VMPX_PROFILE_PATH_WCSLEN; //����� ���� �������

	//Virtual machine contexts
	WCHAR VMPX_Context_vmp2[MAX_PATH]; //VMP2
	WCHAR VMPX_Context_vmp3new[MAX_PATH]; //VMP3 new
	WCHAR VMPX_Context_D4[MAX_PATH]; //D_4 CPS style
	WCHAR VMPX_Context_D5[MAX_PATH]; //D_5
	WCHAR VMPX_Context_ANOTHER[MAX_PATH]; //UNKNOWN type

	//������ ����� ���������
	size_t VMPX_Context_vmp2_len;
	size_t VMPX_Context_vmp3new_len;
	size_t VMPX_Context_D4_len;
	size_t VMPX_Context_D5_len;
	size_t VMPX_Context_ANOTHER_len;

	//Patch arbitary filters context
	WCHAR PATH_Arbitary_filter_vmp2[MAX_PATH]; //VMP2
	WCHAR PATH_Arbitary_filter_vmp3new[MAX_PATH]; //VMP3 new
	WCHAR PATH_Arbitary_filter_D4[MAX_PATH]; //D_4 CPS style
	WCHAR PATH_Arbitary_filter_D5[MAX_PATH]; //D_5
	WCHAR PATH_Arbitary_filter_ANOTHER[MAX_PATH]; //UNKNOWN type
}HUNT_Profiles, *pHUNT_Profiles;


extern HANDLE DH_Current_process_fast_handle;
extern unsigned long DH_Current_process_fast_ID;

 //\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\////////////////////////////////\\

//<<<<<<<<<<<<<<< codedef <<<<<<<<<<<<<<<<<
#define codedef_set_limits_condition_up(lim, set_x, check_y) 	if((check_y) < (lim)) (set_x) = (check_y); else set_x = (lim); 	ii--
#define codedef_set_limits_condition_down(lim, set_x, check_y) 	if((check_y) > (lim)) (set_x) = (check_y); else set_x = (lim); 	ii--
#define codedef_calc_remap_addr(desd, section_start, remap_start_region) 	(remap_start_region) + ((SERVICE_ADDRES)(desd) - (section_start))

//******************************* M A I N		F U N C! ***************************************

extern UINT64 DH_GetProcess(__out UINT64 *pProcessBufferPointer); //����������� ��������
extern HANDLE DH_OpenDENUVO(__in TCHAR* LoadPath, __in pDH_ProcInfo tProcess_Info); //��������� ����
extern bool DH_Get_AA_Process_state(__in LPCVOID NTDLL_AA_RVA_ex, __in HANDLE Research_Process, __out UINT* Replaced_Count); //��������� ��������� anti-attach
bool DH_CloseDENUVO(HANDLE ActiveProcess); //��������� ����
HMODULE DH_GetModuleHANDLE(__in  TCHAR* pModuleFileName); //HANDLE �� ����� ������
//************************************************************************************************


//++++++++++++++++++++++++++++++++ S U P P O R T		F U N C! ++++++++++++++++++++++++++++++++

extern void __fastcall itoa_16_fast(ULONG64 val, TCHAR* buf);
void __fastcall itoa_16_fast_full_DWORD(ULONG val, TCHAR* buf); //��� UNICODE
extern void __fastcall itoa_10_fast(ULONG64 val, TCHAR* buf);
unsigned long __fastcall wcsleave_path__cut_file_name(__in __out TCHAR* Ops_String);
extern void  __fastcall quick_ASCII_to_UNICODE (__in BYTE* Source_str, __out TCHAR* Dest_str);
extern void  __fastcall quick_UNICODE_to_ASCII (__in TCHAR* Source_str, __out char* Dest_str);
extern void  __fastcall quick_UNICODE_to_ASCII_with_ltoa16 (__in TCHAR* Source_str, __out char* Dest_str, __in TCHAR* pSource_ControlSymbol, __in SERVICE_ADDRES val); //�������������� ������ UNICODE to ASCII + ltoa � 16 ��������
extern void  __fastcall quick_UNICODE_to_ASCII_with_insert(__in TCHAR* Source_str, __out char* Dest_str, __in TCHAR* pSource_ControlSymbol, __in const char* pInsert);
extern void  quick_wscpy_UNICODE_to_UNICODE_set_num_filename(__in WCHAR* pSource, __out WCHAR* pDesd, __in size_t Source_len, __in size_t IncFileNumber); //����������� UNICODE � UNICODE ��� ���������� �����
extern UINT64 __fastcall wtou64_16_fast(TCHAR* src);
extern UINT64 __fastcall wtou64_16_fast__NO_SWAP_BYTES(TCHAR* src);
extern long reverse(long i);
void bytes_to_TCHAR_string(BYTE src_byte, TCHAR* desd_string);
extern SERVICE_HASH DH_Calc_area_HASH(
	LPDWORD Area_start, //������ �����
	size_t area_size //������ ����� - � ������� (DWORD ��������� � �������)
	);//��������� ������� ���� �� �������� �������

extern void __fastcall DH_DISASM_to_XED_string(char* DISASM_str, char* XED_str);
extern SERVICE_ID DH_Convert_Bea__to__DH_CPU_register(__in  ULONG Bea_Register); //������������� ��������� Bea � DH

extern  SERVICE_COUNT DH_Get_Target_PE_Section(PIMAGE_SECTION_HEADER* pSectionList); //���������� ���������� ������ � ��������� �� ��������� ������ � ��������� IMAGE_SECTION_HEADER
extern pDH_ProcInfo DH_Get_Target_Process_information(void); //���������� ���������� � ���������� ��������
extern bool DH__Install_HUNT_Profile(__in WCHAR* pProfileFileName, SERVICE_TYPE MainProfileType, SERVICE_TYPE SubType); //��������� ����� ��������
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ H I V E $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
extern tHIVE HUNT_HIVE; //��� � DENUVO
extern bool HUNT_HIVE_ACTIVE; //���� ������������������� �����
extern HUNT_Profiles HUNT_PROFILE_HIVE; //���������
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
extern bool DH__INIT_DENUVO_PROFILER(void); //��������� ��������� ��������� ������������� ����������
extern bool DH__Install_HUNT_Profile(__in WCHAR* pProfileFileName, SERVICE_TYPE MainProfileType, SERVICE_TYPE SubType); //��������� ������-�����������
//-----------------------------------------------------------------------------------------------

//	class DH_Engine
//	{
	//public:
	
		
		
//	};


