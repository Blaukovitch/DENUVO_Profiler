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
#include "VMTracevmp2Dlg.h"
#include "afxdialogex.h"
#include "TRACE_Tool_Cmp_master.h"

// диалоговое окно CVMTracevmp2Dlg

IMPLEMENT_DYNAMIC(CVMTracevmp2Dlg, CDialogEx)

CVMTracevmp2Dlg::CVMTracevmp2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVMTracevmp2Dlg::IDD, pParent)
{
	hSentryThread = NULL;
	active_cluster_trace_list.Total_HANHELD_calls_count = NULL;
}

CVMTracevmp2Dlg::~CVMTracevmp2Dlg()
{
	if (active_cluster_trace_list.Total_HANHELD_calls_count != NULL)
		DH_SPIRUN_Set_restore_mode(true,&active_trace_log_supply15->vm_name[0]);
	else
		DH_SPIRUN_Set_restore_mode(false,NULL);
	
	codedef_check_free_allocatedif(active_trace_log_supply15);

	codedef_check_term_threadif(hSentryThread);

	
}
DH_ENUM_FILES_of_PROFILES CVMTracevmp2Dlg::TRACE_file_lists[MAX_CONTAINTER_FILES];
DH_ENUM_FILES_of_PROFILES CVMTracevmp2Dlg::VM_file_lists[MAX_CONTAINTER_FILES];
pVMPX_VM_PROFILE_INFO_STACK CVMTracevmp2Dlg::active_trace_log_supply15;

void CVMTracevmp2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LAMP_SPIRUN7_CONNECT_STATE, m_LAMP_SPIRUN7_connect);
	DDX_Control(pDX, IDC_EDIT_TRACELOG_HANHELD_ID_ADDRES, m_EDIT_HANHELD_ID_Alloc_region_dump);
	DDX_Control(pDX, IDC_ASSIGN_VM, m_BUTTON_Assign_VM);
	DDX_Control(pDX, IDC_LIST_VM_ENUM, m_LIST_vm_enum);
	DDX_Control(pDX, IDC_SELECT_VM_RECORD, m_BUTTON_select_vm_record);
	DDX_Control(pDX, IDC_EDIT_ASSIGN_Supply15_Name, m_EDIT_assign_supply15);
	//  DDX_Control(pDX, IDC_BUTTON_Interception, m_BUTTON_Execute_intrecept);
	DDX_Control(pDX, IDC_BUTTON_Restore_ogirinal, m_BUTTON_Restore_orignal_supply15);
	DDX_Control(pDX, IDC_BUTTON_Interception, m_BUTTON_Execute_intrecept_supply15);
	DDX_Control(pDX, IDC_TRACE_LIST, m_list_trace);
	DDX_Control(pDX, IDC_TRACE_POPULAR_REPORT, m_list_popular_report);
	DDX_Control(pDX, IDC_EDIT_HANHELD_CALL_COUNT, m_EDIT_H_TOTAL_call_count);
	DDX_Control(pDX, IDC_EDIT_HANHELD_PCODE_SHIFT, m_EDIT_H_TOTAL_PCODE_shift);
	DDX_Control(pDX, IDC_EDIT_HANHELD_RBP_SHIFT, m_EDIT_H_TOTAL_RBP_dif);
	DDX_Control(pDX, IDC_EDIT_ASSIGN_TRACELOG_SAVE_FILENAME, m_EDIT_TRACELOG_SAVE_FILENAME);
	DDX_Control(pDX, IDC_ADD_TRACELOG_RECORD, m_BUTTON_ADD_TRACELOG_RECORD);
	DDX_Control(pDX, IDC_BUTTON_Trace_compare_master, m_BUTTON_Trace_compare_master);
	DDX_Control(pDX, IDC_LIST_TRACELOGS_ENUM, m_LIST_tracelogs_enum);
	//  DDX_Control(pDX, IDC_EDIT_HOW_HANHELDS_USE, m_EDIT_How_hanhelds_use);
	DDX_Control(pDX, IDC_EDIT_HOW_HANHELDS_USE, m_EDIT_H_TOTAL_hanhelds_use);
}


