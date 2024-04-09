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
#include "PatchDlg.h"
#include "afxdialogex.h"
#include "ENGINE/ENGINE_MAP_operations.h"
TCHAR Num[10] = {L'2'};
// диалоговое окно CPatchDlg

IMPLEMENT_DYNAMIC(CPatchDlg, CDialogEx)

CPatchDlg::CPatchDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPatchDlg::IDD, pParent)
{
	pActive_Support_region = NULL;
	hAntiRewriteSentinel = NULL;
	pEnumPatchSnapshot = (pPATCH_SNAPSHOT) ::VirtualAlloc(NULL, sizeof(PATCH_SNAPSHOT) * MAX_PATCH_SNAPSHOTS, MEM_COMMIT, PAGE_READWRITE);
	fpEnumPatchSnapshot = NULL;
}

CPatchDlg::~CPatchDlg()
{
	//DH_MAP_Close_patch_region_config(pActive_Support_region);
	codedef_check_term_threadif(hAntiRewriteSentinel);
	::VirtualFree(pEnumPatchSnapshot, sizeof(PATCH_SNAPSHOT) * MAX_PATCH_SNAPSHOTS, MEM_RELEASE);
}

void CPatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_NOT_APPLIED_LIST, m_na_list);
	//  DDX_Control(pDX, IDC_NOT_APPLIED_LIST2, m_online_list);
	DDX_Control(pDX, IDC_NOT_APPLIED_LIST, m_list_na);
	DDX_Control(pDX, IDC_ONLINE_APPLIED_LIST, m_list_online);
	DDX_Control(pDX, IDC_CHECK_AUTO_ALLOC_HOOK_FILTER, m_CHECK_auto_alloc_hook_and_filter_region);
	DDX_Control(pDX, IDC_CHECK_AUTO_ALLOC_ORIGINAL_VMP, m_CHECK_auto_alloc_original_vm_region);
	DDX_Control(pDX, IDC_CHECK_AUTO_ALLOC_PATCH_MAP, m_CHECK_auto_alloc_patch_map_region);
	DDX_Control(pDX, IDC_LAMP_PATCH_MAP_REGION_STATE, m_LAMP_patch_map_region);
	DDX_Control(pDX, IDC_LAMP_VMP_HOO_AND_FILTER_REGION_STATE, m_LAMP_hook_and_filter_region);
	DDX_Control(pDX, IDC_LAMP_VMP_ORIGINAL_COPY_REGION_STATE, m_LAMP_original_copy_region);
	DDX_Control(pDX, IDC_LAMP_VMP_SECTION_STATE, m_LAMP_vmp_section_determine);
	DDX_Control(pDX, IDC_EDIT_VMP_HOOK_AND_FILTER_ADDRES, m_EDIT_hook_and_filter_addres_start);
	DDX_Control(pDX, IDC_EDIT_VMP_HOOK_AND_FILTER_SIZE, m_EDIT_hook_and_filter_addres_end);
	DDX_Control(pDX, IDC_EDIT_VMP_ORIGINAL_COPY_ADDRES, m_EDIT_original_copy_vmp_addres_start);
	DDX_Control(pDX, IDC_EDIT_VMP_ORIGINAL_COPY_SIZE, m_EDIT_original_copy_vmp_addres_end);
	DDX_Control(pDX, IDC_EDIT_VMP_PATCH_MAP_ADDRES, m_EDIT_patch_map_addres_start);
	DDX_Control(pDX, IDC_EDIT_VMP_PATCH_MAP_SIZE, m_EDIT_patch_map_addres_end);
	DDX_Control(pDX, IDC_EDIT_VMP_SECTION_NAME, m_EDIT_vmp_section_name);
	DDX_Control(pDX, IDC_EDIT_VMP_SECTION_ADDRES, m_EDIT_VMP_SECTION_Addres_start);
	DDX_Control(pDX, IDC_EDIT_VMP_SECTION_SIZE, m_EDIT_VMP_SECTION_size);
	DDX_Control(pDX, IDC_APPLY_NA_STACK_BUTTON, m_BUTTON_APPLY_NA_STACK);
	DDX_Control(pDX, IDC_EDIT_SUCCESS_APLIED_COUNT, m_EDIT_SUCCESS_NA_APPLY_COUNT);
	//  DDX_Control(pDX, IDC_CHECK_ANTIREWRITE_SENTINEL, m_check_antirewrite_sentinel);
	DDX_Control(pDX, IDC_CHECK_ANTIREWRITE_SENTINEL, m_CHECK_antirewrite_sentinel);
	DDX_Control(pDX, IDC_ANTIREWRITE_LIST, m_LIST_antirewrite);
	//  DDX_Control(pDX, IDC_BUTTON_SET_ALLOC_AREA, m_EDIT_antirewrite_sentinel_count);
	DDX_Control(pDX, IDC_EDIT_REWRITE_Sentinel, m_EDIT_antirewrite_sentinel_count);
	DDX_Control(pDX, IDC_EDIT_Q_Online_count, m_EDIT_Q_Online_patches_count);
	DDX_Control(pDX, IDC_EDIT_Q_NA_count, m_EDIT_Q_NA_Patches_count);
	DDX_Control(pDX, IDC_EDIT_Q_REWRITE_Addreses_count, m_EDIT_Q_Rewrite_addreses_count);
}


