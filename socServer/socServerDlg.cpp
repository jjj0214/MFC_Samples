
// socServerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "socServer.h"
#include "socServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PORT 9999   //임의의 포트 설정

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CsocServerDlg 대화 상자



CsocServerDlg::CsocServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CsocServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CsocServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CLIENT, m_list_client);
	DDX_Control(pDX, IDC_LIST_MSG, m_list_msg);
}

BEGIN_MESSAGE_MAP(CsocServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_ACCEPT_SOCKET, &CsocServerDlg::OnAcceptSocket)
	ON_MESSAGE(WM_CLIENT_MSG_RECV, &CsocServerDlg::OnClientMsgRecv)
	ON_MESSAGE(WM_CLIENT_CLOSE, &CsocServerDlg::OnClientClose)
END_MESSAGE_MAP()


// CsocServerDlg 메시지 처리기

BOOL CsocServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	
  //Listen 소켓 초기화
  m_pServerSocket = new CServerSocket;
  m_pServerSocket->SetWnd(this->m_hWnd);
 
  //소켓 Listen하기
  m_pServerSocket->Create(PORT);
  m_pServerSocket->Listen();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CsocServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CsocServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CsocServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



afx_msg LRESULT CsocServerDlg::OnAcceptSocket(WPARAM wParam, LPARAM lParam)
{
  CString str;
  m_pClientSocket = (CClientSocket*) lParam;
  m_ptrClientSocketList.AddTail(m_pClientSocket);
 
  str.Format(_T("Client (%d)"), (int)(m_pClientSocket));
	m_list_client.InsertString(-1,str);
 
  m_pClientSocket = NULL;
  delete m_pClientSocket;

	return 0;
}


afx_msg LRESULT CsocServerDlg::OnClientMsgRecv(WPARAM wParam, LPARAM lParam)
{
  LPCTSTR lpszStr = (LPCTSTR)lParam;
 
  //BroadCasting
  POSITION pos = m_ptrClientSocketList.GetHeadPosition();
  
  while(pos != NULL)
  {
    CClientSocket* pClient = (CClientSocket*)m_ptrClientSocketList.GetNext(pos);
    if(pClient!=NULL)
    {
      //UNICODE 사용하면 기존 메모리크기 *2 해야함
      //Client에 Broadcasting하기
      pClient->Send(lpszStr, lstrlen(lpszStr) * 2);
    }
  }
 
  //m_list_msg에 메시지 추가
  m_list_msg.InsertString(-1,lpszStr);
  m_list_msg.SetCurSel(m_list_msg.GetCount()-1);

	return 0;
}


afx_msg LRESULT CsocServerDlg::OnClientClose(WPARAM wParam, LPARAM lParam)
{
  CClientSocket *pClient = (CClientSocket *)lParam;
  CString str;
  UINT idx=0;
  POSITION pos = m_ptrClientSocketList.Find(pClient);
 
  if(pos!=NULL)
  {
    //m_list_client에서 해당 Client 삭제
    str.Format(_T("Client (%d)"), (int)pClient);
    idx = m_list_client.SelectString(-1, (LPCTSTR)str);
    m_list_client.DeleteString(idx);
     
    //CPtrList에서도 해당 Client 삭제
    m_ptrClientSocketList.RemoveAt(pos);
  }

	return 0;
}
