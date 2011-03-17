/**
* @file    slzipinfo.h
* @brief   ...
* @author  zhangrui
* @date    2009-06-04  11:14
*/

#ifndef SLZIPINFO_H
#define SLZIPINFO_H

#include <atlstr.h>
#include "miniutil\bkjson.h"
#include "skylarkbase.h"
#include "bkbak.h"

NS_SKYLARK_BEGIN

/*
{
   "upload" : {
      "file" : "压缩包中的文件名",
      "path" : "文件原始路径",
      "md5" : "文件的md5",
      "crc" : "文件的crc32",
      "comment" : "备注",
   }
}
*/
/// 上传信息数据对象
class CUploadZipInfo
{
public:
    /// ...
    void    Clear();

    /// ...
    HRESULT SaveToJson(BkJson::Document& jDoc) const;

    /// ...
    HRESULT LoadFromJson(BkJson::Document& jDoc);

    /// ...
    HRESULT AddToZip(IBKZip* piZip, IBKProgress* piCallback = NULL) const;

    /// ...
    HRESULT ExtractFromZip(IBKUnzip* piUnzip, IBKProgress* piCallback = NULL);

public:
    CString m_strUploadFileName;    ///< 上传的文件名
    CString m_strUploadFilePath;    ///< 上传的文件原始路径
    CString m_strComment;           ///< 备注

    BYTE    m_pbyMd5[SKYLARK_MD5_BYTES];  ///< 文件的md5
    DWORD   m_dwCRC32;                    ///< 文件的crc32
};

NS_SKYLARK_END

#endif//SLZIPINFO_H