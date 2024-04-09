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
#include "MapCompareDlg.h"
#include "afxdialogex.h"


// диалоговое окно CMapCompareDlg

IMPLEMENT_DYNAMIC(CMapCompareDlg, CDialogEx)

CMapCompareDlg::CMapCompareDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMapCompareDlg::IDD, pParent)
{
	pAllocated_compare_event_table = NULL;
	pAllocated_COMPARE_BYTES_buffer =  NULL;

	memset(&check_state[0],true,sizeof(check_state));
	check_state[0] = NULL;
	Apply_Filter_state = false;
	Filter_addres_start = 0;
	Filter_addres_end = 0;

	pIdentSectionStacked = (pMAP_IDENTICAL_SECTIONS_STACK)malloc(sizeof(MAP_IDENTICAL_SECTIONS_STACK));
	memset(pIdentSectionStacked, 0, sizeof(MAP_IDENTICAL_SECTIONS_STACK));
}

CMapCompareDlg::~CMapCompareDlg()
{
	if (Load_PE_FILE_2 != NULL)
		{::FreeLibrary(Load_PE_FILE_2);}

	if (Load_PE_FILE_1 != NULL)
		{::FreeLibrary(Load_PE_FILE_1);}

	if (pAllocated_compare_event_table != NULL)
		free(pAllocated_compare_event_table);

	if (pAllocated_COMPARE_BYTES_buffer != NULL)
		free(pAllocated_COMPARE_BYTES_buffer); 
	 
	free(pIdentSectionStacked);
}

void CMapCompareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PE_FILE_1_PATH, m_EDIT_path_PE_File1);
	DDX_Control(pDX, IDC_EDIT_PE_FILE_2_PATH, m_EDIT_path_PE_File2);
	DDX_Control(pDX, IDC_LAMP_FILE_1_LOAD_STATE, m_LAMP_load_state_file1);
	DDX_Control(pDX, IDC_LAMP_FILE_2_LOAD_STATE, m_LAMP_load_state_file2);
	DDX_Control(pDX, IDC_EDIT_PROTECTION_SECTION, m_EDIT_protection_section_name);
	DDX_Control(pDX, IDC_EDIT_DIFF_COUNT, m_EDIT_diff_count);
	DDX_Control(pDX, IDC_LIST_MAP, m_LIST_map);
	//  DDX_Control(pDX, IDC_LIST1, m_list1);
	DDX_Control(pDX, IDC_EDIT_DIFF_COUNT1, m_EDIT_report_diff_count1);
	DDX_Control(pDX, IDC_EDIT_DIFF_COUNT2, m_EDIT_report_diff_count2);
	DDX_Control(pDX, IDC_EDIT_DIFF_COUNT3, m_EDIT_report_diff_count3);
	DDX_Control(pDX, IDC_EDIT_DIFF_COUNT4, m_EDIT_report_diff_count4);
	DDX_Control(pDX, IDC_EDIT_DIFF_COUNT5, m_EDIT_report_diff_count5);
	DDX_Control(pDX, IDC_EDIT_DIFF_COUNT6, m_EDIT_report_diff_count6);
	DDX_Control(pDX, IDC_EDIT_DIFF_COUNT7, m_EDIT_report_diff_count7);
	DDX_Control(pDX, IDC_EDIT_DIFF_COUNT8, m_EDIT_report_diff_count8);
	//  DDX_Control(pDX, IDC_EDIT_DIFF_COUNT_above_9, m_EDIT_report_diff_count9);
	DDX_Control(pDX, IDC_EDIT_DIFF_COUNT_above_9, m_EDIT_report_diff_count_above_9);
	DDX_Control(pDX, IDC_CHECK_DIFF_COUNT1, m_CHECK_diff_count1);
	DDX_Control(pDX, IDC_CHECK_DIFF_COUNT2, m_CHECK_diff_count2);
	DDX_Control(pDX, IDC_CHECK_DIFF_COUNT3, m_CHECK_diff_count3);
	DDX_Control(pDX, IDC_CHECK_DIFF_COUNT4, m_CHECK_diff_count4);
	DDX_Control(pDX, IDC_CHECK_DIFF_COUNT5, m_CHECK_diff_count5);
	DDX_Control(pDX, IDC_CHECK_DIFF_COUNT6, m_CHECK_diff_count6);
	DDX_Control(pDX, IDC_CHECK_DIFF_COUNT7, m_CHECK_diff_count7);
	DDX_Control(pDX, IDC_CHECK_DIFF_COUNT8, m_CHECK_diff_count8);
	DDX_Control(pDX, IDC_CHECK_DIFF_COUNT_ABOVE9, m_CHECK_diff_count_above9);
	DDX_Control(pDX, IDC_EDIT_TRIM_ADDRES, m_EDIT_trim_addres);
	DDX_Control(pDX, IDC_BUTTON_COMPARE_START, m_BUTTON_COMPARE);
	DDX_Control(pDX, IDC_EDIT_IDEN_SECTION_COUNT, m_EDIT_AVIABLE_IDENTI_SECTION_COUNT);
	DDX_Control(pDX, IDC_CHECK_APPLY_FILTER, m_CHECK_Apply_filter);
	DDX_Control(pDX, IDC_EDIT_FILTER_ADDRES_HIDE_START, m_EDIT_Filter_addres_start);
	DDX_Control(pDX, IDC_EDIT_FILTER_ADDRES_HIDE_END, m_EDIT_Filter_addres_end);
	DDX_Control(pDX, IDC_EDIT_DIFF_CLIPPED, m_EDIT_Diff_CLIPPED);

	//  DDX_Control(pDX, IDC_EDIT_DIFF_BYTES1, m_EDIT_diff_bytes_enum1);
	//  DDX_Control(pDX, IDC_EDIT_DIFF_BYTES2, m_EDIT_diff_bytes_enum2);
}


