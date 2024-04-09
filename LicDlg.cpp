/*
	DENUVO_Profiler v0.3
	--------------------
	CRACKLAB, ELF
	--------------------
	Tiberium reversing
	********************
	2011	-	2024
*/

#include "stdafx.h"
#include "12.h"
#include "LicDlg.h"
#include "afxdialogex.h"


// диалоговое окно CLicDlg

IMPLEMENT_DYNAMIC(CLicDlg, CDialogEx)

CLicDlg::CLicDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLicDlg::IDD, pParent)
{
	codedef_check_term_threadif(hSentinelThreadID);
}

CLicDlg::~CLicDlg()
{
	codedef_check_term_threadif(hSentinelThreadID);
}

void CLicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EAX_1, m_EDIT_cpuid_eax_1);
	DDX_Control(pDX, IDC_EBX_1, m_EDIT_cpuid_ebx_1);
	DDX_Control(pDX, IDC_ECX_1, m_EDIT_cpuid_ecx_1);
	DDX_Control(pDX, IDC_EDX_1, m_EDIT_cpuid_edx_1);
	DDX_Control(pDX, IDC_EAX_80000002, m_EDIT_cpuid_eax_80000002);
	DDX_Control(pDX, IDC_EAX_80000003, m_EDIT_cpuid_eax_80000003);
	DDX_Control(pDX, IDC_EAX_80000004, m_EDIT_cpuid_eax_80000004);
	DDX_Control(pDX, IDC_EBX__80000003, m_EDIT_cpuid_ebx_80000003);
	DDX_Control(pDX, IDC_EBX__80000004, m_EDIT_cpuid_ebx_80000004);
	DDX_Control(pDX, IDC_EBX_80000002, m_EDIT_cpuid_ebx_80000002);
	DDX_Control(pDX, IDC_ECX__80000003, m_EDIT_cpuid_ecx_80000003);
	DDX_Control(pDX, IDC_ECX__80000004, m_EDIT_cpuid_ecx_80000004);
	DDX_Control(pDX, IDC_ECX_80000002, m_EDIT_cpuid_ecx_80000002);
	DDX_Control(pDX, IDC_EDX__80000003, m_EDIT_cpuid_edx_80000003);
	DDX_Control(pDX, IDC_EDX__80000004, m_EDIT_cpuid_edx_80000004);
	DDX_Control(pDX, IDC_EDX_80000002, m_EDIT_cpuid_edx_80000002);
	DDX_Control(pDX, IDC_CPUID_HASH, m_EDIT_CPUID_Hash_value);
	//  DDX_Control(pDX, IDC_KUSER_CryptoExponent, m_EDIT_NtMajorVersion_value);
	DDX_Control(pDX, IDC_KUSER_NtMajorVersion, m_EDIT_NtMajorVersion_value);
	DDX_Control(pDX, IDC_KUSER_NtMinorVersion, m_EDIT_NtMinorVersion_value);
	//  DDX_Control(pDX, IDC_KUSER_NtMinorVersion_RVA, m_EDIT_NtMinorVersion_RVA);
	//  DDX_Control(pDX, IDC_KUSER_NtMajorVersion_RVA, m_EDIT_NtMajorVersion_RVA);
	DDX_Control(pDX, IDC_KUSER_NtSystemRoot, m_EDIT_NtSystemRoot_value);
	//  DDX_Control(pDX, IDC_KUSER_NtSystemRoot_RVA, m_EDIT_NtSystemRoot_RVA);
	DDX_Control(pDX, IDC_KUSER_NumberOfPhysicalPages, m_EDIT_NubOfPhyPages_value);
	//  DDX_Control(pDX, IDC_KUSER_NumberOfPhysicalPages_RVA, m_EDIT_NubOfPhyPages_RVA);
	DDX_Control(pDX, IDC_KUSER_ProcessorFeatures, m_EDIT_ProcFeatures_value);
	//  DDX_Control(pDX, IDC_KUSER_ProcessorFeatures_RVA, m_EDIT_ProcFeatures_RVA);
	DDX_Control(pDX, IDC_KUSER_TimeSlip, m_EDIT_TimeSlip_value);
	//  DDX_Control(pDX, IDC_KUSER_TimeSlip_RVA, m_EDIT_TimeSlip_RVA);
	DDX_Control(pDX, IDC_KUSER_CryptoExponent, m_EDIT_CryptoExponent_value);
	//  DDX_Control(pDX, IDC_KUSER_CryptoExponent_RVA, m_EDIT_CryptoExponent_RVA);
	DDX_Control(pDX, IDC_KUSER_HASH, m_EDIT_KUSER_HASH_value);
	DDX_Control(pDX, IDC__EDIT_Common_element_RVA_addres, m_EDIT_Show_element_addres);
	DDX_Control(pDX, IDC__Lib1_ImDatDir_DebugSize_Value, m_EDIT_Lib1_DebugSize);
	DDX_Control(pDX, IDC__Lib1_ImDatDir_DebugVA_Value, m_EDIT_Lib1_DebugVA);
	DDX_Control(pDX, IDC__Lib1_ImDatDir_ExceptionSize_Value, m_EDIT_Lib1_ExceptionSize);
	DDX_Control(pDX, IDC__Lib1_ImDatDir_ExceptionVA_Value, m_EDIT_Lib1_ExceptionValue);
	DDX_Control(pDX, IDC__Lib1_ImDatDir_ExportSize_Value, m_EDIT_Lib1_ExportSize);
	DDX_Control(pDX, IDC__Lib1_ImDatDir_ExportVA_Value, m_EDIT_Lib1_ExportVA);
	DDX_Control(pDX, IDC__Lib1_ImDatDir_ImportSize_Value, m_EDIT_Lib1_ImportSize);
	DDX_Control(pDX, IDC__Lib1_ImDatDir_ImportVA_Value, m_EDIT_Lib1_ImportVA);
	DDX_Control(pDX, IDC__Lib1_ImDatDir_RelocationSize_Value, m_EDIT_Lib1_RelocationSize);
	DDX_Control(pDX, IDC__Lib1_ImDatDir_RelocationVA_Value, m_EDIT_Lib1_RelocationVA);
	DDX_Control(pDX, IDC__Lib1_ImDatDir_ResourceSize_Value, m_EDIT_Lib1_ResourceSize);
	DDX_Control(pDX, IDC__Lib1_ImDatDir_ResourceVA_Value, m_EDIT_Lib1_ResourceVA);
	DDX_Control(pDX, IDC__Lib3_ImDatDir_ResourceSize_Value, m_EDIT_Lib3_ResourceSize);
	DDX_Control(pDX, IDC__Lib2_ImDatDir_ResourceSize_Value, m_EDIT_Lib2_ResourceSize);
	DDX_Control(pDX, IDC__Lib2_ImDatDir_DebugSize_Value, m_EDIT_Lib2_DebugSize);
	DDX_Control(pDX, IDC__Lib3_ImDatDir_DebugSize_Value, m_EDIT_Lib3_DebugSize);
	DDX_Control(pDX, IDC__Lib2_ImDatDir_DebugVA_Value, m_EDIT_Lib2_DebugVA);
	DDX_Control(pDX, IDC__Lib3_ImDatDir_DebugVA_Value, m_EDIT_Lib3_DebugVA);
	DDX_Control(pDX, IDC__Lib2_ImDatDir_ExceptionSize_Value, m_EDIT_Lib2_ExceptionSize);
	DDX_Control(pDX, IDC__Lib3_ImDatDir_ExceptionSize_Value, m_EDIT_Lib3_ExceptionSize);
	DDX_Control(pDX, IDC__Lib3_ImDatDir_ExceptionVA_Value, m_EDIT_Lib3_ExceptionVA);
	DDX_Control(pDX, IDC__Lib2_ImDatDir_ExceptionVA_Value, m_EDIT_Lib2_ExceptionVA);
	DDX_Control(pDX, IDC__Lib2_ImDatDir_ExportSize_Value, m_EDIT_Lib2_ExportSize);
	DDX_Control(pDX, IDC__Lib3_ImDatDir_ExportSize_Value, m_EDIT_Lib3_ExportSize);
	DDX_Control(pDX, IDC__Lib2_ImDatDir_ExportVA_Value, m_EDIT_Lib2_ExportVA);
	DDX_Control(pDX, IDC__Lib3_ImDatDir_ExportVA_Value, m_EDIT_Lib3_ExportVA);
	DDX_Control(pDX, IDC__Lib2_ImDatDir_ImportVA_Value, m_EDIT_Lib2_ImportVA);
	DDX_Control(pDX, IDC__Lib3_ImDatDir_ImportVA_Value, m_EDIT_Lib3_ImportVA);
	DDX_Control(pDX, IDC__Lib3_ImDatDir_ImportSize_Value, m_EDIT_Lib3_ImportSize);
	DDX_Control(pDX, IDC__Lib2_ImDatDir_ImportSize_Value, m_EDIT_Lib2_ImportSize);
	DDX_Control(pDX, IDC__Lib2_ImDatDir_RelocationSize_Value, m_EDIT_Lib2_RelocationSize);
	DDX_Control(pDX, IDC__Lib3_ImDatDir_RelocationSize_Value, m_EDIT_Lib3_RelocationSize);
	DDX_Control(pDX, IDC__Lib2_ImDatDir_RelocationVA_Value, m_EDIT_Lib2_RelocationVA);
	DDX_Control(pDX, IDC__Lib3_ImDatDir_RelocationVA_Value, m_EDIT_Lib3_RelocationVA);
	DDX_Control(pDX, IDC__Lib3_ImDatDir_ResourceVA_Value, m_EDIT_Lib3_ResourceVA);
	DDX_Control(pDX, IDC__Lib2_ImDatDir_ResourceVA_Value, m_EDIT_Lib2_ResourceVA);
	DDX_Control(pDX, IDC_Lib1_HASH, m_EDIT_Lib1_Hash);
	DDX_Control(pDX, IDC_Lib2_HASH, m_EDIT_Lib2_Hash);
	DDX_Control(pDX, IDC_Lib3_HASH, m_EDIT_Lib3_Hash);
	DDX_Control(pDX, IDC_PEB_HASH, m_EDIT_PEB_Hash);
	DDX_Control(pDX, IDC_PEB_ImageSubSystemMajorVersion, m_EDIT_PEB_ImageSubSystemMajorVersion);
	DDX_Control(pDX, IDC_PEB_ImageSubSystemMinorVersion, m_EDIT_PEB_ImageSubSystemMinorVersion);
	DDX_Control(pDX, IDC_PEB_NumberOfProcessors, m_EDIT_PEB_NumberOfProcessors);
	DDX_Control(pDX, IDC_PEB_OSMajorVersion, m_EDIT_PEB_OSMajorVersion);
	DDX_Control(pDX, IDC_PEB_OSMinorVersion, m_EDIT_PEB_OSMinorVersion);
	DDX_Control(pDX, IDC_PEB_OSPlatformId, m_EDIT_PEB_OSPlatformId);
	//  DDX_Control(pDX, IDC_KUSER_ProcessorFeatures2, m_EDIT_ProcFeatures1_value);
	//  DDX_Control(pDX, IDC_KUSER_ProcessorFeatures3, m_EDIT_ProcFeatures2_value);
	//  DDX_Control(pDX, IDC_KUSER_ProcessorFeatures4, m_EDIT_ProcFeatures3_value);
	DDX_Control(pDX, IDC_KUSER_ProcessorFeatures2, m_EDIT_ProcFeatures2_value);
	DDX_Control(pDX, IDC_KUSER_ProcessorFeatures3, m_EDIT_ProcFeatures3_value);
	DDX_Control(pDX, IDC_KUSER_ProcessorFeatures4, m_EDIT_ProcFeatures4_value);
	DDX_Control(pDX, IDC_KUSER_ProcessorFeatures5, m_EDIT_ProcFeatures5_value);
	DDX_Control(pDX, IDC_LAMP_WRITE_LAST_ERROR, m_LAMP_WriteMemDump_error_state);
	DDX_Control(pDX, IDC_LAMP_KUSER_KSYSTEM_TIME, m_LAMP_KUSER__KSYSTEM_TIME);
	DDX_Control(pDX, IDC_LIST_LIC_DUMPS_ENUM, m_LIST_LicRawDumps);
	DDX_Control(pDX, IDC_SELECT_LIC_DATA_DUMP_RECORD, m_BUTTON_Select_LicRawDump);
	DDX_Control(pDX, IDC_CHECK_CPUID_SENTINEL, m_CHECK_CPUID_Sentinel);
	DDX_Control(pDX, IDC_EDIT_CPUID_Sentinel, m_EDIT_cpuid_sentinel);
}


