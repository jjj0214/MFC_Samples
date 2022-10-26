
#include <winsock2.h>


typedef struct {
	DWORD Address;                             // Replying address
	unsigned long  Status;                     // Reply status
	unsigned long  RoundTripTime;              // RTT in milliseconds
	unsigned short DataSize;                   // Echo data size
	unsigned short Reserved;                   // Reserved for system use
	void *Data;                                // Pointer to the echo data
	IP_OPTION_INFORMATION Options;             // Reply options
} IP_ECHO_REPLY, *PIP_ECHO_REPLY;


BOOL PingTest(CString sAddr)
{
  WSADATA wsaData;

  int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData); 
  if (nRet)
  {
    TRACE(_T("WSAStartup() 오류 발생했습니다. 코드 번호 : %d\n"));
    WSACleanup();
    return FALSE;
  }

  HINSTANCE hIcmp = LoadLibrary(_T("ICMP.DLL"));

  if (hIcmp == 0)
  {
    TRACE(_T("error\n"));
    return FALSE;
  }

  //  m_EditIpAddress 컨트롤에서 IP를 받아온다 컨트롤 안쓸거면 그냥 CString 형식의 아이피 주소
  //  _T("xxx.xxx.xxx.xxx") 넣어주면된다

  // Look up an IP address for the given host name
  hostent* phe;
  phe = gethostbyname(sAddr);

  if (phe == 0)
  {
    TRACE(_T("IP 찾을수없음\n"));
    return FALSE;
  }

  // Get handles to the functions inside ICMP.DLL that we'll need
  typedef HANDLE(WINAPI* pfnHV)(VOID);
  typedef BOOL(WINAPI* pfnBH)(HANDLE);
  typedef DWORD(WINAPI* pfnDHDPWPipPDD)(HANDLE, DWORD, LPVOID, WORD, PIP_OPTION_INFORMATION, LPVOID, DWORD, DWORD); // evil, no?
  pfnHV pIcmpCreateFile;
  pfnBH pIcmpCloseHandle;
  pfnDHDPWPipPDD pIcmpSendEcho;

  pIcmpCreateFile = (pfnHV)GetProcAddress(hIcmp, "IcmpCreateFile");
  pIcmpCloseHandle = (pfnBH)GetProcAddress(hIcmp, "IcmpCloseHandle");
  pIcmpSendEcho = (pfnDHDPWPipPDD)GetProcAddress(hIcmp, "IcmpSendEcho");

  if ((pIcmpCreateFile == 0) || (pIcmpCloseHandle == 0) || (pIcmpSendEcho == 0))
  {
    TRACE(_T("Failed to get proc addr for function.\n"));
    FreeLibrary((HINSTANCE)hIcmp);
    return FALSE;
  }

  // Open the ping service
  HANDLE hIP = pIcmpCreateFile();
  if (hIP == INVALID_HANDLE_VALUE) {
    TRACE(_T("Unable to open ping service.\n"));
    return FALSE;
  }

  // Build ping packet
  char acPingBuffer[64];
  memset(acPingBuffer, '\xAA', sizeof(acPingBuffer));
  PIP_ECHO_REPLY pIpe = (PIP_ECHO_REPLY)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, sizeof(IP_ECHO_REPLY) + sizeof(acPingBuffer));

  if (pIpe == 0)
  {
    TRACE(_T("Failed to allocate global ping packet buffer.\n"));
    return FALSE;
  }
  pIpe->Data = acPingBuffer;
  pIpe->DataSize = sizeof(acPingBuffer);

  // Send the ping packet
  DWORD dwStatus = pIcmpSendEcho(hIP, *((DWORD*)phe->h_addr_list[0]),
    acPingBuffer, sizeof(acPingBuffer), NULL, pIpe,
    sizeof(IP_ECHO_REPLY) + sizeof(acPingBuffer), 5000);

  CString sLog;
  BOOL blRet=FALSE;
  if (dwStatus != 0)
  {
    if(pIpe->Status==0)
    {
      sLog.Format(_T("Addr:%d.%d.%d.%d BYTE:%d RTT:%dms TTL:%d\n"),
        int(LOBYTE(LOWORD(pIpe->Address))),
        int(HIBYTE(LOWORD(pIpe->Address))),
        int(LOBYTE(HIWORD(pIpe->Address))),
        int(HIBYTE(HIWORD(pIpe->Address))),
        pIpe->DataSize,
        int(pIpe->RoundTripTime),
        int(pIpe->Options.Ttl));
      blRet=TRUE;
    }
    else
    {
      sLog.Format(_T("Error obtaining info from ping packet.\n"));
    }
  }
  else
  {
    sLog.Format(_T("Error obtaining info from ping packet.\n"));
  }
  TRACE(sLog);

  // m_ListLog 컨트롤 사용하는건데 콘트롤 없으면 그냥 sLog 가 결과값이므로 이걸 알아서 잘 쓰면 된다.
  // CString 형식이다. 그냥 AfxMessageBox에 m_ListLog 변수 찍어보기 바람
  //m_ListLog.AddString(sLog);
  //m_ListLog.SetCurSel(m_ListLog.GetCount() - 1);

  // Shut down...
  GlobalFree(pIpe);
  FreeLibrary(hIcmp);

  WSACleanup();
  return blRet;
}
