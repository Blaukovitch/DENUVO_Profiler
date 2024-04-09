#pragma once

#include "ENGINE/ENGINE.h"
#include "ENGINE/ENGINE_Profile_operations.h"
#include "ENGINE/ENGINE_MAP_operations.h"
// диалоговое окно CMapCompareDlg

#define err_caption L"ERROR"
#define err_too_many_array L"It is too much data in the unit!"
#define err_eq_files L"This sections are equivalent!"
#define err_too_many_diff L"Too many diffrences count! Operation aborted."

#define MAX_FILTER_CHECK_BOX 9
#define CHECK_BOOL_MAP MAX_FILTER_CHECK_BOX+1

#define def_code_Fill_cell itoa_16_fast(fpCompare_event_table->Start_diff_Address,&dig_bufferT[0]); m_LIST_map.InsertItem(0,&dig_bufferT[0]); itoa_10_fast(fpCompare_event_table->Diff_Bytes_Count,&dig_bufferT[0]); m_LIST_map.SetItemText(0,1, &dig_bufferT[0])

class CMapCompareDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMapCompareDlg)

public:
	CMapCompareDlg(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~CMapCompareDlg();

// Данные диалогового окна
	enum { IDD = IDD_MAP_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	virtual BOOL OnInitDialog();
	void GUI_UPDATE_NOW(void);
	void GUI_UPDATE_COMPARE_INFO(void);
	void GUI_FILL_LIST(void);
	void GUI_FILL_LIST_filter(void);
	bool Apply_Filter_state;
	bool check_state[CHECK_BOOL_MAP];
	bool file_num_state;
	void GUI_SECTION_NAME_WORKSHOP(char* NewSectionName);
	SERVICE_ADDRES Filter_addres_start;
	SERVICE_ADDRES Filter_addres_end;

	DECLARE_MESSAGE_MAP()
public:
	HMODULE Load_PE_FILE_1;
	HMODULE Load_PE_FILE_2;


	//HEADER_FILES_COMPARE_RESULTS Compare_info_set;

	pMAP_IDENTICAL_SECTIONS_STACK pIdentSectionStacked; //ALLOC!
	char* pProtSectionName;

	pSET_FILES_COMPARE_EVENT pAllocated_compare_event_table; //ALLOC!
	LPBYTE pAllocated_COMPARE_BYTES_buffer; //ALLOC!

	bool LAMP_Load_State_PE_FILE_1;
	bool LAMP_Load_State_PE_FILE_2;

	afx_msg void OnEnChangeEditPeFile1Path();
	afx_msg void OnEnChangeEditPeFile2Path();
	CEdit m_EDIT_path_PE_File1;
	CEdit m_EDIT_path_PE_File2;
	CStatic m_LAMP_load_state_file1;
	CStatic m_LAMP_load_state_file2;


	afx_msg void OnBnClickedButtonCompareStart();
	CComboBox m_EDIT_protection_section_name;
	CEdit m_EDIT_diff_count;
	CListCtrl m_LIST_map;
//	CListCtrl m_list1;
	afx_msg void OnClickListMap(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_EDIT_report_diff_count1;
	CEdit m_EDIT_report_diff_count2;
	CEdit m_EDIT_report_diff_count3;
	CEdit m_EDIT_report_diff_count4;
	CEdit m_EDIT_report_diff_count5;
	CEdit m_EDIT_report_diff_count6;
	CEdit m_EDIT_report_diff_count7;
	CEdit m_EDIT_report_diff_count8;
//	CEdit m_EDIT_report_diff_count9;
	CEdit m_EDIT_report_diff_count_above_9;
	CEdit m_EDIT_Filter_addres_start;
	CEdit m_EDIT_Filter_addres_end;
	CButton m_CHECK_diff_count1;
	CButton m_CHECK_diff_count2;
	CButton m_CHECK_diff_count3;
	CButton m_CHECK_diff_count4;
	CButton m_CHECK_diff_count5;
	CButton m_CHECK_diff_count6;
	CButton m_CHECK_diff_count7;
	CButton m_CHECK_diff_count8;
	CButton m_CHECK_diff_count_above9;
	CButton m_CHECK_Apply_filter;
	CEdit m_EDIT_Diff_CLIPPED;
	CEdit m_EDIT_AVIABLE_IDENTI_SECTION_COUNT;
//	afx_msg void OnBnClickedCheckDiffCount1();
//	afx_msg void OnHotitemchangeCheckDiffCount1(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnDropdownCheckDiffCount1(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnThemechangedCheckDiffCount1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickedCheckDiffCount1();
	afx_msg void OnClickedCheckDiffCount2();
	afx_msg void OnClickedCheckDiffCount3();
	afx_msg void OnClickedCheckDiffCount4();
	afx_msg void OnClickedCheckDiffCount5();
	afx_msg void OnClickedCheckDiffCount6();
	afx_msg void OnClickedCheckDiffCount7();
	afx_msg void OnClickedCheckDiffCount8();
	afx_msg void OnClickedCheckDiffCountAbove9();
//	afx_msg void OnLvnItemchangedListMap(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemclickListMap(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_EDIT_trim_addres;
	CButton m_BUTTON_COMPARE;
//	CEdit m_EDIT_diff_bytes_enum1;
//	CEdit m_EDIT_diff_bytes_enum2;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnEditchangeEditProtectionSection();
	afx_msg void OnSelchangeEditProtectionSection();
	afx_msg void OnBnClickedCheckApplyFilter();
	afx_msg void OnChangeEditFilterAddresHideStart();
	afx_msg void OnChangeEditFilterAddresHideEnd();
};
