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

#include "ENGINE\ENGINE.h"
#include "ENGINE\ENGINE_Licence_operations.h"
// диалоговое окно CLicDlg


class CLicDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLicDlg)

public:
	CLicDlg(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~CLicDlg();
	
// Данные диалогового окна
	enum { IDD = IDD_LIC_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	HANDLE hSentinelThreadID;
	HANDLE CurrProcessId;
	void FillGUITable(pT80U_Raw_licence_data RAW_Table);
	void ShowFocusAddres(PVOID Focus_Addres);
	static DWORD WINAPI GUI_UPDATE_Sentinel_requests(LPVOID Param);
	DH_ENUM_FILES_of_PROFILES Lic_data_dumps_file_lists[MAX_CONTAINTER_FILES];
	bool Fill_LicRAWDumps_profiles_list(void);
	void GUI_UPDATE_AFTER_FILL_PROFILE(bool Fill_result);
	pT80U_Raw_licence_data Selected_LicRawDump;
	bool Load_current_LicRAWDump_and_fill_all_info_blocks(WCHAR* LicRawDump_NAME);
	UINT GUI_GET_LicRawDump_RECORD_SELECTED_FILENAME(__out WCHAR* FileName);
	void MOVESelectedLicRawDumpTOActual(pT80U_Raw_licence_data SelectedLicRawDump);
	
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	T80U_Raw_licence_data Actual_licence;
	CEdit m_EDIT_cpuid_eax_1;
	CEdit m_EDIT_cpuid_ebx_1;
	CEdit m_EDIT_cpuid_ecx_1;
	CEdit m_EDIT_cpuid_edx_1;
	CEdit m_EDIT_cpuid_eax_80000002;
	CEdit m_EDIT_cpuid_eax_80000003;
	CEdit m_EDIT_cpuid_eax_80000004;
	CEdit m_EDIT_cpuid_ebx_80000003;
	CEdit m_EDIT_cpuid_ebx_80000004;
	CEdit m_EDIT_cpuid_ebx_80000002;
	CEdit m_EDIT_cpuid_ecx_80000003;
	CEdit m_EDIT_cpuid_ecx_80000004;
	CEdit m_EDIT_cpuid_ecx_80000002;
	CEdit m_EDIT_cpuid_edx_80000003;
	CEdit m_EDIT_cpuid_edx_80000004;
	CEdit m_EDIT_cpuid_edx_80000002;
	CEdit m_EDIT_CPUID_Hash_value;
//	CEdit m_EDIT_NtMajorVersion_value;
	CEdit m_EDIT_NtMajorVersion_value;
	CEdit m_EDIT_NtMinorVersion_value;
//	CEdit m_EDIT_NtMinorVersion_RVA;
//	CEdit m_EDIT_NtMajorVersion_RVA;
	CEdit m_EDIT_NtSystemRoot_value;
//	CEdit m_EDIT_NtSystemRoot_RVA;
	CEdit m_EDIT_NubOfPhyPages_value;
//	CEdit m_EDIT_NubOfPhyPages_RVA;
	CEdit m_EDIT_ProcFeatures_value;
//	CEdit m_EDIT_ProcFeatures_RVA;
	CEdit m_EDIT_TimeSlip_value;
//	CEdit m_EDIT_TimeSlip_RVA;
	CEdit m_EDIT_CryptoExponent_value;
//	CEdit m_EDIT_CryptoExponent_RVA;
	CEdit m_EDIT_KUSER_HASH_value;
	CEdit m_EDIT_Show_element_addres;
	CEdit m_EDIT_Lib1_DebugSize;
	CEdit m_EDIT_Lib1_DebugVA;
	CEdit m_EDIT_Lib1_ExceptionSize;
	CEdit m_EDIT_Lib1_ExceptionValue;
	CEdit m_EDIT_Lib1_ExportSize;
	CEdit m_EDIT_Lib1_ExportVA;
	CEdit m_EDIT_Lib1_ImportSize;
	CEdit m_EDIT_Lib1_ImportVA;
	CEdit m_EDIT_Lib1_RelocationSize;
	CEdit m_EDIT_Lib1_RelocationVA;
	CEdit m_EDIT_Lib1_ResourceSize;
	CEdit m_EDIT_Lib1_ResourceVA;
	CEdit m_EDIT_Lib3_ResourceSize;
	CEdit m_EDIT_Lib2_ResourceSize;
	CEdit m_EDIT_Lib2_DebugSize;
	CEdit m_EDIT_Lib3_DebugSize;
	CEdit m_EDIT_Lib2_DebugVA;
	CEdit m_EDIT_Lib3_DebugVA;
	CEdit m_EDIT_Lib2_ExceptionSize;
	CEdit m_EDIT_Lib3_ExceptionSize;
	CEdit m_EDIT_Lib3_ExceptionVA;
	CEdit m_EDIT_Lib2_ExceptionVA;
	CEdit m_EDIT_Lib2_ExportSize;
	CEdit m_EDIT_Lib3_ExportSize;
	CEdit m_EDIT_Lib2_ExportVA;
	CEdit m_EDIT_Lib3_ExportVA;
	CEdit m_EDIT_Lib2_ImportVA;
	CEdit m_EDIT_Lib3_ImportVA;
	CEdit m_EDIT_Lib3_ImportSize;
	CEdit m_EDIT_Lib2_ImportSize;
	CEdit m_EDIT_Lib2_RelocationSize;
	CEdit m_EDIT_Lib3_RelocationSize;
	CEdit m_EDIT_Lib2_RelocationVA;
	CEdit m_EDIT_Lib3_RelocationVA;
	CEdit m_EDIT_Lib3_ResourceVA;
	CEdit m_EDIT_Lib2_ResourceVA;
	CEdit m_EDIT_Lib1_Hash;
	CEdit m_EDIT_Lib2_Hash;
	CEdit m_EDIT_Lib3_Hash;
	CEdit m_EDIT_PEB_Hash;
	CEdit m_EDIT_PEB_ImageSubSystemMajorVersion;
	CEdit m_EDIT_PEB_ImageSubSystemMinorVersion;
	CEdit m_EDIT_PEB_NumberOfProcessors;
	CEdit m_EDIT_PEB_OSMajorVersion;
	CEdit m_EDIT_PEB_OSMinorVersion;
	CEdit m_EDIT_PEB_OSPlatformId;
//	CEdit m_EDIT_ProcFeatures1_value;
//	CEdit m_EDIT_ProcFeatures2_value;
//	CEdit m_EDIT_ProcFeatures3_value;
	CEdit m_EDIT_ProcFeatures2_value;
	CEdit m_EDIT_ProcFeatures3_value;
	CEdit m_EDIT_ProcFeatures4_value;
	CEdit m_EDIT_ProcFeatures5_value;
	afx_msg void OnBnClickedButtonWriteDumpInProcessArea();
	CStatic m_LAMP_WriteMemDump_error_state;
	CStatic m_LAMP_KUSER__KSYSTEM_TIME;
//	afx_msg void OnEnChangeKuserNtmajorversion();
//	afx_msg void OnEnChangeKuserNtminorversion();
	afx_msg void OnSetfocusKuserNtmajorversion();
	afx_msg void OnSetfocusKuserNtminorversion();
	afx_msg void OnSetfocusKuserNtsystemroot();
	afx_msg void OnSetfocusKuserNumberofphysicalpages();
	afx_msg void OnSetfocusKuserProcessorfeatures();
	afx_msg void OnSetfocusKuserTimeslip();
	afx_msg void OnSetfocusPebImagesubsystemmajorversion();
	afx_msg void OnSetfocusPebImagesubsystemminorversion();
	afx_msg void OnSetfocusPebNumberofprocessors();
	afx_msg void OnSetfocusPebOsmajorversion();
	afx_msg void OnSetfocusPebOsminorversion();
	afx_msg void OnSetfocusPebOsplatformid();
	afx_msg void OnSetfocusKuserCryptoexponent();
	afx_msg void OnSetfocusLib1ImdatdirDebugsizeValue();
	afx_msg void OnSetfocusLib1ImdatdirDebugvaValue();
	afx_msg void OnSetfocusLib1ImdatdirExceptionsizeValue();
	afx_msg void OnSetfocusLib1ImdatdirExceptionvaValue();
	afx_msg void OnSetfocusLib1ImdatdirExportsizeValue();
	afx_msg void OnSetfocusLib1ImdatdirExportvaValue();
	afx_msg void OnSetfocusLib1ImdatdirImportsizeValue();
	afx_msg void OnSetfocusLib1ImdatdirImportvaValue();
	afx_msg void OnSetfocusLib1ImdatdirRelocationsizeValue();
	afx_msg void OnSetfocusLib1ImdatdirRelocationvaValue();
	afx_msg void OnSetfocusLib1ImdatdirResourcesizeValue();
	afx_msg void OnSetfocusLib1ImdatdirResourcevaValue();
	afx_msg void OnSetfocusLib2ImdatdirDebugsizeValue();
	afx_msg void OnSetfocusLib2ImdatdirDebugvaValue();
	afx_msg void OnSetfocusLib2ImdatdirExceptionsizeValue();
	afx_msg void OnSetfocusLib2ImdatdirExceptionvaValue();
	afx_msg void OnSetfocusLib2ImdatdirExportsizeValue();
	afx_msg void OnSetfocusLib2ImdatdirExportvaValue();
	afx_msg void OnSetfocusLib2ImdatdirImportsizeValue();
	afx_msg void OnSetfocusLib2ImdatdirImportvaValue();
	afx_msg void OnSetfocusLib2ImdatdirRelocationsizeValue();
	afx_msg void OnSetfocusLib2ImdatdirRelocationvaValue();
	afx_msg void OnSetfocusLib2ImdatdirResourcesizeValue();
	afx_msg void OnSetfocusLib2ImdatdirResourcevaValue();
	afx_msg void OnSetfocusLib3ImdatdirDebugsizeValue();
	afx_msg void OnSetfocusLib3ImdatdirDebugvaValue();
	afx_msg void OnSetfocusLib3ImdatdirExceptionsizeValue();
	afx_msg void OnSetfocusLib3ImdatdirExceptionvaValue();
	afx_msg void OnSetfocusLib3ImdatdirExportsizeValue();
	afx_msg void OnSetfocusLib3ImdatdirExportvaValue();
	afx_msg void OnSetfocusLib3ImdatdirImportsizeValue();
	afx_msg void OnSetfocusLib3ImdatdirImportvaValue();
	afx_msg void OnSetfocusLib3ImdatdirRelocationsizeValue();
	afx_msg void OnSetfocusLib3ImdatdirRelocationvaValue();
	afx_msg void OnSetfocusLib3ImdatdirResourcesizeValue();
	afx_msg void OnSetfocusLib3ImdatdirResourcevaValue();
	afx_msg void OnBnClickedAddLicDataDumpRecord();
	CListBox m_LIST_LicRawDumps;
	CButton m_BUTTON_Select_LicRawDump;
	afx_msg void OnBnClickedSelectLicDataDumpRecord();
	afx_msg void OnSelchangeListLicDumpsEnum();
	afx_msg void OnHotitemchangeCheckCpuidSentinel(NMHDR *pNMHDR, LRESULT *pResult);
	CButton m_CHECK_CPUID_Sentinel;
	afx_msg void OnClickedCheckCpuidSentinel();
	afx_msg void OnDoubleclickedCheckCpuidSentinel();
	CEdit m_EDIT_cpuid_sentinel;
};