BEGIN_MESSAGE_MAP(CMapCompareDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_PE_FILE_1_PATH, &CMapCompareDlg::OnEnChangeEditPeFile1Path)
	ON_EN_CHANGE(IDC_EDIT_PE_FILE_2_PATH, &CMapCompareDlg::OnEnChangeEditPeFile2Path)
	ON_BN_CLICKED(IDC_BUTTON_COMPARE_START, &CMapCompareDlg::OnBnClickedButtonCompareStart)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MAP, &CMapCompareDlg::OnClickListMap)
//	ON_BN_CLICKED(IDC_CHECK_DIFF_COUNT1, &CMapCompareDlg::OnBnClickedCheckDiffCount1)
//ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_CHECK_DIFF_COUNT1, &CMapCompareDlg::OnHotitemchangeCheckDiffCount1)
//ON_NOTIFY(BCN_DROPDOWN, IDC_CHECK_DIFF_COUNT1, &CMapCompareDlg::OnDropdownCheckDiffCount1)
//ON_NOTIFY(NM_THEMECHANGED, IDC_CHECK_DIFF_COUNT1, &CMapCompareDlg::OnThemechangedCheckDiffCount1)
ON_BN_CLICKED(IDC_CHECK_DIFF_COUNT1, &CMapCompareDlg::OnClickedCheckDiffCount1)
ON_BN_CLICKED(IDC_CHECK_DIFF_COUNT2, &CMapCompareDlg::OnClickedCheckDiffCount2)
ON_BN_CLICKED(IDC_CHECK_DIFF_COUNT3, &CMapCompareDlg::OnClickedCheckDiffCount3)
ON_BN_CLICKED(IDC_CHECK_DIFF_COUNT4, &CMapCompareDlg::OnClickedCheckDiffCount4)
ON_BN_CLICKED(IDC_CHECK_DIFF_COUNT5, &CMapCompareDlg::OnClickedCheckDiffCount5)
ON_BN_CLICKED(IDC_CHECK_DIFF_COUNT6, &CMapCompareDlg::OnClickedCheckDiffCount6)
ON_BN_CLICKED(IDC_CHECK_DIFF_COUNT7, &CMapCompareDlg::OnClickedCheckDiffCount7)
ON_BN_CLICKED(IDC_CHECK_DIFF_COUNT8, &CMapCompareDlg::OnClickedCheckDiffCount8)
ON_BN_CLICKED(IDC_CHECK_DIFF_COUNT_ABOVE9, &CMapCompareDlg::OnClickedCheckDiffCountAbove9)
//ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_MAP, &CMapCompareDlg::OnLvnItemchangedListMap)
ON_NOTIFY(HDN_ITEMCLICK, 0, &CMapCompareDlg::OnItemclickListMap)
ON_WM_DROPFILES()
ON_CBN_EDITCHANGE(IDC_EDIT_PROTECTION_SECTION, &CMapCompareDlg::OnEditchangeEditProtectionSection)
ON_CBN_SELCHANGE(IDC_EDIT_PROTECTION_SECTION, &CMapCompareDlg::OnSelchangeEditProtectionSection)
ON_BN_CLICKED(IDC_CHECK_APPLY_FILTER, &CMapCompareDlg::OnBnClickedCheckApplyFilter)
ON_EN_CHANGE(IDC_EDIT_FILTER_ADDRES_HIDE_START, &CMapCompareDlg::OnChangeEditFilterAddresHideStart)
ON_EN_CHANGE(IDC_EDIT_FILTER_ADDRES_HIDE_END, &CMapCompareDlg::OnChangeEditFilterAddresHideEnd)
END_MESSAGE_MAP()


