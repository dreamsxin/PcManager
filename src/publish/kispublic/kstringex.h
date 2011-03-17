#pragma once

#ifndef __KIS_TEXT_KSTRINGEX_H__
#define __KIS_TEXT_KSTRINGEX_H__

#include <string>
#include <tchar.h>
#include <algorithm>
#include <assert.h>



#define KWString WString
#define KAString AString
#define KTString TString

#define defSpaceSymbolA			" \t\n\r\v"
#define defSpaceSymbolW			L" \t\n\r\v"

#ifndef ASSERT 
#define ASSERT assert
#endif



template<class _E>  struct  _CharTraits;

template<>
struct  _CharTraits<char>
{
	//选择适当的字符串。
	static inline const char* k_select ( const char* pcsz, const wchar_t* pcwsz )
	{
		return  pcsz;
	}
	//获得字符串的长度。
	static inline size_t k_tcslen(const char *str)
	{
		return strlen(str);
	}
	//获得要格式化的字符串的长度。
	static inline int k_vsctprintf_p(const char *format,va_list argptr )
	{
		return _vscprintf_p(format, argptr);
	}
	//格式化字符串。
	static inline int k_vstprintf_s(char *buffer,size_t sizeInBytes,const char *format,va_list argptr )
	{
		return vsprintf_s(buffer, sizeInBytes, format, argptr);
	}
	//比较字符串。
	static inline int k_tcscmp(const char *string1,const char *string2 )
	{
		return strcmp(string1, string2);
	}
	//比较字符串，忽略大小写。
	static inline int k_tcsicmp(const char *string1,const char *string2 )
	{
		return _stricmp(string1, string2);
	}
	//比较字符串，忽略大小写。
	static inline int k_tcsnicmp(const char *string1, const char *string2, size_t count)
	{
		return _strnicmp(string1, string2, count);
	}
	//转为小写。
	static inline errno_t k_tcslwr_s(char *str,size_t sizeInBytes)
	{
#if _MSC_VER <= 1200 // vc6.0
		_strlwr(str);
		return 0;
#else
		return _strlwr_s(str, sizeInBytes);
#endif
	}
	//转为大写。
	static inline errno_t k_tcsupr_s(char *str,size_t sizeInBytes)
	{
#if _MSC_VER <= 1200 // vc6.0
		_strupr(str);
		return 0;
#else
		return _strupr_s(str, sizeInBytes);
#endif		
	}
	//int转为字符串。
	static inline void k_int2str(int _value, char szBuffer[32])
	{
#if _MSC_VER <= 1200 // vc6.0
		_itoa(_value, szBuffer, 10);
#else
		_itoa_s(_value, szBuffer, 32, 10);
#endif		
	}
	//long转为字符串。
	static inline void k_long2str(long _value, char szBuffer[32])
	{
#if _MSC_VER <= 1200 // vc6.0
		_ltoa(_value, szBuffer, 10);
#else
		_ltoa_s(_value, szBuffer, 32, 10);
#endif	
	}
	//u_long转为字符串。
	static inline void k_ulong2str(unsigned long _value, char szBuffer[32])
	{
#if _MSC_VER <= 1200 // vc6.0
		_ultoa(_value, szBuffer, 10);
#else
		_ultoa_s(_value, szBuffer, 32, 10);
#endif
	}
	//float转为字符串。
	static inline void k_float2str(double _value, char szBuffer[32])
	{
		//_gcvt_s(szBuffer, 32, _value, 10);
#if _MSC_VER <= 1200 // vc6.0
		sprintf(szBuffer, "%f", _value);
#else
		sprintf_s(szBuffer, 32, "%f", _value);
#endif
	}

	static inline void k_char2str(char _value, char szBuffer[32])
	{
		//_gcvt_s(szBuffer, 32, _value, 10);
#if _MSC_VER <= 1200 // vc6.0
		sprintf(szBuffer, "%c", _value);
#else
		sprintf_s(szBuffer, 32, "%c", _value);
#endif
	}

