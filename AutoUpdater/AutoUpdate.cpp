/*
Source File : AutoUpdate.cpp
Created for the purpose of demonstration for http://www.codeproject.com

Copyright 2017 Michael Haephrati, Secured Globe Inc.
See also: https://www.codeproject.com/script/Membership/View.aspx?mid=5956881

Secured Globe, Inc.
http://www.securedglobe.com

Modified by: Jong Jin Jeong
2021-05-25

*/

#include "stdafx.h"
#include "AutoUpdate.h"
#include <urlmon.h>    
#include <iostream>
#include <WinInet.h>
#include <direct.h>

#pragma comment (lib, "urlmon.lib")
#pragma comment(lib, "Version.lib")


// Utilities
BOOL SG_Run(CString FileName)
{
	CString sStr;
	TRACE(L"Called SG_Run '%s'\n", FileName);
	PROCESS_INFORMATION ProcessInfo; //This is what we get as an [out] parameter

	STARTUPINFO StartupInfo; //This is an [in] parameter

	ZeroMemory(&StartupInfo, sizeof(StartupInfo));
	StartupInfo.cb = sizeof StartupInfo; //Only compulsory field

	if (CreateProcess(FileName, NULL,
		NULL, NULL, FALSE, 0, NULL,
		NULL, &StartupInfo, &ProcessInfo))
	{
		//WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
		CloseHandle(ProcessInfo.hThread);
		CloseHandle(ProcessInfo.hProcess);
		TRACE(L"Success\n");
		return TRUE;
	}
	else
	{
		TRACE(L"Failed\n");
		return FALSE;
	}

}


CString GetFileNameFromPath(CString Path)
{
	int slash = Path.ReverseFind(L'\\');
	return(Path.Mid(slash + 1));
}

// AutoUpdate class
AutoUpdate::AutoUpdate()
{
	m_DownloadLink = ("C:\\AutoUpdateTest\\");

	TCHAR szPath[MAX_PATH];

	if (!GetModuleFileName(NULL, szPath, MAX_PATH))
	{
		AfxMessageBox(GetLastError());
		return;
	}
	SetSelfFullPath(szPath);
	SetSelfFileName(GetFileNameFromPath(szPath));

	//실행파일 경로
	char	name[256];
	_getcwd(name,256);	
	m_SelfFolderPath = (LPSTR)name;

	ReplaceTempVersion();
}

CString AutoUpdate::GetVersiontoString(int nMode)
{
	SG_Version *pVer;

	if(nMode==0)
		pVer = &m_CurrVersion;
	else
		pVer = &m_TargetVersion;

	CString sStr;
	sStr.Format("%d.%d.%d.%d", pVer->Major, pVer->Minor, pVer->Revision, pVer->SubRevision);
	return sStr;
}

BOOL AutoUpdate::GetCurrVersion()
{
	if(!SG_GetVersion(m_SelfFullPath, &m_CurrVersion))
		return FALSE;

	return TRUE;
}

BOOL AutoUpdate::GetTargetVersion()
{
	if(!SG_GetVersion(m_TargetFileName, &m_TargetVersion))
		return FALSE;

	return TRUE;
}

BOOL AutoUpdate::SG_GetVersion(CString ExeFile, SG_Version *ver)
{
	CString sStr;
	BOOL result = FALSE;
	DWORD dwDummy;
	DWORD dwFVISize = GetFileVersionInfoSize(ExeFile, &dwDummy);
	LPBYTE lpVersionInfo = new BYTE[dwFVISize];
	GetFileVersionInfo(ExeFile, 0, dwFVISize, lpVersionInfo);
	UINT uLen;
	VS_FIXEDFILEINFO *lpFfi;
	VerQueryValue(lpVersionInfo, "\\", (LPVOID *)&lpFfi, &uLen);
	if (lpFfi && uLen)
	{
		DWORD dwFileVersionMS = lpFfi->dwFileVersionMS;
		DWORD dwFileVersionLS = lpFfi->dwFileVersionLS;
		delete[] lpVersionInfo;
		ver->Major = HIWORD(dwFileVersionMS);
		ver->Minor = LOWORD(dwFileVersionMS);
		ver->Revision = HIWORD(dwFileVersionLS);
		ver->SubRevision = LOWORD(dwFileVersionLS);
		TRACE(L"Found version: %d.%d.%d.%d\n", ver->Major, ver->Minor, ver->Revision, ver->SubRevision);
		result = TRUE;
	}
	else
	{
		TRACE(L"Can't detect current version\n");
	}
	return result;
}

