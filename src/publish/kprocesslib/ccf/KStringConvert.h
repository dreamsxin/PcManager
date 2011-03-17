/**
* @file     KStringConvert.h
* @brief    转换字符串函数
* @details  
* @author   林杰(Linjie@kingsoft.com)
* @date     2009-6-28
*/

#pragma once
#include <atlbase.h>

using namespace std;
namespace Kingsoft
{
	namespace Test
	{
		namespace CCF
		{
            namespace Text
            {
                /**
                * 字符串转换类
                */
                class KStringConvert
                {
                public:

                    /**
                    * @brief   宽字符转为窄字符
                    * @param   wstr 宽字符串
                    * @param   ansiString 窄字符串
                    * @return  成功返回true，其它返回false
                    */
                    static bool WideToAnsi(const wstring& wstr, string& ansiString);

                    /**
                    * @brief   宽字符转为窄字符
                    * @param   const wstring & wstr 宽字符串
                    * @return  std::string 窄字符串， 如果转换失败，则返回空串。
                    */
                    static string WideToAnsi(const wstring& wstr);

                    /**
                    * @brief   窄字符串转为宽字符串
                    * @param   str 窄字符串
                    * @param   wideString 宽字符串
                    * @return  成功返回true，其它返回false
                    */
                    static bool AnsiToWide(const string& str, wstring& wideString);

                    /**
                    * @brief   窄字符串转为宽字符串
                    * @param   const string & str 窄字符串
                    * @return  std::wstring 宽字符串， 如果转换失败，则返回空串。
                    */
                    static wstring AnsiToWide(const string& str);

                    /**
                    * @brief   Utf8转宽字符串
                    * @param   utf8string Utf8字符串
                    * @param   wideString 宽字符串
                    * @return  成功返回true，其它返回false
                    */
                    static bool Utf8ToWide(const string& utf8string, wstring& wideString);

                    /**
                    * @brief   UTF8字符串转为宽字符
                    * @param   const string & utf8string UTF8字符串
                    * @return  std::wstring 宽字符串， 如果转换失败，则返回空串。
                    */
                    static wstring Utf8ToWide(const string& utf8string);

                    /**
                    * @brief   宽字符串转Utf8
                    * @param   widestring 宽字符串
                    * @param   utf8String Utf8字符串
                    * @return  成功返回true，其它返回false
                    */
                    static bool WideToUtf8(const wstring& widestring, string& utf8String);

                    /**
                    * @brief   宽字符串转Utf8
                    * @param   widestring 宽字符串
                    * @return  string utf8字符串， 如果转换失败，则返回空串。
                    */
                    static string WideToUtf8(const wstring& widestring);

                    static string AnsiToUtf8(const string& ansiStr);
                    static string Utf8ToAnsi(const string& utf8Str);

                    /** 
                    * @brief   实现字符串替换 
                    * @param   originalStr 源串 
                    * @param   oldStr  查找的串 
                    * @param   newStr  替换的新串 
                    * @return  返回修改后的串 
                    * @example
                        wstring originalStr=L"a,b,c";
                        wstring replacedStr = Replace(originalStr, L",", L".");
                        // replacedStr = L"a.b.c"
                    */
                    static wstring Replace(const wstring& originalStr, const wstring& oldStr, const wstring& newStr);

                    /**
                    * @brief   将字符串转为大写
                    * @param   orignalStr 源字符串
                    * @return  std::wstring 转换后的字符串
                    */
                    static wstring ToUpper(const wstring& orignalStr);
                    static string ToUpper(const string& orignalStr);


                    /**
                    * @brief   将字符串转为小写
                    * @param   const wstring & orignalStr
                    * @return  std::wstring 转换后的字符串
                    */
                    static wstring ToLower(const wstring& orignalStr);

                    static string ToLower(const string& orignalStr);

                    /**
                    * @brief   字符串转换为布尔类型
                    * @param   wstr  字符串，如：“True”，不区分大小写
                    * @return  bool  “true/True”转换为true，其它转为false
                    * @note
                            boost 库中的转换只支持“1”转换为true，不支持“True”的转换。
                    */
                    static bool WStringToBool(const wstring& wstr);


                    /**
                    * @brief   布尔类型转换为字符串
                    * @param   bool  布尔值
                    * @return  std::wstring 转换后的字符串，true会转换为“True”，false转换为“False”
                    * @note
                           boost 库中的转换只支持true转换为“1”，false转换为“0”
                    */
                    static wstring BoolToWString(bool value);


					/**
					 * @brief 格式字符串
					 * @param wstring format
					 * @param ...
					 * @return std::wstring
					 * @note 注意！后面...参数不能直接传wstring，因为原生的...只支持wchar_t，不支持wstring。
					 *		 因此使用时，必须记得在wstring后面加c_str()方法。如：
					 *		 wstring fileName;
					 *		 KStringConvert.Format(L"%s", fileName.c_str());
					 */
					static wstring Format(wstring format, ...);

                    /**
					 * @brief 格式字符串
					 * @param string format
					 * @param ...
					 * @return string
					 * @note 注意！后面...参数不能直接传string，因为原生的...只支持char，不支持string。
					 *		 因此使用时，必须记得在string后面加c_str()方法。如：
					 *		 string fileName;
					 *		 KStringConvert.Format("%s", fileName.c_str());
					 */
					static string Format(string format, ...);

					/**
					* @brief	普通C++字符串转换到BSTR
					* @param const wstring& text
					* @note 返回的BSTR字符串需要用::SysFreeString函数进行释放
					*/
					static BSTR WStringToBSTR(const wstring& text);

					/**
					* @brief	BSTR字符串转换到普通C++字符串
					* @param	const BSTR bsText
					* @note
					*/
					static wstring BSTRToWString(const BSTR bsText);


					/**
					 * @brief 分割字符串
					 * @param const wstring & str
					 * @param const wstring & delimiter 分隔符
					 * @param int maxSplit 最大分割数量
					 * @return vector<wstring>
					 * @note 
					 */
					static vector<wstring> Split(const wstring& str, const wstring& delimiter = L" ", int maxSplit = -1);

					/**
					 * @brief 去除首位空格
					 * @param const wstring & str
					 * @return std::wstring
					 * @note 
					 */
					static wstring Trim(const wstring& str);

					/**
					 * @brief 去除前面的空格
					 * @param const wstring & str
					 * @return std::wstring
					 * @note 
					 */
					static wstring TrimLeft(const wstring& str);

					/**
					 * @brief 去除尾部的空格
					 * @param const wstring & str
					 * @return std::wstring
					 * @note 
					 */
					static wstring TrimRight(const wstring& str);
                };
            }
		}
	}
}
