/**
* @file    slcoll.h
* @brief   ...
* @author  zhangrui
* @date    2009-06-09  15:44
*/

#ifndef SLCOLL_H
#define SLCOLL_H

#include <atlcoll.h>
#include "skylarkbase.h"

NS_SKYLARK_BEGIN

/// 可复制的AtlArray
template <class E>
class CCopiableArray: public CAtlArray<E>
{
public:
    /// ...
    CCopiableArray() {};

    /// ...
    CCopiableArray(const CCopiableArray& rhs)
    {
        *this = rhs;
    }

    /// ...
    CCopiableArray& operator=(const CCopiableArray& rhs)
    {
        if (this == &rhs)
            return *this;

        Copy(rhs);  // copy from rhs

        return *this;
    }
};



/// 可复制的AtlList
template <class E>
class CCopiableList: public CAtlList<E>
{
public:
    /// ...
    CCopiableList() {};

    /// ...
    CCopiableList(const CCopiableList& rhs)
    {
        *this = rhs;
    }

    /// ...
    CCopiableList& operator=(const CCopiableList& rhs)
    {
        if (this == &rhs)
            return *this;

        POSITION pos = rhs.GetHeadPosition();
        for (NULL; pos; rhs.GetNext(pos))
        {
            AddTail(rhs.GetAt(pos));
        }

        return *this;
    }
};



class CAutoRefCount
{
public:

    CAutoRefCount(LONG nRef = 0): m_nRef(nRef)
    {
    }

    HRESULT Lock() throw()
    {
        InterlockedIncrement(&m_nRef);
        return S_OK;
    }
    
    HRESULT Unlock() throw()
    {
        InterlockedDecrement(&m_nRef);
        return S_OK;
    }

    operator LONG() const
    {
        return m_nRef;
    }

private:
    LONG m_nRef;
};





class CNoCaseCompareTraits: public CElementTraitsBase<CString>
{
public:
    typedef const CString& INARGTYPE;
    typedef CString& OUTARGTYPE;

    static bool CompareElements( INARGTYPE element1, INARGTYPE element2 )
    {
        return 0 == element1.CompareNoCase(element2);
    }

    static int CompareElementsOrdered( INARGTYPE element1, INARGTYPE element2 )
    {
        return element1.CompareNoCase(element2);
    }
};



NS_SKYLARK_END

#endif//SLCOLL_H