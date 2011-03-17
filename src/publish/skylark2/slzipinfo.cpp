/**
* @file    slzipinfo.cpp
* @brief   ...
* @author  zhangrui
* @date    2009-06-04  11:20
*/

#include "stdafx.h"
#include "slzipinfo.h"

#include <assert.h>
#include "slhashutilex.h"
#include "filepathtruncater.h"

NS_SKYLARK_USING

#define SKYLARK_UPINFO__JV_UPLOAD                       L"upload"   ///< ...
#define SKYLARK_UPINFO__JV_UPLOAD_FILE                  L"file"     ///< ...
#define SKYLARK_UPINFO__JV_UPLOAD_PATH                  L"path"     ///< ...
#define SKYLARK_UPINFO__JV_UPLOAD_NORMALIZED_FILE_PATH  L"nmpath"   ///< ...
#define SKYLARK_UPINFO__JV_UPLOAD_MD5                   L"md5"      ///< ...
#define SKYLARK_UPINFO__JV_UPLOAD_CRC                   L"crc32"    ///< ...
#define SKYLARK_UPINFO__JV_UPLOAD_COMMENT               L"comment"  ///< ...


void CUploadZipInfo::Clear()
{
    m_strUploadFileName.Empty();
    m_strUploadFilePath.Empty();
    m_strComment.Empty();
    memset(m_pbyMd5, 0, sizeof(m_pbyMd5));
    m_dwCRC32 = 0;
}

HRESULT CUploadZipInfo::SaveToJson(BkJson::Document& jDoc) const
{
    BkJson::Handle jUpload = jDoc[SKYLARK_UPINFO__JV_UPLOAD];

    CString strMD5Hex;
    CHexUtilT<CString>::HexFromBuffer(m_pbyMd5, SKYLARK_MD5_BYTES, strMD5Hex);

    CString strCRC32Hex;
    strCRC32Hex.Format(L"%08x", m_dwCRC32);

    CString strNormalizedPath;
    HRESULT hr = CFilePathTruncater::Instance().TruncateFilePath(m_strUploadFilePath, strNormalizedPath);
    if (FAILED(hr))
        strNormalizedPath = m_strUploadFilePath;

    jUpload[SKYLARK_UPINFO__JV_UPLOAD_FILE]                 = m_strUploadFileName;
    jUpload[SKYLARK_UPINFO__JV_UPLOAD_PATH]                 = m_strUploadFilePath;
    jUpload[SKYLARK_UPINFO__JV_UPLOAD_NORMALIZED_FILE_PATH] = strNormalizedPath;
    jUpload[SKYLARK_UPINFO__JV_UPLOAD_MD5]                  = strMD5Hex;
    jUpload[SKYLARK_UPINFO__JV_UPLOAD_CRC]                  = strCRC32Hex;
    jUpload[SKYLARK_UPINFO__JV_UPLOAD_COMMENT]              = m_strComment;

    return S_OK;
}

HRESULT CUploadZipInfo::LoadFromJson(BkJson::Document& jDoc)
{
    if (!jDoc.HasKey(SKYLARK_UPINFO__JV_UPLOAD))
        return E_FAIL;

    BkJson::Handle jUpload = jDoc[SKYLARK_UPINFO__JV_UPLOAD];

    memset(m_pbyMd5, 0, sizeof(m_pbyMd5));
    m_dwCRC32 = 0;

    CString strMD5Ex = jUpload[SKYLARK_UPINFO__JV_UPLOAD_MD5];
    strMD5Ex.Append(jUpload[SKYLARK_UPINFO__JV_UPLOAD_CRC]);

    unsigned int uCRC32 = 0;
    CHexUtilExT<CString>::HexToBuffer(m_pbyMd5, &uCRC32, strMD5Ex);
    m_dwCRC32 = uCRC32;



    m_strUploadFileName = (LPCWSTR)jUpload[SKYLARK_UPINFO__JV_UPLOAD_FILE];
    m_strUploadFilePath = (LPCWSTR)jUpload[SKYLARK_UPINFO__JV_UPLOAD_PATH];  
    m_strComment        = (LPCWSTR)jUpload[SKYLARK_UPINFO__JV_UPLOAD_COMMENT];

    return S_OK;
}


HRESULT CUploadZipInfo::AddToZip(IBKZip* piZip, IBKProgress* piCallback) const
{
    assert(piZip);

    BkJson::Document jDoc;
    HRESULT hr = SaveToJson(jDoc);
    if (FAILED(hr))
        return hr;


    CStringA strJsonText = CW2A(jDoc.Dump(FALSE), CP_UTF8);
    hr = piZip->AddData(
        (const BYTE*)(LPCSTR)strJsonText,
        strJsonText.GetLength(),
        BKBAK_UPLOAD_PACK_INFO_FILE_NAME,
        piCallback);
    if (FAILED(hr))
        return hr;


    return S_OK;
}

HRESULT CUploadZipInfo::ExtractFromZip(IBKUnzip* piUnzip, IBKProgress* piCallback)
{
    assert(piUnzip);

    // 查找备份信息
    HRESULT hr = piUnzip->FindFirstFileInZip(BKBAK_UPLOAD_PACK_INFO_FILE_NAME);
    if (FAILED(hr))
        return hr;


    // 检查信息文件大小
    ULONGLONG uFileSize = 0;
    hr = piUnzip->GetFileSize(&uFileSize);
    if (FAILED(hr))
        return hr;


    if (uFileSize > BKBAK_UPLOAD_PACK_INFO_FILE_MAX_SIZE)
        return E_FAIL;


    // 分配缓冲区
    CAtlArray<BYTE> data;
    data.SetCount((size_t)uFileSize + 1);
    if (data.GetCount() < (size_t)uFileSize + 1)
        return E_FAIL;


    // 解压数据
    hr = piUnzip->ExtractToBuffer(data.GetData(), (DWORD)uFileSize, piCallback);
    if (FAILED(hr))
        return hr;


    // 解析json文档
    data[(size_t)uFileSize] = '\0';
    BkJson::Document jDoc;
    if (!jDoc.LoadString(CA2W((LPCSTR)data.GetData(), CP_UTF8)))
        return E_FAIL;


    hr = LoadFromJson(jDoc);
    if (FAILED((hr)))
        return hr;


    return S_OK;
}