BEGIN_MESSAGE_MAP(CVMTracevmp2Dlg, CDialogEx)
	ON_BN_CLICKED(IDC_ASSIGN_VM, &CVMTracevmp2Dlg::OnBnClickedAssignVm)
	ON_BN_CLICKED(IDC_SELECT_VM_RECORD, &CVMTracevmp2Dlg::OnBnClickedSelectVmRecord)
	ON_BN_CLICKED(IDC_BUTTON_Interception, &CVMTracevmp2Dlg::OnBnClickedButtonInterception)
	ON_BN_CLICKED(IDC_BUTTON_Restore_ogirinal, &CVMTracevmp2Dlg::OnBnClickedButtonRestoreogirinal)
	ON_EN_CHANGE(IDC_EDIT_ASSIGN_TRACELOG_SAVE_FILENAME, &CVMTracevmp2Dlg::OnChangeEditAssignTracelogSaveFilename)
	ON_BN_CLICKED(IDC_ADD_TRACELOG_RECORD, &CVMTracevmp2Dlg::OnBnClickedAddTracelogRecord)
	ON_BN_CLICKED(IDC_BUTTON_Trace_compare_master, &CVMTracevmp2Dlg::OnBnClickedButtonTracecomparemaster)
END_MESSAGE_MAP()


// обработчики сообщений CVMTracevmp2Dlg


BOOL CVMTracevmp2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	GUI_OPENCLOSE_RIGHT_PANEL(false); 
	active_trace_log_supply15 = NULL;
	bIntrecept_state = false;
	bRestore_state = false;

	CurrProcessId = ::GetCurrentProcess();

	m_list_trace.InsertColumn(0,L"Handle",0,90,-1);
	m_list_trace.InsertColumn(1,L"Type",0,140,-1);

	m_list_popular_report.InsertColumn(0,L"Handle",0,90,-1);
	m_list_popular_report.InsertColumn(1,L"Type",0,140,-1);
	m_list_popular_report.InsertColumn(2,L"Count",0,90,-1);
	// TODO:  Добавить дополнительную инициализацию
	memset (&SPIRUN7_dll_info, NULL, sizeof(SPIRUN7_BLOCK));
	DH_SPIRUN_Init(&SPIRUN7_dll_info);

	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	m_BUTTON_Execute_intrecept_supply15.SetIcon(m_hIcon);
	
	if(::DH_SPIRUN_Get_restore_mode(&supply15_vm_text_data[0]) != NULL)
		GUI_Launch_Restore_Mode(&supply15_vm_text_data[0]);

	Fill_TRACE_profiles_list();
	GUI_UPDATE_AFTER_FILL_PROFILE();
	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}

void CVMTracevmp2Dlg::GUI_Launch_Restore_Mode(TCHAR* vmname)
{
	
	::SetWindowTextW(m_EDIT_assign_supply15.m_hWnd,&supply15_vm_text_data[0]);

	if(active_trace_log_supply15 != NULL)
		free(active_trace_log_supply15);

	active_trace_log_supply15 = DH_SPIRUN_Init_assign_vm_profile(&supply15_vm_text_data[0]);

	if (active_trace_log_supply15 == NULL)
		return;

		hSentryThread = ::CreateRemoteThreadEx(CurrProcessId,NULL,NULL, (LPTHREAD_START_ROUTINE)GUI_UPDATE_LIST_AND_CALCULATE_DATA,this,NULL,NULL, &SentryThreadId);
		DH_SPIRUN_Intrecept_Commander_Tower(IOBG_VMP2_SUPPLY15_TRACE_LOGGER,ICMD_VMP2_RESTORE_ORIGINAL_SUPPLY15,active_trace_log_supply15);
}

