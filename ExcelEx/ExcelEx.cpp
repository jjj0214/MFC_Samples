#include "stdafx.h"
#include "ExcelEx.h"
#include "tlhelp32.h"

const short xlBorderWeightThin = 2;
const double dbThresholdPicture = 1.333;

CExcelEx::CExcelEx(void)
{
	Initialize();
}


CExcelEx::~CExcelEx(void)
{
	QuitExcel();
}

void CExcelEx::SaveData(void)
{
	TRY
	{
		// save
		m_book.Save();
	}
	//Clean up if something went wrong.
	CATCH(CException, e)
	{
		AfxMessageBox("Could not save data");
	}
	END_CATCH
}

void CExcelEx::SetFont(int nCol, int nRow, long lValue)
{
	CExcelRange range;
	CExcelBorder border;
	CExcelFont font;
	CString strPos = GetExcelPos(nCol,nRow);
	
	range = m_sheet.get_Range(COleVariant(strPos), COleVariant(strPos));
	font = range.get_Font();
	font.put_Color(COleVariant(COleVariant(lValue)));
}

void CExcelEx::SaveAs(CString strFilePath)
{
	TRY
	{
		// save
		m_book.SaveAs(COleVariant(strFilePath),
			m_covOptional,m_covOptional,m_covOptional,m_covOptional,
			m_covOptional,1,m_covOptional,m_covOptional,
			m_covOptional,m_covOptional);
	}
	//Clean up if something went wrong.
	CATCH(CException, e)
	{
		AfxMessageBox("Could not save data");
	}
	END_CATCH
}

void CExcelEx::InsertString(int nCol, int nRow, CString strValue)
{
	CExcelRange range;
	CExcelBorder border;
	CString strPos = GetExcelPos(nCol,nRow);

	if(m_bIsSheetSelected==TRUE)
	{
		range = m_sheet.get_Range(COleVariant(strPos), COleVariant(strPos));
		range.Select();
	
		//range.get_Font();
		//border = range.get_Borders();
		//border.put_Weight(COleVariant(xlBorderWeightThin));
		range.put_Value(COleVariant(strValue));
	}
	else
	{
		AfxMessageBox ("Sheet is not selected. Select a sheet first.");
	}
}

CString CExcelEx::GetExcelPos(int nCol, int nRow)
{
	CString strRet;
	int nDiv=0, nMod=0;
	if(nCol>26)
	{
		nDiv = nCol / 26;
		nMod = nCol % 26;
		strRet.Format("%c%c%d", 0x40+nDiv, 0x40+nMod, nRow);
	}
	else
	{
		strRet.Format("%c%d", 0x40+nCol, nRow);
	}
	return strRet;
}

int CExcelEx::InsertPicture(int nCol, int nRow, CString strPicPath)
{
	POSITION pos;
	CRange range;
	CPicture pic;
	CPictures pics;
	CBorder border;
	CString strPos = GetExcelPos(nCol,nRow);
	int nNextRow;

	if(m_bIsSheetSelected==TRUE)
	{
		pics = m_sheet.Pictures(m_covOptional);
		//pics.Cut();	//시트 내 사진 지우기

		// get pointer to the first image

		// insert first picture and put a border on it
		range = m_sheet.get_Range(COleVariant(strPos), COleVariant(strPos));
		range.Select ();
		pic = pics.Insert (strPicPath, m_covOptional);
		border = pic.get_Border ();
		border.put_Weight(COleVariant(xlBorderWeightThin));

		// get cell height
		VARIANT var = range.get_Height ();
		double d = var.dblVal;

		// get picture height (in cell units)
		double w = pic.get_Width ();
		double h = pic.get_Height ()*dbThresholdPicture; // in row height units
		int cells = (int) (h/d)+1;
		cells += ((h/d - cells) > 0);
		
		nNextRow = cells + 1;

		// jump to the top of the sheet
		range = m_sheet.get_Range(COleVariant("A1"), COleVariant("A1"));
		range.Show ();
	}
	else
	{
		AfxMessageBox ("Sheet is not selected. Select a sheet first.");
		return -1;
	}
	return nNextRow;
}


void CExcelEx::CloseWorkBook(void)
{
	TRY
	{
		// close already-opened workbook
		m_book.ReleaseDispatch();
		m_books.Close();
		m_books.ReleaseDispatch();
		m_bIsSheetSelected=FALSE;
	}
	//Clean up if something went wrong.
	CATCH(CException, e)
	{
		AfxMessageBox ("Could not clean up workbook.");
	}
	END_CATCH
}


