/********************************************************************
* CreatedOn: 2005-9-5   18:03
* FileName: SCOMUtility.h
* CreatedBy: qiuruifeng <qiuruifeng@kingsoft.net>
* $LastChangedDate$
* $LastChangedRevision$
* $LastChangedBy$
* $HeadURL$
* Purpose:
*********************************************************************/

#ifndef SCOMUTILITY_H
#define SCOMUTILITY_H

#include "SCOMAPI.h"
#include "KSIUnknown.h"

//重载->符的返回值类型,重载了AddRef和Release方法,使其成为私有,不能被外界调用
template <class T>
class _NoAddRefReleaseOnKSCOMPtr : public T
{
private:
    virtual unsigned long __stdcall AddRef(void) = 0;
    virtual unsigned long __stdcall Release(void) = 0;
};

template <class T>
class KSCOMPtr
{
public:
    typedef T _PtrClass;

    KSCOMPtr() 
    {
        m_pInterface = NULL;
    }
    KSCOMPtr(int nNull) 
    {
        KS_ASSERT(nNull == 0);
        (void)nNull;
        m_pInterface = NULL;
    }
    KSCOMPtr(T* lp) 
    {
        m_pInterface = lp;
        if (m_pInterface)
            m_pInterface->AddRef();
    }
	KSCOMPtr(const KSCOMPtr &copy)
	{
		m_pInterface = copy.m_pInterface;
		if(m_pInterface)
			m_pInterface->AddRef();
	}

	KSCOMPtr& operator = (const KSCOMPtr &copy)
	{
		if (m_pInterface != copy.m_pInterface)
		{
			Release();
			m_pInterface = copy.m_pInterface;
			if (m_pInterface)
				m_pInterface->AddRef();
		}
		return *this;
	}

    ~KSCOMPtr() 
    {
        if (m_pInterface)
            m_pInterface->Release();
    }

    //释放接口指针
    void Release() 
    {
        T* pTemp = m_pInterface;
        if (pTemp)
        {
            m_pInterface = NULL;
            pTemp->Release();
        }
    }

    operator T*() const 
    {
        return m_pInterface;
    }

    T& operator*() const 
    {
        KS_ASSERT(m_pInterface != NULL);
        return *m_pInterface;
    }

	//vs2005的stl库容器中构造对象时需要调用取址符,
	//由于类型不一致将导致智能指针无法在容器中构造,所以去掉对取址符号的重载
	//由对象的内存结构决定了智能指针的地址和内部原生指针的地址值一致
	//所以需要取内部原生指针的地址时也可以把智能指针的地址强转得到.
	//作为重载取址符号功能的安全替换,提供一个取址函数
	T** Address()
	{
	   KS_ASSERT(m_pInterface == NULL);
	   return &m_pInterface;
	}
    //使用该符号前要确保指针为空
    //T** operator&() 
    //{
    //   KS_ASSERT(m_pInterface == NULL);
    //    return &m_pInterface;
    //}

	
    //重载->符,返回值不是原始指针,强制不能够使用->AddRef()和->Release()
    //这是为了避免误用->Release()导致析构函数释两次接口指针
    _NoAddRefReleaseOnKSCOMPtr<T>* operator->() const 
    {
        KS_ASSERT(m_pInterface != NULL);
        return (_NoAddRefReleaseOnKSCOMPtr<T>*)m_pInterface;
    }

    bool operator!() const 
    {
        return (m_pInterface == NULL);
    }

    bool operator<(T* pT) const 
    {
        return (m_pInterface < pT);
    }

    bool operator==(T* pT) const 
    {
        return (m_pInterface == pT);
    }

    //把同类型的接口指针赋值到当前指针
    T* operator=(T* pT)
    {
        if (m_pInterface != pT)
        {
            Release();
            m_pInterface = pT;
            if (m_pInterface)
                m_pInterface->AddRef();
        }
        return m_pInterface;
    }

    //把其他类型的接口指针赋值到当前指针,
    //不使用 operator= 的方法是为了可以获取错误码以及
    //减少因为忘记检测赋值是否失败而产生的错误
    KSRESULT Assign(IUnknown *lp)
    {
        Release();
        if (lp != NULL)
        {
             return  lp->QueryInterface(KS_IIDOF(T), (void**)&m_pInterface);              
        }
        else
        {
           return  S_OK;
        }
    }

    //判断两个接口指针是否引用同一个对象
    bool IsEqualObject(IUnknown* pOther) 
    {
        if (m_pInterface == NULL && pOther == NULL)
            return true;

        if (m_pInterface == NULL || pOther == NULL)
            return false;

        KSCOMPtr<KSIUnknown> punk1;
        KSCOMPtr<KSIUnknown> punk2;
        m_pInterface->QueryInterface(KS_IIDOF(IUnknown), (void**)&punk1);
        pOther->QueryInterface(KS_IIDOF(IUnknown), (void**)&punk2);
        return punk1 == punk2;
    }

    //从原始接口指针获取所有权,不增加引用计数
    void Attach(T* p2) 
    {
        if (m_pInterface)
            m_pInterface->Release();
        m_pInterface = p2;
    }

    //把智能指针的所有权交回一个原始的接口指针时使用,不减少引用计数
    T* Detach() 
    {
        T* pt = m_pInterface;
        m_pInterface = NULL;
        return pt;
    }

	void Swap(KSCOMPtr& other)
	{
		 T* pt = m_pInterface;
		 m_pInterface = other.m_pInterface;
		 other.m_pInterface = pt;
	}

    //产生一份经过AddRef的接口指针的拷贝
    KSRESULT CopyTo(T** ppT) const
    {
        KS_ASSERT(ppT != NULL);
        if (ppT == NULL)
            return E_POINTER;
        *ppT = m_pInterface;
        if (m_pInterface)
            m_pInterface->AddRef();
        return S_OK;
    }

    //类型安全的QueryInterface
    template <class Q>
        KSRESULT QueryInterface(Q** pp) const 
    {
        KS_ASSERT(pp != NULL);
        return m_pInterface->QueryInterface(KS_IIDOF(Q), (void**)pp);
    }
    //类型安全的KSCoCreateInstance
    KSRESULT KSCoCreateInstance(const KSCLSID& clsid)
    {
        KS_ASSERT(m_pInterface == NULL);
        return ::KSCoCreateInstance(clsid, KS_IIDOF(T), (void**)&m_pInterface);
    }
private:
    T* m_pInterface;
};

#endif //SCOMUTILITY_H
