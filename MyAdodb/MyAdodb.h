// MyAdodb.h: interface for the CMyAdodb class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYADODB_H__9FE345EF_CFCE_4C9C_A8BF_DEB0CE75CAE2__INCLUDED_)
#define AFX_MYADODB_H__9FE345EF_CFCE_4C9C_A8BF_DEB0CE75CAE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyAdodb  
{
public:
	CMyAdodb();
	virtual ~CMyAdodb();
	
	BOOL InitDB(CString strUserId, CString strPassWord, CString strDbIp, CString strDbName, CString strPort);
	void ReleaseDB();

	// MsSQL 컨넥션 객체
	_ConnectionPtr m_pConn; 
	_RecordsetPtr m_pRs; 
	_CommandPtr m_pComm;
	BOOL m_IsConnectionOpen;

};

#endif // !defined(AFX_MYADODB_H__9FE345EF_CFCE_4C9C_A8BF_DEB0CE75CAE2__INCLUDED_)
