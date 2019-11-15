
// socClientDlg.h : 헤더 파일
//

#pragma once
 
#include "ClientSocket.h"
#include "afxwin.h"

// CsocClientDlg 대화 상자
class CsocClientDlg : public CDialogEx
{
// 생성입니다.
public:
	CsocClientDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SOCCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
  CClientSocket m_ClientSocket;
protected:
	afx_msg LRESULT OnClientRecv(WPARAM wParam, LPARAM lParam);
public:
	CEdit m_edit_send;
	CListBox m_list_msg;
	afx_msg void OnBnClickedBtnSend();
};