BEGIN_MESSAGE_MAP(CPatchDlg, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_NOT_APPLIED_LIST, &CPatchDlg::OnLvnItemchangedNotAppliedList)
	ON_BN_CLICKED(IDC_APPLY_NA_STACK_BUTTON, &CPatchDlg::OnBnClickedApplyNaStackButton)
	ON_BN_CLICKED(IDC_CHECK_ANTIREWRITE_SENTINEL, &CPatchDlg::OnBnClickedCheckAntirewriteSentinel)
END_MESSAGE_MAP()


// обработчики сообщений CPatchDlg

DH_ENUM_FILES_of_PROFILES CPatchDlg::Not_applied_patch_file_lists[MAX_CONTAINTER_FILES];
DH_ENUM_FILES_of_PROFILES CPatchDlg::ONLINE_patch_file_lists[MAX_CONTAINTER_FILES];

bool CPatchDlg::Fill_Not_applied_PATCH_profiles_list(void)
{
	register size_t i = DH_Get_Current_hunt_profile_ALL_FILE_CONTAINERS(PROFILE_TYPE_PATCH__NOT_APPLIED_FLAG, &Not_applied_patch_file_lists[0]);
	if(i==0)
		{return false;}
	m_list_na.DeleteAllItems();
	
	Fill_PATCH_enumerator(&Not_applied_patch_file_lists[0], m_list_na.m_hWnd, i); //call filler for list_na
	
	return true;
}

bool CPatchDlg::Fill_ONLINE_PATCH_profiles_list(void)
{
	
	return false;
}

__inline bool CPatchDlg::Fill_PATCH_enumerator(pDH_ENUM_FILES_of_PROFILES EnumFilesArray, HWND ListHandle, size_t i_count)
{
 register pPATCH_INFO_STACK	Selected_PATCH_Stack = NULL;
 size_t reta;
 TCHAR loc_dig_buffer[40];
 memset(&loc_dig_buffer[0], 0 ,sizeof(loc_dig_buffer));
 wcscat(&loc_dig_buffer[0], _T("0x"));
 
 	do
	{
		i_count--;
		Selected_PATCH_Stack = (pPATCH_INFO_STACK)DH_Read_Current_hunt_profile_CONTAINER(PROFILE_TYPE_PATCH__NOT_APPLIED_FLAG, &EnumFilesArray[i_count].FileName[0],PROFILE_READ_ALL,(PULONG)&reta);
		
		{ //start fill list
		m_list_na.InsertItem(0,&Selected_PATCH_Stack->name[0],NULL); //FILENAME
		
		if(Selected_PATCH_Stack->is_new_added) //is new?
			m_list_na.SetItemText(0,1,_T("yes"));

		m_list_na.SetItemText(0,2, DH_VM_PATCH_OWNERS_type_to_string(Selected_PATCH_Stack->owner_as_profile__of_HELD_GROUP));
			
		m_list_na.SetItemText(0,3, &Selected_PATCH_Stack->owner_name[0]); //OWNER FILENAME

		m_list_na.SetItemText(0,4, DH_VM_HANHELD_type_to_string(Selected_PATCH_Stack->type_of_target_addres));

		m_list_na.SetItemText(0,5, DH_PATCH_ERROR_type_to_string(Selected_PATCH_Stack->last_error_status));
	
		if(Selected_PATCH_Stack->parent_subroutine_address != NULL)
			{
				itoa_16_fast(Selected_PATCH_Stack->parent_subroutine_address, &loc_dig_buffer[DIG_BUFFER_0X_SHR]);
				m_list_na.SetItemText(0,5, &loc_dig_buffer[0]); //S U B R O U T I N E
			}

		}//end of fill list
	free(Selected_PATCH_Stack);
	}
	while(i_count != 0);

	return true;
}

