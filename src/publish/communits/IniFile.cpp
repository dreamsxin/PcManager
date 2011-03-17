//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IniFile.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////////

#define DEF_PROFILE_NUM_LEN			64 // numeric string length, could be quite long for binary format
#define DEF_PROFILE_THRESHOLD		512 // temporary string length
#define DEF_PROFILE_DELIMITER		_T(",") // default string delimiter
#define DEF_PROFILE_TESTSTRING		_T("{63788286-AE30-4D6B-95DF-3B451C1C79F9}") // Uuid for internal use


//////////////////////////////////////////////////////////////////////////

struct PAIR_DATA
{
	LPTSTR lpTarget;
	DWORD dwRemain;
	DWORD dwTotalCopied;
};

//////////////////////////////////////////////////////////////////////////

CIniFile::CIniFile()
{
	mpsPathName = NULL;
}

//////////////////////////////////////////////////////////////////////////

CIniFile::CIniFile(
	LPCTSTR lpPathName)
{
	mpsPathName = NULL;
	SetPathName(lpPathName);
}

//////////////////////////////////////////////////////////////////////////

CIniFile::~CIniFile()
{
	if (mpsPathName != NULL)
		delete [] mpsPathName;
}

//////////////////////////////////////////////////////////////////////////

void CIniFile::SetPathName(
	LPCTSTR lpPathName)
{
	if (lpPathName == NULL)
	{
		if (mpsPathName != NULL)
			*mpsPathName = _T('\0');
	}else
	{
		if (mpsPathName != NULL)
			delete [] mpsPathName;

		mpsPathName = _tcsdup(lpPathName);
	}
}

//////////////////////////////////////////////////////////////////////////

DWORD CIniFile::GetPathName(
	LPTSTR lpBuffer, 
	DWORD dwBufSize) const
{
	*lpBuffer = _T('\0');

	DWORD dwLen = 0;

	if (lpBuffer != NULL)
	{
		wcsncpy(lpBuffer, mpsPathName, dwBufSize);
		dwLen = _tcslen(lpBuffer);
	}else
	{
		dwLen = _tcslen(mpsPathName);
	}

	return dwLen;
}

//////////////////////////////////////////////////////////////////////////

CString CIniFile::GetPathName() const
{
	return mpsPathName;
}

//////////////////////////////////////////////////////////////////////////

DWORD CIniFile::GetString(
	LPCTSTR lpSection, 
	LPCTSTR lpKey, 
	LPTSTR lpBuffer, 
	DWORD dwBufSize, 
	LPCTSTR lpDefault) const
{
	if (lpBuffer != NULL)
		*lpBuffer = _T('\0');

	LPTSTR psTemp = GetStringDynamic(lpSection, lpKey, lpDefault);
	DWORD dwLen = _tcslen(psTemp);

	if (lpBuffer != NULL)
	{
		_tcsncpy(lpBuffer, psTemp, dwBufSize);
		dwLen = min(dwLen, dwBufSize);
	}

	delete [] psTemp;
	return dwLen;
}

//////////////////////////////////////////////////////////////////////////

CString CIniFile::GetString(
	LPCTSTR lpSection, 
	LPCTSTR lpKey, 
	LPCTSTR lpDefault) const
{
	LPTSTR psTemp = GetStringDynamic(lpSection, lpKey, lpDefault);
	CString sTemp(psTemp);
	delete [] psTemp;
	return sTemp;
}

//////////////////////////////////////////////////////////////////////////

BOOL CIniFile::WriteString(
	LPCTSTR lpSection, 
	LPCTSTR lpKey, 
	LPCTSTR lpValue) const
{
	if (lpSection == NULL || lpKey == NULL)
		return FALSE;

	return ::WritePrivateProfileString(lpSection, lpKey, lpValue == NULL ? _T("") : lpValue, mpsPathName);
}

//////////////////////////////////////////////////////////////////////////

BOOL CIniFile::AppendString(
	LPCTSTR lpSection, 
	LPCTSTR lpKey, 
	LPCTSTR lpString) const
{
	if (lpString == NULL)
		return FALSE;

	TCHAR *psTemp = GetStringDynamic(lpSection, lpKey);
	TCHAR *psNew = new TCHAR[_tcslen(psTemp) + _tcslen(lpString) + 1];

	_stprintf(psNew, _T("%s%s"), psTemp, lpString);

	const BOOL bRetCode = WriteString(lpSection, lpKey, psNew);

	delete [] psNew;
	delete [] psTemp;

	return bRetCode;
}

//////////////////////////////////////////////////////////////////////////

