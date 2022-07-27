
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "IPCRecv.h"

#include "MainFrm.h"
#include "IPCRecvDoc.h"
#include "IPCRecvView.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

	const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 중복 실행방지
	HANDLE hEvent;
	hEvent = CreateEvent(NULL, FALSE, TRUE, AfxGetAppName());
	if( GetLastError() == ERROR_ALREADY_EXISTS )
	{
		AfxMessageBox(_T("Already running..."));
		exit(0);
	}

	// 실행파일 경로
	char	name[256];
	_getcwd(name,256);	
	m_sGuiPath = (LPSTR)name;

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;

	cs.hMenu = NULL; // 메뉴바 없애기

	cs.style &= ~(LONG)WS_SYSMENU;			// 아래 버튼들 전부 누를 수 없게...
	cs.style &= ~(LONG)WS_THICKFRAME;		// 윈도우크기를 변경 못하게...
	cs.style &= ~(LONG)WS_MAXIMIZEBOX;		// 최대화버튼을 누룰수 없게...
	cs.style &= ~(LONG)WS_MINIMIZEBOX;		// 최소화버튼을 누룰수 없게...

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 메뉴를 검색합니다. */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// 기본 클래스가 실제 작업을 수행합니다.

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// 모든 사용자 도구 모음에 사용자 지정 단추를 활성화합니다.
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}



LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	CIPCRecvView *pView = (CIPCRecvView *)GetActiveView();

	CString strReceiveMsg, strSendMsg;
	CString sStr;
	switch(message)
	{
	case WM_COPYDATA:
		{
			PCOPYDATASTRUCT pcds = (PCOPYDATASTRUCT) lParam;

			switch(pcds->dwData)
			{
			case Protocol::PROTOCOL_ACK:
				{
					AckInfo ackInfo;
					ackInfo.m_blACK = ((AckInfo*)pcds->lpData)->m_blACK;

					strReceiveMsg.Format(_T("Ack: %d"), ackInfo.m_blACK);

					pView->Log(_T("RECV"),strReceiveMsg);       
					//
					//    wParam으로 들어 온 윈도우 핸들을 이용해서 데이터를 전송한 프로세스에게 데이터를 에코합니다.
					//   
					SSAInfo ssaInfo;

					ssaInfo.m_nResult = 1;
					ssaInfo.m_nErrorCode= 0;
					strSendMsg.Format(_T("Result: %d\tErrorcode: %d"), ssaInfo.m_nResult, ssaInfo.m_nErrorCode);

					COPYDATASTRUCT cds;

					cds.dwData = Protocol::PROTOCOL_ACK;
					cds.cbData = sizeof(ssaInfo);
					cds.lpData = (LPVOID)&ssaInfo;

					HWND hWnd = (HWND) wParam;
					::SendMessage(hWnd, WM_COPYDATA, 0, (LPARAM) &cds);
					pView->Log(_T("SEND"),strSendMsg);
					break;
				}
			case Protocol::PROTOCOL_TFILE_INFO:
				{
					TeachingInfo tInfo;
					//_tcsncpy(tInfo.m_sEquipCode, ((TeachingInfo*)pcds->lpData)->m_sEquipCode, pcds->cbData);
					//_tcsncpy(tInfo.m_sProductCode, ((TeachingInfo*)pcds->lpData)->m_sProductCode, pcds->cbData - (sizeof(TCHAR)*50));
					//_tcsncpy(tInfo.m_sProductType, ((TeachingInfo*)pcds->lpData)->m_sProductType, pcds->cbData - (sizeof(TCHAR)*50) - (sizeof(TCHAR)*50));
					
					_tcscpy_s(tInfo.m_sEquipCode, 50, ((TeachingInfo*)pcds->lpData)->m_sEquipCode); // Copy str   
					_tcscpy_s(tInfo.m_sProductCode, 50, ((TeachingInfo*)pcds->lpData)->m_sProductCode); // Copy str   
					_tcscpy_s(tInfo.m_sProductType, 2, ((TeachingInfo*)pcds->lpData)->m_sProductType); // Copy str   
					tInfo.m_unProductRev	= ((TeachingInfo*)pcds->lpData)->m_unProductRev;
					tInfo.m_unTeachingVer	= ((TeachingInfo*)pcds->lpData)->m_unTeachingVer;
					
					pView->SetTeachingInfo(&tInfo);

					strReceiveMsg.Format(_T("EquipCode: %s|ProductCode: %s|ProductType: %s|ProductRev.: %d|TeachingFileVer: %d"), 
						tInfo.m_sEquipCode, tInfo.m_sProductCode, tInfo.m_sProductType, tInfo.m_unProductRev, tInfo.m_unTeachingVer);

					pView->Log(_T("RECV"),strReceiveMsg);    

					SSAInfo ssaInfo;

					ssaInfo.m_nResult = 1;
					ssaInfo.m_nErrorCode= 0;
					strSendMsg.Format(_T("Result: %d\tErrorcode: %d"), ssaInfo.m_nResult, ssaInfo.m_nErrorCode);

					COPYDATASTRUCT cds;

					cds.dwData = Protocol::PROTOCOL_ACK;
					cds.cbData = sizeof(ssaInfo);
					cds.lpData = (LPVOID)&ssaInfo;

					HWND hWnd = (HWND) wParam;
					::SendMessage(hWnd, WM_COPYDATA, 0, (LPARAM) &cds);
					pView->Log(_T("SEND"),strSendMsg);    
					break;
				}
			}
		}
	}

	return CFrameWndEx::WindowProc(message, wParam, lParam);
}
