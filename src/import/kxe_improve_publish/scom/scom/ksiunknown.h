/********************************************************************
* CreatedOn: 2005-8-16   15:40
* FileName: KSIUnknown.h
* CreatedBy: qiuruifeng <qiuruifeng@kingsoft.net>
* $LastChangedDate$
* $LastChangedRevision$
* $LastChangedBy$
* $HeadURL$
* Purpose:
*********************************************************************/

#ifndef KSIUNKNOWN_H
#define KSIUNKNOWN_H
#include "SCOMDef.h"

#define KS_IUNKNOWN_IID        \
{ 0x00000000, 0x0000, 0x0000, \
{0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46} }

#define KS_IUNKNOWN_IID_STR "00000000-0000-0000-c000-000000000046"

class KSIUnknown
{
public:
	KS_DEFINE_GETIID(KS_IUNKNOWN_IID)

	virtual KSRESULT __stdcall QueryInterface(const KSIID &riid, void **ppv) = 0;

	virtual unsigned long __stdcall AddRef(void) = 0;

	virtual unsigned long __stdcall Release(void) = 0;
};
#endif//KSIUNKNOWN_H