DWORD CIniFile::GetArray(
	LPCTSTR lpSection, 
	LPCTSTR lpKey, 
	LPTSTR lpBuffer, 
	DWORD dwBufSize, 
	LPCTSTR lpDelimiter, 
	BOOL bTrimString) const
{
	if (lpBuffer != NULL)
		*lpBuffer = _T('\0');

	if (lpSection == NULL || lpKey == NULL)
		return 0;	

	LPTSTR psTemp = GetStringDynamic(lpSection, lpKey);
	
	DWORD dwCopied = 0;

	if (*psTemp != _T('\0'))
	{
		if (lpBuffer == NULL)
		{
			// just calculate the required buffer size
			const DWORD dwMaxLen = _tcslen(psTemp) + 2;
			LPTSTR psSplit = new TCHAR[dwMaxLen + 1];
			dwCopied = StringSplit(psTemp, psSplit, dwMaxLen, lpDelimiter, bTrimString);
			delete [] psSplit;
		}else
		{
			dwCopied = StringSplit(psTemp, lpBuffer, dwBufSize, lpDelimiter, bTrimString);
		}
	}		

	delete [] psTemp;

	return dwCopied;
}

//////////////////////////////////////////////////////////////////////////

void CIniFile::GetArray(
	LPCTSTR lpSection, 
	LPCTSTR lpKey, 
	CAtlArray<CString> *pArray, 
	LPCTSTR lpDelimiter, 
	BOOL bTrimString) const
{
	if (pArray != NULL)
		pArray->RemoveAll();

	const DWORD dwLen = GetArray(lpSection, lpKey, NULL, 0, lpDelimiter);
	if (dwLen == 0)
		return;

	LPTSTR psTemp = new TCHAR[dwLen + 3];

	GetArray(lpSection, lpKey, psTemp, dwLen + 2, lpDelimiter);
	ParseDNTString(psTemp, SubStrAdd, (LPVOID)pArray);

	delete [] psTemp;
}

//////////////////////////////////////////////////////////////////////////

BOOL CIniFile::WriteArray(
	LPCTSTR lpSection, 
	LPCTSTR lpKey, 
	const CAtlArray<CString> *pArray, 
	int nWriteCount, 
	LPCTSTR lpDelimiter) const
{
	if (pArray == NULL)
		return FALSE;

	if (nWriteCount < 0)
		nWriteCount = pArray->GetCount();
	else
		nWriteCount = min(nWriteCount, (int)pArray->GetCount());

	const CString sDelimiter = (lpDelimiter == NULL || *lpDelimiter == _T('\0')) ? _T(",") : lpDelimiter;
	CString sLine;

	for (int nLoop = 0; nLoop < nWriteCount; nLoop++)
	{
		sLine += pArray->GetAt(nLoop);

		if (nLoop != nWriteCount - 1)
			sLine += sDelimiter;
	}

	return WriteString(lpSection, lpKey, sLine);
}


//////////////////////////////////////////////////////////////////////////

int CIniFile::GetInt(
	LPCTSTR lpSection, 
	LPCTSTR lpKey, 
	int nDefault, 
	int nBase) const
{
	TCHAR sTemp[DEF_PROFILE_NUM_LEN + 1] = _T("");

	GetString(lpSection, lpKey, sTemp, DEF_PROFILE_NUM_LEN);

	return *sTemp == _T('\0') ? nDefault : int(_tcstoul(sTemp, NULL, ValidateBase(nBase)));
}

//////////////////////////////////////////////////////////////////////////

UINT CIniFile::GetUInt(
	LPCTSTR lpSection, 
	LPCTSTR lpKey, 
	UINT nDefault, 
	int nBase) const
{
	TCHAR sTemp[DEF_PROFILE_NUM_LEN + 1] = _T("");

	GetString(lpSection, lpKey, sTemp, DEF_PROFILE_NUM_LEN);

	return *sTemp == _T('\0') ? nDefault : UINT(_tcstoul(sTemp, NULL, ValidateBase(nBase)));
}

//////////////////////////////////////////////////////////////////////////

BOOL CIniFile::GetBool(
	LPCTSTR lpSection, 
	LPCTSTR lpKey, 
	BOOL bDefault) const
{
	TCHAR sTemp[DEF_PROFILE_NUM_LEN + 1] = _T("");

	GetString(lpSection, lpKey, sTemp, DEF_PROFILE_NUM_LEN);

	return StringToBool(sTemp, bDefault);
}

//////////////////////////////////////////////////////////////////////////

double CIniFile::GetDouble(
	LPCTSTR lpSection, 
	LPCTSTR lpKey, 
	double fDefault) const
{
	TCHAR sTemp[DEF_PROFILE_NUM_LEN + 1] = _T("");

	GetString(lpSection, lpKey, sTemp, DEF_PROFILE_NUM_LEN);

	return *sTemp == _T('\0') ? fDefault : _tcstod(sTemp, NULL);
}

//////////////////////////////////////////////////////////////////////////

BOOL CIniFile::WriteInt(
	LPCTSTR lpSection, 
	LPCTSTR lpKey, 
	int nValue, 
	int nBase) const
{
	TCHAR szValue[DEF_PROFILE_NUM_LEN + 1] = _T("");

	IntToString(nValue, szValue, nBase);

	return WriteString(lpSection, lpKey, szValue);
}

//////////////////////////////////////////////////////////////////////////

