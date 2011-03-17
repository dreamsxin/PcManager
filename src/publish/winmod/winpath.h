/**
* @file    winpath.h
* @brief   ...
* @author  bbcallen
* @date    2009-03-07  15:51
*/

#ifndef WINPATH_H
#define WINPATH_H

#include <shobjidl.h>
#include <atlstr.h>
#include <atlsecurity.h>
#include "winmodbase.h"

NS_WINMOD_BEGIN

/// stateless path methods, path can be greater than MAX_PATH
class CWinPathApi
{
public:

    /// constant
    enum EM_WIN_PATH_API_CONST
    {
        WIN_PATH_MAX_UNICODE_PATH           = 32767 + 4,    ///< max size of buffer to store path, in bytes
        WIN_PATH_UNICODE_PATH_PREFIX        = 4,            ///< length of "\\\\??\\"
        WIN_PATH_UNICODE_UNC_PATH_PREFIX    = 7,            ///< length of "\\\\??\\UNC\\"
    };

    /// see Win32 API GetFullPathName
    static HRESULT  ExpandFullPathName(CString& strFullPathName);

    /// see Win32 API GetLongPathName
    static HRESULT  ExpandLongPathName(CString& strFullPathName);


    /// replace 'lpPattern' at begging of 'strPath' with 'csidl'
    static BOOL     ExpandSpecialFolderPathAtBeginning(LPWSTR lpszPath, DWORD cchBuf, LPCWSTR lpszPattern, int csidl);
    static BOOL     ExpandSpecialFolderPathAtBeginning(CString& strPath, LPCWSTR lpszPattern, int csidl);

    /// replace 'lpPattern' at begging of 'strPath' with 'lpExpandAs'
    static BOOL     ExpandPatternAtBeginning(LPWSTR lpszPath, DWORD cchBuf, LPCWSTR lpszPattern, LPCWSTR lpszExpandAs);
    static BOOL     ExpandPatternAtBeginning(CString& strPath, LPCWSTR lpszPattern, LPCWSTR lpszExpandAs);

    /// see Win32 API ExpandEnvironmentStrings
    static BOOL     ExpandEnvironmentStrings(LPCWSTR lpszSrc, LPWSTR lpszDest, DWORD cchBuf);
    static BOOL     ExpandEnvironmentStrings(CString& strPath);


    /// replace special string in unaccessible path, such as '%SYSTEM%' '\??\' '\\SystemRoot'
    static BOOL     ExpandAsAccessiblePath(LPWSTR lpszPath, DWORD cchBuf);
    static BOOL     ExpandAsAccessiblePath(CString& strPath);


    /// check if path begin with "\\\\??\\" or "\\\\??\\UNC\\"
    static BOOL     HasUnicodePrefix(LPCWSTR pszPath);

    // for non-unicode unc path, it returns the position right after first "\\\\"
    // for unicode path, it returns the position right after "\\\\?\\" or "\\\\?\\UNC\\"
    // for other case, it returns the whole string
    static LPCWSTR  FindAfterAnyPrefix(LPCWSTR pszPath);

    // for non-unicode unc path, it returns the position right after first "\\\\"
    // for unicode path, it returns the position right after "\\\\?\\"
    // for other case, it returns the whole string
    static LPCWSTR  FindAfterUnicodePrefix(LPCWSTR pszPath);

    static LPCWSTR  FindFileName(LPCWSTR pszPath);

    static LPCWSTR  FindExtension(LPCWSTR pszPath);

    /// create short cut
    static HRESULT  CreateLnkFile(LPCWSTR pszPath, LPCWSTR pszArguments, LPCWSTR pszDesc, LPCWSTR pszLnkFilePath, LPCWSTR pszIcon = NULL);
    /// resolve short cut
    static HRESULT  ResolveLnkFileNoSafe(LPCWSTR pszLnkFile, CString& strTargetPath, DWORD dwFlag = SLR_NOUPDATE | SLR_NOTRACK | SLR_NOSEARCH | SLR_NO_UI);
    /// resolve short cut 
    static HRESULT  ResolveLnkFile(LPCWSTR pszLnkFile, CString& strTargetPath, DWORD dwFlag = SLR_NOUPDATE | SLR_NOTRACK | SLR_NOSEARCH | SLR_NO_UI);


    static HRESULT  SetFileAces(
        LPCWSTR lpszFilePath,
        CSid&   sid,
        DWORD   dwAllowedAccess,
        DWORD   dwDeniedAccess,
        BOOL    bRemoveOldAces = TRUE,
        DWORD   AceFlags = 0);



    /// path is "." or ".."
    static BOOL     IsDots(LPCWSTR pszPath);

    static BOOL     IsDirectory(LPCWSTR pszPath);
    static BOOL     IsRelative(LPCWSTR pszPath);
    static BOOL     IsRoot(LPCWSTR pszPath);
    static BOOL     IsUNC(LPCWSTR pszPath);
    static BOOL     IsUNCServer(LPCWSTR pszPath);
    static BOOL     IsUNCServerShare(LPCWSTR pszPath);
    static BOOL     IsFileExisting(LPCWSTR pszPath);