// обработчики сообщений CMapCompareDlg

BOOL CMapCompareDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Load_PE_FILE_1 = NULL;
	Load_PE_FILE_2 = NULL;

	::SetWindowTextW(m_EDIT_path_PE_File1.m_hWnd, L"C:\\Engeener_ELF\\Drop_1.exe");
	::SetWindowTextW(m_EDIT_path_PE_File2.m_hWnd, L"C:\\Engeener_ELF\\Drop_2.exe");

	DWORD dwStyle=m_LIST_map.GetExtendedStyle();
	m_LIST_map.SetExtendedStyle(dwStyle|LVS_REPORT);

	//check_boxS
	m_CHECK_diff_count1.SetCheck(BST_CHECKED);
	m_CHECK_diff_count2.SetCheck(BST_CHECKED);
	m_CHECK_diff_count3.SetCheck(BST_CHECKED);
	m_CHECK_diff_count4.SetCheck(BST_CHECKED);
	m_CHECK_diff_count5.SetCheck(BST_CHECKED);
	m_CHECK_diff_count6.SetCheck(BST_CHECKED);
	m_CHECK_diff_count7.SetCheck(BST_CHECKED);
	m_CHECK_diff_count8.SetCheck(BST_CHECKED);
	m_CHECK_diff_count_above9.SetCheck(BST_CHECKED);
	file_num_state = false;

	return true;
}

void CMapCompareDlg::OnEnChangeEditPeFile1Path()
{
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Добавьте код элемента управления

	if (::GetWindowTextW(m_EDIT_path_PE_File1.m_hWnd,&buffer[0],MAX_PATH) == 0)
			return;

	if (Load_PE_FILE_1 != NULL)
		{::FreeLibrary(Load_PE_FILE_1);}

	Load_PE_FILE_1 = DH_Load_PE_as_datafile(&buffer[0]);
	

	if(Load_PE_FILE_1 != NULL)
		LAMP_Load_State_PE_FILE_1 = true;
	else
		LAMP_Load_State_PE_FILE_1 = false;

	pProtSectionName = DH_MAP_Determine_identical_protection_section(Load_PE_FILE_1, Load_PE_FILE_2, pIdentSectionStacked);
	
	GUI_UPDATE_NOW();
}


void CMapCompareDlg::OnEnChangeEditPeFile2Path()
{
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Добавьте код элемента управления

	if (::GetWindowTextW(m_EDIT_path_PE_File2.m_hWnd,&buffer[0],MAX_PATH) == 0)
			return;

	if (Load_PE_FILE_2 != NULL)
		{::FreeLibrary(Load_PE_FILE_2);}

	Load_PE_FILE_2 = DH_Load_PE_as_datafile(&buffer[0]);

	if(Load_PE_FILE_2 != NULL)
		LAMP_Load_State_PE_FILE_2 = true;
	else
		LAMP_Load_State_PE_FILE_2 = false;

	pProtSectionName = DH_MAP_Determine_identical_protection_section(Load_PE_FILE_1, Load_PE_FILE_2, pIdentSectionStacked);

	GUI_UPDATE_NOW();
}

