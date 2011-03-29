#pragma once
#include <time.h>
#include <atltime.h>

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

	inline void FileTimeToUnixTime(LPFILETIME pft, __time32_t* pt)
	{
		LONGLONG ll; // 64 bit value
		ll = (((LONGLONG)(pft->dwHighDateTime)) << 32) + pft->dwLowDateTime;
		*pt = (__time32_t)((ll - 116444736000000000ui64)/10000000ui64);
	}

	int GetDayOver( CTime& timeCur, CTime& timeLast )
	{
		if( timeLast.GetTime() > timeCur.GetTime() )
			return 0;

		return ( timeCur.GetTime() - timeLast.GetTime() ) / 3600 * 24;
	}

	BOOL log( LPCTSTR lpszPath )
	{
		CString path = lpszPath;
		if(path.GetLength()<4)
			return FALSE;

		TCHAR tszUseTime[20] = {0};
		GetPrivateProfileString( path, L"lastuse", 
								TEXT("0"), tszUseTime, 
								20, _db_name );

		CTime timeCur = CTime::GetCurrentTime();

		//没有监控的软件需要在界面加入相应逻辑
		//该文件没有记录过
		if( _tcsicmp( tszUseTime, TEXT("0")) == 0 )
		{
			CString tm;

			CPath pathFile( lpszPath );
			pathFile.RemoveFileSpec();
			int nCnt = 1;

			WIN32_FILE_ATTRIBUTE_DATA wfad;
			if(::GetFileAttributesExW( pathFile.m_strPath, GetFileExInfoStandard, &wfad))
			{
				__time32_t time;
				FileTimeToUnixTime(&wfad.ftLastWriteTime, &time);

				CTime tLastUse( time );
				int nDayCnt = GetDayOver( timeCur, tLastUse );
				if( nDayCnt > 15 )
				{
					nCnt = 1;
				}
				else if( nDayCnt >= 4 )
				{
					nCnt = 10;
				}
				else
				{
					nCnt = 22;
				}
			}
			
			tm.Format(L"%I64d",timeCur.GetTime() );
			CString strDay;
			strDay.Format( TEXT("%d"), nCnt );
			WritePrivateProfileString( path, L"lastuse", tm, _db_name );
			WritePrivateProfileString( path, L"count", strDay, _db_name );
		}
		else
		{
			int nCnt = GetPrivateProfileInt( path, L"count", -1, _db_name );
			CTime tLastUse( _wtoi64_l( tszUseTime, 0) );
			
			//今天还没有记录次数,如果有不记录
			if( timeCur.GetDay() != tLastUse.GetDay() )
			{
				//有lastuse 没有count
				if( nCnt == -1 )
				{
					int nDayCnt = GetDayOver( timeCur, tLastUse );
					if( nDayCnt > 15 )
					{
						nCnt = 1;
					}
					else if( nDayCnt >= 4 )
					{
						nCnt = 10;
					}
					else
					{
						nCnt = 22;
					}
				}
				else
				{
					//过一个月次数减半
					if( timeCur.GetMonth() != tLastUse.GetMonth() )
						nCnt = nCnt / 2;

					nCnt ++;
				}
				
				CString tm, strCnt;				
				strCnt.Format( L"%d", nCnt );
				tm.Format(L"%I64d",timeCur.GetTime() );
				WritePrivateProfileString( path, L"lastuse", tm, _db_name );
				WritePrivateProfileString( path, L"count", strCnt, _db_name );
			}
		}
		
		return TRUE;
	}
	
private:
	CString _db_name;
};