BEGIN_MESSAGE_MAP(CLicDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_DUMP_IN_PROCESS_AREA, &CLicDlg::OnBnClickedButtonWriteDumpInProcessArea)
//	ON_EN_CHANGE(IDC_KUSER_NtMajorVersion, &CLicDlg::OnEnChangeKuserNtmajorversion)
//	ON_EN_CHANGE(IDC_KUSER_NtMinorVersion, &CLicDlg::OnEnChangeKuserNtminorversion)
ON_EN_SETFOCUS(IDC_KUSER_NtMajorVersion, &CLicDlg::OnSetfocusKuserNtmajorversion)
ON_EN_SETFOCUS(IDC_KUSER_NtMinorVersion, &CLicDlg::OnSetfocusKuserNtminorversion)
ON_EN_SETFOCUS(IDC_KUSER_NtSystemRoot, &CLicDlg::OnSetfocusKuserNtsystemroot)
ON_EN_SETFOCUS(IDC_KUSER_NumberOfPhysicalPages, &CLicDlg::OnSetfocusKuserNumberofphysicalpages)
ON_EN_SETFOCUS(IDC_KUSER_ProcessorFeatures, &CLicDlg::OnSetfocusKuserProcessorfeatures)
ON_EN_SETFOCUS(IDC_KUSER_TimeSlip, &CLicDlg::OnSetfocusKuserTimeslip)
ON_EN_SETFOCUS(IDC_PEB_ImageSubSystemMajorVersion, &CLicDlg::OnSetfocusPebImagesubsystemmajorversion)
ON_EN_SETFOCUS(IDC_PEB_ImageSubSystemMinorVersion, &CLicDlg::OnSetfocusPebImagesubsystemminorversion)
ON_EN_SETFOCUS(IDC_PEB_NumberOfProcessors, &CLicDlg::OnSetfocusPebNumberofprocessors)
ON_EN_SETFOCUS(IDC_PEB_OSMajorVersion, &CLicDlg::OnSetfocusPebOsmajorversion)
ON_EN_SETFOCUS(IDC_PEB_OSMinorVersion, &CLicDlg::OnSetfocusPebOsminorversion)
ON_EN_SETFOCUS(IDC_PEB_OSPlatformId, &CLicDlg::OnSetfocusPebOsplatformid)
ON_EN_SETFOCUS(IDC_KUSER_CryptoExponent, &CLicDlg::OnSetfocusKuserCryptoexponent)
ON_EN_SETFOCUS(IDC__Lib1_ImDatDir_DebugSize_Value, &CLicDlg::OnSetfocusLib1ImdatdirDebugsizeValue)
ON_EN_SETFOCUS(IDC__Lib1_ImDatDir_DebugVA_Value, &CLicDlg::OnSetfocusLib1ImdatdirDebugvaValue)
ON_EN_SETFOCUS(IDC__Lib1_ImDatDir_ExceptionSize_Value, &CLicDlg::OnSetfocusLib1ImdatdirExceptionsizeValue)
ON_EN_SETFOCUS(IDC__Lib1_ImDatDir_ExceptionVA_Value, &CLicDlg::OnSetfocusLib1ImdatdirExceptionvaValue)
ON_EN_SETFOCUS(IDC__Lib1_ImDatDir_ExportSize_Value, &CLicDlg::OnSetfocusLib1ImdatdirExportsizeValue)
ON_EN_SETFOCUS(IDC__Lib1_ImDatDir_ExportVA_Value, &CLicDlg::OnSetfocusLib1ImdatdirExportvaValue)
ON_EN_SETFOCUS(IDC__Lib1_ImDatDir_ImportSize_Value, &CLicDlg::OnSetfocusLib1ImdatdirImportsizeValue)
ON_EN_SETFOCUS(IDC__Lib1_ImDatDir_ImportVA_Value, &CLicDlg::OnSetfocusLib1ImdatdirImportvaValue)
ON_EN_SETFOCUS(IDC__Lib1_ImDatDir_RelocationSize_Value, &CLicDlg::OnSetfocusLib1ImdatdirRelocationsizeValue)
ON_EN_SETFOCUS(IDC__Lib1_ImDatDir_RelocationVA_Value, &CLicDlg::OnSetfocusLib1ImdatdirRelocationvaValue)
ON_EN_SETFOCUS(IDC__Lib1_ImDatDir_ResourceSize_Value, &CLicDlg::OnSetfocusLib1ImdatdirResourcesizeValue)
ON_EN_SETFOCUS(IDC__Lib1_ImDatDir_ResourceVA_Value, &CLicDlg::OnSetfocusLib1ImdatdirResourcevaValue)
ON_EN_SETFOCUS(IDC__Lib2_ImDatDir_DebugSize_Value, &CLicDlg::OnSetfocusLib2ImdatdirDebugsizeValue)
ON_EN_SETFOCUS(IDC__Lib2_ImDatDir_DebugVA_Value, &CLicDlg::OnSetfocusLib2ImdatdirDebugvaValue)
ON_EN_SETFOCUS(IDC__Lib2_ImDatDir_ExceptionSize_Value, &CLicDlg::OnSetfocusLib2ImdatdirExceptionsizeValue)
ON_EN_SETFOCUS(IDC__Lib2_ImDatDir_ExceptionVA_Value, &CLicDlg::OnSetfocusLib2ImdatdirExceptionvaValue)
ON_EN_SETFOCUS(IDC__Lib2_ImDatDir_ExportSize_Value, &CLicDlg::OnSetfocusLib2ImdatdirExportsizeValue)
ON_EN_SETFOCUS(IDC__Lib2_ImDatDir_ExportVA_Value, &CLicDlg::OnSetfocusLib2ImdatdirExportvaValue)
ON_EN_SETFOCUS(IDC__Lib2_ImDatDir_ImportSize_Value, &CLicDlg::OnSetfocusLib2ImdatdirImportsizeValue)
ON_EN_SETFOCUS(IDC__Lib2_ImDatDir_ImportVA_Value, &CLicDlg::OnSetfocusLib2ImdatdirImportvaValue)
ON_EN_SETFOCUS(IDC__Lib2_ImDatDir_RelocationSize_Value, &CLicDlg::OnSetfocusLib2ImdatdirRelocationsizeValue)
ON_EN_SETFOCUS(IDC__Lib2_ImDatDir_RelocationVA_Value, &CLicDlg::OnSetfocusLib2ImdatdirRelocationvaValue)
ON_EN_SETFOCUS(IDC__Lib2_ImDatDir_ResourceSize_Value, &CLicDlg::OnSetfocusLib2ImdatdirResourcesizeValue)
ON_EN_SETFOCUS(IDC__Lib2_ImDatDir_ResourceVA_Value, &CLicDlg::OnSetfocusLib2ImdatdirResourcevaValue)
ON_EN_SETFOCUS(IDC__Lib3_ImDatDir_DebugSize_Value, &CLicDlg::OnSetfocusLib3ImdatdirDebugsizeValue)
ON_EN_SETFOCUS(IDC__Lib3_ImDatDir_DebugVA_Value, &CLicDlg::OnSetfocusLib3ImdatdirDebugvaValue)
ON_EN_SETFOCUS(IDC__Lib3_ImDatDir_ExceptionSize_Value, &CLicDlg::OnSetfocusLib3ImdatdirExceptionsizeValue)
ON_EN_SETFOCUS(IDC__Lib3_ImDatDir_ExceptionVA_Value, &CLicDlg::OnSetfocusLib3ImdatdirExceptionvaValue)
ON_EN_SETFOCUS(IDC__Lib3_ImDatDir_ExportSize_Value, &CLicDlg::OnSetfocusLib3ImdatdirExportsizeValue)
ON_EN_SETFOCUS(IDC__Lib3_ImDatDir_ExportVA_Value, &CLicDlg::OnSetfocusLib3ImdatdirExportvaValue)
ON_EN_SETFOCUS(IDC__Lib3_ImDatDir_ImportSize_Value, &CLicDlg::OnSetfocusLib3ImdatdirImportsizeValue)
ON_EN_SETFOCUS(IDC__Lib3_ImDatDir_ImportVA_Value, &CLicDlg::OnSetfocusLib3ImdatdirImportvaValue)
ON_EN_SETFOCUS(IDC__Lib3_ImDatDir_RelocationSize_Value, &CLicDlg::OnSetfocusLib3ImdatdirRelocationsizeValue)
ON_EN_SETFOCUS(IDC__Lib3_ImDatDir_RelocationVA_Value, &CLicDlg::OnSetfocusLib3ImdatdirRelocationvaValue)
ON_EN_SETFOCUS(IDC__Lib3_ImDatDir_ResourceSize_Value, &CLicDlg::OnSetfocusLib3ImdatdirResourcesizeValue)
ON_EN_SETFOCUS(IDC__Lib3_ImDatDir_ResourceVA_Value, &CLicDlg::OnSetfocusLib3ImdatdirResourcevaValue)
ON_BN_CLICKED(IDC_ADD_LIC_DATA_DUMP_RECORD, &CLicDlg::OnBnClickedAddLicDataDumpRecord)
ON_BN_CLICKED(IDC_SELECT_LIC_DATA_DUMP_RECORD, &CLicDlg::OnBnClickedSelectLicDataDumpRecord)
ON_LBN_SELCHANGE(IDC_LIST_LIC_DUMPS_ENUM, &CLicDlg::OnSelchangeListLicDumpsEnum)
ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_CHECK_CPUID_SENTINEL, &CLicDlg::OnHotitemchangeCheckCpuidSentinel)
ON_BN_CLICKED(IDC_CHECK_CPUID_SENTINEL, &CLicDlg::OnClickedCheckCpuidSentinel)
ON_BN_DOUBLECLICKED(IDC_CHECK_CPUID_SENTINEL, &CLicDlg::OnDoubleclickedCheckCpuidSentinel)
END_MESSAGE_MAP()


