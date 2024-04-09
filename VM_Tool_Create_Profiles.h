#pragma once
#include "ENGINE/ENGINE.h"
#include "ENGINE/ENGINE_VM_operations.h"
#include "ENGINE/ENGINE_Profile_operations.h"
#include "ENGINE/ENGINE_SPIRUN7_Communication.h"
#include "ENGINE/ENGINE_PIRATEMAP.h"

#define A_COMBO_DECODE_INSTRUCTION_ADD(instruction_name) ::SendMessage(m_COMBO_decode_instruction.m_hWnd, CB_ADDSTRING, 0, (LPARAM)&instruction_name);
#define GUI_RIGHT_TAB_SIZE_ToolCreateProrfile 405

#define codedef_check_term_threadif(threadid) if(threadid != NULL) ::TerminateThread(threadid, 1); threadid = NULL
#define codedef_save_presserve_PIRATEMAP pThis->Save_PIRATEMAPs.FileList_count++; pThis->fpPMAP_Main_info_stack->PirateMAP_addres_area_type = PMAP_TYPE_PE_IMAGE_SECTION; pThis->fpPMAP_Main_info_stack->PIRATEMAPPING_Region_start_addres = RegionStart; pThis->fpPMAP_Main_info_stack->PIRATEMAPPING_Region_size =  Current_Selected_SECTION_HEADER->Misc.VirtualSize; memcpy(&pThis->fpPMAP_Main_info_stack->PE_Section_Header, Current_Selected_SECTION_HEADER, sizeof(IMAGE_SECTION_HEADER)); pThis->fpPMAP_Main_info_stack++

#define TS_BUTTON_SCAN_STANDBY_MODE _T("Scan...")
#define TS_BUTTON_ABORT_MODE _T("Abort!")

#define LOWER_PROGRESS_RANGE 0
#define UPPER_PROGRESS_RANGE 100
// диалоговое окно CVM_Tool_Create_Profiles

