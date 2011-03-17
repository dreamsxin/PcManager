#include "stdafx.h"
#include <vector>
#include <algorithm>
#include "KStringConvert.h"
#include <atlconv.h>

using namespace Kingsoft::Test::CCF::Text;
using namespace std;

bool KStringConvert::WideToAnsi(const wstring& wideString, string& ansiString)
{
    if (wideString.empty())
    {
        return false;
    }

    // 使用-1后 返回的长度包含最后的NULL字符
    int nLen = WideCharToMultiByte(CP_ACP, 0, wideString.c_str(), -1, NULL, 0, NULL, NULL);

    if (nLen<= 0) 
    {
        ansiString = "";
        return false;
    }

    vector<char> resultstring(nLen);

    WideCharToMultiByte(CP_ACP, 0, wideString.c_str(), -1, &resultstring[0], nLen, NULL, NULL);

    ansiString = string(string(&resultstring[0]));

    return true;
}


string KStringConvert::WideToAnsi( const wstring& wstr )
{
    string ansiStr;
    if (wstr.empty())
    {
        return ansiStr;
    }

    if (!WideToAnsi(wstr, ansiStr))
    {
        ansiStr = "";
    }

    return ansiStr;
}

bool KStringConvert::AnsiToWide(const string& str, wstring& wideString)
{
    if (str.empty())
    {
        return false;
    }

    int nLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

    if (nLen<= 0) 
    {
        wideString = L"";
        return false;
    }

    vector<wchar_t> resultstring(nLen);

    ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &resultstring[0], nLen);
    
    wideString = wstring(&resultstring[0]);

    return true;
}


std::wstring KStringConvert::AnsiToWide( const string& str )
{
    wstring wideStr;

    if (str.empty())
    {
        return wideStr;
    }

    if (!AnsiToWide(str, wideStr))
    {
        wideStr = L"";
    }

    return wideStr;

}
// 这里使用的是Windows API转换，
// 还有一种方法是使用Unicode组织提供的C代码：
//  http://www.unicode.org/Public/PROGRAMS/CVTUTF/ConvertUTF.h
//  http://www.unicode.org/Public/PROGRAMS/CVTUTF/ConvertUTF.c
// 该代码提供了UTF-32 UTF-16 UTF-8 三者间的互相转换
// CodeProject  提供了包装类：http://www.codeproject.com/KB/string/UtfConverter.aspx
bool KStringConvert::Utf8ToWide(const string& utf8string, wstring& wideString)
{
    if (utf8string.empty())
    {
        wideString = L"";
        return true;
    }

    int widesize = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, NULL, 0);

    // Invalid UTF-8 sequence.
    if (ERROR_NO_UNICODE_TRANSLATION == widesize ||
        0 == widesize)
    {
        wideString = L"";
        return false;
    }

    vector<wchar_t> resultstring(widesize);

    int convresult = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, &resultstring[0], widesize);

    // Error in convert.
    if (convresult != widesize)
    {
        wideString = L"";
        return false;
    }

    wideString = std::wstring(&resultstring[0]);
    return true;
}

std::wstring KStringConvert::Utf8ToWide( const string& utf8string )
{
    wstring wideStr;
    if (utf8string.empty())
    {
        return wideStr;
    }

    if (!Utf8ToWide(utf8string, wideStr))
    {
        wideStr = L"";
    }

    return wideStr;
}

bool KStringConvert::WideToUtf8( const wstring& widestring, string& utf8String )
{
    if (widestring.empty())
    {
        utf8String = "";
        return true;
    }

    int utf8size = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, NULL, 0, NULL, NULL);
    if (utf8size == 0)
    {
        utf8String = "";
        return false;
    }

    vector<char> resultstring(utf8size);

    int convresult = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, &resultstring[0], utf8size, NULL, NULL);

    if (convresult != utf8size)
    {
        utf8String = "";
        return false;
    }

    utf8String = string(&resultstring[0]);
    return true;
}


std::string KStringConvert::WideToUtf8( const wstring& widestring )
{
    if (widestring.empty())
    {
        return string("");
    }

    string str;
    if (!WideToUtf8(widestring, str))
    {
        str = "";
    }

    return str;
}

std::wstring KStringConvert::Replace( const wstring& originalStr, const wstring& oldStr, const wstring& newStr )
{
    if (originalStr.empty())
    {
        return wstring(L"");
    }

    size_t pos = 0; 
    wstring tempStr = originalStr; 
    wstring::size_type newStrLen = newStr.length(); 
    wstring::size_type oldStrLen = oldStr.length(); 
    while(true) 
    { 
        pos = tempStr.find(oldStr, pos); 
        if (pos == wstring::npos) 
        {
            break;
        }

        tempStr.replace(pos, oldStrLen, newStr);         
        pos += newStrLen;
    }

    return tempStr;
}

std::wstring KStringConvert::ToUpper( const wstring& orignalStr )
{
    if (orignalStr.empty())
    {
        return wstring(L"");
    }

    wstring convertedStr = orignalStr;
    transform(convertedStr.begin(), convertedStr.end(), convertedStr.begin(), towupper);

    return convertedStr;
}

std::wstring KStringConvert::ToLower( const wstring& orignalStr )
{
    if (orignalStr.empty())
    {
        return wstring(L"");
    }

    wstring convertedStr = orignalStr;
    transform(convertedStr.begin(), convertedStr.end(), convertedStr.begin(), towlower);

    return convertedStr;
}


std::wstring KStringConvert::BoolToWString( bool value )
{
    if (value)
    {
        return L"True";
    }
    else
    {
        return L"False";
    }
}

