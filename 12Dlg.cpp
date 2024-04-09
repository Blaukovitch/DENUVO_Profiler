
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
#include "12Dlg.h"
#include "afxdialogex.h"
#include "VMDlg.h"
#include "MapCompareDlg.h"
#include "PatchDlg.h"
#include "VM_DLG_NewWave.h"
#include "LicDlg.h"
#include "VMTracevmp2Dlg.h"
#include "СDLC_Lic_Files.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMy12Dlg dialog




CMy12Dlg::CMy12Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMy12Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	LAMP_FALSE.LoadBitmapW(IDB_FALSE);
	LAMP_TRUE.LoadBitmapW(IDB_TRUE);
}

void CMy12Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_select_target);
	DDX_Control(pDX, IDC_ProINFO_PID, m_info_PID);
	DDX_Control(pDX, IDC_ProINFO_ENTRYPOINT, m_info_EP);
	DDX_Control(pDX, IDC_ProINFO_HMODULE, m_info_HMODULE);
	DDX_Control(pDX, IDC_ProINFO_IMG_SIZE, m_info_IMG_SIZE);
	DDX_Control(pDX, IDC_ProINFO_EXE_PATH, m_info_exe_path);
	DDX_Control(pDX, IDC_ProINFO_Parent_Module_COunt, m_info_module_count);
	DDX_Control(pDX, IDC_ProINFO_Parent_PID, m_info_parent_id);
	DDX_Control(pDX, IDC_ProINFO_Parent_Thread_COunt, m_info_threads_count);
	DDX_Control(pDX, IDC_ProINFO_Handle_count, m_info_handle_count);
	DDX_Control(pDX, IDC_ProINFO_PEB, m_info_PEB);
	DDX_Control(pDX, IDC_AA_STATE, m_info_aa_lamp);
	DDX_Control(pDX, IDC_MORE_INFO_AA, m_MORE_info_aa);
	DDX_Control(pDX, IDC_ORIGIN_LAMP, m_info_origin_LAMP);
	DDX_Control(pDX, IDC_STEAM_LAMP, m_info_steam_LAMP);
	DDX_Control(pDX, IDC_ProINFO_ENTRYPOINT_Section_name, m_info_EP_SectionName);
	DDX_Control(pDX, IDC_ProINFO_PE_NumOfSections, m_info_PE_Num_of_Sections);
	DDX_Control(pDX, IDC_VM_INTERCEPT, m_intrecept_vm);
	//  DDX_Control(pDX, IDC_IMPORT_INTRECEPT, m_intrecept_import_table);
	DDX_Control(pDX, IDC_HUNT, m_hunt_action);
	DDX_Control(pDX, IDC_LAMP_HUNT_STATE, m_info_HUNT_state);
	DDX_Control(pDX, IDC_UPDATE_LIST, m_update_list);
	DDX_Control(pDX, IDC_PATCH_INSPECTOR, m_BUTTON_Patch_intrecept);
	DDX_Control(pDX, IDC_ProINFO_HMODULE_kernel32, m_info_HMODULE_kernel32);
	DDX_Control(pDX, IDC_ProINFO_HMODULE_kernelbase, m_info_HMODULE_kernelbase);
	DDX_Control(pDX, IDC_ProINFO_HMODULE_ntdll, m_info_HMODULE_ntdll);
	DDX_Control(pDX, IDC_BUTTON_LICENCE_RAW_DUMP, m_BUTTON_Licence_raw_dump);
	DDX_Control(pDX, IDC_VM_vmp2_TRACER, m_tracer_vm);
	DDX_Control(pDX, IDC_ProINFO_main_thread_id, m_info_MAIN_THREAD_ID);
	DDX_Control(pDX, IDC_BUTTON_REOPEN, m_BUTTON_reopen);
	DDX_Control(pDX, IDC_BUTTON_LICENCE_DLC_FILES, m_BUTTON_DLC_licence_files);
	DDX_Control(pDX, IDC_EDIT_PROFILE_VMPX_CONTEXT_D4, m_EDIT_PROFILE_VMPX_CONTEXT_D4);
	//  DDX_Control(pDX, IDC_EDIT_PROFILE_VMPX_CONTEXT_vmp2, m_EDIT_PROFILE_VMPX_CONTEXT_v3new);
	DDX_Control(pDX, IDC_EDIT_PROFILE_VMPX_CONTEXT_vmp2, m_EDIT_PROFILE_VMPX_CONTEXT_vmp2);
	DDX_Control(pDX, IDC_EDIT_PROFILE_VMPX_CONTEXT_vmp3new, m_EDIT_PROFILE_VMPX_CONTEXT_vmp3new);
	DDX_Control(pDX, IDC_CONTEXT__D4_STATE, m_LAMP_context_D4_state);
	DDX_Control(pDX, IDC_CONTEXT__vmp2_STATE, m_LAMP_context_vmp2_state);
	DDX_Control(pDX, IDC_CONTEXT__vmp3new_STATE, m_LAMP_context_vmp3new_state);
}

