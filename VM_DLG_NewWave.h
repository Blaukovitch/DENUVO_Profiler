#pragma once

#include "ENGINE\ENGINE.h"
#include "ENGINE\ENGINE_VM_operations.h"
// ���������� ���� CVM_DLG_NewWave

class CVM_DLG_NewWave : public CDialogEx
{
	DECLARE_DYNAMIC(CVM_DLG_NewWave)

public:
	CVM_DLG_NewWave(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~CVM_DLG_NewWave();

// ������ ����������� ����
	enum { IDD = IDD_VM_DIALOG_NEW_WAVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