	static inline void k_wchar2str(wchar_t _value, char szBuffer[32])
	{
		std::wstring strTemp = (& _value);
		strTemp = strTemp.substr(0, 1);
		//_gcvt_s(szBuffer, 32, _value, 10);
#if _MSC_VER <= 1200 // vc6.0
		sprintf(szBuffer, "%s", (_KW2A(strTemp.c_str(), strTemp.size())).c_str());
#else
		sprintf_s(szBuffer, 32, "%s", (_KW2A(strTemp.c_str(), (int)strTemp.size())).c_str());
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	//add
	//字符串转为int。
	static inline int k_str2int(const char *pBuffer)
	{
		if (pBuffer == NULL)
			return 0;

		return atoi(pBuffer);
	}

	//字符串转为long。
	static inline long k_str2long(const char* pBuffer)
	{
		if (pBuffer == NULL)
			return 0;

		return atol(pBuffer);
	}
	
	//字符串转为double。
	static inline double k_str2float(const char* pBuffer)
	{
		if (pBuffer == NULL)
			return 0;

		return atof(pBuffer);
	}

private:

	static std::wstring _KA2W(IN LPCSTR lpStrA , IN size_t nStrSize)
	{
		int nStrWSize = 0;

		ASSERT(lpStrA);
		ASSERT(nStrSize);

		if (lpStrA && nStrSize > 0)
		{
			nStrWSize = MultiByteToWideChar (CP_UTF8, 0, lpStrA, (int)nStrSize, NULL, 0);

			wchar_t *pBuffer = new wchar_t[nStrWSize + 1];
			if (pBuffer == NULL)
				return L"";

			//::ZeroMemory()
			memset(pBuffer, 0, (nStrWSize + 1) * sizeof(wchar_t) / sizeof(char));
			MultiByteToWideChar (CP_UTF8, 0, lpStrA, (int)nStrSize, pBuffer/*(wchar_t*)this->GetBuffer(nStrWSize)*/, nStrWSize);
			//ReleaseBuffer();
			std::wstring strBuffer(pBuffer);
			if (pBuffer != NULL)
			{
				delete[] pBuffer;
				pBuffer = NULL;
			}
			return strBuffer;
		}

		return L"";
	}

	static std::string _KW2A(IN LPCWSTR lpStrW , IN int nStrSize)
	{
		BOOL bIsUnicode = FALSE;
		int nStrASize = 0;

		ASSERT(lpStrW);
		ASSERT(nStrSize);

		if (lpStrW && nStrSize > 0)
		{

			nStrASize = WideCharToMultiByte (CP_ACP, 0, lpStrW, nStrSize, NULL, 0, NULL, NULL);

			char* pBuffer = new char[nStrSize + 1];
			if (pBuffer == NULL)
				return "";
			memset(pBuffer, 0, (nStrSize + 1) * sizeof(char));
			WideCharToMultiByte (CP_ACP, 0, lpStrW, nStrSize, pBuffer/*(char*)this->GetBuffer(nStrASize)*/, nStrASize, NULL, NULL);
			//ReleaseBuffer();

			if (pBuffer != NULL)
			{
				std::string strBuffer(pBuffer);
				delete[] pBuffer;
				pBuffer = NULL;
				return strBuffer;
			}

			return "";	
		}

		return "";	
	}


};

template<>
struct  _CharTraits<wchar_t>
{
	//选择适当的字符串。
	static inline const wchar_t* k_select ( const char* pcsz, const wchar_t* pcwsz )
	{
		return  pcwsz;
	}
	//获得字符串的长度。
	static inline size_t k_tcslen(const wchar_t *str)
	{
		return wcslen(str);
	}
	//获得要格式化的字符串的长度。
	static inline int k_vsctprintf_p(const wchar_t *format,va_list argptr )
	{
		return _vscwprintf_p(format, argptr);
	}
	//格式化字符串。
	static inline int k_vstprintf_s(wchar_t *buffer,size_t sizeInBytes,const wchar_t *format,va_list argptr )
	{
		return vswprintf_s(buffer, sizeInBytes, format, argptr);
	}
	//比较字符串。
	static inline int k_tcscmp(const wchar_t *string1,const wchar_t *string2 )
	{
		return wcscmp(string1, string2);
	}
	//比较字符串，忽略大小写。
	static inline int k_tcsicmp(const wchar_t *string1,const wchar_t *string2 )
	{
		return _wcsicmp(string1, string2);
	}
	//比较字符串，忽略大小写。
	static inline int k_tcsnicmp(const wchar_t *string1, const wchar_t *string2, size_t count)
	{
		return _wcsnicmp(string1, string2, count);
	}
	//转为小写。
	static inline errno_t k_tcslwr_s(wchar_t *str,size_t sizeInBytes)
	{
#if _MSC_VER <= 1200 // vc6.0
		_wcslwr(str);
		return 0;
#else
		return _wcslwr_s(str, sizeInBytes);
#endif
	}
	//转为大写。
	static inline errno_t k_tcsupr_s(wchar_t *str,size_t sizeInBytes)
	{
#if _MSC_VER <= 1200 // vc6.0
		_wcsupr(str);
		return 0;
#else
		return _wcsupr_s(str, sizeInBytes);
#endif
	}
	//int转为字符串。
	static inline void k_int2str(int _value, wchar_t szBuffer[32])
	{
#if _MSC_VER <= 1200 // vc6.0
		_itow(_value, szBuffer, 10);
#else
		_itow_s(_value, szBuffer, 32, 10);
#endif
		
	}
	//long转为字符串。
	static inline void k_long2str(long _value, wchar_t szBuffer[32])
	{
#if _MSC_VER <= 1200 // vc6.0
		_ltow(_value, szBuffer, 10);
#else
		_ltow_s(_value, szBuffer, 32, 10);
#endif
	}
	//u_long转为字符串。
	static inline void k_ulong2str(unsigned long _value, wchar_t szBuffer[32])
	{
#if _MSC_VER <= 1200 // vc6.0
		_ultow(_value, szBuffer, 10);
#else
		_ultow_s(_value, szBuffer, 32, 10);
#endif
	}
	//float转为字符串。
	static inline void k_float2str(double _value, wchar_t szBuffer[32])
	{
		//_gcvt_s(szBuffer, 32, _value, 10);
#if _MSC_VER <= 1200 // vc6.0
		swprintf(szBuffer, L"%f", _value);
#else
		swprintf_s(szBuffer, 32, L"%f", _value);
#endif
	}

	static inline void k_char2str(char _value, wchar_t szBuffer[32])
	{
		std::string strTemp = (& _value);
		strTemp = strTemp.substr(0, 1);
		//_gcvt_s(szBuffer, 32, _value, 10);
#if _MSC_VER <= 1200 // vc6.0
		swprintf(szBuffer, L"%s", (_KA2W(strTemp.c_str(), strTemp.size())).c_str());
#else
		swprintf_s(szBuffer, 32, L"%s", (_KA2W(strTemp.c_str(), strTemp.size())).c_str());
#endif
		
	}