BOOL CIniFile::WriteUInt(
	LPCTSTR lpSection, 
	LPCTSTR lpKey, 
	UINT nValue, 
	int nBase) const
{
	TCHAR szValue[DEF_PROFILE_NUM_LEN + 1] = _T("");

	UIntToString(nValue, szValue, nBase);

	return WriteString(lpSection, lpKey, szValue);
}

//////////////////////////////////////////////////////////////////////////

BOOL CIniFile::WriteDouble(
	LPCTSTR lpSection, 
	LPCTSTR lpKey, 
	double fValue, 
	int nPrecision) const
{
	TCHAR szFmt[16] = _T("%f");

	if (nPrecision > 0)
		_stprintf(szFmt, _T("%%.%df"), nPrecision);

	TCHAR szValue[DEF_PROFILE_NUM_LEN + 1] = _T("");

	_stprintf(szValue, szFmt, fValue);

	return WriteString(lpSection, lpKey, szValue);
}

//////////////////////////////////////////////////////////////////////////

BOOL CIniFile::IncreaseDouble(
	LPCTSTR lpSection, 
	LPCTSTR lpKey, 
	double fIncrease, 
	int nPrecision) const
{
	double f = GetDouble(lpSection, lpKey, 0.0);

	f += fIncrease;

	return WriteDouble(lpSection, lpKey, f, nPrecision);
}

//////////////////////////////////////////////////////////////////////////

BOOL CIniFile::WriteBool(
	LPCTSTR lpSection, 
	LPCTSTR lpKey, 
	BOOL bValue) const
{
	return WriteInt(lpSection, lpKey, bValue ? 1 : 0, BASE_DECIMAL);
}

//////////////////////////////////////////////////////////////////////////

BOOL CIniFile::InvertBool(
	LPCTSTR lpSection, 
	LPCTSTR lpKey) const
{
	return WriteBool(lpSection, lpKey, !GetBool(lpSection, lpKey, FALSE));
}

//////////////////////////////////////////////////////////////////////////

BOOL CIniFile::IncreaseInt(
	LPCTSTR lpSection, 
	LPCTSTR lpKey, 
	int nIncrease, int nBase) const
{
	int nVal = GetInt(lpSection, lpKey, 0, nBase);

	nVal += nIncrease;

	return WriteInt(lpSection, lpKey, nVal, nBase);
}

//////////////////////////////////////////////////////////////////////////

BOOL CIniFile::IncreaseUInt(
	LPCTSTR lpSection, 
	LPCTSTR lpKey, 
	UINT nIncrease, 
	int nBase) const
{
	UINT nVal = GetUInt(lpSection, lpKey, 0, nBase);

	nVal += nIncrease;

	return WriteUInt(lpSection, lpKey, nVal, nBase);
}

//////////////////////////////////////////////////////////////////////////

TCHAR CIniFile::GetChar(
	LPCTSTR lpSection, 
	LPCTSTR lpKey, 
	TCHAR cDefault) const
{
	TCHAR sTemp[2] = _T("");

	GetString(lpSection, lpKey, sTemp, 1);

	return *sTemp == _T('\0') ? cDefault : sTemp[0];
}

//////////////////////////////////////////////////////////////////////////

BOOL CIniFile::WriteChar(
	LPCTSTR lpSection, 
	LPCTSTR lpKey, 
	TCHAR c) const
{
	TCHAR sTemp[2] = { c, _T('\0') };

	return WriteString(lpSection, lpKey, sTemp);
}

//////////////////////////////////////////////////////////////////////////

DWORD CIniFile::GetDataBlock(
	LPCTSTR lpSection, 
	LPCTSTR lpKey, 
	LPVOID lpBuffer, 
	DWORD dwBufSize, 
	DWORD dwOffset) const
{
	LPTSTR psTemp = GetStringDynamic(lpSection, lpKey);

	DWORD dwLen = _tcslen(psTemp) / 2;

	if (dwLen <= dwOffset)
	{
		delete [] psTemp;
		return 0;
	}

	for (int nLoop = 0; psTemp[nLoop] != _T('\0'); nLoop++)
	{
		TCHAR c = psTemp[nLoop];

		if ( (c >= _T('0') && c <= _T('9')) || 
			 (c >= _T('a') && c <= _T('f')) || 
			 (c >= _T('A') && c <= _T('F')) )
		{
		}else
		{
			delete [] psTemp;
			return 0;
		}
	}

	DWORD dwProcLen = 0;
	LPBYTE lpb = (LPBYTE)lpBuffer;

	if (lpb != NULL)
	{
		dwProcLen = min(dwLen - dwOffset, dwBufSize);

		LPCTSTR p = &psTemp[dwOffset * 2];

		for (DWORD dwLoop = 0; dwLoop < dwProcLen; dwLoop++)
		{			
			TCHAR sTemp[3] = _T("");

			_tcsncpy(sTemp, p, 2);			
			lpb[dwLoop] = BYTE(_tcstoul(sTemp, NULL, 16));
			p = &p[2];
		}			
	}else
	{
		dwProcLen = dwLen - dwOffset;
	}

	delete [] psTemp;

	return dwProcLen;
}

//////////////////////////////////////////////////////////////////////////