    static BOOL     IsFileNoDirExisting(LPCWSTR pszPath);
    static BOOL     IsLnkFile(LPCWSTR pszPath);

    static BOOL     IsDeviceAccessible(WCHAR cRoot);
};


/// path class
class CWinPath
{
// Constructors
public:
    CWinPath();
	CWinPath(const CWinPath& path);
	CWinPath(LPCWSTR pszPath);


// Operators
public:
    operator LPCWSTR() const;
    CWinPath& operator+=(LPCWSTR pszMore);

// Operations
public:
    void        AddBackslash();
    BOOL        AddExtension(LPCWSTR pszExtension);
    BOOL        Append(LPCWSTR pszMore);
    void        BuildRoot(int iDrive);
    //void        Canonicalize();
    void        Combine(LPCWSTR pszDir, LPCWSTR pszFile);
    //CWinPath    CommonPrefix(LPCWSTR pszOther );
    //BOOL        CompactPath(HDC hDC, UINT nWidth);
    BOOL        CompactPathEx(UINT nMaxChars, DWORD dwFlags = 0);
    //BOOL        FileExists() const;
    int         FindExtension() const;
    int         FindFileName() const;
    //int         GetDriveNumber() const;
    CString     GetExtension() const;
    BOOL        IsDirectory() const;
    //BOOL        IsFileSpec() const;
    //BOOL        IsPrefix(LPCWSTR pszPrefix ) const;
    BOOL        IsRelative() const;
    BOOL        IsRoot() const;
    //BOOL        IsSameRoot(LPCWSTR pszOther) const;
    BOOL        IsUNC() const;
    BOOL        IsUNCServer() const;
    BOOL        IsUNCServerShare() const;
    //BOOL        MakePretty();
    //BOOL        MatchSpec(LPCWSTR pszSpec) const;
    //void        QuoteSpaces();
    //BOOL        RelativePathTo(LPCWSTR pszFrom, DWORD dwAttrFrom, LPCWSTR pszTo, DWORD dwAttrTo );
    void        RemoveArgs();
    void        RemoveBackslash();
    //void        RemoveBlanks();
    void        RemoveExtension();
    BOOL        RemoveFileSpec();
    //BOOL        RenameExtension(LPCWSTR pszExtension);
    //int         SkipRoot() const;
    void        StripPath();
    BOOL        StripToRoot();
    void        UnquoteSpaces();


// Extra Operation
public:

    BOOL        IsExisting() const;
    void        RemoveSingleArg();

    BOOL        HasUnicodePrefix() const;
    void        RemoveUnicodePrefix();
    void        AddUnicodePrefix();
    CWinPath    GetPathWithoutUnicodePrefix() const;

    HRESULT     ExpandFullPathName();
    HRESULT     ExpandLongPathName();
    void        ExpandEnvironmentStrings();
    void        ExpandNormalizedPathName();
    BOOL        ExpandAsAccessiblePath();

    DWORD       GetModuleFileName(HMODULE hModule, DWORD dwMaxSize = MAX_PATH);

    CString m_strPath;
};




inline CWinPath::CWinPath() throw()
{
}

inline CWinPath::CWinPath(const CWinPath& path):
    m_strPath(path.m_strPath)
{
}

inline CWinPath::CWinPath(LPCWSTR pszPath):
    m_strPath(pszPath )
{
}

inline CWinPath::operator LPCWSTR() const throw()
{
    return m_strPath;
}

inline CWinPath& CWinPath::operator+=(LPCWSTR pszMore)
{
    Append( pszMore );

    return *this;
}

class CWinPathAttr // dummyz@126.com
{
public:
	CWinPathAttr()
	{
	}

	CWinPathAttr(LPCTSTR lpFilePath)
	{
		m_strFilePath = lpFilePath;
		_Init();
	}

	CWinPathAttr(const CString& strFilePath)
	{
		m_strFilePath = strFilePath;
	}
	
	BOOL	IsChanged()
	{
		WIN32_FILE_ATTRIBUTE_DATA wfad;

		if ( GetFileAttributesEx(m_strFilePath, GetFileExInfoStandard, &wfad) )
		{
			if ( CompareFileTime(&wfad.ftLastWriteTime, &m_wfad.ftLastWriteTime) != 0 )
			{
				memcpy(&m_wfad, &wfad, sizeof (wfad));
				return TRUE;
			}
		}

		return FALSE;
	}

protected:
	void	_Init()
	{
		if ( !GetFileAttributesEx(m_strFilePath, GetFileExInfoStandard, &m_wfad) )
		{
			memset(&m_wfad, 0, sizeof (m_wfad));
		}
	}

protected:
	WIN32_FILE_ATTRIBUTE_DATA m_wfad;
	CString		m_strFilePath;
};


NS_WINMOD_END

#endif//WINPATH_H