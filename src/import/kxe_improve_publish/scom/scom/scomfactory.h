/********************************************************************
* CreatedOn: 2005-8-24   16:45
* FileName: SCOMFactory.h
* CreatedBy: qiuruifeng <qiuruifeng@kingsoft.net>
* $LastChangedDate$
* $LastChangedRevision$
* $LastChangedBy$
* $HeadURL$
* Purpose:
*********************************************************************/

#ifndef SCOMFACTORY_H
#define SCOMFACTORY_H
#include <climits>
#include <map>
#include "SCOMDef.h"
#include "Singleton.h"

#ifdef _DEBUG
#include <sstream>
#include <algorithm>
#include "scomhelper.h"
#endif

//////////////////////////////////////////////////////////////////////////
//类厂
class KFactory;

typedef SingletonHolder_Static<KFactory> GlobalFactory;
//typedef KFactory GlobalFactory;
class KFactory
{
#if _MSC_VER >= 1400
	
template<class T>
        friend class SingletonHolder_Static;
#else
		friend class SingletonHolder_Static<KFactory>;
#endif

public:
    typedef std::map< KSCLSID, KMODULEINFO > KMODULEINFOMAP;
#ifdef _DEBUG
	typedef std::basic_ostringstream<SCOM_ASSERT_CHAR, std::char_traits<SCOM_ASSERT_CHAR>,
		std::allocator<SCOM_ASSERT_CHAR> > AssertMsgStringStream;
#endif
    KSRESULT Register(KMODULEINFO Info)
    {
		std::pair<KMODULEINFOMAP::iterator, bool> RetPair = 
			m_ComponentMap.insert(
			KMODULEINFOMAP::value_type(Info.m_CLSID, Info)
			);
        //m_ComponentMap[Info.m_CLSID] = Info;
		if(RetPair.second)
		{
			return S_OK; 
		}
		else
		{
#ifdef _DEBUG
			AssertMsgStringStream os;
			os << SCOM_ASSERT_TEXT("GUID Conflict:") << Info.m_CLSID;
			SCOM_ASSERT_MESSAGE(os.str().c_str(), RetPair.second);
#endif
			return S_FALSE;
		}
    }

    KSRESULT CreateInstance(const KSCLSID &clsid, const KSIID &riid, void **ppv)
    {
		//try
		//{
		KMODULEINFOMAP::iterator iter = m_ComponentMap.find(clsid);
		if (iter != m_ComponentMap.end())
		{
			KSRESULT hRes = (iter->second).m_pfnCreateInstance(riid, ppv);
			return hRes;
		}
		else
		{
			return NTE_NOT_FOUND;
		}
		//}
		//catch(...)
		//{
		//	SCOM_ASSERT_MESSAGE(SCOM_ASSERT_TEXT("CreateInstance failed.Maybe you haven't Initialized SCOM library"), false);
		//	return E_FAIL;
		//}
    }
    
    KSRESULT CanUnloadNow(void)
    {
        return (m_Lockcnt == 0) ? S_OK : S_FALSE;
    }
    KSRESULT LockServer(bool bLock)
    {
        if (bLock)
            Lock();
        else
            UnLock();

        return S_OK;
    }
    long Lock()
    {
        KS_ASSERT(long(m_Lockcnt) >= 0);
        return ++m_Lockcnt;
    }
    long UnLock()
    {
        KS_ASSERT(long(m_Lockcnt) > 0);
        return --m_Lockcnt;
    }    
    long GetLockCount(void)
    {
        return m_Lockcnt;
    }
    
    unsigned long GetClassCount(void)
    {
        return static_cast<unsigned long>(m_ComponentMap.size());
    }

    KSRESULT GetClassInfo(CLASSINFO *pInClassInfo, int nInSize)
    {
        if (NULL == pInClassInfo)
        {
            return E_FAIL;
        }
        KMODULEINFOMAP::iterator iter;
        int i = 0;
        for (iter = m_ComponentMap.begin(); 
            iter != m_ComponentMap.end();
            ++iter, ++i)
        {
            if (i > nInSize - 1)
            {
                break;
            }
            pInClassInfo[i].m_CLSID = iter->second.m_CLSID;
            pInClassInfo[i].m_pszProgID = iter->second.m_pszProgID;
			pInClassInfo[i].m_dwProperty = iter->second.m_dwProperty;
        }
        return S_OK;
    }

#ifdef _DEBUG
	typedef std::map<KSCLSID, long> ComponentCountMap;
	class ReadComponentLeakInfoFunctor
	{
	public:
		ReadComponentLeakInfoFunctor(AssertMsgStringStream& os):m_os(os)
		{
		}
		ReadComponentLeakInfoFunctor & operator=( const ReadComponentLeakInfoFunctor & ) {}
		void operator()(const ComponentCountMap::value_type& value)
		{
			m_os << value.first << SCOM_ASSERT_TEXT(":") << value.second << SCOM_ASSERT_TEXT(" ,");
		}
	private:
		AssertMsgStringStream& m_os;
	};