__inline void CPatchDlg::Fill_SUPPORT_REGION_AREA(void)
{
	TCHAR dig_buuffer[64];
	
	::SetWindowText(m_EDIT_vmp_section_name.m_hWnd, &pActive_Support_region->VMP_SectionName[0]); //vmp section name

	itoa_16_fast(pActive_Support_region->VMP_Section_start_address,&dig_buuffer[0]);
	::SetWindowText(m_EDIT_VMP_SECTION_Addres_start.m_hWnd, &dig_buuffer[0]); //vmp section address start

	itoa_16_fast(pActive_Support_region->VMP_Section_size,&dig_buuffer[0]);
	::SetWindowText(m_EDIT_VMP_SECTION_size.m_hWnd, &dig_buuffer[0]); //vmp section size

	itoa_16_fast(pActive_Support_region->Region_VMP_original_copy_start,&dig_buuffer[0]);
	::SetWindowText(m_EDIT_original_copy_vmp_addres_start.m_hWnd, &dig_buuffer[0]); //vmp original copy start

	itoa_16_fast(pActive_Support_region->Region_VMP_original_copy_end,&dig_buuffer[0]);
	::SetWindowText(m_EDIT_original_copy_vmp_addres_end.m_hWnd, &dig_buuffer[0]); //vmp original copy end

	itoa_16_fast(pActive_Support_region->Region_VMP_patch_map_start,&dig_buuffer[0]);
	::SetWindowText(m_EDIT_patch_map_addres_start.m_hWnd, &dig_buuffer[0]); //patch map start

	itoa_16_fast(pActive_Support_region->Region_VMP_patch_map_end,&dig_buuffer[0]);
	::SetWindowText(m_EDIT_patch_map_addres_end.m_hWnd, &dig_buuffer[0]); //patch map start

	itoa_16_fast(pActive_Support_region->Region_VMP_hook_and_filter_start,&dig_buuffer[0]);
	::SetWindowText(m_EDIT_hook_and_filter_addres_start.m_hWnd, &dig_buuffer[0]); //h&f start

	itoa_16_fast(pActive_Support_region->Region_VMP_hook_and_filter_end,&dig_buuffer[0]);
	::SetWindowText(m_EDIT_hook_and_filter_addres_end.m_hWnd, &dig_buuffer[0]); //h&f start

	if(pActive_Support_region->is_VMP_section_validate)
		m_LAMP_vmp_section_determine.SetBitmap(LAMP_TRUE);
	else
		m_LAMP_vmp_section_determine.SetBitmap(LAMP_FALSE);

	if(pActive_Support_region->is_Region_VMP_original_copy_validate)
		m_LAMP_original_copy_region.SetBitmap(LAMP_TRUE);
	else
		m_LAMP_original_copy_region.SetBitmap(LAMP_FALSE);

	if(pActive_Support_region->is_Region_VMP_patch_map_validate)
		m_LAMP_patch_map_region.SetBitmap(LAMP_TRUE);
	else
		m_LAMP_patch_map_region.SetBitmap(LAMP_FALSE);

	if(pActive_Support_region->is_Region_VMP_hook_and_filter_validate)
		m_LAMP_hook_and_filter_region.SetBitmap(LAMP_TRUE);
	else
		m_LAMP_hook_and_filter_region.SetBitmap(LAMP_FALSE);
}

