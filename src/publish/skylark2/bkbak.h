/**
* @file    bkbak.h
* @brief   ...
* @author  bbcallen
* @date    2010-01-05 21:21
*/

#ifndef BKBAK_H
#define BKBAK_H

#include <unknwn.h>
#include "bkbakdef.h"

NS_SKYLARK_BEGIN





/** 
* @brief    备份文件查询接口
*/
class __declspec(uuid("C49884D9-EC30-4916-9B0D-7293FB6CA4D5"))
IBKFileBackupFinder: public IUnknown
{
public:

    /** 
    * @brief        查找备份目录的第一个文件
    * @param[out]   pBackupID       返回备份文件标识
    * @param[out]   pFindData       如果不为NULL,会返回备份文件压缩后的信息
    * @param[out]   pbstrOrigPath   返回原始路径
    */
    virtual BOOL STDMETHODCALLTYPE FindFirstBackupFile(
        BKBAK_BACKUP_ID*            pBackupID,
        BKBAK_BACKUP_SHORT_INFO*    pBackupData     = NULL,
        BSTR*                       pbstrOrigPath = NULL) = 0;

    /** 
    * @brief        查找备份目录的下一个文件
    * @param[out]   pBackupID       返回备份文件标识
    * @param[out]   pFindData       如果不为NULL,会返回备份文件压缩后的信息
    * @param[out]   pbstrOrigPath   返回原始路径
    */
    virtual BOOL STDMETHODCALLTYPE FindNextBackupFile(
        BKBAK_BACKUP_ID*            pBackupID,
        BKBAK_BACKUP_SHORT_INFO*    pBackupData     = NULL,
        BSTR*                       pbstrOrigPath = NULL) = 0;

    /** 
    * @brief        根据备份文件标识获取详细
    * @param[in]    byBackupID      备份文件标识
    * @param[out]   pBackupData     返回备份信息
    * @param[out]   pbstrOrigPath   返回原始路径
    */
    virtual HRESULT STDMETHODCALLTYPE GetBackupInfo(
        const BKBAK_BACKUP_ID*  pBackupID,
        BKBAK_BACKUP_INFO*      pBackupData,
        BSTR*                   pbstrOrigPath) = 0;

    /** 
    * @brief        根据备份文件标识获取较短的信息
    * @param[in]    byBackupID      备份文件标识
    * @param[out]   pBackupData     返回备份信息
    * @param[out]   pbstrOrigPath   返回原始路径
    * @remark       该调用比 GetBackupInfo 更快
    */
    virtual HRESULT STDMETHODCALLTYPE GetBackupShortInfo(
        const BKBAK_BACKUP_ID*      pBackupID,
        BKBAK_BACKUP_SHORT_INFO*    pBackupData,
        BSTR*                       pbstrOrigPath) = 0;
};



// 上传接口的前向声明
class   IBKEngUploader;
class   IBKProgress;
struct  BKENG_UPLOAD_PARAM;


/** 
* @brief    备份文件接口
*/
class __declspec(uuid("0D3FFB09-C94F-4d58-8D7D-C50D679F2382"))
IBKFileBackup: public IUnknown
{
public:

    /** 
    * @brief        查找备份目录的第一个文件
    * @param[in]    lpszBackupFile  要备份的文件
    * @param[in]    pBackupParam    备份参数
    * @param[out]   pBackupID       如果不为NULL,会返回生成的备份标示
    * @param[out]   pBackupInfo     如果不为NULL,会返回生成的备份信息
    */
    virtual HRESULT STDMETHODCALLTYPE BackupFile(
        LPCWSTR                     lpszBackupFile,
        const BKBAK_BACKUP_PARAM*   pBackupParam,
        BKBAK_BACKUP_ID*            pBackupID,
        BKBAK_BACKUP_INFO*          pBackupInfo = NULL) = 0;

    /** 
    * @brief        恢复指定的文件
    * @param[in]    pBackupID           要恢复的文件标示
    * @param[in]    bOverwrite          如果恢复的位置有文件,是否覆盖之
    * @param[in]    lpszNewDirectory    恢复目录,如果为NULL则恢复到原始目录
    * @param[in]    lpszNewFileName     恢复文件名,如果为NULL则恢复为原始文件名
    *
    * @retval       E_SKYLARK_RETRIEVE_FAILED_TO_OVERWRITE_DIRECTORY        无法还原文件(指定的恢复位置是一个目录)
    * @retval       E_SKYLARK_RETRIEVE_FAILED_TO_OVERWRITE_FILE             无法还原文件(指定的恢复位置存在一个文件)
    * @retval       E_SKYLARK_RETRIEVE_MISSING_BACKUP_FILE                  备份数据不存在
    * @retval       E_SKYLARK_RETRIEVE_FAILED_TO_EXTRACT_FILE               解压文件的时候发生错误
    * @retval       E_SKYLARK_RETRIEVE_FAILED_TO_EXTRACT_BACKUP_INFO        获取备份文件信息的时候发生错误
    * @retval       E_SKYLARK_RETRIEVE_FAILED_TO_REPLACE_FILE               替换文件的时候发生错误
    */
    virtual HRESULT STDMETHODCALLTYPE RetrieveFile(
        const BKBAK_BACKUP_ID*  pBackupID,
        BOOL                    bOverwrite,
        LPCWSTR                 lpszNewDirectory = NULL,
        LPCWSTR                 lpszNewFileName  = NULL) = 0;

    /** 
    * @brief        移除指定的文件
    * @param[in]    pBackupID           要移除的文件标示
    */
    virtual HRESULT STDMETHODCALLTYPE RemoveBackupFile(
        const BKBAK_BACKUP_ID*  pBackupID) = 0;

    /** 
    * @brief        移除所有备份文件
    */
    virtual HRESULT STDMETHODCALLTYPE RemoveAllBackupFiles() = 0;

    /** 
    * @brief        移除过期的备份文件
    */
    virtual HRESULT STDMETHODCALLTYPE RemoveExpiredBackupFiles() = 0;

    
    /** 
    * @brief        上传指定的文件
    * @param[in]    pBackupID           要恢复的文件标示
    * @param[in]    piUploader          上传组件
    * @param[in]    pUploadParam        上传参数,会被传递给uploader,只需要填写uUploadChannel和uUploadAct
    * @param[in]    piUploader          上传回调                              
    */
    virtual HRESULT STDMETHODCALLTYPE UploadBackupFile(
        const BKBAK_BACKUP_ID*      pBackupID,
        IBKEngUploader*             piUploader,
        const BKENG_UPLOAD_PARAM*   pUploadParam,
        IBKProgress*                piCallback) = 0;
};









