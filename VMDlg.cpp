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
#include "VMDlg.h"
#include "afxdialogex.h"
#include "VM_Tool_Create_Profiles.h"
#include "ENGINE/ENGINE_MAP_operations.h"

#include "BEA/BeaEngine.h" //BEA ENGINE

// CVMDlg dialog


IMPLEMENT_DYNAMIC(CVMDlg, CDialogEx)

CVMDlg::CVMDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVMDlg::IDD, pParent)
{
	LOCK_VM_PROFILE_stack = NULL;
	Is_checl_only_selected_vm_profile = false;
}

CVMDlg::~CVMDlg()
{
	codedef_check_free_allocatedif(LOCK_VM_PROFILE_stack);
}

void CVMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_LIST_VM_ENUM, m_list_vms_records);
	DDX_Control(pDX, IDC_EDIT_VM_HANDLES_COUNT, m_EDIT_info_vm_count_consist_of);
	DDX_Control(pDX, IDC_EDIT_VM_SUPPLY15, m_EDIT_info_VM_supply15_adress);
	DDX_Control(pDX, IDC_EDIT_VM_VER, m_EDIT_info_VM_version);
	DDX_Control(pDX, IDC_SELECT_VM_RECORD, m_BUTTON_Select_VM_record);
	DDX_Control(pDX, IDC_EDIT_SEL_HANHELD_ADDRES, m_EDIT_info_HANHELD_ADDRES);
	DDX_Control(pDX, IDC_EDIT_SEL_HANHELD_NUMBER, m_EDIT_info_HANHELD_NUM);
	//  DDX_Control(pDX, IDC_EDIT_VM_HANDLES_DUPLICATE_VS_INFUC, m_EDIT_info_vm_count_consist_of);
	DDX_Control(pDX, IDC_CHECK_EXCLUDE_DUPLICATE_IN_TABLE, m_CHECK_exclude_duplicate_handles);
	DDX_Control(pDX, IDC_EDIT_SEL_HANHELD_JUMPOUTs_COUNT, m_EDIT_info_JUMPOUTs_count);
	DDX_Control(pDX, IDC_EDIT_SEL_HANHELD_CRITADDR_COUNT2, m_EDIT_info_crit_addr_count);
	DDX_Control(pDX, IDC_EDIT_VM_RESERVED_STACK_SIZE, m_EDIT_info_stack_reserved_bytes);
	DDX_Control(pDX, IDC_EDIT_VM_RESERVED_CONTEXT_SITE, m_EDIT_info_context_site);
	DDX_Control(pDX, IDC_EDIT_SEARCH_OBJECT, m_EDIT_SEARCH_object);
	DDX_Control(pDX, IDC_CHECK_SEARCH_ONLY_SELECTED_VM_PROFILE, m_CHECK_Search_only_selected_vm_profile);
	DDX_Control(pDX, IDC_EDIT_SEARCH_COUNT, m_EDIT_search_objects_count);
	DDX_Control(pDX, IDC_EDIT_SEARCH_COUNT2, m_EDIT_search_objects_SUB_count);
	DDX_Control(pDX, IDC_LIST2, m_SEARCH_list);
}


BEGIN_MESSAGE_MAP(CVMDlg, CDialogEx)
//	ON_BN_CLICKED(IDC_SUPPLY_OPS, &CVMDlg::OnBnClickedSupplyOps)
//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CVMDlg::OnLvnItemchangedList1)
//	ON_BN_CLICKED(IDC_ADD_VM_RECORD, &CVMDlg::OnBnClickedAddVmRecord)
//	ON_BN_CLICKED(IDC_ACCEPT_ADD_VM_RECORD, &CVMDlg::OnBnClickedAcceptAddVmRecord)
ON_BN_CLICKED(IDC_ADD_VM_RECORD, &CVMDlg::OnBnClickedAddVmRecord)
ON_LBN_SELCHANGE(IDC_LIST_VM_ENUM, &CVMDlg::OnLbnSelchangeListVmEnum)
ON_BN_CLICKED(IDC_SELECT_VM_RECORD, &CVMDlg::OnBnClickedSelectVmRecord)
//ON_NOTIFY(LVN_LINKCLICK, IDC_LIST1, &CVMDlg::OnLinkclickList1)
ON_NOTIFY(NM_CLICK, IDC_LIST1, &CVMDlg::OnClickList1)
ON_BN_CLICKED(IDC_CHECK_EXCLUDE_DUPLICATE_IN_TABLE, &CVMDlg::OnBnClickedCheckExcludeDuplicateInTable)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CVMDlg::OnLvnItemchangedList1)
ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CVMDlg::OnRclickList1)
ON_NOTIFY(LVN_KEYDOWN, IDC_LIST1, &CVMDlg::OnKeydownList1)
ON_EN_CHANGE(IDC_EDIT_SEARCH_OBJECT, &CVMDlg::OnChangeEditSearchObject)
ON_BN_CLICKED(IDC_CHECK_SEARCH_ONLY_SELECTED_VM_PROFILE, &CVMDlg::OnBnClickedCheckSearchOnlySelectedVmProfile)
END_MESSAGE_MAP()


// CVMDlg message handlers
DH_ENUM_FILES_of_PROFILES CVMDlg::VM_file_lists[MAX_CONTAINTER_FILES];

BOOL CVMDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	GUI_UPDATE_AFTER_FILL_PROFILE(Fill_VM_profiles_list());
	m_CHECK_exclude_duplicate_handles.SetCheck(BST_CHECKED); //BST_CHECK_exclude_duplicate = false;
	BST_CHECK_exclude_duplicate = true;

	//m_list.DeleteColumn(5);
	m_list.DeleteColumn(4);
	m_list.DeleteColumn(3);
	m_list.DeleteColumn(2);
	m_list.DeleteColumn(1);
	m_list.DeleteColumn(0);

	m_list.InsertColumn(0, L"Handle", 0, 90, -1);
	m_list.InsertColumn(1, L"Cell", 0, 90, -1);
	m_list.InsertColumn(2, L"Type", 0, 140, -1);
	m_list.InsertColumn(3, L"p-code", 0, 50, -1); //shift
	m_list.InsertColumn(4, L"RBP", 0, 50, -1); //shift
											   //m_list.InsertColumn(5,L"First asm",0,150,-1);
	//if (DH_VM_Get_current_profile_data_first(&VM_current_stack))
//	{
//	}

//	::SetWindowText(m_edit_address_supply.m_hWnd,_T("00000001481BADEA"));
	return true;
}

//void CVMDlg::OnBnClickedSupplyOps()
//{
//	
//	::GetWindowText(m_edit_address_supply.m_hWnd,&digital_buffer[0],MAX_PATH);
//	UINT64 A = wtou64_16_fast(&digital_buffer[0]);
//	DH_VM_Inspector__Get_Supply_Deport(HUNT_SUPPLY_HIVE);
//}


//void CVMDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//	// TODO: добавьте свой код обработчика уведомлений
//	*pResult = 0;
//}



void CVMDlg::OnBnClickedAddVmRecord()
{
	// TODO: добавьте свой код обработчика уведомлений
	CVM_Tool_Create_Profiles VM_tool_create_new_record;
	if (VM_tool_create_new_record.DoModal() == 1)
		GUI_UPDATE_AFTER_FILL_PROFILE(Fill_VM_profiles_list());
}

bool CVMDlg::Fill_VM_profiles_list(void)
{
size_t i = DH_Get_Current_hunt_profile_ALL_FILE_CONTAINERS(PROFILE_TYPE_VM, &VM_file_lists[0]);
	if(i==0)
		{return false;}
	VM_file_lists_count = i;
	::SendMessage(m_list_vms_records.m_hWnd, LB_RESETCONTENT, 0, 0);  //m_list_vms_records.ResetContent();
	register pDH_ENUM_FILES_of_PROFILES pEnumFileprofile = &VM_file_lists[0];
	do
	{
		::SendMessage(m_list_vms_records.m_hWnd, LB_ADDSTRING, 0, (LPARAM)&pEnumFileprofile->FileName[0]); //m_list_vms_records.AddString(&pEnumFileprofile->FileName[0]);
		pEnumFileprofile++;
	}while(--i);
	return true;
}
	