BOOL CPatchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ƒобавить дополнительную инициализацию
	//not applieds list //
	m_list_na.InsertColumn(0,_T("File"),0,150);
	m_list_na.InsertColumn(1,_T("Is new?"),0,50);
	m_list_na.InsertColumn(2,_T("Owner type"),0,100);
	m_list_na.InsertColumn(3,_T("Owner"),0,60);
	m_list_na.InsertColumn(4,_T("Asm type"),0,120);
	m_list_na.InsertColumn(5,_T("ERROR"),0,120);

	//online list//
	m_list_online.InsertColumn(0,_T("File"),0,150);
	m_list_online.InsertColumn(1,_T("Verify"),0,90);
	m_list_online.InsertColumn(2,_T("Patch method"),0,100);
	m_list_online.InsertColumn(3,_T("Owner filename"),0,150);
	m_list_online.InsertColumn(4,_T("Asm type"),0,120);

	//antirewrite//
	m_LIST_antirewrite.InsertColumn(0,_T("Addres"),0,90);
	m_LIST_antirewrite.InsertColumn(1,_T("Bytes"),0,50);
	m_LIST_antirewrite.InsertColumn(2,_T("R-Type"),0,90);
	m_LIST_antirewrite.InsertColumn(3,_T("Owner filename"),0,100);
	
	

	Fill_Not_applied_PATCH_profiles_list();
	Fill_ONLINE_PATCH_profiles_list();
	Fill_Partially_Q_report();
	
	pActive_Support_region = DH_MAP_Release_patch_region_config();//Load_Region_support_area_config();
	if(pActive_Support_region != NULL)
		Fill_SUPPORT_REGION_AREA();

	PARSE_ERROR Error_block;
 	DH_MAP_Do_INIT_Arbitary_FILTERs(&Num[0], VM_TYPE_VMP2);

	GUI_UPDATE_AFTER_FILL_PROFILE();

	return TRUE;  // return TRUE unless you set the focus to a control
	// »сключение: страница свойств OCX должна возвращать значение FALSE
}

void CPatchDlg::GUI_UPDATE_AFTER_FILL_PROFILE(void)
{
	if(pActive_Support_region == NULL)
		{::EnableWindow(m_BUTTON_APPLY_NA_STACK.m_hWnd,false); return;}

	if(pActive_Support_region->is_Region_VMP_patch_map_auto_alloc == true)
		m_CHECK_auto_alloc_patch_map_region.SetCheck(BST_CHECKED);
	else
		m_CHECK_auto_alloc_patch_map_region.SetCheck(BST_UNCHECKED);

	if(pActive_Support_region->is_Region_VMP_original_copy_auto_alloc == true)
		m_CHECK_auto_alloc_original_vm_region.SetCheck(BST_CHECKED);
	else
		m_CHECK_auto_alloc_original_vm_region.SetCheck(BST_UNCHECKED);

	if(pActive_Support_region->is_Region_VMP_hook_and_filter_auto_alloc == true)
		m_CHECK_auto_alloc_hook_and_filter_region.SetCheck(BST_CHECKED);
	else
		m_CHECK_auto_alloc_hook_and_filter_region.SetCheck(BST_UNCHECKED);

	if((pActive_Support_region->VMP_Section_start_address != NULL) && (pActive_Support_region->Region_VMP_patch_map_start != NULL) && (pActive_Support_region->Region_VMP_original_copy_start != NULL) && (pActive_Support_region->Region_VMP_hook_and_filter_start != NULL))
		::EnableWindow(m_BUTTON_APPLY_NA_STACK.m_hWnd,true);
	else
		::EnableWindow(m_BUTTON_APPLY_NA_STACK.m_hWnd,false);
}