void CMapCompareDlg::GUI_UPDATE_NOW(void)
{

	if (LAMP_Load_State_PE_FILE_1 == true)
		m_LAMP_load_state_file1.SetBitmap(LAMP_TRUE);
	else
		m_LAMP_load_state_file1.SetBitmap(LAMP_FALSE);

		if (LAMP_Load_State_PE_FILE_2 == true)
		m_LAMP_load_state_file2.SetBitmap(LAMP_TRUE);
	else
		m_LAMP_load_state_file2.SetBitmap(LAMP_FALSE);

		if ((LAMP_Load_State_PE_FILE_2 != NULL) && (LAMP_Load_State_PE_FILE_1 != NULL) && (pProtSectionName != NULL))
		{
			//OnBnClickedButtonCompareStart();
			::EnableWindow(m_BUTTON_COMPARE.m_hWnd, true);
		}
		else
		{
			::EnableWindow(m_BUTTON_COMPARE.m_hWnd, false);
		}

		m_EDIT_protection_section_name.Clear();
		m_EDIT_protection_section_name.ResetContent();
		if (pProtSectionName != NULL)
		{
			::SetWindowTextA(m_EDIT_protection_section_name.m_hWnd, pProtSectionName);
			if (pIdentSectionStacked->NumOfSection_ALL > 0)
			{
				TCHAR Buff[16];
				SERVICE_COUNT is = pIdentSectionStacked->NumOfSection_ALL;
				TCHAR dif_buff[64];
				itoa_10_fast(is, &dif_buff[0]);
					::SetWindowText(m_EDIT_AVIABLE_IDENTI_SECTION_COUNT.m_hWnd, &dif_buff[0]);

				register PIMAGE_SECTION_HEADER fpSection_IMAGE = pIdentSectionStacked->IDENTICAL_Section_vpp_1[0];
				fpSection_IMAGE--;
				do
				{
					fpSection_IMAGE++;
					::SendMessageA(m_EDIT_protection_section_name.m_hWnd, CB_ADDSTRING, 0, (LPARAM)&fpSection_IMAGE->Name[0]);
				} while (--is);
			}//if(pIdentSectionStacked->NumOfSection_ALL > 0)
			else
				::SetWindowText(m_EDIT_AVIABLE_IDENTI_SECTION_COUNT.m_hWnd, TS_DEF_NA);
		}
}


void CMapCompareDlg::OnBnClickedButtonCompareStart()
{
	// TODO: добавьте свой код обработчика уведомлений
	if ((Load_PE_FILE_2 == NULL) || (Load_PE_FILE_1 == NULL) || (pProtSectionName == NULL))
		return;

	char SectionName[16];
	size_t f = ::GetWindowTextA(m_EDIT_protection_section_name.m_hWnd, &SectionName[0], IMAGE_SIZEOF_SHORT_NAME);

	if (f == NULL) //вариатор
		pProtSectionName = DH_MAP_Determine_identical_protection_section(Load_PE_FILE_1, Load_PE_FILE_2, pIdentSectionStacked);
	else
		pProtSectionName = DH_MAP_Verify_and_set_protection_section(&SectionName[0], pIdentSectionStacked);

	if (pProtSectionName == 0) //фильтр №1
	{
		::SetWindowText(m_EDIT_AVIABLE_IDENTI_SECTION_COUNT.m_hWnd, TS_DEF_NA);
		return;
	}

	//memset(&Compare_info_set,0, sizeof(Compare_info_set));
	if(pAllocated_compare_event_table != NULL)
		free(pAllocated_compare_event_table);

	if(pAllocated_COMPARE_BYTES_buffer != NULL)
		free(pAllocated_COMPARE_BYTES_buffer);

	pAllocated_compare_event_table = NULL;
	pAllocated_COMPARE_BYTES_buffer =  NULL;

		
		pAllocated_compare_event_table = DH_PE_Files_compare(Load_PE_FILE_1,Load_PE_FILE_2,&pAllocated_COMPARE_BYTES_buffer, pIdentSectionStacked);
			

		switch((SERVICE_ADDRES)pAllocated_compare_event_table)
		{
			case MAP_RETURN_COMPARE_EQ_FILES:
				{
				GUI_UPDATE_COMPARE_INFO();
				::MessageBoxExW(NULL,err_eq_files,err_caption,MB_OK|MB_ICONASTERISK,NULL);
				pAllocated_compare_event_table = NULL; 
				return; break; }
			case MAP_RETURN_COMPARE_TOO_MANY_DIFFERECES:
			{
				GUI_UPDATE_COMPARE_INFO();
				::MessageBoxExW(NULL, err_too_many_diff, err_caption, MB_OK | MB_ICONASTERISK, NULL); 
				pAllocated_compare_event_table = NULL;
				return; break;
			}
		}//end ERROR switch
		GUI_UPDATE_COMPARE_INFO();
		GUI_FILL_LIST_filter();
}


