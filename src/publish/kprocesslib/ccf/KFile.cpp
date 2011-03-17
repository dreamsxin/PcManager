/**
* @file     KFile.cpp
* @brief    
* @details  
* @author   廖泉辉
* @date     2009.4.1
*/

#include "stdafx.h"
#include <wincrypt.h>
#include <stdio.h>
#pragma warning( disable : 4995 ) 
#include <Shlwapi.h>
#include "KFile.h"
#include "KPath.h"
#include "KStringConvert.h"

#pragma comment(lib, "Version.lib")
#pragma comment(lib, "crypt32.lib")

using namespace Kingsoft::Test::CCF::IO;
using namespace Kingsoft::Test::CCF::Text;
#define ENCODING (X509_ASN_ENCODING | PKCS_7_ASN_ENCODING)
KFile::KFile(LPCWSTR fileName)
{
    wcscpy_s(KFile::szFileName, MAX_PATH, fileName);
}

WCHAR* KFile::DigitalSignature() const
{
	HCERTSTORE hStore = NULL;
	HCRYPTMSG hMsg = NULL; 
	PCCERT_CONTEXT pCertContext = NULL; 
	DWORD dwEncoding, dwContentType, dwFormatType;
	PCMSG_SIGNER_INFO pSignerInfo = NULL;
	PCMSG_SIGNER_INFO pCounterSignerInfo = NULL;
	DWORD dwSignerInfo;
	CERT_INFO CertInfo;
	LPTSTR szName = NULL;
	DWORD dwData;

	__try
	{
		// Get message handle and store handle from the signed file.
		if (!CryptQueryObject(CERT_QUERY_OBJECT_FILE,
			szFileName,
			CERT_QUERY_CONTENT_FLAG_PKCS7_SIGNED_EMBED,
			CERT_QUERY_FORMAT_FLAG_BINARY,
			0,
			&dwEncoding,
			&dwContentType,
			&dwFormatType,
			&hStore,
			&hMsg,
			NULL))
		{
			__leave;
		}

		// Get signer information size.
		if (!CryptMsgGetParam(hMsg, 
			CMSG_SIGNER_INFO_PARAM, 
			0, 
			NULL, 
			&dwSignerInfo))
		{
			__leave;
		}

		// Allocate memory for signer information.
		pSignerInfo = (PCMSG_SIGNER_INFO)LocalAlloc(LPTR, dwSignerInfo);
		if (!pSignerInfo)
		{
			__leave;
		}

		// Get Signer Information.
		if (!CryptMsgGetParam(hMsg, 
			CMSG_SIGNER_INFO_PARAM, 
			0, 
			(PVOID)pSignerInfo, 
			&dwSignerInfo))
		{
			__leave;
		}

		// Search for the signer certificate in the temporary 
		// certificate store.
		CertInfo.Issuer = pSignerInfo->Issuer;
		CertInfo.SerialNumber = pSignerInfo->SerialNumber;

		pCertContext = CertFindCertificateInStore(hStore,
			ENCODING,
			0,
			CERT_FIND_SUBJECT_CERT,
			(PVOID)&CertInfo,
			NULL);
		if (!pCertContext)
		{
			__leave;
		}

		_tprintf(_T("Serial Number: "));
		dwData = pCertContext->pCertInfo->SerialNumber.cbData;

		// Get Issuer name size.
        dwData = CertGetNameString(pCertContext, 
            CERT_NAME_SIMPLE_DISPLAY_TYPE,
            CERT_NAME_ISSUER_FLAG,
            NULL,
            NULL,
            0);

		if (!dwData)
		{
			__leave;
		}

		// Allocate memory for Issuer name.
		szName = (LPTSTR)LocalAlloc(LPTR, dwData * sizeof(TCHAR));
		if (!szName)
		{
			__leave;
		}

		// Get subject name.
		if (!(CertGetNameString(pCertContext, 
			CERT_NAME_SIMPLE_DISPLAY_TYPE,
			0,
			NULL,
			szName,
			dwData)))
		{
			__leave;
		}

		// Print Subject Name.
		_tprintf(_T("Subject Name: %s\n"), szName);
		return szName;
	}
	__finally
	{               
		if (pSignerInfo != NULL) LocalFree(pSignerInfo);
		if (pCounterSignerInfo != NULL) LocalFree(pCounterSignerInfo);
		if (pCertContext != NULL) CertFreeCertificateContext(pCertContext);
		if (hStore != NULL) CertCloseStore(hStore, 0);
		if (hMsg != NULL) CryptMsgClose(hMsg);
	}

	return NULL;
}

