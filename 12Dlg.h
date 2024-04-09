
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
#define GUI_HUNT_OPEN_ACTION_LABEL L"Hunt!"
#define GUI_HUNT_CLOSE_ACTION_LABEL L"End hunt."


#include "ENGINE/ENGINE_SPIRUN7.h"
#include "ENGINE/ENGINE_Profile_operations.h"
#include "ENGINE/ENGINE_MAP_operations.h"

// CMy12Dlg dialog
class CMy12Dlg : public CDialogEx
{
// Construction
public:
	CMy12Dlg(CWnd* pParent = NULL);	// standard constructor
	
	//HIVE
		 HANDLE OPT_LOCK_Target;
	pDH_ProcInfo My_DH_Hive_pointer;

	size_t iPointers;

	

// Dialog Data
	enum { IDD = IDD_MY12_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	bool Get_Selected_Target(__out TCHAR* Name, __out UINT64* Found_Hive_Pointer); //выборка по combo
	bool Update_Process_List(void); //выборка по combo
	
	bool GUI_APPROACH_COMMAND_LINE(TCHAR* procname); //проверяет командную строку
	
	bool START_HUNT_Ops(TCHAR* procname); //запускает процесс HUNT
	
	void GUI_SHOW_PROCESS_INFO(); //отображает инфо о выбранном процессе

	void GUI_Set_Enable_Windows_when_open_close_target(__in bool state); //разрешает/запрещает окошки при открытии-закрытии процесса
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeCombo1();
	CComboBox m_select_target;
//	afx_msg void OnEditupdateCombo1();
	afx_msg void OnEditchangeCombo1();
	CEdit m_info_PID;
	CEdit m_info_EP;
	CEdit m_info_HMODULE;
	CEdit m_info_IMG_SIZE;
	CEdit m_info_exe_path;
	CEdit m_info_module_count;
	CEdit m_info_parent_id;
	CEdit m_info_threads_count;
	CEdit m_info_handle_count;
	CEdit m_info_PEB;
	CStatic m_info_aa_lamp;
	CButton m_MORE_info_aa;
	CStatic m_info_origin_LAMP;
	CStatic m_info_steam_LAMP;
	afx_msg void OnBnClickedHunt();
	afx_msg void OnBnClickedVmIntercept();
//	afx_msg void OnBnClickedImportIntrecept();
	CEdit m_info_EP_SectionName;
	CEdit m_info_PE_Num_of_Sections;
	CButton m_intrecept_vm;
//	CButton m_intrecept_import_table;
	CButton m_hunt_action;
	CStatic m_info_HUNT_state;
	afx_msg void OnBnClickedUpdateList();
	CButton m_update_list;
	afx_msg void OnBnClickedMapIntrecept();
	CButton m_BUTTON_Patch_intrecept;
	afx_msg void OnBnClickedPatchInspector();
	afx_msg void OnBnClickedLicence();
	CEdit m_info_HMODULE_kernel32;
	CEdit m_info_HMODULE_kernelbase;
	CEdit m_info_HMODULE_ntdll;
	CButton m_BUTTON_Licence_raw_dump;
	afx_msg void OnBnClickedButtonLicenceRawDump();
	afx_msg void OnBnClickedVmvmp2Tracer();
	CButton m_tracer_vm;
	CEdit m_info_MAIN_THREAD_ID;
	afx_msg void OnBnClickedButtonReopen();
	CButton m_BUTTON_reopen;
	CButton m_BUTTON_DLC_licence_files;
	afx_msg void OnBnClickedButtonLicenceDlcFiles();
	CEdit m_EDIT_PROFILE_VMPX_CONTEXT_D4;
//	CEdit m_EDIT_PROFILE_VMPX_CONTEXT_v3new;
	CEdit m_EDIT_PROFILE_VMPX_CONTEXT_vmp2;
	CEdit m_EDIT_PROFILE_VMPX_CONTEXT_vmp3new;
	CStatic m_LAMP_context_D4_state;
	CStatic m_LAMP_context_vmp2_state;
	CStatic m_LAMP_context_vmp3new_state;
	afx_msg void OnChangeEditProfileVmpxContextVmp2();
	afx_msg void OnChangeEditProfileVmpxContextVmp3new();
	afx_msg void OnChangeEditProfileVmpxContextD4();
};