bool CVMDlg::Fill_selected_VM_information(WCHAR* VM_NAME)
{
	ULONG reta=0;
	pVMPX_VM_PROFILE_INFO_STACK Selected_Stack=0;

	/////////////////// FILL INFO BLOCK DATA /////////////////////////////////////
	Selected_Stack = (pVMPX_VM_PROFILE_INFO_STACK)DH_Read_Current_hunt_profile_CONTAINER(PROFILE_TYPE_VM, VM_NAME,PROFILE_READ_ALL,&reta);
	if (!Selected_Stack)
			return false;


	//				VMP version		+ VM SUPPLY 15 ADDRESS (conditional)	//
	switch(Selected_Stack->VMPX_version)
	{
	case VM_TYPE_VMP2: {::SetWindowTextW(m_EDIT_info_VM_version.m_hWnd,TS_VM_TYPE_VMP2); break;}
	case VM_TYPE_VMP3: {::SetWindowTextW(m_EDIT_info_VM_version.m_hWnd,TS_VM_TYPE_VMP3); ::SetWindowTextW(m_EDIT_info_VM_supply15_adress.m_hWnd,TS_DEF_NA); break;}
	case VM_TYPE_VMX_D4: {::SetWindowTextW(m_EDIT_info_VM_version.m_hWnd, TS_VM_TYPE_D4); ::SetWindowTextW(m_EDIT_info_VM_supply15_adress.m_hWnd, TS_DEF_NA); break; }
	case VM_TYPE_VMX_D5: {::SetWindowTextW(m_EDIT_info_VM_version.m_hWnd, TS_VM_TYPE_D5); ::SetWindowTextW(m_EDIT_info_VM_supply15_adress.m_hWnd, TS_DEF_NA); break; }
	default: {::SetWindowTextW(m_EDIT_info_VM_version.m_hWnd,TS_VM_TYPE_ANOTHER); ::SetWindowTextW(m_EDIT_info_VM_supply15_adress.m_hWnd,TS_DEF_NA); break;}
	}

	//				VM SUPPLY 15 ADDRESS	-	 VMP2		//
	if(Selected_Stack->VMPX_version == VM_TYPE_VMP2)
	{
		itoa_16_fast((SERVICE_ADDRES)Selected_Stack->supply_15_start_address,&digital_buffer[DIG_BUFFER_0X_SHR]);
		::SetWindowTextW(m_EDIT_info_VM_supply15_adress.m_hWnd,&digital_buffer[0]);
	}

	//				VM SUPPLY 15 duplicate hanhelds	 and infucient		-	 VMP2		//
	TCHAR local_dig_buffer[64];
	//format (DEC): all_succes_found/count_of_duplicate/count_of_original/identificated/percent
	itoa_10_fast((size_t)Selected_Stack->handles_count, &buffer[0]);
	itoa_10_fast((size_t)Selected_Stack->duplicate_handles_count, &local_dig_buffer[0]);
	wcscat(&buffer[0], TS_DEF_DIV_UNIT);
	wcscat(&buffer[0], &local_dig_buffer[0]);
	itoa_10_fast(((size_t)Selected_Stack->handles_count - (size_t)Selected_Stack->duplicate_handles_count),&local_dig_buffer[0]);
	wcscat(&buffer[0], TS_DEF_DIV_UNIT);
	wcscat(&buffer[0], &local_dig_buffer[0]);
	itoa_10_fast(((size_t)Selected_Stack->identeficated_handles_count), &local_dig_buffer[0]);
	wcscat(&buffer[0], TS_DEF_DIV_UNIT);
	wcscat(&buffer[0], &local_dig_buffer[0]);
	wcscat(&buffer[0], TS_DEF_DIV_UNIT);
	if (Selected_Stack->identeficated_handles_count != NULL)
	{ 
		itoa_10_fast(((size_t)(Selected_Stack->identeficated_handles_count * 100) / Selected_Stack->handles_count), &local_dig_buffer[0]);
		wcscat(&buffer[0], &local_dig_buffer[0]);
		wcscat(&buffer[0], TS_DEF_PERCENT);
	}
	else
	{
		wcscat(&buffer[0], TS_DEF_NULL_PERCENT);
	}

	::SetWindowText(m_EDIT_info_vm_count_consist_of.m_hWnd,&buffer[0]);
	_ltow(Selected_Stack->Stack_RSP_Different, &buffer[0], 10);
		//if(Selected_Stack->Stack_RSP_Different > 0)
			//itoa_10_fast((ULONG64)Selected_Stack->Stack_RSP_Different, &buffer[0]);
		//else
			//itoa_10_fast((ULONG64)-Selected_Stack->Stack_RSP_Different, &buffer[0]);

	::SetWindowText(m_EDIT_info_stack_reserved_bytes.m_hWnd, &buffer[0]);
	::SetWindowTextA(m_EDIT_info_context_site.m_hWnd, (LPCSTR)&Selected_Stack->RegionSectionInfo.Name[0]);
	// comments //
	//::SetWindowTextW(m_EDIT_info_VM_comments.m_hWnd,&Selected_Stack->comment[0]);

	free(Selected_Stack);
	return true;
}

