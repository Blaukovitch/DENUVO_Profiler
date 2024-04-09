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
#include "TRACE_Tool_Cmp_master.h"
#include "afxdialogex.h"


// диалоговое окно CTRACE_Tool_Cmp_master

IMPLEMENT_DYNAMIC(CTRACE_Tool_Cmp_master, CDialogEx)

CTRACE_Tool_Cmp_master::CTRACE_Tool_Cmp_master(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTRACE_Tool_Cmp_master::IDD, pParent)
{
	LAMP_Get_ready_combo1 = false;
	LAMP_Get_ready_combo2 = false;
	vmp2_Info_stack1 = NULL;
	vmp2_Info_stack2 = NULL;

	memset(&Compare_two_result, 0, sizeof(Compare_two_result));
}

CTRACE_Tool_Cmp_master::~CTRACE_Tool_Cmp_master()
{
		codedef_check_free_allocatedif(vmp2_Info_stack1);
		codedef_check_free_allocatedif(vmp2_Info_stack2);
}

void CTRACE_Tool_Cmp_master::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TRACE1, m_COMBO_trace1);
	DDX_Control(pDX, IDC_COMBO_TRACE2, m_COMBO_trace2);
	DDX_Control(pDX, IDC_LAMP_TRACE1_READY, m_LAMP_TRACE1_READY);
	DDX_Control(pDX, IDC_LAMP_TRACE1_READY2, m_LAMP_TRACE2_READY);
	DDX_Control(pDX, IDC_TRACE_identical_to_first_diff, m_list_identical);
	DDX_Control(pDX, IDC_TRACE_different_2nd_trace, m_list_diff2);
	DDX_Control(pDX, IDC_TRACE_different_1st_trace, m_list_diff1);
	DDX_Control(pDX, IDC_EDIT_CALLS_COUNT_UNTIL_FIRST_DIFF, m_edit_calls_count_until_first_different);
}


BEGIN_MESSAGE_MAP(CTRACE_Tool_Cmp_master, CDialogEx)
	ON_CBN_EDITCHANGE(IDC_COMBO_TRACE1, &CTRACE_Tool_Cmp_master::OnEditchangeComboTrace1)
	ON_CBN_SELCHANGE(IDC_COMBO_TRACE1, &CTRACE_Tool_Cmp_master::OnSelchangeComboTrace1)
	ON_CBN_SELCHANGE(IDC_COMBO_TRACE2, &CTRACE_Tool_Cmp_master::OnSelchangeComboTrace2)
	ON_CBN_EDITCHANGE(IDC_COMBO_TRACE2, &CTRACE_Tool_Cmp_master::OnEditchangeComboTrace2)
END_MESSAGE_MAP()


// обработчики сообщений CTRACE_Tool_Cmp_master


BOOL CTRACE_Tool_Cmp_master::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Добавить дополнительную инициализацию
	Fill_comboBox();

	m_list_identical.InsertColumn(0,L"Addr",0,90,-1);
	m_list_identical.InsertColumn(1,L"Type",0,170,-1);
	m_list_identical.InsertColumn(2,L"Count",0,60,-1);

	m_list_diff1.InsertColumn(0,L"Addr",0,90,-1);
	m_list_diff1.InsertColumn(1,L"Type",0,170,-1);
	m_list_diff1.InsertColumn(2,L"Count",0,60,-1);

	m_list_diff2.InsertColumn(0,L"Addr",0,90,-1);
	m_list_diff2.InsertColumn(1,L"Type",0,170,-1);
	m_list_diff2.InsertColumn(2,L"Count",0,60,-1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}

void CTRACE_Tool_Cmp_master::GUI_UPDATE_AFTER_FILL_PROFILE()
{
	if(vmp2_Info_stack1 != NULL)
		m_LAMP_TRACE1_READY.SetBitmap(LAMP_TRUE);
	else
		m_LAMP_TRACE1_READY.SetBitmap(LAMP_FALSE);

	if(vmp2_Info_stack2 != NULL)
		m_LAMP_TRACE2_READY.SetBitmap(LAMP_TRUE);
	else
		m_LAMP_TRACE2_READY.SetBitmap(LAMP_FALSE);

		if(vmp2_Info_stack2 != NULL && vmp2_Info_stack1 != NULL)
		{
				m_list_identical.DeleteAllItems();
				m_list_diff1.DeleteAllItems();
				m_list_diff2.DeleteAllItems();
				::SetWindowTextW(m_edit_calls_count_until_first_different.m_hWnd, TS_DEF_NULL);

			if (DH_PROFILE_SPIRUN7_compare_vmp2_containers(vmp2_Info_stack1,vmp2_Info_stack2, &Compare_two_result) == NULL)
				Fill_compare_lists();
		}
}

bool CTRACE_Tool_Cmp_master::Fill_comboBox()
{
	UINT i = DH_Get_Current_hunt_profile_ALL_FILE_CONTAINERS(PROFILE_TYPE_TRACE_VMP2_LOG, &TRACE_file_lists[0]);
	if(i==0)
		{return false;}
	m_COMBO_trace1.ResetContent();
	m_COMBO_trace2.ResetContent();
	do
	{i--;m_COMBO_trace1.AddString(&TRACE_file_lists[i].FileName[0]);m_COMBO_trace2.AddString(&TRACE_file_lists[i].FileName[0]);}while(i != 0);
	return true;
}

