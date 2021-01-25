
// CommTestDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "CommTest.h"
#include "CommTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCommTestDlg 대화 상자



CCommTestDlg::CCommTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCommTestDlg::IDD, pParent)
	, m_nIdxPort(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCommTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_CBB_PORT, m_nIdxPort);
	DDX_Control(pDX, IDC_REDIT_RECV, m_LogRecv);
}

BEGIN_MESSAGE_MAP(CCommTestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CCommTestDlg::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT, &CCommTestDlg::OnBnClickedBtnDisconnect)
	ON_MESSAGE(WM_MYCLOSE, &CCommTestDlg::OnMyclose)
	ON_MESSAGE(WM_MYRECEIVE, &CCommTestDlg::OnMyreceive)
	ON_BN_CLICKED(IDC_BTN_SEND, &CCommTestDlg::OnBnClickedBtnSend)
END_MESSAGE_MAP()


// CCommTestDlg 메시지 처리기

BOOL CCommTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	((CComboBox*)GetDlgItem(IDC_CBB_PORT))->SetCurSel(0);


 
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CCommTestDlg::OnPaint()
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
HCURSOR CCommTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCommTestDlg::OnBnClickedBtnConnect()
{
	UpdateData(TRUE);
	
	m_Comm = new CSerialCom();
	if(m_Comm->Create(GetSafeHwnd())!=0)
	{
		m_Comm->Clear();
	}
	else
	{
		AfxMessageBox("Create CSerialCom Failed");
	}
	
	int nPort = m_nIdxPort + 1;
  if(m_Comm->OpenConnection(nPort,CBR_115200,8,NOPARITY,ONESTOPBIT) == true)
  {
    //Connection Success
		AfxMessageBox("Connected");
  }
  else
  {
		AfxMessageBox("Connection Error[Port 설정 재확인]");
    //Connection Error
  }
}


void CCommTestDlg::OnBnClickedBtnDisconnect()
{
	if(m_Comm)
	{
		m_Comm->CloseConnection();
		m_Comm=NULL;
		AfxMessageBox("Disconnected");
	}
}


afx_msg LRESULT CCommTestDlg::OnMyclose(WPARAM wParam, LPARAM lParam)
{
	//overlapped i/o 핸들을 닫음
	((CSerialCom*)lParam)->HandleClose();
	delete ((CSerialCom*)lParam);

	return 0;
}


afx_msg LRESULT CCommTestDlg::OnMyreceive(WPARAM length, LPARAM lParam)
{
	CString str;
	char data[20000];
	if(m_Comm)
	{
		m_Comm->Receive(data,length);                    // length 길이 만큼 데이터를 얻음
		data[length]='\0';
		for(int i = 0;i<(int)length;i++)
		{
			str += data[i];
		}

		AddLogMsg(str);
		//m_RcvData.ReplaceSel(str);                                        // 에디트 박스에 표시하기 위함
		str = "";
	}

	return 0;
}


void CCommTestDlg::OnBnClickedBtnSend()
{
	CString sStr, s;
	UpdateData(TRUE);

	GetDlgItemText(IDC_EDIT_SEND, sStr);

	sStr+="\r";

	m_Comm->SendMsg(sStr);
}

void CCommTestDlg::AddLogMsg(CString sStr)
{
	//For Rich Text Edit
	CString sData;
	CString sTime;

	CHARFORMAT cf;
	cf.cbSize		= sizeof(CHARFORMAT);
	cf.dwMask		= CFM_COLOR;
	cf.dwEffects	= 0;
	cf.crTextColor	= RGB(0,0,0);	
		
	SYSTEMTIME	tTime;
	GetLocalTime(&tTime);

	sTime.Format("[%02d:%02d:%02d.%03d] > ", tTime.wHour, tTime.wMinute, tTime.wSecond, tTime.wMilliseconds);	
	sData.Format("%s%s\r\n",sTime, sStr);

	m_LogRecv.SetSelectionCharFormat(cf);
	m_LogRecv.LineScroll(3,0);
	m_LogRecv.ReplaceSel(sData, TRUE);
	//m_cLog.ReplaceSel(CString(sData+"\r\n"), TRUE);
	
	if( m_LogRecv.GetLineCount() > 300 )
	{
		m_LogRecv.SetSel(0, m_LogRecv.LineLength(0)+1);		
		m_LogRecv.ReplaceSel("");	
	}

	m_LogRecv.SetSel(-1, -1);
}
