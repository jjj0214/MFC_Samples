#include "stdafx.h"
#include "SerialCom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//IMPLEMENT_DYNCREATE(CSerialCom, CCmdTarget)

//1. 생성자
CSerialCom::CSerialCom(void)  
{
	m_hWnd=NULL;
  m_hComm = NULL;
	m_bIsOpenned=FALSE;
	m_nLength = 0;
	memset(m_sInBuf,0,MAX_BUF*2);
	m_pEvent = new CEvent(FALSE,TRUE);
}
 
//2. 소멸자
CSerialCom::~CSerialCom(void)   
{
	if (m_bIsOpenned)
		CloseConnection();
	delete m_pEvent;
}
 
//3. 통신 연결
BOOL CSerialCom::OpenConnection(BYTE nPort, DWORD nBaudrate, BYTE nSize, BYTE nParity, BYTE nStop)
{
  //이미 통신이 연결되어 있다면 통신 연결 해제 후 진행
  if(m_hComm != NULL)
    CloseConnection();
   
  //드라이버 핸들 생성
  char      szPort[15];
  wsprintf(szPort, "\\\\.\\COM%d", nPort);
  if((m_hComm = CreateFile(
    szPort,
    GENERIC_READ | GENERIC_WRITE,
    0, 
    NULL, 
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 
    NULL)) == (HANDLE) - 1) return (false);
 
	m_bIsOpenned=TRUE;

  //TIME-OUT
  if(!SetTimeouts())
  {
    //Failed to set Comm Timeouts
    CloseHandle(m_hComm);
    return FALSE;
  }
 
  //DCB Structure
  if (!SetDCB(nBaudrate, nSize, nParity, nStop))
  {
    //Failed to set DCB Structure
    CloseHandle(m_hComm);
    return FALSE;
  }

	//RX Receive Event Handler
	if(!SetCommMask(m_hComm,EV_RXCHAR))
	{
		//Failed Set Mask
		CloseHandle(m_hComm);
		return FALSE;
	}
	
	//Overlapped 구조체 설정
	m_OLW.Offset = 0;                                      // Write OVERLAPPED structure 초기화
	m_OLW.OffsetHigh = 0;
	m_OLR.Offset = 0;                                      // Read OVERLAPPED structure 초기화
	m_OLR.OffsetHigh = 0;

	// Overlapped 구조체의 이벤트를 생성
	m_OLR.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (m_OLR.hEvent == NULL) {
		CloseHandle(m_OLR.hEvent);
		return FALSE;
	}
	m_OLW.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (m_OLW.hEvent == NULL) {
		CloseHandle(m_OLW.hEvent);
		return FALSE;
	}

	// 시리얼 데이터를 받기위한 스레드 생성
	AfxBeginThread(CommThread,(LPVOID)this);

	// DTR (Data Terminal Ready) signal 을 보냄
	EscapeCommFunction(m_hComm, SETDTR);   // MSDN 참조
 
  return TRUE;
}
 
