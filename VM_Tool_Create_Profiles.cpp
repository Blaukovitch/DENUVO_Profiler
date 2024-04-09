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
#include "VM_Tool_Create_Profiles.h"
#include "afxdialogex.h"

// диалоговое окно CVM_Tool_Create_Profiles

IMPLEMENT_DYNAMIC(CVM_Tool_Create_Profiles, CDialogEx)

CVM_Tool_Create_Profiles::CVM_Tool_Create_Profiles(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVM_Tool_Create_Profiles::IDD, pParent)
{
	pDH_ProcInfo pTargetProcessBaseInfo;
	hGUI_UpdateProgressState = NULL;
	hGUI_MultiScan_work_thread = NULL;
	GUI_SCAN_MODE = false; //stand-by for SCAN
	pTargetProcessBaseInfo = DH_Get_Target_Process_information();
	hTarget_Main_module = pTargetProcessBaseInfo->Main_HMODULE;
	CurrentFoundSeq_stack = NULL;
	Save_PIRATEMAPs.FileList_count = NULL;
	CurrentThreadWork_stack = NULL;
	fpPMAP_Main_info_stack = &Save_PIRATEMAPs.PMAP_Main_info_stack[0];
}
CVM_Tool_Create_Profiles::~CVM_Tool_Create_Profiles()
{
	codedef_check_term_threadif(hGUI_UpdateProgressState);
	codedef_check_term_threadif(hGUI_MultiScan_work_thread);

		if (CurrentFoundSeq_stack != NULL)
			DH_VM_Seq_ClearSet(&pFoundSeqStack_array[0], &CurrentFoundSeq_stack);

		if (CurrentThreadWork_stack != NULL)
			DH_VM_ThreadWork_ClearSet(&pActiveThreadWork[0], &CurrentThreadWork_stack); //end if (CurrentThreadWork_stack != NULL)

		GUI_SCAN_MODE=false;
		CurrentFoundSeq_stack = NULL;
}

void CVM_Tool_Create_Profiles::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_VM_NAME_EDIT, m_VM_name);
	DDX_Control(pDX, IDC_VM_handles, m_EDIT_VM_handles_count);
	DDX_Control(pDX, IDC_VM_NAME_EDIT, m_EDIT_VM_Name);
	DDX_Control(pDX, IDC_VM_COMMENTS_EDIT, m_EDIT_VM_commentaries);
	DDX_Control(pDX, IDC_ACCEPT_BUTTON, m_BUTTON_Accept);
	DDX_Control(pDX, IDC_LAMP_ADDRES_CHECK_STATE, m_LAMP_Sypply15_addres_check);
	DDX_Control(pDX, IDC_VM_SUPPLY_DECODE_COMMAND, m_COMBO_decode_instruction);
	DDX_Control(pDX, IDC_VM_Supply15_start_addres_EDIT, m_EDIT_Supply15_start_addres);
	DDX_Control(pDX, IDC_VM_VER_another_RADIO3, m_RADIO_ver_another);
	DDX_Control(pDX, IDC_VM_VER_VMP3_RADIO2,  m_RADIO_ver_VMP3);
	DDX_Control(pDX, IDC_VM_VER_VMP2_RADIO,  m_RADIO_ver_VMP2);
	DDX_Control(pDX, IDC_LAMP_FILE_NAME_CHECK_STATE2, m_LAMP_VM_FILE_NAME);
	DDX_Control(pDX, IDC_VM_Supply15_start_addres_SECTION_NAME, m_EDIT_LOCK_supply15_sectionname);
	DDX_Control(pDX, IDC_VM_Supply15_DECODE_operand, m_EDIT_Supply15_DEcode_OPERAND);
	DDX_Control(pDX, IDC_LAMP_ADDRES_CHECK_STATE2, m_LAMP_VM_ENTER_addres_state);
	DDX_Control(pDX, IDC_EDIT_vm_enter_addres, m_EDIT_vm_enter_addres);
	DDX_Control(pDX, IDC_EDIT_vm_enter_addres_SECTION_NAME, m_EDIT_vm_enter_addres_SECTION_NAME);
	DDX_Control(pDX, IDC_EDIT_asm_mnemonic, m_EDIT_dissasm_mnemonic_text);
	DDX_Control(pDX, IDC_LIST_SECTIONS, m_LIST_PE_Sections_Scan);
	DDX_Control(pDX, IDC_CHECK_MULTI_SCAN, m_CHECK_EnableMultiScan);
	DDX_Control(pDX, IDC_EDIT_SECTIONS_SEL_COUNT, m_EDIT_Sel_SECTIONS_COUNT);
	DDX_Control(pDX, IDC_EDIT_SECTIONS_SEL_COUNT2, m_EDIT_Sel_EQ_BYTES_COUNT);
	DDX_Control(pDX, IDC_BUTTON_SCAN_MULTI, m_BUTTON_MULTI_SCAN);
	DDX_Control(pDX, IDC_PROGRESS1, m_PROGRESS_MULTI);
	DDX_Control(pDX, IDC_BUTTON_SINGLE_MULTI_PANEL_SHOW, m_BUTTON_RIGHT_PANEL);
	DDX_Control(pDX, IDC_EDIT_VMP_CONTEXT_FOUND_COUNT, m_EDIT_FOUND_VM_ENTERs);
	//  DDX_Control(pDX, IDC_EDIT_VMP_CONTEXT_FOUND_COUNT2, m_EDIT_FOUND_VM_CONTEXTs);
	//  DDX_Control(pDX, IDC_EDIT_VMP_CONTEXT_FOUND_COUNT2, m_FOUND_VM_HANHELDs);
	DDX_Control(pDX, IDC_EDIT_VMP_CONTEXT_FOUND_COUNT2, m_EDIT_FOUND_VM_HANHELDs);
	DDX_Control(pDX, IDC_EDIT_VMP_CONTEXT_FOUND_COUNT3, m_EDIT_FOUND_VM_CONTEXTs);
	DDX_Control(pDX, IDC_ACCEPT_BUTTON2, m_BUTTON_Accept_multi);
	DDX_Control(pDX, IDC_EDIT_VMP_CONTEXT_FOUND_COUNT4, m_EDIT_RESEARCHED_VM_CONTEXTs);
	DDX_Control(pDX, IDC_EDIT_VMP_CONTEXT_FOUND_COUNT5, m_EDIT_IDENTIFICATED_VM_CONTEXTs_percent);
	DDX_Control(pDX, IDC_EDIT_CREATED_VM_PROFILES_COUNT, m_EDIT_WILL_BE_CREATE_VM_CONTEXTs);
	DDX_Control(pDX, IDC_EDIT_CREATED_BILLET_VM_PROFILES_COUNT, m_EDIT_WILL_BE_CREATE_BILLETSs);
	DDX_Control(pDX, IDC_EDIT_CREATED_PIRATE_MAPS, m_EDIT_WILL_BE_CREATE_PIRATEMAPs);
	DDX_Control(pDX, IDC_BUTTON_SELECT_ALL_SECTION, m_BUTTON_Sel_all_PE_Sections);
	DDX_Control(pDX, IDC_EDIT_CREATE_MANUAL_Last_ERROR, m_EDIT_CREATE_MANUAL_Last_ERROR);
	DDX_Control(pDX, IDC_VM_STACK_RESERVED_EDIT, m_EDIT_VMPX_stack_reserved_bytes);
		
}


BEGIN_MESSAGE_MAP(CVM_Tool_Create_Profiles, CDialogEx)
	ON_EN_CHANGE(IDC_VM_NAME_EDIT, &CVM_Tool_Create_Profiles::OnEnChangeVmNameEdit)
	ON_EN_CHANGE(IDC_VM_Supply15_start_addres_EDIT, &CVM_Tool_Create_Profiles::OnEnChangeVmSupply15startaddresEdit)
	ON_BN_CLICKED(IDC_VM_VER_VMP2_RADIO, &CVM_Tool_Create_Profiles::OnBnClickedVmVerVmp2Radio)
	ON_BN_CLICKED(IDC_VM_VER_VMP3_RADIO2, &CVM_Tool_Create_Profiles::OnBnClickedVmVerVmp3Radio2)
	ON_BN_CLICKED(IDC_VM_VER_another_RADIO3, &CVM_Tool_Create_Profiles::OnBnClickedVmVeranotherRadio3)
	ON_BN_CLICKED(IDC_ACCEPT_BUTTON, &CVM_Tool_Create_Profiles::OnBnClickedAcceptButton)
	ON_CBN_SELCHANGE(IDC_VM_SUPPLY_DECODE_COMMAND, &CVM_Tool_Create_Profiles::OnCbnSelchangeVmSupplyDecodeCommand)
	ON_CBN_EDITCHANGE(IDC_VM_SUPPLY_DECODE_COMMAND, &CVM_Tool_Create_Profiles::OnEditchangeVmSupplyDecodeCommand)
