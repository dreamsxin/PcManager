/********************************************************************
* CreatedOn: 2005-10-17   16:35
* FileName: SCOMMallocObj.h
* CreatedBy: qiuruifeng <qiuruifeng@kingsoft.net>
* $LastChangedDate$
* $LastChangedRevision$
* $LastChangedBy$
* $HeadURL$
* Purpose:重载了new和delete的类
*********************************************************************/

#ifndef SCOMMALLOCOBJ_H
#define SCOMMALLOCOBJ_H
#include "SCOMAPI.h"

class SCOMMallocObj
{
public:
	static void* operator new(unsigned int uSize)
	{
		return KSCoTaskMemAlloc(uSize);
	}

	static void operator delete(void* p, unsigned int uSize)
	{
		KSCoTaskMemFree(p, uSize);
	}
	static void* operator new[](unsigned int uSize)
	{
		return KSCoTaskMemAlloc(uSize);
	}
	static void operator delete[](void* p)
	{
		KSCoTaskMemFree(p, 0);
	}
};

#endif //SCOMMALLOCOBJ_H