	static inline void k_wchar2str(wchar_t _value, wchar_t szBuffer[32])
	{
		//_gcvt_s(szBuffer, 32, _value, 10);
		std::wstring strTemp = (& _value);
		strTemp = strTemp.substr(0, 1);
#if _MSC_VER <= 1200 // vc6.0
		swprintf(szBuffer, L"%s", strTemp.c_str());
#else
		swprintf_s(szBuffer, 32, L"%s", strTemp.c_str());
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	//add
	//字符串转为int。
	static inline int k_str2int(const wchar_t *pBuffer)
	{
		if (pBuffer == NULL)
			return 0;

		return _wtoi(pBuffer);
	}

	//字符串转为long。
	static inline long k_str2long(const wchar_t* pBuffer)
	{
		if (pBuffer == NULL)
			return 0;

		return _wtol(pBuffer);
	}

	//字符串转为double。
	static inline double k_str2float(const wchar_t* pBuffer)
	{
		if (pBuffer == NULL)
			return 0;

		return _wtof(pBuffer);
	}

private:

	static std::wstring _KA2W(IN LPCSTR lpStrA , IN size_t nStrSize)
	{
		int nStrWSize = 0;

		ASSERT(lpStrA);
		ASSERT(nStrSize);

		if (lpStrA && nStrSize > 0)
		{
			nStrWSize = MultiByteToWideChar (CP_UTF8, 0, lpStrA, (int)nStrSize, NULL, 0);

			wchar_t *pBuffer = new wchar_t[nStrWSize + 1];
			if (pBuffer == NULL)
				return L"";

			//::ZeroMemory()
			memset(pBuffer, 0, (nStrWSize + 1) * sizeof(wchar_t) / sizeof(char));
			MultiByteToWideChar (CP_UTF8, 0, lpStrA, (int)nStrSize, pBuffer/*(wchar_t*)this->GetBuffer(nStrWSize)*/, nStrWSize);
			//ReleaseBuffer();
			std::wstring strBuffer(pBuffer);
			if (pBuffer != NULL)
			{
				delete[] pBuffer;
				pBuffer = NULL;
			}
			return strBuffer;
		}

		return L"";
	}

	static std::string _KW2A(IN LPCWSTR lpStrW , IN int nStrSize)
	{
		BOOL bIsUnicode = FALSE;
		int nStrASize = 0;

		ASSERT(lpStrW);
		ASSERT(nStrSize);

		if (lpStrW && nStrSize > 0)
		{

			nStrASize = WideCharToMultiByte (CP_ACP, 0, lpStrW, nStrSize, NULL, 0, NULL, NULL);

			char* pBuffer = new char[nStrSize + 1];
			if (pBuffer == NULL)
				return "";
			memset(pBuffer, 0, (nStrSize + 1) * sizeof(char));
			WideCharToMultiByte (CP_ACP, 0, lpStrW, nStrSize, pBuffer/*(char*)this->GetBuffer(nStrASize)*/, nStrASize, NULL, NULL);
			//ReleaseBuffer();

			if (pBuffer != NULL)
			{
				std::string strBuffer(pBuffer);
				delete[] pBuffer;
				pBuffer = NULL;
				return strBuffer;
			}

			return "";	
		}

		return "";	
	}
};

 /*字符串扩展。。。*/
template<class _Elem, 
		class _Traits = std::char_traits<_Elem>, 
		class _Ax = std::allocator<_Elem> 
		>
class TStringEx : public std::basic_string<_Elem, _Traits, _Ax>
{
public:
	typedef TStringEx _This;
public:
	TStringEx(){}
	~TStringEx(){}

	TStringEx(IN LPCSTR lpStrA)
	{
		if (sizeof(value_type) == sizeof(char))
		{
			this->assign((_Elem*)lpStrA);
		}
		else 
		{
			this->assign((_Elem*)((_KA2W(lpStrA, (int)strlen(lpStrA))).c_str()));
		}
		//this->operator =((LPCSTR)lpStrA);
	}

	TStringEx(IN LPCWSTR lpStrW)
	{
		if (sizeof(value_type) == sizeof(wchar_t))
		{
			this->assign((_Elem*)lpStrW);
		}
		else
		{
			this->assign((_Elem *)((_KW2A(lpStrW, (int)wcslen(lpStrW))).c_str()));
		}
	}

	TStringEx(IN const std::string& strA)
	{
		if (sizeof(value_type) == sizeof(char))
		{
			this->assign((_Elem*)strA.c_str());
		}
		else 
		{
			this->assign((_Elem*)((_KA2W(strA.c_str(), (int)strA.length())).c_str()));
		}
	}