BOOL CIniFile::WriteDataBlock(
	LPCTSTR lpSection, 
	LPCTSTR lpKey, 
	LPCVOID lpData, 
	DWORD dwDataSize) const
{
	const BYTE *lpb = (const BYTE*)lpData;
	if (lpb == NULL)
		return FALSE;

	LPTSTR psTemp = new TCHAR[dwDataSize * 2 + 1];

	for (DWORD dwLoop = 0, j = 0; dwLoop < dwDataSize; dwLoop++, j += 2)
		_stprintf(&psTemp[j], _T("%02X"), lpb[dwLoop]);

	const BOOL bRetCode = WriteString(lpSection, lpKey, psTemp);

	delete [] psTemp;

	return bRetCode;
}

//////////////////////////////////////////////////////////////////////////

BOOL CIniFile::AppendDataBlock(
	LPCTSTR lpSection, 
	LPCTSTR lpKey, 
	LPCVOID lpData, 
	DWORD dwDataSize) const
{
	const BYTE *lpb = (const BYTE*)lpData;
	if (lpb == NULL)
		return FALSE;

	LPTSTR psTemp = new TCHAR[dwDataSize * 2 + 1];

	for (DWORD dwLoop = 0, j = 0; dwLoop < dwDataSize; dwLoop++, j += 2)
		_stprintf(&psTemp[j], _T("%02X"), lpb[dwLoop]);

	const BOOL bRetCode = AppendString(lpSection, lpKey, psTemp);

	delete [] psTemp;

	return bRetCode;
}

//////////////////////////////////////////////////////////////////////////

POINT CIniFile::GetPoint(
	LPCTSTR lpSection, 
	LPCTSTR lpKey, 
	POINT ptDefault) const
{
	POINT pt;

	if (GetDataBlock(lpSection, lpKey, &pt, sizeof(POINT)) != sizeof(POINT))
		pt = ptDefault;

	return pt;
}

//////////////////////////////////////////////////////////////////////////

RECT CIniFile::GetRect(
	LPCTSTR lpSection, 
	LPCTSTR lpKey, 
	RECT rcDefault) const
{
	RECT rc;

	if (GetDataBlock(lpSection, lpKey, &rc, sizeof(RECT)) != sizeof(RECT))
		rc = rcDefault;

	return rc;
}

//////////////////////////////////////////////////////////////////////////

BOOL CIniFile::WritePoint(
	LPCTSTR lpSection, 
	LPCTSTR lpKey, 
	POINT pt) const
{
	return WriteDataBlock(lpSection, lpKey, &pt, sizeof(POINT));
}

//////////////////////////////////////////////////////////////////////////

BOOL CIniFile::WriteRect(
	LPCTSTR lpSection, 
	LPCTSTR lpKey, 
	RECT rc) const
{
	return WriteDataBlock(lpSection, lpKey, &rc, sizeof(RECT));
}

//////////////////////////////////////////////////////////////////////////

DWORD CIniFile::GetKeyLines(
	LPCTSTR lpSection, 
	LPTSTR lpBuffer, 
	DWORD dwBufSize) const
{
	if (lpBuffer != NULL)
		*lpBuffer = _T('\0');

	if (lpSection == NULL)
		return 0;	

	if (lpBuffer == NULL)
	{
		DWORD dwLen = DEF_PROFILE_THRESHOLD;
		LPTSTR psTemp = new TCHAR[dwLen + 1];
		DWORD dwCopied = ::GetPrivateProfileSection(lpSection, psTemp, dwLen, mpsPathName);

		while (dwCopied + 2 >= dwLen)
		{
			dwLen += DEF_PROFILE_THRESHOLD;

			delete [] psTemp;

			psTemp = new TCHAR[dwLen + 1];
			dwCopied = ::GetPrivateProfileSection(lpSection, psTemp, dwLen, mpsPathName);
		}

		delete [] psTemp;
		return dwCopied + 2;
	}else
	{
		return ::GetPrivateProfileSection(lpSection, lpBuffer, dwBufSize, mpsPathName);
	}
}

//////////////////////////////////////////////////////////////////////////

DWORD CIniFile::GetKeyNames(
	LPCTSTR lpSection, 
	LPTSTR lpBuffer, 
	DWORD dwBufSize) const
{
	if (lpBuffer != NULL)
		*lpBuffer = _T('\0');

	if (lpSection == NULL)
		return 0;	

	PAIR_DATA PairData;	

	PairData.lpTarget = lpBuffer;
	PairData.dwRemain = dwBufSize;
	PairData.dwTotalCopied = 0;

	const DWORD dwLen = GetKeyLines(lpSection, NULL, 0);
	if (dwLen == 0)
		return 0;

	LPTSTR psTemp = new TCHAR[dwLen + 1];

	GetKeyLines(lpSection, psTemp, dwLen);
	ParseDNTString(psTemp, KeyPairProc, (LPVOID)(&PairData));

	delete [] psTemp;

	if (lpBuffer != NULL)
		lpBuffer[PairData.dwTotalCopied] = _T('\0');

	return PairData.dwTotalCopied;
}