void CVMTracevmp2Dlg::GUI_UPDATE_AFTER_FILL_PROFILE()
{
	if (SPIRUN7_dll_info.SPIRUN7_HMODULE != NULL)
		{m_LAMP_SPIRUN7_connect.SetBitmap(LAMP_TRUE); ::EnableWindow(m_BUTTON_Assign_VM, true);}
	else
		{m_LAMP_SPIRUN7_connect.SetBitmap(LAMP_FALSE); ::EnableWindow(m_BUTTON_Assign_VM, false);}

	if(active_trace_log_supply15 != NULL)
	{
		if(bRestore_state == false && bIntrecept_state == false)
			{::EnableWindow(m_BUTTON_Execute_intrecept_supply15.m_hWnd, true); ::EnableWindow(m_BUTTON_Restore_orignal_supply15.m_hWnd, false);}

		if(bRestore_state == true && bIntrecept_state == false)
			{::EnableWindow(m_BUTTON_Execute_intrecept_supply15.m_hWnd, true); ::EnableWindow(m_BUTTON_Restore_orignal_supply15.m_hWnd, false);}

		if(bRestore_state == false && bIntrecept_state == true)
			{::EnableWindow(m_BUTTON_Execute_intrecept_supply15.m_hWnd, false); ::EnableWindow(m_BUTTON_Restore_orignal_supply15.m_hWnd, true);}
	}
	else
	{
		::EnableWindow(m_BUTTON_Execute_intrecept_supply15.m_hWnd,false);
		::EnableWindow(m_BUTTON_Restore_orignal_supply15.m_hWnd,false);
	}

	//	::EnableWindow(m_BUTTON_Restore_orignal_supply15.m_hWnd,supply15_assign_state);

	if (m_LIST_tracelogs_enum.GetCount() > 1)
		::EnableWindow(m_BUTTON_Trace_compare_master.m_hWnd, true);
	else
		::EnableWindow(m_BUTTON_Trace_compare_master.m_hWnd, false);
	
	TCHAR Byte_buffer[64];

	itoa_16_fast(SPIRUN7_dll_info.HANHELD_ID_dumping_region.region_start_addres,&Byte_buffer[0]);
	::SetWindowTextW(m_EDIT_HANHELD_ID_Alloc_region_dump.m_hWnd,&Byte_buffer[0]);

	
	::UpdateWindow(m_hWnd);
}

