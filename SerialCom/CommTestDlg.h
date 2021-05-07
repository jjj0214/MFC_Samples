
// CommTestDlg.h : 헤더 파일
//

#pragma once
#include "SerialCom.h"
#include "afxwin.h"
#include "SerialCom\ComPortCombo.h"

// CCommTestDlg 대화 상자
class CCommTestDlg : public CDialogEx
{
// 생성입니다.
public:
	CCommTestDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_COMMTEST_DIALOG };
	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
  CSerialCom* m_Comm;
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnBnClickedBtnDisconnect();
protected:
	afx_msg LRESULT OnMyclose(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMyreceive(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedBtnSend();
	CRichEditCtrl	m_LogRecv;	
	void AddLogMsg(CString sStr);
	CComPortCombo m_cbPort;
};