void CMapCompareDlg::GUI_UPDATE_COMPARE_INFO(void)
{
	if (pIdentSectionStacked->compare_event_table_count == NULL)
		return;


	TCHAR dig_bufferT[64];
	memset(&dig_bufferT[0],0, sizeof(dig_bufferT));
	
	::SetWindowTextA(m_EDIT_protection_section_name.m_hWnd, (LPCSTR) &pIdentSectionStacked->File_1_protection_section_header.Name[0]);

	itoa_10_fast(pIdentSectionStacked->compare_event_table_count, &dig_bufferT[0]);

	::SetWindowTextW(m_EDIT_diff_count.m_hWnd, &dig_bufferT[0]);

	SERVICE_COUNT i = pIdentSectionStacked->compare_event_table_count;
	register pSET_FILES_COMPARE_EVENT fpCompare_event_table = &pAllocated_compare_event_table[i-1];

	SERVICE_COUNT diff_count1 = 0;
	SERVICE_COUNT diff_count2 = 0;
	SERVICE_COUNT diff_count3 = 0;
	SERVICE_COUNT diff_count4 = 0;
	SERVICE_COUNT diff_count5 = 0;
	SERVICE_COUNT diff_count6 = 0;
	SERVICE_COUNT diff_count7 = 0;
	SERVICE_COUNT diff_count8 = 0;
	SERVICE_COUNT diff_count_above_9 = 0;

	do
	{
		switch(fpCompare_event_table->Diff_Bytes_Count)
		{
			case 0: {break;}
			case 1: {diff_count1++; break;}
			case 2: {diff_count2++; break;}
			case 3: {diff_count3++; break;}
			case 4: {diff_count4++; break;}
			case 5: {diff_count5++; break;}
			case 6: {diff_count6++; break;}
			case 7: {diff_count7++; break;}
			case 8: {diff_count8++; break;}
			default: {diff_count_above_9++; break;}
		}
		fpCompare_event_table--;
	}while(i--);

	if(diff_count1 != NULL) //count 1
	{itoa_10_fast(diff_count1,&dig_bufferT[0]); ::SetWindowTextW(m_EDIT_report_diff_count1.m_hWnd,&dig_bufferT[0]);}
	else
	::SetWindowTextW(m_EDIT_report_diff_count1.m_hWnd,TS_DEF_NULL);

	if(diff_count2 != NULL) //count 2
	{itoa_10_fast(diff_count2,&dig_bufferT[0]); ::SetWindowTextW(m_EDIT_report_diff_count2.m_hWnd,&dig_bufferT[0]);}
	else
	::SetWindowTextW(m_EDIT_report_diff_count2.m_hWnd,TS_DEF_NULL);

	if(diff_count3 != NULL) //count 3
	{itoa_10_fast(diff_count3,&dig_bufferT[0]); ::SetWindowTextW(m_EDIT_report_diff_count3.m_hWnd,&dig_bufferT[0]);}
	else
	::SetWindowTextW(m_EDIT_report_diff_count3.m_hWnd,TS_DEF_NULL);

	if(diff_count4 != NULL) //count 4
	{itoa_10_fast(diff_count4,&dig_bufferT[0]); ::SetWindowTextW(m_EDIT_report_diff_count4.m_hWnd,&dig_bufferT[0]);}
	else
	::SetWindowTextW(m_EDIT_report_diff_count4.m_hWnd,TS_DEF_NULL);

	if(diff_count5 != NULL) //count 5
	{itoa_10_fast(diff_count5,&dig_bufferT[0]); ::SetWindowTextW(m_EDIT_report_diff_count5.m_hWnd,&dig_bufferT[0]);}
	else
	::SetWindowTextW(m_EDIT_report_diff_count5.m_hWnd,TS_DEF_NULL);

	if(diff_count5 != NULL) //count 5
	{itoa_10_fast(diff_count5,&dig_bufferT[0]); ::SetWindowTextW(m_EDIT_report_diff_count5.m_hWnd,&dig_bufferT[0]);}
	else
	::SetWindowTextW(m_EDIT_report_diff_count5.m_hWnd,TS_DEF_NULL);

	if(diff_count1 != NULL) //count 6
	{itoa_10_fast(diff_count6,&dig_bufferT[0]); ::SetWindowTextW(m_EDIT_report_diff_count6.m_hWnd,&dig_bufferT[0]);}
	else
	::SetWindowTextW(m_EDIT_report_diff_count6.m_hWnd,TS_DEF_NULL);

	if(diff_count7 != NULL) //count 7
	{itoa_10_fast(diff_count7,&dig_bufferT[0]); ::SetWindowTextW(m_EDIT_report_diff_count7.m_hWnd,&dig_bufferT[0]);}
	else
	::SetWindowTextW(m_EDIT_report_diff_count7.m_hWnd,TS_DEF_NULL);

	if(diff_count8 != NULL) //count 8
	{itoa_10_fast(diff_count8,&dig_bufferT[0]); ::SetWindowTextW(m_EDIT_report_diff_count8.m_hWnd,&dig_bufferT[0]);}
	else
	::SetWindowTextW(m_EDIT_report_diff_count8.m_hWnd,TS_DEF_NULL);

	if(diff_count_above_9 != NULL) //count >=9
	{itoa_10_fast(diff_count_above_9,&dig_bufferT[0]); ::SetWindowTextW(m_EDIT_report_diff_count_above_9.m_hWnd,&dig_bufferT[0]);}
	else
	::SetWindowTextW(m_EDIT_report_diff_count_above_9.m_hWnd,TS_DEF_NULL);

}