// обработчики сообщений CLicDlg

static char input_data_my[] = {
...
};

BOOL CLicDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Добавить дополнительную инициализацию
	//pT80U_Raw_licence_data Actual_licence = (pT80U_Raw_licence_data)malloc(sizeof (T80U_Raw_licence_data));
	DH_Licence__Get_RAW_table_data_from_current_profile(&Actual_licence);
	FillGUITable(&Actual_licence);

	CurrProcessId = ::GetCurrentProcess();

	if (DH_Licence_Check_KUSER_SYSTEM_TIME_update_status_from_current_profile())
			m_LAMP_KUSER__KSYSTEM_TIME.SetBitmap(LAMP_TRUE);
	else
			m_LAMP_KUSER__KSYSTEM_TIME.SetBitmap(LAMP_FALSE);

	GUI_UPDATE_AFTER_FILL_PROFILE(Fill_LicRAWDumps_profiles_list());

	//ULONG64 Supporta = 0;
//	DH_DENUVO_Native_Decrypt(&input_data_my[0], 0x47B, (DWORD*)&Supporta);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}


void CLicDlg::FillGUITable(pT80U_Raw_licence_data RAW_Table)
{
	TCHAR dig_bufferT[64];
	memset(&dig_bufferT[0],0, sizeof(dig_bufferT));

	//CPUID *********** EAX = 1 ************* //
	itoa_16_fast(RAW_Table->CPUID_stack.EAX_1.reg_EAX, &dig_bufferT[0]); //EAX
	::SetWindowTextW(m_EDIT_cpuid_eax_1.m_hWnd,&dig_bufferT[0]);
	itoa_16_fast(RAW_Table->CPUID_stack.EAX_1.reg_EBX, &dig_bufferT[0]); //EBX
	::SetWindowTextW(m_EDIT_cpuid_ebx_1.m_hWnd,&dig_bufferT[0]);
	itoa_16_fast(RAW_Table->CPUID_stack.EAX_1.reg_ECX, &dig_bufferT[0]); //ECX
	::SetWindowTextW(m_EDIT_cpuid_ecx_1.m_hWnd,&dig_bufferT[0]);
	itoa_16_fast(RAW_Table->CPUID_stack.EAX_1.reg_EDX, &dig_bufferT[0]); //EDX
	::SetWindowTextW(m_EDIT_cpuid_edx_1.m_hWnd,&dig_bufferT[0]);

	//CPUID *********** EAX = 0x80000002 ************* //
	itoa_16_fast(RAW_Table->CPUID_stack.EAX_80000002.reg_EAX, &dig_bufferT[0]); //EAX
	::SetWindowTextW(m_EDIT_cpuid_eax_80000002.m_hWnd,&dig_bufferT[0]);
	itoa_16_fast(RAW_Table->CPUID_stack.EAX_80000002.reg_EBX, &dig_bufferT[0]); //EBX
	::SetWindowTextW(m_EDIT_cpuid_ebx_80000002.m_hWnd,&dig_bufferT[0]);
	itoa_16_fast(RAW_Table->CPUID_stack.EAX_80000002.reg_ECX, &dig_bufferT[0]); //ECX
	::SetWindowTextW(m_EDIT_cpuid_ecx_80000002.m_hWnd,&dig_bufferT[0]);
	itoa_16_fast(RAW_Table->CPUID_stack.EAX_80000002.reg_EDX, &dig_bufferT[0]); //EDX
	::SetWindowTextW(m_EDIT_cpuid_edx_80000002.m_hWnd,&dig_bufferT[0]);

	//CPUID *********** EAX = 0x80000003 ************* //
	itoa_16_fast(RAW_Table->CPUID_stack.EAX_80000003.reg_EAX, &dig_bufferT[0]); //EAX
	::SetWindowTextW(m_EDIT_cpuid_eax_80000003.m_hWnd,&dig_bufferT[0]);
	itoa_16_fast(RAW_Table->CPUID_stack.EAX_80000003.reg_EBX, &dig_bufferT[0]); //EBX
	::SetWindowTextW(m_EDIT_cpuid_ebx_80000003.m_hWnd,&dig_bufferT[0]);
	itoa_16_fast(RAW_Table->CPUID_stack.EAX_80000003.reg_ECX, &dig_bufferT[0]); //ECX
	::SetWindowTextW(m_EDIT_cpuid_ecx_80000003.m_hWnd,&dig_bufferT[0]);
	itoa_16_fast(RAW_Table->CPUID_stack.EAX_80000003.reg_EDX, &dig_bufferT[0]); //EDX
	::SetWindowTextW(m_EDIT_cpuid_edx_80000003.m_hWnd,&dig_bufferT[0]);

	//CPUID *********** EAX = 0x80000004 ************* //
	itoa_16_fast(RAW_Table->CPUID_stack.EAX_80000004.reg_EAX, &dig_bufferT[0]); //EAX
	::SetWindowTextW(m_EDIT_cpuid_eax_80000004.m_hWnd,&dig_bufferT[0]);
	itoa_16_fast(RAW_Table->CPUID_stack.EAX_80000004.reg_EBX, &dig_bufferT[0]); //EBX
	::SetWindowTextW(m_EDIT_cpuid_ebx_80000004.m_hWnd,&dig_bufferT[0]);
	itoa_16_fast(RAW_Table->CPUID_stack.EAX_80000004.reg_ECX, &dig_bufferT[0]); //ECX
	::SetWindowTextW(m_EDIT_cpuid_ecx_80000004.m_hWnd,&dig_bufferT[0]);
	itoa_16_fast(RAW_Table->CPUID_stack.EAX_80000004.reg_EDX, &dig_bufferT[0]); //EDX
	::SetWindowTextW(m_EDIT_cpuid_edx_80000004.m_hWnd,&dig_bufferT[0]);

	itoa_16_fast(RAW_Table->CPUID_stack.HASH, &dig_bufferT[0]); //CPUID Hash
	::SetWindowTextW(m_EDIT_CPUID_Hash_value.m_hWnd,&dig_bufferT[0]);


	//KUSER_SHARED_DATA ********************************//
	//addreses - !ввден единый EDIT box для показа индивидуального адреса
	/*
	itoa_16_fast((ULONG64)RAW_Table->KUSER_SHARED_DATA_stack.RVA_NtMajorVersion, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_NtMajorVersion_RVA.m_hWnd,&dig_bufferT[0]);
	itoa_16_fast((ULONG64)RAW_Table->KUSER_SHARED_DATA_stack.RVA_NtMinorVersion, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_NtMinorVersion_RVA.m_hWnd,&dig_bufferT[0]);
	itoa_16_fast((ULONG64)RAW_Table->KUSER_SHARED_DATA_stack.RVA_NtSystemRoot, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_NtSystemRoot_RVA.m_hWnd,&dig_bufferT[0]);
	itoa_16_fast((ULONG64)RAW_Table->KUSER_SHARED_DATA_stack.RVA_ProcessorFeatures, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_ProcFeatures_RVA.m_hWnd,&dig_bufferT[0]);
	itoa_16_fast((ULONG64)RAW_Table->KUSER_SHARED_DATA_stack.RVA_NumberOfPhysicalPages, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_NubOfPhyPages_RVA.m_hWnd,&dig_bufferT[0]);
	itoa_16_fast((ULONG64)RAW_Table->KUSER_SHARED_DATA_stack.RVA_CryptoExponent, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_CryptoExponent_RVA.m_hWnd,&dig_bufferT[0]);
	itoa_16_fast((ULONG64)RAW_Table->KUSER_SHARED_DATA_stack.RVA_TimeSlip, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_TimeSlip_RVA.m_hWnd,&dig_bufferT[0]);
	*/
	//value
	itoa_16_fast((ULONG64)RAW_Table->KUSER_SHARED_DATA_stack.NtMajorVersion, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_NtMajorVersion_value.m_hWnd,&dig_bufferT[0]);
	itoa_16_fast((ULONG64)RAW_Table->KUSER_SHARED_DATA_stack.NtMinorVersion, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_NtMinorVersion_value.m_hWnd,&dig_bufferT[0]);
	//itoa_16_fast_full_DWORD((ULONG64)RAW_Table->KUSER_SHARED_DATA_stack.NtSystemRoot[0], &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_NtSystemRoot_value.m_hWnd,&RAW_Table->KUSER_SHARED_DATA_stack.NtSystemRoot[0]);
	itoa_16_fast_full_DWORD(*(unsigned long*)&RAW_Table->KUSER_SHARED_DATA_stack.ProcessorFeatures[0], &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_ProcFeatures_value.m_hWnd,&dig_bufferT[0]);
	itoa_16_fast_full_DWORD(*(unsigned long*)&RAW_Table->KUSER_SHARED_DATA_stack.ProcessorFeatures[4], &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_ProcFeatures2_value.m_hWnd,&dig_bufferT[0]);
	itoa_16_fast_full_DWORD(*(unsigned long*)&RAW_Table->KUSER_SHARED_DATA_stack.ProcessorFeatures[8], &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_ProcFeatures3_value.m_hWnd,&dig_bufferT[0]);
	itoa_16_fast_full_DWORD(*(unsigned long*)&RAW_Table->KUSER_SHARED_DATA_stack.ProcessorFeatures[12], &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_ProcFeatures4_value.m_hWnd,&dig_bufferT[0]);
	itoa_16_fast_full_DWORD(*(unsigned long*)&RAW_Table->KUSER_SHARED_DATA_stack.ProcessorFeatures[16], &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_ProcFeatures5_value.m_hWnd,&dig_bufferT[0]);
	itoa_16_fast((ULONG64)RAW_Table->KUSER_SHARED_DATA_stack.NumberOfPhysicalPages, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_NubOfPhyPages_value.m_hWnd,&dig_bufferT[0]);
	itoa_16_fast((ULONG64)RAW_Table->KUSER_SHARED_DATA_stack.CryptoExponent, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_CryptoExponent_value.m_hWnd,&dig_bufferT[0]);
	itoa_16_fast((ULONG64)RAW_Table->KUSER_SHARED_DATA_stack.TimeSlip, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_TimeSlip_value.m_hWnd,&dig_bufferT[0]);
	//hash
	itoa_16_fast((ULONG64)RAW_Table->KUSER_SHARED_DATA_stack.HASH, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_KUSER_HASH_value.m_hWnd,&dig_bufferT[0]);

	//IMAGE_DATA_DIRECTORY ********************************//
	//library #1
	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_1.ImportDirectorySize, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib1_ImportSize.m_hWnd,&dig_bufferT[0]); //importsize
	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_1.ImportDirectoryVA, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib1_ImportVA.m_hWnd,&dig_bufferT[0]); //importva

	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_1.ExportDirectorySize, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib1_ExportSize.m_hWnd,&dig_bufferT[0]); //exportsize
	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_1.ExportDirectoryVA, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib1_ExportVA.m_hWnd,&dig_bufferT[0]); //exportva

	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_1.ResourseDirectorySize, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib1_ResourceSize.m_hWnd,&dig_bufferT[0]); //Resourcesize
	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_1.ResourseDirectoryVA, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib1_ResourceVA.m_hWnd,&dig_bufferT[0]); //Resourceva

	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_1.ExceptionDirectorySize, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib1_ExceptionSize.m_hWnd,&dig_bufferT[0]); //exceptionsize
	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_1.ExceptionDirectoryVA, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib1_ExceptionValue.m_hWnd,&dig_bufferT[0]); //exceptionva

	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_1.RelocationDirectorySize, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib1_RelocationSize.m_hWnd,&dig_bufferT[0]); //relocationsize
	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_1.RelocationDirectoryVA, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib1_RelocationVA.m_hWnd,&dig_bufferT[0]); //relocationva

	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_1.DebugDirectorySize, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib1_DebugSize.m_hWnd,&dig_bufferT[0]); //debugsize
	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_1.DebugDirectoryVA, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib1_DebugVA.m_hWnd,&dig_bufferT[0]); //debugva

	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_1.HASH, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib1_Hash.m_hWnd,&dig_bufferT[0]); //hash

	//library #2
	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_2.ImportDirectorySize, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib2_ImportSize.m_hWnd,&dig_bufferT[0]); //importsize
	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_2.ImportDirectoryVA, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib2_ImportVA.m_hWnd,&dig_bufferT[0]); //importva

	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_2.ExportDirectorySize, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib2_ExportSize.m_hWnd,&dig_bufferT[0]); //exportsize
	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_2.ExportDirectoryVA, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib2_ExportVA.m_hWnd,&dig_bufferT[0]); //exportva

	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_2.ResourseDirectorySize, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib2_ResourceSize.m_hWnd,&dig_bufferT[0]); //Resourcesize
	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_2.ResourseDirectoryVA, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib2_ResourceVA.m_hWnd,&dig_bufferT[0]); //Resourceva

	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_2.ExceptionDirectorySize, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib2_ExceptionSize.m_hWnd,&dig_bufferT[0]); //exceptionsize
	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_2.ExceptionDirectoryVA, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib2_ExceptionVA.m_hWnd,&dig_bufferT[0]); //exceptionva

	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_2.RelocationDirectorySize, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib2_RelocationSize.m_hWnd,&dig_bufferT[0]); //relocationsize
	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_2.RelocationDirectoryVA, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib2_RelocationVA.m_hWnd,&dig_bufferT[0]); //relocationva

	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_2.DebugDirectorySize, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib2_DebugSize.m_hWnd,&dig_bufferT[0]); //debugsize
	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_2.DebugDirectoryVA, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib2_DebugVA.m_hWnd,&dig_bufferT[0]); //debugva

	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_2.HASH, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib2_Hash.m_hWnd,&dig_bufferT[0]); //hash

	//library #3
	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_3.ImportDirectorySize, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib3_ImportSize.m_hWnd,&dig_bufferT[0]); //importsize
	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_3.ImportDirectoryVA, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib3_ImportVA.m_hWnd,&dig_bufferT[0]); //importva

	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_3.ExportDirectorySize, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib3_ExportSize.m_hWnd,&dig_bufferT[0]); //exportsize
	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_3.ExportDirectoryVA, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib3_ExportVA.m_hWnd,&dig_bufferT[0]); //exportva

	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_3.ResourseDirectorySize, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib3_ResourceSize.m_hWnd,&dig_bufferT[0]); //Resourcesize
	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_3.ResourseDirectoryVA, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib3_ResourceVA.m_hWnd,&dig_bufferT[0]); //Resourceva

	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_3.ExceptionDirectorySize, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib3_ExceptionSize.m_hWnd,&dig_bufferT[0]); //exceptionsize
	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_3.ExceptionDirectoryVA, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib3_ExceptionVA.m_hWnd,&dig_bufferT[0]); //exceptionva

	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_3.RelocationDirectorySize, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib3_RelocationSize.m_hWnd,&dig_bufferT[0]); //relocationsize
	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_3.RelocationDirectoryVA, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib3_RelocationVA.m_hWnd,&dig_bufferT[0]); //relocationva

	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_3.DebugDirectorySize, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib3_DebugSize.m_hWnd,&dig_bufferT[0]); //debugsize
	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_3.DebugDirectoryVA, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib3_DebugVA.m_hWnd,&dig_bufferT[0]); //debugva

	itoa_16_fast((ULONG64)RAW_Table->IDD_system_dll_3.HASH, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Lib3_Hash.m_hWnd,&dig_bufferT[0]); //hash

	//Process Enviroment Block (PEB)********************************//
	itoa_16_fast((ULONG64)RAW_Table->PEB_stack.OSMajorVersion, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_PEB_OSMajorVersion.m_hWnd,&dig_bufferT[0]); //OSMajorVersion

	itoa_16_fast((ULONG64)RAW_Table->PEB_stack.OSMinorVersion, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_PEB_OSMinorVersion.m_hWnd,&dig_bufferT[0]); //OSMinorVersion

	itoa_16_fast((ULONG64)RAW_Table->PEB_stack.ImageSubSystemMajorVersion, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_PEB_ImageSubSystemMajorVersion.m_hWnd,&dig_bufferT[0]); //ImageSubSystemMajorVersion

	itoa_16_fast((ULONG64)RAW_Table->PEB_stack.ImageSubSystemMinorVersion, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_PEB_ImageSubSystemMinorVersion.m_hWnd,&dig_bufferT[0]); //ImageSubSystemMinorVersion

	itoa_16_fast((ULONG64)RAW_Table->PEB_stack.NumberOfProcessors, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_PEB_NumberOfProcessors.m_hWnd,&dig_bufferT[0]); //NumberOfProcessors

	itoa_16_fast((ULONG64)RAW_Table->PEB_stack.OSPlatformId_short, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_PEB_OSPlatformId.m_hWnd,&dig_bufferT[0]); //OSPlatformId_short

	itoa_16_fast((ULONG64)RAW_Table->PEB_stack.HASH, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_PEB_Hash.m_hWnd,&dig_bufferT[0]); //OSPlatformId_short
}

