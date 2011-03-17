#pragma once
#include "public.h"

NS_BEGIN(KIS)
NS_BEGIN(IO)
NS_BEGIN(KDirectory)

/*
* @brief								创建指定的文件目录。
* @param lpFilePath					--- 指定需要创建的文件夹的全路径
* @remark							--- 该函数会创建文件夹路径下的所有目录（不包括盘符）
*										如：lpStrFilePath == c:\\a\\b\\c\\
*										调用本函数后，则会创建c:\\a; c:\\a\\b; c:\\a\\b\\c\\ 三个目录
* @return 
*							   TRUE --- 创建成功
*							   FALSE--- 创建失败
*/
STDAPIN_(int)  CreateDirA(IN LPCSTR lpStrFilePath);
STDAPIN_(int)  CreateDirW(IN LPCWSTR lpStrFilePath);

/*
* @brief								创建包括文件名的指定的文件目录，但是创建之前会将文件名清除
* @param lpFilePath					--- 指定需要创建的文件夹的全路径
* @remark							--- 该函数会创建包括文件名的指定的文件目录，但是创建之前会将文件名清除
*										如：lpFileFullName == c:\\a\\b\\c.exe
*										调用本函数后，则会创建c:\\a c:\\a\\b的目录,其他和CreateDirA一样
* @return 
*							   TRUE --- 创建成功
*							   FALSE--- 创建失败
*/
STDAPIN_(int)  CreateDirCleanFileNameA(IN LPCSTR lpFileFullName);
STDAPIN_(int)  CreateDirCleanFileNameW(IN LPCWSTR lpFileFullName);


/*
* @brief								删除文件夹及其里面的文件。
* @param lpFolderPath				--- 指定的需要删除的文件夹
* @param bDeleteReboot				--- 如果文件夹删除失败，指定是否重启删除，默认为重启删除
* @remark							--- 删除指定的文件夹，如果文件夹里还有文件夹，则全部删除。
* @return 
*							   TRUE --- 删除成功
*							   FALSE--- 删除失败
*/
STDAPIN_(int)  DeleteDirA(IN LPCSTR lpFolderPath, IN BOOL bDeleteReboot = TRUE);
STDAPIN_(int)  DeleteDirW(IN LPCWSTR lpFolderPath, IN BOOL bDeleteReboot = TRUE);


/*
* @brief								拷贝文件夹。
* @param lpSrcDir					--- 指定的需要拷贝的文件夹目录
* @param lpDstDir					--- 指定的拷贝到的文件夹的目录
* @param nFailedIfExist				--- 指定如果文件存在，是否继续拷贝，TRUE为继续拷贝。FALSE为放弃拷贝。默认为TRUE
* @return 
*							   TRUE --- 存在
*							   FALSE--- 不存在
*/
STDAPIN_(int)  CopyDirA(IN LPCSTR lpSrcDir, IN LPCSTR lpDstDir, IN int nFailedIfExist = TRUE);
STDAPIN_(int)  CopyDirW(IN LPCWSTR lpSrcDir, IN LPCWSTR lpDstDir, IN int nFailedIfExist = TRUE);


NS_END(KDirectory)

NS_END(IO)
NS_END(KIS)




