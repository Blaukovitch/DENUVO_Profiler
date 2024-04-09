#pragma once

#include "ENGINE\ENGINE.h"
#include "ENGINE\ENGINE_VM_operations.h"
// диалоговое окно CVM_DLG_NewWave

class CVM_DLG_NewWave : public CDialogEx
{
	DECLARE_DYNAMIC(CVM_DLG_NewWave)

public:
	CVM_DLG_NewWave(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~CVM_DLG_NewWave();

// Данные диалогового окна
	enum { IDD = IDD_VM_DIALOG_NEW_WAVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
