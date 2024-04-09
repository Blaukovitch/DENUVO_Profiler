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

typedef struct _KSYSTEM_TIME
{
     ULONG LowPart;
     LONG High1Time;
     LONG High2Time;
} KSYSTEM_TIME, *PKSYSTEM_TIME;

typedef enum _ALTERNATIVE_ARCHITECTURE_TYPE
{
         StandardDesign = 0,
         NEC98x86 = 1,
         EndAlternatives = 2
} ALTERNATIVE_ARCHITECTURE_TYPE;

typedef enum _NT_PRODUCT_TYPE
{
         NtProductWinNt = 1,
         NtProductLanManNt = 2,
         NtProductServer = 3
} NT_PRODUCT_TYPE;

typedef struct _KUSER_SHARED_DATA
{
     ULONG TickCountLowDeprecated;
     ULONG TickCountMultiplier;
     KSYSTEM_TIME InterruptTime;
     KSYSTEM_TIME SystemTime;
     KSYSTEM_TIME TimeZoneBias;
     WORD ImageNumberLow;
     WORD ImageNumberHigh;
     WCHAR NtSystemRoot[MAX_PATH];
     ULONG MaxStackTraceDepth;
     ULONG CryptoExponent;
     ULONG TimeZoneId;
     ULONG LargePageMinimum;
     ULONG Reserved2[7];
     NT_PRODUCT_TYPE NtProductType;
     UCHAR ProductTypeIsValid;
     ULONG NtMajorVersion;
     ULONG NtMinorVersion;
     UCHAR ProcessorFeatures[64];
     ULONG Reserved1;
     ULONG Reserved3;
     ULONG TimeSlip;
     ALTERNATIVE_ARCHITECTURE_TYPE AlternativeArchitecture;
     LARGE_INTEGER SystemExpirationDate;
     ULONG SuiteMask;
     UCHAR KdDebuggerEnabled;
     UCHAR NXSupportPolicy;
     ULONG ActiveConsoleId;
     ULONG DismountCount;
     ULONG ComPlusPackage;
     ULONG LastSystemRITEventTickCount;
     ULONG NumberOfPhysicalPages;
     UCHAR SafeBootMode;
     ULONG SharedDataFlags;
     ULONG DbgErrorPortPresent: 1;
     ULONG DbgElevationEnabled: 1;
     ULONG DbgVirtEnabled: 1;
     ULONG DbgInstallerDetectEnabled: 1;
     ULONG SystemDllRelocated: 1;
     ULONG SpareBits: 27;
     UINT64 TestRetInstruction;
     ULONG SystemCall;
     ULONG SystemCallReturn;
     UINT64 SystemCallPad[3];
     union
     {
          KSYSTEM_TIME TickCount;
          UINT64 TickCountQuad;
     };
     ULONG Cookie;
     INT64 ConsoleSessionForegroundProcessId;
     ULONG Wow64SharedInformation[16];
     WORD UserModeGlobalLogger[8];
     ULONG HeapTracingPid[2];
     ULONG CritSecTracingPid[2];
     ULONG ImageFileExecutionOptions;
     union
     {
          UINT64 AffinityPad;
          ULONG ActiveProcessorAffinity;
     };
     UINT64 InterruptTimeBias;
} KUSER_SHARED_DATA, *PKUSER_SHARED_DATA;