void CLicDlg::OnBnClickedButtonWriteDumpInProcessArea()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (DH_Licence__Write_dump_into_target_process(&Actual_licence) == 0x31)
			m_LAMP_WriteMemDump_error_state.SetBitmap(LAMP_TRUE);
	else
		    m_LAMP_WriteMemDump_error_state.SetBitmap(LAMP_FALSE);
}

__forceinline void CLicDlg::ShowFocusAddres(PVOID Focus_Addres)
{
	TCHAR dig_bufferT[64];
	itoa_16_fast((ULONG64)Focus_Addres, &dig_bufferT[0]);
	::SetWindowTextW(m_EDIT_Show_element_addres.m_hWnd,&dig_bufferT[0]);
}



void CLicDlg::OnSetfocusKuserNtmajorversion()
{
	ShowFocusAddres(Actual_licence.KUSER_SHARED_DATA_stack.RVA_NtMajorVersion);
}


void CLicDlg::OnSetfocusKuserNtminorversion()
{
	ShowFocusAddres(Actual_licence.KUSER_SHARED_DATA_stack.RVA_NtMinorVersion);
}


void CLicDlg::OnSetfocusKuserNtsystemroot()
{
	ShowFocusAddres(Actual_licence.KUSER_SHARED_DATA_stack.RVA_NtSystemRoot);
}


