/**
* @file    winfilefinddata.h
* @brief   ...
* @author  bbcallen
* @date    2009-03-06  22:32
*/

#ifndef WINFILEFINDDATA_H
#define WINFILEFINDDATA_H

#include <atlstr.h>
#include "winmodbase.h"

NS_WINMOD_BEGIN

class CWinFileFindData
{
public:
    CWinFileFindData();
    ~CWinFileFindData();

// Attributes
public:
    WIN32_FIND_DATAW* GetFindDataBuffer();


    DWORD           GetFileAttributes() const;
    const FILETIME& GetCreationTime() const;
    const FILETIME& GetLastAccessTime() const;
    const FILETIME& GetLastWriteTime() const;
    DWORD           GetFileSizeHigh() const;
    DWORD           GetFileSizeLow() const;
    LPCWSTR         GetFileName() const;
    CString         GetFileNameBase() const;
    CString         GetFileNameExt() const;
    LPCWSTR         GetAlternateFileName() const;

    DWORD           GetReparsePointTags() const;




    ULONGLONG GetFileSize() const;

    BOOL MatchesMask(DWORD dwMask) const;

    BOOL IsDots() const;


    BOOL IsReadOnly() const;
    BOOL IsDirectory() const;
    BOOL IsCompressed() const;
    BOOL IsSystem() const;
    BOOL IsHidden() const;
    BOOL IsTemporary() const;
    BOOL IsNormal() const;
    BOOL IsArchived() const;
    BOOL IsEncrypted() const;
    BOOL IsNotContentIndexed() const;
    BOOL IsOffline() const;
    BOOL IsReparsePoint() const;
    BOOL IsSparseFile() const;
    BOOL IsVirtual() const;


protected:
    void ResetFindData();

    WIN32_FIND_DATA m_findData;
};




inline DWORD CWinFileFindData::GetFileAttributes() const
{
    return m_findData.dwFileAttributes;
}

inline const FILETIME& CWinFileFindData::GetCreationTime() const
{
    return m_findData.ftCreationTime;
}

inline const FILETIME& CWinFileFindData::GetLastAccessTime() const
{
    return m_findData.ftLastAccessTime;
}

inline const FILETIME& CWinFileFindData::GetLastWriteTime() const
{
    return m_findData.ftLastWriteTime;
}

inline DWORD CWinFileFindData::GetFileSizeHigh() const
{
    return m_findData.nFileSizeHigh;
}

inline DWORD CWinFileFindData::GetFileSizeLow() const
{
    return m_findData.nFileSizeLow;
}

inline LPCWSTR CWinFileFindData::GetFileName() const
{
    return m_findData.cFileName;
}

inline LPCWSTR CWinFileFindData::GetAlternateFileName() const
{
    return m_findData.cAlternateFileName;
}




inline BOOL CWinFileFindData::IsReadOnly() const
{ 
    return MatchesMask(FILE_ATTRIBUTE_READONLY);
}

inline BOOL CWinFileFindData::IsDirectory() const
{
    return MatchesMask(FILE_ATTRIBUTE_DIRECTORY);
}

inline BOOL CWinFileFindData::IsCompressed() const
{
    return MatchesMask(FILE_ATTRIBUTE_COMPRESSED);
}

inline BOOL CWinFileFindData::IsSystem() const
{
    return MatchesMask(FILE_ATTRIBUTE_SYSTEM);
}

inline BOOL CWinFileFindData::IsHidden() const
{
    return MatchesMask(FILE_ATTRIBUTE_HIDDEN);
}

inline BOOL CWinFileFindData::IsTemporary() const
{
    return MatchesMask(FILE_ATTRIBUTE_TEMPORARY);
}

inline BOOL CWinFileFindData::IsNormal() const
{
    return MatchesMask(FILE_ATTRIBUTE_NORMAL);
}

inline BOOL CWinFileFindData::IsArchived() const
{
    return MatchesMask(FILE_ATTRIBUTE_ARCHIVE);
}

inline BOOL CWinFileFindData::IsEncrypted() const
{
    return MatchesMask(FILE_ATTRIBUTE_ENCRYPTED);
}

inline BOOL CWinFileFindData::IsNotContentIndexed() const
{
    return MatchesMask(FILE_ATTRIBUTE_NOT_CONTENT_INDEXED);
}

inline BOOL CWinFileFindData::IsOffline() const
{
    return MatchesMask(FILE_ATTRIBUTE_OFFLINE);
}

inline BOOL CWinFileFindData::IsReparsePoint() const
{
    return MatchesMask(FILE_ATTRIBUTE_REPARSE_POINT);
}

inline BOOL CWinFileFindData::IsSparseFile() const
{
    return MatchesMask(FILE_ATTRIBUTE_SPARSE_FILE);
}

#ifndef FILE_ATTRIBUTE_VIRTUAL
#define FILE_ATTRIBUTE_VIRTUAL              0x00010000
#endif
inline BOOL CWinFileFindData::IsVirtual() const
{
    return MatchesMask(FILE_ATTRIBUTE_VIRTUAL);
}



class CWinFileFindApi
{
public:
    // 该调用会自动忽略目录"."和目录".."
    static HANDLE   FindFirstFileSkipDots(LPCWSTR lpFileName, CWinFileFindData& findData);

    static BOOL     FindNextFileSkipDots(HANDLE hContext, CWinFileFindData& findData);

    static BOOL     FindClose(HANDLE hContext);
};



NS_WINMOD_END

#endif//WINFILEFINDDATA_H