bool CVMDlg::Load_current_VM_profile_and_fill_all_info_blocks(__in WCHAR* VM_NAME, __in LPVOID pParam)
{
	CVMDlg *pThis = reinterpret_cast<CVMDlg*>(pParam);
	ULONG reta=0;
	
	/////////////////// FILL INFO BLOCK DATA /////////////////////////////////////
	if (pThis->LOCK_VM_PROFILE_stack != NULL)
		free(pThis->LOCK_VM_PROFILE_stack);

	pThis->LOCK_VM_PROFILE_stack = (pVMPX_VM_PROFILE_INFO_STACK)DH_Read_Current_hunt_profile_CONTAINER(PROFILE_TYPE_VM, VM_NAME,PROFILE_READ_ALL,&reta);
	if (!pThis->LOCK_VM_PROFILE_stack)
			return false;

	//				VM SUPPLY 15 duplicate hanhelds	 and infucient		-	 VMP2		//
	TCHAR local_dig_buffer[64];
	//format (DEC): all_succes_found/count_of_duplicate/count_of_original/identificated/percent
	itoa_10_fast((size_t)pThis->LOCK_VM_PROFILE_stack->handles_count, &buffer[0]);
	itoa_10_fast((size_t)pThis->LOCK_VM_PROFILE_stack->duplicate_handles_count, &local_dig_buffer[0]);
	wcscat(&buffer[0], TS_DEF_DIV_UNIT);
	wcscat(&buffer[0], &local_dig_buffer[0]);
	itoa_10_fast(((size_t)pThis->LOCK_VM_PROFILE_stack->handles_count - (size_t)pThis->LOCK_VM_PROFILE_stack->duplicate_handles_count), &local_dig_buffer[0]);
	wcscat(&buffer[0], TS_DEF_DIV_UNIT);
	wcscat(&buffer[0], &local_dig_buffer[0]);
	itoa_10_fast(((size_t)pThis->LOCK_VM_PROFILE_stack->identeficated_handles_count), &local_dig_buffer[0]);
	wcscat(&buffer[0], TS_DEF_DIV_UNIT);
	wcscat(&buffer[0], &local_dig_buffer[0]);
	wcscat(&buffer[0], TS_DEF_DIV_UNIT);
	if (pThis->LOCK_VM_PROFILE_stack->identeficated_handles_count != NULL)
	{
		itoa_10_fast(((size_t)(pThis->LOCK_VM_PROFILE_stack->identeficated_handles_count * 100) / pThis->LOCK_VM_PROFILE_stack->handles_count), &local_dig_buffer[0]);
		wcscat(&buffer[0], &local_dig_buffer[0]);
		wcscat(&buffer[0], TS_DEF_PERCENT);
	}
	else
	{
		wcscat(&buffer[0], TS_DEF_NULL_PERCENT);
	}

	::SetWindowText(pThis->m_EDIT_info_vm_count_consist_of.m_hWnd, &buffer[0]);

	//				VMP version		+ VM SUPPLY 15 ADDRESS (conditional)	//
	switch(pThis->LOCK_VM_PROFILE_stack->VMPX_version)
	{
	case VM_TYPE_VMP2: {::SetWindowText(pThis->m_EDIT_info_VM_version.m_hWnd,TS_VM_TYPE_VMP2); break;}
	case VM_TYPE_VMP3: {::SetWindowText(pThis->m_EDIT_info_VM_version.m_hWnd,TS_VM_TYPE_VMP3); ::SetWindowTextW(pThis->m_EDIT_info_VM_supply15_adress.m_hWnd,TS_DEF_NA); break;}
	default: {::SetWindowTextW(pThis->m_EDIT_info_VM_version.m_hWnd,TS_VM_TYPE_ANOTHER); ::SetWindowTextW(pThis->m_EDIT_info_VM_supply15_adress.m_hWnd,TS_DEF_NA); break;}
	}

	//				VM SUPPLY 15 ADDRESS	-	 VMP2		//
	if(pThis->LOCK_VM_PROFILE_stack->VMPX_version == VM_TYPE_VMP2)
	{
		itoa_16_fast((ULONG64)pThis->LOCK_VM_PROFILE_stack->supply_15_start_address,&digital_buffer[DIG_BUFFER_0X_SHR]);
		::SetWindowTextW(pThis->m_EDIT_info_VM_supply15_adress.m_hWnd,&digital_buffer[0]);
	}

	_ltow(pThis->LOCK_VM_PROFILE_stack->Stack_RSP_Different, &buffer[0], 10);
	::SetWindowTextW(pThis->m_EDIT_info_stack_reserved_bytes.m_hWnd, &buffer[0]);
	::SetWindowTextA(pThis->m_EDIT_info_context_site.m_hWnd, (LPCSTR)&pThis->LOCK_VM_PROFILE_stack->RegionSectionInfo.Name[0]);

	// comments //
	//::SetWindowTextW(m_EDIT_info_VM_comments.m_hWnd,&LOCK_VM_PROFILE_stack->comment[0]);

	/////////////////// parse HANHEDL struct /////////////////////////////////////
	if(pThis->LOCK_VM_PROFILE_stack->handles_count == NULL)
		return false;

	pThis->m_list.DeleteAllItems();

	TCHAR dst_byte_buffer[CHAR_MAX];

		register size_t i = pThis->LOCK_VM_PROFILE_stack->handles_count;
		i--;
		register pHANHELD fpHANHELD = (pHANHELD)((SERVICE_ADDRES)pThis->LOCK_VM_PROFILE_stack + pThis->LOCK_VM_PROFILE_stack->ofs_1Containt_handler);
		fpHANHELD--;
		do
		{
			fpHANHELD++;
				if(pThis->BST_CHECK_exclude_duplicate == true)
				{
					if(fpHANHELD->is_duplicate_hanheld_in_supply15 == true)
							continue;
				}

			
			itoa_16_fast((ULONG64)fpHANHELD->HANDELD_Primary_Addres,&digital_buffer[DIG_BUFFER_0X_SHR]);
			pThis->m_list.InsertItem(0,&digital_buffer[0]);

			itoa_16_fast((ULONG64)fpHANHELD->Cell_Addres,&digital_buffer[DIG_BUFFER_0X_SHR]);		
			pThis->m_list.SetItemText(0,1,&digital_buffer[0]);

			//itoa_16_fast((ULONG64)LOCK_VM_PROFILE_stack->Containt_handler[i].HANDELD_type,&dst_byte_buffer[0]);
			//m_list.SetItemText(0,2,&dst_byte_buffer[0]);
			//HANHELD_TYPE_VM_ENTER, HANHELD_TYPE_VM_EXIT, HANHELD_TYPE_CALL, HANHELD_TYPE_RDTSC, HANHELD_TYPE_XOR_byte_from_addres, HANHELD_TYPE_MOV_reg_memreg, HANHELD_TYPE_MOV_memreg_reg
			TCHAR* HANHELD_type_str = DH_VM_HANHELD_type_to_string(fpHANHELD->HANDELD_type);
			if (HANHELD_type_str)
				pThis->m_list.SetItemText(0,2,HANHELD_type_str);

			_ltow(fpHANHELD->PCODE_Shift,&dst_byte_buffer[0],10);
			pThis->m_list.SetItemText(0,3,&dst_byte_buffer[0]);

			_ltow(fpHANHELD->stack_RBP_Shift,&dst_byte_buffer[0],10);
			pThis->m_list.SetItemText(0,4,&dst_byte_buffer[0]);

			//quick_ASCII_to_UNICODE((BYTE*)&fpHANHELD->hanheld_dissasm_first_insruction[0],&dst_byte_buffer[0]);
			//m_list.SetItemText(0,5,(LPCTSTR)&dst_byte_buffer[0]);
			/*
			LVITEMA lvi;
			lvi.iItem = 0;
			lvi.iSubItem = 4;
			lvi.pszText = (LPSTR) &LOCK_VM_PROFILE_stack->Containt_handler[i].hanheld_dissasm_first_insruction[0];
			::SendMessageA(m_hWnd, LVM_SETITEMTEXTA, NULL, (LPARAM)&lvi);*/
			
		}while(i--);

	return true;
}

