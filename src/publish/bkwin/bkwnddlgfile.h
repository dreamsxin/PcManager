#pragma once

#include "bkwnddlg.h"

/*
使用另外一个资源XML来加载子元素

<dlgfile src=16 />
*/
class CBkDialogFile
    : public CBkDialog
{
    BKOBJ_DECLARE_CLASS_NAME(CBkDialogFile, "dlgfile")

public:

    virtual BOOL Load(TiXmlElement* pTiXmlElem)
    {
		if (!CBkWindow::Load(pTiXmlElem))
			return FALSE;

		CStringA	strSrc = pTiXmlElem->Attribute("src");

		ATLASSERT( !strSrc.IsEmpty() );

		if (strSrc.IsEmpty())
			return FALSE;

		CStringA	strXML;
		UINT		uResId = atoi(strSrc);
		BOOL		bRet   = BkResManager::LoadResource(uResId,strXML);
		
		ATLASSERT(bRet);

		if (!bRet)
			return TRUE;

		TiXmlDocument xmlDoc;
		{
			xmlDoc.Parse(strXML, NULL, TIXML_ENCODING_UTF8);
		}
		if (xmlDoc.Error())
		{
			ATLASSERT(NULL);
			return FALSE;
		}

		TiXmlElement *pSubTiElement = xmlDoc.RootElement();
        bRet = __super::LoadChilds(pSubTiElement);

        return bRet;
    }

protected:

};