/// zip的压缩接口
class __declspec(uuid("F6188AE6-ED50-4d82-A551-B7F5FFDCD8E3"))
IBKZip: public IUnknown
{
public:
    /**
    * @brief    创建zip文件
    * @param    lpZipFileName   zip文件路径
    * @param    lpszPassword    压缩口令
    * @param    lpszComment     压缩包的注释(约定为UTF-8编码)
    * @note     原来位置的文件会被清除
    */
    virtual HRESULT STDMETHODCALLTYPE CreateZipToFile(
        LPCWSTR lpZipFileName,
        LPCSTR  lpszPassword,
        LPCSTR  lpszComment) = 0;

    /**
    * @brief    创建zip文件
    * @param    hFile           zip文件句柄
    * @param    lpszPassword    压缩口令
    * @param    lpszComment     压缩包的注释(约定为UTF-8编码)
    * @note     原来位置的文件会被清除
    */
    virtual HRESULT STDMETHODCALLTYPE CreateZipToHandle(
        HANDLE  hFile,
        LPCSTR  lpszPassword,
        LPCSTR  lpszComment) = 0;

    /**
    * @brief    向zip中添加一个文件
    * @param    lpFileName      被添加的文件
    * @param    lpDisplayName   文件在zip包中的显示名
    * @param    piCallback      回调接口
    */
    virtual HRESULT STDMETHODCALLTYPE AddFile(
        LPCWSTR         lpFileName,
        LPCWSTR         lpDisplayName,
        IBKProgress*    piCallback = NULL) = 0;

    /**
    * @brief    向zip中添加一个二进制数据
    * @param    pbyData         要添加的二进制数据缓冲区
    * @param    dwBytes         数据长度
    * @param    lpDisplayName   文件在zip包中的显示名
    * @param    piCallback      回调接口
    */
    virtual HRESULT STDMETHODCALLTYPE AddData(
        const BYTE*     pbyData,
        DWORD           dwBytes,
        LPCWSTR         lpDisplayName,
        IBKProgress*    piCallback = NULL) = 0;

    /**
    * @brief    向zip中添加一个文件
    * @param    hFile           被添加的文件句柄
    * @param    lpDisplayName   文件在zip包中的显示名
    * @param    piCallback      回调接口
    */
    virtual HRESULT STDMETHODCALLTYPE AddFileHandle(
        HANDLE          hFile,
        LPCWSTR         lpDisplayName,
        IBKProgress*    piCallback = NULL) = 0;
};