bool CVMDlg::Load_current_VM_profile_and_fill_all_info_blocks__manual_hanheld_show(__in WCHAR* VM_NAME, __in LPVOID pParam, __in pHANHELD* ppHanheldStack_start, __in SERVICE_COUNT Stack_count)
{
	CVMDlg *pThis = reinterpret_cast<CVMDlg*>(pParam);
	ULONG reta = 0;

	/////////////////// FILL INFO BLOCK DATA /////////////////////////////////////
	if (pThis->LOCK_VM_PROFILE_stack != NULL)
		free(pThis->LOCK_VM_PROFILE_stack);

	pThis->LOCK_VM_PROFILE_stack = (pVMPX_VM_PROFILE_INFO_STACK)DH_Read_Current_hunt_profile_CONTAINER(PROFILE_TYPE_VM, VM_NAME, PROFILE_READ_ALL, &reta);
	if (!pThis->LOCK_VM_PROFILE_stack)
		return false;

	//				VM SUPPLY 15 duplicate hanhelds	 and infucient		-	 VMP2		//
	TCHAR local_dig_buffer[64];
	//format (DEC): all_succes_found/count_of_duplicate/count_of_original/identificated/percent
	itoa_10_fast((size_t)pThis->LOCK_VM_PROFILE_stack->handles_count, &buffer[0]);
	itoa_10_fast((size_t)pThis->LOCK_VM_PROFILE_stack->duplicate_handles_count, &local_dig_buffer[0]);
	wcscat(&buffer[0], TS_DEF_DIV_UNIT);
	wcscat(&buffer[0], &local_dig_buffer[0]);
	itoa_10_fast(((size_t)pThis->LOCK_VM_PROFILE_stack->handles_count - (size_t)pThis->LOCK_VM_PROFILE_stack->duplicate_handles_count), &local_dig_buffer[0]);
	wcscat(&buffer[0], TS_DEF_DIV_UNIT);
	wcscat(&buffer[0], &local_dig_buffer[0]);
	itoa_10_fast(((size_t)pThis->LOCK_VM_PROFILE_stack->identeficated_handles_count), &local_dig_buffer[0]);
	wcscat(&buffer[0], TS_DEF_DIV_UNIT);
	wcscat(&buffer[0], &local_dig_buffer[0]);
	wcscat(&buffer[0], TS_DEF_DIV_UNIT);
	if (pThis->LOCK_VM_PROFILE_stack->identeficated_handles_count != NULL)
	{
		itoa_10_fast(((size_t)(pThis->LOCK_VM_PROFILE_stack->identeficated_handles_count * 100) / pThis->LOCK_VM_PROFILE_stack->handles_count), &local_dig_buffer[0]);
		wcscat(&buffer[0], &local_dig_buffer[0]);
		wcscat(&buffer[0], TS_DEF_PERCENT);
	}
	else
	{
		wcscat(&buffer[0], TS_DEF_NULL_PERCENT);
	}

	::SetWindowText(pThis->m_EDIT_info_vm_count_consist_of.m_hWnd, &buffer[0]);

	//				VMP version		+ VM SUPPLY 15 ADDRESS (conditional)	//
	switch (pThis->LOCK_VM_PROFILE_stack->VMPX_version)
	{
	case VM_TYPE_VMP2: {::SetWindowText(pThis->m_EDIT_info_VM_version.m_hWnd, TS_VM_TYPE_VMP2); break; }
	case VM_TYPE_VMP3: {::SetWindowText(pThis->m_EDIT_info_VM_version.m_hWnd, TS_VM_TYPE_VMP3); ::SetWindowTextW(pThis->m_EDIT_info_VM_supply15_adress.m_hWnd, TS_DEF_NA); break; }
	default: {::SetWindowTextW(pThis->m_EDIT_info_VM_version.m_hWnd, TS_VM_TYPE_ANOTHER); ::SetWindowTextW(pThis->m_EDIT_info_VM_supply15_adress.m_hWnd, TS_DEF_NA); break; }
	}

	//				VM SUPPLY 15 ADDRESS	-	 VMP2		//
	if (pThis->LOCK_VM_PROFILE_stack->VMPX_version == VM_TYPE_VMP2)
	{
		itoa_16_fast((ULONG64)pThis->LOCK_VM_PROFILE_stack->supply_15_start_address, &digital_buffer[DIG_BUFFER_0X_SHR]);
		::SetWindowTextW(pThis->m_EDIT_info_VM_supply15_adress.m_hWnd, &digital_buffer[0]);
	}

	_ltow(pThis->LOCK_VM_PROFILE_stack->Stack_RSP_Different, &buffer[0], 10);
	::SetWindowTextW(pThis->m_EDIT_info_stack_reserved_bytes.m_hWnd, &buffer[0]);
	::SetWindowTextA(pThis->m_EDIT_info_context_site.m_hWnd, (LPCSTR)&pThis->LOCK_VM_PROFILE_stack->RegionSectionInfo.Name[0]);

	// comments //
	//::SetWindowTextW(m_EDIT_info_VM_comments.m_hWnd,&LOCK_VM_PROFILE_stack->comment[0]);

	/////////////////// parse HANHEDL struct /////////////////////////////////////
	if (pThis->LOCK_VM_PROFILE_stack->handles_count == NULL)
		return false;

	pThis->m_list.DeleteAllItems();

	TCHAR dst_byte_buffer[CHAR_MAX];

	register size_t i = Stack_count;

	
	ppHanheldStack_start--;
	do
	{
		ppHanheldStack_start++;

		if (*ppHanheldStack_start == 0)
			continue;

		register pHANHELD fpHANHELD = *ppHanheldStack_start;
		if (pThis->BST_CHECK_exclude_duplicate == true)
		{
			if (fpHANHELD->is_duplicate_hanheld_in_supply15 == true)
				continue;
		}


		itoa_16_fast((ULONG64)fpHANHELD->HANDELD_Primary_Addres, &digital_buffer[DIG_BUFFER_0X_SHR]);
		pThis->m_list.InsertItem(0, &digital_buffer[0]);

		itoa_16_fast((ULONG64)fpHANHELD->Cell_Addres, &digital_buffer[DIG_BUFFER_0X_SHR]);
		pThis->m_list.SetItemText(0, 1, &digital_buffer[0]);

		TCHAR* HANHELD_type_str = DH_VM_HANHELD_type_to_string(fpHANHELD->HANDELD_type);
		if (HANHELD_type_str)
			pThis->m_list.SetItemText(0, 2, HANHELD_type_str);

		_ltow(fpHANHELD->PCODE_Shift, &dst_byte_buffer[0], 10);
		pThis->m_list.SetItemText(0, 3, &dst_byte_buffer[0]);

		_ltow(fpHANHELD->stack_RBP_Shift, &dst_byte_buffer[0], 10);
		pThis->m_list.SetItemText(0, 4, &dst_byte_buffer[0]);

	} while (--i);

	return true;
}

void CVMDlg::OnLbnSelchangeListVmEnum()
{
	// TODO: добавьте свой код обработчика уведомлений
	WCHAR text_data[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN];
	if (!GUI_GET_VM_RECORD_SELECTED_FILENAME(&text_data[0]))
			return;

	Fill_selected_VM_information(&text_data[0]);
}


void CVMDlg::OnBnClickedSelectVmRecord()
{
	// TODO: добавьте свой код обработчика уведомлений
	WCHAR text_data[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN];
	if (!GUI_GET_VM_RECORD_SELECTED_FILENAME(&text_data[0]))
			return;

	

	Load_current_VM_profile_and_fill_all_info_blocks(&text_data[0], this);
	GUI_Switch_LISTS(true, this);
	
}

void CVMDlg::GUI_UPDATE_AFTER_FILL_PROFILE(bool Fill_result)
{
	::EnableWindow(m_BUTTON_Select_VM_record.m_hWnd, Fill_result);
	UpdateWindow();
}

UINT CVMDlg::GUI_GET_VM_RECORD_SELECTED_FILENAME(__out WCHAR* FileName)
{
	UINT csl = m_list_vms_records.GetCurSel();

	if((HANDLE)csl == INVALID_HANDLE_VALUE)
		return 0;


	if (m_list_vms_records.GetTextLen(csl) > PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN)
		return 0;

	LOCK_VM_PROFILE_filename_len = m_list_vms_records.GetText(csl, &LOCK_VM_PROFILE_filename[0]);
	wcscpy(FileName, &LOCK_VM_PROFILE_filename[0]);

return LOCK_VM_PROFILE_filename_len;
}



void CVMDlg::OnClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений

	WCHAR text_data[G_ADDRES_STRING_UNICODE_MAX_LEN];
	POSITION sel_pos = m_list.GetFirstSelectedItemPosition();
	if(sel_pos == (POSITION)-1 || sel_pos == NULL)
		return;

	sel_pos--; //real item id
	m_list.GetItemText((int)sel_pos,0, &text_data[0],MAX_PATH);
	::SetWindowTextW(m_EDIT_info_HANHELD_ADDRES.m_hWnd, &text_data[0]);
	itoa_10_fast((int)sel_pos,&text_data[0]);
	::SetWindowTextW(m_EDIT_info_HANHELD_NUM.m_hWnd, &text_data[0]);

	ULONG item_wtext_len = m_list.GetItemText((int)sel_pos,1, &buffer[0],MAX_PATH);
	register pHANHELD fpHANHELD = 0;
		if (item_wtext_len > 0)
			{
			fpHANHELD = Find_selected_hanheld_in_current_VM_stack(&buffer[0], item_wtext_len);
					if (fpHANHELD == NULL)
							return;

				itoa_10_fast(fpHANHELD->critical_jumpouts_array_count,&text_data[0]);
				::SetWindowTextW(m_EDIT_info_JUMPOUTs_count.m_hWnd, &text_data[0]);
				itoa_10_fast(fpHANHELD->determine_address_array_count,&text_data[0]);
				::SetWindowTextW(m_EDIT_info_crit_addr_count.m_hWnd, &text_data[0]);
			}

	*pResult = 0;
}