void CLicDlg::OnSetfocusKuserNumberofphysicalpages()
{
	ShowFocusAddres(Actual_licence.KUSER_SHARED_DATA_stack.RVA_NumberOfPhysicalPages);
}


void CLicDlg::OnSetfocusKuserProcessorfeatures()
{
	ShowFocusAddres(Actual_licence.KUSER_SHARED_DATA_stack.RVA_ProcessorFeatures);
}


void CLicDlg::OnSetfocusKuserTimeslip()
{
	ShowFocusAddres(Actual_licence.KUSER_SHARED_DATA_stack.RVA_TimeSlip);
}


void CLicDlg::OnSetfocusPebImagesubsystemmajorversion()
{
	ShowFocusAddres((PVOID64)Actual_licence.PEB_stack.RVA_ImageSubSystemMajorVersion);
}


void CLicDlg::OnSetfocusPebImagesubsystemminorversion()
{
	ShowFocusAddres((PVOID64)Actual_licence.PEB_stack.RVA_ImageSubSystemMinorVersion);
}


void CLicDlg::OnSetfocusPebNumberofprocessors()
{
	ShowFocusAddres((PVOID64)Actual_licence.PEB_stack.RVA_NumberOfProcessors);
}


void CLicDlg::OnSetfocusPebOsmajorversion()
{
	ShowFocusAddres((PVOID64)Actual_licence.PEB_stack.RVA_OSMajorVersion);
}


