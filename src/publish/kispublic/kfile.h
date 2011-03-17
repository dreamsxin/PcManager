#pragma once
#include "public.h"

NS_BEGIN(KIS)
NS_BEGIN(IO)
NS_BEGIN(KFile)

/*
* @brief								判断文件或文件夹是否存在。
* @param lpFilePath					--- 指定需要判断的文件全路径
* @remark							--- 使用GetFileAttributes函数来判断文件是否存在
*										使用该方法的好处在于：可以将文件全路径的字符串长度增加到32767个字符。
*										同时能够支持"\\?\"的路径格式。
* @return 
*							   TRUE --- 存在
*							   FALSE--- 不存在
*/
STDAPIN_(int)  FileExistsA(LPCSTR lpFilePath); 
STDAPIN_(int)  FileExistsW(LPCWSTR lpFilePath); 

/*
* @brief								获取文件的大小。
* @param lpFilePath					--- 指定文件的全路径
* @param uFileSize					--- 文件的大小
* @return 
*							   TRUE --- 成功
*							   FALSE--- 失败
*/
STDAPIN_(int)  GetFileSizeA(IN LPCSTR lpStrFilePath, OUT size_t& uFileSize);
STDAPIN_(int)  GetFileSizeW(IN LPCWSTR lpStrFilePath, OUT size_t& uFileSize);

/*
* @brief								删除指定的文件。
* @param lpFilePath					--- 指定的需要删除的文件
* @param bDeleteReboot				--- 如果文件删除失败，指定是否重启删除，默认为重启删除
* @return 
*							   TRUE --- 成功
*							   FALSE--- 失败
*/
STDAPIN_(int)  DeleteFileA(IN LPCSTR lpFilePath, IN BOOL bDeleteReboot = TRUE);
STDAPIN_(int)  DeleteFileW(IN LPCWSTR lpFilePath, IN BOOL bDeleteReboot = TRUE);


/*
* @brief								删除文件，当电脑重启之后。
* @param lpFilePath					--- 指定的需要删除的文件
* @return 
*							   TRUE --- 成功
*							   FALSE--- 失败
*/
STDAPIN_(int)  MoveFileRebootA(IN LPCSTR lpFilePath);
STDAPIN_(int)  MoveFileRebootW(IN LPCWSTR lpFilePath);

/*
* @brief								拷贝文件
* @param lpFilePathSrc				--- 指定需要被拷贝的文件路径
* @param lpFilePathDes				--- 指定拷贝到的文件路径
* @param bFailIfExist				--- 如果失败就不拷贝
* @return 
*							   TRUE --- 成功
*							   FALSE--- 失败
*/
STDAPIN_(int)  CopyFileA(IN LPCSTR lpFilePathSrc, IN LPCSTR lpFilePathDes, IN BOOL bFailIfExist = FALSE);
STDAPIN_(int)  CopyFileW(IN LPCWSTR lpFilePathSrc, IN LPCWSTR lpFilePathDes, IN BOOL bFailIfExist = FALSE);


/*
* @brief								获取文件版本号
* @param lpFilePath					--- 指定需要查询的文件版本号的文件
* @param pdwFileVersionMS			--- 指定文件的版本号（高位）
* @param pdwFileVersionLS			--- 指定文件的版本号（低位）
* @return 
*							   TRUE --- 成功
*							   FALSE--- 失败
*/
STDAPIN_(int) GetFileVersionDWORDA(IN LPCSTR lpFilePath, IN OUT DWORD *pdwFileVersionMS, IN OUT DWORD *pdwFileVersionLS);
STDAPIN_(int) GetFileVersionDWORDW(IN LPCWSTR lpFilePath, IN OUT DWORD *pdwFileVersionMS, IN OUT DWORD *pdwFileVersionLS);

/*
* @brief								获取文件版本号
* @param lpFilePath					--- 指定需要查询的文件版本号的文件
* @param lpVersion					--- 文件的版本号
* @param nVersionSize				--- 指定字符串的长度，并返回的当前字符串的长度
* @return 
*							   TRUE --- 成功
*							   FALSE--- 失败
*/
STDAPIN_(int) GetFileVersionStrA(IN LPCSTR lpFilePath, IN OUT LPSTR lpVersion, IN int& nVersionSize);
STDAPIN_(int) GetFileVersionStrW(IN LPCWSTR lpFilePath, IN OUT LPWSTR lpVersion, IN int& nVersionSize);

NS_END(KFile)
NS_END(IO)
NS_END(KIS)