void CVMDlg::OnBnClickedCheckExcludeDuplicateInTable()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (m_CHECK_exclude_duplicate_handles.GetCheck() == BST_CHECKED)
		BST_CHECK_exclude_duplicate = true;
	else
		BST_CHECK_exclude_duplicate = false;
}


void CVMDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	*pResult = 0;
}


void CVMDlg::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	POSITION sel_pos = m_list.GetFirstSelectedItemPosition();
	if(sel_pos == (POSITION)-1 || sel_pos == NULL)
		return;

	POINT cursor_point;
	::GetCursorPos(&cursor_point);

	CMenu menu;
	menu.CreatePopupMenu();
	
	menu.AppendMenu(MF_STRING|MF_POPUP, nBaseId+1, _T("(Only this) Add type determine instructions to path inspector"));
	menu.AppendMenu(MF_SEPARATOR, nBaseId+2, _T("..."));
	menu.AppendMenu(MF_STRING|MF_POPUP, nBaseId+3, _T("(All VMs) Add all -= read/write _DS=- to path inspector"));
	menu.AppendMenu(MF_STRING|MF_POPUP, nBaseId+4, _T("(All VMs) Add all -= _radn (RDTSC)=- to path inspector"));
	menu.AppendMenu(MF_STRING|MF_POPUP, nBaseId+5, _T("(All VMs) Add all -= _CRC check=- to path inspector"));
	menu.AppendMenu(MF_SEPARATOR, nBaseId+6, _T("..."));
	menu.AppendMenu(MF_STRING|MF_POPUP, nBaseId+7, _T("(All VMs) Add all hanhelds tables to path inspector"));

	
	register ULONG menusel = TrackPopupMenuEx(menu, TPM_LEFTALIGN|TPM_RIGHTBUTTON|TPM_RETURNCMD, cursor_point.x, cursor_point.y,m_hWnd,NULL);
	
	
	switch(menusel)
	{
	case nBaseId+1:{
		/*
		ULONG item_wtext_len = m_list.GetItemText((int)--sel_pos,1, &buffer[0],MAX_PATH);
		if (item_wtext_len > 0)
			{
				ULONG array_i = Find_selected_hanheld_in_current_VM_stack(&buffer[0], item_wtext_len);
					if (array_i == NULL)
							break;

				DH_PATCH_Add_new_vmp_execute_address_patch(&LOCK_VM_PROFILE_stack->Containt_handler[array_i].determine_type_addres_array[9],
					&LOCK_VM_PROFILE_stack->Containt_handler[array_i].determine_type_addres_array__HANHELD_in_AddrPool_ID[9],
					&LOCK_VM_PROFILE_stack->name[0],
					array_i,
					LOCK_VM_PROFILE_stack->Containt_handler[array_i].HANDELD_type, PATCH_OWNER_VM_INSIDE_EXECUTE);
			}	*/
		break;}
	
	case nBaseId+3:{AddPatchInspectorOps_AllVM_readwriteDS(); break;}
	case nBaseId+4:{AddPatchInspectorOps_AllVM_randRDTSC();   break;}
	case nBaseId+5:{AddPatchInspectorOps_AllVM_CRC_check();   break;}
	}

	menu.DestroyMenu();
	*pResult = 0;
}
pHANHELD CVMDlg::Find_selected_hanheld_in_current_VM_stack(WCHAR* wchar_hanheld_addres, ULONG wtext_len)
{
	register SERVICE_ADDRES list_cell_addres = _wcstoui64(wchar_hanheld_addres,NULL, 16);

	if (list_cell_addres == NULL)
		return NULL;

	if (LOCK_VM_PROFILE_stack->handles_count == NULL)
		return NULL;

	register size_t i = LOCK_VM_PROFILE_stack->handles_count;
	register pHANHELD fpHANHELD = (pHANHELD)((SERVICE_ADDRES)LOCK_VM_PROFILE_stack + LOCK_VM_PROFILE_stack->ofs_1Containt_handler);
	do
	{
		if(list_cell_addres == (SERVICE_ADDRES)fpHANHELD->Cell_Addres)
			return fpHANHELD;
		fpHANHELD++;
	}while (i--);

	return NULL;
}

void CVMDlg::OnKeydownList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	POSITION sel_pos = m_list.GetFirstSelectedItemPosition();
    if(sel_pos == (POSITION)-1 || sel_pos == NULL)
		return;

	 switch (pLVKeyDow->wVKey)
    {
	case VK_UP: {OnClickList1(pNMHDR,pResult); break;}
	case VK_DOWN: {OnClickList1(pNMHDR,pResult); break;}
	}
	*pResult = 0;
}

void CVMDlg::AddPatchInspectorOps_AllVM_readwriteDS()
{
	register SERVICE_COUNT i = DH_Get_Current_hunt_profile_ALL_FILE_CONTAINERS(PROFILE_TYPE_VM, &VM_file_lists[0]);
	if(i==0)
		return;
	/*
	register pVMPX_VM_PROFILE_INFO_STACK Operation_vmp2_stack;
	DWORD reta = NULL;

	register SERVICE_COUNT hanheld_ii;
	do
	{
		Operation_vmp2_stack = (pVMPX_VM_PROFILE_INFO_STACK)DH_Read_Current_hunt_profile_CONTAINER(PROFILE_TYPE_VM, &VM_file_lists[i].FileName[0],PROFILE_READ_ALL,&reta);
		if(Operation_vmp2_stack == NULL)
			continue;

		hanheld_ii = VM_HANDLES_MAX_COUNT;

		do
		{
			switch(Operation_vmp2_stack->Containt_handler[hanheld_ii].HANDELD_type)
			{
			case HANHELD_TYPE_MOV_memreg64_reg8_DS:
			case HANHELD_TYPE_MOV_memreg64_reg16_DS:
			case HANHELD_TYPE_MOV_memreg64_reg32_DS:
			case HANHELD_TYPE_MOV_memreg64_reg64_DS:

			case HANHELD_TYPE_MOV_reg8_memreg64_DS:
			case HANHELD_TYPE_MOV_reg16_memreg64_DS:
			case HANHELD_TYPE_MOV_reg32_memreg64_DS:
			case HANHELD_TYPE_MOV_reg64_memreg64_DS: 	
				DH_PATCH_Add_new_vmp_execute_address_patch(&Operation_vmp2_stack->Containt_handler[hanheld_ii].determine_type_addres_array[9], 
					&Operation_vmp2_stack->Containt_handler[hanheld_ii].determine_type_addres_array__HANHELD_in_AddrPool_ID[9],
															&Operation_vmp2_stack->name[0],
					hanheld_ii,
					Operation_vmp2_stack->Containt_handler[hanheld_ii].HANDELD_type, PATCH_OWNER_VM_INSIDE_EXECUTE);
			}//end switch
		}while(hanheld_ii--);//do - while

	DH_PATCH_Add_new_vmp_DATA_address_patch((SERVICE_ADDRES)Operation_vmp2_stack->r12_supply_15_start_address, sizeof(CELL_ADDRES)*LIMIT_HANDLES_ON_SUPPLY, &Operation_vmp2_stack->name[0],PATCH_OWNER_VM_SUPPLY15_DATA); //DATA - cover SUPPLY15

	DH_PATCH_Add_new_vmp_DATA_address_patch((SERVICE_ADDRES)Operation_vmp2_stack->Containt_handler[0].HANDELD_Primary_Addres, sizeof(CELL_ADDRES)*LIMIT_HANDLES_ON_SUPPLY, &Operation_vmp2_stack->name[0],PATCH_OWNER_VM_COVERAGE); //DATA - cover hanhleds addreses

		free(Operation_vmp2_stack);
	}while(i--);
	*/
}