bool KFile::GetFileVersion( const wstring& fileName, DWORD *pdwFileVersionMS, DWORD *pdwFileVersionLS )
{
	if (fileName.empty() || !pdwFileVersionMS || !pdwFileVersionLS)
	{
		return false;
	}

	LPDWORD dwHandle = NULL;
	DWORD dwSize;
	dwSize=GetFileVersionInfoSize(fileName.c_str(), dwHandle);
	if (dwSize==0)
		return false;

	wchar_t* pData=new wchar_t[dwSize];

	if (!GetFileVersionInfo(fileName.c_str(), NULL, dwSize, pData))
	{
		delete[] pData;
		return false;
	}
	VS_FIXEDFILEINFO *pVerInfo;
	UINT uSize = sizeof(pVerInfo);
	VerQueryValue(pData, L"\\", (LPVOID*)&pVerInfo, &uSize);
	
	if (uSize > 0)
	{
		*pdwFileVersionMS = pVerInfo->dwFileVersionMS;
		*pdwFileVersionLS = pVerInfo->dwFileVersionLS;
	}

	delete[] pData;
	return true;
}

HANDLE KFile::GetFileHandle(const wstring& filename)
{
    if (filename.empty())
    {
        return NULL;
    }

	HANDLE hFile = NULL;
	if (KFile::Exists(filename))
	{
		hFile = ::CreateFile(
			filename.c_str(),
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_SEQUENTIAL_SCAN,
			NULL);
	}
	return hFile;
}

bool KFile::GetFileVersion( const wstring& fileName, wstring& fileVersion )
{
    if (fileName.empty())
    {
        return false;
    }

	DWORD dwFileVersionMS = 0; 
	DWORD dwFileVersionLS = 0;

	if (!KFile::GetFileVersion(fileName, &dwFileVersionMS, &dwFileVersionLS))
		return false;
	
	wchar_t wszFileVersion[MAX_PATH] = {0};

	swprintf_s(wszFileVersion, MAX_PATH, L"%04d.%02d.%02d.%d",
		HIWORD(dwFileVersionMS),
		LOWORD(dwFileVersionMS),
		HIWORD(dwFileVersionLS),
		LOWORD(dwFileVersionLS));
	
	fileVersion.assign(wszFileVersion);

	return true;
}


bool KFile::Copy( const wstring& sourceFileName, const wstring& destFileName, bool overwrite /*= true*/ )
{
    if (sourceFileName.empty() || destFileName.empty())
    {
        return false;
    }

	wstring sourceFileFullPath = KPath::GetFullPath(sourceFileName);
	wstring destFileFullPath = KPath::GetFullPath(destFileName);

	if (overwrite)
	{
		return (TRUE == ::CopyFile(sourceFileFullPath.c_str(), destFileFullPath.c_str(), FALSE));
	}

	if (::CopyFile(sourceFileFullPath.c_str(), destFileFullPath.c_str(), TRUE))
	{
		return true;
	}
	else
	{
		// Error Code 80 表示文件存在
		return 80 == GetLastError();
	}
}

bool KFile::Exists( const wstring& path )
{
    if (path.empty())
    {
        return false;
    }

	wstring fullPath = KPath::GetFullPath(path);
	return (TRUE == ::PathFileExists(fullPath.c_str()));
}

bool KFile::Delete( const wstring& path )
{
    if (path.empty())
    {
        return false;
    }

	wstring fullPath = KPath::GetFullPath(path);
	if (::DeleteFile(fullPath.c_str()))
	{
		return true;
	}
	else
	{
		// Error Code 2 表示文件不存在
		return ERROR_FILE_NOT_FOUND == GetLastError();
	}
}

