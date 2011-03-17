#pragma once
#include "comdata_sc_def.h"

class   IBkServiceCall
{
public:
    virtual HRESULT Execute(const CString& strExePath, const CString& strCmdline, BOOL bShow ) = 0;
};