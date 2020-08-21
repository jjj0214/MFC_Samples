#pragma once

#include "CExcelApplication.h"
#include "CExcelBorder.h"
#include "CExcelBorders.h"
#include "CExcelPicture.h"
#include "CExcelPictures.h"
#include "CExcelRange.h"
#include "CExcelWorkbook.h"
#include "CExcelWorkbooks.h"
#include "CExcelWorksheet.h"
#include "CExcelWorksheets.h"
#include "CExcelFont.h"

class CExcelEx
{
public:
	CExcelEx(void);
	~CExcelEx(void);
protected:
	// spreadsheet variables
	CExcelApplication	m_app;
	CExcelWorkbook			m_book;
	CExcelWorkbooks		m_books;
	CExcelWorksheet		m_sheet;
	CExcelWorksheets		m_sheets;
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
	BOOL SaveData(void);
	BOOL SaveAs(CString strFilePath);
	int InsertPicture(int nCol, int nRow, CString strPicPath);	//Return: 그림 크기에 따라 삽입 이후에 엑셀에 표시 가능한 row 위치 반환
	void InsertString(int nCol, int nRow, CString strValue);
	void SetFont(int nCol, int nRow, long lValue);
	void ViewSheetNamesExist(CComboBox* pCbBox);
	void StartExcel(void);
	void QuitExcel(void);
	void ExitExcelProcess(void);
	void ShowExcel(BOOL bShow);
	CString ReadData(int nCol, int nRow);
protected:
	void Initialize(void);
};

