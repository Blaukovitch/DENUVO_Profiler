/*
	DENUVO_Profiler v0.3
	--------------------
	CRACKLAB, ELF
	--------------------
	Tiberium reversing
	********************
	2011	-	2024
*/

#pragma once
#include "ENGINE\ENGINE.h"
#include "ENGINE\ENGINE_Licence_operations.h"

// диалоговое окно СDLC_Lic_Files

class СDLC_Lic_Files : public CDialogEx
{
	DECLARE_DYNAMIC(СDLC_Lic_Files)

public:
	СDLC_Lic_Files(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~СDLC_Lic_Files();

// Данные диалогового окна
	enum { IDD = IDD_LIC_DIALOG_DLC_FILES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	pLIC_DLC_FileInfo_STACK pFileinfoStack;
	pDH_LICENCE_DECODE_DLC_FILE_DATA CurrentFileDecodeInfo;
	void Fill_DLC_licence_list(void);
	pDH_LICENCE_DECODE_DLC_FILE_DATA pCurSelDLC_Info;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox m_FILELIST;
//	CEdit m_EDIT_SECRET_TABLE;
	CEdit m_EDIT_STEAM_LIC_COUNT;
	CEdit m_EDIT_ORIGIN_LIC_COUNT;
	CEdit m_EDIT_XML_CONTAINER;
	afx_msg void OnSelchangeFilelist1();
	CListBox m_LIST_X_TABLE;
	CEdit m_EDIT_XTABLE_DWORD_count;
	CEdit m_EDIT_ENTROPY;
	afx_msg void OnLbnSelchangeFilelist1();
};
