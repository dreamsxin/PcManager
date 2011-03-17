/********************************************************************
* CreatedOn: 2005-8-16   15:05
* FileName: SCOM.h
* CreatedBy: qiuruifeng <qiuruifeng@kingsoft.net>
* $LastChangedDate$
* $LastChangedRevision$
* $LastChangedBy$
* $HeadURL$
* Purpose:
*********************************************************************/

#ifndef SCOM_H
#define SCOM_H

#include "SCOMAPI.h"
#include "SCOMUtility.h"
#include "SCOMMallocObj.h"

//默认情况下是使用动态加载，除非明确的定义了SCOM_USE_STATIC_LOADING
#ifdef SCOM_USE_STATIC_LOADING
#pragma comment(lib, "SCOM.lib")
#else
#include "SCOMDynamicLoadFunc.h"
#endif


#endif//SCOM_H