//////////////////////////////////////////////////////////////////////////

DWORD CIniFile::GetSectionNames(
	LPTSTR lpBuffer, 
	DWORD dwBufSize) const
{
	if (lpBuffer == NULL)
	{
		DWORD dwLen = DEF_PROFILE_THRESHOLD;
		LPTSTR psTemp = new TCHAR[dwLen + 1];
		DWORD dwCopied = ::GetPrivateProfileSectionNames(psTemp, dwLen, mpsPathName);

		while (dwCopied + 2 >= dwLen)
		{
			dwLen += DEF_PROFILE_THRESHOLD;
			delete [] psTemp;
			psTemp = new TCHAR[dwLen + 1];
			dwCopied = ::GetPrivateProfileSectionNames(psTemp, dwLen, mpsPathName);
		}
		
		delete [] psTemp;
		return dwCopied + 2;
	}else
	{
		return ::GetPrivateProfileSectionNames(lpBuffer, dwBufSize, mpsPathName);
	}
}

//////////////////////////////////////////////////////////////////////////

void CIniFile::GetSectionNames(
	CAtlArray<CString> *pArray) const
{
	if (pArray != NULL)
		pArray->RemoveAll();

	const DWORD dwLen = GetSectionNames(NULL, 0);
	if (dwLen == 0)
		return;

	LPTSTR psTemp = new TCHAR[dwLen + 1];

	GetSectionNames(psTemp, dwLen);
	ParseDNTString(psTemp, SubStrAdd, pArray);

	delete [] psTemp;
}

//////////////////////////////////////////////////////////////////////////

void CIniFile::GetKeyLines(
	LPCTSTR lpSection, 
	CAtlArray<CString> *pArray) const
{
	if (pArray != NULL)
		pArray->RemoveAll();

	const DWORD dwLen = GetKeyLines(lpSection, NULL, 0);
	if (dwLen == 0)
		return;

	LPTSTR psTemp = new TCHAR[dwLen + 1];

	GetKeyLines(lpSection, psTemp, dwLen);
	ParseDNTString(psTemp, SubStrAdd, pArray);

	delete [] psTemp;
}

//////////////////////////////////////////////////////////////////////////

void CIniFile::GetKeyNames(
	LPCTSTR lpSection, 
	CAtlArray<CString> *pArray) const
{
	if (pArray == NULL)
		return;

	pArray->RemoveAll();
	const DWORD dwLen = GetKeyNames(lpSection, NULL, 0);
	LPTSTR psTemp = new TCHAR[dwLen + 1];

	GetKeyNames(lpSection, psTemp, dwLen);
	ParseDNTString(psTemp, SubStrAdd, (LPVOID)pArray);

	delete [] psTemp;
}

//////////////////////////////////////////////////////////////////////////

DWORD CIniFile::GetKeyNameCount(
	LPCTSTR lpSection) const
{
	DWORD dwCount = 0;

	const DWORD dwLen = GetKeyNames(lpSection, NULL, 0);
	LPTSTR psTemp = new TCHAR[dwLen + 1];

	GetKeyNames(lpSection, psTemp, dwLen);
	ParseDNTString(psTemp, SubStrCount, (LPVOID)&dwCount);

	delete [] psTemp;

	return dwCount;
}

//////////////////////////////////////////////////////////////////////////

BOOL CIniFile::DeleteSection(
	LPCTSTR lpSection) const
{
	return ::WritePrivateProfileString(lpSection, NULL, _T(""), mpsPathName);
}

//////////////////////////////////////////////////////////////////////////

BOOL CIniFile::DeleteKey(
	LPCTSTR lpSection, 
	LPCTSTR lpKey) const
{
	return ::WritePrivateProfileString(lpSection, lpKey, NULL, mpsPathName);
}

//////////////////////////////////////////////////////////////////////////

BOOL CIniFile::IsSectionExist(
	LPCTSTR lpSection) const
{
	if (lpSection == NULL)
		return FALSE;

	const DWORD dwLen = GetSectionNames(NULL, 0);
	if (dwLen == 0)
		return FALSE;

	LPTSTR psTemp = new TCHAR[dwLen + 1];

	GetSectionNames(psTemp, dwLen);
	BOOL bRetCode = !ParseDNTString(psTemp, SubStrCompare, (LPVOID)lpSection);

	delete [] psTemp;
	return bRetCode;
}

//////////////////////////////////////////////////////////////////////////

BOOL CIniFile::IsKeyExist(
	LPCTSTR lpSection, 
	LPCTSTR lpKey) const
{
	if (lpSection == NULL || lpKey == NULL)
		return FALSE;

	LPTSTR psTemp = GetStringDynamic(lpSection, lpKey, DEF_PROFILE_TESTSTRING);
	const BOOL bRetCode = (_tcscmp(psTemp, DEF_PROFILE_TESTSTRING) != 0);
	delete [] psTemp;
	return bRetCode;
}

//////////////////////////////////////////////////////////////////////////