//4. 통신 연결 해제
void CSerialCom::CloseConnection(void)
{
  if(!m_bIsOpenned)
    return;
   
	m_bIsOpenned=FALSE;
	SetCommMask(m_hComm, 0);
	EscapeCommFunction(m_hComm, CLRDTR);
	PurgeComm(m_hComm,PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
	Sleep(500);
}

void CSerialCom::HandleClose(void)
{
	CloseHandle(m_hComm);
	CloseHandle(m_OLW.hEvent);
	CloseHandle(m_OLR.hEvent);
}

void CSerialCom::Clear(void)
{
  PurgeComm(m_hComm, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
  memset(m_sInBuf,0,MAX_BUF*2);
  m_nLength = 0;
}
 
//5. DCB Structure 설정
BOOL CSerialCom::SetDCB(DWORD nBaudrate, BYTE nSize, BYTE nParity,  BYTE nStop)
{
  DCB dcb;
 
  dcb.DCBlength = sizeof(DCB);
  if(GetCommState(m_hComm, &dcb)==0)
  {
    //GetCommState Error
    CloseHandle(m_hComm);
    return FALSE;
  }
 
  dcb.BaudRate = nBaudrate;
  dcb.ByteSize = nSize;
  dcb.Parity = nParity;
  dcb.StopBits = nStop;
 
  return (SetCommState(m_hComm, &dcb) != 0);  //C4800 해결
}
 
//6. Timeout 설정
BOOL CSerialCom::SetTimeouts(void)
{
  COMMTIMEOUTS  CommTimeOuts; 
  CommTimeOuts.ReadIntervalTimeout      = 0xFFFFFFFF;
  CommTimeOuts.ReadTotalTimeoutMultiplier   = 0; 
  CommTimeOuts.ReadTotalTimeoutConstant   = 1000; 
  CommTimeOuts.WriteTotalTimeoutMultiplier  = 0; 
  CommTimeOuts.WriteTotalTimeoutConstant    = 1000;
  return (SetCommTimeouts(m_hComm, &CommTimeOuts) != 0);  //C4800 해결
}

BOOL CSerialCom::Create(HWND hWnd)
{
	m_hWnd= hWnd;
	return TRUE;
}

BOOL CSerialCom::SendMsg(CString sMsg)
{
	char lpBuffer[MAX_BUF]={0,};
	strcpy_s(lpBuffer,sMsg.GetString());
	DWORD dNoOFBytestoWrite;         // No of bytes to write into the port
	DWORD dNoOfBytesWritten = 0;     // No of bytes written to the port

	COMSTAT ComStat;
	DWORD ErrorFlags;
	BOOL bRet=TRUE;

	//Clear Comm Error
	ClearCommError(m_hComm, &ErrorFlags, &ComStat);

	dNoOFBytestoWrite = sMsg.GetLength();

	if(!WriteFile(m_hComm,        // Handle to the Serial port
		lpBuffer,     // Data to be written to the port
		dNoOFBytestoWrite,  //No of bytes to write
		&dNoOfBytesWritten, //Bytes written
		&m_OLW))
	{
		if (GetLastError() == ERROR_IO_PENDING){
			if (WaitForSingleObject(m_OLW.hEvent,1000) != WAIT_OBJECT_0)
				bRet=FALSE;
			else
				GetOverlappedResult(m_hComm, &m_OLW, &dNoOFBytestoWrite, false);
		}
		else /* I/O error */
			bRet=FALSE; /* ignore error */
	}
	//Clear Comm Error
	ClearCommError(m_hComm, &ErrorFlags, &ComStat);

	return bRet;
}

int CSerialCom::Receive(LPSTR inbuf, int len)
{
	CSingleLock lockObj((CSyncObject*) m_pEvent,FALSE);
	// argument value is not valid
	if (len == 0)
		return -1;
	else if  (len > MAX_BUF)
		return -1;

	if (m_nLength == 0) {
		inbuf[0] = '\0';
		return 0;
	}
	// 정상적이라면 본루틴으로 들어와 실제 들어온 데이터의 길이 만큼 Input Buffer에서 데이터를 읽음
	else if (m_nLength <= len) {
		lockObj.Lock();
		memcpy(inbuf,m_sInBuf,m_nLength);
		memset(m_sInBuf,0,MAX_BUF*2);
		int tmp = m_nLength;
		m_nLength = 0;
		lockObj.Unlock();
		return tmp;
	}
	else {
		lockObj.Lock();
		memcpy(inbuf,m_sInBuf,len);
		memmove(m_sInBuf,m_sInBuf+len,MAX_BUF*2-len);
		m_nLength -= len;
		lockObj.Unlock();
		return len;
	}
}

UINT CSerialCom::CommThread(LPVOID lpData)
{
	extern short   g_nRemoteStatus;
	DWORD          ErrorFlags;
	COMSTAT        ComStat;
	DWORD          EvtMask;
	char           buf[MAX_BUF];
	DWORD          Length;
	int            size;
	int            insize = 0;

	// 통신클래스의 객체포인터를 얻음
	CSerialCom* Comm = (CSerialCom*)lpData;

	// 통신포트가 열려 있다면
	while (Comm->m_bIsOpenned) {
		EvtMask = 0;
		Length = 0;
		insize = 0;
		memset(buf,'\0',MAX_BUF);
		// 이벤트를 기다림
		WaitCommEvent(Comm->m_hComm,&EvtMask, NULL);
		ClearCommError(Comm->m_hComm, &ErrorFlags, &ComStat);
		// EV_RXCHAR에서 이벤트가 발생하면
		if ((EvtMask & EV_RXCHAR) && ComStat.cbInQue) {
			if (ComStat.cbInQue > MAX_BUF)
				size = MAX_BUF;
			else
				size = ComStat.cbInQue;
			do {
				ClearCommError(Comm->m_hComm, &ErrorFlags, &ComStat);
				// overlapped I/O를 통해 데이터를 읽음
				if (!ReadFile(Comm->m_hComm,buf+insize,size,&Length,&(Comm->m_OLR))) {
					// 에러
					TRACE("Error in ReadFilen");
					if (GetLastError() == ERROR_IO_PENDING)       {
						if (WaitForSingleObject(Comm->m_OLR.hEvent, 1000) != WAIT_OBJECT_0)
							Length = 0;
						else
							GetOverlappedResult(Comm->m_hComm,&(Comm->m_OLR),&Length,FALSE);
					}
					else
						Length = 0;
				}
				insize += Length;
			} while ((Length!=0) && (insize<size));
			ClearCommError(Comm->m_hComm, &ErrorFlags, &ComStat);

			if (Comm->m_nLength + insize > MAX_BUF*2)
				insize = (Comm->m_nLength + insize) - MAX_BUF*2;

			// 이벤트 발생을 잠시 중단하고 input buffer에 데이터를 복사
			Comm->m_pEvent->ResetEvent();
			memcpy(Comm->m_sInBuf+Comm->m_nLength,buf,insize);
			Comm->m_nLength += insize;
			// 복사가 끝나면 다시 이벤트를 활성화 시키고
			Comm->m_pEvent->SetEvent();
			LPARAM temp=(LPARAM)Comm;
			// 데이터가 들어왔다는 메시지를 발생
			SendMessage(Comm->m_hWnd,WM_MYRECEIVE,Comm->m_nLength,temp);
		}
	}
	PurgeComm(Comm->m_hComm, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
	LPARAM temp=(LPARAM)Comm;
	// 쓰레드가 종료될 때 종료 메시지를 보냄
	SendMessage(Comm->m_hWnd,WM_MYCLOSE,0,temp);
	return 0;
}																																																																																													
