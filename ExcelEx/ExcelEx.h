#pragma once

#include "CApplication.h"
#include "CBorder.h"
#include "CBorders.h"
#include "CPicture.h"
#include "CPictures.h"
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"

class CExcelEx
{
public:
	CExcelEx(void);
	~CExcelEx(void);
protected:
	// spreadsheet variables
	CApplication	m_app;
	CWorkbook			m_book;
	CWorkbooks		m_books;
	CWorksheet		m_sheet;
	CWorksheets		m_sheets;
	COleVariant		m_covTrue;
	COleVariant		m_covFalse;
	COleVariant		m_covOptional;
	void usrTerminateProcess(LPCTSTR szImageName);
	CString GetExcelPos(int nCol, int nRow);
public:
	BOOL					m_bIsExcelStarted;
	BOOL					m_bIsSheetSelected;
	void CloseWorkBook(void);
	void OpenWorkBook(CString strFilePath);
	void CreateWorkBook(CString strFilePath);
	void SelectSheet(int nNum);
	void SelectSheet(CString strName);
	void SetSheetName(CString strName);
	void AddSheet(CString strName="");
	void SaveData(void);
	int InsertPicture(int nCol, int nRow, CString strPicPath);	//Return: 그림 크기에 따라 삽입 이후에 엑셀에 표시 가능한 row 위치 반환
	void InsertString(int nCol, int nRow, CString strValue);
	void ViewSheetNamesExist(CComboBox* pCbBox);
	void StartExcel(void);
	void QuitExcel(void);
	void ShowExcel(BOOL bShow);
	CString ReadData(int nCol, int nRow);
protected:
	void Initialize(void);
};