BOOL CIniFile::CopySection(
	LPCTSTR lpSrcSection, 
	LPCTSTR lpDestSection, 
	BOOL bFailIfExist) const
{
	if (lpSrcSection == NULL || lpDestSection == NULL)
		return FALSE;

	if (_tcsicmp(lpSrcSection, lpDestSection) == 0)
		return FALSE;

	if (!IsSectionExist(lpSrcSection))
		return FALSE;

	if (bFailIfExist && IsSectionExist(lpDestSection))
		return FALSE;

	DeleteSection(lpDestSection);

	const DWORD dwSrcLen = GetKeyLines(lpSrcSection, NULL, 0);
	LPTSTR psTemp = new TCHAR[dwSrcLen + 2];

	GetKeyLines(lpSrcSection, psTemp, dwSrcLen);	
	const BOOL bRetCode = ::WritePrivateProfileSection(lpDestSection, psTemp, mpsPathName);
	delete [] psTemp;

	return bRetCode;
}

//////////////////////////////////////////////////////////////////////////

BOOL CIniFile::CopyKey(
	LPCTSTR lpSrcSection, 
	LPCTSTR lpSrcKey, 
	LPCTSTR lpDestSection, 
	LPCTSTR lpDestKey, 
	BOOL bFailIfExist) const
{
	if (lpSrcSection == NULL || lpSrcKey == NULL || lpDestKey == NULL)
		return FALSE;

	if (_tcsicmp(lpSrcSection, lpDestSection) == 0 && _tcsicmp(lpSrcKey, lpDestKey) == 0)
		return FALSE;

	if (!IsKeyExist(lpSrcSection, lpSrcKey))
		return FALSE;

	if (bFailIfExist && IsKeyExist(lpDestSection, lpDestKey))
		return FALSE;
	
	LPTSTR psTemp = GetStringDynamic(lpSrcSection, lpSrcKey);
	const BOOL bRetCode = WriteString(lpDestSection, lpDestKey, psTemp);
	delete [] psTemp;
	return bRetCode;
}

//////////////////////////////////////////////////////////////////////////

BOOL CIniFile::MoveSection(
	LPCTSTR lpSrcSection, 
	LPCTSTR lpDestSection, 
	BOOL bFailIfExist) const
{
	return CopySection(lpSrcSection, lpDestSection, bFailIfExist) && DeleteSection(lpSrcSection);
}

//////////////////////////////////////////////////////////////////////////

BOOL CIniFile::MoveKey(
	LPCTSTR lpSrcSection, 
	LPCTSTR lpSrcKey, 
	LPCTSTR lpDestSection, 
	LPCTSTR lpDestKey, 
	BOOL bFailIfExist) const
{
	return CopyKey(lpSrcSection, lpSrcKey, lpDestSection, lpDestKey, bFailIfExist) && DeleteKey(lpSrcSection, lpSrcKey);
}

//////////////////////////////////////////////////////////////////////////

LPTSTR CIniFile::GetStringDynamic(
	LPCTSTR lpSection, 
	LPCTSTR lpKey, 
	LPCTSTR lpDefault) const
{
	TCHAR *psTemp = NULL;

	if (lpSection == NULL || lpKey == NULL)
	{
		// Invalid section or key name, just return the default string
		if (lpDefault == NULL)
		{
			// Empty string
			psTemp = new TCHAR[1];
			*psTemp = _T('\0');
		}else
		{
			psTemp = new TCHAR[_tcslen(lpDefault) + 1];
			_tcscpy(psTemp, lpDefault);
		}
		
		return psTemp;
	}
	
	// Keep enlarging the buffer size until being certain on that the string we
	// retrieved was original(not truncated).
	DWORD dwLen = DEF_PROFILE_THRESHOLD;
	psTemp = new TCHAR[dwLen + 1];
	DWORD dwCopied = ::GetPrivateProfileString(lpSection, lpKey, lpDefault == NULL ? _T("") : lpDefault, psTemp, dwLen, mpsPathName);
	while (dwCopied + 1 >= dwLen)
	{		
		dwLen += DEF_PROFILE_THRESHOLD;
		delete [] psTemp;
		psTemp = new TCHAR[dwLen + 1];
		dwCopied = ::GetPrivateProfileString(lpSection, lpKey, lpDefault == NULL ? _T("") : lpDefault, psTemp, dwLen, mpsPathName);
	}
	
	return psTemp; // !!! Requires the caller to free this memory !!!
}

