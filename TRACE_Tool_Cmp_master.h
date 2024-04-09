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
#include "ENGINE\ENGINE_SPIRUN7.h"
#include "ENGINE\ENGINE_Profile_operations.h"

// диалоговое окно CTRACE_Tool_Cmp_master

class CTRACE_Tool_Cmp_master : public CDialogEx
{
	DECLARE_DYNAMIC(CTRACE_Tool_Cmp_master)

public:
	CTRACE_Tool_Cmp_master(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~CTRACE_Tool_Cmp_master();

// Данные диалогового окна
	enum { IDD = IDD_VM_TRACER2_TRACE_COMPARE_MASTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	bool Fill_comboBox(void); //заливка комбобоксов профилями
	DH_ENUM_FILES_of_PROFILES TRACE_file_lists[MAX_CONTAINTER_FILES]; //трейслист
	void Get_ready_combo1_filename(TCHAR* filename1);
	bool LAMP_Get_ready_combo1;
	pSPIRUN7_TRACELOG_VMP2_INFO_STACK vmp2_Info_stack1;
	void Get_ready_combo2_filename(TCHAR* filename2);
	pSPIRUN7_TRACELOG_VMP2_INFO_STACK vmp2_Info_stack2;
	bool LAMP_Get_ready_combo2;
	void GUI_UPDATE_AFTER_FILL_PROFILE();
	SPIRUN7_TRACELOG_VMP2_COMPARE_INFO_CLUSTER Compare_two_result;
	void Fill_compare_lists();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_COMBO_trace1;
	CComboBox m_COMBO_trace2;
	afx_msg void OnEditchangeComboTrace1();
	afx_msg void OnSelchangeComboTrace1();
	CStatic m_LAMP_TRACE1_READY;
	CStatic m_LAMP_TRACE2_READY;
	afx_msg void OnSelchangeComboTrace2();
	afx_msg void OnEditchangeComboTrace2();
	CListCtrl m_list_identical;
	CListCtrl m_list_diff2;
	CListCtrl m_list_diff1;
	CEdit m_edit_calls_count_until_first_different;
};