void CExcelEx::OpenWorkBook(CString strFilePath)
{
	// close already-opened workbook
	CloseWorkBook();

	// open the specified workbook
	TRY
	{
		if(m_bIsExcelStarted==TRUE)
		{
			// Get Workbooks collection.
			LPDISPATCH lpDisp;
			lpDisp = m_app.get_Workbooks();  // Get an IDispatch pointer
			ASSERT(lpDisp);
			m_books.AttachDispatch( lpDisp );  // Attach the IDispatch pointer to the books object.

			// open the document
			lpDisp = m_books.Open(strFilePath,
							m_covOptional, m_covOptional, m_covOptional, m_covOptional,
							m_covOptional, m_covOptional, m_covOptional, m_covOptional,
							m_covOptional, m_covOptional, m_covOptional, m_covOptional
							);
			ASSERT(lpDisp);

			//Set CWorkbook to use lpDisp, the IDispatch* of the actual workbook.
			m_book.AttachDispatch(lpDisp);
		}
		else
		{
			AfxMessageBox ("Excel is not started. Start Excel first.");
		}
	}
	//Clean up if something went wrong.
	CATCH(CException, e)
	{
		AfxMessageBox ("Could not open workbook.");
		Initialize();
		return;
	}
	END_CATCH
}

void CExcelEx::CreateWorkBook(CString strFilePath)
{
	// close already-opened workbook
	CloseWorkBook();

	// open the specified workbook
	TRY
	{
		if(m_bIsExcelStarted==TRUE)
		{
			// Get Workbooks collection.
			LPDISPATCH lpDisp;
			lpDisp = m_app.get_Workbooks();  // Get an IDispatch pointer
			ASSERT(lpDisp);
			m_books.AttachDispatch( lpDisp );  // Attach the IDispatch pointer to the books object.
			
			// Create the document
			lpDisp = m_books.Add(m_covOptional);
			ASSERT(lpDisp);

			// Set CWorkbook to use lpDisp, the IDispatch* of the actual workbook.
			m_book.AttachDispatch(lpDisp);

			m_book.SaveAs(COleVariant(strFilePath),
				m_covOptional,m_covOptional,m_covOptional,m_covOptional,
				m_covOptional,1,m_covOptional,m_covOptional,
				m_covOptional,m_covOptional);
		}
		else
		{
			AfxMessageBox ("Excel is not started. Start Excel first.");
		}
	}
	//Clean up if something went wrong.
	CATCH(CException, e)
	{
		AfxMessageBox ("Could not create workbook.");
		Initialize();
		return;
	}
	END_CATCH
}

void CExcelEx::usrTerminateProcess(LPCTSTR szImageName)
{
	HANDLE         hProcessSnap = NULL;
	BOOL           bRet = FALSE;
	PROCESSENTRY32 pe32 = { 0 };

	//  Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == INVALID_HANDLE_VALUE)
		return;

	//  Fill in the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);

	//  Walk the snapshot of the processes, and for each process,
	//  display information.

	if (Process32First(hProcessSnap, &pe32))
	{
		do
		{
			BOOL bTerminate = FALSE;
			if (_stricmp(pe32.szExeFile, szImageName) == 0)
			{
				bTerminate = TRUE;
			}
			else
			{
				int nCount = strlen(pe32.szExeFile), nCount2 = strlen(szImageName) + 1;
				char szExeFile[255] = { 0, };

				for (; ; )
				{
					if (nCount2 == -1) break;
					szExeFile[nCount2] = pe32.szExeFile[nCount];
					nCount2--;
					nCount--;
				}
				if (szExeFile[0] == '\\')
				{
					strcpy_s(szExeFile, &(szExeFile[1]));
				}
				if (_stricmp(szExeFile, szImageName) == 0)
				{
					bTerminate = TRUE;
				}
			}

			if (bTerminate)
			{
				// terminate
				HANDLE hProcess = NULL;
				if (hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID))
				{
					bRet = TerminateProcess(hProcess, 0);
					CloseHandle(hProcess);
				}
			}
		} while (Process32Next(hProcessSnap, &pe32));
		bRet = TRUE;
	}
	else
		bRet = FALSE;    // could not walk the list of processes
	// Do not forget to clean up the snapshot object.

	CloseHandle(hProcessSnap);
}

