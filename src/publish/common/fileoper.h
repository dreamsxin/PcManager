

#pragma once

#include <atlfile.h>
#include <atlconv.h>

class CFileOper
{
public:
	CFileOper(){}
	~CFileOper(){}

	BOOL GetFileContentA(LPCTSTR lpszFilename, CStringA& strA)
	{
		CAtlFile file;
		if( FAILED( file.Create(lpszFilename, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING) ) )
			return FALSE;

		BOOL bRet = FALSE;
		do 
		{
			ULONGLONG filesize = 0;
			if( FAILED( file.GetSize(filesize) ) ) 
				break;

			strA = "";
			if(filesize>0)
			{
				file.Read( strA.GetBuffer((int)filesize), (DWORD)filesize );
				strA.ReleaseBuffer();
			}
			bRet = TRUE;
		} while (FALSE);
		file.Close();
		return bRet;
	}

	BOOL GetFileContent(LPCTSTR lpszFilename, CString &strBuffer)
	{
		CStringA strA;
		BOOL bRet = GetFileContentA(lpszFilename, strA);

		USES_CONVERSION;
		strBuffer = CA2CT( strA );		
		return bRet;
	}

	BOOL GetFileBuffer(LPCTSTR lpszFilename, BYTE* & pBuffer, int & nLen)
	{
		CAtlFile file;

		pBuffer = NULL;
		nLen	= 0;

		if( FAILED( file.Create(lpszFilename, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING) ) )
			return FALSE;

		BOOL bRet = FALSE;
		do 
		{
			ULONGLONG filesize = 0;
			if( FAILED( file.GetSize(filesize) ) ) 
				break;

			if(filesize>0)
			{
				pBuffer = new BYTE[(int)filesize+1];
				nLen	= (int)filesize;
				memset(pBuffer,0,nLen+1);
				file.Read( pBuffer, nLen );
			}
			bRet = TRUE;
		} while (FALSE);
		file.Close();
		return bRet;
	}
};