//	ON_EN_CHANGE(IDC_VM_Supply15_DECODE_operand, &CVM_Tool_Create_Profiles::OnEnChangeVmSupply15Decodeoperand)
ON_EN_CHANGE(IDC_EDIT_vm_enter_addres, &CVM_Tool_Create_Profiles::OnEnChangeEditvmenteraddres)
ON_BN_CLICKED(IDC_CHECK_MULTI_SCAN, &CVM_Tool_Create_Profiles::OnBnClickedCheckMultiScan)
//ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_CHECK_MULTI_SCAN, &CVM_Tool_Create_Profiles::OnHotitemchangeCheckMultiScan)
ON_BN_SETFOCUS(IDC_CHECK_MULTI_SCAN, &CVM_Tool_Create_Profiles::OnSetfocusCheckMultiScan)
ON_BN_CLICKED(IDC_BUTTON_SCAN_MULTI, &CVM_Tool_Create_Profiles::OnBnClickedButtonScanMulti)
ON_LBN_SELCHANGE(IDC_LIST_SECTIONS, &CVM_Tool_Create_Profiles::OnSelchangeListSections)
ON_BN_CLICKED(IDC_BUTTON_SINGLE_MULTI_PANEL_SHOW, &CVM_Tool_Create_Profiles::OnBnClickedButtonSingleMultiPanelShow)
ON_BN_CLICKED(IDC_ACCEPT_BUTTON2, &CVM_Tool_Create_Profiles::OnBnClickedAcceptButton2)
ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL_SECTION, &CVM_Tool_Create_Profiles::OnBnClickedButtonSelectAllSection)
ON_BN_CLICKED(IDC_VM_VER_D4_RADIO, &CVM_Tool_Create_Profiles::OnBnClickedVmVerD4Radio)
END_MESSAGE_MAP()


// обработчики сообщений CVM_Tool_Create_Profiles


void CVM_Tool_Create_Profiles::OnOK()
{
	// TODO: добавьте специализированный код или вызов базового класса

	//CDialogEx::OnOK();
}


BOOL CVM_Tool_Create_Profiles::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Добавить дополнительную инициализацию
	//set limit text
	m_EDIT_Supply15_start_addres.SetLimitText(G_ADDRES_MAX_LEN);
	m_EDIT_VM_Name.SetLimitText(G_FILE_NAME_VM_PROFILE_MAX_LEN);
	m_EDIT_Supply15_DEcode_OPERAND.SetLimitText(G_ADDRES_MAX_LEN);
	m_EDIT_VM_handles_count.SetLimitText(G_HANDLES_COUNT);
	m_EDIT_VM_commentaries.SetLimitText(G_VM_COMMENTERIES_MAX_LEN);



	//set first radio
	m_RADIO_ver_VMP2.SetCheck(BST_CHECKED);



	//set gui interface control bools
	GUI_LAMP_FILE_NAME_CORRECT = false;
	GUI_LAMP_Supply15_ADDRES_CORRECT = false;
	GUI_LAMP_VM_ENTER_CORRECT = false;
	GUI_VMP2_exten = true;
	GUI_VMP3_exten = false;
	GUI_VMP_another = false;
	GUI_VM_D4_exten = false;
	GUI_VM_D5_exten = false;
	GUI_RIGHT_PANEL_state = false;
	SelectedVMPX_version = VM_TYPE_VMP2;

		//swt
	::SetWindowText(m_EDIT_VM_handles_count.m_hWnd, L"256");
	::SetWindowText(m_EDIT_Supply15_start_addres.m_hWnd, L"0x1415ECDED"); //клубок
	::SetWindowText(m_EDIT_VMPX_stack_reserved_bytes.m_hWnd, L"0x140");
	//::SetWindowTextW(m_EDIT_Supply15_start_addres.m_hWnd, L"0x1487b8be6"); //резидентев7

	m_PROGRESS_MULTI.SetRange(LOWER_PROGRESS_RANGE,UPPER_PROGRESS_RANGE);
	
	/*#define S__SERVICE_NODECODE 0
	#define S__INC 1
	#define S__DEC 2
	#define S__XOR 3
	#define S__NOT 4
	#define S__NEG 5
	#define S__ADD 5
	#define S__SUB 6
	#define S__BSWAP 7
	static WCHAR TS__INC[] = L"INC";
static WCHAR TS__DEC[] = L"DEC";
static WCHAR TS__XOR[] = L"XOR";
static WCHAR TS__NOT[] = L"NOT";
static WCHAR TS__NEG[] = L"NEG";
static WCHAR TS__ADD[] = L"ADD";
static WCHAR TS__SUB[] = L"SUB";
static WCHAR TS__BSWAP[] = L"BSWAP";
	*/
	
	//::SendMessage(m_COMBO_decode_instruction.m_hWnd, CB_ADDSTRING, 0, (LPARAM)&TS__INC);
	A_COMBO_DECODE_INSTRUCTION_ADD(TS__INC);
	A_COMBO_DECODE_INSTRUCTION_ADD(TS__DEC);
	A_COMBO_DECODE_INSTRUCTION_ADD(TS__XOR);
	A_COMBO_DECODE_INSTRUCTION_ADD(TS__NOT);
	A_COMBO_DECODE_INSTRUCTION_ADD(TS__NEG);
	A_COMBO_DECODE_INSTRUCTION_ADD(TS__ADD);
	A_COMBO_DECODE_INSTRUCTION_ADD(TS__SUB);
	A_COMBO_DECODE_INSTRUCTION_ADD(TS__BSWAP);
	A_COMBO_DECODE_INSTRUCTION_ADD(TS__ROL);
	A_COMBO_DECODE_INSTRUCTION_ADD(TS__ROR);

	VMPX_3 = DH_VM_Do_INIT_Arbitary_VMPX_context(L"3x.ini", VM_TYPE_VMP3);
	if(VMPX_3 == NULL)
		::EnableWindow(m_CHECK_EnableMultiScan.m_hWnd, false);
	else
		::EnableWindow(m_CHECK_EnableMultiScan.m_hWnd, true);

	VMPX_2 = DH_VM_Do_INIT_Arbitary_VMPX_context(L"2x.ini", VM_TYPE_VMP2);

	VMX_D4 = DH_VM_Do_INIT_Arbitary_VMPX_context(L"4x.ini", VM_TYPE_VMX_D4);
	
	Fill_PE_Sections_list();
	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}


void CVM_Tool_Create_Profiles::OnEnChangeVmNameEdit()
{
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO:  Добавьте код элемента управления
	::GetWindowTextW(m_EDIT_VM_Name.m_hWnd,&buffer[0],G_ADDRES_MAX_LEN);
	UINT a = wcslen(&buffer[0]);
	if (a == NULL)
	{GUI_LAMP_FILE_NAME_CORRECT = false; GUI_UPDATE_NOW(); return;}

	if (DH_Test_Current_hunt_profile_CONTAINER(PROFILE_TYPE_VM,&buffer[0]) == true)
		{GUI_LAMP_FILE_NAME_CORRECT = false;}
	else
		{GUI_LAMP_FILE_NAME_CORRECT = true;}
	GUI_UPDATE_NOW();
}