void CMapCompareDlg::GUI_FILL_LIST(void)
{
	if (pIdentSectionStacked->compare_event_table_count == NULL)
		return;

//	m_LIST_map.DeleteAllItems();

	m_LIST_map.DeleteColumn(1);
	m_LIST_map.DeleteColumn(0);

	m_LIST_map.InsertColumn(0,L"Address",0,150);
	m_LIST_map.InsertColumn(1,L"Diff count",0,150);

	TCHAR dig_bufferT[64];
	memset(&dig_bufferT[0],0, sizeof(dig_bufferT));

	SERVICE_COUNT i = pIdentSectionStacked->compare_event_table_count;
	register pSET_FILES_COMPARE_EVENT fpCompare_event_table = &pAllocated_compare_event_table[i-1];

	do
	{
		def_code_Fill_cell;

		/*
		lvi.iSubItem = 1;
		lvi.pszText = (LPTSTR) &dig_bufferT[0];
		 ::SendMessageW(m_LIST_map.m_hWnd, LVM_SETITEMTEXT, 0, (LPARAM)&lvi); //m_LIST_map.SetItemText(0,1,&dig_bufferT[0]);
		*/

		fpCompare_event_table--;
	}while(--i);
}

void CMapCompareDlg::GUI_FILL_LIST_filter(void)
{
	if (pIdentSectionStacked->compare_event_table_count == NULL)
		return;

	m_LIST_map.DeleteAllItems();

	m_LIST_map.DeleteColumn(1);
	m_LIST_map.DeleteColumn(0);

	m_LIST_map.InsertColumn(0,L"Address",0,150);
	m_LIST_map.InsertColumn(1,L"Diff count",0,150);

	
	TCHAR dig_bufferT[64];
	memset(&dig_bufferT[0],0, sizeof(dig_bufferT));

	SERVICE_COUNT i = pIdentSectionStacked->compare_event_table_count;
	register pSET_FILES_COMPARE_EVENT fpCompare_event_table = &pAllocated_compare_event_table[i-1];

	
		if (pIdentSectionStacked->compare_event_table_count > DEFAULT_NUM_OF_COMPARE_EVENTS)
			{::MessageBoxExW(NULL,err_too_many_array,err_caption,MB_OK|MB_ICONEXCLAMATION,NULL);return;}

		//check limits range
		if (Filter_addres_start != 0)
		{
			SERVICE_ADDRES temp_addr;
			if (Filter_addres_end < Filter_addres_start)
			{
				temp_addr = Filter_addres_start;
				Filter_addres_start = Filter_addres_end;
				Filter_addres_end = temp_addr;
				itoa_16_fast(Filter_addres_start, &dig_bufferT[0]);
				::SetWindowText(m_EDIT_Filter_addres_start.m_hWnd, &dig_bufferT[0]);
				itoa_16_fast(Filter_addres_end, &dig_bufferT[0]);
				::SetWindowText(m_EDIT_Filter_addres_end.m_hWnd, &dig_bufferT[0]);
			}
		}
	register SERVICE_COUNT Calc_count;
	size_t Clipped_addr_count = 0;
	fpCompare_event_table++;
	do
	{
		fpCompare_event_table--;
		Calc_count = fpCompare_event_table->Diff_Bytes_Count;
		
		if(Calc_count > MAX_FILTER_CHECK_BOX)
			Calc_count = MAX_FILTER_CHECK_BOX;
		
		if (Apply_Filter_state == true)
		{
			if ((fpCompare_event_table->Start_diff_Address >= Filter_addres_start) && (fpCompare_event_table->Start_diff_Address <= Filter_addres_end))
			{
				Clipped_addr_count++;
				continue;
			}
		}

		switch(Calc_count*check_state[Calc_count])
		{
		case 1:
			{def_code_Fill_cell; break;}
		case 2:
			{def_code_Fill_cell; break;}
		case 3:
			{def_code_Fill_cell; break;}
		case 4:
			{def_code_Fill_cell; break;}
		case 5:
			{def_code_Fill_cell; break;}
		case 6:
			{def_code_Fill_cell; break;}
		case 7:
			{def_code_Fill_cell; break;}
		case 8:
			{def_code_Fill_cell; break;}
		case 9:
			{def_code_Fill_cell; break;}
		}
		
	}while(--i);


	if (Apply_Filter_state == true)
	{
		TCHAR dig_buff[64];
		itoa_10_fast(Clipped_addr_count, &dig_buff[0]);
		::SetWindowText(m_EDIT_Diff_CLIPPED.m_hWnd, &dig_buff[0]);
	}
	else
	{
		::SetWindowText(m_EDIT_Diff_CLIPPED.m_hWnd, TS_DEF_NULL);
	}
	
}


