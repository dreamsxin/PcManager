///////////////////////////////////////////////////////////////
//
//	Filename: 	KDllComObject.h
//	Creator:	lichenglin  <lichenglin@kingsoft.net>
//	Date:		2008-2-21  9:01
//	Comment:	
//
///////////////////////////////////////////////////////////////

#ifndef _KDllComObject_h_
#define _KDllComObject_h_
#include "comobject.h"
#include "dlllock.h"



template <class T>
class KDllComObject : public KComObject<T>
{
public:
	KDllComObject()
	{
		KDllLock::GetInstance()->Lock();
	}

	~KDllComObject()
	{
		KDllLock::GetInstance()->UnLock();
	}
};


#endif // _KDllComObject_h_