void CVM_Tool_Create_Profiles::GUI_UPDATE_NOW(void)
{
	//m_LAMP_VM_FILE_NAME
	if(GUI_LAMP_FILE_NAME_CORRECT == false)
		{m_LAMP_VM_FILE_NAME.SetBitmap(LAMP_FALSE);}
		else
		{m_LAMP_VM_FILE_NAME.SetBitmap(LAMP_TRUE);}

	//m_LAMP_Sypply15_addres_check
	if(GUI_LAMP_Supply15_ADDRES_CORRECT == false)
		{m_LAMP_Sypply15_addres_check.SetBitmap(LAMP_FALSE);}
		else
		{m_LAMP_Sypply15_addres_check.SetBitmap(LAMP_TRUE);}

	if(GUI_LAMP_VM_ENTER_CORRECT == false)
		{m_LAMP_VM_ENTER_addres_state.SetBitmap(LAMP_FALSE);}
		else
		{m_LAMP_VM_ENTER_addres_state.SetBitmap(LAMP_TRUE);}

	//GUI_VMP2_exten flag - enable/disable supply15 extended
	::EnableWindow(m_EDIT_Supply15_start_addres.m_hWnd,GUI_VMP2_exten);
	::EnableWindow(m_EDIT_VM_handles_count.m_hWnd,GUI_VMP2_exten);
	::EnableWindow(m_COMBO_decode_instruction.m_hWnd,GUI_VMP2_exten);
	::EnableWindow(m_EDIT_LOCK_supply15_sectionname.m_hWnd,GUI_VMP2_exten);
	::EnableWindow(m_EDIT_Supply15_DEcode_OPERAND.m_hWnd,GUI_VMP2_exten);
	::EnableWindow(m_EDIT_vm_enter_addres.m_hWnd,GUI_VMP3_exten);
		
	//m_BUTTON_Accept
	//enable = GUI_LAMP_FILE_NAME_CORRECT - GUI_VMP2_exten - GUI_LAMP_Supply15_ADDRES_CORRECT 
	//1 =  1 1 1
	//1 =  1 0 0
	//1 =  1 0 1
	//bool baa = GUI_VMP2_exten & GUI_LAMP_Supply15_ADDRES_CORRECT;
	//bool bax = GUI_LAMP_Supply15_ADDRES_CORRECT | GUI_VMP2_exten;
	
	if(GUI_LAMP_FILE_NAME_CORRECT == true && (GUI_LAMP_Supply15_ADDRES_CORRECT == GUI_VMP2_exten || (GUI_VMP2_exten == false && GUI_LAMP_Supply15_ADDRES_CORRECT == true)))
		{::EnableWindow(m_BUTTON_Accept.m_hWnd, true);}
		else
		{::EnableWindow(m_BUTTON_Accept.m_hWnd, false);}

	if(GUI_VMP3_exten == true)
	{
		if(GUI_LAMP_FILE_NAME_CORRECT == true && GUI_LAMP_VM_ENTER_CORRECT == true)
			{::EnableWindow(m_BUTTON_Accept.m_hWnd, true);}
			else
			{::EnableWindow(m_BUTTON_Accept.m_hWnd, false);}
	}

UpdateWindow();
}

void CVM_Tool_Create_Profiles::OnEnChangeVmSupply15startaddresEdit()
{
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Добавьте код элемента управления
	register size_t f = ::GetWindowTextW(m_EDIT_Supply15_start_addres.m_hWnd,&buffer[0],G_ADDRES_STRING_UNICODE_MAX_LEN);
	if (f == NULL)
		{GUI_LAMP_Supply15_ADDRES_CORRECT = false; GUI_UPDATE_NOW(); return;}

	xaddres = _wcstoi64(&buffer[0],(wchar_t**)&buffer[f],16);
	GUI_LAMP_Supply15_ADDRES_CORRECT = DH_VerifyProcessAddres(HUNT_HIVE.tOpenProcHANDLE,(LPVOID)xaddres);
	if (GUI_LAMP_Supply15_ADDRES_CORRECT)
		{
			size_t bf = DH_get_quick_module_address64_accessory(HUNT_HIVE.tOpenProcHANDLE, HUNT_HIVE.Process_HIVE.Main_HMODULE,xaddres,&buffer[0]);
			if(bf != NULL)
			{::SetWindowTextW(m_EDIT_LOCK_supply15_sectionname.m_hWnd,&buffer[0]);}
			else
			{::SetWindowTextW(m_EDIT_LOCK_supply15_sectionname.m_hWnd,L"?");}
		}
		else
		{
			::SetWindowTextW(m_EDIT_LOCK_supply15_sectionname.m_hWnd,L"???");
		}

	GUI_UPDATE_NOW();
}


void CVM_Tool_Create_Profiles::OnBnClickedVmVerVmp2Radio()
{
	// TODO: добавьте свой код обработчика уведомлений
	GUI_VMP2_exten = true;
	GUI_VMP3_exten = false;
	GUI_VMP_another = false;
	SelectedVMPX_version = VM_TYPE_VMP2;
	GUI_UPDATE_NOW();
}


void CVM_Tool_Create_Profiles::OnBnClickedVmVerVmp3Radio2()
{
	// TODO: добавьте свой код обработчика уведомлений
	GUI_VMP2_exten = false;
	GUI_VMP3_exten = true;
	GUI_VMP_another = false;
	SelectedVMPX_version = VM_TYPE_VMP3;
	GUI_UPDATE_NOW();
}


void CVM_Tool_Create_Profiles::OnBnClickedVmVeranotherRadio3()
{
	// TODO: добавьте свой код обработчика уведомлений
	GUI_VMP2_exten = false;
	GUI_VMP3_exten = false;
	GUI_VMP_another = true;
	SelectedVMPX_version = VM_TYPE_ANOTHER;
	GUI_UPDATE_NOW();
}


void CVM_Tool_Create_Profiles::OnBnClickedAcceptButton()
{
	// TODO: добавьте свой код обработчика уведомлений
	VM_Manual_input Collect_manual_information;



	::GetWindowTextW(m_EDIT_VM_Name.m_hWnd, &Collect_manual_information.VM_name[0], PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN); //name
	//::GetWindowTextW(m_EDIT_VM_commentaries.m_hWnd, &SAVE_InfoStack->comment[0],sizeof(SAVE_InfoStack->comment));//КОММЕНТАРИИ
//	memcpy(&SAVE_AsmAddrPoolStack->name[0], &SAVE_InfoStack->name[0], sizeof(SAVE_InfoStack->name));
	
	size_t a = ::GetWindowTextW(m_EDIT_VM_handles_count.m_hWnd, &buffer[0],G_HANDLES_COUNT); //handles count (handles count ->>> 	if(GUI_VMP2_exten == true)//VM TYPE)
			if(a==NULL)
			{
				Collect_manual_information.handles_count = NULL;}
			else
			{
				Collect_manual_information.handles_count = wcstoul(&buffer[0],NULL,10);}

	Collect_manual_information.supply_15_start_address = (PVOID)xaddres; //r12_supply_15_start_address

	Collect_manual_information.table_DEcode_instruction = xDECODE_INSRUCTION; //decode instruction

	size_t z = ::GetWindowTextW(m_EDIT_Supply15_DEcode_OPERAND.m_hWnd,&buffer[0],G_ADDRES_MAX_LEN); //decode operand
		if (z>0)
		{
			Collect_manual_information.table_DEcode_instruction_operand = _wcstoui64(&buffer[0],NULL ,16);}
		else
		{
			Collect_manual_information.table_DEcode_instruction_operand = NULL;}
	

	if(GUI_VMP2_exten == true)//VM TYPE
	{
		Collect_manual_information.VMPX_version = VM_TYPE_VMP2;
	}
	else
	{ 
		if(GUI_VMP3_exten == true)
		{
			Collect_manual_information.VMPX_version = VM_TYPE_VMP3;}
		else
		{
			Collect_manual_information.VMPX_version = VM_TYPE_ANOTHER;
		}
	}

	a = ::GetWindowTextW(m_EDIT_VMPX_stack_reserved_bytes.m_hWnd, &buffer[0], G_VM_DECODE_INSTUCTION_NAME_MAX_LEN);
	if (a != NULL)
		Collect_manual_information.Stack_RSP_Different = _wcstoui64(&buffer[0], NULL, 16);
	else
		Collect_manual_information.Stack_RSP_Different = 0; //set NO_STACK disbalance (default)


	register SERVICE_ERROR err_create_profile_manual = DH_VMPX_Create_VM_Profile_MANUAL(&Collect_manual_information);
	if (err_create_profile_manual != NULL)
	{
		::SetWindowText(m_EDIT_CREATE_MANUAL_Last_ERROR.m_hWnd, DH_VM_ERROR_type_to_string(err_create_profile_manual));
		return;
	}

	EndDialog(true);
}



void CVM_Tool_Create_Profiles::OnCbnSelchangeVmSupplyDecodeCommand()
{
	// TODO: добавьте свой код обработчика уведомлений
	
	UINT f = m_COMBO_decode_instruction.GetCurSel(); //get cur sel
	if(f == -1)
		return;

	
	UINT g = m_COMBO_decode_instruction.GetLBText(f,&buffer[0]); //(f,,G_VM_DECODE_INSTUCTION_NAME_MAX_LEN);
		if(g == NULL)
		return;
		GUI_and_OPS_upd_decode_instruction_sets(&buffer[0]);
	
}