//////////////////////////////////////////////////////////////////////////
// Split a string usinf a particular delimiter, split result are copied into lpBuffer
// in the "double null terminated string" format as the following figure shows:
// xxx\0xxxx\0xx\0xxx\0\0
//
// For example, if the delimiter is ",", then string "ab,cd,e" will be
// splitted into "ab\0cd\0e\0\0", this string format can be parsed into an array
// of sub strings easily using user defined functions or CIni::ParseStringArray.
DWORD CIniFile::StringSplit(
	LPCTSTR lpString, 
	LPTSTR lpBuffer, 
	DWORD dwBufSize, 
	LPCTSTR lpDelimiter, 
	BOOL bTrimString)
{
	if (lpString == NULL || lpBuffer == NULL || dwBufSize == 0)
		return 0;	

	DWORD dwCopied = 0;
	*lpBuffer = _T('\0');
	if (*lpString == _T('\0'))
		return 0;

	// If lpDelimiter is NULL, use the default delimiter ",", if delimiter length
	// is 0, then return whole string
	if (lpDelimiter != NULL && *lpDelimiter == _T('\0'))
	{
		_tcsncpy(lpBuffer, lpString, dwBufSize - 1);
		return _tcslen(lpBuffer);
	}

	LPTSTR pszDel = (lpDelimiter == NULL) ? _tcsdup(DEF_PROFILE_DELIMITER) : _tcsdup(lpDelimiter);
	const DWORD dwDeleteLen = _tcslen(pszDel);
	LPTSTR lpTarget = lpBuffer;

	// Search through lpString for delimiter matches, and extract sub strings out
	LPCTSTR lpPos = lpString;
	LPCTSTR lpEnd = _tcsstr(lpPos, pszDel);

	while (lpEnd != NULL)
	{
		LPTSTR pszSeg = StrDupEx(lpPos, lpEnd);
		if (bTrimString)
			TrimString(pszSeg);

		const DWORD dwSegLen = _tcslen(pszSeg);
		const DWORD dwCopyLen = min(dwSegLen, dwBufSize - dwCopied);

		// Need to avoid buffer overflow
		if (dwCopyLen > 0)
		{
			dwCopied += dwCopyLen + 1;
			_tcsncpy(lpTarget, pszSeg, dwCopyLen);
			lpTarget[dwCopyLen] = _T('\0');
			lpTarget = &lpTarget[dwSegLen + 1];
		}

		delete [] pszSeg;
		lpPos = &lpEnd[dwDeleteLen]; // Advance the pointer for next search		
		lpEnd = _tcsstr(lpPos, pszDel);
	}

	// The last part of string, there may not be the trailing delimiter, so we
	// need to take care of this part, too
	LPTSTR pszSeg = _tcsdup(lpPos);
	if (bTrimString)
		TrimString(pszSeg);

	const DWORD dwSegLen = _tcslen(pszSeg);
	const DWORD dwCopyLen = min(dwSegLen, dwBufSize - dwCopied);

	if (dwCopyLen > 0)
	{
		dwCopied += dwCopyLen + 1;
		_tcsncpy(lpTarget, pszSeg, dwCopyLen);
		lpTarget[dwCopyLen] = _T('\0');
	}

	delete [] pszSeg;
	lpBuffer[dwCopied] = _T('\0');
	delete [] pszDel;
	return dwCopied;
}