void CVMDlg::AddPatchInspectorOps_AllVM_randRDTSC()
{
	register SERVICE_COUNT i = DH_Get_Current_hunt_profile_ALL_FILE_CONTAINERS(PROFILE_TYPE_VM, &VM_file_lists[0]);
	if(i==0)
		return;
	
	register pVMPX_VM_PROFILE_INFO_STACK Operation_vmp2_stack;
	DWORD reta = NULL;
	/*
	register SERVICE_COUNT hanheld_ii;
	do
	{
		Operation_vmp2_stack = (pVMPX_VM_PROFILE_INFO_STACK)DH_Read_Current_hunt_profile_CONTAINER(PROFILE_TYPE_VM, &VM_file_lists[i].FileName[0],PROFILE_READ_ALL,&reta);
		if(Operation_vmp2_stack == NULL)
			continue;

		hanheld_ii = VM_HANDLES_MAX_COUNT;

		do
		{
			switch(Operation_vmp2_stack->Containt_handler[hanheld_ii].HANDELD_type)
			{
			case HANHELD_TYPE_RDTSC: 	
				DH_PATCH_Add_new_vmp_execute_address_patch(&Operation_vmp2_stack->Containt_handler[hanheld_ii].determine_type_addres_array[9], 
					&Operation_vmp2_stack->Containt_handler[hanheld_ii].determine_type_addres_array__HANHELD_in_AddrPool_ID[9],
															&Operation_vmp2_stack->name[0],
					hanheld_ii,
					Operation_vmp2_stack->Containt_handler[hanheld_ii].HANDELD_type,PATCH_OWNER_VM_INSIDE_EXECUTE);
			}//end switch
		}while(hanheld_ii--);//do - while

		free(Operation_vmp2_stack);
	}while(i--);
	*/
}

void CVMDlg::AddPatchInspectorOps_AllVM_CRC_check()
{
	register SERVICE_COUNT i = DH_Get_Current_hunt_profile_ALL_FILE_CONTAINERS(PROFILE_TYPE_VM, &VM_file_lists[0]);
	if(i==0)
		return;
	
	register pVMPX_VM_PROFILE_INFO_STACK Operation_vmp2_stack;
	DWORD reta = NULL;
	/*
	register SERVICE_COUNT hanheld_ii;
	do
	{
		Operation_vmp2_stack = (pVMPX_VM_PROFILE_INFO_STACK)DH_Read_Current_hunt_profile_CONTAINER(PROFILE_TYPE_VM, &VM_file_lists[i].FileName[0],PROFILE_READ_ALL,&reta);
		if(Operation_vmp2_stack == NULL)
			continue;


		hanheld_ii = VM_HANDLES_MAX_COUNT;

		do
		{
			switch(Operation_vmp2_stack->Containt_handler[hanheld_ii].HANDELD_type)
			{
			case HANHELD_TYPE_XOR_byte_from_memreg64: 	
				DH_PATCH_Add_new_vmp_execute_address_patch(&Operation_vmp2_stack->Containt_handler[hanheld_ii].determine_type_addres_array[9],
					&Operation_vmp2_stack->Containt_handler[hanheld_ii].determine_type_addres_array__HANHELD_in_AddrPool_ID[9],
															&Operation_vmp2_stack->name[0],
					hanheld_ii,
					Operation_vmp2_stack->Containt_handler[hanheld_ii].HANDELD_type,PATCH_OWNER_VM_INSIDE_EXECUTE);
			}//end switch
		}while(hanheld_ii--);//do - while

		free(Operation_vmp2_stack);
	}while(i--);
	*/
}

void CVMDlg::OnChangeEditSearchObject()
{
	TCHAR dig_buf[64];
	size_t f = ::GetWindowText(m_EDIT_SEARCH_object.m_hWnd, &dig_buf[0], 64);
	if (f == 0) //фильтр №1
	{
		::SetWindowText(m_EDIT_search_objects_count.m_hWnd, TS_DEF_NA);
		::SetWindowText(m_EDIT_search_objects_SUB_count.m_hWnd, TS_DEF_NA);
		return;
	}

	//попытка определить как тип
	SERVICE_TYPE prep_hanheld_type = DH_VM_HANHELD_string_To_type(&dig_buf[0]);

	if (prep_hanheld_type == NULL)
	{
		//определяем, как адрес
		SERVICE_ADDRES pObject_text_addres_wcstoul = _wcstoui64(&dig_buf[0], NULL, 16);
		if (pObject_text_addres_wcstoul == NULL || pObject_text_addres_wcstoul <= 0xFF)
			return;
	}

	//корректно определены введенные данные
	DWORD TID = NULL;
	::CreateRemoteThreadEx(DH_Current_process_fast_handle, NULL, NULL, (LPTHREAD_START_ROUTINE)SearchObject_Ops, this, NULL, NULL, &TID);
}

