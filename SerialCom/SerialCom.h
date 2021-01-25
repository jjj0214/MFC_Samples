
#pragma once

#define MAX_BUF 50000
#define WM_MYRECEIVE	WM_USER + 0x100
#define WM_MYCLOSE		WM_USER + 0x101

class CSerialCom
{
public:
  CSerialCom(void);
  ~CSerialCom(void);
  HANDLE m_hComm;     //Handle of COM Port
	
	HWND m_hWnd;
  BOOL Create(HWND hWnd);
  BOOL OpenConnection(BYTE nPort, DWORD nBaudrate, BYTE nSize, BYTE nParity, BYTE nStop);
  void CloseConnection(void);
  void HandleClose(void);
  void Clear(void);
  BOOL SetDCB(DWORD nBaudrate, BYTE nSize, BYTE nParity, BYTE nStop);
  BOOL SetTimeouts(void);
	BOOL SendMsg(CString sMsg);
	int Receive(LPSTR inbuf, int len);
	BOOL m_bIsOpenned;
	OVERLAPPED m_OLW, m_OLR;
	CEvent *m_pEvent;
	int m_nLength;
	char	m_sInBuf[MAX_BUF*2];
	static UINT CommThread(LPVOID lpData);
};