DWORD WINAPI CVMTracevmp2Dlg::GUI_UPDATE_LIST_AND_CALCULATE_DATA(LPVOID Param)
{
	TCHAR dig_bufferL[64];
	CVMTracevmp2Dlg *pThis = reinterpret_cast<CVMTracevmp2Dlg*>(Param);

	register SERVICE_COUNT i;
	do
	{
		bool sentry_err = DH_SPIRUN_Sentry_for_HANHELD_ID_dumping_region(pThis->active_trace_log_supply15,&pThis->active_cluster_trace_list, &pThis->active_cluster_popular_list, &pThis->SPIRUN7_dll_info.HANHELD_ID_dumping_region);
		if (sentry_err)
		{
			pThis->m_list_trace.DeleteAllItems();
			pThis->m_list_popular_report.DeleteAllItems();
			i = pThis->active_cluster_trace_list.Last__Fill_HANHELDs_count-1;
			register TCHAR* HANHELD_type_str;

			itoa_10_fast(pThis->active_cluster_trace_list.Total_HANHELD_calls_count,&dig_bufferL[0]);
			::SetWindowText(pThis->m_EDIT_H_TOTAL_call_count.m_hWnd,&dig_bufferL[0]);
			_ltow(pThis->active_cluster_trace_list.Total_PCODE_shift,&dig_bufferL[0], 10);
			::SetWindowText(pThis->m_EDIT_H_TOTAL_PCODE_shift.m_hWnd,&dig_bufferL[0]);
			_ltow(pThis->active_cluster_trace_list.Current_RBP_different,&dig_bufferL[0], 10);
			::SetWindowText(pThis->m_EDIT_H_TOTAL_RBP_dif.m_hWnd,&dig_bufferL[0]);

			register pHANHELD* pLast_Called_Hanheld_es = &pThis->active_cluster_trace_list.pLast__Called_HANHELD[1];
			pLast_Called_Hanheld_es--;
			do
			{
				pLast_Called_Hanheld_es++;
				if (*pLast_Called_Hanheld_es == 0)
					continue;

				pHANHELD pLast_Called_Hanheld = *pLast_Called_Hanheld_es;
				itoa_16_fast(pLast_Called_Hanheld->HANDELD_Primary_Addres,&dig_bufferL[0]);
				pThis->m_list_trace.InsertItem(0,&dig_bufferL[0]); //hanheld addr
				HANHELD_type_str = DH_VM_HANHELD_type_to_string(pLast_Called_Hanheld->HANDELD_type);
				if (HANHELD_type_str)
					pThis->m_list_trace.SetItemText(0,1, HANHELD_type_str); //тип hanheld
			}while(--i);

				i = VM_HANDLES_MAX_COUNT-1; //pThis->active_cluster_popular_list.pPopular_HANHELD
				size_t how_use = NULL;
				register pHANHELD* pHanheld_es = &pThis->active_cluster_popular_list.pPopular_HANHELD[1];
				SERVICE_COUNT*  pHanheldPopular_Count = &pThis->active_cluster_popular_list.pPopular_HANHELD_calls_count[1];
				pHanheld_es--;
				pHanheldPopular_Count--;
				do
				{
					pHanheld_es++;
					pHanheldPopular_Count++;
					if(*pHanheld_es == NULL)
						continue;

					register pHANHELD pHanheld = *pHanheld_es;
					itoa_16_fast(pHanheld->HANDELD_Primary_Addres,&dig_bufferL[0]);
					pThis->m_list_popular_report.InsertItem(0, &dig_bufferL[0]);
					HANHELD_type_str = DH_VM_HANHELD_type_to_string(pHanheld->HANDELD_type);
					if (HANHELD_type_str)
						pThis->m_list_popular_report.SetItemText(0,1, HANHELD_type_str); //тип hanheld
					itoa_10_fast(*pHanheldPopular_Count,&dig_bufferL[0]);
					pThis->m_list_popular_report.SetItemText(0,2, &dig_bufferL[0]);
					how_use++;
				}while(--i);
				itoa_10_fast(how_use,&dig_bufferL[0]);
				wcscat(&dig_bufferL[0],L"/");
				how_use = (pThis->active_trace_log_supply15->handles_count)-(pThis->active_trace_log_supply15->duplicate_handles_count);
				itoa_10_fast(how_use,&dig_bufferL[(wcslen(&dig_bufferL[0]))]);
				::SetWindowText(pThis->m_EDIT_H_TOTAL_hanhelds_use.m_hWnd, &dig_bufferL[0]);

		::EnableWindow(pThis->m_EDIT_TRACELOG_SAVE_FILENAME.m_hWnd, true);
		}//end if (sentry_err)

		::SleepEx(17,false);
	}while(1);

	return NULL;
}

void CVMTracevmp2Dlg::OnBnClickedAssignVm()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (SPIRUN7_dll_info.SPIRUN7_HMODULE == NULL)
		return;

	GUI_OPENCLOSE_RIGHT_PANEL(GUI_RIGHT_PANEL_state);
	Fill_VM_profiles_list();
}