void CPatchDlg::OnLvnItemchangedNotAppliedList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	*pResult = 0;
}



void CPatchDlg::OnBnClickedApplyNaStackButton()
{
	// TODO: добавьте свой код обработчика уведомлений
	TCHAR dig_buuffer[64];
	register size_t Success_NA_count = NULL;
	Success_NA_count = DH_MAP_Do_APPLY_NA_STACK(pActive_Support_region);

	itoa_10_fast(Success_NA_count,&dig_buuffer[0]);
	::SetWindowText(m_EDIT_SUCCESS_NA_APPLY_COUNT.m_hWnd, &dig_buuffer[0]); //NA SUCCESS count

	//update file lists
	Fill_Not_applied_PATCH_profiles_list();
}

bool CPatchDlg::Load_Region_support_area_config(void)
{
	ULONG reta = 0;
}

void CPatchDlg::OnBnClickedCheckAntirewriteSentinel()
{
	// TODO: добавьте свой код обработчика уведомлений
	 if (m_CHECK_antirewrite_sentinel.GetCheck() == BST_CHECKED)
	{
	codedef_check_term_threadif(hAntiRewriteSentinel);
	hAntiRewriteSentinel = ::CreateRemoteThreadEx(DH_Current_process_fast_handle,NULL,NULL, (LPTHREAD_START_ROUTINE)GUI_UPDATE_Sentinel_requests,this,NULL,NULL, NULL);
	}
	else
	{
		if (m_CHECK_antirewrite_sentinel.GetState() == BST_UNCHECKED)
			codedef_check_term_threadif(hAntiRewriteSentinel);
	}
}

DWORD WINAPI CPatchDlg::GUI_UPDATE_Sentinel_requests(LPVOID Param)
{
	CPatchDlg *pThis = reinterpret_cast<CPatchDlg*>(Param);

	register size_t snap_i = DH_MAP_Take_patch_list_snapshot(pThis->pEnumPatchSnapshot);

		if(snap_i == NULL)
			{pThis->m_CHECK_antirewrite_sentinel.SetCheck(BST_UNCHECKED); ::SetWindowText(pThis->m_EDIT_antirewrite_sentinel_count.m_hWnd, TS_DEF_NULL); return NULL;}

		TCHAR buf[64];

			itoa_10_fast(snap_i, &buf[0]);
			::SetWindowText(pThis->m_EDIT_antirewrite_sentinel_count.m_hWnd, &buf[0]);
	
		register pPATCH_SNAPSHOT pRetval_first = NULL;
		register signed long immexitstate=true;
		SERVICE_COUNT ret_how_count_to_end; //сколько осталось в списке до конца от первого попашегос€
		SERVICE_COUNT ret_how_snap_whith_rewrites; //сколько всего перезаписей
		pPATCH_SNAPSHOT OLD_ret_first = NULL; //предыдуща€ ссылка
		SERVICE_COUNT OLD_ret_how_snap_whith_rewrites = 0; //предыдущих сколько всего перезаписей
		do
		{
			ret_how_count_to_end = snap_i; //set full cycle
			ret_how_snap_whith_rewrites = 0;
			pRetval_first = DH_MAP_AntiRewrite_Sentinel(pThis->pEnumPatchSnapshot, &ret_how_count_to_end, &ret_how_snap_whith_rewrites);
			if (pRetval_first == 0)
				{SleepEx(5, false); continue;}

			if (pRetval_first == (pPATCH_SNAPSHOT)-1)
				immexitstate = false; //exit cycle == terminate sentinel thread

			if(pThis->hAntiRewriteSentinel == NULL)
					immexitstate = false; //imm exit

				//--------- found first difference! --------------- //
				if (OLD_ret_first != pRetval_first || OLD_ret_how_snap_whith_rewrites != ret_how_snap_whith_rewrites)
				{
					pThis->m_LIST_antirewrite.DeleteAllItems(); 
					pThis->Fill_AntiRewrite_list_report(Param, pRetval_first, ret_how_count_to_end, ret_how_snap_whith_rewrites);

					itoa_10_fast(ret_how_snap_whith_rewrites, &buf[0]);
					::SetWindowText(pThis->m_EDIT_Q_Rewrite_addreses_count.m_hWnd, &buf[0]);

					OLD_ret_first = pRetval_first; OLD_ret_how_snap_whith_rewrites = ret_how_snap_whith_rewrites; //locked!
				}//end if(OLD_ret_first != pRetval_first && OLD_ret_how_snap_whith_rewrites != ret_how_snap_whith_rewrites)
		
			::SleepEx(1,false);
		}while(immexitstate==true);

return snap_i;
}

