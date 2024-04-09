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
#include "ImportDlg.h"
#include "afxdialogex.h"


// диалоговое окно CImportDlg

IMPLEMENT_DYNAMIC(CImportDlg, CDialogEx)

CImportDlg::CImportDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImportDlg::IDD, pParent)
{

}

CImportDlg::~CImportDlg()
{
}

void CImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CImportDlg, CDialogEx)
	ON_BN_CLICKED(IDC_LOAD_FROM_DUMP_FILE, &CImportDlg::OnBnClickedLoadFromDumpFile)
END_MESSAGE_MAP()


// обработчики сообщений CImportDlg


void CImportDlg::OnBnClickedLoadFromDumpFile()
{
	// TODO: добавьте свой код обработчика уведомлений
	CFileDialog fileDialog(TRUE,NULL,L"*.TXT",NULL,L"debugger import log file (*.txt)|*.txt|");


	int result = fileDialog.DoModal();
	if (result==IDOK)
	{
	POSITION ps=fileDialog.GetStartPosition();		// получить начальную позицию
	CString inport_file_path = fileDialog.GetNextPathName(ps);// пока есть выбранные файлы

	}
	
	
}
