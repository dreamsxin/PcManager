/**
* @file    bkdirtywork.h
* @brief   ...
* @author  bbcallen
* @date    2010-01-26 14:49
*/

#ifndef BKDIRTYWORK_H
#define BKDIRTYWORK_H

#include "skylarkbase.h"
#include "bkbak_mod.h"
#include "bkcache_mod.h"

NS_SKYLARK_BEGIN

class CBKDirtyWork
{
public:
    static HRESULT Execute();

protected:
    static HRESULT RemoveExpiredBackupFile();

    static HRESULT RemoveExpiredScanLog();

    static HRESULT RemoveExpiredCache();
};

NS_SKYLARK_END

#endif//BKDIRTYWORK_H