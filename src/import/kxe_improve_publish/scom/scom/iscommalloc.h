/********************************************************************
* CreatedOn: 2005-10-17   16:07
* FileName: ISCOMMalloc.h
* CreatedBy: qiuruifeng <qiuruifeng@kingsoft.net>
* $LastChangedDate$
* $LastChangedRevision$
* $LastChangedBy$
* $HeadURL$
* Purpose:
*********************************************************************/
#ifndef ISCOMMALLOC_H
#define ISCOMMALLOC_H

//#include "KSIUnknown.h"
#include <Unknwn.h>

// {3DB65D82-CF40-4aed-B36B-AB1C461D3918}
// #define KS_ISCOMMALLOC_IID \
// { 0x3db65d82, 0xcf40, 0x4aed, \
// { 0xb3, 0x6b, 0xab, 0x1c, 0x46, 0x1d, 0x39, 0x18 } }

MIDL_INTERFACE("3DB65D82-CF40-4aed-B36B-AB1C461D3918")
ISCOMMalloc : public IUnknown
{
public:
	//KS_DEFINE_GETIID(KS_ISCOMMALLOC_IID)
	virtual void* __stdcall Alloc(size_t uSize)          = 0;
	virtual void  __stdcall Free(void *pv,  size_t uSize)= 0;
	virtual void  __stdcall HeapMinimize(void)           = 0;
};

#endif	//ISCOMMALLOC_H