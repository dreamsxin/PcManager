//////////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////////

#define BASE_BINARY			2
#define BASE_OCTAL			8
#define BASE_DECIMAL		10
#define BASE_HEXADECIMAL	16

//////////////////////////////////////////////////////////////////////////

typedef BOOL (CALLBACK *PFN_IniParseCallback)(
	LPCTSTR, 
	LPVOID);

//////////////////////////////////////////////////////////////////////////

class CIniFile
{
public:		
	CIniFile();

	CIniFile(
		LPCTSTR lpPathName);

	virtual ~CIniFile();

	void SetPathName(
		LPCTSTR lpPathName);

	DWORD GetPathName(
		LPTSTR lpBuffer, 
		DWORD dwBufSize) const;

	CString GetPathName() const;
	
	DWORD GetString(
		LPCTSTR lpSection, 
		LPCTSTR lpKey, 
		LPTSTR lpBuffer, 
		DWORD dwBufSize, 
		LPCTSTR lpDefault = NULL) const;

	CString GetString(
		LPCTSTR lpSection, 
		LPCTSTR lpKey, 
		LPCTSTR lpDefault = NULL) const;

	BOOL WriteString(
		LPCTSTR lpSection, 
		LPCTSTR lpKey, 
		LPCTSTR lpValue) const;

	BOOL AppendString(
		LPCTSTR Section, 
		LPCTSTR lpKey, 
		LPCTSTR lpString) const;
	
	DWORD GetArray(
		LPCTSTR lpSection, 
		LPCTSTR lpKey, 
		LPTSTR lpBuffer, 
		DWORD dwBufSize, 
		LPCTSTR lpDelimiter = NULL, 
		BOOL bTrimString = TRUE) const;

	void GetArray(
		LPCTSTR lpSection, 
		LPCTSTR lpKey, 
		CAtlArray<CString> *pArray, 
		LPCTSTR lpDelimiter = NULL, 
		BOOL bTrimString = TRUE) const;

	BOOL WriteArray(
		LPCTSTR lpSection, 
		LPCTSTR lpKey, 
		const CAtlArray<CString> *pArray, 
		int nWriteCount = -1, 
		LPCTSTR lpDelimiter = NULL) const;
	
	int GetInt(
		LPCTSTR lpSection, 
		LPCTSTR lpKey, 
		int nDefault, 
		int nBase = BASE_DECIMAL) const;

	BOOL WriteInt(
		LPCTSTR lpSection, 
		LPCTSTR lpKey, 
		int nValue, 
		int nBase = BASE_DECIMAL) const;

	BOOL IncreaseInt(
		LPCTSTR lpSection, 
		LPCTSTR lpKey, 
		int nIncrease = 1, 
		int nBase = BASE_DECIMAL) const;
	
	UINT GetUInt(
		LPCTSTR lpSection, 
		LPCTSTR lpKey, 
		UINT nDefault, 
		int nBase = BASE_DECIMAL) const;

	BOOL WriteUInt(
		LPCTSTR lpSection, 
		LPCTSTR lpKey, 
		UINT nValue, 
		int nBase = BASE_DECIMAL) const;

	BOOL IncreaseUInt(
		LPCTSTR lpSection, 
		LPCTSTR lpKey, 
		UINT nIncrease = 1, 
		int nBase = BASE_DECIMAL) const;
	
	BOOL GetBool(
		LPCTSTR lpSection, 
		LPCTSTR lpKey, 
		BOOL bDefault) const;

	BOOL WriteBool(
		LPCTSTR lpSection, 
		LPCTSTR lpKey, 
		BOOL bValue) const;

	BOOL InvertBool(
		LPCTSTR lpSection, 
		LPCTSTR lpKey) const;
	
	double GetDouble(
		LPCTSTR lpSection, 
		LPCTSTR lpKey, 
		double fDefault) const;

	BOOL WriteDouble(
		LPCTSTR lpSection, 
		LPCTSTR lpKey, 
		double fValue, 
		int nPrecision = -1) const;

	BOOL IncreaseDouble(
		LPCTSTR lpSection, 
		LPCTSTR lpKey, 
		double fIncrease, 
		int nPrecision = -1) const;

	TCHAR GetChar(
		LPCTSTR lpSection, 
		LPCTSTR lpKey, 
		TCHAR cDefault) const;

	BOOL WriteChar(
		LPCTSTR lpSection, 
		LPCTSTR lpKey, 
		TCHAR c) const;

	POINT GetPoint(
		LPCTSTR lpSection, 
		LPCTSTR lpKey, 
		POINT ptDefault) const;

	BOOL WritePoint(
		LPCTSTR lpSection, 
		LPCTSTR lpKey, 
		POINT pt) const;
	
