#pragma once
#include "public.h"

NS_BEGIN(KIS)
NS_BEGIN(IO)
NS_BEGIN(KPath)

/*
* @brief								获取卫士OEM的目录。
* @param lpOemPath					--- 返回的金山卫士的OEM的目录，如果设置为NULL,则nSize返回实际需要的大小，返回的结果最后带有'\\'
* @param nSize						--- 指定最大的长度，返回实际需要的长度；如果函数失败，nSize为实际需要的大小
* @remark							--- 建议使用：

std::wstring strValue;
CallFunc<int, LPWSTR, size_t&>(KIS::IO::KPath::GetKafeOemPathW, strValue);
* @return 
*							   TRUE --- 成功
*							   FALSE--- 失败
*/
STDAPIN_(int)  GetKafeOemFilePathA(OUT LPSTR lpOemPath, IN OUT size_t& nSize); 
STDAPIN_(int)  GetKafeOemFilePathW(OUT LPWSTR lpOemPath, IN OUT size_t& nSize); 

/*
* @brief								得到当前调用的进程所在的文件夹路径。
* @param lpOemPath					--- 返回的金山卫士的OEM的目录，如果设置为NULL,则nSize返回实际需要的大小，返回的结果最后带有'\\
* @param nSize						--- 指定最大的长度，返回实际需要的长度；如果函数失败，nSize为实际需要的大小
* @remark							--- 建议使用：

std::wstring strValue;
CallFunc<int, LPWSTR, size_t&>(KIS::IO::KPath::GetCurDirectoryW, strValue);
* @return 
*							   TRUE --- 成功
*							   FALSE--- 失败
*/
STDAPIN_(int)  GetCurDirectoryA(OUT LPSTR lpPath, IN OUT size_t& nSize); 
STDAPIN_(int)  GetCurDirectoryW(OUT LPWSTR lpPath, IN OUT size_t& nSize); 


/*
* @brief								得到当前进程的全路径，包括文件名。
* @param lpOemPath					--- 返回当前进程的全路径，如果设置为NULL,则nSize返回实际需要的大小
* @param nSize						--- 指定最大的长度，返回实际需要的长度；如果函数失败，nSize为实际需要的大小
* @remark							--- 建议使用：

std::wstring strValue;
CallFunc<int, LPWSTR, size_t&>(KIS::IO::KPath::GetCurFullPathW, strValue);
* @return 
*							   TRUE --- 成功
*							   FALSE--- 失败
*/
STDAPIN_(int)  GetCurFullPathA(OUT LPSTR lpPath, IN OUT size_t& nSize); 
STDAPIN_(int)  GetCurFullPathW(OUT LPWSTR lpPath, IN OUT size_t& nSize); 


NS_END(KPath)
NS_END(IO)
NS_END(KIS)




