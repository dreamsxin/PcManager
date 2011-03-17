#pragma once

#include <libheader/libupdhelper.h>
#include <com_s/com/comobject.h>
#include <libheader/libheader.h>
#include "UpdateLibDefine.h"

template <class T>
class CDataUpdate : public IUpdateHelper
{
public:
	CDataUpdate(){}
	~CDataUpdate(void){}

	SCOM_BEGIN_COM_MAP(CDataUpdate)
		SCOM_INTERFACE_ENTRY(IUpdateHelper)
	SCOM_END_COM_MAP()

	BOOL Update(LPCTSTR lpUpdateFile)
	{
		CMarkup				xmlDocNew;
		BOOL				bRes = FALSE;
		BkDatLibContent2	ct2;

		::SetThreadLocale(0x804);
		bRes = CDataFileLoader().GetLibDatContent2(lpUpdateFile,ct2);
		if ( !bRes )
			goto _error_exit;

		bRes = xmlDocNew.SetDoc(ct2.strXML);
		if ( !bRes )
			goto _error_exit;

		if ( xmlDocNew.FindElem( XML_PLUG_UPDATE_ROOT) )
		{
			xmlDocNew.IntoElem();

			while ( xmlDocNew.FindElem( XML_PLUG_UPDATE_FILE) )
			{
				CSimpleArray<DWORD>		idDelList;
				CSimpleArray<CString>	strAddList;

				xmlDocNew.IntoElem();

				while ( xmlDocNew.FindElem(XML_PLUG_UPDATE_DEL) )
				{
					DWORD	nId = _ttoi( xmlDocNew.GetAttrib(XML_PLUG_UPDATE_DEL_ID_ATTRIB) );
					AddIdtoList(idDelList, nId);
				}

				if ( xmlDocNew.FindElem(XML_PLUG_UPDATE_ADD) )
				{
					xmlDocNew.IntoElem();
					while(xmlDocNew.FindElem(XML_PLUG_UPDATE_ITEM))
					{
						CString	strAdd = xmlDocNew.GetSubDoc();
						if ( !strAdd.IsEmpty() )
							strAddList.Add(strAdd);
					}
					xmlDocNew.OutOfElem();
				}
		
				bRes = T::UpdateXML(ct2.content.header,idDelList,strAddList);
			}
		}

_error_exit:
		return bRes;
	}

public:
	virtual HRESULT STDMETHODCALLTYPE Combine( LPCWSTR lpwszDifflib )
	{
		if ( Update(lpwszDifflib) )
			return S_OK;
		else
			return S_FALSE;
	}

	virtual HRESULT STDMETHODCALLTYPE Notify( LPCWSTR lpwszFileName )
	{
		return S_FALSE;
	}
	
protected:
	VOID AddIdtoList(CSimpleArray<DWORD> & idList, DWORD nId)
	{
		for ( INT i = 0; i < idList.GetSize(); i++)
		{
			if ( idList[i] == nId )
				return;
		}
		idList.Add(nId);
	}
};
