#pragma once

#include "../md5/cpp/md5.h"
#include "kpfw/RC4.h"
#include <string>
#include <windows.h>
#include <shlwapi.h>

#define VALUE_MD5LEN          16

#pragma comment(lib, "shlwapi.lib")

class CPublicEncrypt
{
public:
    CPublicEncrypt(const std::wstring& wstrFileName)
    {
        m_wstrFileName = wstrFileName;
        m_pbyDecryptBuf = NULL;
        m_lDecryptBufSize = 0;
    }

    virtual ~CPublicEncrypt()
    {
        if (m_pbyDecryptBuf)
        {
            delete[] m_pbyDecryptBuf;
            m_pbyDecryptBuf = NULL;
            m_lDecryptBufSize = 0;
        }

    }

    BOOL Encrypt()
    {
        MD5 md5;
        RC4_KEY rc4_key;
        BYTE    byMD5[VALUE_MD5LEN] = {0};
        FILE    *fp = NULL;
        BOOL    bRetVal = FALSE;
        PBYTE   pbyEncryptBuf = NULL;
        long    lEncryptBufSize = 0;

        if (m_wstrFileName.empty())
        {
            return FALSE;
        }

        if (!GetEncryptBufInfo(pbyEncryptBuf, lEncryptBufSize))
        {
            DeleteFileW(m_wstrFileName.c_str());
            return FALSE;
        }

        md5.update((PVOID)pbyEncryptBuf, lEncryptBufSize);

        memcpy_s(byMD5, VALUE_MD5LEN, md5.digest(), VALUE_MD5LEN);

        prepare_key(byMD5, VALUE_MD5LEN, &rc4_key);

        rc4(pbyEncryptBuf, lEncryptBufSize, &rc4_key);

        if (_wfopen_s(&fp, m_wstrFileName.c_str(), L"wb+"))
        {
            goto exit0;
        }

        if (NULL == fp)
        {
            goto exit0;
        }

        if (fwrite(pbyEncryptBuf, lEncryptBufSize, 1, fp) != 1)
        {
            goto exit0;
        }

        if (fwrite(byMD5, VALUE_MD5LEN, 1, fp) != 1)
        {
            goto exit0;
        }

        fflush(fp);

        bRetVal = TRUE;
exit0:
        if (fp)
        {
            fclose(fp);
            fp = NULL;
        }

        if (pbyEncryptBuf)
        {
            delete[] pbyEncryptBuf;
            pbyEncryptBuf = NULL;
        }
        return bRetVal;
    }


    BOOL Decrypt()
    {
        if (VerifyFile())
        {
            return ParseDecryptData(m_pbyDecryptBuf, m_lDecryptBufSize);
        }
        else
        {
            return FALSE;
        }

    }

private:

    virtual BOOL ParseDecryptData(PBYTE pbyDecryptBuf, long lDecryptBufSize) = 0;
    virtual BOOL GetEncryptBufInfo(PBYTE& pbyEncryptBuf, long& lEncryptBufSize) = 0;

private:
    BOOL VerifyFile()
    {
        FILE*   fp = NULL;
        long    lSize = 0;
        BOOL    bRetVal = FALSE;
        BYTE    byBackupMD5[VALUE_MD5LEN] = {0};
        MD5     md5;
        RC4_KEY rc4_key;

        if (m_wstrFileName.empty())
        {
            return FALSE;
        }

        if (!PathFileExistsW(m_wstrFileName.c_str()))
        {
            return FALSE;
        }
        
        if (_wfopen_s(&fp, m_wstrFileName.c_str(), L"rb"))
        {
            return FALSE;
        }

        if (NULL == fp)
        {
            return FALSE;
        }

        fseek(fp, 0, SEEK_END);

        lSize = ftell(fp);

        if (lSize <= VALUE_MD5LEN)
        {
            goto exit0;
        }

        m_pbyDecryptBuf = new BYTE[lSize - VALUE_MD5LEN];

        if (NULL == m_pbyDecryptBuf)
        {
            goto exit0;
        }

        m_lDecryptBufSize = lSize - VALUE_MD5LEN;

        ZeroMemory(m_pbyDecryptBuf, m_lDecryptBufSize);

        rewind(fp);

        if (fread_s(m_pbyDecryptBuf, m_lDecryptBufSize, m_lDecryptBufSize, 1, fp) != 1)
        {
            goto exit0;
        }

        if (fread_s(byBackupMD5, VALUE_MD5LEN, VALUE_MD5LEN, 1, fp) != 1)
        {
            goto exit0;
        }
        
        prepare_key(byBackupMD5, VALUE_MD5LEN, &rc4_key);

        rc4(m_pbyDecryptBuf, m_lDecryptBufSize, &rc4_key);

        md5.update((PVOID)m_pbyDecryptBuf, m_lDecryptBufSize);

        if  (memcmp(byBackupMD5, md5.digest(), VALUE_MD5LEN) == 0)
        {
            bRetVal = TRUE;
        }

exit0:
        if (fp)
        {
            fclose(fp);
        }
        return bRetVal;
    }

private:
    std::wstring m_wstrFileName;
    PBYTE   m_pbyDecryptBuf;
    long    m_lDecryptBufSize;
};