BEGIN_MESSAGE_MAP(CMy12Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMy12Dlg::OnCbnSelchangeCombo1)
//	ON_CBN_EDITUPDATE(IDC_COMBO1, &CMy12Dlg::OnEditupdateCombo1)
ON_CBN_EDITCHANGE(IDC_COMBO1, &CMy12Dlg::OnEditchangeCombo1)
ON_BN_CLICKED(IDC_HUNT, &CMy12Dlg::OnBnClickedHunt)
ON_BN_CLICKED(IDC_VM_INTERCEPT, &CMy12Dlg::OnBnClickedVmIntercept)
//ON_BN_CLICKED(IDC_IMPORT_INTRECEPT, &CMy12Dlg::OnBnClickedImportIntrecept)
ON_BN_CLICKED(IDC_UPDATE_LIST, &CMy12Dlg::OnBnClickedUpdateList)
ON_BN_CLICKED(IDC_MAP_INTRECEPT, &CMy12Dlg::OnBnClickedMapIntrecept)
ON_BN_CLICKED(IDC_PATCH_INSPECTOR, &CMy12Dlg::OnBnClickedPatchInspector)
ON_BN_CLICKED(IDC_BUTTON_LICENCE_RAW_DUMP, &CMy12Dlg::OnBnClickedButtonLicenceRawDump)
ON_BN_CLICKED(IDC_VM_vmp2_TRACER, &CMy12Dlg::OnBnClickedVmvmp2Tracer)
ON_BN_CLICKED(IDC_BUTTON_REOPEN, &CMy12Dlg::OnBnClickedButtonReopen)
ON_BN_CLICKED(IDC_BUTTON_LICENCE_DLC_FILES, &CMy12Dlg::OnBnClickedButtonLicenceDlcFiles)
ON_EN_CHANGE(IDC_EDIT_PROFILE_VMPX_CONTEXT_vmp2, &CMy12Dlg::OnChangeEditProfileVmpxContextVmp2)
ON_EN_CHANGE(IDC_EDIT_PROFILE_VMPX_CONTEXT_vmp3new, &CMy12Dlg::OnChangeEditProfileVmpxContextVmp3new)
ON_EN_CHANGE(IDC_EDIT_PROFILE_VMPX_CONTEXT_D4, &CMy12Dlg::OnChangeEditProfileVmpxContextD4)
END_MESSAGE_MAP()


// CMy12Dlg message handlers

BOOL CMy12Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	//ORIGIN_INIT("?");

	DH__INIT_DENUVO_PROFILER(); //INIT FIRST!!!
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;

			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, _T("aBOUT..."));
		//DH_Engine dh;
	
	}

	test(&EnumProcBuffer);
	
	Update_Process_List(); //update first

	DH_InitProfiles_Engine_Path(&Profle_Folder[0]); //CRITICAL init profile

		TCHAR procname[MAX_PATH];
		if (GUI_APPROACH_COMMAND_LINE(&procname[0]) == true)
		{
			if(START_HUNT_Ops(&procname[0]) == true)
			{
				GUI_Set_Enable_Windows_when_open_close_target(HUNT_HIVE_ACTIVE);
				GUI_SHOW_PROCESS_INFO();
				::SetWindowText(m_select_target.m_hWnd, &procname[0]);
			}
		}
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_EDIT_PROFILE_VMPX_CONTEXT_D4.SetLimitText(PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN);
	m_EDIT_PROFILE_VMPX_CONTEXT_vmp2.SetLimitText(PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN);
	m_EDIT_PROFILE_VMPX_CONTEXT_vmp3new.SetLimitText(PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN);

	::SetWindowText(m_EDIT_PROFILE_VMPX_CONTEXT_D4.m_hWnd, L"4x.ini");
	::SetWindowText(m_EDIT_PROFILE_VMPX_CONTEXT_vmp2.m_hWnd, L"2x.ini");
	::SetWindowText(m_EDIT_PROFILE_VMPX_CONTEXT_vmp3new.m_hWnd, L"3x.ini");
	// TODO: Add extra initialization here
	ORIGIN_INIT("");

	return TRUE;  // return TRUE  unless you set the focus to a control
	}


