#pragma once
#include "uplive.h"
#include "winmod/winmodule.h"
#include "winmod/winpath.h"

template< typename I >
class CProxy : public WinMod::CWinModule
{
public:
    CProxy() :
      m_pfnGetInterface( NULL ),
          m_pi( NULL )
      {

      }

      virtual ~CProxy()
      {
   //       ATLASSERT(NULL == m_pi);

          if (m_pi)
          {
              Uninitialize();
          }
      }

      HRESULT Initialize()
      {
          HRESULT hr = S_OK;

          WinMod::CWinPath    PathLoadDllFile;
          PathLoadDllFile.GetModuleFileName( NULL );
          PathLoadDllFile.RemoveFileSpec();
          PathLoadDllFile.Append( L"ksafeup.dll" );

          hr = WinMod::CWinModule::LoadLib( PathLoadDllFile.m_strPath );
          if ( FAILED( hr ) )
          {
              goto Exit0;
          }

          m_pfnGetInterface = ( PFN_CreateObject )WinMod::CWinModule::GetProcAddr( 
              FN_CreateObject 
              );
          if ( NULL == m_pfnGetInterface )
          {
              hr = HRESULT_FROM_WIN32( GetLastError() );
              goto Exit0;
          }

          hr = m_pfnGetInterface( __uuidof( I ), ( LPVOID* )&m_pi );
          if ( FAILED( hr ) )
          {
              goto Exit0;
          }
Exit0:
          if ( FAILED( hr ) )
          {
              if ( m_pi )
              {
                  m_pi->Release();
                  m_pi = NULL;
              }
              WinMod::CWinModule::FreeLib();
          }
          return hr;
      }

      HRESULT Uninitialize()
      {
          if ( m_pi )
          {
              m_pi->Release();
              m_pi = NULL;
          }
          WinMod::CWinModule::FreeLib();
          return S_OK;
      }

      I*  GetInterface() { return m_pi; }
protected:
    PFN_CreateObject m_pfnGetInterface;
private:
    I*  m_pi;
};


//模板化此接口实现类方便继承新接口是使用  [12/23/2010 zhangbaoliang]
template< typename T >
class CUpliveProxyT : public CProxy<T>
{
public:
    CUpliveProxyT() :
      m_piUplive( NULL )
    {

    }

    HRESULT Initialize()
    {
        HRESULT hr = S_OK;

        hr = CProxy::Initialize();
        if ( FAILED( hr ) )
        {
            goto Exit0;
        }

        m_piUplive = CProxy::GetInterface();
        if ( NULL == m_piUplive )
        {
            hr = E_NOINTERFACE;
            goto Exit0;
        }

        hr = m_piUplive->Initialize();
Exit0:
        if ( FAILED( hr ) )
        {
            CProxy::Uninitialize();
        }

        return hr;
    }

    HRESULT Uninitialize()
    {
        if ( m_piUplive )
        {
            m_piUplive->Uninitialize();
            CProxy::Uninitialize();
            m_piUplive = NULL;
        }
        return S_OK;
    }

    void    SetCallback( 
        IStdUpdateCallback* piStd, 
        IUpdatePolicyCallback* piUpdPolicy 
        )
    {
        if ( m_piUplive )
        {
            m_piUplive->SetCallback( piStd, piUpdPolicy );
        }
    }

    HRESULT Start()
    {
        if ( NULL == m_piUplive )
        {
            return E_NOINTERFACE;
        }

        return m_piUplive->Start();
    }

    void  Stop()
    {
        if ( m_piUplive )
        {
            m_piUplive->Stop();
        }
    }

    void  WaitExit( DWORD dwWaitTime = INFINITE )
    {
        if ( m_piUplive )
        {
            m_piUplive->WaitExit( dwWaitTime );
        }
    }

    HRESULT NotifyUpdateMessage()
    {
        if ( NULL == m_piUplive )
        {
            return E_NOINTERFACE;
        }

        return m_piUplive->NotifyUpdateMessage();
    }

    HRESULT NotifyUpdate( int nType )
    {
        if ( NULL == m_piUplive )
        {
            return E_NOINTERFACE;
        }

        return m_piUplive->NotifyUpdate( nType );
    }

	HRESULT ParseMsgIniModule(LPCTSTR pszMsgIniFilePath)
	{
		if ( NULL == m_piUplive )
		{
			return E_NOINTERFACE;
		}

		return m_piUplive->ParseMsgIniModuleUpdate(pszMsgIniFilePath);

	}

public:
    //IUplive*        m_piUplive;
	T*        m_piUplive;
};

//IUplive  [12/23/2010 zhangbaoliang]
class CUpliveProxy : public CUpliveProxyT<IUplive>
{
public:

};

//IUplive2 [12/23/2010 zhangbaoliang]
class CUpliveProxy2 : public CUpliveProxyT<IUplive2>
{
public:
	HRESULT ForceUpdateSpecial(LPCTSTR pszUpdateUrl, LPCTSTR pszDownloadFileName, DWORD dwUpdateType)
	{
		if ( NULL == m_piUplive )
		{
			return E_NOINTERFACE;
		}

		return m_piUplive->ForceUpdateSpecialFile(pszUpdateUrl, pszDownloadFileName, dwUpdateType);
		//return E_NOINTERFACE;
	}
};

class CUpdUtilityProxy : public CProxy<IUpdUtility>
{
public:
    CUpdUtilityProxy() :
      m_piUpdUtility( NULL )
    {

    }

    HRESULT Initialize()
    {
        HRESULT hr = S_OK;

        hr = CProxy::Initialize();
        if ( FAILED( hr ) )
        {
            goto Exit0;
        }

        m_piUpdUtility = CProxy::GetInterface();
        if ( NULL == m_piUpdUtility )
        {
            hr = E_NOINTERFACE;
            goto Exit0;
        }

Exit0:
        if ( FAILED( hr ) )
        {
            CProxy::Uninitialize();
        }

        return hr;
    }

    HRESULT Uninitialize()
    {
        if ( m_piUpdUtility )
        {
            CProxy::Uninitialize();
            m_piUpdUtility = NULL;
        }
        return S_OK;
    }

    HRESULT DownloadFile(
        LPCWSTR lpwszUrl,
        LPCWSTR lpwszTargetPath, 
        BOOL bPack, 
        LPCWSTR lpwszHashCheck ,/* = NULL */ 
        IUpdateDownloadFileCallback* piCallback = NULL
        )
    {
        if ( NULL == m_piUpdUtility )
        {
            return E_NOINTERFACE;
        }

        return  m_piUpdUtility->DownloadFile(
            lpwszUrl,
            lpwszTargetPath,
            bPack,
            lpwszHashCheck,
            piCallback
            );
    }

    virtual BOOL    DoExpression( LPCWSTR lpwszExpression )
    {
        if ( NULL == m_piUpdUtility )
        {
            return E_NOINTERFACE;
        }

        return  m_piUpdUtility->DoExpression( lpwszExpression );
    }
private:
    IUpdUtility*        m_piUpdUtility;
};