void CVMTracevmp2Dlg::GUI_OPENCLOSE_RIGHT_PANEL(bool variant)
{
RECT rect = {0};
::GetWindowRect(m_hWnd, &rect);

	if (variant == true)
	::ShowWindow(m_LIST_vm_enum.m_hWnd, SW_SHOW);//::EnableWindow(m_LIST_vm_enum.m_hWnd,variant);
	else
	::ShowWindow(m_LIST_vm_enum.m_hWnd, SW_HIDE);

::EnableWindow(m_BUTTON_select_vm_record.m_hWnd,variant);


	if (variant)
	{
		::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0,  (rect.right-rect.left)+GUI_RIGHT_TAB_SIZE/*width*/, (rect.bottom-rect.top) /*height*/, SWP_NOMOVE|SWP_NOZORDER);
		::SetWindowTextW(m_BUTTON_Assign_VM.m_hWnd,_T("<"));
	}
	else
	{
		::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0,  (rect.right-rect.left)-GUI_RIGHT_TAB_SIZE/*width*/, (rect.bottom-rect.top) /*height*/, SWP_NOMOVE|SWP_NOZORDER);
		::SetWindowTextW(m_BUTTON_Assign_VM.m_hWnd,_T(">"));
	}

GUI_RIGHT_PANEL_state = !variant;
}

bool CVMTracevmp2Dlg::Fill_VM_profiles_list(void)
{
ULONG i = DH_Get_Current_hunt_profile_ALL_FILE_CONTAINERS(PROFILE_TYPE_VM, &VM_file_lists[0]);
	if(i==0)
		{return false;}

	::SendMessage(m_LIST_vm_enum.m_hWnd, LB_RESETCONTENT, 0, 0);  //m_LIST_tracelogs_enum.ResetContent();
	pDH_ENUM_FILES_of_PROFILES fpVMFileList = &VM_file_lists[0];
	do
	{
		::SendMessage(m_LIST_vm_enum.m_hWnd, LB_ADDSTRING, 0, (LPARAM)&fpVMFileList->FileName[0]); //m_LIST_tracelogs_enum.AddString(&TRACE_file_lists[i].FileName[0]);}
		fpVMFileList++;
	} while (--i);
	return true;
}

ULONG CVMTracevmp2Dlg::GUI_GET_VM_RECORD_SELECTED_FILENAME(__out WCHAR* FileName)
{
	ULONG csl = m_LIST_vm_enum.GetCurSel();

	if((ULONG)csl == 0xffffffff)
		return 0;


	if (m_LIST_vm_enum.GetTextLen(csl) > PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN)
		return 0;

		return m_LIST_vm_enum.GetText(csl,FileName);

}

void CVMTracevmp2Dlg::OnBnClickedSelectVmRecord()
{
	// TODO: добавьте свой код обработчика уведомлений
	
	if (!GUI_GET_VM_RECORD_SELECTED_FILENAME(&supply15_vm_text_data[0]))
			return;

	::SetWindowTextW(m_EDIT_assign_supply15.m_hWnd,&supply15_vm_text_data[0]);
	GUI_OPENCLOSE_RIGHT_PANEL(false);

	if(active_trace_log_supply15 != NULL)
		free(active_trace_log_supply15);

	active_trace_log_supply15 = DH_SPIRUN_Init_assign_vm_profile(&supply15_vm_text_data[0]);
	GUI_UPDATE_AFTER_FILL_PROFILE();
}



void CVMTracevmp2Dlg::OnBnClickedButtonInterception()
{
	// TODO: добавьте свой код обработчика уведомлений
	bIntrecept_state = DH_SPIRUN_Intrecept_Commander_Tower(IOBG_VMP2_SUPPLY15_TRACE_LOGGER,ICMD_VMP2_APLLY_INTRECEPT_SUPPLY15,active_trace_log_supply15);
	bRestore_state = !bIntrecept_state;
	GUI_UPDATE_AFTER_FILL_PROFILE();
	if (bIntrecept_state == true)
	{
		codedef_check_term_threadif(hSentryThread);
		hSentryThread = ::CreateRemoteThreadEx(CurrProcessId,NULL,NULL, (LPTHREAD_START_ROUTINE)GUI_UPDATE_LIST_AND_CALCULATE_DATA,this,NULL,NULL, &SentryThreadId);
	}
}