void CMy12Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMy12Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMy12Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CMy12Dlg::OnCbnSelchangeCombo1()
{
	
	// TODO: Add your control notification handler code here
	unsigned long cs = m_select_target.GetCurSel();
		if (cs!=-1) {m_select_target.GetLBText(::SendMessage(m_select_target.m_hWnd, CB_GETCURSEL, 0, 0),&buffer[0]);}

	if(Get_Selected_Target(&buffer[0],&iPointers))
		GUI_SHOW_PROCESS_INFO();
}




void CMy12Dlg::OnEditchangeCombo1()
{
	// TODO: Add your control notification handler code here

}


void CMy12Dlg::OnBnClickedHunt()
{
	//проверяем выбор
	if(HUNT_HIVE_ACTIVE==false)
		{
			unsigned long cs = m_select_target.GetCurSel();
			if (cs!=-1) {m_select_target.GetLBText(::SendMessage(m_select_target.m_hWnd, CB_GETCURSEL, 0, 0),&buffer[0]);}
		//	::GetWindowText(m_select_target.m_hWnd,&buffer[0],MAX_PATH);

			if(!Get_Selected_Target(&buffer[0],&iPointers))
			{return;}

			START_HUNT_Ops(&buffer[0]);
		}
		else
			DH_CloseDENUVO(OPT_LOCK_Target);
	//работа с GUI
	GUI_Set_Enable_Windows_when_open_close_target(HUNT_HIVE_ACTIVE);
}


bool CMy12Dlg::Get_Selected_Target(__out TCHAR* Name, __out UINT64* Found_Hive_Pointer)
{
		size_t ii = DH_Main_Count;
	
		My_DH_Hive_pointer = (pDH_ProcInfo) EnumProcBuffer;
	
	do
	{
		if(!wcscmp(Name,&My_DH_Hive_pointer->ProcessName[0]))
		{
			*Found_Hive_Pointer=ii;
			return true;
		}
		My_DH_Hive_pointer++;
	}while(ii--);
	*Found_Hive_Pointer=0;
	return false;
}
void __forceinline CMy12Dlg::GUI_Set_Enable_Windows_when_open_close_target(__in bool state)
{
	::EnableWindow(m_select_target.m_hWnd,!state);
	::EnableWindow(m_intrecept_vm.m_hWnd,state);
	::EnableWindow(m_tracer_vm.m_hWnd,state);
	::EnableWindow(m_update_list.m_hWnd, !state);
	::EnableWindow(m_BUTTON_Patch_intrecept.m_hWnd, state);
	::EnableWindow(m_BUTTON_Licence_raw_dump.m_hWnd, state);
	::EnableWindow(m_BUTTON_reopen.m_hWnd, state);
	//::EnableWindow(m_BUTTON_DLC_licence_files.m_hWnd, state);

	if(!state)
	{::SetWindowTextW(m_hunt_action.m_hWnd,GUI_HUNT_OPEN_ACTION_LABEL);
	m_info_HUNT_state.SetBitmap(LAMP_FALSE);}
	else
	{::SetWindowTextW(m_hunt_action.m_hWnd,GUI_HUNT_CLOSE_ACTION_LABEL);
	m_info_HUNT_state.SetBitmap(LAMP_TRUE);}

	
}

void CMy12Dlg::OnBnClickedVmIntercept()
{
	CVMDlg VM_ops;
//	CVM_DLG_NewWave VM_ops;
	VM_ops.DoModal();
}


//void CMy12Dlg::OnBnClickedImportIntrecept()
//{
//
//}


void CAboutDlg::OnOK()
{
	// TODO: добавьте специализированный код или вызов базового класса

	CDialogEx::OnOK();
}


void CMy12Dlg::OnBnClickedUpdateList()
{
	// TODO: добавьте свой код обработчика уведомлений
	Update_Process_List();
}