void CVM_Tool_Create_Profiles::OnEditchangeVmSupplyDecodeCommand()
{
	// TODO: добавьте свой код обработчика уведомлений
	UINT f = ::GetWindowTextW(m_COMBO_decode_instruction.m_hWnd,&buffer[0],G_VM_DECODE_INSTUCTION_NAME_MAX_LEN);
		if(f == 0)
			return;

	GUI_and_OPS_upd_decode_instruction_sets(&buffer[0]);
}

void CVM_Tool_Create_Profiles::GUI_and_OPS_upd_decode_instruction_sets(WCHAR* buff)
{
	do
	{
		if (!_wcsicmp(buff,&TS__INC[0])) // S__INC
			{xDECODE_INSRUCTION = S__INC; ::EnableWindow(m_EDIT_Supply15_DEcode_OPERAND.m_hWnd, false); break;}

		if (!_wcsicmp(buff,&TS__DEC[0])) // S__DEC
			{xDECODE_INSRUCTION = S__DEC; ::EnableWindow(m_EDIT_Supply15_DEcode_OPERAND.m_hWnd, false); break;}

		if (!wcsicmp(buff,&TS__XOR[0])) // S__XOR
			{xDECODE_INSRUCTION = S__XOR; ::EnableWindow(m_EDIT_Supply15_DEcode_OPERAND.m_hWnd, true); break;}

		if (!_wcsicmp(buff,&TS__NOT[0])) // S__NOT
			{xDECODE_INSRUCTION = S__NOT; ::EnableWindow(m_EDIT_Supply15_DEcode_OPERAND.m_hWnd, false); break;}

		if (!_wcsicmp(buff,&TS__NEG[0])) // S__NEG
			{xDECODE_INSRUCTION = S__NEG; ::EnableWindow(m_EDIT_Supply15_DEcode_OPERAND.m_hWnd, false); break;}

		if (!_wcsicmp(buff,&TS__ADD[0])) // S__ADD
			{xDECODE_INSRUCTION = S__ADD; ::EnableWindow(m_EDIT_Supply15_DEcode_OPERAND.m_hWnd, true); break;}

		if (!_wcsicmp(buff,&TS__SUB[0])) // S__SUB
			{xDECODE_INSRUCTION = S__SUB; ::EnableWindow(m_EDIT_Supply15_DEcode_OPERAND.m_hWnd, true); break;}

		if (!_wcsicmp(buff,&TS__BSWAP[0])) //S__BSWAP
			{xDECODE_INSRUCTION = S__BSWAP; ::EnableWindow(m_EDIT_Supply15_DEcode_OPERAND.m_hWnd, false); break;}

		if (!_wcsicmp(buff,&TS__ROL[0])) //S__ROL
			{xDECODE_INSRUCTION = S__ROL; ::EnableWindow(m_EDIT_Supply15_DEcode_OPERAND.m_hWnd, true); break;}

		if (!_wcsicmp(buff,&TS__ROR[0])) //S__ROR
			{xDECODE_INSRUCTION = S__ROR; ::EnableWindow(m_EDIT_Supply15_DEcode_OPERAND.m_hWnd, true); break;}
		
		xDECODE_INSRUCTION = S__SERVICE_NODECODE;
		break;
	}while(0);
}



void CVM_Tool_Create_Profiles::OnEnChangeEditvmenteraddres()
{
	char OutMnemonicBuff[INSTRUCT_LENGTH];

	register size_t f = ::GetWindowTextW(m_EDIT_vm_enter_addres.m_hWnd,&buffer[0],G_ADDRES_STRING_UNICODE_MAX_LEN);
	if (f == NULL)
		return;

	xaddres = _wcstoi64(&buffer[0],NULL,16);
	GUI_LAMP_VM_ENTER_CORRECT = DH_VerifyProcessAddres(HUNT_HIVE.tOpenProcHANDLE,(LPVOID)xaddres);
	if (GUI_LAMP_VM_ENTER_CORRECT)
		{
			register size_t bf = DH_get_quick_module_address64_accessory(HUNT_HIVE.tOpenProcHANDLE, HUNT_HIVE.Process_HIVE.Main_HMODULE,xaddres,&buffer[0]);
			if(bf != NULL)
			{
				::SetWindowTextW(m_EDIT_vm_enter_addres_SECTION_NAME.m_hWnd,&buffer[0]);
				DH_VM_Dissasm_instruction(xaddres, &OutMnemonicBuff[0]);
				::SetWindowTextA(m_EDIT_dissasm_mnemonic_text.m_hWnd, &OutMnemonicBuff[0]);
			}
			else
			{::SetWindowTextW(m_EDIT_vm_enter_addres_SECTION_NAME.m_hWnd,L"?");}
		}
		else
		{
			::SetWindowTextW(m_EDIT_vm_enter_addres_SECTION_NAME.m_hWnd,L"???");
		}

	GUI_UPDATE_NOW();
}


void CVM_Tool_Create_Profiles::Fill_PE_Sections_list(void)
{
	PIMAGE_SECTION_HEADER pPESectionList;
	register SERVICE_COUNT NumofSections = DH_Get_Target_PE_Section(&pPESectionList);
		if(NumofSections == NULL)
			return;
	
		TCHAR UnicodeName[16];
		::SendMessage(m_LIST_PE_Sections_Scan.m_hWnd, LB_RESETCONTENT, 0, 0);
		do
		{
			quick_ASCII_to_UNICODE(&pPESectionList->Name[0],&UnicodeName[0]);
			m_LIST_PE_Sections_Scan.AddString(&UnicodeName[0]);
			pPESectionList++;
		}while(--NumofSections);
}

void CVM_Tool_Create_Profiles::OnBnClickedCheckMultiScan()
{
	// TODO: добавьте свой код обработчика уведомлений
	if(m_CHECK_EnableMultiScan.GetCheck() == BST_CHECKED)
	{
		::EnableWindow(m_LIST_PE_Sections_Scan.m_hWnd, true);
		::EnableWindow(m_BUTTON_Sel_all_PE_Sections.m_hWnd, true);
			if(m_LIST_PE_Sections_Scan.GetSelCount() != 0)
			{
			::EnableWindow(m_BUTTON_MULTI_SCAN.m_hWnd, true);
			::ShowWindow(m_PROGRESS_MULTI.m_hWnd, SW_SHOW);
			}
		}
	else
	{
	::EnableWindow(m_LIST_PE_Sections_Scan.m_hWnd, false);
	::EnableWindow(m_BUTTON_MULTI_SCAN.m_hWnd, false);
	::EnableWindow(m_BUTTON_Sel_all_PE_Sections.m_hWnd, false);
	::ShowWindow(m_PROGRESS_MULTI.m_hWnd, SW_HIDE);
	}
}


//void CVM_Tool_Create_Profiles::OnHotitemchangeCheckMultiScan(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// Для этого средства требуется Internet Explorer 6 или более поздняя версия.
//	// Символ _WIN32_IE должен быть >= 0x0600.
//	LPNMBCHOTITEM pHotItem = reinterpret_cast<LPNMBCHOTITEM>(pNMHDR);
//	// TODO: добавьте свой код обработчика уведомлений
//	*pResult = 0;
//}


void CVM_Tool_Create_Profiles::OnSetfocusCheckMultiScan()
{
	// TODO: добавьте свой код обработчика уведомлений
	if(m_CHECK_EnableMultiScan.GetState() == BST_CHECKED)
		::EnableWindow(m_LIST_PE_Sections_Scan.m_hWnd, true);
	else
		::EnableWindow(m_LIST_PE_Sections_Scan.m_hWnd, false);
}


