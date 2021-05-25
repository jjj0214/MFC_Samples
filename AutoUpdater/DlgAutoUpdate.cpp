// DlgAutoUpdate.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Updater.h"
#include "DlgAutoUpdate.h"
#include "afxdialogex.h"


enum
{
	TIMER_CHECK_CURRENT_VERSION = 100,
	TIMER_CHECK_UPDATE_EXIST,
	TIMER_CHECK_VERSION_MATCH,
	TIMER_UPDATE_FILE,
	TIMER_OPEN_NEW_FILE,
	TIMER_NO_UPDATE,
	NUM_TIMER_ID
};

// CDlgAutoUpdate 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgAutoUpdate, CDialogEx)

CDlgAutoUpdate::CDlgAutoUpdate(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAutoUpdate::IDD, pParent)
{

}

CDlgAutoUpdate::~CDlgAutoUpdate()
{
}

void CDlgAutoUpdate::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOG_AUTOUPDATE, m_Log);
}


BEGIN_MESSAGE_MAP(CDlgAutoUpdate, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgAutoUpdate 메시지 처리기입니다.

BOOL CDlgAutoUpdate::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	m_cf.cbSize		= sizeof(CHARFORMAT);
	m_cf.dwMask		= CFM_COLOR;
	m_cf.dwEffects	= 0;
	m_cf.crTextColor	= RGB(0,0,0);	

	CenterWindow();
	SetTimer(TIMER_CHECK_CURRENT_VERSION,100,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CDlgAutoUpdate::AddLog(CString sLog)
{
	CString sData;
	
	sData.Format("%s\r\n",sLog);

	m_Log.SetSelectionCharFormat(m_cf);
	m_Log.LineScroll(3,0);
	m_Log.ReplaceSel(sData, TRUE);

	if( m_Log.GetLineCount() > 300 )
	{
		m_Log.SetSel(0, m_Log.LineLength(0)+1);		
		m_Log.ReplaceSel("");	
	}

	m_Log.SetSel(-1, -1);
	return TRUE;
}

void CDlgAutoUpdate::OnTimer(UINT_PTR nIDEvent)
{
	CString sStr;
	
	switch (nIDEvent)
	{
	case TIMER_CHECK_CURRENT_VERSION:
		if(::IsWindow(m_Log.m_hWnd))
		{
			KillTimer(nIDEvent);
			
			//Release 에서만 작동 하게 변경
			//Debug 모드에서는 비활성화
#ifdef DEBUG
		AddLog("Debug Mode Activate. Disable AutoUpdate");
		SetTimer(TIMER_NO_UPDATE,3000,NULL);
		break;
#endif

			AddLog("Check current version");
			if(!m_au.GetCurrVersion())
			{
				AddLog("[Error] Get version");
				return;
			}
			sStr.Format("Current version: [%s]",m_au.GetVersiontoString(0));
			AddLog(sStr);
			SetTimer(TIMER_CHECK_UPDATE_EXIST,1000,NULL);
		}
		break;
	case TIMER_CHECK_UPDATE_EXIST:
		KillTimer(nIDEvent);
		AddLog("Check update exists");
		if(m_au.CheckUpdatesExist())
		{
			if(!m_au.GetTargetVersion())
			{
				AddLog("[Error] Get version");
				return;
			}

			sStr.Format("New version: [%s]",m_au.GetVersiontoString(1));
			AddLog(sStr);
			SetTimer(TIMER_CHECK_VERSION_MATCH,1000,NULL);
		}
		else
		{
			AddLog("No updates. Current version is the latest version.");
			SetTimer(TIMER_NO_UPDATE,2000,NULL);
		}
		break;
	case TIMER_CHECK_VERSION_MATCH:
		KillTimer(nIDEvent);
		AddLog("Check version match");
		if(m_au.CheckVersionMatch())
		{
			AddLog("No updates. Current version is the latest version.");
			SetTimer(TIMER_NO_UPDATE,2000,NULL);
		}
		else
		{
			AddLog("new version found. updating...");
			SetTimer(TIMER_UPDATE_FILE,1000,NULL);
		}
		break;
	case TIMER_UPDATE_FILE:
		KillTimer(nIDEvent);
		if(m_au.UpdateFile())
		{
			AddLog("Update finished. restart the program after 3secs..");
			SetTimer(TIMER_OPEN_NEW_FILE,3000,NULL);
		}
		else
		{
			AfxMessageBox("[Error] Update File");
			SetTimer(TIMER_NO_UPDATE,2000,NULL);
		}
		break;
	case TIMER_OPEN_NEW_FILE:
		KillTimer(nIDEvent);
		m_au.OpenNewFile();
		_exit(0);
		break;
	case TIMER_NO_UPDATE:
		KillTimer(nIDEvent);
		ShowWindow(SW_HIDE);
		break;
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}