void CLicDlg::OnSetfocusPebOsminorversion()
{
	ShowFocusAddres((PVOID64)Actual_licence.PEB_stack.RVA_OSMinorVersion);
}


void CLicDlg::OnSetfocusPebOsplatformid()
{
	ShowFocusAddres((PVOID64)Actual_licence.PEB_stack.RVA_OSPlatformId_short);
}


void CLicDlg::OnSetfocusKuserCryptoexponent()
{
	ShowFocusAddres((PVOID64)Actual_licence.KUSER_SHARED_DATA_stack.RVA_CryptoExponent);
}


void CLicDlg::OnSetfocusLib1ImdatdirDebugsizeValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_1.RVA_DebugDirectorySize);
}


void CLicDlg::OnSetfocusLib1ImdatdirDebugvaValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_1.RVA_DebugDirectoryVA);
}


void CLicDlg::OnSetfocusLib1ImdatdirExceptionsizeValue()
{
ShowFocusAddres(Actual_licence.IDD_system_dll_1.RVA_ExceptionDirectorySize);
}


void CLicDlg::OnSetfocusLib1ImdatdirExceptionvaValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_1.RVA_ExceptionDirectoryVA);
}


void CLicDlg::OnSetfocusLib1ImdatdirExportsizeValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_1.RVA_ExportDirectorySize);
}


void CLicDlg::OnSetfocusLib1ImdatdirExportvaValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_1.RVA_ExportDirectoryVA);
}


void CLicDlg::OnSetfocusLib1ImdatdirImportsizeValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_1.RVA_ImportDirectorySize);
}


void CLicDlg::OnSetfocusLib1ImdatdirImportvaValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_1.RVA_ImportDirectoryVA);
}


void CLicDlg::OnSetfocusLib1ImdatdirRelocationsizeValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_1.RVA_RelocationDirectorySize);
}


void CLicDlg::OnSetfocusLib1ImdatdirRelocationvaValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_1.RVA_RelocationDirectoryVA);
}


void CLicDlg::OnSetfocusLib1ImdatdirResourcesizeValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_1.RVA_ResourseDirectorySize);
}


void CLicDlg::OnSetfocusLib1ImdatdirResourcevaValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_1.RVA_ResourseDirectoryVA);
}


void CLicDlg::OnSetfocusLib2ImdatdirDebugsizeValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_2.RVA_DebugDirectorySize);
}


void CLicDlg::OnSetfocusLib2ImdatdirDebugvaValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_2.RVA_DebugDirectoryVA);
}


void CLicDlg::OnSetfocusLib2ImdatdirExceptionsizeValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_2.RVA_ExceptionDirectorySize);
}


void CLicDlg::OnSetfocusLib2ImdatdirExceptionvaValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_2.RVA_ExceptionDirectoryVA);
}


void CLicDlg::OnSetfocusLib2ImdatdirExportsizeValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_2.RVA_ExportDirectorySize);
}


void CLicDlg::OnSetfocusLib2ImdatdirExportvaValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_2.RVA_ExportDirectoryVA);
}


void CLicDlg::OnSetfocusLib2ImdatdirImportsizeValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_2.RVA_ImportDirectorySize);
}


void CLicDlg::OnSetfocusLib2ImdatdirImportvaValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_2.RVA_ImportDirectoryVA);
}


void CLicDlg::OnSetfocusLib2ImdatdirRelocationsizeValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_2.RVA_RelocationDirectorySize);
}


void CLicDlg::OnSetfocusLib2ImdatdirRelocationvaValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_2.RVA_RelocationDirectoryVA);
}


void CLicDlg::OnSetfocusLib2ImdatdirResourcesizeValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_2.RVA_ResourseDirectorySize);
}


void CLicDlg::OnSetfocusLib2ImdatdirResourcevaValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_2.RVA_ResourseDirectoryVA);
}


void CLicDlg::OnSetfocusLib3ImdatdirDebugsizeValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_3.RVA_DebugDirectorySize);
}


void CLicDlg::OnSetfocusLib3ImdatdirDebugvaValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_3.RVA_DebugDirectoryVA);
}


void CLicDlg::OnSetfocusLib3ImdatdirExceptionsizeValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_3.RVA_ExceptionDirectorySize);
}


void CLicDlg::OnSetfocusLib3ImdatdirExceptionvaValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_3.RVA_ExceptionDirectoryVA);
}


void CLicDlg::OnSetfocusLib3ImdatdirExportsizeValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_3.RVA_ExportDirectorySize);
}


void CLicDlg::OnSetfocusLib3ImdatdirExportvaValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_3.RVA_ExportDirectoryVA);
}


void CLicDlg::OnSetfocusLib3ImdatdirImportsizeValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_3.RVA_ImportDirectorySize);
}


void CLicDlg::OnSetfocusLib3ImdatdirImportvaValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_3.RVA_ImportDirectoryVA);
}


void CLicDlg::OnSetfocusLib3ImdatdirRelocationsizeValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_3.RVA_RelocationDirectorySize);
}


void CLicDlg::OnSetfocusLib3ImdatdirRelocationvaValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_3.RVA_RelocationDirectoryVA);
}


void CLicDlg::OnSetfocusLib3ImdatdirResourcesizeValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_3.RVA_ResourseDirectorySize);
}


void CLicDlg::OnSetfocusLib3ImdatdirResourcevaValue()
{
	ShowFocusAddres(Actual_licence.IDD_system_dll_3.RVA_ResourseDirectoryVA);
}


void CLicDlg::OnBnClickedAddLicDataDumpRecord()
{
	// TODO: добавьте свой код обработчика уведомлений
	//memset(&Actual_licence,0,sizeof(Actual_licence)); - null license
	DH_Write_Current_hunt_profile_CONTAINER_savedlg(PROFILE_TYPE_LIC_DATA_DUMP,(BYTE*)&Actual_licence,sizeof(Actual_licence));
	GUI_UPDATE_AFTER_FILL_PROFILE(Fill_LicRAWDumps_profiles_list());
}

bool CLicDlg::Fill_LicRAWDumps_profiles_list()
{
	UINT i = DH_Get_Current_hunt_profile_ALL_FILE_CONTAINERS(PROFILE_TYPE_LIC_DATA_DUMP, &Lic_data_dumps_file_lists[0]);
	if(i==0)
		{return false;}
	m_LIST_LicRawDumps.ResetContent();

	do
	{i--;m_LIST_LicRawDumps.AddString(&Lic_data_dumps_file_lists[i].FileName[0]);}while(i != 0);
	return true;
}

void CLicDlg::GUI_UPDATE_AFTER_FILL_PROFILE(bool Fill_result)
{
		::EnableWindow(m_BUTTON_Select_LicRawDump.m_hWnd, Fill_result);
	UpdateWindow();
}