	RECT GetRect(
		LPCTSTR lpSection, 
		LPCTSTR lpKey, 
		RECT rcDefault) const;

	BOOL WriteRect(
		LPCTSTR lpSection, 
		LPCTSTR lpKey, 
		RECT rc) const;

	DWORD GetDataBlock(
		LPCTSTR lpSection, 
		LPCTSTR lpKey, 
		LPVOID lpBuffer, 
		DWORD dwBufSize, 
		DWORD dwOffset = 0) const;

	BOOL WriteDataBlock(
		LPCTSTR lpSection, 
		LPCTSTR lpKey, 
		LPCVOID lpData, 
		DWORD dwDataSize) const;

	BOOL AppendDataBlock(
		LPCTSTR lpSection, 
		LPCTSTR lpKey, 
		LPCVOID lpData, 
		DWORD dwDataSize) const;
	
	BOOL IsSectionExist(
		LPCTSTR lpSection) const;

	DWORD GetSectionNames(
		LPTSTR lpBuffer, 
		DWORD dwBufSize) const;

	void GetSectionNames(
		CAtlArray<CString> *pArray) const;

	BOOL CopySection(
		LPCTSTR lpSrcSection, 
		LPCTSTR lpDestSection, 
		BOOL bFailIfExist) const;

	BOOL MoveSection(
		LPCTSTR lpSrcSection, 
		LPCTSTR lpDestSection, 
		BOOL bFailIfExist = TRUE) const;

	BOOL DeleteSection(
		LPCTSTR lpSection) const;
	
	BOOL IsKeyExist(
		LPCTSTR lpSection, 
		LPCTSTR lpKey) const;	

	DWORD GetKeyLines(
		LPCTSTR lpSection, 
		LPTSTR lpBuffer, 
		DWORD dwBufSize) const;

	void GetKeyLines(
		LPCTSTR lpSection, 
		CAtlArray<CString> *pArray) const;

	DWORD GetKeyNames(
		LPCTSTR lpSection, 
		LPTSTR lpBuffer, 
		DWORD dwBufSize) const;

	void GetKeyNames(
		LPCTSTR lpSection, 
		CAtlArray<CString> *pArray) const;

	DWORD GetKeyNameCount(
		LPCTSTR lpSection) const;

	BOOL CopyKey(
		LPCTSTR lpSrcSection, 
		LPCTSTR lpSrcKey, 
		LPCTSTR lpDestSection, 
		LPCTSTR lpDestKey, 
		BOOL bFailIfExist) const;

	BOOL MoveKey(
		LPCTSTR lpSrcSection, 
		LPCTSTR lpSrcKey, 
		LPCTSTR lpDestSection, 
		LPCTSTR lpDestKey, 
		BOOL bFailIfExist = TRUE) const;

	BOOL DeleteKey(
		LPCTSTR lpSection, 
		LPCTSTR lpKey) const;

	// Parse a "Double-Null Terminated String"
	static BOOL ParseDNTString(
		LPCTSTR lpString, 
		PFN_IniParseCallback lpFnStrProc, 
		LPVOID lpParam = NULL);

	// Check for Whether a String Representing TRUE or FALSE
	static BOOL StringToBool(
		LPCTSTR lpString, 
		BOOL bDefault = FALSE);
		
protected:	
	static LPTSTR StrDupEx(
		LPCTSTR lpStart, 
		LPCTSTR lpEnd);

	static BOOL TrimString(
		LPTSTR lpBuffer);

	LPTSTR GetStringDynamic(
		LPCTSTR lpSection, 
		LPCTSTR lpKey, 
		LPCTSTR lpDefault = NULL) const;

	static DWORD StringSplit(
		LPCTSTR lpString, 
		LPTSTR lpBuffer, 
		DWORD dwBufSize, 
		LPCTSTR lpDelimiter = NULL, 
		BOOL bTrimString = TRUE);

	static void ToBinaryString(
		UINT nNumber, 
		LPTSTR lpBuffer, 
		DWORD dwBufSize);

	static int ValidateBase(
		int nBase);

	static void IntToString(
		int nNumber, 
		LPTSTR lpBuffer, 
		int nBase);

	static void UIntToString(
		UINT nNumber, 
		LPTSTR lpBuffer, 
		int nBase);

	static BOOL CALLBACK SubStrCompare(
		LPCTSTR lpString1, 
		LPVOID lpParam);

	static BOOL CALLBACK KeyPairProc(
		LPCTSTR lpString, 
		LPVOID lpParam);	

	static BOOL CALLBACK SubStrAdd(
		LPCTSTR lpString, 
		LPVOID lpParam);

	static BOOL CALLBACK SubStrCount(
		LPCTSTR lpString, 
		LPVOID lpParam);

private:
	LPTSTR mpsPathName;
};

//////////////////////////////////////////////////////////////////////////