//////////////////////////////////////////////////////////////////////////
// Parse a "double null terminated string", pass each sub string to a user-defined
// callback function
BOOL CIniFile::ParseDNTString(
	LPCTSTR lpString, 
	PFN_IniParseCallback lpFnStrProc, 
	LPVOID lpParam)
{
	if (lpString == NULL || lpFnStrProc == NULL)
		return FALSE;

	LPCTSTR psTemp = lpString;
	DWORD dwLen = _tcslen(psTemp);

	while (dwLen > 0)
	{
		if (!lpFnStrProc(psTemp, lpParam))
			return FALSE;

		psTemp = &psTemp[dwLen + 1];
		dwLen = _tcslen(psTemp);
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL CALLBACK CIniFile::SubStrCompare(
	LPCTSTR lpString1, 
	LPVOID lpParam)
{
	assert(lpString1 != NULL);
	LPCTSTR lpString2 = (LPCTSTR)lpParam;
	assert(lpString2 != NULL);

	return _tcsicmp(lpString1, lpString2) != 0;
}

//////////////////////////////////////////////////////////////////////////

BOOL CALLBACK CIniFile:: KeyPairProc(
	LPCTSTR lpString, 
	LPVOID lpParam)
{
	PAIR_DATA *pPairData = (PAIR_DATA*)lpParam;
	if (lpString == NULL || pPairData== NULL)
		return FALSE;
	
	LPCTSTR psTemp = _tcschr(lpString, _T('='));
	if (psTemp == NULL || psTemp == lpString)
		return TRUE;
	
	// extract the sub-string on left side of the '='
	LPTSTR psz = new TCHAR[_tcslen(lpString) + 1];
	int nLoop;

	for (nLoop = 0; &lpString[nLoop] < psTemp; nLoop++)
		psz[nLoop] = lpString[nLoop];

	psz[nLoop] = _T('\0');

	TrimString(psz);
	DWORD dwNameLen = _tcslen(psz);
	DWORD dwCopyLen = 0;
	
	if (pPairData->lpTarget != NULL)
	{
		dwCopyLen = (pPairData->dwRemain > 1) ? min(dwNameLen, pPairData->dwRemain - 1) : 0;
		_tcsncpy(pPairData->lpTarget, psz, dwCopyLen);
		pPairData->lpTarget[dwCopyLen] = _T('\0');
		pPairData->lpTarget = &(pPairData->lpTarget[dwCopyLen + 1]); 
		pPairData->dwRemain -= dwCopyLen + 1;
	}else
	{
		dwCopyLen = dwNameLen;
	}

	delete [] psz;
	pPairData->dwTotalCopied += dwCopyLen + 1;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL CALLBACK CIniFile::SubStrAdd(
	LPCTSTR lpString, 
	LPVOID lpParam)
{
	CAtlArray<CString> *pArray = (CAtlArray<CString> *)lpParam;
	if (pArray == NULL || lpString == NULL)
		return FALSE;

	pArray->Add(lpString);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL CALLBACK CIniFile::SubStrCount(
	LPCTSTR lpString, 
	LPVOID lpParam)
{
	DWORD *pArray = (DWORD*)lpParam;
	if (pArray == NULL || lpString == NULL)
		return FALSE;

	(*pArray) ++;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

void CIniFile::ToBinaryString(
	UINT nNumber, 
	LPTSTR lpBuffer, 
	DWORD dwBufSize)
{
	if (dwBufSize == 0)
		return;
	
	DWORD dwIndex = 0;	

	do
	{
		lpBuffer[dwIndex++] = (nNumber % 2) ? _T('1') : _T('0');
		nNumber /= 2;
	}while (nNumber > 0 && dwIndex < dwBufSize);

	lpBuffer[dwIndex] = _T('\0');
	_tcsrev(lpBuffer);
}

//////////////////////////////////////////////////////////////////////////

int CIniFile::ValidateBase(
	int nBase)
{
	switch (nBase)
	{
	case BASE_BINARY:
	case BASE_OCTAL:
	case BASE_HEXADECIMAL:
		break;

	default:
		nBase = BASE_DECIMAL;
	}

	return nBase;
}

//////////////////////////////////////////////////////////////////////////

void CIniFile::IntToString(
	int nNumber, 
	LPTSTR lpBuffer, 
	int nBase)
{
	switch (nBase)
	{
	case BASE_BINARY:
	case BASE_OCTAL:
	case BASE_HEXADECIMAL:
		UIntToString((UINT)nNumber, lpBuffer, nBase);
		break;

	default:
		_stprintf(lpBuffer, _T("%d"), nNumber);
		break;
	}	
}

//////////////////////////////////////////////////////////////////////////

void CIniFile::UIntToString(
	UINT nNumber, 
	LPTSTR lpBuffer, 
	int nBase)
{
	switch (nBase)
	{
	case BASE_BINARY:
		ToBinaryString(nNumber, lpBuffer, DEF_PROFILE_NUM_LEN);
		break;

	case BASE_OCTAL:
		_stprintf(lpBuffer, _T("%o"), nNumber);
		break;

	case BASE_HEXADECIMAL:
		_stprintf(lpBuffer, _T("%X"), nNumber);
		break;

	default:
		_stprintf(lpBuffer, _T("%u"), nNumber);
		break;
	}	
}

//////////////////////////////////////////////////////////////////////////

BOOL CIniFile::StringToBool(
	LPCTSTR lpString, 
	BOOL bDefault)
{
	if (lpString == NULL || *lpString == _T('\0'))
		return bDefault;

	return (_tcsicmp(lpString, _T("true")) == 0 || 
			_tcsicmp(lpString, _T("yes")) == 0 || 
			_tcstol(lpString, NULL, BASE_DECIMAL) != 0);
}

//////////////////////////////////////////////////////////////////////////

BOOL CIniFile::TrimString(
	LPTSTR lpString)
{
	if (lpString == NULL)
		return FALSE;

	BOOL bTrimmed = FALSE;
	int nLen = _tcslen(lpString);

	// '\n' and '\r' are actually not possible in this case, but anyway...
	
	// Trim right side
	while (nLen >= 0 && 
		  (lpString[nLen - 1] == _T(' ') || 
		   lpString[nLen - 1] == _T('\t') || 
		   lpString[nLen - 1] == _T('\r') || 
		   lpString[nLen - 1] == _T('\n')))
	{
		lpString[--nLen] = _T('\0');
		bTrimmed = TRUE;		
	}

	LPCTSTR p = lpString; 
	while (*p == _T(' ') || 
		   *p == _T('\t') || 
		   *p == _T('\r') || 
		   *p == _T('\n'))
	{
		p = &p[1];
		bTrimmed = TRUE;
	}

	if (p != lpString)
	{
		LPTSTR psz = _tcsdup(p);
		_tcscpy(lpString, psz);
		delete [] psz;
	}

	return bTrimmed;
}

//////////////////////////////////////////////////////////////////////////

LPTSTR CIniFile::StrDupEx(
	LPCTSTR lpStart, 
	LPCTSTR lpEnd)
{
	const DWORD dwLen = ((DWORD)lpEnd - (DWORD)lpStart) / sizeof(TCHAR);
	LPTSTR psz = new TCHAR[dwLen + 1];
	_tcsncpy(psz, lpStart, dwLen);
	psz[dwLen] = _T('\0');
	return psz;
}

//////////////////////////////////////////////////////////////////////////