void CTRACE_Tool_Cmp_master::Fill_compare_lists()
{
	TCHAR stack_buff[64];

	register SERVICE_COUNT ii_fill = Compare_two_result.Equal_hahelds_count;
	
	do //do equal list
	{
		itoa_16_fast(Compare_two_result.Equal_hahelds[ii_fill].Hanhled_addr, &stack_buff[0]);
	m_list_identical.InsertItem(0,&stack_buff[0]);
	m_list_identical.SetItemText(0,1,Compare_two_result.Equal_hahelds[ii_fill].Type_string);
		itoa_10_fast(Compare_two_result.Equal_hahelds[ii_fill].How_count, &stack_buff[0]);
	m_list_identical.SetItemText(0,2,&stack_buff[0]);
	}while(ii_fill--);

	ii_fill = Compare_two_result.Different_hahelds_1_count;
	do //do diff №1 list
	{
		itoa_16_fast(Compare_two_result.Different_hahelds_1[ii_fill].Hanhled_addr, &stack_buff[0]);
	m_list_diff1.InsertItem(0,&stack_buff[0]);
	m_list_diff1.SetItemText(0,1,Compare_two_result.Different_hahelds_1[ii_fill].Type_string);
		itoa_10_fast(Compare_two_result.Different_hahelds_1[ii_fill].How_count, &stack_buff[0]);
	m_list_diff1.SetItemText(0,2,&stack_buff[0]);
	}while(ii_fill--);


	ii_fill = Compare_two_result.Different_hahelds_2_count;
	do //do diff №2 list
	{
		itoa_16_fast(Compare_two_result.Different_hahelds_2[ii_fill].Hanhled_addr, &stack_buff[0]);
	m_list_diff2.InsertItem(0,&stack_buff[0]);
	m_list_diff2.SetItemText(0,1,Compare_two_result.Different_hahelds_2[ii_fill].Type_string);
		itoa_10_fast(Compare_two_result.Different_hahelds_2[ii_fill].How_count, &stack_buff[0]);
	m_list_diff2.SetItemText(0,2,&stack_buff[0]);
	}while(ii_fill--);

	itoa_10_fast(Compare_two_result.Equal_hahelds_until_first_different, &stack_buff[0]);
	::SetWindowTextW(m_edit_calls_count_until_first_different.m_hWnd, &stack_buff[0]);
}


void CTRACE_Tool_Cmp_master::OnEditchangeComboTrace1()
{
	// TODO: добавьте свой код обработчика уведомлений
	TCHAR filename[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN];

	ULONG c = ::GetWindowText(m_COMBO_trace1.m_hWnd, &filename[0], PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN);

	if(c == NULL)
		return;

	Get_ready_combo1_filename(&filename[0]);
	GUI_UPDATE_AFTER_FILL_PROFILE();
}


void CTRACE_Tool_Cmp_master::OnSelchangeComboTrace1()
{
	// TODO: добавьте свой код обработчика уведомлений
	TCHAR filename[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN];

	unsigned long cs = m_COMBO_trace1.GetCurSel();
		if (cs!=-1) {m_COMBO_trace1.GetLBText(::SendMessage(m_COMBO_trace1.m_hWnd, CB_GETCURSEL, 0, 0),&filename[0]);}

	Get_ready_combo1_filename(&filename[0]);
	GUI_UPDATE_AFTER_FILL_PROFILE();
}

void CTRACE_Tool_Cmp_master::Get_ready_combo1_filename(TCHAR* filename1)
{
	ULONG readsize;

		codedef_check_free_allocatedif(vmp2_Info_stack1);

	vmp2_Info_stack1 = (pSPIRUN7_TRACELOG_VMP2_INFO_STACK)DH_Read_Current_hunt_profile_CONTAINER(PROFILE_TYPE_TRACE_VMP2_LOG,filename1,PROFILE_READ_ALL, &readsize);

	if(vmp2_Info_stack1 == NULL)
		return;
}

void CTRACE_Tool_Cmp_master::Get_ready_combo2_filename(TCHAR* filename2)
{
		ULONG readsize;

		codedef_check_free_allocatedif(vmp2_Info_stack2);

	vmp2_Info_stack2 = (pSPIRUN7_TRACELOG_VMP2_INFO_STACK)DH_Read_Current_hunt_profile_CONTAINER(PROFILE_TYPE_TRACE_VMP2_LOG,filename2,PROFILE_READ_ALL, &readsize);

	if(vmp2_Info_stack2 == NULL)
		return;
}

void CTRACE_Tool_Cmp_master::OnSelchangeComboTrace2()
{
	// TODO: добавьте свой код обработчика уведомлений
	TCHAR filename2[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN];

	unsigned long cs = m_COMBO_trace2.GetCurSel();
		if (cs!=-1) {m_COMBO_trace2.GetLBText(::SendMessage(m_COMBO_trace2.m_hWnd, CB_GETCURSEL, 0, 0),&filename2[0]);}

	Get_ready_combo2_filename(&filename2[0]);
	GUI_UPDATE_AFTER_FILL_PROFILE();
}


void CTRACE_Tool_Cmp_master::OnEditchangeComboTrace2()
{
	// TODO: добавьте свой код обработчика уведомлений
	TCHAR filename2[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN];

	ULONG c = ::GetWindowText(m_COMBO_trace2.m_hWnd, &filename2[0], PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN);

	if(c == NULL)
		return;

	Get_ready_combo2_filename(&filename2[0]);
	GUI_UPDATE_AFTER_FILL_PROFILE();
}