void CExcelEx::AddSheet(CString strName)
{
	TRY
	{
		CWorksheets sheets=m_book.get_Worksheets();
		CWorksheet sheetLast = sheets.get_Item(COleVariant((short)(sheets.get_Count()))); // sheets are indexed starting from 1
		CWorksheet sheetFirst = sheets.get_Item(COleVariant(1L)); // sheets are indexed starting from 1
		COleVariant vDefault;
		vDefault.vt = VT_DISPATCH;
		vDefault.pdispVal = (LPDISPATCH)sheetLast;

		CWorksheet sheetAdded = sheets.Add(m_covOptional, vDefault, COleVariant(1L),COleVariant(-4167L));
		if(strName!="")
			sheetAdded.put_Name(strName);
		sheetFirst.Activate();
	}
	//Clean up if something went wrong.
	CATCH(CException, e)
	{
		AfxMessageBox("Could not add sheet");
	}
	END_CATCH
}

void CExcelEx::ViewSheetNamesExist(CComboBox* pCbBox)
{
	// populate combo fields
	TRY
	{
		CWorksheets sheets;
		CWorksheet sheet;
		sheets = m_book.get_Worksheets();

		// get all the worksheet names and populate the combo box
		int count = sheets.get_Count();
		if (count)
		{
			for (int i = 0; i < count; i++)
			{
				sheet = sheets.get_Item(COleVariant((short)(i+1))); // sheets are indexed starting from 1
				CString name = sheet.get_Name ();
				pCbBox->AddString (name);
			}
			pCbBox->SetCurSel(0);
			//pCbBox->EnableWindow();
		}
	}
	//Clean up if something went wrong.
	CATCH(CException, e)
	{
		AfxMessageBox ("Could not get sheet names.");
	}
	END_CATCH
}


void CExcelEx::StartExcel(void)
{
	// start excel process
	TRY
	{
		if(!m_app.CreateDispatch("Excel.Application"))
		{
			AfxMessageBox("Could not start Excel.");
		}
		m_app.put_DisplayAlerts (VARIANT_FALSE);
		m_app.put_UserControl(FALSE);
		m_bIsExcelStarted=TRUE;
	}
	//Clean up if something went wrong.
	CATCH(CException, e)
	{
		AfxMessageBox ("Could not start Excel.");
	}
	END_CATCH
}


void CExcelEx::QuitExcel(void)
{
	CloseWorkBook();
	TRY
	{
		m_app.Quit();
		m_app.ReleaseDispatch();
	}
	//Clean up if something went wrong.
	CATCH(CException, e)
	{
		AfxMessageBox ("Could not quit Excel.");
	}
	END_CATCH
	usrTerminateProcess("EXCEL.EXE");
}


void CExcelEx::SelectSheet(int nNum)
{
	int nCnt;
	
	m_sheets = m_book.get_Worksheets();
	nCnt = m_sheets.get_Count();
	if(nNum<=nCnt)
	{
		m_sheet = m_sheets.get_Item(COleVariant((short)nNum));
		m_bIsSheetSelected=TRUE;
		m_sheet.Select(m_covOptional);
		m_app.put_UserControl(TRUE);
	}
	else
	{
		AfxMessageBox("There is no sheet number you select."); //선택한 번호보다 Sheet 카운트가 적은 경우
	}
}


void CExcelEx::SelectSheet(CString strName)
{
	m_sheets = m_book.get_Worksheets();
	m_sheet = m_sheets.get_Item(COleVariant(strName));
	m_sheet.Select(m_covOptional);
	m_bIsSheetSelected=TRUE;
	m_app.put_UserControl(TRUE);
}

void CExcelEx::SetSheetName(CString strName)
{
	if(m_bIsSheetSelected)
	{
		m_sheet.put_Name(strName);
	}
	else
	{
		AfxMessageBox("There is no sheet number you select."); //선택한 번호보다 Sheet 카운트가 적은 경우
	}
}


void CExcelEx::ShowExcel(BOOL bShow)
{
	if(m_bIsExcelStarted==TRUE)
	{
		m_app.put_Visible(bShow);
	}
	else
	{
		AfxMessageBox("Excel is not started yet. Start excel first.");
	}
}


void CExcelEx::Initialize(void)
{
	m_covTrue = ((short)TRUE);
	m_covFalse = ((short)FALSE);
	m_covOptional = COleVariant((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	m_bIsExcelStarted=FALSE;
	m_bIsSheetSelected=FALSE;
	QuitExcel();	//처음 시작시 엑셀 프로그램 열기 오류 방지
}


CString CExcelEx::ReadData(int nCol, int nRow)
{
	CString strPos;
	CString strRet="";
	CRange range;
	if(m_bIsSheetSelected==TRUE)
	{
		strPos = GetExcelPos(nCol,nRow);
		range = m_sheet.get_Range(COleVariant(strPos), COleVariant(strPos));
		strRet = range.get_Value();
	}
	else
	{
		AfxMessageBox("Sheet is not selected yet. Select sheet first.");
	}
	return strRet;
}