void CMapCompareDlg::OnClickListMap(NMHDR *pNMHDR, LRESULT *pResult)
{

	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
		POSITION sel_pos = m_LIST_map.GetFirstSelectedItemPosition();
	if(sel_pos == (POSITION)-1 || sel_pos == NULL)
		return;

	TCHAR text_data[64];
	sel_pos--; //real item id
	m_LIST_map.GetItemText((int)sel_pos,0, &text_data[0],64);
	::SetWindowTextW(m_EDIT_trim_addres.m_hWnd, &text_data[0]);

	//TCHAR diff_bytes = malloc(sizeof(TCHAR)*MAX_PATH);
	//bytes_to_TCHAR_string();

	*pResult = 0;
}



void CMapCompareDlg::OnClickedCheckDiffCount1()
{
	// TODO: добавьте свой код обработчика уведомлений
	 check_state[1] = m_CHECK_diff_count1.GetCheck();

	 GUI_FILL_LIST_filter();
}


void CMapCompareDlg::OnClickedCheckDiffCount2()
{
	// TODO: добавьте свой код обработчика уведомлений
	 check_state[2] = m_CHECK_diff_count2.GetCheck();

	GUI_FILL_LIST_filter();
}


void CMapCompareDlg::OnClickedCheckDiffCount3()
{
	// TODO: добавьте свой код обработчика уведомлений
	 check_state[3] = m_CHECK_diff_count3.GetCheck();

	GUI_FILL_LIST_filter();
}


void CMapCompareDlg::OnClickedCheckDiffCount4()
{
	// TODO: добавьте свой код обработчика уведомлений

	 check_state[4] = m_CHECK_diff_count4.GetCheck();

	 GUI_FILL_LIST_filter();
}


void CMapCompareDlg::OnClickedCheckDiffCount5()
{
	// TODO: добавьте свой код обработчика уведомлений
	 check_state[5] = m_CHECK_diff_count5.GetCheck();

	 GUI_FILL_LIST_filter();
}


void CMapCompareDlg::OnClickedCheckDiffCount6()
{
	// TODO: добавьте свой код обработчика уведомлений
	 check_state[6] = m_CHECK_diff_count6.GetCheck();

	 GUI_FILL_LIST_filter();
}


void CMapCompareDlg::OnClickedCheckDiffCount7()
{
	// TODO: добавьте свой код обработчика уведомлений
	 check_state[7] = m_CHECK_diff_count7.GetCheck();

	 GUI_FILL_LIST_filter();
}


void CMapCompareDlg::OnClickedCheckDiffCount8()
{
	// TODO: добавьте свой код обработчика уведомлений
	 check_state[8] = m_CHECK_diff_count8.GetCheck();

	 GUI_FILL_LIST_filter();
}


void CMapCompareDlg::OnClickedCheckDiffCountAbove9()
{
	// TODO: добавьте свой код обработчика уведомлений
	 check_state[9] = m_CHECK_diff_count_above9.GetCheck();

	 GUI_FILL_LIST_filter();
}





