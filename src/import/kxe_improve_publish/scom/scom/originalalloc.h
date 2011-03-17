/********************************************************************
* CreatedOn: 2005-9-26   9:52
* FileName: OriginalAlloc.h
* CreatedBy: qiuruifeng <qiuruifeng@kingsoft.net>
* $LastChangedDate$
* $LastChangedRevision$
* $LastChangedBy$
* $HeadURL$
* Purpose:
*********************************************************************/

#ifndef ORIGINALALLOC_H
#define ORIGINALALLOC_H
#include <new>
#include <malloc.h>
class __new_alloc
{
public:
	static void*
    allocate(size_t __n)
	{ 
		return ::operator new(__n); 
	}
    static void
    deallocate(void* __p, size_t)
    { 
		::operator delete(__p); 
	}
	static void
	heap_minimize(void)
	{
	}
};
class __malloc_alloc
{
public:
	static void*
    allocate(size_t __n)
	{ 
		void *result = malloc(__n);
		if (0 == result )       
			throw std::bad_alloc();
		return result;        
    }
	static void
    deallocate(void* __p, size_t)
	{ 
		free(__p);
	}
	static void
	heap_minimize(void)
	{
	}
};
#endif //ORIGINALALLOC_H