/// zip的解压接口
class __declspec(uuid("B56C6BE9-3BAE-480d-9794-A2E5553DFBE7"))
IBKUnzip: public IUnknown
{
public:
    /**
    * @brief    打开zip文件用于解压
    * @param    lpZipFileName   zip文件路径
    * @param    lpszPassword    压缩口令
    */
    virtual HRESULT STDMETHODCALLTYPE OpenZipFromFile(
        LPCWSTR lpZipFileName,
        LPCSTR  lpszPassword) = 0;

    /**
    * @brief    打开zip文件用于解压
    * @param    hFile           zip文件句柄
    * @param    lpszPassword    压缩口令
    */
    virtual HRESULT STDMETHODCALLTYPE OpenZipFromHandle(
        HANDLE  hFile,
        LPCSTR  lpszPassword) = 0;

    /**
    * @brief    获取压缩包的注释
    * @param    lpBuf           保存注释的缓冲区(约定为UTF-8编码)
    * @param    dwLen           缓冲区的长度(字符数)
    * @param    pdwRealLen      返回注释的实际长度
    */
    virtual HRESULT STDMETHODCALLTYPE GetGlobalComment(
        LPSTR   lpBuf,
        DWORD   dwLen,
        DWORD*  pdwRealLen) = 0;


    /**
    * @brief    查找压缩包中的文件
    * @param    lpFileNameInZip 要查找的文件在压缩包中的名字,如果为NULL,则返回第一个文件
    */
    virtual BOOL    STDMETHODCALLTYPE FindFirstFileInZip(LPCWSTR lpFileNameInZip = NULL) = 0;

    /**
    * @brief    查找压缩包中下一个文件
    * @note     如果 FindFirstFileInZip 指定了文件名,那么这里会迭代到当前文件的下一个文件
    */
    virtual BOOL    STDMETHODCALLTYPE FindNextFileInZip() = 0;

    /**
    * @brief    压缩包中没有更多的文件
    */
    virtual BOOL    STDMETHODCALLTYPE IsEndOfFindInZip() = 0;

    /**
    * @brief    获取压缩包内的当前文件的文件名
    * @param    pbstrFileNameInZip  返回文件名
    */
    virtual HRESULT STDMETHODCALLTYPE GetFileNameInZip(BSTR* pbstrFileNameInZip) = 0;

    /**
    * @brief    获取压缩包内的当前文件的大小(压缩前)
    * @param    pullSize    返回文件大小
    */
    virtual HRESULT STDMETHODCALLTYPE GetFileSize(ULONGLONG* pullSize) = 0;

    /**
    * @brief    将压缩包内的当前文件解压到指定位置
    * @param    lpTargetPath        解压文件的输出路径,如果为NULL,则使用当前目录,文件名使用压缩包中的名字
    * @param    piCallback          回调接口
    * @param    bOverwrite          是否覆盖原来位置已有的文件
    */
    virtual HRESULT STDMETHODCALLTYPE ExtractToPath(
        LPCWSTR         lpTargetPath        = NULL,
        IBKProgress*    piCallback          = NULL,
        BOOL            bOverwrite          = TRUE) = 0;


    /**
    * @brief    将压缩包内的当前文件解压到指定位置
    * @param    lpTargetDir         解压文件的输出目录,如果为NULL,则使用当前目录
    * @param    lpTargetFileName    解压文件的输出文件名,如果为NULL,则使用压缩包中的名字
    * @param    piCallback          回调接口
    * @param    bOverwrite          是否覆盖原来位置已有的文件
    */
    virtual HRESULT STDMETHODCALLTYPE ExtractToDirectory(
        LPCWSTR         lpTargetDir         = NULL,
        LPCWSTR         lpTargetFileName    = NULL,
        IBKProgress*    piCallback          = NULL,
        BOOL            bOverwrite          = TRUE) = 0;

    /**
    * @brief    将压缩包内的当前文件解压到指定句柄
    * @param    hFileHandle         解压文件的输出句柄
    * @param    piCallback          回调接口
    */
    virtual HRESULT STDMETHODCALLTYPE ExtractToHandle(
        HANDLE          hFileHandle,
        IBKProgress*    piCallback = NULL) = 0;

    /**
    * @brief    将压缩包内的当前文件解压到指定内存位置
    * @param    pBuffer             解压文件的缓冲区
    * @param    dwBufferSize        缓冲区大小
    * @param    piCallback          回调接口
    * @retval   S_OK                已经解压完成
    * @retval   S_FALSE             还有更多数据
    */
    virtual HRESULT STDMETHODCALLTYPE ExtractToBuffer(
        BYTE*           pBuffer,
        DWORD           dwBufferSize,
        IBKProgress*    piCallback = NULL) = 0;
};

NS_SKYLARK_END


#define FN_BKBakCreateObject    "BKBakCreateObject"     ///< export BKBakCreateObject


/**
* @brief    创建BKBak的模块
* @param    dwBKEngVer      BKEng的版本号,默认使用BKENG_CURRENT_VERSION,传入0表示不作版本检查
* @param    riid            要创建的对象的iid
* @param    ppvObj          返回创建的对象
*/
EXTERN_C HRESULT WINAPI BKBakCreateObject(REFIID riid, void** ppvObj, DWORD dwBKEngVer = BKENG_CURRENT_VERSION);
/// typedef for BKBakCreateObject
typedef HRESULT (WINAPI *PFN_BKBakCreateObject)(REFIID riid, void** ppvObj, DWORD dwBKEngVer);




#endif//BKBAK_H