bool KStringConvert::WStringToBool( const wstring& wstr )
{
    wstring tempStr = KStringConvert::ToLower(wstr);
    if (tempStr == L"true")
    {
        return true;
    }
    else
    {
        return false;
    }
}

wstring KStringConvert::Format( wstring format, ... )
{
	if (format.empty())
	{
		return wstring(L"");
	}

	wstring retStr(L"");

	va_list marker = NULL; 

	// initialize variable arguments
	va_start(marker, format); 

	// Get formatted string length adding one for NULL
	size_t len = _vscwprintf(format.c_str(), marker) + 1;

	// Create a wchar vector to hold the formatted string.
	vector<wchar_t> buffer(len, '\0');
	int nWritten = _vsnwprintf_s(&buffer[0], buffer.size(), len, format.c_str(), marker);    

	if (nWritten > 0)
	{
		retStr = &buffer[0];
	}

	// Reset variable arguments
	va_end(marker); 

	return retStr; 
}

BSTR KStringConvert::WStringToBSTR(const wstring& text)
{
	BSTR bsText = NULL;
	if (!text.empty())
    {
		bsText = ::SysAllocString(text.c_str());
    }

	return bsText;
}

wstring KStringConvert::BSTRToWString(const BSTR bsText)
{
	USES_CONVERSION;
	wstring value = L"";
	if (::SysStringLen(bsText) > 0)
    {
		value = OLE2W(bsText);
    }

	return value;
}

vector<wstring> KStringConvert::Split(const wstring& str, const wstring& delimiter /*= L""*/, int maxSplit /*= -1*/)
{
	vector<wstring> strVector;

    if (str.empty())
    {
        return strVector;
    }

	size_t offsize = 0;
	size_t count = 0;

	while (offsize < str.size())
	{
		size_t startIndex = str.find(delimiter, offsize);

		if (startIndex == std::wstring::npos)
		{
			break;
		}

		size_t subStrCount = startIndex - offsize;
		wstring subStr = str.substr(offsize, subStrCount);
		
		subStr = KStringConvert::Trim(subStr);

		offsize = offsize + subStrCount + delimiter.size();

		if (!subStr.empty())
		{
			strVector.push_back(subStr);
			count++;
			if (count == static_cast<size_t>(maxSplit - 1))
			{
				break;
			}
		}
	}

	wstring leftStr = str.substr(offsize);
	leftStr = KStringConvert::Trim(leftStr);
	if (!leftStr.empty())
		strVector.push_back(leftStr);

	return strVector;
}

wstring KStringConvert::Trim(const wstring& str)
{
	if (str.empty())
	{
		return wstring(L"");
	}

	wstring strResult = TrimLeft(str);

	strResult = TrimRight(strResult);

	return strResult;
	
}

wstring KStringConvert::TrimLeft( const wstring& str )
{
	if (str.empty())
	{
		return wstring(L"");
	}

	wstring strResult = str;

	bool endLeftTrim = false;

	while (!endLeftTrim)
	{
		wstring subStr = strResult.substr(0, 1);
		if (subStr == L" ")
		{
			strResult = strResult.substr(1);
		}
		else
		{
			endLeftTrim = true;
		}
	}
	return strResult;
}

wstring KStringConvert::TrimRight( const wstring& str )
{
	if (str.empty())
	{
		return wstring(L"");
	}

	wstring strResult = str;

	bool endRightTrim = false;

	while (!endRightTrim)
	{
		wstring subStr = strResult.substr(strResult.size() - 1, 1);
		if (subStr == L" ")
		{
			strResult = strResult.substr(0, strResult.size() - 1);
		}
		else
		{
			endRightTrim = true;
		}
	}

	return strResult;
}

std::string KStringConvert::Format( string format, ... )
{
    if (format.empty())
    {
        return string("");
    }

    string formattedStr("");

    va_list argsList = NULL; 

    // initialize variable arguments
    va_start(argsList, format); 

    // Get formatted string length adding one for NULL
    size_t formattedStrLen = _vscprintf(format.c_str(), argsList) + 1;

    // Create a char vector to hold the formatted string.
    vector<char> buffer(formattedStrLen, '\0');

    int nWritten = _vsnprintf_s(&buffer[0], buffer.size(), formattedStrLen, format.c_str(), argsList);    

    if (nWritten > 0)
    {
        formattedStr = &buffer[0];
    }

    // Reset variable arguments
    va_end(argsList); 

    return formattedStr; 
}

std::string KStringConvert::Utf8ToAnsi( const string& utf8Str )
{
    wstring unicodeStr = Utf8ToWide(utf8Str);
    string ansistr = WideToAnsi(unicodeStr);

    return ansistr;
}
std::string KStringConvert::AnsiToUtf8( const string& ansiStr )
{
    wstring wideStr = AnsiToWide(ansiStr);
    string utf8Str = WideToUtf8(wideStr);

    return utf8Str;
}

std::string Kingsoft::Test::CCF::Text::KStringConvert::ToUpper( const string& orignalStr )
{
    if (orignalStr.empty())
    {
        return "";
    }

    string convertedStr = orignalStr;
    transform(convertedStr.begin(), convertedStr.end(), convertedStr.begin(), toupper);

    return convertedStr;
}

std::string Kingsoft::Test::CCF::Text::KStringConvert::ToLower( const string& orignalStr )
{
    if (orignalStr.empty())
    {
        return "";
    }

    string convertedStr = orignalStr;
    transform(convertedStr.begin(), convertedStr.end(), convertedStr.begin(), tolower);

    return convertedStr;
}