bool CMy12Dlg::Update_Process_List(void)
{
size_t i;
	
		::SendMessage(m_select_target.m_hWnd, CB_RESETCONTENT, 0, 0);

		i = DH_Main_Count =  ::DH_GetProcess(&EnumProcBuffer);
		if(DH_GetProcess == 0)
		{
			return false;
		}
		My_DH_Hive_pointer = (pDH_ProcInfo) EnumProcBuffer;
			do
			{
				::SendMessage(m_select_target.m_hWnd, CB_ADDSTRING, 0, (LPARAM)&My_DH_Hive_pointer->ProcessName[0]); //m_select_target.AddString(&My_DH_Hive_pointer->ProcessName[0]);
				My_DH_Hive_pointer++;
			}
			while(i--);
	
return true;
}

void CMy12Dlg::OnBnClickedMapIntrecept()
{
	// TODO: добавьте свой код обработчика уведомлений
	CMapCompareDlg dlg;
	dlg.DoModal();
}


void CMy12Dlg::OnBnClickedPatchInspector()
{
	// TODO: добавьте свой код обработчика уведомлений
	CPatchDlg path_dlg;
	path_dlg.DoModal();
}


void CMy12Dlg::OnBnClickedLicence()
{
	// TODO: добавьте свой код обработчика уведомлений
	CLicDlg lic_dlg;
	lic_dlg.DoModal();
}


void CMy12Dlg::OnBnClickedButtonLicenceRawDump()
{
	// TODO: добавьте свой код обработчика уведомлений
		CLicDlg lic_dlg;
	lic_dlg.DoModal();
}


void CMy12Dlg::OnBnClickedVmvmp2Tracer()
{
	// TODO: добавьте свой код обработчика уведомлений
	CVMTracevmp2Dlg vmtrace2_dlg;
	vmtrace2_dlg.DoModal();
}

bool CMy12Dlg::GUI_APPROACH_COMMAND_LINE(TCHAR* procname)
{
	//wcscpy(procname, _T("Unravel_dump_SCY.exe"));
	//return true;

	register LPCTSTR cmdline = ::GetCommandLineW();
	//cmdline = _T("//Unravel_dump_SCY.exe");

	register unsigned long i_len = wcslen(cmdline);

	if (i_len > MAX_PATH)
		return false;
	do
	{
		if(*cmdline++ == (WCHAR)*_T("/"))
		{
			wcscpy(procname,cmdline);
			return Get_Selected_Target(procname, &iPointers);
		}
	}while(i_len--);
return false;
}

bool CMy12Dlg::START_HUNT_Ops(TCHAR* procname)
{
	//DH открываем процесс
		OPT_LOCK_Target = DH_OpenDENUVO(procname,My_DH_Hive_pointer);
				if (!OPT_LOCK_Target)
				{return false;}

			//DH проверяем связЪ
	//	if (DH_SPIRUN_Injectx64(OPT_LOCK_Target))
		//		{}
				
				//DH иниализация hunt профиля
						if (DH_Is_HAVE_hunt_Profile(My_DH_Hive_pointer,Profle_Folder) == true)
						{/*DH_SetCurrentHuntProfile(My_DH_Hive_pointer, Profle_Folder);*/} //уже устанорвлен путь профиля от OpenDenuvo
						else
						{ if(DH_CreateNewHuntProfile(My_DH_Hive_pointer, Profle_Folder) == false)
							{DH_SetCurrentHuntProfile(My_DH_Hive_pointer, Profle_Folder);}
						}
		
	
	/*
	if(Get_Selected_Target(&buffer[0],&iPointers))
	{
		OPT_LOCK_Target = DH_OpenDENUVO(&buffer[0],My_DH_Hive_pointer);
		if (OPT_LOCK_Target)
		{ 
			if (!DH_SPIRUN_Injectx64(OPT_LOCK_Target, My_DH_Inject_pool_pointer))
			{

			}

				//иниализация hunt профиля
				if (DH_Is_HAVE_hunt_Profile(My_DH_Hive_pointer,Profle_Folder) == true)
				{DH_SetCurrentHuntProfile(My_DH_Hive_pointer, Profle_Folder);}
				else
				{ if(DH_CreateNewHuntProfile(My_DH_Hive_pointer, Profle_Folder) == false)
					{DH_SetCurrentHuntProfile(My_DH_Hive_pointer, Profle_Folder);}
				}
		}
	}
	*/
return true;
}

