#pragma once
class CSpeedCal
{
public:
	CSpeedCal(void);
	~CSpeedCal(void);
protected:
	LARGE_INTEGER m_Frequency;
	LARGE_INTEGER m_BeginTime;
	LARGE_INTEGER m_EndTime;
	double GetSpeed(void);
public:
	void Init(void);
	void Start(void);
	CString End(CString strName);
	double GetTime(void);
};
