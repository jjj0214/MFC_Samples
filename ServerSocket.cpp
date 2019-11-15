// ServerSocket.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "socServer.h"
#include "ServerSocket.h"


// CServerSocket

CServerSocket::CServerSocket()
{
}

CServerSocket::~CServerSocket()
{
}


// CServerSocket 멤버 함수
void CServerSocket::SetWnd(HWND hWnd)
{
  m_hWnd = hWnd;
}
 
void CServerSocket::OnAccept(int nErrorCode)
{
  // TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CClientSocket* pClient = new CClientSocket; //Client 소켓 포인터 추가
 
  if(Accept(*pClient))  //Listen에서 클라이언트 접속을 확인하면
  {
    pClient->SetWnd(m_hWnd); //Client소켓에 메인핸들 연결
  }
  else    //클라이언트 접속 문제시
  {
    delete pClient; 
    AfxMessageBox(_T("ERROR : Failed can't accept new Client!"));
  }
 
  SendMessage(m_hWnd, WM_ACCEPT_SOCKET, 0, (LPARAM)pClient);
  CSocket::OnAccept(nErrorCode);
}