void __forceinline CMy12Dlg::GUI_SHOW_PROCESS_INFO()
{
		//ProcessID
			itoa_16_fast(My_DH_Hive_pointer->ProcessID,&digital_buffer[DIG_BUFFER_0X_SHR]);
			::SetWindowText(m_info_PID.m_hWnd,&digital_buffer[0]);

			//Parent ProcessID
			itoa_16_fast(My_DH_Hive_pointer->ParentProcessID,&digital_buffer[DIG_BUFFER_0X_SHR]);
			::SetWindowText(m_info_parent_id.m_hWnd,&digital_buffer[0]);

			//EntryPoint
			itoa_16_fast((UINT64)My_DH_Hive_pointer->Ext_Module_Information.EntryPoint,&digital_buffer[DIG_BUFFER_0X_SHR]);
			::SetWindowText(m_info_EP.m_hWnd,&digital_buffer[0]);

			//HMODULE
			itoa_16_fast((UINT64)My_DH_Hive_pointer->Main_HMODULE,&digital_buffer[DIG_BUFFER_0X_SHR]);
			::SetWindowText(m_info_HMODULE.m_hWnd,&digital_buffer[0]);

			//IMAGE SIZE
			itoa_16_fast((unsigned long)My_DH_Hive_pointer->Ext_Module_Information.SizeOfImage,&digital_buffer[DIG_BUFFER_0X_SHR]);
			::SetWindowText(m_info_IMG_SIZE.m_hWnd,&digital_buffer[0]);

			//Module count
			itoa_16_fast((unsigned long)My_DH_Hive_pointer->ModuleCount,&digital_buffer[DIG_BUFFER_0X_SHR]);
			::SetWindowText(m_info_module_count.m_hWnd,&digital_buffer[0]);

			//Threads count
			itoa_16_fast((unsigned long)My_DH_Hive_pointer->ThreadCount,&digital_buffer[DIG_BUFFER_0X_SHR]);
			::SetWindowText(m_info_threads_count.m_hWnd,&digital_buffer[0]);

			//Threads count
			itoa_16_fast((unsigned long)My_DH_Hive_pointer->HandleCount,&digital_buffer[DIG_BUFFER_0X_SHR]);
			::SetWindowText(m_info_handle_count.m_hWnd,&digital_buffer[0]);

			//Main thread ID
			itoa_16_fast(My_DH_Hive_pointer->Thread0.UniqueThread,&digital_buffer[DIG_BUFFER_0X_SHR]);
			::SetWindowText(m_info_MAIN_THREAD_ID.m_hWnd,&digital_buffer[0]);

			//PEB
			itoa_16_fast((UINT64)My_DH_Hive_pointer->PEB_BaseAddress,&digital_buffer[DIG_BUFFER_0X_SHR]);
			::SetWindowText(m_info_PEB.m_hWnd,&digital_buffer[0]);

			//PE.Section count
			itoa_16_fast((UINT64)My_DH_Hive_pointer->eXT_IMAGE_Section_Count,&digital_buffer[DIG_BUFFER_0X_SHR]);
			::SetWindowText(m_info_PE_Num_of_Sections.m_hWnd,&digital_buffer[0]);

			//HMODULE kernel32
			itoa_16_fast((UINT64)My_DH_Hive_pointer->Kernel32,&digital_buffer[DIG_BUFFER_0X_SHR]);
			::SetWindowText(m_info_HMODULE_kernel32.m_hWnd,&digital_buffer[0]);

			//HMODULE ntdll
			itoa_16_fast((UINT64)My_DH_Hive_pointer->Ntdll,&digital_buffer[DIG_BUFFER_0X_SHR]);
			::SetWindowText(m_info_HMODULE_ntdll.m_hWnd,&digital_buffer[0]);

			//HMODULE kernelbase
			itoa_16_fast((UINT64)My_DH_Hive_pointer->Kernelbase,&digital_buffer[DIG_BUFFER_0X_SHR]);
			::SetWindowText(m_info_HMODULE_kernelbase.m_hWnd,&digital_buffer[0]);

			//EP Section name
			::SetWindowText(m_info_EP_SectionName.m_hWnd,My_DH_Hive_pointer->eXT_EP_Section_Name);

			//Path
			::SetWindowText(m_info_exe_path.m_hWnd,&My_DH_Hive_pointer->Process_FilePath[0]);

			if(!My_DH_Hive_pointer->AA_Probe.AA_STATE) {m_info_aa_lamp.SetBitmap(LAMP_FALSE); ::EnableWindow(m_MORE_info_aa.m_hWnd,false);}
			else {m_info_aa_lamp.SetBitmap(LAMP_TRUE);::EnableWindow(m_MORE_info_aa.m_hWnd,true);}

			if((My_DH_Hive_pointer->Digital_distribution_client_present.is_steamclient64_present) || ((My_DH_Hive_pointer->Digital_distribution_client_present.is_steam_api64_present))) {::ShowWindow(m_info_steam_LAMP.m_hWnd,true);} else
				 {::ShowWindow(m_info_steam_LAMP.m_hWnd,false);}
			if((My_DH_Hive_pointer->Digital_distribution_client_present.is_Core_ActivationUI) ||
				
				((My_DH_Hive_pointer->Digital_distribution_client_present.is_core_activation))) {::ShowWindow(m_info_origin_LAMP.m_hWnd,true);} else
				 {::ShowWindow(m_info_origin_LAMP.m_hWnd,false);}
}

