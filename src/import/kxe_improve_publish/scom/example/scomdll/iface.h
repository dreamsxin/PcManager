/********************************************************************
* CreatedOn: 2005-8-24   14:27
* FileName: IFACE.h
* CreatedBy: qiuruifeng <qiuruifeng@kingsoft.net>
* $LastChangedDate$
* $LastChangedRevision$
* $LastChangedBy$
* $HeadURL$
* Purpose:
*********************************************************************/
#ifndef IFACE_H
#define IFACE_H

#include <Unknwn.h>

MIDL_INTERFACE("30FB8724-EA7A-48f4-979F-1009583593CF")
IHi: public IUnknown
{
	//KS_DEFINE_GETIID(KS_IHI_IID);
	virtual bool __stdcall Hi() = 0;
};

MIDL_INTERFACE("A17695C5-3EA0-4f43-8425-02BEC0D7DA89")
IOK : public IUnknown
{
	//KS_DEFINE_GETIID(KS_IOK_IID);
	virtual bool __stdcall OK() = 0;
};

MIDL_INTERFACE("23031722-F263-4074-80FB-5F766AEC49C1")
IGood : public IOK
{
	//KS_DEFINE_GETIID(KS_IGOOD_IID);
	virtual bool __stdcall Good() = 0;
};

MIDL_INTERFACE("23031722-F263-4074-80FB-5F766AEC49C1")
IWell : public IOK
{
	//KS_DEFINE_GETIID(KS_IWELL_IID);
	virtual bool __stdcall Well() = 0;
};

#endif //IFACE_H