void CPatchDlg::Fill_Partially_Q_report(void)
{
	TCHAR dig_buff[64];

	register size_t i = DH_Get_Current_hunt_profile_ALL_FILE_CONTAINERS(PROFILE_TYPE_PATCH_ONLINE, &ONLINE_patch_file_lists[0]);
	itoa_10_fast(i, &dig_buff[0]);
	::SetWindowText(m_EDIT_Q_Online_patches_count.m_hWnd, &dig_buff[0]);


	i = DH_Get_Current_hunt_profile_ALL_FILE_CONTAINERS(PROFILE_TYPE_PATCH__NOT_APPLIED_FLAG, &Not_applied_patch_file_lists[0]);
	itoa_10_fast(i, &dig_buff[0]);
	::SetWindowText(m_EDIT_Q_NA_Patches_count.m_hWnd, &dig_buff[0]);
}

void __fastcall CPatchDlg::Fill_AntiRewrite_list_report(__in LPVOID Param, __in pPATCH_SNAPSHOT pFirst_snap, __in size_t how_count_to_end, __in size_t how_count_all_snap_with_rewrites)
{
	CPatchDlg *pThis = reinterpret_cast<CPatchDlg*>(Param);
	TCHAR dig_buff[64];
	register SERVICE_COUNT* pRewriteCOUNT = &pFirst_snap->COUNT_Rewrite_struct;
	//pRewriteCOUNT-=sizeof(PATCH_SNAPSHOT);

				pFirst_snap--;
				do
				{
					pFirst_snap++;
					pRewriteCOUNT = &pFirst_snap->COUNT_Rewrite_struct; //pRewriteCOUNT+=sizeof(PATCH_SNAPSHOT);
						
					if(*pRewriteCOUNT == NULL)
							continue; //если в стрктуре REWRITE пусто
					if (how_count_all_snap_with_rewrites-- == NULL) //по оставшимс€ снапам с перезапис€ми
							break; //если закончились все снапы с заполненной REWRITE

					register SERVICE_COUNT RewriteCOUNT = *pRewriteCOUNT;
							register SERVICE_ADDRES* fpRewriteAddresPos = &pFirst_snap->Rewrite_addres[0]; //start point address
							register SERVICE_COUNT* fpRewriteAddresBytes = &pFirst_snap->Bytes_rewrite[0]; //bytes rewrite
							register SERVICE_TYPE* fpRewriteAddresAbilites = &pFirst_snap->Detected_rewrite_abilites[0]; //rewrite abilites
						do
						{

								itoa_16_fast(*fpRewriteAddresPos++, &dig_buff[0]);
								pThis->m_LIST_antirewrite.InsertItem(0, &dig_buff[0]); //addres

								itoa_16_fast(*fpRewriteAddresBytes++, &dig_buff[0]);
								pThis->m_LIST_antirewrite.SetItemText(0,1,  &dig_buff[0]); //bytes

								//itoa_16_fast(*fpRewriteAddresAbilites++, &dig_buff[0]);
								pThis->m_LIST_antirewrite.SetItemText(0,2,  DH_PATCH_ReWRITE_type_to_string(*fpRewriteAddresAbilites++)); //abilites

								pThis->m_LIST_antirewrite.SetItemText(0,3,  &pFirst_snap->owner_name[0]); //owner
						}while(--RewriteCOUNT);
					
					
				}while(--how_count_to_end);  //оставшейс€ элементы с первого попавшегос€
}