void CVM_Tool_Create_Profiles::OnBnClickedButtonScanMulti()
{
	// TODO: добавьте свой код обработчика уведомлений
	register SERVICE_COUNT SelSections = m_LIST_PE_Sections_Scan.GetSelCount();
	if(SelSections==NULL)
		return;
	
	codedef_check_term_threadif(hGUI_MultiScan_work_thread);

	if(GUI_SCAN_MODE == false)
	{
	//change stand-by mode to SCAN
		if(CurrentFoundSeq_stack != NULL)
			{
				register SERVICE_COUNT i = CurrentFoundSeq_stack;
				fpFoundSeqStack_array = &pFoundSeqStack_array[0];
				//SEQ STACK CREARSET
				fpFoundSeqStack_array--;
				do
				{
					fpFoundSeqStack_array++;
					if(*fpFoundSeqStack_array == NULL)
						continue;
					::VirtualFreeEx(DH_Current_process_fast_handle, *fpFoundSeqStack_array, 0, MEM_RELEASE);
				}while(--i); //clearsetcycle
			}//end if (CurrentFoundSeq_stack != NULL)
		GUI_SCAN_MODE=true;
		CurrentFoundSeq_stack = NULL;
		GUI_Progress_multi_scan_information.Found_VM_ENTERs = NULL;
		GUI_Progress_multi_scan_information.Found_VM_HANHELDs = NULL;
		GUI_Progress_multi_scan_information.Found_VM_CONTEXTs = NULL;
		GUI_Progress_multi_scan_information.Researched_VM_HANHELDs = NULL;
		GUI_Progress_multi_scan_information.Identeficated_VM_HANHELDs = NULL;
		GUI_Progress_multi_scan_information.Bytes_leave = NULL;
		Will_be_create_new_VM_profiles_count = NULL;
		Will_be_create_new_billets_count = NULL;
		Will_be_create_new_PIRATEMAPs = NULL;
		::SetWindowText(m_BUTTON_MULTI_SCAN.m_hWnd,TS_BUTTON_ABORT_MODE);
		::EnableWindow(m_BUTTON_Accept_multi.m_hWnd, false);
		GUI_Progress_multi_scan_information.Total_Bytes = 1;
		if(hGUI_UpdateProgressState == NULL)
			hGUI_UpdateProgressState = ::CreateRemoteThreadEx(DH_Current_process_fast_handle,NULL,NULL, (LPTHREAD_START_ROUTINE)GUI_UPDATE_Progress_inf,this,NULL,NULL, NULL);
		hGUI_MultiScan_work_thread = ::CreateRemoteThreadEx(DH_Current_process_fast_handle,NULL,NULL, (LPTHREAD_START_ROUTINE)Work_multi_scan_proc,this,NULL,NULL, NULL);
	}
	else
	{
	//RETURN to stand-by mode (abort)
		::SetWindowText(m_BUTTON_MULTI_SCAN.m_hWnd,TS_BUTTON_SCAN_STANDBY_MODE);
		GUI_SCAN_MODE=false;
	}
	
//	GUI_SCAN_MODE!=GUI_SCAN_MODE;
}


void CVM_Tool_Create_Profiles::OnSelchangeListSections()
{
	// TODO: добавьте свой код обработчика уведомлений
	TCHAR dig_buf[64];
	register SERVICE_COUNT SelSections = m_LIST_PE_Sections_Scan.GetSelCount();
	itoa_10_fast(SelSections, &dig_buf[0]);
	::SetWindowText(m_EDIT_Sel_SECTIONS_COUNT.m_hWnd, &dig_buf[0]);

	All_selected_section_count = SelSections;

	if(SelSections==NULL)
		{::SetWindowText(m_EDIT_Sel_EQ_BYTES_COUNT.m_hWnd,TS_DEF_NULL); ::EnableWindow(m_BUTTON_MULTI_SCAN.m_hWnd, false); ::ShowWindow(m_PROGRESS_MULTI.m_hWnd, SW_HIDE); All_eq_bytes_in_selected_sections = NULL; return;}
	::EnableWindow(m_BUTTON_MULTI_SCAN.m_hWnd, true);
	::ShowWindow(m_PROGRESS_MULTI.m_hWnd, SW_SHOW);

	PIMAGE_SECTION_HEADER pPESectionList;
	register PIMAGE_SECTION_HEADER* fpSelectedSections = &pSelectedSections[0];
	register SERVICE_COUNT NumofSections = DH_Get_Target_PE_Section(&pPESectionList);
	register size_t Eq_Bytes = NULL;
	do
	{
		if (m_LIST_PE_Sections_Scan.GetSel(NumofSections) == true)
		{
			Eq_Bytes+=pPESectionList[NumofSections].Misc.VirtualSize;
			*fpSelectedSections++ = &pPESectionList[NumofSections];
			if (--SelSections == NULL)
				break;
		}
	}while(NumofSections--);

	itoa_10_fast(Eq_Bytes, &dig_buf[0]);
	::SetWindowText(m_EDIT_Sel_EQ_BYTES_COUNT.m_hWnd, &dig_buf[0]);
	All_eq_bytes_in_selected_sections = Eq_Bytes;
}


void CVM_Tool_Create_Profiles::OnBnClickedButtonSingleMultiPanelShow()
{
	// TODO: добавьте свой код обработчика уведомлений
	GUI_OPENCLOSE_RIGHT_PANEL(GUI_RIGHT_PANEL_state);
}

void CVM_Tool_Create_Profiles::GUI_OPENCLOSE_RIGHT_PANEL(bool variant)
{
RECT rect = {0};
::GetWindowRect(m_hWnd, &rect);

	if (variant == true)
	{
	::ShowWindow(m_CHECK_EnableMultiScan.m_hWnd, SW_SHOW);
	::ShowWindow(m_LIST_PE_Sections_Scan.m_hWnd, SW_SHOW);
	::ShowWindow(m_BUTTON_MULTI_SCAN.m_hWnd, SW_SHOW);
	}
	else
	{
	::ShowWindow(m_CHECK_EnableMultiScan.m_hWnd, SW_HIDE);
	::ShowWindow(m_LIST_PE_Sections_Scan.m_hWnd, SW_HIDE);
	::ShowWindow(m_BUTTON_MULTI_SCAN.m_hWnd, SW_HIDE);
	}


	if (variant)
	{
		::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0,  (rect.right-rect.left)+GUI_RIGHT_TAB_SIZE_ToolCreateProrfile/*width*/, (rect.bottom-rect.top) /*height*/, SWP_NOMOVE|SWP_NOZORDER);
		::SetWindowTextW(m_BUTTON_RIGHT_PANEL.m_hWnd,_T("<"));
	}
	else
	{
		::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0,  (rect.right-rect.left)-GUI_RIGHT_TAB_SIZE_ToolCreateProrfile/*width*/, (rect.bottom-rect.top) /*height*/, SWP_NOMOVE|SWP_NOZORDER);
		::SetWindowTextW(m_BUTTON_RIGHT_PANEL.m_hWnd,_T(">"));
	}

GUI_RIGHT_PANEL_state = !variant;
}