void CVMDlg::SearchObject_Ops(LPVOID pParam)
{
	CVMDlg *pThis = reinterpret_cast<CVMDlg*>(pParam);
	TCHAR dig_buf[64];

	::SendMessage(pThis->m_SEARCH_list.m_hWnd, LVM_DELETEALLITEMS, 0, 0L); //pThis->m_SEARCH_list.DeleteAllItems();
	::SendMessage(pThis->m_SEARCH_list.m_hWnd, LVM_DELETECOLUMN, 5, 0); //pThis->m_SEARCH_list.DeleteColumn(5);
	::SendMessage(pThis->m_SEARCH_list.m_hWnd, LVM_DELETECOLUMN, 4, 0);
	::SendMessage(pThis->m_SEARCH_list.m_hWnd, LVM_DELETECOLUMN, 3, 0);
	::SendMessage(pThis->m_SEARCH_list.m_hWnd, LVM_DELETECOLUMN, 2, 0);
	::SendMessage(pThis->m_SEARCH_list.m_hWnd, LVM_DELETECOLUMN, 1, 0);
	::SendMessage(pThis->m_SEARCH_list.m_hWnd, LVM_DELETECOLUMN, 0, 0);

	size_t f = ::GetWindowText(pThis->m_EDIT_SEARCH_object.m_hWnd, &dig_buf[0], 64);
	if (f == 0) //фильтр №1
	{
		::SetWindowText(pThis->m_EDIT_search_objects_count.m_hWnd, TS_DEF_NA);
		::SetWindowText(pThis->m_EDIT_search_objects_SUB_count.m_hWnd, TS_DEF_NA);
		return;
	}

	::EnableWindow(pThis->m_EDIT_SEARCH_object.m_hWnd, false);
	register pVM_THREADWORK__OBJECT_SEARCH pf_object_threadwork;

	if (!pThis->Is_checl_only_selected_vm_profile)
		pf_object_threadwork = DH_VM_Search_Object_in_containters_MT(&pThis->VM_file_lists[0], pThis->VM_file_lists_count, &dig_buf[0]);
	else
	{
		if (pThis->LOCK_VM_PROFILE_filename_len == 0)
		{
			::EnableWindow(pThis->m_EDIT_SEARCH_object.m_hWnd, true);
			return;
		}

		DH_ENUM_FILES_of_PROFILES One_Enum_file;
		wcscpy(&One_Enum_file.FileName[0], &pThis->LOCK_VM_PROFILE_filename[0]);
		pf_object_threadwork = DH_VM_Search_Object_in_containters_MT(&One_Enum_file, 1, &dig_buf[0]);
	}

	if (pf_object_threadwork == NULL)
	{
		::SetWindowText(pThis->m_EDIT_search_objects_count.m_hWnd, TS_DEF_NA);
		::SetWindowText(pThis->m_EDIT_search_objects_SUB_count.m_hWnd, TS_DEF_NA);
		::EnableWindow(pThis->m_EDIT_SEARCH_object.m_hWnd, true);
		return;
	}
	if (pf_object_threadwork->TA_Total_VM_Profiles_found_count == NULL)
	{
		DH_VM_ThreadWork_Object_found_ClearSet(pf_object_threadwork);
		::SendMessage(pThis->m_SEARCH_list.m_hWnd, LVM_DELETEALLITEMS, 0, 0L);
		::SetWindowText(pThis->m_EDIT_search_objects_count.m_hWnd, TS_DEF_NULL);
		::SetWindowText(pThis->m_EDIT_search_objects_SUB_count.m_hWnd, TS_DEF_NULL);
		::EnableWindow(pThis->m_EDIT_SEARCH_object.m_hWnd, true);
		return;
	}

	//ЗАПОЛНЕНИЕ ТАБЛИЦЫ НАЙДЕННЫХ ЗНАЧЕНИЙ
	itoa_10_fast(pf_object_threadwork->TA_Total_VM_Profiles_found_count, &dig_buf[0]);
	::SetWindowText(pThis->m_EDIT_search_objects_count.m_hWnd, &dig_buf[0]);
	itoa_10_fast(pf_object_threadwork->TA_Total_HANHELDS__in_VM_Profiles_found_count, &dig_buf[0]);
	::SetWindowText(pThis->m_EDIT_search_objects_SUB_count.m_hWnd, &dig_buf[0]);

	//ПРОЦЕСС ПОКАЗА РЕЗУЛЬТАТОВ (GUI)
	switch(pf_object_threadwork->TA_Total_VM_Profiles_found_count) //сколько VM профилей было найдено
	{
		case 0:
		{
			DH_VM_ThreadWork_Object_found_ClearSet(pf_object_threadwork);
			return;
		break;}
		case 1: //только один контекст - находим и выбираем (Select)  через VM_file_lists
		{
			switch (pf_object_threadwork->TA_Total_HANHELDS__in_VM_Profiles_found_count) //сколько ХЕНДЛОВ было найдено в VM профилях
			{
			case 0:
				{
				//ПРОСТО ЗАМЕНИТЬ ТЕКУЩИЙ ОТКРЫТЫЙ (SELECT) VM ПРОФИЛЬ
				register size_t i_profiles = MAX_CONTAINTER_FILES;
				register pDH_VM_FOUND_RESULT fpVM_found_result = &pf_object_threadwork->VM_Found_result[0];
				register pDH_ENUM_FILES_of_PROFILES fpEnumFiles = &pThis->VM_file_lists[0];
				//перечисляем только VM_профили
				fpVM_found_result--;
				fpEnumFiles--;
				do
				{
					fpVM_found_result++; //VM_Profiles inc
					fpEnumFiles++; //VM file lists

					if (fpVM_found_result->pVM_Success_found_VM_profile == NULL)
						continue;

					Load_current_VM_profile_and_fill_all_info_blocks(&fpEnumFiles->FileName[0], pThis);
					GUI_Switch_LISTS(true, pThis);

					long fa = pThis->m_list_vms_records.GetCount();
					if (fa <= 0)
						break;
					TCHAR text_buff[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN];
					do
					{
						::SendMessage(pThis->m_list_vms_records.m_hWnd, LB_GETTEXT, fa, (LPARAM)&text_buff[0]);
						if (wcsicmp(&fpEnumFiles->FileName[0], &text_buff[0]))
							continue;
					//	pThis->m_list_vms_records.SetSel(fa, true);
						::SendMessage(pThis->m_list_vms_records.m_hWnd, LB_SETCURSEL, TRUE, (LPARAM)fa); //pThis->m_list_vms_records.SetSel(fa, true);
						break;
					} while (--fa);
					break;
				} while (--i_profiles);
				break;}
			default:
			{
				//ЗАМЕНИТЬ ТЕКУЩИЙ ОТКРЫТЫЙ (SELECT) VM ПРОФИЛЬ И ПОКАЗАТЬ ХЕНДЛЫ
				register size_t i_profiles = MAX_CONTAINTER_FILES;
				register pDH_VM_FOUND_RESULT fpVM_found_result = &pf_object_threadwork->VM_Found_result[0];
				register pDH_ENUM_FILES_of_PROFILES fpEnumFiles = &pThis->VM_file_lists[0];
				//перечисляем только VM_профили
				fpVM_found_result--;
				fpEnumFiles--;
				do
				{
					fpVM_found_result++; //VM_Profiles inc
					fpEnumFiles++; //VM file lists

					if (fpVM_found_result->pVM_Success_found_VM_profile == NULL)
						continue;

					Load_current_VM_profile_and_fill_all_info_blocks__manual_hanheld_show(&fpEnumFiles->FileName[0], pThis, &fpVM_found_result->pFoundHanheld[0], fpVM_found_result->Found_hanheld_stack_count);
					GUI_Switch_LISTS(true, pThis);

					long fa = pThis->m_list_vms_records.GetCount();
					if (fa <= 0)
						break;
					TCHAR text_buff[PROFILE_nfo_MAX_CONTAINER_FILENAME_LEN];
					do
					{
						::SendMessage(pThis->m_list_vms_records.m_hWnd, LB_GETTEXT, fa, (LPARAM)&text_buff[0]);
						if (wcsicmp(&fpEnumFiles->FileName[0], &text_buff[0]))
							continue;
						//	pThis->m_list_vms_records.SetSel(fa, true);
						::SendMessage(pThis->m_list_vms_records.m_hWnd, LB_SETCURSEL, TRUE, (LPARAM)fa); //pThis->m_list_vms_records.SetSel(fa, true);
						break;
					} while (--fa);
					break;
				} while (--i_profiles);

				break; }
			}//end switch (pf_object_threadwork->TA_Total_HANHELDS__in_VM_Profiles_found_count)
		break;}
		default: //множественная выборка
		{
			__assume(pf_object_threadwork->TA_Total_VM_Profiles_found_count>1);

			switch (pf_object_threadwork->TA_Total_HANHELDS__in_VM_Profiles_found_count) //сколько ХЕНДЛОВ было найдено в VM профилях
			{
				case 0:
				{
				pThis->m_SEARCH_list.InsertColumn(0, L"VM Profile", 0, 100, -1);
				pThis->m_SEARCH_list.InsertColumn(1, L"Found data type", 0, 120, -1);
				pThis->m_SEARCH_list.InsertColumn(2, L"Supply15", 0, 110, -1);
				pThis->m_SEARCH_list.InsertColumn(3, L"VM version", 0, 110, -1);
				pThis->m_SEARCH_list.InsertColumn(4, L"Stack reserve", 0, 110, -1);
				pThis->m_SEARCH_list.InsertColumn(5, L"Ident hanhles", 0, 100, -1);

				register size_t i_Succes_found = pf_object_threadwork->TA_Total_VM_Profiles_found_count;
				register size_t i_profiles = MAX_FOUND_HANHELDS;
				register pDH_VM_FOUND_RESULT fpVM_found_result = &pf_object_threadwork->VM_Found_result[0];

				//перечисляем только VM_профили
				fpVM_found_result--;
					do
					{
						fpVM_found_result++; //VM_Profiles inc

						if (fpVM_found_result->pVM_Success_found_VM_profile == NULL)
							continue;

						//set "VM Profile" name (0)
						pThis->m_SEARCH_list.InsertItem(0, &fpVM_found_result->pVM_Success_found_VM_profile->vm_name[0]);

						//set "Found data type" name (1)
						pThis->m_SEARCH_list.SetItemText(0, 1, DH_VM_FOUND_RESULT_type_to_string(fpVM_found_result->Found_target_type[0]));

						//set VM version + suplly15 (3) (2)
						switch (fpVM_found_result->pVM_Success_found_VM_profile->VMPX_version)
						{
						case VM_TYPE_VMP2: {pThis->m_SEARCH_list.SetItemText(0, 3, TS_VM_TYPE_VMP2); 
							itoa_16_fast((size_t)fpVM_found_result->pVM_Success_found_VM_profile->supply_15_start_address, &dig_buf[0]);
							pThis->m_SEARCH_list.SetItemText(0, 2, &dig_buf[0]);
							break; }
						case VM_TYPE_VMP3: {pThis->m_SEARCH_list.SetItemText(0, 3, TS_VM_TYPE_VMP3);  pThis->m_SEARCH_list.SetItemText(0, 2, TS_DEF_NA); break; }
						case VM_TYPE_VMX_D4: {pThis->m_SEARCH_list.SetItemText(0, 3, TS_VM_TYPE_D4); pThis->m_SEARCH_list.SetItemText(0, 2, TS_DEF_NA); break; }
						case VM_TYPE_VMX_D5: {pThis->m_SEARCH_list.SetItemText(0, 3, TS_VM_TYPE_D5);  pThis->m_SEARCH_list.SetItemText(0, 2, TS_DEF_NA); break; }
						default: {pThis->m_SEARCH_list.SetItemText(0, 3, TS_VM_TYPE_ANOTHER);  itoa_16_fast((size_t)fpVM_found_result->pVM_Success_found_VM_profile->supply_15_start_address, &dig_buf[0]);
							pThis->m_SEARCH_list.SetItemText(0, 2, &dig_buf[0]);
							break; }
						}

						//set "Stack reserve" (4)
						_ltow(fpVM_found_result->pVM_Success_found_VM_profile->Stack_RSP_Different, &dig_buf[0], 10);
						//itoa_10_fast((size_t)fpVM_found_result->pVM_Success_found_VM_profile->Stack_RSP_Different, &dig_buf[0]);
						pThis->m_SEARCH_list.SetItemText(0, 4, &dig_buf[0]);

						//set "Handles count" (5)
						itoa_10_fast((size_t)fpVM_found_result->pVM_Success_found_VM_profile->handles_count, &dig_buf[0]);
						pThis->m_SEARCH_list.SetItemText(0, 5, &dig_buf[0]);

						if (--i_Succes_found == 0)
							break;
					}while (--i_profiles);
				break;}
				default:
				{
					__assume(pf_object_threadwork->TA_Total_HANHELDS__in_VM_Profiles_found_count>0);
					pThis->m_SEARCH_list.InsertColumn(0, L"VM Profile", 0, 100, -1);
					pThis->m_SEARCH_list.InsertColumn(1, L"Found data type", 0, 120, -1);
					pThis->m_SEARCH_list.InsertColumn(2, L"Supply15", 0, 110, -1);
					pThis->m_SEARCH_list.InsertColumn(3, L"VM version", 0, 110, -1);
					pThis->m_SEARCH_list.InsertColumn(4, L"Hanheld", 0, 100, -1);
					pThis->m_SEARCH_list.InsertColumn(5, L"Hanheld TYPEDEF", 0, 120, -1);

					register size_t i_Succes_found = pf_object_threadwork->TA_Total_VM_Profiles_found_count;
					register size_t i_profiles = MAX_FOUND_HANHELDS;
					register pDH_VM_FOUND_RESULT fpVM_found_result = &pf_object_threadwork->VM_Found_result[0];

					//перечисляем VM_профили
					fpVM_found_result--;
					do
					{
						fpVM_found_result++; //VM_Profiles inc

						if (fpVM_found_result->pVM_Success_found_VM_profile == NULL)
							continue;

						SERVICE_COUNT hanheld_stack_count = fpVM_found_result->Found_hanheld_stack_count;
						if (hanheld_stack_count == 0)
							continue;

							//Fill from Hanhleds stack
							pHANHELD* ppHanheld = &fpVM_found_result->pFoundHanheld[0];
							SERVICE_TYPE* ppTargetObjectType = &fpVM_found_result->Found_target_type[0];
							ppHanheld--;
							ppTargetObjectType--;
							do
							{ 
								ppHanheld++;
								ppTargetObjectType++;
								pHANHELD pHanheld = *ppHanheld;
								if (pHanheld == 0)
									continue;

								//set "VM Profile" name (0)
								pThis->m_SEARCH_list.InsertItem(0, &fpVM_found_result->pVM_Success_found_VM_profile->vm_name[0]);

								//set "Found data type" name (1)
								pThis->m_SEARCH_list.SetItemText(0, 1, DH_VM_FOUND_RESULT_type_to_string(*ppTargetObjectType));

								//set VM version + suplly15 (3) (2)
								switch (fpVM_found_result->pVM_Success_found_VM_profile->VMPX_version)
								{
								case VM_TYPE_VMP2: {pThis->m_SEARCH_list.SetItemText(0, 3, TS_VM_TYPE_VMP2);
									itoa_16_fast((size_t)fpVM_found_result->pVM_Success_found_VM_profile->supply_15_start_address, &dig_buf[0]);
									pThis->m_SEARCH_list.SetItemText(0, 2, &dig_buf[0]);
									break; }
								case VM_TYPE_VMP3: {pThis->m_SEARCH_list.SetItemText(0, 3, TS_VM_TYPE_VMP3);  pThis->m_SEARCH_list.SetItemText(0, 2, TS_DEF_NA); break; }
								case VM_TYPE_VMX_D4: {pThis->m_SEARCH_list.SetItemText(0, 3, TS_VM_TYPE_D4); pThis->m_SEARCH_list.SetItemText(0, 2, TS_DEF_NA); break; }
								case VM_TYPE_VMX_D5: {pThis->m_SEARCH_list.SetItemText(0, 3, TS_VM_TYPE_D5);  pThis->m_SEARCH_list.SetItemText(0, 2, TS_DEF_NA); break; }
								default: {pThis->m_SEARCH_list.SetItemText(0, 3, TS_VM_TYPE_ANOTHER);  itoa_16_fast((size_t)fpVM_found_result->pVM_Success_found_VM_profile->supply_15_start_address, &dig_buf[0]);
									pThis->m_SEARCH_list.SetItemText(0, 2, &dig_buf[0]);
									break; }
								}

								//set "Hanheld" address (4)
								itoa_16_fast((size_t)pHanheld->HANDELD_Primary_Addres, &dig_buf[0]);
								pThis->m_SEARCH_list.SetItemText(0, 4, &dig_buf[0]);

								//set "Hanheld TYPEDEF" (5)
								pThis->m_SEARCH_list.SetItemText(0, 5, DH_VM_HANHELD_type_to_string(pHanheld->HANDELD_type));

							} while (--hanheld_stack_count);
						

						if (--i_Succes_found == 0)
							break;
					} while (--i_profiles);
				break; }
			}//end SWITCH pf_object_threadwork->TA_Total_HANHELDS__in_VM_Profiles_found_count
			GUI_Switch_LISTS(false, pThis);
			break; }
	}//end count switch

	DH_VM_ThreadWork_Object_found_ClearSet(pf_object_threadwork);
	::EnableWindow(pThis->m_EDIT_SEARCH_object.m_hWnd, true);
	return;
}


