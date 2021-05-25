/*
Source File : AutoUpdate.h
Created for the purpose of demonstration for http://www.codeproject.com

Copyright 2017 Michael Haephrati, Secured Globe Inc.
See also: https://www.codeproject.com/script/Membership/View.aspx?mid=5956881

Secured Globe, Inc.
http://www.securedglobe.com

modified by: Jong Jin Jeong
21-05-25
*/

#pragma once

class SG_Version
{
public:
	int Major;
	int Minor;
	int Revision;
	int SubRevision;
	
	bool SG_Version::operator==(const SG_Version &other) const{
		if(
			(other.Major==Major)
			&& (other.Minor==Minor)
			&& (other.Revision==Revision)
			&& (other.SubRevision==SubRevision)
			)
			return TRUE;
		else
			return FALSE;
	};
	
	bool SG_Version::operator>(const SG_Version &other) const{
		if(Major>other.Major)
			return TRUE;
		else if(Minor>other.Minor)
			return TRUE;
		else if(Revision>other.Revision)
			return TRUE;
		else if(SubRevision>other.SubRevision)
			return TRUE;
		else
			return FALSE;
	};
	
	const SG_Version& SG_Version::operator=(const SG_Version &ci)
	{
		Major = ci.Major;
		Minor = ci.Minor;
		Revision = ci.Revision;
		SubRevision = ci.SubRevision;

		return *this;
	}
};

class AutoUpdate 
{
public:
	AutoUpdate();
	~AutoUpdate();
	BOOL SG_GetVersion(CString ExeFile, SG_Version *ver);
	BOOL GetCurrVersion();
	BOOL GetTargetVersion();
	CString GetVersiontoString(int nMode);
	CString GetSelfFullPath();
	BOOL ReplaceTempVersion();
	BOOL CheckUpdatesExist(void);
	BOOL CheckVersionMatch(void);
	BOOL UpdateFile(void);
	BOOL OpenNewFile(void);
	void CreateFolder(CString sPath);
	void SetSelfFullPath(CString Path);
	CString GetSelfFileName();
	void SetSelfFileName(CString FileName);
	BOOL AutoUpdate::ApplyAgentUpdate(CString Agent, BOOL ReplaceSelf);

private:
	CString m_SelfFullPath;
	CString m_SelfFolderPath;
	CString m_SelfFileName;
	CString m_SelfFileNamewoExe;
	CString m_TargetFileName;
	CString m_DownloadLink;
	CString m_sModifiedFileName;
	BOOL _downloading;
	SG_Version m_CurrVersion, m_TargetVersion;
};


using namespace std;

class MyCallback : public IBindStatusCallback
{
public:
	MyCallback() {}

	~MyCallback() { }

	// This one is called by URLDownloadToFile
	STDMETHOD(OnProgress)(/* [in] */ ULONG ulProgress, /* [in] */ ULONG ulProgressMax, /* [in] */ ULONG ulStatusCode, /* [in] */ LPCWSTR wszStatusText)
	{
		// You can use your own logging function here
		wprintf(L"Downloaded %d of %d. Status code %d\n", ulProgress, ulProgressMax, ulStatusCode);
		return S_OK;
	}

	STDMETHOD(OnStartBinding)(/* [in] */ DWORD dwReserved, /* [in] */ IBinding __RPC_FAR *pib)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(GetPriority)(/* [out] */ LONG __RPC_FAR *pnPriority)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnLowResource)(/* [in] */ DWORD reserved)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnStopBinding)(/* [in] */ HRESULT hresult, /* [unique][in] */ LPCWSTR szError)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(GetBindInfo)(/* [out] */ DWORD __RPC_FAR *grfBINDF, /* [unique][out][in] */ BINDINFO __RPC_FAR *pbindinfo)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnDataAvailable)(/* [in] */ DWORD grfBSCF, /* [in] */ DWORD dwSize, /* [in] */ FORMATETC __RPC_FAR *pformatetc, /* [in] */ STGMEDIUM __RPC_FAR *pstgmed)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnObjectAvailable)(/* [in] */ REFIID riid, /* [iid_is][in] */ IUnknown __RPC_FAR *punk)
	{
		return E_NOTIMPL;
	}

	// IUnknown stuff
	STDMETHOD_(ULONG, AddRef)()
	{
		return 0;
	}

	STDMETHOD_(ULONG, Release)()
	{
		return 0;
	}

	STDMETHOD(QueryInterface)(/* [in] */ REFIID riid, /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject)
	{
		return E_NOTIMPL;
	}
};


