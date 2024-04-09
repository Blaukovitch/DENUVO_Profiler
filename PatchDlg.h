#pragma once
#include "ENGINE/ENGINE_Profile_operations.h"
#include "ENGINE/ENGINE_MAP_operations.h"

// диалоговое окно CPatchDlg
#define codedef_check_term_threadif(threadid) if(threadid != NULL) ::TerminateThread(threadid, 1); threadid = NULL
#define codedef_check_free_allocatedif(allocmem) if(allocmem != NULL) free(allocmem); allocmem = NULL

class CPatchDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPatchDlg)

public:
	CPatchDlg(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~CPatchDlg();
	bool Fill_Not_applied_PATCH_profiles_list(void);
	bool Fill_ONLINE_PATCH_profiles_list(void);
	bool Load_Region_support_area_config(void);
	bool Fill_PATCH_enumerator(pDH_ENUM_FILES_of_PROFILES EnumFilesArray, HWND ListHandle, size_t i_count);
	void Fill_SUPPORT_REGION_AREA(void);
	void Fill_Partially_Q_report(void);
	static void __fastcall Fill_AntiRewrite_list_report(__in LPVOID Param, __in pPATCH_SNAPSHOT pFirst_snap, __in size_t how_count_to_end, __in size_t how_count_all_snap_with_rewrites);
	void GUI_UPDATE_AFTER_FILL_PROFILE(void);
	static DH_ENUM_FILES_of_PROFILES Not_applied_patch_file_lists[MAX_CONTAINTER_FILES];
	static DH_ENUM_FILES_of_PROFILES ONLINE_patch_file_lists[MAX_CONTAINTER_FILES];
	HANDLE hAntiRewriteSentinel;

// Данные диалогового окна
	enum { IDD = IDD_PATCH_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	pPATCH_SUPPORTING_REGION_INI_STACK pActive_Support_region; //конфигурация регионов поддержки
	static DWORD WINAPI GUI_UPDATE_Sentinel_requests(LPVOID Param);
	pPATCH_SNAPSHOT pEnumPatchSnapshot;
	pPATCH_SNAPSHOT fpEnumPatchSnapshot;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
//	CListCtrl m_na_list;
//	CListCtrl m_online_list;
	CListCtrl m_list_na;
	CListCtrl m_list_online;
	afx_msg void OnLvnItemchangedNotAppliedList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedApplyNaStackButton();
	CButton m_CHECK_auto_alloc_hook_and_filter_region;
	CButton m_CHECK_auto_alloc_original_vm_region;
	CButton m_CHECK_auto_alloc_patch_map_region;
	CStatic m_LAMP_patch_map_region;
	CStatic m_LAMP_hook_and_filter_region;
	CStatic m_LAMP_original_copy_region;
	CStatic m_LAMP_vmp_section_determine;
	CEdit m_EDIT_hook_and_filter_addres_start;
	CEdit m_EDIT_hook_and_filter_addres_end;
	CEdit m_EDIT_original_copy_vmp_addres_start;
	CEdit m_EDIT_original_copy_vmp_addres_end;
	CEdit m_EDIT_patch_map_addres_start;
	CEdit m_EDIT_patch_map_addres_end;
	CEdit m_EDIT_vmp_section_name;
	CEdit m_EDIT_VMP_SECTION_Addres_start;
	CEdit m_EDIT_VMP_SECTION_size;
	CButton m_BUTTON_APPLY_NA_STACK;
	CEdit m_EDIT_SUCCESS_NA_APPLY_COUNT;
//	CButton m_check_antirewrite_sentinel;
	CButton m_CHECK_antirewrite_sentinel;
	CListCtrl m_LIST_antirewrite;
	afx_msg void OnBnClickedCheckAntirewriteSentinel();
//	CButton m_EDIT_antirewrite_sentinel_count;
	CEdit m_EDIT_antirewrite_sentinel_count;
	CEdit m_EDIT_Q_Online_patches_count;
	CEdit m_EDIT_Q_NA_Patches_count;
	CEdit m_EDIT_Q_Rewrite_addreses_count;
};
