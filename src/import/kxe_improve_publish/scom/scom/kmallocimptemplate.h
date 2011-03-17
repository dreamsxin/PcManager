/********************************************************************
* CreatedOn: 2005-10-17   17:39
* FileName: KMallocImpTemplate.h
* CreatedBy: qiuruifeng <qiuruifeng@kingsoft.net>
* $LastChangedDate$
* $LastChangedRevision$
* $LastChangedBy$
* $HeadURL$
* Purpose:
*********************************************************************/

#ifndef KMALLOCIMPTEMPLATE_H
#define KMALLOCIMPTEMPLATE_H

#include "ISCOMMalloc.h"
#include "SCOMBase.h"

//内存管理组件的模板

#if _MSC_VER >= 1400
template<typename _InnerAlloc, const KSCLSID& classID>
class KMallocImpTemplate:
	public KSComRoot< KMallocImpTemplate<_InnerAlloc, classID> >,
	public ISCOMMalloc
{
public:
	void* __stdcall Alloc(size_t uSize)
	{
		return _InnerAlloc::allocate(uSize);
	}
	void __stdcall Free(void *pv,  size_t uSize)
	{
		_InnerAlloc::deallocate(pv, uSize);
	}
	void __stdcall HeapMinimize(void)
	{
		_InnerAlloc::heap_minimize();
	}

	KS_DEFINE_GETCLSID(classID);
	//DECLARE_INDEPENDENT_OBJECT();
	DECLARE_SINGLETON_OBJECT();
	KSCOM_QUERY_BEGIN;
	KSCOM_QUERY_ENTRY(ISCOMMalloc);
	KSCOM_QUERY_END;
};
#else
	template<typename _InnerAlloc>
	class KMallocImpTemplate:
		public KSComRoot< KMallocImpTemplate<_InnerAlloc> >,
		public ISCOMMalloc
	{
	public:
		void* __stdcall Alloc(size_t uSize)
		{
			return _InnerAlloc::allocate(uSize);
		}
		void __stdcall Free(void *pv,  size_t uSize)
		{
			_InnerAlloc::deallocate(pv, uSize);
		}
		void __stdcall HeapMinimize(void)
		{
			_InnerAlloc::heap_minimize();
		}
	
// 		KS_DEFINE_GETCLSID(classID);
// 		//DECLARE_INDEPENDENT_OBJECT();
// 		DECLARE_SINGLETON_OBJECT();
// 		KSCOM_QUERY_BEGIN;
// 		KSCOM_QUERY_ENTRY(ISCOMMalloc);
// 		KSCOM_QUERY_END;
	};

#endif

#endif //KMALLOCIMPTEMPLATE_H