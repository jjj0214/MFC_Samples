
// IPCSendView.h : CIPCSendView 클래스의 인터페이스
//

#pragma once

#include "resource.h"

class CIPCSendView : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CIPCSendView();
	DECLARE_DYNCREATE(CIPCSendView)

public:
	enum{ IDD = IDD_DLG_FRAME };

// 특성입니다.
public:
	CIPCSendDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~CIPCSendView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	CRichEditCtrl	m_cLog;	

public:
	afx_msg void OnBnClickedBtnExit();
	afx_msg void OnBnClickedBtnFrameCommAck();
	afx_msg void OnBnClickedBtnFrameCommAskTeaching();
	
	void Log(CString sSubject, CString sStr);
};

#ifndef _DEBUG  // IPCSendView.cpp의 디버그 버전
inline CIPCSendDoc* CIPCSendView::GetDocument() const
   { return reinterpret_cast<CIPCSendDoc*>(m_pDocument); }
#endif

