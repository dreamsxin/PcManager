#include "stdafx.h"
// Disable compile warning:'StrCpy': name was marked as #pragma deprecated
#pragma warning( disable : 4995 ) 
#include<shlwapi.h>

#include "KPath.h"
#include "KModule.h"
#include "KStringConvert.h"

using namespace Kingsoft::Test::CCF::IO;
using namespace Kingsoft::Test::CCF::System;
using namespace Kingsoft::Test::CCF::Text;

wstring KPath::GetCurrentModuleFileDir()
{
	wstring currentModuleFileDir;	
    WCHAR buffer[MAX_PATH] = {0};

	GetModuleFileName(NULL, buffer, MAX_PATH);
	PathRemoveFileSpec(buffer);
	
    currentModuleFileDir.assign(buffer);

	return currentModuleFileDir;
}

bool KPath::Exists(const wstring& path)
{
    return FALSE != PathFileExistsW(path.c_str());
}

std::wstring KPath::Combine( const wstring& pathDir, const wstring& pathFile )
{
    WCHAR szPathCombined[MAX_PATH] = {0};
    WCHAR* pszPathCombined = NULL;

    pszPathCombined = PathCombineW(szPathCombined, pathDir.c_str(), pathFile.c_str());

    if (NULL == pszPathCombined)
    {
        return L"";
    }

    return szPathCombined;
}

std::wstring KPath::RemoveLastBackslash( const wstring& path )
{
    if (path.empty())
    {
        return L"";
    }

    wstring tempPath = path;

    if ('\\' == tempPath[tempPath.length() - 1])
    {
        tempPath = tempPath.substr(0, tempPath.length() - 1);
    }

    return tempPath;
}

wstring KPath::GetDirectoryName( const wstring& path )
{
    if (path.empty())
    {
        return wstring(L"");
    }

	WCHAR buffer[MAX_PATH] = {0};
	wcscpy_s(buffer, MAX_PATH, path.c_str());
	::PathRemoveFileSpec(buffer);

	// Fix a bug, when path like "c:\",
	// We must return "" not "c:\"
	if (_wcsicmp(path.c_str(), buffer) == 0)
	{
		return L"";
	}

	return buffer;
}

wstring KPath::GetFullPath( const wstring& path )
{
    if (path.empty())
    {
        return false;
    }

	if (!::PathIsRelative(path.c_str()))
	{
		return path;
	}
	// 原本应该使用KDirectory::GetCurrentDirectory()的，但发现在VS里调试时，路径
	// 获取到的路径是代码所在的路径，使用起来非常不方便，因此改成
	// KModule::GetCurrentModuleDirectory() 看以后是否有必要再修改。
	wstring currentDir = KModule::GetCurrentModuleDirectory();
	wstring fullPath = KPath::Combine(currentDir, path);

	return fullPath;
}

wstring KPath::GetFileName( const wstring& path )
{
	if (path.empty())
	{
        return wstring(L"");
	}

    wchar_t drive[_MAX_DRIVE];
	wchar_t dir[_MAX_DIR];
	wchar_t fname[_MAX_FNAME];
	wchar_t ext[_MAX_EXT];
	errno_t err;

	err = ::_wsplitpath_s(path.c_str(), drive, _MAX_DRIVE, dir, _MAX_DIR, 
		fname, _MAX_FNAME, ext, _MAX_EXT);
	
	if (err != 0 || wcslen(fname) == 0)
	{
		return L"";
	}

	wstring fileName = KStringConvert::Format(L"%s%s", fname, ext);

	return fileName;
}

wstring KPath::GetExtension( const wstring& path )
{
	if (path.empty())
	{
		return wstring(L"");
	}

	wchar_t drive[_MAX_DRIVE];
	wchar_t dir[_MAX_DIR];
	wchar_t fname[_MAX_FNAME];
	wchar_t ext[_MAX_EXT];
	errno_t err;

	err = ::_wsplitpath_s(path.c_str(), drive, _MAX_DRIVE, dir, _MAX_DIR, 
		fname, _MAX_FNAME, ext, _MAX_EXT);

	if (err != 0 || wcslen(ext) == 0)
	{
		return L"";
	}

	return ext;
}

std::wstring KPath::GetFileNameWithoutExtension( const wstring& path )
{
    if (path.empty())
    {
        return wstring(L"");
    }

    wchar_t drive[_MAX_DRIVE];
    wchar_t dir[_MAX_DIR];
    wchar_t fname[_MAX_FNAME];
    wchar_t ext[_MAX_EXT];
    errno_t err;

    err = ::_wsplitpath_s(path.c_str(), drive, _MAX_DRIVE, dir, _MAX_DIR, 
        fname, _MAX_FNAME, ext, _MAX_EXT);

    if (err != 0 || wcslen(fname) == 0)
    {
        return L"";
    }

    return fname;
}