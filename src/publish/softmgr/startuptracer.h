#pragma once
#include <time.h>

class StartupTracer
{
public:
	StartupTracer()
	{
		CAppPath::Instance().GetLeidianLogPath(_db_name, TRUE);
		_db_name+=L"\\startup.log";
	}
	~StartupTracer()
	{
	}

	BOOL log( LPCTSTR lpszPath )
	{
		CString path = lpszPath;
		if(path.GetLength()<4)
			return FALSE;
		//PathRemoveFileSpec(path.GetBuffer());
		CString tm;
		__time64_t long_time;
		_time64( &long_time );
		tm.Format(L"%I64d",long_time);
		
		return WritePrivateProfileString(path,L"lastuse",tm,_db_name);
	}
	
private:
	CString _db_name;
};
