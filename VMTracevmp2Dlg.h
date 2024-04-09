#pragma once
#include "ENGINE\ENGINE.h"
#include "ENGINE\ENGINE_SPIRUN7.h"
#include "ENGINE\ENGINE_Profile_operations.h"
#include "ENGINE\ENGINE_VM_operations.h"

#define GUI_RIGHT_TAB_SIZE 205
// диалоговое окно CVMTracevmp2Dlg

class CVMTracevmp2Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVMTracevmp2Dlg)

public:
	CVMTracevmp2Dlg(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~CVMTracevmp2Dlg();

// Данные диалогового окна
	enum { IDD = IDD_VM_TRACER2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	HANDLE CurrProcessId;
	HANDLE hSentryThread;
	DWORD SentryThreadId;

	HICON m_hIcon;

	void GUI_UPDATE_AFTER_FILL_PROFILE(void);
	static DWORD WINAPI GUI_UPDATE_LIST_AND_CALCULATE_DATA(LPVOID Param);
	void GUI_OPENCLOSE_RIGHT_PANEL(bool variant);
	bool GUI_RIGHT_PANEL_state;
	bool Fill_VM_profiles_list(void);
	bool Fill_TRACE_profiles_list(void);
	void GUI_Launch_Restore_Mode(TCHAR* vmname);

	static DH_ENUM_FILES_of_PROFILES TRACE_file_lists[MAX_CONTAINTER_FILES];
	static DH_ENUM_FILES_of_PROFILES VM_file_lists[MAX_CONTAINTER_FILES];
	ULONG GUI_GET_VM_RECORD_SELECTED_FILENAME(__out WCHAR* FileName);
	WCHAR supply15_vm_text_data[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN];

	static pVMPX_VM_PROFILE_INFO_STACK active_trace_log_supply15; //выбранная вм для трэйс-логгинга

	SPIERUN7_INFO_CLUSTER_TRACE_LIST active_cluster_trace_list; //трэйслог лист и основные счетчики
	SPIERUN7_INFO_CLUSTER_POPULAR_LIST active_cluster_popular_list; //лист популярности

	HMODULE xa; //SPIRUN7 HMODULE
	SPIRUN7_BLOCK SPIRUN7_dll_info;

	bool bIntrecept_state; //состояние кнопки перехвата
	bool bRestore_state; //состояние кнопки возврата

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CStatic m_LAMP_SPIRUN7_connect;

	CEdit m_EDIT_HANHELD_ID_Alloc_region_dump;
	afx_msg void OnBnClickedAssignVm();
	CButton m_BUTTON_Assign_VM;
	CListBox m_LIST_vm_enum;
	CButton m_BUTTON_select_vm_record;
	afx_msg void OnBnClickedSelectVmRecord();
	CEdit m_EDIT_assign_supply15;
//	CButton m_BUTTON_Execute_intrecept;
	CButton m_BUTTON_Restore_orignal_supply15;
	CButton m_BUTTON_Execute_intrecept_supply15;
	CListCtrl m_list_trace;
	afx_msg void OnBnClickedButtonInterception();
	afx_msg void OnBnClickedButtonRestoreogirinal();
	CListCtrl m_list_popular_report;
	CEdit m_EDIT_H_TOTAL_call_count;
	CEdit m_EDIT_H_TOTAL_PCODE_shift;
	CEdit m_EDIT_H_TOTAL_RBP_dif;
	CEdit m_EDIT_TRACELOG_SAVE_FILENAME;
	CButton m_BUTTON_ADD_TRACELOG_RECORD;
	CButton m_BUTTON_Trace_compare_master;
	afx_msg void OnChangeEditAssignTracelogSaveFilename();
	afx_msg void OnBnClickedAddTracelogRecord();
	CListBox m_LIST_tracelogs_enum;
	afx_msg void OnBnClickedButtonTracecomparemaster();
//	CEdit m_EDIT_How_hanhelds_use;
	CEdit m_EDIT_H_TOTAL_hanhelds_use;
};
