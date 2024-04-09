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
#include "VM_DLG_NewWave.h"
#include "afxdialogex.h"


// диалоговое окно CVM_DLG_NewWave

IMPLEMENT_DYNAMIC(CVM_DLG_NewWave, CDialogEx)

CVM_DLG_NewWave::CVM_DLG_NewWave(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVM_DLG_NewWave::IDD, pParent)
{

}

CVM_DLG_NewWave::~CVM_DLG_NewWave()
{
}

void CVM_DLG_NewWave::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVM_DLG_NewWave, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CVM_DLG_NewWave::OnBnClickedButton1)
END_MESSAGE_MAP()


// обработчики сообщений CVM_DLG_NewWave


void CVM_DLG_NewWave::OnBnClickedButton1()
{
	// TODO: добавьте свой код обработчика уведомлений
	DH_VM_Get_NEW_RSP_Enters();
}
