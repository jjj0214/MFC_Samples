#pragma once

#include "AutoUpdate\AutoUpdate.h"
// CDlgAutoUpdate 대화 상자입니다.

class CDlgAutoUpdate : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAutoUpdate)

public:
	CDlgAutoUpdate(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgAutoUpdate();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_AUTOUPDATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_listLog;
	virtual BOOL OnInitDialog();
	BOOL AddLog(CString sLog);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CRichEditCtrl m_Log;
	AutoUpdate m_au;
	CHARFORMAT m_cf;
};
