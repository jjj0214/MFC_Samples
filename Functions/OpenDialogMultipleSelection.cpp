
void CTabSetupSpec::OnBnClickedBtnMultipleSelection()
{
	CMainFrame *pFrame  = ((CMainFrame*)AfxGetApp()->m_pMainWnd);
	CString strFullPath[100];
	int nCntFile=0;
  char strFilter[] = { "Datasheet File(*.xlsx, *.xls)|*.xlsx;*.xls|All Files (*.*)|*.*||" };
	
	//CFileDialog dlg(TRUE, "Datasheet to CPK", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT, strFilter, NULL);
	CFileDialog dlg(TRUE, "Datasheet to CPK", NULL, OFN_ALLOWMULTISELECT, strFilter, NULL);
	
	CExcelEx *pExcel;

	//Domodal 전에 File 이름관련 buffer 설정
	CString data;
	dlg.m_pOFN->nMaxFile = (256*(MAX_PATH+1))+1;
	dlg.m_pOFN->lpstrFile = data.GetBuffer((256*(MAX_PATH+1))+1);
	//Model Cancel 선택 시 return
	if( dlg.DoModal() != IDOK )
	{
		return;
	}
	
	POSITION pos ( dlg.GetStartPosition() );
	
	CString sStr;

	pExcel = new CExcelEx();

	//Excel Start
	if(!pExcel->StartExcel())
	{
		//Excel Process End
		delete(pExcel);
		return;
	}
	
	while( pos )
	{
		strFullPath[nCntFile++]= dlg.GetNextPathName(pos);
	}

	data.ReleaseBuffer();
	
	for (int nIdx = 0; nIdx < nCntFile; nIdx++)
	{
		//do something with the filename variable
		if(("xlsx" == strFullPath[nIdx].Right(4)) || (".xls" == strFullPath[nIdx].Right(4)))
		{
      //Doing stuffs
		}
	}
	
	pExcel->QuitExcel();
	
	Sleep(500);

	//Excel Process End
	delete(pExcel);
}
