// ClientSocket.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "socServer.h"
#include "ClientSocket.h"


// CClientSocket

CClientSocket::CClientSocket()
{
}

CClientSocket::~CClientSocket()
{
}


// CClientSocket 멤버 함수
void CClientSocket::SetWnd(HWND hWnd)
{
  m_hWnd = hWnd;
}
 
void CClientSocket::OnReceive(int nErrorCode)
{
  // TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
  CString strTmp = _T(""), strIPAddress = _T("");
  UINT uPortNumber = 0;
	TCHAR strBuffer[1024];
  ZeroMemory(strBuffer, sizeof(strBuffer));
   
  GetPeerName(strIPAddress, uPortNumber);
  if(Receive(strBuffer, sizeof(strBuffer)) > 0) { // 전달된 데이터(문자열)가 있을 경우
    strTmp.Format(_T("[%s : %d]: %s"), strIPAddress, uPortNumber, strBuffer);
  }
  //Main Window에 Send
  SendMessage(m_hWnd,WM_CLIENT_MSG_RECV,0,(LPARAM)((LPCTSTR)strTmp));
 
  CSocket::OnReceive(nErrorCode);
}
 
void CClientSocket::OnClose(int nErrorCode)
{
  // TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
  SendMessage(m_hWnd, WM_CLIENT_CLOSE, 0, (LPARAM)this);
  CSocket::OnClose(nErrorCode);
}