void CVMDlg::OnBnClickedCheckSearchOnlySelectedVmProfile()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (m_CHECK_Search_only_selected_vm_profile.GetCheck())
		Is_checl_only_selected_vm_profile = true;
	else
		Is_checl_only_selected_vm_profile = false;

	OnChangeEditSearchObject();
}

void CVMDlg::GUI_Switch_LISTS(__in bool list1, __in LPVOID pParam)
{
	CVMDlg *pThis = reinterpret_cast<CVMDlg*>(pParam);
	switch(list1)
	{
		case true:
		{
			::EnableWindow(pThis->m_list.m_hWnd, true);
			::ShowWindow(pThis->m_list.m_hWnd, SW_SHOW);

			::EnableWindow(pThis->m_SEARCH_list.m_hWnd, false);
			::ShowWindow(pThis->m_SEARCH_list.m_hWnd, SW_HIDE);
		break; }
		case false:
		{
			::EnableWindow(pThis->m_list.m_hWnd, false);
			::ShowWindow(pThis->m_list.m_hWnd, SW_HIDE);

			::EnableWindow(pThis->m_SEARCH_list.m_hWnd, true);
			::ShowWindow(pThis->m_SEARCH_list.m_hWnd, SW_SHOW);
		break; }
	}//end switch(list1)
}