	long AddComponentCount(const KSCLSID clisd)
	{
		long count = 1;
		std::pair<ComponentCountMap::iterator, bool> pairInsert;
		KSAutoLockGuard guard(m_ComponentCountMutex);
		pairInsert = m_ComponentCountMap.insert(std::map<KSCLSID, long>::value_type(clisd, 1));
		if (!pairInsert.second)
		{
			count = ++pairInsert.first->second;
		}
		return count;
	}

	long ReleaseComponentCount(const KSCLSID clisd)
	{
		long count = LONG_MIN;
		std::map<KSCLSID, long>::iterator iter;
		KSAutoLockGuard guard(m_ComponentCountMutex);
		iter = m_ComponentCountMap.find(clisd);
		if (iter != m_ComponentCountMap.end())
		{
			count = --iter->second;
			if (count == 0)
			{
				m_ComponentCountMap.erase(iter);
			}
		}
		return count;
	}
private:
	std::map<KSCLSID, long> m_ComponentCountMap;
	KSAutoLockGuard::ThreadMutex m_ComponentCountMutex;
#endif

private:
    KMODULEINFOMAP m_ComponentMap;
    KSRefCnt m_Lockcnt;

private:
    KFactory():m_Lockcnt(0L)
    {
    };
    KFactory(const KFactory&){};
    KFactory& operator = (const KFactory&){};
    ~KFactory()
	{
#ifdef _DEBUG
		if (m_ComponentCountMap.size())
		{
			AssertMsgStringStream os;
			os << SCOM_ASSERT_TEXT("Component leak infomation:");
			ReadComponentLeakInfoFunctor reader(os);
			std::for_each(m_ComponentCountMap.begin(), m_ComponentCountMap.end(), reader);
			SCOM_ASSERT_MESSAGE(os.str().c_str(), false);
		}
#endif
	}

};

//注册组件信息的类
class RegisterDummy
{
public:
    RegisterDummy(const KSCLSID& clsid, _SCOM_CREATORFUNC *pfnCreateInstance, const char *pszProgID, DWORD dwProperty)
    {    
        m_MoudleInfo.m_CLSID = clsid;
        m_MoudleInfo.m_pfnCreateInstance = pfnCreateInstance;
        m_MoudleInfo.m_pszProgID = pszProgID;
		m_MoudleInfo.m_dwProperty = dwProperty;
        GlobalFactory::Instance().Register(m_MoudleInfo);
    }
private:
    KMODULEINFO m_MoudleInfo;
};

//产生静态对象的宏
#define REGISTER_TO_FACTORY(_class, _ProgID) \
static RegisterDummy dummy##_class(_class::GetCLSID(), _class::CreateInstance, _ProgID, 0);

//产生静态对象的宏
#define REGISTER_TO_FACTORY_EX(_class, _ProgID, _Property) \
	static RegisterDummy dummy##_class(_class::GetCLSID(), _class::CreateInstance, _ProgID, _Property);

#define IMPLEMENT_KSDLLGETCLASSOBJECT() \
extern "C" \
KSRESULT __stdcall  KSDllGetClassObject(const KSCLSID &clsid, const KSIID &riid , void **ppv) \
{\
    return GlobalFactory::Instance().CreateInstance(clsid,riid, ppv); \
}

#define IMPLEMENT_KSDLLGETCLASSCOUNT()  \
extern "C" \
KSRESULT __stdcall  KSDllGetClassCount(unsigned long &ulReturnSize)\
{\
    ulReturnSize =  GlobalFactory::Instance().GetClassCount();\
    return S_OK;\
}

#define IMPLEMENT_KSDLLGETCLASSINFO() \
extern "C" \
KSRESULT __stdcall  KSDllGetClassInfo(CLASSINFO *ClassInfo, int nInSize) \
{\
    if (NULL != ClassInfo)   \
    {                        \
        return GlobalFactory::Instance().GetClassInfo(ClassInfo, nInSize);\
    }               \
    return S_FALSE; \
}

#define IMPLEMENT_KSDLLCANUNLOADNOW() \
extern "C" \
KSRESULT __stdcall  KSDllCanUnloadNow(void) \
{\
    return GlobalFactory::Instance().CanUnloadNow();\
}

#define DECLARE_DLLEXPORTS() \
    IMPLEMENT_KSDLLGETCLASSOBJECT()  \
    IMPLEMENT_KSDLLGETCLASSCOUNT()   \
    IMPLEMENT_KSDLLGETCLASSINFO()    \
    IMPLEMENT_KSDLLCANUNLOADNOW()

#endif //SCOMFACTORY_H