	TStringEx(IN const std::wstring& strW)
	{
		if (sizeof(value_type) == sizeof(wchar_t))
		{
			this->assign((_Elem*)strW.c_str());
		}
		else
		{
			this->assign((_Elem*)((_KW2A(strW.c_str(), (int)strW.length()).c_str())));
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//add
	TStringEx(IN int nStrA)
	{
		_Elem szBuffer[32] = { 0 };
		_CharTraits<_Elem>::k_int2str(nStrA, szBuffer);
		this->append(szBuffer);
	}

	TStringEx(IN long lStrA)
	{
		_Elem szBuffer[32] = { 0 };
		_CharTraits<_Elem>::k_long2str(lStrA, szBuffer);
		this->append(szBuffer);
	}

	TStringEx(IN float fStrA)
	{
		_Elem szBuffer[32] = { 0 };
		_CharTraits<_Elem>::k_float2str(fStrA, szBuffer);
		this->append(szBuffer);
	}

	TStringEx(IN double fStrA)
	{
		_Elem szBuffer[32] = { 0 };
		_CharTraits<_Elem>::k_float2str(fStrA, szBuffer);
		this->append(szBuffer);
	}

	TStringEx(IN unsigned long uStrA)
	{
		_Elem szBuffer[32] = { 0 };
		_CharTraits<_Elem>::k_long2str(uStrA, szBuffer);
		this->append(szBuffer);
	}

	TStringEx(IN char _value)
	{
		_Elem szBuffer[32] = { 0 };
		_CharTraits<_Elem>::k_char2str(_value, szBuffer);
		this->append(szBuffer);
	
	}

	TStringEx(IN wchar_t _value)
	{
		_Elem szBuffer[32] = { 0 };
		_CharTraits<_Elem>::k_wchar2str(_value, szBuffer);
		this->append(szBuffer);

	}
	//////////////////////////////////////////////////////////////////////////
public:

	_This & operator = (IN int _value)
	{
		_Elem szBuffer[32] = { 0 };
		
		_CharTraits<_Elem>::k_int2str(_value, szBuffer);
		
		this->assign(szBuffer);
		return (*this);
	}
	_This & operator = (IN short _value)
	{
		_Elem szBuffer[32] = { 0 };

		_CharTraits<_Elem>::k_int2str(_value, szBuffer);

		this->assign(szBuffer);
		return (*this);
	}
	_This & operator = (IN long _value)
	{
		_Elem szBuffer[32] = { 0 };

		_CharTraits<_Elem>::k_long2str(_value, szBuffer);

		this->assign(szBuffer);
		return (*this);
	}
	_This & operator = (IN unsigned long _value)
	{
		_Elem szBuffer[32] = { 0 };

		_CharTraits<_Elem>::k_ulong2str(_value, szBuffer);

		this->assign(szBuffer);
		return (*this);
	}
	_This & operator = (IN float _value)
	{
		_Elem szBuffer[32] = { 0 };

		_CharTraits<_Elem>::k_float2str(_value, szBuffer);

		this->assign(szBuffer);
		return (*this);
	}
	_This & operator = (IN double _value)
	{
		_Elem szBuffer[32] = { 0 };

		_CharTraits<_Elem>::k_float2str(_value, szBuffer);

		this->assign(szBuffer);
		return (*this);
	}

	_This & operator = (IN char _value)
	{
		_Elem szBuffer[32] = { 0 };
		_CharTraits<_Elem>::k_char2str(_value, szBuffer);
		this->append(szBuffer);

		return (*this);
	}

	_This & operator = (IN wchar_t _value)
	{
		_Elem szBuffer[32] = { 0 };
		_CharTraits<_Elem>::k_wchar2str(_value, szBuffer);
		this->append(szBuffer);

		return (*this);
	}

	//////////////////////////////////////////////////////////////////////////
	_This & operator = (IN LPCSTR lpStrA)
	{
		if (sizeof(value_type) == sizeof(char))
		{
			this->assign((_Elem*)lpStrA);
		}
		else 
		{
			this->assign((_Elem*)((_KA2W(lpStrA, (int)strlen(lpStrA))).c_str()));
		}

		return (*this);
	}

	_This & operator = (IN LPCWSTR lpStrW)
	{
		if (sizeof(value_type) == sizeof(wchar_t))
		{
			this->assign((_Elem*)lpStrW);
		}
		else
		{
			this->assign((_Elem*)((_KW2A(lpStrW, (int)wcslen(lpStrW))).c_str()));
		}

		return (*this);
	}

	_This & operator = (IN const _This& strSrc)
	{
		this->assign(strSrc.c_str());

		return (*this);
	}

	_This & operator = (IN const std::wstring& strW)
	{
		return this->operator =((LPCWSTR)strW.c_str());
	}

	_This & operator = (IN const std::string& strA)
	{
		return this->operator =((LPCSTR)strA.c_str());
	}

	//////////////////////////////////////////////////////////////////////////
	//add
	_This operator += (IN int _value)
	{

		_Elem szBuffer[32] = { 0 };

		_CharTraits<_Elem>::k_int2str(_value, szBuffer);

		this->append(szBuffer);
		return (*this);
	}
	_This & operator += (IN short _value)
	{
		_Elem szBuffer[32] = { 0 };

		_CharTraits<_Elem>::k_int2str(_value, szBuffer);

		this->append(szBuffer);
		return (*this);
	}
	_This & operator += (IN long _value)
	{
		_Elem szBuffer[32] = { 0 };

		_CharTraits<_Elem>::k_long2str(_value, szBuffer);

		this->append(szBuffer);
		return (*this);
	}
	_This & operator += (IN unsigned long _value)
	{
		_Elem szBuffer[32] = { 0 };

		_CharTraits<_Elem>::k_ulong2str(_value, szBuffer);

		this->append(szBuffer);
		return (*this);
	}
	_This & operator += (IN float _value)
	{
		_Elem szBuffer[32] = { 0 };

		_CharTraits<_Elem>::k_float2str(_value, szBuffer);

		this->append(szBuffer);
		return (*this);
	}
	_This & operator += (IN double _value)
	{
		_Elem szBuffer[32] = { 0 };

		_CharTraits<_Elem>::k_float2str(_value, szBuffer);

		this->append(szBuffer);
		return (*this);
	}

	_This & operator += (IN char lpStrA)
	{
		_Elem szBuffer[32] = { 0 };
		_CharTraits<_Elem>::k_char2str(lpStrA, szBuffer);
		this->append(szBuffer);

		return (*this);
	}

	_This & operator += (IN wchar_t lpStrW)
	{
		_Elem szBuffer[32] = { 0 };
		_CharTraits<_Elem>::k_wchar2str(lpStrW, szBuffer);
		this->append(szBuffer);

		return (*this);
	}

	_This & operator += (IN LPCSTR lpStrA)
	{
		if (sizeof(value_type) == sizeof(char))
		{
			this->append((_Elem*)lpStrA);
		}
		else 
		{
			this->append((_Elem*)((_KA2W(lpStrA, (int)strlen(lpStrA))).c_str()));
		}

		return (*this);
	}

	_This & operator += (IN LPCWSTR lpStrW)
	{
		if (sizeof(value_type) == sizeof(wchar_t))
		{
			this->append((_Elem*)lpStrW);
		}
		else
		{
			this->append((_Elem*)((_KW2A(lpStrW, (int)wcslen(lpStrW))).c_str()));
		}

		return (*this);
	}

	_This & operator += (IN const _This& strSrc)
	{
		this->append(strSrc.c_str());

		return (*this);
	}

	_This & operator += (IN const std::wstring& strW)
	{
		return this->operator +=((LPCWSTR)strW.c_str());
	}

	_This & operator += (IN const std::string& strA)
	{
		return this->operator +=((LPCSTR)strA.c_str());
	}

	//////////////////////////////////////////////////////////////////////////	
	//operator +

	_This operator + (IN int _value)
	{
		_This strTemp = this->c_str();
		_Elem szBuffer[32] = { 0 };

		_CharTraits<_Elem>::k_int2str(_value, szBuffer);

		strTemp.append(szBuffer);
		return strTemp;
	}

	_This  operator + (IN short _value)
	{
		_This strTemp = this->c_str();
		_Elem szBuffer[32] = { 0 };

		_CharTraits<_Elem>::k_int2str(_value, szBuffer);

		strTemp.append(szBuffer);
		return strTemp;
	}
	_This operator + (IN long _value)
	{
		_This strTemp = this->c_str();
		_Elem szBuffer[32] = { 0 };

		_CharTraits<_Elem>::k_long2str(_value, szBuffer);

		strTemp.append(szBuffer);
		return strTemp;
	}
	_This  operator + (IN unsigned long _value)
	{
		_This strTemp = this->c_str();
		_Elem szBuffer[32] = { 0 };

		_CharTraits<_Elem>::k_ulong2str(_value, szBuffer);

		strTemp.append(szBuffer);
		return strTemp;
	}
	_This operator + (IN float _value)
	{
		_This strTemp = this->c_str();
		_Elem szBuffer[32] = { 0 };

		_CharTraits<_Elem>::k_float2str(_value, szBuffer);

		strTemp.append(szBuffer);
		return strTemp;
	}
	_This operator + (IN double _value)
	{
		_This strTemp = this->c_str();
		_Elem szBuffer[32] = { 0 };

		_CharTraits<_Elem>::k_float2str(_value, szBuffer);

		strTemp.append(szBuffer);
		return strTemp;
	}
	_This operator + (IN char lpStrA)
	{
		_This strTemp = this->c_str();
		_Elem szBuffer[32] = { 0 };
		_CharTraits<_Elem>::k_char2str(lpStrA, szBuffer);
		strTemp.append(szBuffer);
		return strTemp;
	}
	_This operator + (IN wchar_t lpStrW)
	{
		_This strTemp = this->c_str();
		_Elem szBuffer[32] = { 0 };
		_CharTraits<_Elem>::k_wchar2str(lpStrW, szBuffer);
		strTemp.append(szBuffer);
		return strTemp;
	}
	_This operator + (IN LPCSTR lpStrA)
	{
		_This strTemp = this->c_str();
		if (sizeof(value_type) == sizeof(char))
			strTemp.append((_Elem*)lpStrA);
		else 
			strTemp.append((_Elem*)((_KA2W(lpStrA, (int)strlen(lpStrA))).c_str()));
		
		return strTemp;
	}

	_This operator + (IN LPCWSTR lpStrW)
	{
		_This strTemp = this->c_str();
		if (sizeof(value_type) == sizeof(char))
			strTemp.append((_Elem*)((_KW2A(lpStrW, (int)wcslen(lpStrW))).c_str()));
		else 
			strTemp.append((_Elem*)lpStrW);

		return strTemp;
	}

	_This operator + (IN const _This& strSrc)
	{
		_This strTemp = this->c_str();
		strTemp.append(strSrc.c_str());

		return strTemp;
	}

	_This operator + (IN const std::wstring& strW)
	{
		_This strTemp = this->c_str();
		return strTemp.operator +=((LPCWSTR)strW.c_str());
	}

	_This operator + (IN const std::string& strA)
	{
		_This strTemp = this->c_str();
		return strTemp.operator +=((LPCSTR)strA.c_str());
	}

public:
	operator int()
	{
		return _CharTraits<_Elem>::k_str2int(this->c_str());
	}

	operator long()
	{
		return _CharTraits<_Elem>::k_str2long(this->c_str());
	}

	operator float()
	{
		return _CharTraits<_Elem>::k_str2float(this->c_str());
	}

	operator double()
	{
		return _CharTraits<_Elem>::k_str2float(this->c_str());
	}

	//////////////////////////////////////////////////////////////////////////
public:
	/*
	@ FuncName	: GetBuffer
	@ Param		: nBufLen 获取Buffer的长度
	@ Brief		: 本函数仅给字符对象分配可用的内存。
	*/
	_Elem * GetBuffer(IN int nBufLen = -1)
	{
		size_type nLen = 0;
		_Elem* _Buf = NULL;

		nLen = (nBufLen == -1 ? length() : nBufLen) + 1;
		// The 'reserve' method cannot be used here because 'resize' 
		// called in the set_buf method will prevent in this case only 
		// as many initial characters of the new string as was the 
		// length of this string when 'get_buf' was invoked.
		resize (nLen);
		begin();	// freeze the buffer
		_Buf = const_cast<_Elem*>(c_str());

		_Buf[nLen-1] = _T('\0');
		return  _Buf;
	}
	/*
	@ FuncName	: ReleaseBuffer
	@ Param		: nBufLen 保留Buffer的长度
	@ Brief		: 调用GetBuffer后应马上调用ReleaseBuffer。
	*/
	void  ReleaseBuffer (IN int nBufLen = -1 )
	{
		size_type nLen = 0;
		nLen = (nBufLen == -1 ? _Traits::length(c_str()) : nBufLen );

		resize (nLen);
	}
	//////////////////////////////////////////////////////////////////////////
	/*
	@ FuncName	: MakeUpper
	@ Brief		: 将字符串转为大写。
	*/
	_This & MakeUpper(void)
	{
		std::transform(this->begin(), this->end(), this->begin(), toupper);
		//哪个效率高？
		//_CharTraits<_Elem>::k_tcsupr_s(this->GetBuffer(), this->length());
		//this->ReleaseBuffer();

		return (*this);
	}
	/*
	@ FuncName	: MakeLower
	@ Brief		: 将字符串转为小写。
	*/
	_This & MakeLower(void)
	{
		std::transform(this->begin(), this->end(), this->begin(), tolower);
		//哪个效率高？
		//_CharTraits<_Elem>::k_tcslwr_s(this->GetBuffer(), this->length());
		//this->ReleaseBuffer();

		return (*this);
	}
	/*
	@ FuncName	: Remove
	@ Param		: _E 要Remove的字符。
	@ Brief		: 删除字符串中所有指定的字符，并返回删除的个数。
	*/
	int Remove(IN _Elem _E)
	{
		size_type nRemoveCount = 0;

		nRemoveCount = this->size();
		
		this->erase( std::remove_if(this->begin(), 
									this->end(), 
									std::bind2nd( std::equal_to<_Elem>(), 
												  _E)
												  ), 
									 this->end()
					);
		
		nRemoveCount -= this->size();
		
		return ((int)nRemoveCount);
	}
	/*
	@ FuncName	: Replace
	@ Param		: chOld 被替换的字符。
	@ Param		: chNew 替换的字符。
	@ Brief		: 替换指定的字符。替换成功返回true。
	*/
	int Replace ( _Elem chOld, _Elem chNew )
	{
		int nResult = false;

		_Elem * lpStr = (_Elem*)c_str();
		while ( *lpStr )  
		{
			if ( *lpStr == chOld )
			{
				*lpStr = chNew;
				//  可替换，则返回true。
				nResult = true;
			}
			lpStr++;
		}

		return nResult;
	}
	/*
	@ FuncName	: Replace
	@ Param		: pszOld 被替换的字符。
	@ Param		: pszNew 替换的字符。
	@ Brief		: 替换指定字符串。替换成功返回true。
	*/
	int Replace(IN const _Elem * pszOld, IN const _Elem * pszNew)
	{
		int nResult = false;
		size_type pos = 0;
		size_type srclen = 0;
		size_type dstlen = 0;

		ASSERT(pszOld);
		ASSERT(pszNew);
	//	KAV_PROCESS_ERROR(pszOld);
	//	KAV_PROCESS_ERROR(pszNew);
		if (pszOld == NULL || pszNew == NULL)
			goto Exit0;

		srclen = _CharTraits<_Elem>::k_tcslen(pszOld);
		dstlen = _CharTraits<_Elem>::k_tcslen(pszNew);
		
		while (npos != ( pos = this->find(pszOld, pos)) )
		{
			this->replace(pos, srclen, pszNew, dstlen);
			pos += dstlen;
			//  可替换，则返回true。
			nResult = true;
		}
		
	Exit0:
		return nResult;
	}
	/*
	@ FuncName	: Format
	@ Param		: szFormat 格式化格式。
	@ Brief		: 格式化指定替换指定字符串。
	*/
	int Format(IN const _Elem * szFormat, ...)
	{
		int nResult = false;
		va_list args;

		va_start(args, szFormat);
		nResult = this->FormatV(szFormat, args);
		va_end(args);

		return nResult;
	}
	/*
	@ FuncName	: FormatV
	@ Param		: szFormat 格式化格式。
	@ Param		: args 可变参数列表。
	@ Brief		: 格式化指定替换指定字符串。
	*/
	int FormatV(IN const _Elem * szFormat, IN va_list args)
	{
		int nNeedLen = 0;
		_This strBuffer;

		nNeedLen = _CharTraits<_Elem>::k_vsctprintf_p(szFormat, args) + 1;
		//强制分配内存。
		_Elem * pszbuffer = strBuffer.GetBuffer(nNeedLen);
		_CharTraits<_Elem>::k_vstprintf_s(const_cast<_Elem*>(pszbuffer), nNeedLen, szFormat, args);
		strBuffer.ReleaseBuffer();

		this->swap(strBuffer);

		return true;
		/*
		int nRetCode = -1;
		nNeedLen = MAX_PATH;
		do 
		{
			//强制分配内存。
			strBuffer.reserve(nNeedLen);

			nRetCode = _CharTraits<_Elem>::k_vstprintf_s(const_cast<_Elem*>(strBuffer.c_str()), nNeedLen, szFormat, args);

			nNeedLen += MAX_PATH;

		} while (-1 == nRetCode);
		*/
	}
	/*
	@ FuncName	: AppendFormat
	@ Param		: szFormat 格式化格式。
	@ Brief		: 在原有字符串后面，格式化指定的字符串。
	*/
	int AppendFormat(IN const _Elem * szFormat, ...)
	{
		AppendFormat(_T("%d"));
		/*
		int nResult = false;
		int nNeedLen = 0;
		va_list args;
		_This strBuffer;
	
		va_start(args, szFormat);
		nNeedLen = _CharTraits<_Elem>::k_vsctprintf_p(szFormat, args) + 1;
		//强制分配内存。
		strBuffer.reserve(nWholeLen);
		_CharTraits<_Elem>::k_vstprintf_s(const_cast<_Elem*>(strBuffer.c_str()), nNeedLen, szFormat, args);
		va_end(args);

		this->append(strBuffer.c_str());

		nResult = true;

		return nResult;
		*/
		int nResult = false;
		int nNeedLen = 0;
		int nOldLen = 0;
		va_list args;

		nOldLen = (int)this->length();

		va_start(args, szFormat);
		nNeedLen = _CharTraits<_Elem>::k_vsctprintf_p(szFormat, args) + 1;
		//强制分配内存。
		
		_Elem * strbuffer = this->GetBuffer(nOldLen + nNeedLen);
		
		strbuffer += nOldLen;
		_CharTraits<_Elem>::k_vstprintf_s(strbuffer, nNeedLen, szFormat, args);
		va_end(args);

		this->ReleaseBuffer();

		nResult = true;

		return nResult;
	}
	/*
	@ FuncName	: Delete
	@ Param		: iIndex 要删除字符索引。
	@ Param		: nCount 要删除的个数。
	@ Brief		: 删除nCount个字符。
	*/
	int Delete(IN size_type iIndex, IN int nCount = 1)
	{
		this->erase(iIndex, nCount);
		
		return true;
	}
	/*
	@ FuncName	: Insert
	@ Param		: iIndex 要插入字符索引。
	@ Param		: _E 插入的字符。
	@ Brief		: 插入字符。
	*/
	int Insert(IN size_type iIndex, IN _Elem _E)
	{
		size_type nLen = 0;

		nLen = this->length();
		if (iIndex > nLen)
		{
			iIndex = nLen;
		}
		
		this->insert(iIndex, 1, _E);

		return true;
	}
	/*
	@ FuncName	: Insert
	@ Param		: iIndex 要插入字符索引。
	@ Param		: pszStr 插入字符串。
	@ Brief		: 插入字符串。
	*/
	int Insert(IN size_type iIndex, IN _Elem * pszStr)
	{
		int nResult = false;
		size_type nLen = 0;

		ASSERT(pszStr);
		//KAV_PROCESS_ERROR(pszStr);
		if (pszStr == NULL)
			goto Exit0;

		nLen = this->length();
		if (iIndex > nLen)
		{
			iIndex = nLen;
		}

		this->insert(iIndex,pszStr);
	
		nResult = true;
	Exit0:
		return nResult;
	}


	_Elem GetAt(int nIndex)
	{
		if (nIndex >= 0 && nIndex < this->size())
			return (*this)[nIndex];
		
		return -1;
	}

	void SetAt(int nIndex, _Elem charValue)
	{
		if (nIndex >= 0 && nIndex < this->size())
			(*this)[nIndex] = charValue;
	}

	void PathRemoveFileSpec(void)
	{
		size_t nPos = ReverseFind('\\');
		if (nPos != npos)
			this->assign(Left(nPos + 1));
	}

	void AddPathEnd(void)
	{
		if (Right(1) != _T("\\"))
			this->append(_T("\\"));
	}

	/*
	@ FuncName	: IsEmpty
	@ Brief		: 是否为空。
	*/
	bool IsEmpty(void)
	{
		return this->empty();
	}
private:
	//不提供Empty实现！
	void Empty(void);
	//////////////////////////////////////////////////////////////////////////
public:
	int Compare(IN const _Elem * psz)
	{
		return _CharTraits<_Elem>::k_tcscmp(this->c_str(), psz);
	}

	int Compare(IN const std::wstring& strW)
	{
		_This strTemp(strW);

		return _CharTraits<_Elem>::k_tcscmp(this->c_str(), strTemp.c_str());
	}

	int Compare(IN const std::string& strA)
	{
		_This strTemp(strA);

		return _CharTraits<_Elem>::k_tcscmp(this->c_str(), strTemp.c_str());
	}

	int CompareNoCase(IN const _Elem * psz)
	{
		return _CharTraits<_Elem>::k_tcsicmp(this->c_str(), psz);
	}

	int CompareNoCase(IN const std::wstring& strW)
	{
		_This strTemp(strW);

		return _CharTraits<_Elem>::k_tcsicmp(this->c_str(), strTemp.c_str());
	}

	int CompareNoCase(IN const std::string& strA)
	{
		_This strTemp(strA);

		return _CharTraits<_Elem>::k_tcsicmp(this->c_str(), strTemp.c_str());
	}
	//////////////////////////////////////////////////////////////////////////
	_This  Mid (IN size_type _Pos, IN size_type _N)  const
	{
		return  substr(_Pos, _N);
	}

	_This  Mid (IN size_type _Pos)  const
	{
		size_type _N = size() - _Pos;
		return  substr(_Pos, _N);
	}


	_This  Left (IN size_type _N )  const
	{
		return  substr(0, _N);
	}

	_This  Right (IN size_type _N )  const
	{
		return  substr(max(0, (long)length() - (long)_N), _N);
	}
	//////////////////////////////////////////////////////////////////////////

	int GetLength(void) const
	{
		return (int)this->size();
	}

	size_type Find (IN _Elem _E, IN size_type _Pos = 0 ) const
	{
		return  find(_E, _Pos);
	}
	size_type Find (IN const _This& _S, IN size_type _Pos = 0 ) const
	{
		return  find(_S, _Pos);
	}

	size_type FindNoCase (IN _Elem _E, IN size_type _Pos = 0 ) const
	{
		size_type _FindOff = npos;
		size_type nStrLen = 0;
		_Elem _C[2] = { 0 };
		_Elem * lpStr = NULL;

		nStrLen = this->length();
	//	KAV_PROCESS_ERROR(_Pos >= 0 && _Pos <nStrLen);
		if (!(_Pos >=0 && _Pos < nStrLen))
			goto Exit0;

		_C[0] = _E;
		lpStr = const_cast<_Elem*>(this->c_str());

		{
			for (size_type i = _Pos; i < nStrLen; ++i)
			{
				if (!_CharTraits<_Elem>::k_tcsnicmp(lpStr + i, _C, 1) )
				{
					_FindOff = i;
					break;
				}
			}
		}
	Exit0:
		return _FindOff;
	}

	size_type FindNoCase (IN const _This& _S, IN size_type _Pos = 0 ) const
	{
		size_type _FindOff = npos;

		_Elem * lpStr = const_cast<_Elem*>(this->c_str());
		_Elem * lpStr2 = const_cast<_Elem*>(_S.c_str());
		size_type nStr1Len = this->length();
		size_type nStr2Len = _S.length();

// 		KAV_PROCESS_ERROR(_Pos >= 0 && _Pos <nStr1Len);
// 		KAV_PROCESS_ERROR(nStr1Len >= nStr2Len);
		if (!(_Pos >= 0 && _Pos <nStr1Len))
			goto Exit0;

		if (!(nStr1Len >= nStr2Len))
			goto Exit0;

		{
			for (size_type i = _Pos; i <= nStr1Len - nStr2Len ; ++i)
			{
				if (!_CharTraits<_Elem>::k_tcsnicmp(lpStr + i, lpStr2, nStr2Len) )
				{
					_FindOff = i;
					break;
				}
			}
		}
	Exit0:
		return _FindOff;
	}

	size_type ReverseFind (IN _Elem _E, IN size_type _Pos = npos ) const
	{
		return  rfind(_E, _Pos);
	}

	size_type ReverseFind (IN const _This& _S, IN size_type _Pos = npos ) const
	{
		return  rfind(_S, _Pos);
	}

	size_type ReverseFindNoCase (IN _Elem _E, IN size_type _Pos = npos ) const
	{
		size_type _FindOff = npos;
		size_type nStrLen = 0;
		_Elem _C[2] = { 0 };
		_Elem * lpStr = NULL;

		nStrLen = this->length();
		if (_Pos < 0 || _Pos > nStrLen)
		{
			_Pos = nStrLen;
		}

		_C[0] = _E;
		lpStr = const_cast<_Elem*>(this->c_str());

		for (size_type i = _Pos - 1; i >= 0; --i)
		{
			if (!_CharTraits<_Elem>::k_tcsnicmp(lpStr + i, _C, 1) )
			{
				_FindOff = i;
				break;
			}
		}

		return _FindOff;
	}

	size_type ReverseFindNoCase (IN const _This& _S, IN size_type _Pos = npos ) const
	{
		size_type _FindOff = npos;

		_Elem * lpStr = const_cast<_Elem*>(this->c_str());
		_Elem * lpStr2 = const_cast<_Elem*>(_S.c_str());
		size_type nStr1Len = this->length();
		size_type nStr2Len = _S.length();

		if (_Pos < 0 || _Pos > nStr1Len)
		{
			_Pos = nStr1Len;
		}

		for (size_type i = _Pos - nStr2Len; i >= 0; --i)
		{
			if (!_CharTraits<_Elem>::k_tcsnicmp(lpStr + i, lpStr2, nStr2Len) )
			{
				_FindOff = i;
				break;
			}
		}
		return _FindOff;
	}

	//正向查找_CharSet字符集
	size_type FindOneOf (IN const _This& _CharSet, IN size_type _Pos = 0 ) const
	{
		return find_first_of(_CharSet, _Pos);
	}
	//正向查找不存在_CharSet字符集
	size_type FindOneNotOf (IN const _This& _CharSet, IN size_type _Pos = 0 ) const
	{
		return find_first_not_of(_CharSet, _Pos);
	}
	//逆向查找_CharSet字符集
	size_type FindLastOf (IN const _This& _CharSet, IN size_type _Pos = npos ) const
	{
		return find_last_of(_CharSet, _Pos);
	}
	//逆向查找不存在_CharSet字符集
	size_type FindLastNotOf (IN const _This& _CharSet, IN size_type _Pos = npos ) const
	{
		return find_last_not_of(_CharSet, _Pos);
	}
	//////////////////////////////////////////////////////////////////////////
	_This & TrimLeft(IN _Elem _E)
	{
		size_type nFirstNotOf = 0;
		
		nFirstNotOf = this->find_first_not_of(_E);
		
		this->erase(0, nFirstNotOf);

		return (*this);
	}

	_This & TrimLeft(IN const _Elem * pszStr)
	{
		int nResult = false;
		size_type nFirstNotOf = 0;
		
		ASSERT(pszStr);
		//KAV_PROCESS_ERROR(pszStr);
		if (pszStr == NULL)
			goto Exit0;

		nFirstNotOf = this->find_first_not_of(pszStr);

		this->erase(0, nFirstNotOf);
		
	Exit0:
		return (*this);
	}
	
	_This & TrimLeft(void)
	{
		return TrimLeft(_CharTraits<_Elem>::k_select(defSpaceSymbolA, defSpaceSymbolW));
	}

	_This & TrimRight(IN _Elem _E)
	{
		size_type nLastNotOf = 0;

		nLastNotOf = this->find_last_not_of(_E);

		this->erase(nLastNotOf + 1);

		return (*this);
	}

	_This & TrimRight(IN const _Elem * pszStr)
	{
		int nResult = false;
		size_type nLastNotOf = 0;

		ASSERT(pszStr);
	//	KAV_PROCESS_ERROR(pszStr);
		if (pszStr == NULL)
			goto Exit0;


		nLastNotOf = this->find_last_not_of(pszStr);

		this->erase(nLastNotOf + 1);

Exit0:
		return (*this);
	}

	_This & TrimRight(void)
	{
		return TrimRight(_CharTraits<_Elem>::k_select(defSpaceSymbolA, defSpaceSymbolW));
	}

	_This & Trim(IN _Elem _E)
	{
		this->TrimRight(_E);
		this->TrimLeft(_E);

		return (*this);
	}

	_This & Trim(IN const _Elem * pszStr)
	{
		this->TrimRight(pszStr);
		this->TrimLeft(pszStr);
		
		return (*this);
	}

	_This & Trim()
	{
		return Trim(_CharTraits<_Elem>::k_select(defSpaceSymbolA, defSpaceSymbolW));
	}

private:

	std::wstring _KA2W(IN LPCSTR lpStrA , IN size_t nStrSize)
	{
		int nStrWSize = 0;

		ASSERT(lpStrA);
		ASSERT(nStrSize);

		if (lpStrA && nStrSize > 0)
		{
			nStrWSize = MultiByteToWideChar (CP_UTF8, 0, lpStrA, (int)nStrSize, NULL, 0);

			wchar_t *pBuffer = new wchar_t[nStrWSize + 1];
			if (pBuffer == NULL)
				return L"";

			//::ZeroMemory()
			memset(pBuffer, 0, (nStrWSize + 1) * sizeof(wchar_t) / sizeof(char));
			MultiByteToWideChar (CP_UTF8, 0, lpStrA, (int)nStrSize, pBuffer/*(wchar_t*)this->GetBuffer(nStrWSize)*/, nStrWSize);
			//ReleaseBuffer();
			std::wstring strBuffer(pBuffer);
			if (pBuffer != NULL)
			{
				delete[] pBuffer;
				pBuffer = NULL;
			}
			return strBuffer;
		}

		return L"";
	}

	std::string _KW2A(IN LPCWSTR lpStrW , IN int nStrSize)
	{
		BOOL bIsUnicode = FALSE;
		int nStrASize = 0;

		ASSERT(lpStrW);
		ASSERT(nStrSize);

		if (lpStrW && nStrSize > 0)
		{

			nStrASize = WideCharToMultiByte (CP_ACP, 0, lpStrW, nStrSize, NULL, 0, NULL, NULL);

			char* pBuffer = new char[nStrSize + 1];
			if (pBuffer == NULL)
				return "";
			memset(pBuffer, 0, (nStrSize + 1) * sizeof(char));
			WideCharToMultiByte (CP_ACP, 0, lpStrW, nStrSize, pBuffer/*(char*)this->GetBuffer(nStrASize)*/, nStrASize, NULL, NULL);
			//ReleaseBuffer();

			if (pBuffer != NULL)
			{
				std::string strBuffer(pBuffer);
				delete[] pBuffer;
				pBuffer = NULL;
				return strBuffer;
			}

			return "";	
		}

		return "";	
	}

};

typedef TStringEx<char>			AString;
typedef TStringEx<wchar_t>		WString;
typedef TStringEx<TCHAR>		TString;


#endif	//__KIS_TEXT_KSTRINGEX_H__
