#pragma once
#include "ENGINE/ENGINE.h"
#include "ENGINE/ENGINE_VM_operations.h"
#include "ENGINE/ENGINE_Profile_operations.h"
#include "ENGINE/ENGINE_MAP_operations.h"
#include "ENGINE/ENGINE_Licence_operations.h"
// CVMDlg dialog
#define nBaseId 0x0E00 // Your own base id

class CVMDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVMDlg)

public:
	CVMDlg(CWnd* pParent = NULL);   //standard constructor
	//pVM_INFO_STACK VM_current_stack;
	static DH_ENUM_FILES_of_PROFILES VM_file_lists[MAX_CONTAINTER_FILES];
	SERVICE_COUNT VM_file_lists_count;
	bool BST_CHECK_exclude_duplicate; //показывать или исключать дублирующиеся позиции в supply15
	
	bool Fill_VM_profiles_list(void); //заполняет список допустных профилей VM
	bool Fill_selected_VM_information(WCHAR* VM_NAME); //заполняет данные по выбранному профилю

	bool Is_checl_only_selected_vm_profile;//искать только в только выбранный VM профиль (в GUI)

	void GUI_UPDATE_AFTER_FILL_PROFILE(bool Fill_result); //управляет состоянием окна после обновления списка профилей VM

	UINT GUI_GET_VM_RECORD_SELECTED_FILENAME(__out WCHAR* FileName); //возвращает выбранным пользователем текст в списке профилей VM

	static bool Load_current_VM_profile_and_fill_all_info_blocks(__in WCHAR* VM_NAME, __in LPVOID pParam); //ЗАГРУЖАЕТ выбранный профиль с формированием списка "островков" и обновлением данных о выбранном профиле в инфо-блоке
	static bool Load_current_VM_profile_and_fill_all_info_blocks__manual_hanheld_show(__in WCHAR* VM_NAME, __in LPVOID pParam, __in pHANHELD* ppHanheldStack_start, __in SERVICE_COUNT Stack_count); //ЗАГРУЖАЕТ выбранный профиль с формированием списка "островков" ИЗ АРГУМЕНТА и обновлением данных о выбранном профиле в инфо-блоке

	pHANHELD Find_selected_hanheld_in_current_VM_stack(WCHAR* wchar_hanheld_addres, ULONG wtext_len); //находит заданный хендлик в текущем загруженном стеке

	WCHAR LOCK_VM_PROFILE_filename[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN];
	size_t LOCK_VM_PROFILE_filename_len;

	pVMPX_VM_PROFILE_INFO_STACK LOCK_VM_PROFILE_stack;

	static void SearchObject_Ops(LPVOID pParam);

	static void GUI_Switch_LISTS(__in bool list1, __in LPVOID pParam);//переключает списки list1(main) и list2(search)

	virtual ~CVMDlg();

// Dialog Data
	enum { IDD = IDD_VM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		virtual BOOL OnInitDialog();
		//patch inspector
		void AddPatchInspectorOps_AllVM_readwriteDS(); //ReadWrite Data section - all VMs
		void AddPatchInspectorOps_AllVM_randRDTSC(); //RDTSC - all VMs
		void AddPatchInspectorOps_AllVM_CRC_check(); //xor al, [rdx] (CRC check) - all VMs

	DECLARE_MESSAGE_MAP()
public:
//	CListBox m_VM_HANDLES;
//	CListBox m_VM_HANDLES_DISASM;
//	CListBox m_VM_SUPPLY_LIST;
//	CStatic m_LAMP_vaid_supply;
//	CStatic m_LAMP_sypply_handles;
//	CEdit m_edit_address_supply;
//	afx_msg void OnBnClickedSupplyOps();
//	CButton m_supply_edit_add;
	CListCtrl m_list;
	CListCtrl m_SEARCH_list;
//	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnBnClickedAddVmRecord();
	CListBox m_list_vms_records;
//	afx_msg void OnBnClickedAcceptAddVmRecord();
	afx_msg void OnBnClickedAddVmRecord();
	afx_msg void OnLbnSelchangeListVmEnum();
//	CEdit m_EDIT_info_VM_comments;
	CEdit m_EDIT_info_vm_count_consist_of;
	CEdit m_EDIT_info_VM_supply15_adress;
	CEdit m_EDIT_info_VM_version;
	afx_msg void OnBnClickedSelectVmRecord();
//	CButton m_BUTTON_Secet_VM_record;
	CButton m_BUTTON_Select_VM_record;
//	CEdit m_EDIT_info_HANHELD_NUM;
	CEdit m_EDIT_info_HANHELD_ADDRES;
	CEdit m_EDIT_info_HANHELD_NUM;
//	afx_msg void OnLinkclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickList1(NMHDR *pNMHDR, LRESULT *pResult);
//	CEdit m_EDIT_info_vm_count_consist_of;
	afx_msg void OnBnClickedCheckExcludeDuplicateInTable();
	CButton m_CHECK_exclude_duplicate_handles;
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_EDIT_info_JUMPOUTs_count;
	CEdit m_EDIT_info_crit_addr_count;
	CEdit m_EDIT_info_stack_reserved_bytes;
	CEdit m_EDIT_info_context_site;
	CEdit m_EDIT_SEARCH_object;
	CButton m_CHECK_Search_only_selected_vm_profile;
	CEdit m_EDIT_search_objects_count;
	CEdit m_EDIT_search_objects_SUB_count;
	afx_msg void OnKeydownList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnChangeEditSearchObject();
	afx_msg void OnBnClickedCheckSearchOnlySelectedVmProfile();
};