void CLicDlg::OnBnClickedSelectLicDataDumpRecord()
{
	// TODO: добавьте свой код обработчика уведомлений
	WCHAR text_data[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN];
	if (!GUI_GET_LicRawDump_RECORD_SELECTED_FILENAME(&text_data[0]))
			return;

	Load_current_LicRAWDump_and_fill_all_info_blocks(&text_data[0]);
}

bool CLicDlg::Load_current_LicRAWDump_and_fill_all_info_blocks(WCHAR* LicRawDump_NAME)
{
	DWORD reta;
//	if(Selected_LicRawDump != NULL)
	//	{free(Selected_LicRawDump);}

	Selected_LicRawDump = (pT80U_Raw_licence_data)DH_Read_Current_hunt_profile_CONTAINER(PROFILE_TYPE_LIC_DATA_DUMP, LicRawDump_NAME,PROFILE_READ_ALL,&reta);
	if (!Selected_LicRawDump)
			return false;

	FillGUITable(Selected_LicRawDump);

	MOVESelectedLicRawDumpTOActual(Selected_LicRawDump);

	free(Selected_LicRawDump);
//	Selected_LicRawDump == NULL;
}

UINT CLicDlg::GUI_GET_LicRawDump_RECORD_SELECTED_FILENAME(__out WCHAR* FileName)
{
	UINT csl = m_LIST_LicRawDumps.GetCurSel();

	if(csl == 0xffffffff)
		return 0;


	if (m_LIST_LicRawDumps.GetTextLen(csl) > PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN)
		return 0;

		return m_LIST_LicRawDumps.GetText(csl,FileName);
}

void CLicDlg::OnSelchangeListLicDumpsEnum()
{
	// TODO: добавьте свой код обработчика уведомлений
}

void CLicDlg::MOVESelectedLicRawDumpTOActual(pT80U_Raw_licence_data SelectedLicRawDump)
{
	//CPUID, eax 1
	Actual_licence.CPUID_stack.EAX_1.reg_EAX = SelectedLicRawDump->CPUID_stack.EAX_1.reg_EAX;
	Actual_licence.CPUID_stack.EAX_1.reg_EBX = SelectedLicRawDump->CPUID_stack.EAX_1.reg_EBX;
	Actual_licence.CPUID_stack.EAX_1.reg_ECX = SelectedLicRawDump->CPUID_stack.EAX_1.reg_ECX;
	Actual_licence.CPUID_stack.EAX_1.reg_EDX = SelectedLicRawDump->CPUID_stack.EAX_1.reg_EDX;

	//CPUID, eax 80000002
	Actual_licence.CPUID_stack.EAX_80000002.reg_EAX = SelectedLicRawDump->CPUID_stack.EAX_80000002.reg_EAX;
	Actual_licence.CPUID_stack.EAX_80000002.reg_EBX = SelectedLicRawDump->CPUID_stack.EAX_80000002.reg_EBX;
	Actual_licence.CPUID_stack.EAX_80000002.reg_ECX = SelectedLicRawDump->CPUID_stack.EAX_80000002.reg_ECX;
	Actual_licence.CPUID_stack.EAX_80000002.reg_EDX = SelectedLicRawDump->CPUID_stack.EAX_80000002.reg_EDX;

	//CPUID, eax 80000003
	Actual_licence.CPUID_stack.EAX_80000003.reg_EAX = SelectedLicRawDump->CPUID_stack.EAX_80000003.reg_EAX;
	Actual_licence.CPUID_stack.EAX_80000003.reg_EBX = SelectedLicRawDump->CPUID_stack.EAX_80000003.reg_EBX;
	Actual_licence.CPUID_stack.EAX_80000003.reg_ECX = SelectedLicRawDump->CPUID_stack.EAX_80000003.reg_ECX;
	Actual_licence.CPUID_stack.EAX_80000003.reg_EDX = SelectedLicRawDump->CPUID_stack.EAX_80000003.reg_EDX;

	//CPUID, eax 80000004
	Actual_licence.CPUID_stack.EAX_80000004.reg_EAX = SelectedLicRawDump->CPUID_stack.EAX_80000004.reg_EAX;
	Actual_licence.CPUID_stack.EAX_80000004.reg_EBX = SelectedLicRawDump->CPUID_stack.EAX_80000004.reg_EBX;
	Actual_licence.CPUID_stack.EAX_80000004.reg_ECX = SelectedLicRawDump->CPUID_stack.EAX_80000004.reg_ECX;
	Actual_licence.CPUID_stack.EAX_80000004.reg_EDX = SelectedLicRawDump->CPUID_stack.EAX_80000004.reg_EDX;
	Actual_licence.CPUID_stack.HASH = SelectedLicRawDump->CPUID_stack.HASH;

	
	//IMAGE_DATA_DIRECTORY, lib #1
	Actual_licence.IDD_system_dll_1.DebugDirectorySize = SelectedLicRawDump->IDD_system_dll_1.DebugDirectorySize;
	Actual_licence.IDD_system_dll_1.DebugDirectoryVA = SelectedLicRawDump->IDD_system_dll_1.DebugDirectoryVA;
	Actual_licence.IDD_system_dll_1.ExceptionDirectorySize = SelectedLicRawDump->IDD_system_dll_1.ExceptionDirectorySize;
	Actual_licence.IDD_system_dll_1.ExceptionDirectoryVA = SelectedLicRawDump->IDD_system_dll_1.ExceptionDirectoryVA;
	Actual_licence.IDD_system_dll_1.ExportDirectorySize = SelectedLicRawDump->IDD_system_dll_1.ExportDirectorySize;
	Actual_licence.IDD_system_dll_1.ExportDirectoryVA = SelectedLicRawDump->IDD_system_dll_1.ExportDirectoryVA;
	Actual_licence.IDD_system_dll_1.ImportDirectorySize = SelectedLicRawDump->IDD_system_dll_1.ImportDirectorySize;
	Actual_licence.IDD_system_dll_1.ImportDirectoryVA = SelectedLicRawDump->IDD_system_dll_1.ImportDirectoryVA;
	Actual_licence.IDD_system_dll_1.RelocationDirectorySize = SelectedLicRawDump->IDD_system_dll_1.RelocationDirectorySize;
	Actual_licence.IDD_system_dll_1.RelocationDirectoryVA = SelectedLicRawDump->IDD_system_dll_1.RelocationDirectoryVA;
	Actual_licence.IDD_system_dll_1.ResourseDirectorySize = SelectedLicRawDump->IDD_system_dll_1.ResourseDirectorySize;
	Actual_licence.IDD_system_dll_1.ResourseDirectoryVA = SelectedLicRawDump->IDD_system_dll_1.ResourseDirectoryVA;
	Actual_licence.IDD_system_dll_1.HASH = SelectedLicRawDump->IDD_system_dll_1.HASH;

	//IMAGE_DATA_DIRECTORY, lib #2
	Actual_licence.IDD_system_dll_2.DebugDirectorySize = SelectedLicRawDump->IDD_system_dll_2.DebugDirectorySize;
	Actual_licence.IDD_system_dll_2.DebugDirectoryVA = SelectedLicRawDump->IDD_system_dll_2.DebugDirectoryVA;
	Actual_licence.IDD_system_dll_2.ExceptionDirectorySize = SelectedLicRawDump->IDD_system_dll_2.ExceptionDirectorySize;
	Actual_licence.IDD_system_dll_2.ExceptionDirectoryVA = SelectedLicRawDump->IDD_system_dll_2.ExceptionDirectoryVA;
	Actual_licence.IDD_system_dll_2.ExportDirectorySize = SelectedLicRawDump->IDD_system_dll_2.ExportDirectorySize;
	Actual_licence.IDD_system_dll_2.ExportDirectoryVA = SelectedLicRawDump->IDD_system_dll_2.ExportDirectoryVA;
	Actual_licence.IDD_system_dll_2.ImportDirectorySize = SelectedLicRawDump->IDD_system_dll_2.ImportDirectorySize;
	Actual_licence.IDD_system_dll_2.ImportDirectoryVA = SelectedLicRawDump->IDD_system_dll_2.ImportDirectoryVA;
	Actual_licence.IDD_system_dll_2.RelocationDirectorySize = SelectedLicRawDump->IDD_system_dll_2.RelocationDirectorySize;
	Actual_licence.IDD_system_dll_2.RelocationDirectoryVA = SelectedLicRawDump->IDD_system_dll_2.RelocationDirectoryVA;
	Actual_licence.IDD_system_dll_2.ResourseDirectorySize = SelectedLicRawDump->IDD_system_dll_2.ResourseDirectorySize;
	Actual_licence.IDD_system_dll_2.ResourseDirectoryVA = SelectedLicRawDump->IDD_system_dll_2.ResourseDirectoryVA;
	Actual_licence.IDD_system_dll_2.HASH = SelectedLicRawDump->IDD_system_dll_2.HASH;

	//IMAGE_DATA_DIRECTORY, lib #3
	Actual_licence.IDD_system_dll_3.DebugDirectorySize = SelectedLicRawDump->IDD_system_dll_3.DebugDirectorySize;
	Actual_licence.IDD_system_dll_3.DebugDirectoryVA = SelectedLicRawDump->IDD_system_dll_3.DebugDirectoryVA;
	Actual_licence.IDD_system_dll_3.ExceptionDirectorySize = SelectedLicRawDump->IDD_system_dll_3.ExceptionDirectorySize;
	Actual_licence.IDD_system_dll_3.ExceptionDirectoryVA = SelectedLicRawDump->IDD_system_dll_3.ExceptionDirectoryVA;
	Actual_licence.IDD_system_dll_3.ExportDirectorySize = SelectedLicRawDump->IDD_system_dll_3.ExportDirectorySize;
	Actual_licence.IDD_system_dll_3.ExportDirectoryVA = SelectedLicRawDump->IDD_system_dll_3.ExportDirectoryVA;
	Actual_licence.IDD_system_dll_3.ImportDirectorySize = SelectedLicRawDump->IDD_system_dll_3.ImportDirectorySize;
	Actual_licence.IDD_system_dll_3.ImportDirectoryVA = SelectedLicRawDump->IDD_system_dll_3.ImportDirectoryVA;
	Actual_licence.IDD_system_dll_3.RelocationDirectorySize = SelectedLicRawDump->IDD_system_dll_3.RelocationDirectorySize;
	Actual_licence.IDD_system_dll_3.RelocationDirectoryVA = SelectedLicRawDump->IDD_system_dll_3.RelocationDirectoryVA;
	Actual_licence.IDD_system_dll_3.ResourseDirectorySize = SelectedLicRawDump->IDD_system_dll_3.ResourseDirectorySize;
	Actual_licence.IDD_system_dll_3.ResourseDirectoryVA = SelectedLicRawDump->IDD_system_dll_3.ResourseDirectoryVA;
	Actual_licence.IDD_system_dll_3.HASH = SelectedLicRawDump->IDD_system_dll_3.HASH;

	//KUSER_SHARED_DATA
	Actual_licence.KUSER_SHARED_DATA_stack.CryptoExponent = SelectedLicRawDump->KUSER_SHARED_DATA_stack.CryptoExponent;
	Actual_licence.KUSER_SHARED_DATA_stack.NtMajorVersion = SelectedLicRawDump->KUSER_SHARED_DATA_stack.NtMajorVersion;
	Actual_licence.KUSER_SHARED_DATA_stack.NtMinorVersion = SelectedLicRawDump->KUSER_SHARED_DATA_stack.NtMinorVersion;
	memcpy(&Actual_licence.KUSER_SHARED_DATA_stack.NtSystemRoot[0], &SelectedLicRawDump->KUSER_SHARED_DATA_stack.NtSystemRoot[0], sizeof(Actual_licence.KUSER_SHARED_DATA_stack.NtSystemRoot));
	Actual_licence.KUSER_SHARED_DATA_stack.NumberOfPhysicalPages = SelectedLicRawDump->KUSER_SHARED_DATA_stack.NumberOfPhysicalPages;
	memcpy(&Actual_licence.KUSER_SHARED_DATA_stack.ProcessorFeatures[0], &SelectedLicRawDump->KUSER_SHARED_DATA_stack.ProcessorFeatures[0], sizeof(Actual_licence.KUSER_SHARED_DATA_stack.ProcessorFeatures));
	Actual_licence.KUSER_SHARED_DATA_stack.TimeSlip = SelectedLicRawDump->KUSER_SHARED_DATA_stack.TimeSlip;
	Actual_licence.KUSER_SHARED_DATA_stack.HASH = SelectedLicRawDump->KUSER_SHARED_DATA_stack.HASH;

	//PEB
	Actual_licence.PEB_stack.ImageSubSystemMajorVersion = SelectedLicRawDump->PEB_stack.ImageSubSystemMajorVersion;
	Actual_licence.PEB_stack.ImageSubSystemMinorVersion = SelectedLicRawDump->PEB_stack.ImageSubSystemMinorVersion;
	Actual_licence.PEB_stack.NumberOfProcessors = SelectedLicRawDump->PEB_stack.NumberOfProcessors;
	Actual_licence.PEB_stack.OSMajorVersion = SelectedLicRawDump->PEB_stack.OSMajorVersion;
	Actual_licence.PEB_stack.OSMinorVersion = SelectedLicRawDump->PEB_stack.OSMinorVersion;
	Actual_licence.PEB_stack.OSPlatformId_short = SelectedLicRawDump->PEB_stack.OSPlatformId_short;
	Actual_licence.PEB_stack.HASH = SelectedLicRawDump->PEB_stack.HASH;
}