AutoUpdate::~AutoUpdate()
{
}


CString AutoUpdate::GetSelfFullPath()
{
	return m_SelfFullPath;
}

void AutoUpdate::SetSelfFullPath(CString Path)
{
	m_SelfFullPath = Path;
}

CString AutoUpdate::GetSelfFileName()
{
	return m_SelfFileName;
}

void AutoUpdate::SetSelfFileName(CString FileName)
{
	m_SelfFileName = FileName;
	m_SelfFileNamewoExe = m_SelfFileName;
	m_SelfFileNamewoExe.Replace(".exe","");
}

BOOL AutoUpdate::ReplaceTempVersion()
{
	CString sStr;
	BOOL result;
	TRACE(L"We are running the normal version\n");
	for(int nTries=0; nTries<5; nTries++)
	{
		result = DeleteFile("#"+ m_SelfFileName);
		if (result)
		{
			TRACE(L"temp File '%s' deleted\n", "#" + m_SelfFileName);
			break;
		}
		else
		{
			if(nTries==5)
			{
				TRACE(L"temp File '%s' can't be deleted or doesn't exist\n", "#" + m_SelfFileName);
			}
		}
	}

	return TRUE;
}

BOOL AutoUpdate::CheckVersionMatch(void)
{
	if(m_TargetVersion==m_CurrVersion)
	{
		TRACE(L"No new version on the server\n");
		DeleteFile(m_TargetFileName);
		return TRUE;
	}
	else
	{
		if(m_TargetVersion>m_CurrVersion)
			return FALSE;
		else
			return TRUE;
	}
}

BOOL AutoUpdate::UpdateFile(void)
{
	CString sOldFolderPath = m_SelfFolderPath+"\\Old";
	CString sStr;

	//Create Old folder
	CreateFolder(sOldFolderPath);

	//Change name of existing file
	//Move File to old folder
	sStr.Format("%s\\%s_%d%d%d.exe", sOldFolderPath, m_SelfFileNamewoExe, 
		m_CurrVersion.Major, m_CurrVersion.Minor, m_CurrVersion.Revision);

	if(!MoveFileEx(m_SelfFullPath, sStr, MOVEFILE_REPLACE_EXISTING))
		return FALSE;

	if(!MoveFileEx(m_TargetFileName, m_SelfFullPath, MOVEFILE_REPLACE_EXISTING))
		return FALSE;

	return TRUE;
}

BOOL AutoUpdate::OpenNewFile(void)
{
	//Start new file
	if (SG_Run(m_SelfFullPath.GetBuffer()))
	{
		TRACE(L"Successfully started the new version (%s)\n", m_SelfFullPath);
		return TRUE;
	}
	else
	{
		TRACE(L"Couldn't start the new version (%s)\n", m_SelfFullPath);
		return FALSE;
	}
}

BOOL AutoUpdate::CheckUpdatesExist(void)
{
	MyCallback pCallback;
	m_TargetFileName = "#" + m_SelfFileName;
	CString URL = m_DownloadLink + m_SelfFileName;
	
	if (URL == "") return FALSE;
	TRACE(L"Looking for files at %s\n", URL);
	DeleteUrlCacheEntry(URL);

	HRESULT hr = 0;
	hr = URLDownloadToFile(
		NULL,   // A pointer to the controlling IUnknown interface (not needed here)
		URL,
		m_TargetFileName,
		0,		      // Reserved. Must be set to 0.
		&pCallback); // status callback interface (not needed for basic use)
	
	if (!SUCCEEDED(hr))
		return FALSE;

	return TRUE;
}

void AutoUpdate::CreateFolder(CString sPath)
{
	if(GetFileAttributes(sPath) == INVALID_FILE_ATTRIBUTES)
		CreateDirectory(sPath,NULL);
}




