// MyAdodb.cpp: implementation of the CMyAdodb class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyAdodb.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyAdodb::CMyAdodb()
{
	CoInitialize(0);
	m_IsConnectionOpen=FALSE;
}

CMyAdodb::~CMyAdodb()
{
	CoUninitialize();
}

BOOL CMyAdodb::InitDB(CString strUserId, CString strPassWord, CString strDbIp, CString strDbName, CString strPort)
{	
	CString strConn = "";
	strConn.Format("User ID=%s;Password=%s;Data Source=%s;"
					"Initial Catalog=%s;Network Address=%s,%s;"
					"Network Library=dbmssocn", 
					strUserId, strPassWord, strDbIp, strDbName, strDbIp, strPort);

//	m_pConn = new _ConnectionPtr("ADODB.Connection"); 

	m_pConn.CreateInstance("ADODB.Connection"); 

	m_pRs.CreateInstance("ADODB.Recordset");
	m_pRs->CursorType = adOpenDynamic;

	m_pComm = NULL;
	m_pConn->Provider = "SQLOLEDB";
	m_pConn->ConnectionString = strConn.GetBuffer(0);  

	try
	{
		m_pConn->Open("","","", adConnectUnspecified);
	}catch(...)
	{
		//first-chance exception
		return FALSE;
	}

	m_pComm.CreateInstance(__uuidof(Command));
	m_pComm->ActiveConnection = m_pConn;
	m_pComm->CommandType = adCmdText;
	m_IsConnectionOpen=TRUE;

	return TRUE;
}


void CMyAdodb::ReleaseDB()
{
	CString strTmp;
	try
	{
		if(m_IsConnectionOpen)
		{
			m_IsConnectionOpen=FALSE;
			//Close 시 에러 나는 상황 대비
			m_pRs->putref_ActiveConnection(NULL);
			//m_pRs->Close();
			m_pConn->Close();
		}
	}
	catch(_com_error &e)
	{
		_bstr_t btSource(e.Source());
		_bstr_t btDescription(e.Description());

		strTmp.Format("DumpError [%s], [%s]", (LPCTSTR) btSource, (LPCTSTR)btDescription);

		TRACE((LPTSTR)(LPCTSTR)strTmp);
	}
}