void CMy12Dlg::OnBnClickedButtonReopen()
{
	// TODO: добавьте свой код обработчика уведомлений
	TCHAR temp_buff[MAX_PATH];
	::GetWindowText(m_select_target.m_hWnd,&temp_buff[0],MAX_PATH);
	DH_CloseDENUVO(OPT_LOCK_Target);
	Update_Process_List();

		if(!Get_Selected_Target(&temp_buff[0],&iPointers))
			{return;}
			
			if(START_HUNT_Ops(&temp_buff[0]) == true)
				GUI_SHOW_PROCESS_INFO();
	//работа с GUI
	GUI_Set_Enable_Windows_when_open_close_target(HUNT_HIVE_ACTIVE);
}


void CMy12Dlg::OnBnClickedButtonLicenceDlcFiles()
{
	// TODO: добавьте свой код обработчика уведомлений
	СDLC_Lic_Files lic_dlc_dlg;
	lic_dlc_dlg.DoModal();
}


void CMy12Dlg::OnChangeEditProfileVmpxContextVmp2()
{
	WCHAR WindowText[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN];
	
	if (::GetWindowText(m_EDIT_PROFILE_VMPX_CONTEXT_vmp2.m_hWnd, &WindowText[0], PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN) == NULL)
		return;

	if (!DH__Install_HUNT_Profile(&WindowText[0], DH_INSTALL_PROFILE__VMPX_Context, VM_TYPE_VMP2))
		m_LAMP_context_vmp2_state.SetBitmap(LAMP_FALSE);
	else
		m_LAMP_context_vmp2_state.SetBitmap(LAMP_TRUE);
}


void CMy12Dlg::OnChangeEditProfileVmpxContextVmp3new()
{
	WCHAR WindowText[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN];
	
	if (::GetWindowText(m_EDIT_PROFILE_VMPX_CONTEXT_vmp3new.m_hWnd, &WindowText[0], PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN) == NULL)
		return;

	if (!DH__Install_HUNT_Profile(&WindowText[0], DH_INSTALL_PROFILE__VMPX_Context, VM_TYPE_VMP3))
		m_LAMP_context_vmp3new_state.SetBitmap(LAMP_FALSE);
	else
		m_LAMP_context_vmp3new_state.SetBitmap(LAMP_TRUE);
}


void CMy12Dlg::OnChangeEditProfileVmpxContextD4()
{
		WCHAR WindowText[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN];
	
	if (::GetWindowText(m_EDIT_PROFILE_VMPX_CONTEXT_D4.m_hWnd, &WindowText[0], PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN) == NULL)
		return;

	if (!DH__Install_HUNT_Profile(&WindowText[0], DH_INSTALL_PROFILE__VMPX_Context, VM_TYPE_VMX_D4))
		m_LAMP_context_D4_state.SetBitmap(LAMP_FALSE);
	else
		m_LAMP_context_D4_state.SetBitmap(LAMP_TRUE);
}