DWORD WINAPI CVM_Tool_Create_Profiles::GUI_UPDATE_Progress_inf(LPVOID Param)
{
	CVM_Tool_Create_Profiles *pThis = reinterpret_cast<CVM_Tool_Create_Profiles*>(Param);
	TCHAR dig_buf[64];

	register SERVICE_COUNT TotalBytes = pThis->GUI_Progress_multi_scan_information.Total_Bytes;
		if(TotalBytes==NULL)
			{pThis->hGUI_UpdateProgressState = NULL; return;}
	register SERVICE_COUNT Bytes_leave = pThis->GUI_Progress_multi_scan_information.Total_Bytes;
	register SERVICE_COUNT LastUpdVar_bytes_leave=NULL;
	register SERVICE_COUNT LastUpdVar_found_VM_enters=NULL;
	register SERVICE_COUNT LastUpdVar_found_VM_hanhelds=NULL;
	register SERVICE_COUNT LastUpdVar_found_VM_contexts=NULL;
	SERVICE_COUNT LastUpdVar_researched_VM_hanhelds=NULL;

	do
	{
		TotalBytes = pThis->GUI_Progress_multi_scan_information.Total_Bytes;
		Bytes_leave = pThis->GUI_Progress_multi_scan_information.Bytes_leave;
		::SleepEx(1,false);
		if((LastUpdVar_bytes_leave == Bytes_leave) || TotalBytes == 1)
			continue;

			pThis->m_PROGRESS_MULTI.SetPos((((TotalBytes-Bytes_leave)*(UPPER_PROGRESS_RANGE-LOWER_PROGRESS_RANGE))/TotalBytes));
			
			if(LastUpdVar_found_VM_enters != pThis->GUI_Progress_multi_scan_information.Found_VM_ENTERs) //Found_VM_ENTERs
			{itoa_10_fast(pThis->GUI_Progress_multi_scan_information.Found_VM_ENTERs,&dig_buf[0]);::SetWindowText(pThis->m_EDIT_FOUND_VM_ENTERs.m_hWnd,&dig_buf[0]); LastUpdVar_found_VM_enters = pThis->GUI_Progress_multi_scan_information.Found_VM_ENTERs;}

			if(LastUpdVar_found_VM_hanhelds != pThis->GUI_Progress_multi_scan_information.Found_VM_HANHELDs) //Found_VM_HANHELDs
			{itoa_10_fast(pThis->GUI_Progress_multi_scan_information.Found_VM_HANHELDs,&dig_buf[0]);::SetWindowText(pThis->m_EDIT_FOUND_VM_HANHELDs.m_hWnd,&dig_buf[0]); LastUpdVar_found_VM_hanhelds = pThis->GUI_Progress_multi_scan_information.Found_VM_HANHELDs;}

			if(LastUpdVar_found_VM_contexts != pThis->GUI_Progress_multi_scan_information.Found_VM_CONTEXTs) //Found_VM_CONTEXTs
			{itoa_10_fast(pThis->GUI_Progress_multi_scan_information.Found_VM_CONTEXTs,&dig_buf[0]);::SetWindowText(pThis->m_EDIT_FOUND_VM_CONTEXTs.m_hWnd,&dig_buf[0]);LastUpdVar_found_VM_contexts == pThis->GUI_Progress_multi_scan_information.Found_VM_CONTEXTs;}

			if(LastUpdVar_researched_VM_hanhelds != pThis->GUI_Progress_multi_scan_information.Researched_VM_HANHELDs) //Researched_VM_HANHELDs
			{
				itoa_10_fast(pThis->GUI_Progress_multi_scan_information.Researched_VM_HANHELDs,&dig_buf[0]);::SetWindowText(pThis->m_EDIT_RESEARCHED_VM_CONTEXTs.m_hWnd,&dig_buf[0]);LastUpdVar_researched_VM_hanhelds == pThis->GUI_Progress_multi_scan_information.Researched_VM_HANHELDs;

				if(pThis->GUI_Progress_multi_scan_information.Identeficated_VM_HANHELDs != NULL)
				{
				size_t percent = ((pThis->GUI_Progress_multi_scan_information.Identeficated_VM_HANHELDs)*100)/pThis->GUI_Progress_multi_scan_information.Researched_VM_HANHELDs;
				itoa_10_fast(percent,&dig_buf[0]);
				wcscat(&dig_buf[0], TS_DEF_PERCENT);
				::SetWindowText(pThis->m_EDIT_IDENTIFICATED_VM_CONTEXTs_percent.m_hWnd,&dig_buf[0]);
				}//end if true
				else
				{
					::SetWindowText(pThis->m_EDIT_IDENTIFICATED_VM_CONTEXTs_percent.m_hWnd,TS_DEF_NULL_PERCENT);
				}//end if(pThis->GUI_Progress_multi_scan_information.Identeficated_VM_HANHELDs != NULL)
			}

	}while(pThis->GUI_SCAN_MODE != false);
	//pThis->GUI_Progress_multi_scan_information.Researched_VM_HANHELDs = 10;
	//pThis->GUI_Progress_multi_scan_information.Identeficated_VM_HANHELDs = 5;

	//last upd
	itoa_10_fast(pThis->GUI_Progress_multi_scan_information.Found_VM_ENTERs,&dig_buf[0]);::SetWindowText(pThis->m_EDIT_FOUND_VM_ENTERs.m_hWnd,&dig_buf[0]);
	itoa_10_fast(pThis->GUI_Progress_multi_scan_information.Found_VM_HANHELDs,&dig_buf[0]);::SetWindowText(pThis->m_EDIT_FOUND_VM_HANHELDs.m_hWnd,&dig_buf[0]);
	itoa_10_fast(pThis->GUI_Progress_multi_scan_information.Found_VM_CONTEXTs,&dig_buf[0]);::SetWindowText(pThis->m_EDIT_FOUND_VM_CONTEXTs.m_hWnd,&dig_buf[0]);
	itoa_10_fast(pThis->GUI_Progress_multi_scan_information.Researched_VM_HANHELDs,&dig_buf[0]);::SetWindowText(pThis->m_EDIT_RESEARCHED_VM_CONTEXTs.m_hWnd,&dig_buf[0]);LastUpdVar_researched_VM_hanhelds == pThis->GUI_Progress_multi_scan_information.Researched_VM_HANHELDs;


				if(pThis->GUI_Progress_multi_scan_information.Identeficated_VM_HANHELDs != NULL)
				{
				size_t percent = ((pThis->GUI_Progress_multi_scan_information.Identeficated_VM_HANHELDs)*100)/pThis->GUI_Progress_multi_scan_information.Researched_VM_HANHELDs;
				itoa_10_fast(percent,&dig_buf[0]);
				wcscat(&dig_buf[0], TS_DEF_PERCENT);
				::SetWindowText(pThis->m_EDIT_IDENTIFICATED_VM_CONTEXTs_percent.m_hWnd,&dig_buf[0]);
				}//end if true
				else
				{
					::SetWindowText(pThis->m_EDIT_IDENTIFICATED_VM_CONTEXTs_percent.m_hWnd,TS_DEF_NULL_PERCENT);
				}//end if(pThis->GUI_Progress_multi_scan_information.Identeficated_VM_HANHELDs != NULL)

	pThis->m_PROGRESS_MULTI.SetPos(UPPER_PROGRESS_RANGE);
	pThis->hGUI_UpdateProgressState = NULL;
}

