/**
* @file    zipdef.h
* @brief   ...
* @author  zhangrui
* @date    2009-06-02  18:16
*/

#ifndef ZIPDEF_H
#define ZIPDEF_H

class IZipInfoCallback
{
public:
    virtual void STDMETHODCALLTYPE OnProgress(DWORD dwCompletedBytes, DWORD dwTotalBytes) = 0;
};

#endif//ZIPDEF_H