bool KFile::Rename( const wstring& oldPath, const wstring& newPath, bool replaceWhenExist)
{
    if (oldPath.empty() || newPath.empty())
    {
        return false;
    }

    wstring fullOldPath = KPath::GetFullPath(oldPath);
    wstring fullNewPath = KPath::GetFullPath(newPath);

    if (!KPath::Exists(fullOldPath))
    {
        return false;
    }

    if (KPath::Exists(fullNewPath) && replaceWhenExist)
    {
        if(!::DeleteFile(fullNewPath.c_str()))
        {
            return false;
        }
    }

    if (::MoveFile(fullOldPath.c_str(), fullNewPath.c_str()))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool KFile::CreateTextFile( const wstring& filePath, const wstring& fileContent, bool isUTF8, bool isUseBom )
{
    if (filePath.empty())
    {
        return false;
    }

    wstring fullPath = KPath::GetFullPath(filePath);
    if (KPath::Exists(fullPath))
    {
        return false;
    }
    
    HANDLE hFile = ::CreateFile(fullPath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    DWORD dwWritted = 0;
    DWORD nNumberOfByteToWrite = (DWORD)(fileContent.size()*sizeof(WCHAR));

    if (isUTF8)
    {
        string utf8String = KStringConvert::WideToUtf8(fileContent);
        if (isUseBom)
        {
            utf8String = "\xEF\xBB\xBF" + utf8String;
        }
        
        nNumberOfByteToWrite = (DWORD)(utf8String.size());
        if(!::WriteFile(hFile, utf8String.c_str(), nNumberOfByteToWrite, &dwWritted, NULL))
        {
            CloseHandle(hFile);
            return false;
        }
    }
    else
    {
		// 非UTF8的话，默认用Ansi
		string aString = KStringConvert::WideToAnsi(fileContent);
		nNumberOfByteToWrite = (DWORD)(aString.size());
        if(!::WriteFile(hFile, aString.c_str(), nNumberOfByteToWrite, &dwWritted, NULL))
        {
            CloseHandle(hFile);
            return false;
        }
    }

    CloseHandle(hFile);
    return true;
}

wstring KFile::ReadAllTextUTF16( const wstring& path )
{
	wstring allText;

	if (path.empty())
	{
		return false;
	}

	wstring fullPath = KPath::GetFullPath(path);

	if (!KPath::Exists(fullPath))
	{
		return L"";
	}

	HANDLE hFile = ::CreateFile(fullPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return L"";
	}

	DWORD dwLen = ::GetFileSize(hFile, NULL);
	wchar_t* readBuf = new wchar_t[dwLen/2 + 1];
	::memset(readBuf, 0, dwLen/2 + 1);

	DWORD dwReadLen;
	if (!::ReadFile(hFile, readBuf, dwLen, &dwReadLen, NULL))
	{
		delete[] readBuf;
		::CloseHandle(hFile);
		return L"";
	}

	readBuf[dwLen/2] = '\0';
	allText.assign(readBuf);

	if (allText.find(L"\xFF\xFE") == 0)
	{// 自动去掉开通的BOM
		allText = allText.substr(wcslen(L"\xFF\xFE"));
	}

	delete[] readBuf;
	::CloseHandle(hFile);

	return allText;
}

wstring KFile::ReadAllText( const wstring& path, bool isUTF8 /*=true*/ )
{
    if (path.empty())
    {
        return false;
    }

	string ansiAllText;
	wstring allText;

	wstring fullPath = KPath::GetFullPath(path);
	if (!KPath::Exists(fullPath))
	{
		return L"";
	}

	HANDLE hFile = ::CreateFile(fullPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return L"";
	}

	DWORD dwLen = ::GetFileSize(hFile, NULL);
	char* readBuf = new char[dwLen + 1];
	::memset(readBuf, 0, dwLen + 1);

	DWORD dwReadLen;
	if (!::ReadFile(hFile, readBuf, dwLen, &dwReadLen, NULL))
	{
		delete[] readBuf;
		::CloseHandle(hFile);
		return L"";
	}

	ansiAllText.assign(readBuf);
		
	if (isUTF8)
	{
        if (ansiAllText.find("\xEF\xBB\xBF") == 0)
        {// 自动去掉开通的BOM
            ansiAllText = ansiAllText.substr(strlen("\xEF\xBB\xBF"));
        }

		allText = KStringConvert::Utf8ToWide(ansiAllText);
	}
	else
	{
		allText = KStringConvert::AnsiToWide(ansiAllText);
	}

	delete[] readBuf;
	::CloseHandle(hFile);
	
	return allText;
}

bool KFile::GetCreateTime(const wstring& filename, LPFILETIME lpCreateTime)
{
    if (filename.empty())
    {
        return false;
    }

	bool bOK = false;
	HANDLE hFile = KFile::GetFileHandle(filename);
	if (INVALID_HANDLE_VALUE != hFile)
	{
		bOK = (::GetFileTime(hFile, lpCreateTime, NULL, NULL) == TRUE);
		::CloseHandle(hFile);
	}

	return bOK;
}

bool KFile::GetLastAccessTime(const wstring& filename, LPFILETIME lpLastAccessTime)
{
    if (filename.empty())
    {
        return false;
    }

	bool bOK = false;
	HANDLE hFile = KFile::GetFileHandle(filename);
	if (INVALID_HANDLE_VALUE != hFile)
	{
		bOK = (::GetFileTime(hFile, NULL, lpLastAccessTime, NULL) == TRUE);
		::CloseHandle(hFile);
	}

	return bOK;
}

bool KFile::GetLastWriteTime(const wstring& filename, LPFILETIME lpLastWriteTime)
{
    if (filename.empty())
    {
        return false;
    }

	bool bOK = false;
	HANDLE hFile = KFile::GetFileHandle(filename);
	if (INVALID_HANDLE_VALUE != hFile)
	{
		bOK = (::GetFileTime(hFile, NULL, NULL, lpLastWriteTime) == TRUE);
		::CloseHandle(hFile);
	}

	return bOK;
}

bool Kingsoft::Test::CCF::IO::KFile::GetFileSize( const wstring& path, LARGE_INTEGER& fileSize )
{
    if (path.empty())
    {
        return false;
    }

    if (!KFile::Exists(path))
    {
        return false;
    }

    HANDLE fileHandle = GetFileHandle(path);

    if (INVALID_HANDLE_VALUE != fileHandle)
    {
        bool result = TRUE == ::GetFileSizeEx(fileHandle, &fileSize)? true:false;
        ::CloseHandle(fileHandle);
        return result;
    }
    else
    {
        return false;
    }
}