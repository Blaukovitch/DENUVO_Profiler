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

typedef LPVOID *PPVOID;

//typedef LONG KPRIORITY;
/*
typedef struct _LSA_UNICODE_STRING {
  USHORT Length;
  USHORT MaximumLength;
  PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;
*/
/*
typedef struct _CLIENT_ID{
	DWORD					ClientID0;
	DWORD					ClientID1; // thread id
} CLIENT_ID, *PCLIENT_ID;
*/

typedef struct _LDR_MODULE {
	LIST_ENTRY				InLoadOrderModuleList;
	LIST_ENTRY				InMemoryOrderModuleList;
	LIST_ENTRY				InInitializationOrderModuleList;
	PVOID					BaseAddress;
	PVOID					EntryPoint;
	ULONG					SizeOfImage;
	UNICODE_STRING			FullDllName;
	UNICODE_STRING			BaseDllName;
	ULONG					Flags;
	SHORT					LoadCount;
	SHORT					TlsIndex;
	LIST_ENTRY				HashTableEntry;
	ULONG					TimeDateStamp;
} LDR_MODULE, *PLDR_MODULE;


typedef struct _PEB_LDR_DATA {
	ULONG					Length;
	BOOL					Initialized;
	PVOID					SsHandle;
	LIST_ENTRY				InLoadOrderModuleList;
	LIST_ENTRY				InMemoryOrderModuleList;
	LIST_ENTRY				InInitializationOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;


typedef struct _RTL_DRIVE_LETTER_CURDIR {
	USHORT					Flags;
	USHORT					Length;
	ULONG					TimeStamp;
	UNICODE_STRING			DosPath;
} RTL_DRIVE_LETTER_CURDIR, *PRTL_DRIVE_LETTER_CURDIR;


typedef struct _RTL_USER_PROCESS_PARAMETERS {
	ULONG					MaximumLength;
	ULONG					Length;
	ULONG					Flags;
	ULONG					DebugFlags;
	PVOID					ConsoleHandle;
	ULONG					ConsoleFlags;
	HANDLE					StdInputHandle;
	HANDLE					StdOutputHandle;
	HANDLE					StdErrorHandle;
	UNICODE_STRING			CurrentDirectoryPath;
	HANDLE					CurrentDirectoryHandle;
	UNICODE_STRING			DllPath;
	UNICODE_STRING			ImagePathName;
	UNICODE_STRING			CommandLine;
	PVOID					Environment;
	ULONG					StartingPositionLeft;
	ULONG					StartingPositionTop;
	ULONG					Width;
	ULONG					Height;
	ULONG					CharWidth;
	ULONG					CharHeight;
	ULONG					ConsoleTextAttributes;
	ULONG					WindowFlags;
	ULONG					ShowWindowFlags;
	UNICODE_STRING			WindowTitle;
	UNICODE_STRING			DesktopName;
	UNICODE_STRING			ShellInfo;
	UNICODE_STRING			RuntimeData;
	RTL_DRIVE_LETTER_CURDIR DLCurrentDirectory[0x20];
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;


typedef struct _SECTION_IMAGE_INFORMATION {
	PVOID					EntryPoint;
	ULONG					StackZeroBits;
	ULONG					StackReserved;
	ULONG					StackCommit;
	ULONG					ImageSubsystem;
	WORD					SubsystemVersionLow;
	WORD					SubsystemVersionHigh;
	ULONG					Unknown1;
	ULONG					ImageCharacteristics;
	ULONG					ImageMachineType;
	ULONG					Unknown2[3];
} SECTION_IMAGE_INFORMATION, *PSECTION_IMAGE_INFORMATION;

typedef struct _RTL_USER_PROCESS_INFORMATION {
	ULONG					Size;
	HANDLE					ProcessHandle;
	HANDLE					ThreadHandle;
	CLIENT_ID				ClientId;
	SECTION_IMAGE_INFORMATION ImageInformation;
} RTL_USER_PROCESS_INFORMATION, *PRTL_USER_PROCESS_INFORMATION;


typedef struct _wPEB_FREE_BLOCK {
	struct _PEB_FREE_BLOCK	*Next;
	ULONG					Size;
} PEB_FREE_BLOCK, *PPEB_FREE_BLOCK;

typedef void (*PPEBLOCKROUTINE)( PVOID PebLock ); 



/////////////////////// PEB ////////////////////////////////
#pragma pack(0)
typedef struct _wPEB {

 BOOLEAN                 InheritedAddressSpace;
  BOOLEAN                 ReadImageFileExecOptions;
  BOOLEAN                 BeingDebugged;
  DWORD                   Spare;
  PVOID                   Mutant;
  PVOID                   ImageBaseAddress;
  PPEB_LDR_DATA           LoaderData;
  PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
  PVOID                   SubSystemData;
  PVOID                   ProcessHeap;
  PVOID                   FastPebLock;
  PPEBLOCKROUTINE         FastPebLockRoutine;
  PPEBLOCKROUTINE         FastPebUnlockRoutine;
  ULONG                   EnvironmentUpdateCount;
  PPVOID                  KernelCallbackTable;
  PVOID                   EventLogSection;
  PVOID                   EventLog;
  PPEB_FREE_BLOCK         FreeList;
  ULONG                   TlsExpansionCounter;
  PVOID                   TlsBitmap;
  ULONG                   TlsBitmapBits[0x2];
  PVOID                   ReadOnlySharedMemoryBase;
  PVOID                   ReadOnlySharedMemoryHeap;
  PPVOID                  ReadOnlyStaticServerData;
  PVOID                   AnsiCodePageData;
  PVOID                   OemCodePageData;
  PVOID                   UnicodeCaseTableData;
  ULONG                   NumberOfProcessors;
  ULONG                   NtGlobalFlag;
  BYTE                    Spare2[0x4];
  LARGE_INTEGER           CriticalSectionTimeout;
  ULONG                   HeapSegmentReserve;
  ULONG                   HeapSegmentCommit;
  ULONG                   HeapDeCommitTotalFreeThreshold;
  ULONG                   HeapDeCommitFreeBlockThreshold;
  ULONG                   NumberOfHeaps;
  ULONG                   MaximumNumberOfHeaps;
  PPVOID                  *ProcessHeaps;
  PVOID                   GdiSharedHandleTable;
  PVOID                   ProcessStarterHelper;
  PVOID                   GdiDCAttributeList;
  PVOID                   LoaderLock;
  ULONG                   OSMajorVersion;
  ULONG                   OSMinorVersion;
  ULONG                   OSBuildNumber;
  ULONG                   OSPlatformId;
  ULONG                   ImageSubSystem;
  ULONG                   ImageSubSystemMajorVersion;
  ULONG                   ImageSubSystemMinorVersion;
  ULONG                   GdiHandleBuffer[0x22];
  ULONG                   PostProcessInitRoutine;
  ULONG                   TlsExpansionBitmap;
  BYTE                    TlsExpansionBitmapBits[0x80];
  ULONG                   SessionId;

} wPEB, *pwPEB;
#pragma pack()