void CVMTracevmp2Dlg::OnBnClickedButtonRestoreogirinal()
{
	// TODO: добавьте свой код обработчика уведомлений
	bRestore_state = DH_SPIRUN_Intrecept_Commander_Tower(IOBG_VMP2_SUPPLY15_TRACE_LOGGER,ICMD_VMP2_RESTORE_ORIGINAL_SUPPLY15,active_trace_log_supply15);
	bIntrecept_state = !bRestore_state;
	GUI_UPDATE_AFTER_FILL_PROFILE();

	if (bRestore_state == true)
	{
		if(hSentryThread!= NULL)
			{::TerminateThread(hSentryThread,1); hSentryThread = NULL; }

		DH_SPIRUN_Set_initial_HANHELD_ID_dumping_region();
	}
}


void CVMTracevmp2Dlg::OnChangeEditAssignTracelogSaveFilename()
{
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Добавьте код элемента управления
	TCHAR text_buff_local[64];
	ULONG savenamelen = ::GetWindowText(m_EDIT_TRACELOG_SAVE_FILENAME.m_hWnd,&text_buff_local[0],PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN);

	if ((savenamelen != NULL) && (active_cluster_trace_list.Total_HANHELD_calls_count != NULL))
		::EnableWindow(m_BUTTON_ADD_TRACELOG_RECORD.m_hWnd, true);
	else
		::EnableWindow(m_BUTTON_ADD_TRACELOG_RECORD.m_hWnd, false);

}


void CVMTracevmp2Dlg::OnBnClickedAddTracelogRecord()
{
	// TODO: добавьте свой код обработчика уведомлений

	TCHAR text_buff_local[64];
	ULONG savenamelen = ::GetWindowText(m_EDIT_TRACELOG_SAVE_FILENAME.m_hWnd,&text_buff_local[0],PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN);

	if ((savenamelen == NULL) && (SPIRUN7_dll_info.HANHELD_ID_dumping_region.region_start_addres == NULL))
		return;
	
	SIZE_T BytesOutWrite;
	pSPIRUN7_TRACELOG_VMP2_INFO_STACK savestack = DH_PROFILE_SPIRUN7_Pack_save_profile_vmp2(&active_cluster_trace_list, &active_cluster_popular_list, &SPIRUN7_dll_info.HANHELD_ID_dumping_region, &active_trace_log_supply15->vm_name[0], &BytesOutWrite);

	if (savestack == NULL)
		return;

	DWORD WriteBy;
	DH_Write_Current_hunt_profile_CONTAINER(PROFILE_TYPE_TRACE_VMP2_LOG,&text_buff_local[0],(BYTE*)savestack,BytesOutWrite,&WriteBy);

	free(savestack);

	::SetWindowText(m_EDIT_TRACELOG_SAVE_FILENAME.m_hWnd, TS_DEF_NOTEXT);
	Fill_TRACE_profiles_list();
	GUI_UPDATE_AFTER_FILL_PROFILE();
}

bool CVMTracevmp2Dlg::Fill_TRACE_profiles_list(void)
{
register UINT i = DH_Get_Current_hunt_profile_ALL_FILE_CONTAINERS(PROFILE_TYPE_TRACE_VMP2_LOG, &TRACE_file_lists[0]);
	if(i==0)
		{return false;}
	::SendMessage(m_LIST_tracelogs_enum.m_hWnd, LB_RESETCONTENT, 0, 0);  //m_LIST_tracelogs_enum.ResetContent();
	pDH_ENUM_FILES_of_PROFILES fpTraceFileList = &TRACE_file_lists[0];
	do
	{
		::SendMessage(m_LIST_tracelogs_enum.m_hWnd, LB_ADDSTRING, 0, (LPARAM)&fpTraceFileList->FileName[0]); //m_LIST_tracelogs_enum.AddString(&TRACE_file_lists[i].FileName[0]);}
		fpTraceFileList++;
	}while (--i);
	return true;
}


void CVMTracevmp2Dlg::OnBnClickedButtonTracecomparemaster()
{
	// TODO: добавьте свой код обработчика уведомлений
	CTRACE_Tool_Cmp_master trace_cmp_master;
	trace_cmp_master.DoModal();
}
