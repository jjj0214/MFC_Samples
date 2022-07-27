
// IPCRecvView.cpp : CIPCRecvView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "IPCRecv.h"
#endif

#include "IPCRecvDoc.h"
#include "IPCRecvView.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIPCRecvView

IMPLEMENT_DYNCREATE(CIPCRecvView, CFormView)

	BEGIN_MESSAGE_MAP(CIPCRecvView, CFormView)
		ON_WM_CONTEXTMENU()
		ON_WM_RBUTTONUP()
		ON_BN_CLICKED(IDC_BTN_EXIT, &CIPCRecvView::OnBnClickedBtnExit)
	END_MESSAGE_MAP()

	// CIPCRecvView 생성/소멸

	CIPCRecvView::CIPCRecvView()
		: CFormView(CIPCRecvView::IDD)
	{
		// TODO: 여기에 생성 코드를 추가합니다.

	}

	CIPCRecvView::~CIPCRecvView()
	{
	}

	void CIPCRecvView::DoDataExchange(CDataExchange* pDX)
	{
		CFormView::DoDataExchange(pDX);
		DDX_Control(pDX, IDC_REDIT_LOG, m_cLog);
	}

	BOOL CIPCRecvView::PreCreateWindow(CREATESTRUCT& cs)
	{
		// TODO: CREATESTRUCT cs를 수정하여 여기에서
		//  Window 클래스 또는 스타일을 수정합니다.

		return CFormView::PreCreateWindow(cs);
	}

	void CIPCRecvView::OnInitialUpdate()
	{
		CFormView::OnInitialUpdate();
		GetParentFrame()->RecalcLayout();
		ResizeParentToFit();

	}

	void CIPCRecvView::OnRButtonUp(UINT /* nFlags */, CPoint point)
	{
		ClientToScreen(&point);
		OnContextMenu(this, point);
	}

	void CIPCRecvView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
	{
#ifndef SHARED_HANDLERS
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
	}


	// CIPCRecvView 진단

#ifdef _DEBUG
	void CIPCRecvView::AssertValid() const
	{
		CFormView::AssertValid();
	}

	void CIPCRecvView::Dump(CDumpContext& dc) const
	{
		CFormView::Dump(dc);
	}

	CIPCRecvDoc* CIPCRecvView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIPCRecvDoc)));
		return (CIPCRecvDoc*)m_pDocument;
	}
#endif //_DEBUG


	// CIPCRecvView 메시지 처리기


	void CIPCRecvView::OnBnClickedBtnExit()
	{		
		CMainFrame * pFrame = ((CMainFrame*)AfxGetApp()->m_pMainWnd);	
		if( IDCANCEL == MessageBox(_T("Do you want to exit the program?"), _T("IPCRecv"), MB_OKCANCEL) )
			return;

		pFrame->OnClose();	
	}

	void CIPCRecvView::Log(CString sSubject, CString sStr)
	{
		CMainFrame * pFrame = ((CMainFrame*)AfxGetApp()->m_pMainWnd);
		CString sData;

		CHARFORMAT cf;
		cf.cbSize		= sizeof(CHARFORMAT);
		cf.dwMask		= CFM_COLOR;
		cf.dwEffects	= 0;
		cf.crTextColor	= RGB_BLACK;	

		SYSTEMTIME	tTime;
		GetLocalTime(&tTime);

		sData.Format(_T("[%02d:%02d:%02d.%03d][%s] > "), tTime.wHour, tTime.wMinute, tTime.wSecond, tTime.wMilliseconds, sSubject);	
		sData = sData + sStr;

		m_cLog.SetSelectionCharFormat(cf);
		m_cLog.LineScroll(3,0);
		m_cLog.ReplaceSel(LPCTSTR(sData+"\r\n"), TRUE);

		if( m_cLog.GetLineCount() > 1000 )
		{
			m_cLog.SetSel(0, m_cLog.LineLength(0)+1);		
			m_cLog.ReplaceSel(_T(""));	
		}

		m_cLog.SetSel(-1, -1);
	}
	
	void CIPCRecvView::SetTeachingInfo(TeachingInfo *pInfo)
	{
		SetDlgItemText(IDC_EDIT_FRAME_COMM_EQUIP_CODE, pInfo->m_sEquipCode);
		SetDlgItemText(IDC_EDIT_FRAME_COMM_PRODUCT_CODE, pInfo->m_sProductCode);
		SetDlgItemText(IDC_EDIT_FRAME_COMM_PRODUCT_TYPE, pInfo->m_sProductType);
		SetDlgItemInt(IDC_EDIT_FRAME_COMM_TEACHING_VER, pInfo->m_unTeachingVer);
		SetDlgItemInt(IDC_EDIT_FRAME_COMM_PRODUCT_REV, pInfo->m_unProductRev);
	}
