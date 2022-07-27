
// IPCSendView.cpp : CIPCSendView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "IPCSend.h"
#endif

#include "IPCSendDoc.h"
#include "IPCSendView.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIPCSendView

IMPLEMENT_DYNCREATE(CIPCSendView, CFormView)

	BEGIN_MESSAGE_MAP(CIPCSendView, CFormView)
		ON_WM_CONTEXTMENU()
		ON_WM_RBUTTONUP()
		ON_BN_CLICKED(IDC_BTN_EXIT, &CIPCSendView::OnBnClickedBtnExit)
		ON_BN_CLICKED(IDC_BTN_FRAME_COMM_ACK, &CIPCSendView::OnBnClickedBtnFrameCommAck)
		ON_BN_CLICKED(IDC_BTN_FRAME_COMM_ASK_TEACHING, &CIPCSendView::OnBnClickedBtnFrameCommAskTeaching)
	END_MESSAGE_MAP()

	// CIPCSendView 생성/소멸

	CIPCSendView::CIPCSendView()
		: CFormView(CIPCSendView::IDD)
	{
		// TODO: 여기에 생성 코드를 추가합니다.

	}

	CIPCSendView::~CIPCSendView()
	{
	}

	void CIPCSendView::DoDataExchange(CDataExchange* pDX)
	{
		CFormView::DoDataExchange(pDX);
		DDX_Control(pDX, IDC_REDIT_LOG, m_cLog);
	}

	BOOL CIPCSendView::PreCreateWindow(CREATESTRUCT& cs)
	{
		// TODO: CREATESTRUCT cs를 수정하여 여기에서
		//  Window 클래스 또는 스타일을 수정합니다.

		return CFormView::PreCreateWindow(cs);
	}

	void CIPCSendView::OnInitialUpdate()
	{
		CFormView::OnInitialUpdate();
		GetParentFrame()->RecalcLayout();
		ResizeParentToFit();

	}

	void CIPCSendView::OnRButtonUp(UINT /* nFlags */, CPoint point)
	{
		ClientToScreen(&point);
		OnContextMenu(this, point);
	}

	void CIPCSendView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
	{
#ifndef SHARED_HANDLERS
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
	}


	// CIPCSendView 진단

#ifdef _DEBUG
	void CIPCSendView::AssertValid() const
	{
		CFormView::AssertValid();
	}

	void CIPCSendView::Dump(CDumpContext& dc) const
	{
		CFormView::Dump(dc);
	}

	CIPCSendDoc* CIPCSendView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIPCSendDoc)));
		return (CIPCSendDoc*)m_pDocument;
	}
#endif //_DEBUG


	// CIPCSendView 메시지 처리기


	void CIPCSendView::OnBnClickedBtnExit()
	{		
		CMainFrame * pFrame = ((CMainFrame*)AfxGetApp()->m_pMainWnd);	
		if( IDCANCEL == MessageBox(_T("Do you want to exit the program?"), _T("IPCSend"), MB_OKCANCEL) )
			return;

		pFrame->OnClose();	
	}


	void CIPCSendView::OnBnClickedBtnFrameCommAck()
	{
		CString strSendMsg;
		//
		//    데이터를 전송 할 프로세스의 핸들을 이용하기 위해 윈도우 객체를 얻어 옵니다.
		//
		CWnd* pWnd = FindWindow(NULL, _T("IPC Recv"));
		ASSERT(pWnd);
		//
		//    전송할 데이터를 셋팅합니다.
		//
		AckInfo ackInfo = {TRUE};
		//
		//    전송할 데이터를 COPYDATASTURCT에 셋팅하고 전송합니다.
		//
		COPYDATASTRUCT cds;

		cds.dwData = Protocol::PROTOCOL_ACK;
		cds.cbData = sizeof(ackInfo);
		cds.lpData = (LPVOID)&ackInfo;
		
		strSendMsg.Format(_T("Ack: %d"), ackInfo.m_blACK);
		Log(_T("SEND"),strSendMsg);    

		::SendMessage(pWnd->m_hWnd, WM_COPYDATA, (WPARAM)(CMainFrame*)AfxGetMainWnd()->m_hWnd, (LPARAM)&cds);
	}


	void CIPCSendView::OnBnClickedBtnFrameCommAskTeaching()
	{
		UpdateData(TRUE);
		CString sStr;
		CString strSendMsg;

		//
		//    데이터를 전송 할 프로세스의 핸들을 이용하기 위해 윈도우 객체를 얻어 옵니다.
		//
		CWnd* pWnd = FindWindow(NULL, _T("IPC Recv"));
		if(pWnd != NULL)
		{
			//
			//    전송할 데이터를 셋팅합니다.
			//
			TeachingInfo tInfo;

			GetDlgItemText(IDC_EDIT_FRAME_COMM_EQUIP_CODE, sStr);
			_tcsncpy(tInfo.m_sEquipCode, sStr, _tcslen(sStr));
			GetDlgItemText(IDC_EDIT_FRAME_COMM_PRODUCT_CODE, sStr);
			_tcsncpy(tInfo.m_sProductCode, sStr, _tcslen(sStr));
			GetDlgItemText(IDC_EDIT_FRAME_COMM_PRODUCT_TYPE, sStr);
			_tcsncpy(tInfo.m_sProductType, sStr, _tcslen(sStr));
			tInfo.m_unTeachingVer = GetDlgItemInt(IDC_EDIT_FRAME_COMM_TEACHING_VER);
			tInfo.m_unProductRev = GetDlgItemInt(IDC_EDIT_FRAME_COMM_PRODUCT_REV);
			//
			//    전송할 데이터를 COPYDATASTURCT에 셋팅하고 전송합니다.
			//
			COPYDATASTRUCT cds;

			cds.dwData = Protocol::PROTOCOL_TFILE_INFO;
			cds.cbData = sizeof(tInfo);
			cds.lpData = (LPVOID)&tInfo;

			strSendMsg.Format(_T("EquipCode: %s|ProductCode: %s|ProductType: %s|ProductRev.: %d|TeachingFileVer: %d"), 
				tInfo.m_sEquipCode, tInfo.m_sProductCode, tInfo.m_sProductType, tInfo.m_unProductRev, tInfo.m_unTeachingVer);
			Log(_T("SEND"),strSendMsg);    

			::SendMessage(pWnd->m_hWnd, WM_COPYDATA, (WPARAM)(CMainFrame*)AfxGetMainWnd()->m_hWnd, (LPARAM)&cds);
		}
	}

	void CIPCSendView::Log(CString sSubject, CString sStr)
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