void CLicDlg::OnHotitemchangeCheckCpuidSentinel(NMHDR *pNMHDR, LRESULT *pResult)
{
	// Для этого средства требуется Internet Explorer 6 или более поздняя версия.
	// Символ _WIN32_IE должен быть >= 0x0600.
	LPNMBCHOTITEM pHotItem = reinterpret_cast<LPNMBCHOTITEM>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	*pResult = 0;
}

DWORD WINAPI CLicDlg::GUI_UPDATE_Sentinel_requests(LPVOID Param)
{
	TCHAR dig_bufferL[64];
	CLicDlg *pThis = reinterpret_cast<CLicDlg*>(Param);

	signed long howcount = NULL;
	signed long retval = NULL;
	signed long immexitstate=true;

	::SetWindowText(pThis->m_EDIT_cpuid_sentinel.m_hWnd,_T("0"));

	CPUID_licence_stack MyCPUID_thread;
	DH_Licence_Get_my_original_CPUID_stack(&MyCPUID_thread);
	do
	{
		retval = DH_Licence_CPUID_Sentinel(&pThis->Actual_licence.CPUID_stack, &MyCPUID_thread);
		if (retval == -1)
			immexitstate = false; //exit cycle == terminate sentinel thread
	
		if (retval != NULL)
			{itoa_10_fast(++howcount, &dig_bufferL[0]);::SetWindowText(pThis->m_EDIT_cpuid_sentinel.m_hWnd,&dig_bufferL[0]);}

		::SleepEx(1,false);
	}while(immexitstate==true);

	::SetWindowText(pThis->m_EDIT_cpuid_sentinel.m_hWnd,_T("-1"));
	pThis->m_CHECK_CPUID_Sentinel.SetCheck(BST_UNCHECKED);
}

void CLicDlg::OnClickedCheckCpuidSentinel()
{
	// TODO: добавьте свой код обработчика уведомлений
		if (m_CHECK_CPUID_Sentinel.GetCheck() == BST_CHECKED)
	{
	codedef_check_term_threadif(hSentinelThreadID);
	hSentinelThreadID = ::CreateRemoteThreadEx(CurrProcessId,NULL,NULL, (LPTHREAD_START_ROUTINE)GUI_UPDATE_Sentinel_requests,this,NULL,NULL, NULL);
	}
	else
	{
		if (m_CHECK_CPUID_Sentinel.GetState() == BST_UNCHECKED)
			codedef_check_term_threadif(hSentinelThreadID);
	}
}


void CLicDlg::OnDoubleclickedCheckCpuidSentinel()
{
	// TODO: добавьте свой код обработчика уведомлений
	OnClickedCheckCpuidSentinel();
}