class CVM_Tool_Create_Profiles : public CDialogEx
{
	DECLARE_DYNAMIC(CVM_Tool_Create_Profiles)

public:
	CVM_Tool_Create_Profiles(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~CVM_Tool_Create_Profiles();

	HANDLE hGUI_UpdateProgressState;
	HANDLE hGUI_MultiScan_work_thread;

	bool GUI_LAMP_FILE_NAME_CORRECT;
	bool GUI_LAMP_Supply15_ADDRES_CORRECT;
	bool GUI_LAMP_VM_ENTER_CORRECT;
	bool GUI_VMP2_exten; //radio1
	bool GUI_VMP3_exten; //radio2
	bool GUI_VM_D4_exten; //radio4
	bool GUI_VM_D5_exten; //radio5
	bool GUI_VMP_another; //radio3
	bool GUI_SCAN_MODE; //SCAN/ABORT mode BUTTON; true = scan/false = abort
	bool GUI_RIGHT_PANEL_state;//состояние правой панели
	
	// !СОСТАВНОЕ для формирования структуры //
	SERVICE_ADDRES xaddres; //адрес supply15 
	size_t xDECODE_INSRUCTION; //адрес supply15 
	SERVICE_ADDRES xDECODE_INSRUCTION_OPERAND; //адрес supply15 

	void GUI_UPDATE_NOW(void);
	void GUI_and_OPS_upd_decode_instruction_sets(WCHAR* buff);
	void Fill_PE_Sections_list(void);
	void GUI_OPENCLOSE_RIGHT_PANEL(bool variant);

	static DWORD WINAPI Work_multi_scan_proc(LPVOID Param);
	static DWORD WINAPI GUI_UPDATE_Progress_inf(LPVOID Param);
	
	HMODULE hTarget_Main_module; //HMODULE цели

	SERVICE_COUNT All_selected_section_count;
	SERVICE_COUNT All_eq_bytes_in_selected_sections;
	PIMAGE_SECTION_HEADER pSelectedSections[40];
	SERVICE_TYPE SelectedVMPX_version;

	SERVICE_COUNT Will_be_create_new_VM_profiles_count;
	SERVICE_COUNT Will_be_create_new_billets_count;
	SERVICE_COUNT Will_be_create_new_PIRATEMAPs;

	PIRATEMAP_FILELIST_INFO Save_PIRATEMAPs; //ЗАГОТОВКА для сохранения PIRATEMAPS (заранее обработать по причине парсинга секций PE-файла)
	pPIRATEMAP_PROFILE_MAIN_INFO fpPMAP_Main_info_stack; //плавающий указатель на PIRATEMAP_PROFILE_MAIN_INFO
// Данные диалогового окна
	enum { IDD = IDD_VM_DLG_CREATE_NEW_VM_PROFILE };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	pVM_VMPX_PROFILE_INI_STACK VMX_D4;
	pVM_VMPX_PROFILE_INI_STACK VMPX_3;
	pVM_VMPX_PROFILE_INI_STACK VMPX_2;
	PROGRESS_MULTI_SCAN_INFO_STACK GUI_Progress_multi_scan_information;

	pVM_VMPX_PROFILE_FOUND_SEQUENCE_STACK pFoundSeqStack_array[MAX_FOUND_REGION_IN_SEQ];
	pVM_VMPX_PROFILE_FOUND_SEQUENCE_STACK* fpFoundSeqStack_array;

	pVM_THREADWORK pActiveThreadWork[MAX_FOUND_REGION_IN_SEQ];
	pVM_THREADWORK* fpActiveThreadWork_array;

	SERVICE_COUNT CurrentFoundSeq_stack;
	SERVICE_COUNT CurrentThreadWork_stack;
public:
	virtual BOOL OnInitDialog();
//	CEdit m_VM_name;
	CEdit m_EDIT_VM_handles_count;
	CEdit m_EDIT_VM_Name;
	CEdit m_EDIT_VM_commentaries;
	CEdit m_EDIT_VMPX_stack_reserved_bytes;
	CButton m_BUTTON_Accept;
	CStatic m_LAMP_Sypply15_addres_check;
	CStatic m_LAMP_VM_ENTER_addres_state;
	CComboBox m_COMBO_decode_instruction;
	CEdit m_EDIT_Supply15_start_addres;
	CButton m_RADIO_ver_VMP3;
	CButton m_RADIO_ver_VMP2;
	CButton m_RADIO_ver_another;
	CStatic m_LAMP_VM_FILE_NAME;
	afx_msg void OnEnChangeVmNameEdit();
	afx_msg void OnEnChangeVmSupply15startaddresEdit();
	afx_msg void OnBnClickedVmVerVmp2Radio();
	afx_msg void OnBnClickedVmVerVmp3Radio2();
	afx_msg void OnBnClickedVmVeranotherRadio3();
	afx_msg void OnBnClickedAcceptButton();
	afx_msg void OnEnChangeVmSupply15startaddresEdit2();
	CEdit m_EDIT_LOCK_supply15_sectionname;
	CEdit m_EDIT_Supply15_DEcode_OPERAND;
	afx_msg void OnCbnSelchangeVmSupplyDecodeCommand();
	afx_msg void OnEditchangeVmSupplyDecodeCommand();
	CEdit m_EDIT_vm_enter_addres;
	CEdit m_EDIT_vm_enter_addres_SECTION_NAME;
	afx_msg void OnEnChangeEditvmenteraddres();
	CEdit m_EDIT_dissasm_mnemonic_text;
	CListBox m_LIST_PE_Sections_Scan;
	CButton m_CHECK_EnableMultiScan;
	afx_msg void OnBnClickedCheckMultiScan();
//	afx_msg void OnHotitemchangeCheckMultiScan(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSetfocusCheckMultiScan();
	afx_msg void OnBnClickedButtonScanMulti();
	afx_msg void OnSelchangeListSections();
	CEdit m_EDIT_Sel_SECTIONS_COUNT;
	CEdit m_EDIT_Sel_EQ_BYTES_COUNT;
	CButton m_BUTTON_MULTI_SCAN;
	CProgressCtrl m_PROGRESS_MULTI;
	afx_msg void OnBnClickedButtonSingleMultiPanelShow();
	CButton m_BUTTON_RIGHT_PANEL;
	CEdit m_EDIT_FOUND_VM_ENTERs;
	CEdit m_EDIT_FOUND_VM_HANHELDs;
	CEdit m_EDIT_FOUND_VM_CONTEXTs;
	CButton m_BUTTON_Accept_multi;
	afx_msg void OnBnClickedAcceptButton2();
	CEdit m_EDIT_RESEARCHED_VM_CONTEXTs;
	CEdit m_EDIT_IDENTIFICATED_VM_CONTEXTs_percent;
	CEdit m_EDIT_WILL_BE_CREATE_VM_CONTEXTs;
	CEdit m_EDIT_WILL_BE_CREATE_BILLETSs;
	CEdit m_EDIT_WILL_BE_CREATE_PIRATEMAPs;
	CEdit m_EDIT_CREATE_MANUAL_Last_ERROR;
	afx_msg void OnBnClickedButtonSelectAllSection();
	CButton m_BUTTON_Sel_all_PE_Sections;
	afx_msg void OnBnClickedVmVerD4Radio3();
	afx_msg void OnBnClickedVmVerD4Radio();
};