DWORD WINAPI CVM_Tool_Create_Profiles::Work_multi_scan_proc(LPVOID Param)
{
	CVM_Tool_Create_Profiles *pThis = reinterpret_cast<CVM_Tool_Create_Profiles*>(Param);
	pThis->fpFoundSeqStack_array = &pThis->pFoundSeqStack_array[0];
	pThis->CurrentFoundSeq_stack = NULL;

	if(pThis->CurrentThreadWork_stack != NULL)
		{DH_VM_ThreadWork_ClearSet(&pThis->pActiveThreadWork[0], &pThis->CurrentThreadWork_stack);}
	
	pThis->fpActiveThreadWork_array = &pThis->pActiveThreadWork[0];
	
	register PIMAGE_SECTION_HEADER* fpSelectedSections = &pThis->pSelectedSections[0];
	register SERVICE_COUNT sec_sel_i = pThis->All_selected_section_count;
	pPIRATEMAP_FILELIST_INFO pPMAP_Cache_info = DH_PIRATEMAP_Create_main_info_stack_cache();
	 pThis->fpPMAP_Main_info_stack =  &pThis->Save_PIRATEMAPs.PMAP_Main_info_stack[0];
	do
	{
		PIMAGE_SECTION_HEADER Current_Selected_SECTION_HEADER = *fpSelectedSections;
		pThis->GUI_Progress_multi_scan_information.Total_Bytes = Current_Selected_SECTION_HEADER->Misc.VirtualSize;
		SERVICE_ADDRES RegionStart = Current_Selected_SECTION_HEADER->VirtualAddress + (SERVICE_ADDRES)pThis->hTarget_Main_module;

		
		//allocate new region - include vm code
		 TARGET_PROC_REGION_COPY IncludeVMRegion;
		 IncludeVMRegion.type_of_region = REGION_TYPE_ALL_SECTION_INCLUDE_VM;
		 IncludeVMRegion.region_size = Current_Selected_SECTION_HEADER->Misc.VirtualSize;
		 IncludeVMRegion.region_start_addres = RegionStart;
		 IncludeVMRegion.allocated_pool = (LPBYTE)::VirtualAllocEx(DH_Current_process_fast_handle, NULL, IncludeVMRegion.region_size, MEM_COMMIT, PAGE_READWRITE);

		 if (IncludeVMRegion.allocated_pool == NULL)
			 continue;

		 if(!DH_ReadProcessAddres_DATA(HUNT_HIVE.tOpenProcHANDLE,(LPVOID)IncludeVMRegion.region_start_addres,IncludeVMRegion.region_size,IncludeVMRegion.allocated_pool))
			{::VirtualFreeEx(DH_Current_process_fast_handle, IncludeVMRegion.allocated_pool, 0, MEM_RELEASE); continue;}
		
		*pThis->fpFoundSeqStack_array = DH_VM_Scan_VirtualMachine_Contexts(&IncludeVMRegion, pThis->SelectedVMPX_version, &pThis->GUI_Progress_multi_scan_information);
			if(*pThis->fpFoundSeqStack_array != NULL)
				{
					register pVM_VMPX_PROFILE_FOUND_SEQUENCE_STACK pTempSeqStack = *pThis->fpFoundSeqStack_array;
					if (pTempSeqStack->Found_sequences != NULL)
						{
							pThis->fpFoundSeqStack_array++; 
							pThis->CurrentFoundSeq_stack++;

							memcpy(&pTempSeqStack->IncludeVMRegion, &IncludeVMRegion, sizeof(IncludeVMRegion)); //save allocated region info
							memcpy(&pTempSeqStack->RegionSectionInfo, Current_Selected_SECTION_HEADER, sizeof(IMAGE_SECTION_HEADER)); //save req section info - for region

								if(pPMAP_Cache_info == NULL)
								{
									pThis->Will_be_create_new_PIRATEMAPs++;
									//pThis->Save_PIRATEMAPs.FileList_count++;
									//pThis->fpPMAP_Main_info_stack->PirateMAP_addres_area_type = PMAP_TYPE_PE_IMAGE_SECTION;
									//pThis->fpPMAP_Main_info_stack->PIRATEMAPPING_Region_start_addres = RegionStart;
									//pThis->fpPMAP_Main_info_stack->PIRATEMAPPING_Region_size =  Current_Selected_SECTION_HEADER->Misc.VirtualSize;
									//memcpy(&pThis->fpPMAP_Main_info_stack->PE_Section_Header, Current_Selected_SECTION_HEADER, sizeof(IMAGE_SECTION_HEADER));
									//pThis->fpPMAP_Main_info_stack++; //inc
									codedef_save_presserve_PIRATEMAP;
								}
								else
								{
									if(!DH_PIRATEMAP_Found_target_addres_in_stack_cache(RegionStart,pPMAP_Cache_info))
										{pThis->Will_be_create_new_PIRATEMAPs++; codedef_save_presserve_PIRATEMAP;}
								}//end if ((pPMAP_Cache_info == NULL)

								
								//threadwork for supply15
							//	if(pThis->SelectedVMPX_version == VM_TYPE_VMP2) // ONLY vmp2 PROFILE SUPPORT!
								//{
									*pThis->fpActiveThreadWork_array = DH_VM_Supply15_Shedule_threadwork(pTempSeqStack);
										if(*pThis->fpActiveThreadWork_array != NULL)
												{pThis->fpActiveThreadWork_array++; pThis->CurrentThreadWork_stack++;}
								//}//END IF (pThis->SelectedVMPX_version == VM_TYPE_VMP2)
						}//end if (pTempSeqStack->Found_sequences != NULL)
					else
					{
						::VirtualFreeEx(DH_Current_process_fast_handle, *pThis->fpFoundSeqStack_array, 0, MEM_RELEASE);
						::VirtualFreeEx(DH_Current_process_fast_handle, IncludeVMRegion.allocated_pool, 0, MEM_RELEASE);
					}//end else (pTempSeqStack->Found_sequences != NULL)

				}//end if if(*pThis->fpFoundSeqStack_array != NULL)
		fpSelectedSections++;
	}while(--sec_sel_i);

	//hold on! - waiting for end work threadwork
	if(pThis->CurrentThreadWork_stack)
	{
		sec_sel_i = pThis->CurrentThreadWork_stack;
		pVM_THREADWORK* ffpTrW_stack = &pThis->pActiveThreadWork[0];
		ffpTrW_stack--; //inc
		do
		{
			ffpTrW_stack++; //inc
			pVM_THREADWORK fpTrW_stack= *ffpTrW_stack;
			do
			{
				::SleepEx(0, false);
			}while(fpTrW_stack->TA_Active_Threads_count != NULL); //ОЖИДАЕМ ЗАВЕРШЕНИЯ РАБОТЫ ВСЕХ ПОТОКОВ

		//update - количественный отчёт по хендлам "островкам"
		DH_VM_Supply15_Calc_HANHELDs_quantitative_report__by_ThreadWork(&pThis->pActiveThreadWork[0], &pThis->GUI_Progress_multi_scan_information.Researched_VM_HANHELDs, &pThis->GUI_Progress_multi_scan_information.Identeficated_VM_HANHELDs, pThis->CurrentThreadWork_stack);
		}while(--sec_sel_i);

	//ASSIGN profiles filenames for success SAVE ops - for ALL CONTEXTs
	DH_VMPX_VM_Profiles__Assign_Name_with_mark_crossref_in_Supply15__by_ThreadWork(&pThis->pActiveThreadWork[0], pThis->CurrentThreadWork_stack, &pThis->Will_be_create_new_VM_profiles_count, &pThis->Will_be_create_new_billets_count);
	DH_VM_Supply15_Terminate_ALL_Shedule_threadworks(&pThis->pActiveThreadWork[0], pThis->CurrentThreadWork_stack);
	}//end if (sec_sel_i)
	else
	{
		//ASSIGN profiles filenames for success SAVE ops - for ALL CONTEXTs
		DH_VMPX_VM_Profiles__Assign_Name_with_mark_crossref_in_Supply15__by_ThreadWork(&pThis->pActiveThreadWork[0], pThis->CurrentThreadWork_stack, &pThis->Will_be_create_new_VM_profiles_count, &pThis->Will_be_create_new_billets_count);
	}

	TCHAR dig_buf[64];
	//retail gui CALCULATE edits
	itoa_10_fast(pThis->Will_be_create_new_VM_profiles_count, &dig_buf[0]); ::SetWindowText(pThis->m_EDIT_WILL_BE_CREATE_VM_CONTEXTs.m_hWnd, &dig_buf[0]);
	itoa_10_fast(pThis->Will_be_create_new_billets_count, &dig_buf[0]); ::SetWindowText(pThis->m_EDIT_WILL_BE_CREATE_BILLETSs.m_hWnd, &dig_buf[0]);
	itoa_10_fast(pThis->Will_be_create_new_PIRATEMAPs, &dig_buf[0]); ::SetWindowText(pThis->m_EDIT_WILL_BE_CREATE_PIRATEMAPs.m_hWnd, &dig_buf[0]);


	//free allocated section regions, free SEQ stack
	DH_VM_Seq_ClearSet(&pThis->pFoundSeqStack_array[0], &pThis->CurrentFoundSeq_stack);
	

	::SetWindowText(pThis->m_BUTTON_MULTI_SCAN.m_hWnd,TS_BUTTON_SCAN_STANDBY_MODE);
	pThis->GUI_Progress_multi_scan_information.Bytes_leave = pThis->GUI_Progress_multi_scan_information.Total_Bytes;
	pThis->GUI_SCAN_MODE = false; //false SCAN MODE!!!!!!!!!!!!!!!!!!
	pThis->hGUI_MultiScan_work_thread = NULL;

	if(pThis->GUI_Progress_multi_scan_information.Found_VM_ENTERs == NULL)
		::EnableWindow (pThis->m_BUTTON_Accept_multi.m_hWnd, false);
	else
		::EnableWindow (pThis->m_BUTTON_Accept_multi.m_hWnd, true);

	if(pPMAP_Cache_info != NULL)
		free(pPMAP_Cache_info);
}

