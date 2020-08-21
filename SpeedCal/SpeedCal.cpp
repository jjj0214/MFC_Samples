#include "stdafx.h"
#include "SpeedCal.h"


CSpeedCal::CSpeedCal(void)
{
}


CSpeedCal::~CSpeedCal(void)
{
}


//프로그램이나 클래스 시작부분에 선언
void CSpeedCal::Init(void)
{
  QueryPerformanceFrequency( &m_Frequency );
}


void CSpeedCal::Start(void)
{
  QueryPerformanceCounter( &m_BeginTime );
}


CString CSpeedCal::End(CString strName)
{
	double dTime;
	CString strTmp;
  QueryPerformanceCounter( &m_EndTime );
	dTime = GetSpeed();
	strTmp.Format("Time[%s]: %0.2f ms", strName, dTime);
	TRACE(strTmp);
	return strTmp;
}

double CSpeedCal::GetTime(void)
{
  __int64 elapsed;
	double dTime;
  QueryPerformanceCounter( &m_EndTime );
  elapsed = m_EndTime.QuadPart- m_BeginTime.QuadPart; 
  dTime = (double)elapsed / (double)m_Frequency.QuadPart;
 
  dTime *=1000;  //ms로 변환
	return dTime;
}


double CSpeedCal::GetSpeed(void)
{
  __int64 elapsed;
	double dTime;
  elapsed = m_EndTime.QuadPart- m_BeginTime.QuadPart; 
  dTime = (double)elapsed / (double)m_Frequency.QuadPart;
 
  dTime *=1000;  //ms로 변환
	return dTime;
}