void CMapCompareDlg::OnItemclickListMap(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	*pResult = 0;
}


void CMapCompareDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного

	CDialogEx::OnDropFiles(hDropInfo);
	WCHAR fName[MAX_PATH];
	size_t filenum = DragQueryFile(hDropInfo, -1, NULL, NULL);
	size_t i = 0;
	do
	{
		DragQueryFile(hDropInfo, i++, &fName[0], MAX_PATH);
		file_num_state = !file_num_state;
		if(file_num_state)
			::SetWindowText(m_EDIT_path_PE_File1.m_hWnd, &fName[0]); 
		else
			::SetWindowText(m_EDIT_path_PE_File2.m_hWnd, &fName[0]);
		

	}while(--filenum);
	DragFinish(hDropInfo);

}


void CMapCompareDlg::OnEditchangeEditProtectionSection()
{
	// TODO: добавьте свой код обработчика уведомлений
	char SectionName[16];
	size_t f = ::GetWindowTextA(m_EDIT_protection_section_name.m_hWnd, &SectionName[0], IMAGE_SIZEOF_SHORT_NAME);

	if (f == NULL) //фильтр №1
		return;

	GUI_SECTION_NAME_WORKSHOP(&SectionName[0]);
}

void CMapCompareDlg::GUI_SECTION_NAME_WORKSHOP(char* NewSectionName)
{
	pProtSectionName = DH_MAP_Determine_identical_protection_section(Load_PE_FILE_1, Load_PE_FILE_2, pIdentSectionStacked);

	if (pProtSectionName == NULL) //фильтр №2
		return;

	register SERVICE_COUNT i = pIdentSectionStacked->NumOfSection_ALL;

	if (i == 0)
		return;

	PIMAGE_SECTION_HEADER pIMAGESECTIONHEAD = &pIdentSectionStacked->Protection_SECTION_1[0];
	pIMAGESECTIONHEAD--;
	do
	{
		pIMAGESECTIONHEAD++;

		if (strcmp(&NewSectionName[0], (const char*)&pIMAGESECTIONHEAD->Name[0]) != NULL)
			continue;
		GUI_UPDATE_NOW();
		return;
	} while (--i);
}


void CMapCompareDlg::OnSelchangeEditProtectionSection()
{
	// TODO: добавьте свой код обработчика уведомлений
	/*
	char SectionName[16];
	long f = ::SendMessage(m_hWnd, CB_GETCURSEL, 0, 0);
	::SendMessageA(m_hWnd, CB_GETLBTEXT, f , (LPARAM)&SectionName[0]);

	GUI_SECTION_NAME_WORKSHOP(&SectionName[0]);
	*/
}


void CMapCompareDlg::OnBnClickedCheckApplyFilter()
{
	// TODO: добавьте свой код обработчика уведомлений
	Apply_Filter_state = m_CHECK_Apply_filter.GetCheck();

	if (Apply_Filter_state == false)
	{
		::EnableWindow(m_EDIT_Filter_addres_start.m_hWnd, false);
		::EnableWindow(m_EDIT_Filter_addres_end.m_hWnd, false);
		return;
	}
	else
	{
		::EnableWindow(m_EDIT_Filter_addres_start.m_hWnd, true);
		::EnableWindow(m_EDIT_Filter_addres_end.m_hWnd, true);
	}
	

	TCHAR dig_buff[64];
	::GetWindowText(m_EDIT_Filter_addres_start.m_hWnd, &dig_buff[0], 64);
	Filter_addres_start = _wcstoui64(&dig_buff[0], NULL, 16);
	::GetWindowText(m_EDIT_Filter_addres_end.m_hWnd, &dig_buff[0], 64);
	Filter_addres_end = _wcstoui64(&dig_buff[0], NULL, 16);
}


void CMapCompareDlg::OnChangeEditFilterAddresHideStart()
{
	TCHAR dig_buff[64];
	::GetWindowText(m_EDIT_Filter_addres_start.m_hWnd, &dig_buff[0], 64);
	Filter_addres_start = _wcstoui64(&dig_buff[0], NULL, 16);
}


void CMapCompareDlg::OnChangeEditFilterAddresHideEnd()
{
	TCHAR dig_buff[64];
	::GetWindowText(m_EDIT_Filter_addres_end.m_hWnd, &dig_buff[0], 64);
	Filter_addres_end = _wcstoui64(&dig_buff[0], NULL, 16);
}