void CVM_Tool_Create_Profiles::OnBnClickedAcceptButton2()
{
	// TODO: добавьте свой код обработчика уведомлений
	if(GUI_Progress_multi_scan_information.Found_VM_ENTERs == NULL) //filter #1
		return;
	if(CurrentThreadWork_stack == NULL) //filter #2
		return;

	register SERVICE_COUNT i = CurrentThreadWork_stack;
	fpActiveThreadWork_array = &pActiveThreadWork[0];
	do
	{
		DH_Profile_Shedule_threadwork__Write(*fpActiveThreadWork_array++, TW_pVM_THREADWORK);
	} while (--i);


	if (CurrentThreadWork_stack)
	{
		register size_t sec_sel_i = CurrentThreadWork_stack;
		pVM_THREADWORK* ffpTrW_stack = &pActiveThreadWork[0];
		ffpTrW_stack--; //inc
		do
		{
			ffpTrW_stack++; //inc
			pVM_THREADWORK fpTrW_stack = *ffpTrW_stack;
			do
			{
				::SleepEx(0, false);
				::UpdateWindow(m_hWnd);
			} while (fpTrW_stack->TA_Active_Threads_count != NULL); //ОЖИДАЕМ ЗАВЕРШЕНИЯ РАБОТЫ ВСЕХ ПОТОКОВ																	
		} while (--sec_sel_i);
	}//end if (sec_sel_i)

	/*
	pVMPX_BILLET_INFO_STACK pSAVE_BILLET_InfoStack = (pVMPX_BILLET_INFO_STACK)malloc(sizeof(VMPX_BILLET_INFO_STACK));//стек информации по заготовке
	if(pSAVE_BILLET_InfoStack == NULL) //filter #3
		return;

	pVMPX_VM_PROFILE_INFO_STACK pSAVE_VMPX_VM_InfoStack = (pVMPX_VM_PROFILE_INFO_STACK)malloc(sizeof(VMPX_VM_PROFILE_INFO_STACK));//стек информации о VM профиле
	if(pSAVE_VMPX_VM_InfoStack == NULL) //filter #4
		{free(pSAVE_BILLET_InfoStack);return;}

	//init billet
	memset(pSAVE_BILLET_InfoStack, 0, sizeof(VMPX_BILLET_INFO_STACK)); //set clear
	pSAVE_BILLET_InfoStack->is_vm_billet = true; //billet type
	if (GUI_Progress_multi_scan_information.Found_VM_HANHELDs < LIMIT_HANDLES_ON_SUPPLY) //set approx nums
		pSAVE_BILLET_InfoStack->approx_scan_handles_count = LIMIT_HANDLES_ON_SUPPLY;
	else
		pSAVE_BILLET_InfoStack->approx_scan_handles_count = GUI_Progress_multi_scan_information.Found_VM_HANHELDs;
	pSAVE_BILLET_InfoStack->VMPX_version = pFoundSeqStack_array[0]->VMPX_Version;
	
	size_t VMPX_Profile_name_counter = 1;

	//fp
	fpFoundSeqStack_array = &pFoundSeqStack_array[0]; //SET NULLPRT FP
	ULONG WriteBy = 0;
	//calculate will be created VM profiles & billets
	fpFoundSeqStack_array--;
	do
	{
		fpFoundSeqStack_array++;
		register pVM_VMPX_PROFILE_FOUND_SEQUENCE_STACK pTempSeqStack = *fpFoundSeqStack_array;
		if(pTempSeqStack == NULL)
			continue;
			if (pTempSeqStack->Found_sequences == NULL)
				continue;

				register SERVICE_COUNT ii = pTempSeqStack->Found_sequences;
				pTempSeqStack->fpSeqStackESP = &pTempSeqStack->Found_Seq[0];
				pTempSeqStack->fpSeqStackESP--;
				do
				{
					pTempSeqStack->fpSeqStackESP++;
					if(pTempSeqStack->fpSeqStackESP->isDuplicateContext == true) //skip duplicate
						continue;

						if (pTempSeqStack->fpSeqStackESP->Researched_VM_hanhelds == NULL)
						{
							// ********* BILLET stack SAVE ************
							//select filename
							if(pTempSeqStack->fpSeqStackESP->EXTRACT_extracted_value != NULL)
								itoa_16_fast(pTempSeqStack->fpSeqStackESP->EXTRACT_extracted_value, &pSAVE_BILLET_InfoStack->vm_name[0]);
							else
								itoa_16_fast(pTempSeqStack->fpSeqStackESP->Predict_STATIC_addres, &pSAVE_BILLET_InfoStack->vm_name[0]);

							//set all found vm_enters use currect context (duplicate)
							pSAVE_BILLET_InfoStack->VM_enter_first_asm_addres[0] = pTempSeqStack->fpSeqStackESP->Predict_STATIC_addres;
							pSAVE_BILLET_InfoStack->VM_enter_first_asm_addres_count = 1;

							memcpy(&pSAVE_BILLET_InfoStack->VM_ENTER_Seq_Stack, pTempSeqStack->fpSeqStackESP, sizeof(VM_VMPX_PROFILE_FOUND_SEQUENCE));
							DH_Write_Current_hunt_profile_CONTAINER(PROFILE_TYPE_VM_BILLET,&pSAVE_BILLET_InfoStack->vm_name[0],(BYTE*)pSAVE_BILLET_InfoStack,sizeof(VMPX_BILLET_INFO_STACK),&WriteBy);
						}
						else
						{
							// ********* VM profile stack SAVE ************
							//select filename
							itoa_16_fast(VMPX_Profile_name_counter++, &pSAVE_VMPX_VM_InfoStack->vm_name[0]);

							//set all found vm_enters use currect context (duplicate)
							pSAVE_VMPX_VM_InfoStack->VM_enter_first_asm_addres[0] = pTempSeqStack->fpSeqStackESP->Predict_STATIC_addres;
							pSAVE_VMPX_VM_InfoStack->VM_enter_first_asm_addres_count = 1;

							memcpy(&pSAVE_VMPX_VM_InfoStack->VM_ENTER_Seq_Stack, pTempSeqStack->fpSeqStackESP, sizeof(VM_VMPX_PROFILE_FOUND_SEQUENCE));
							DH_Write_Current_hunt_profile_CONTAINER(PROFILE_TYPE_VM,&pSAVE_VMPX_VM_InfoStack->vm_name[0],(BYTE*)pSAVE_VMPX_VM_InfoStack,sizeof(VMPX_VM_PROFILE_INFO_STACK),&WriteBy);
						}
				
				}while(--ii); //slave parse seq unit

	}while(--i);//common seq stack parse

	free(pSAVE_BILLET_InfoStack);
	free(pSAVE_VMPX_VM_InfoStack);

	// /////////////////////////// SAVE PIRATEMAP /////////////////////////////////////
	fpPMAP_Main_info_stack =  &Save_PIRATEMAPs.PMAP_Main_info_stack[0]; //SET NULLPRT FP
	i = Save_PIRATEMAPs.FileList_count;
	
	if(!i)
		{EndDialog(true); return;}

	//pPIRATEMAP_PROFILE_INFO_STACK pPMAPInfo_Stack = (pPIRATEMAP_PROFILE_INFO_STACK)malloc(sizeof(PIRATEMAP_PROFILE_INFO_STACK));
	fpPMAP_Main_info_stack--;
	do
	{
		fpPMAP_Main_info_stack++;
		size_t FullStructSize = sizeof(PIRATEMAP_PROFILE_INFO_STACK) + fpPMAP_Main_info_stack->PE_Section_Header.Misc.VirtualSize;
		register pPIRATEMAP_PROFILE_INFO_STACK pPMAPInfo_Stack = (pPIRATEMAP_PROFILE_INFO_STACK) ::VirtualAllocEx(DH_Current_process_fast_handle, NULL, FullStructSize , MEM_COMMIT, PAGE_READWRITE);

		if(!pPMAPInfo_Stack)
			continue;
							//select filename
							if(strlen((char*)&fpPMAP_Main_info_stack->PE_Section_Header.Name[0]))
								quick_ASCII_to_UNICODE(&fpPMAP_Main_info_stack->PE_Section_Header.Name[0], &fpPMAP_Main_info_stack->filename[0]);
							else
								itoa_16_fast(fpPMAP_Main_info_stack->PIRATEMAPPING_Region_start_addres, &fpPMAP_Main_info_stack->filename[0]);

		memcpy(&pPMAPInfo_Stack->PMAP_Main_info, fpPMAP_Main_info_stack, sizeof(PIRATEMAP_PROFILE_INFO_STACK));
		DH_Write_Current_hunt_profile_CONTAINER(PROFILE_TYPE_PIRATE_MAP,&fpPMAP_Main_info_stack->filename[0],(BYTE*)pPMAPInfo_Stack,FullStructSize,&WriteBy);

		::VirtualFreeEx(DH_Current_process_fast_handle, pPMAPInfo_Stack, FullStructSize, MEM_RELEASE);
	}while(--i);
	//free(pPMAPInfo_Stack);
	*/

	EndDialog(true);
}


void CVM_Tool_Create_Profiles::OnBnClickedButtonSelectAllSection()
{
	// TODO: добавьте свой код обработчика уведомлений
	size_t f = m_LIST_PE_Sections_Scan.GetCount();

	if(f == 0)
		return;

	do
	{
		m_LIST_PE_Sections_Scan.SetSel(f, true);
	}while(f--);

	OnSelchangeListSections();
}


void CVM_Tool_Create_Profiles::OnBnClickedVmVerD4Radio()
{
	// TODO: добавьте свой код обработчика уведомлений
	GUI_VMP2_exten = false;
	GUI_VMP3_exten = false;
	GUI_VMP_another = false;
	GUI_VM_D4_exten = true;
	GUI_VM_D5_exten = false;
	SelectedVMPX_version = VM_TYPE_VMX_D4;
	GUI_UPDATE_NOW();
}
