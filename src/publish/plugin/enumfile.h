#pragma once

#include <atlstr.h>

class CEnumFile
{
public:

	class FileInfo  
	{
	public:
		FileInfo(){}
		~FileInfo(){}

		CString m_strFileName;
		DWORD	m_nSize;

		operator CString&()
		{
			return m_strFileName;
		}

		CString& GetStr()
		{
			return m_strFileName;
		}
	};

	CEnumFile(LPCTSTR lpDirName, LPCTSTR lpPost=_T("*.*"))
	{
		CString		strSearch;

		strSearch=lpDirName;

		if (strSearch.Right(1)!=_T("\\"))
			strSearch.Append(_T("\\"));
		strSearch.Append(lpPost);
		m_strDir = lpDirName;

		WIN32_FIND_DATA	wfd;
		HANDLE			hFind = ::FindFirstFile(strSearch,&wfd);

		if (hFind!=INVALID_HANDLE_VALUE)
		{
			do 
			{
				FileInfo	fi;
				CString		strFileName = wfd.cFileName;

				fi.m_strFileName = strFileName;
				fi.m_nSize		 = wfd.nFileSizeLow;	

				if (wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
				{
					if (strFileName.CompareNoCase(_T("."))==0||
						strFileName.CompareNoCase(_T(".."))==0)
						continue;

					m_listDirs.Add(strFileName);
				}
				else
				{
					m_listFiles.Add(fi);
				}
				
			}while (::FindNextFile(hFind,&wfd));

            FindClose( hFind );
		} 
	}

	int GetFileCount()
	{
		return m_listFiles.GetSize();
	}

	int GetDirCount()
	{
		return m_listDirs.GetSize();
	}

	CString& GetFileName(int i)
	{
		return m_listFiles[i].GetStr();
	}

	DWORD GetFilesize(int i)
	{
		return m_listFiles[i].m_nSize;
	}

	CString& GetDirName(int i)
	{
		return m_listDirs[i];
	}

	CString	GetFileFullPath(int i)
	{
		CString	strFull = m_strDir;
		strFull.Append(_T("\\"));
		strFull.Append(m_listFiles[i].GetStr());
		return strFull;
	}

	CString GetDirFullPath(int i)
	{
		CString	strFull = m_strDir;
		strFull.Append(_T("\\"));
		strFull.Append(m_listDirs[i]);
		return strFull;
	}

protected:
	CSimpleArray<FileInfo>	m_listFiles;
	CSimpleArray<CString>	m_listDirs;
	CString					m_strDir;
};