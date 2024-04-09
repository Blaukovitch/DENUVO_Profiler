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
#include "СDLC_Lic_Files.h"
#include "afxdialogex.h"



// диалоговое окно СDLC_Lic_Files

IMPLEMENT_DYNAMIC(СDLC_Lic_Files, CDialogEx)

СDLC_Lic_Files::СDLC_Lic_Files(CWnd* pParent /*=NULL*/)
	: CDialogEx(СDLC_Lic_Files::IDD, pParent)
{
	pFileinfoStack = NULL;
	CurrentFileDecodeInfo = NULL;
	pCurSelDLC_Info = NULL;
}

СDLC_Lic_Files::~СDLC_Lic_Files()
{
	if(pFileinfoStack)
		free(pFileinfoStack);

	if(CurrentFileDecodeInfo)
		free(CurrentFileDecodeInfo);

	if (pCurSelDLC_Info != NULL)
		free(pCurSelDLC_Info);
}

void СDLC_Lic_Files::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILELIST1, m_FILELIST);
	//  DDX_Control(pDX, IDC_EDIT_SECRET_TABLE, m_EDIT_SECRET_TABLE);
	DDX_Control(pDX, IDC_EDIT_VAVLE_STEAM_LIC_COUNT2, m_EDIT_STEAM_LIC_COUNT);
	DDX_Control(pDX, IDC_EDIT_EA_ORIGIN_LIC_COUNT, m_EDIT_ORIGIN_LIC_COUNT);
	DDX_Control(pDX, IDC_EDIT_XML_CONTAINTER, m_EDIT_XML_CONTAINER);
	DDX_Control(pDX, IDC_X_TABLE_LIST1, m_LIST_X_TABLE);
	DDX_Control(pDX, IDC_EDIT_DWORDS_COUNT, m_EDIT_XTABLE_DWORD_count);
	DDX_Control(pDX, IDC_EDIT_ENTROPY, m_EDIT_ENTROPY);
}


BEGIN_MESSAGE_MAP(СDLC_Lic_Files, CDialogEx)
	ON_LBN_SELCHANGE(IDC_FILELIST1, &СDLC_Lic_Files::OnSelchangeFilelist1)
	ON_LBN_SELCHANGE(IDC_FILELIST1, &СDLC_Lic_Files::OnLbnSelchangeFilelist1)
END_MESSAGE_MAP()


// обработчики сообщений СDLC_Lic_Files


BOOL СDLC_Lic_Files::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// TODO:  Добавить дополнительную инициализацию
	Fill_DLC_licence_list();

	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}


void СDLC_Lic_Files::Fill_DLC_licence_list(void)
{
	TCHAR DigBuf[64];
	pFileinfoStack = DH_Licence_Get_DLC_licence_filestack(true, true);
	if(!pFileinfoStack)
		return;

	register size_t i = pFileinfoStack->NumofFoundFiles;
	if(!i)
		return;

	
	register pLIC_DLC_FILE fpFileInfoUnit = &pFileinfoStack->FileUnit[0]; //fp
	::SendMessage(m_FILELIST.m_hWnd, LB_RESETCONTENT, 0, 0); //m_FILELIST.ResetContent();
	do
	{
		::SendMessage(m_FILELIST.m_hWnd, LB_ADDSTRING, 0, (LPARAM)&fpFileInfoUnit->FileFIND_DAT.cFileName[0]); //m_FILELIST.AddString(&fpFileInfoUnit->FileFIND_DAT.cFileName[0]);
		fpFileInfoUnit++;
	}while(--i);

	itoa_10_fast(pFileinfoStack->EA_ORIGIN_licence, &DigBuf[0]);
	::SetWindowText(m_EDIT_ORIGIN_LIC_COUNT.m_hWnd, &DigBuf[0]);
	itoa_10_fast(pFileinfoStack->Vavle_Steam_licence, &DigBuf[0]);
	::SetWindowText(m_EDIT_STEAM_LIC_COUNT.m_hWnd, &DigBuf[0]);
	return;
}

void СDLC_Lic_Files::OnSelchangeFilelist1()
{
	// TODO: добавьте свой код обработчика уведомлений
	register size_t i = m_FILELIST.GetCurSel();
		if(i == CB_ERR)
			return;

		if(pFileinfoStack->NumofFoundFiles == NULL)
			return;

		if(pCurSelDLC_Info != NULL)
			free(pCurSelDLC_Info);

	m_LIST_X_TABLE.ResetContent();
	::SetWindowText(m_EDIT_ENTROPY.m_hWnd,  TS_DEF_NULL);
	::SetWindowText(m_EDIT_XTABLE_DWORD_count.m_hWnd, TS_DEF_NULL);

	TCHAR dig_buf[80];
	WCHAR GetText[MAX_PATH];
	
	m_FILELIST.GetText(i, &GetText[0]);
	i =  pFileinfoStack->NumofFoundFiles;
	register pLIC_DLC_FILE fpFileInfoUnit = &pFileinfoStack->FileUnit[0]; //fp
	fpFileInfoUnit--;
		do
		{
			fpFileInfoUnit++;
			if (wcsicmp(&GetText[0], &fpFileInfoUnit->FileFIND_DAT.cFileName[0]))
				continue;

				pCurSelDLC_Info = DH_Licence_Decode_Licence_data_from_file(&fpFileInfoUnit->FileFIND_DAT.cFileName[0], fpFileInfoUnit->DLC_TYPE);
				if (!pCurSelDLC_Info)
				{
					::SetWindowText(m_EDIT_ENTROPY.m_hWnd, TS_DEF_NULL);
					::SetWindowText(m_EDIT_XTABLE_DWORD_count.m_hWnd, TS_DEF_NULL);
					::SetWindowText(m_EDIT_XML_CONTAINER.m_hWnd, TS_DEF_NA);	
					break;
				}

				::SetWindowTextA(m_EDIT_XML_CONTAINER.m_hWnd,  &pCurSelDLC_Info->XML_DATA[0]);

				if(pCurSelDLC_Info->X_TABLE_DWORDs_count == NULL)
						break;
				register size_t j = pCurSelDLC_Info->X_TABLE_DWORDs_count;
				register LPDWORD fpXTABLE_DWORD = &pCurSelDLC_Info->X_TABLE[1]; //skip table info DWORD
				itoa_10_fast(j, &dig_buf[0]);
				::SetWindowText(m_EDIT_XTABLE_DWORD_count.m_hWnd, &dig_buf[0]);
					if(j > 1000) //too loooooong table
						break;

		
					swprintf(&dig_buf[0],L"%.7lf",pCurSelDLC_Info->X_TABLE_entropy);
					::SetWindowText(m_EDIT_ENTROPY.m_hWnd,  &dig_buf[0]);
				do
				{
					itoa_16_fast(*fpXTABLE_DWORD++, &dig_buf[0]);
					::SendMessage(m_LIST_X_TABLE.m_hWnd, LB_ADDSTRING, 0, (LPARAM)&dig_buf[0]); //m_LIST_X_TABLE.AddString(&dig_buf[0]);
				}while(--j);
				break;
		}while(--i); //end cycle
}


void СDLC_Lic_Files::OnLbnSelchangeFilelist1()
{
	// TODO: добавьте свой код обработчика уведомлений
}
