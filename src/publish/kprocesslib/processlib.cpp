#include "StdAfx.h"
#include <string>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

// ccf是测试同事写的一套库，这里主要使用字符串转换
#include "ccf/KStringConvert.h"
#include "ccf/KPath.h"
using namespace Kingsoft::Test::CCF::Text;
using namespace Kingsoft::Test::CCF::IO;

#define TIXML_USE_STL
#include "3rdparty/tinyxml/tinyxml.h"

#include "processlib.h"
#include "kprocessinfo.h"
#include "kprocessitem.h"
#include "kprocesslib/kprocesslibdefine.h"
#include "kprocesslib/processliboverwriterule.h"
#include "kprocesslib/interface.h"

// 数字签名
#include "winmod/wintrustverifier.h"

// 文件描述，公司名
#include "kprocesslib/kis_filever.h"
using namespace kis;

ProcessLib::ProcessLib(void)
{
    m_pXmlDoc = NULL;
    m_nMaxPathMapId = 0;
    m_nMaxCompanyNamesMapId = 0;
    m_nMaxProcessItemMapId = 0;
    m_nMaxSignNamesMapId = 0;
}

ProcessLib::~ProcessLib(void)
{
    if (m_pXmlDoc)
    {
        delete m_pXmlDoc;
        m_pXmlDoc = NULL;
    }
}

BOOL ProcessLib::Load( LPCTSTR filePath )
{
    if (NULL != m_pXmlDoc)
    {
        delete m_pXmlDoc;
        m_pXmlDoc = NULL;

        ClearAll();
    }

    m_pXmlDoc = new TiXmlDocument();

    if (NULL == m_pXmlDoc)
    {
        return FALSE;
    }

    bool result = m_pXmlDoc->LoadFile(KStringConvert::WideToAnsi(wstring(filePath)),  TIXML_DEFAULT_ENCODING );

    if (result)
    {
        return ParseXml();
    }

    return FALSE;
}

BOOL ProcessLib::Load( char* buffer, long nLen )
{
    if (NULL != m_pXmlDoc)
    {
        delete m_pXmlDoc;
        m_pXmlDoc = NULL;

        ClearAll();
    }

    m_pXmlDoc = new TiXmlDocument();

    if (NULL == m_pXmlDoc)
    {
        return FALSE;
    }

    bool result = m_pXmlDoc->LoadBuf(buffer, nLen, TIXML_ENCODING_UTF8);

    if (result)
    {
        return ParseXml();
    }

    return FALSE;
}
BOOL ProcessLib::ParseXml()
{
    TiXmlHandle hDoc(m_pXmlDoc);

    return 	ParsePathsNode() &&
            ParseCompanyNamesNode() &&
            ParseSignNamesNode() &&
            ParseMaxProcessItemIdNode() &&
            ParseProcessesNode();
}

BOOL ProcessLib::ParsePathsNode()
{
    TiXmlHandle hDoc(m_pXmlDoc);

    TiXmlElement* pElem = hDoc.FirstChild(PROCESSLIB_NODE).FirstChild(PATHS_NODE).ToElement();

    if(!pElem->FirstChild())
    {
        return FALSE;
    }

    TiXmlElement* pNode = pElem->FirstChild()->ToElement();

    if (!pNode)
    {
        return FALSE;
    }

    do
    {
        int id = 0;
        string strPath;

        pNode->Attribute(PATH_ID, &id);
        strPath = pNode->Attribute(PATH_VALUE);

        AddPathNode(id, KStringConvert::Utf8ToWide(strPath));

        m_nMaxPathMapId = max(m_nMaxPathMapId, id);

        pNode = pNode->NextSiblingElement();
    }
    while (pNode != NULL);

    return TRUE;
}

BOOL ProcessLib::ParseCompanyNamesNode()
{
    TiXmlHandle hDoc(m_pXmlDoc);

    TiXmlElement* pElem = hDoc.FirstChild(PROCESSLIB_NODE).FirstChild(COMPANY_NAMES).ToElement();

    if(!pElem->FirstChild())
    {
        return FALSE;
    }

    TiXmlElement* pNode = pElem->FirstChild()->ToElement();

    if (!pNode)
    {
        return FALSE;
    }

    do
    {
        int id = -1;
        string strName;

        pNode->Attribute(COMPANY_ID, &id);
        strName = pNode->Attribute(COMPANY_VALUE);

        AddCompanyNameNode(id, KStringConvert::Utf8ToWide(strName));

        m_nMaxCompanyNamesMapId = max(m_nMaxCompanyNamesMapId, id);

        pNode = pNode->NextSiblingElement();
    }
    while (pNode != NULL);

    return TRUE;
}

BOOL ProcessLib::ParseSignNamesNode()
{
    TiXmlHandle hDoc(m_pXmlDoc);

    TiXmlElement* pElem = hDoc.FirstChild(PROCESSLIB_NODE).FirstChild(SIGN_NAMES).ToElement();

    if(!pElem->FirstChild())
    {
        return FALSE;
    }

    TiXmlElement* pChild = pElem->FirstChild()->ToElement();

    if (!pChild)
    {
        return FALSE;
    }

    do
    {
        int id = -1;
        string strName;

        pChild->Attribute(SIGN_NAME_ID, &id);
        strName = pChild->Attribute(SIGN_NAME_VALUE);

        // 保证key唯一
        map<int, wstring>::const_iterator iter = m_mapSignNames.find(id);

        if (iter != m_mapSignNames.end())
        {
            return FALSE;
        }

        m_mapSignNames.insert(make_pair(id, KStringConvert::Utf8ToWide(strName)));
        m_nMaxSignNamesMapId = max(m_nMaxSignNamesMapId, id);

        pChild = pChild->NextSiblingElement();
    }
    while (pChild != NULL);

    return TRUE;
}

BOOL ProcessLib::ParseProcessesNode()
{
    TiXmlHandle hDoc(m_pXmlDoc);

    TiXmlElement* pElem = hDoc.FirstChild(PROCESSLIB_NODE).FirstChild(PROCESSES_NODE).ToElement();

    if(!pElem->FirstChild())
    {
        return FALSE;
    }

    TiXmlElement* pNode = pElem->FirstChild()->ToElement();

    if (!pNode)
    {
        return FALSE;
    }

    // 处理Process节点
    do
    {
        string strName;
        strName = pNode->Attribute(PROCESS_NAME);

        KProcessInfo processInfo;
        processInfo.ProcessName(KStringConvert::ToLower(KStringConvert::Utf8ToWide(strName)));

        ParseProcessItemNode(pNode, processInfo);

        m_mapProcesses.insert(make_pair(processInfo.ProcessName(), processInfo));

        pNode = pNode->NextSiblingElement();
    }
    while (pNode != NULL);

    return TRUE;
}

BOOL ProcessLib::ParseMaxProcessItemIdNode()
{
    TiXmlHandle hDoc(m_pXmlDoc);

    TiXmlElement* pNode = hDoc.FirstChild(PROCESSLIB_NODE).FirstChild(MAX_PROCESS_ITEM_ID_NODE).ToElement();

    int id = 0;

    pNode->Attribute(MAX_PROCESS_ITEM_ID, &id);

    m_nMaxProcessItemMapId = max(m_nMaxProcessItemMapId, id);

    return TRUE;
}


BOOL ProcessLib::ParseProcessItemNode( TiXmlElement* node , KProcessInfo& processInfo)
{
    if (!node->FirstChild())
    {
        return TRUE;
    }

    TiXmlElement* pNode = node->FirstChild()->ToElement();

    do
    {
        KProcessItem processItem;
        processItem.ProcessName(processInfo.ProcessName());

        int id = 0;
        pNode->Attribute(PROCESS_ITEM_ID, &id);
        processItem.ProcessItemId(id);

        processItem.DisplayName(KStringConvert::Utf8ToWide(pNode->Attribute(PROCESS_DISPLAY_NAME)));
        processItem.Desc(KStringConvert::Utf8ToWide(pNode->Attribute(PROCESS_DESCRIPTION)));

        int pathIndex = -1;
        pNode->Attribute(PROCESS_PATHINDEX, &pathIndex);
        processItem.PathIndex(pathIndex);

        int pathLevel = -1;
        pNode->Attribute(PROCESS_PATH_LEVEL, &pathLevel);
        processItem.PathLevel(pathLevel);

        int companyNameIndex = -1;
        pNode->Attribute(PROCESS_COMPANYNAMEINDEX, &companyNameIndex);
        processItem.CompanyIndex(companyNameIndex);

        int signNameIndex = -1;
        pNode->Attribute(PROCESS_SIGNNAMEINDEX, &signNameIndex);
        processItem.SignNameIndex(signNameIndex);

        int processType = -1;
        pNode->Attribute(PROCESS_PROCESSTYPE, &processType);
        processItem.ProcessType(processType);

        int canClose = TRUE;
        pNode->Attribute(PROCESS_CANCLOSE, &canClose);
        processItem.CanClose(canClose);

        wstring softId = L"";
        softId = KStringConvert::Utf8ToWide(pNode->Attribute(PROCESS_SOFTID));
        processItem.SoftId(softId);

        processInfo.AddProcessItem(processItem);

        m_mapProcessItems.insert(make_pair(processItem.ProcessName(), processItem));

        m_nMaxProcessItemMapId = max(m_nMaxProcessItemMapId, id);

        pNode = pNode->NextSiblingElement();
    }
    while (pNode != NULL);

    return TRUE;
}

BOOL ProcessLib::AddPathNode(int id, const wstring& strPath )
{
    if (strPath.empty())
    {
        return FALSE;
    }

    // 保证key唯一
    map<int, wstring>::const_iterator iter = m_mapPath.find(id);

    if (iter != m_mapPath.end())
    {
        return FALSE;
    }

    m_mapPath.insert(make_pair(id, strPath));

    return TRUE;
}

int ProcessLib::AddPathNode( const wstring& strPath )
{
    if (strPath.empty())
    {
        return -1;
    }

    AddPathNode(++m_nMaxPathMapId, strPath);

    return m_nMaxPathMapId;
}

BOOL ProcessLib::RemovePathNode( int id )
{
    map<int, wstring>::iterator iter = m_mapPath.find(id);

    if (iter != m_mapPath.end())
    {
        m_mapPath.erase(iter);
        return TRUE;
    }

    return FALSE;
}

BOOL ProcessLib::ModifyPathNode( int id, const wstring& strNewPath )
{
    map<int, wstring>::iterator iter = m_mapPath.find(id);

    if (iter != m_mapPath.end())
    {
        iter->second = strNewPath;
        return TRUE;
    }

    return FALSE;
}

BOOL ProcessLib::RemoveCompanyNameNode( int id )
{
    map<int, wstring>::iterator iter = m_mapCompanyNames.find(id);

    if (iter != m_mapCompanyNames.end())
    {
        m_mapCompanyNames.erase(iter);
        return TRUE;
    }

    return FALSE;
}

BOOL ProcessLib::ModifyCompanyNameNode( int id, const wstring& strNewCompanyName )
{
    map<int, wstring>::iterator iter = m_mapCompanyNames.find(id);

    if (iter != m_mapCompanyNames.end())
    {
        iter->second = strNewCompanyName;
        return TRUE;
    }

    return FALSE;
}

BOOL ProcessLib::AddCompanyNameNode( int id, const wstring& strCompanyName )
{
    // 保证key唯一
    map<int, wstring>::const_iterator iter = m_mapCompanyNames.find(id);

    if (iter != m_mapCompanyNames.end())
    {
        return FALSE;
    }

    m_mapCompanyNames.insert(make_pair(id, strCompanyName));

    return TRUE;
}

int ProcessLib::AddCompanyNameNode( const wstring& strCompanyName )
{
    if (strCompanyName.empty())
    {
        return -1;
    }

    AddCompanyNameNode(++m_nMaxCompanyNamesMapId, strCompanyName);

    return m_nMaxCompanyNamesMapId;
}

BOOL ProcessLib::Save( LPCTSTR filePath )
{
    TiXmlDocument* pDoc = new TiXmlDocument;

    if (NULL == pDoc)
    {
        return false;
    }

    TiXmlDeclaration* pDeclaration = new TiXmlDeclaration("1.0", "utf-8", "");

    if (NULL == pDeclaration)
    {
        return false;
    }

    pDoc->LinkEndChild(pDeclaration);

    TiXmlElement* pRoot = new TiXmlElement(PROCESSLIB_NODE);

    if (NULL == pRoot)
    {
        return false;
    }

    pDoc->LinkEndChild(pRoot);
    GenPathNodes(pRoot);
    GenCompanyNameNodes(pRoot);
    GenSignNameNodes(pRoot);
    GenMaxProcessItemIdNode(pRoot);
    GenProcessNodes(pRoot);

    pDoc->SaveFile(KStringConvert::WideToAnsi(wstring(filePath)));

	delete pDoc;

    return TRUE;
}

BOOL ProcessLib::GenPathNodes( TiXmlElement* pRootNode )
{
    TiXmlElement* pPathsNode = new TiXmlElement(PATHS_NODE);

    if (NULL == pPathsNode)
    {
        return false;
    }

    pRootNode->LinkEndChild(pPathsNode);

    map<int, wstring>::const_iterator iter = m_mapPath.begin();

    for (; iter != m_mapPath.end(); iter++)
    {
        // 生成子节点
        TiXmlElement* pPath = new TiXmlElement(PATH_NODE);

        if (NULL == pPath)
        {
            return FALSE;
        }

        pPath->SetAttribute(PATH_ID, iter->first);
        pPath->SetAttribute(PATH_VALUE, KStringConvert::WideToUtf8(iter->second));

        pPathsNode->LinkEndChild(pPath);
    }

    return TRUE;
}

BOOL ProcessLib::GenCompanyNameNodes( TiXmlElement* pRootNode )
{
    TiXmlElement* pCompanyNamesNode = new TiXmlElement(COMPANY_NAMES);

    if (NULL == pCompanyNamesNode)
    {
        return false;
    }

    pRootNode->LinkEndChild(pCompanyNamesNode);

    map<int, wstring>::const_iterator iter = m_mapCompanyNames.begin();

    for (; iter != m_mapCompanyNames.end(); iter++)
    {
        // 生成子节点
        TiXmlElement* pCompanyNameNode = new TiXmlElement(COMPANY_NAME);

        if (NULL == pCompanyNameNode)
        {
            return FALSE;
        }

        pCompanyNameNode->SetAttribute(COMPANY_ID, iter->first);
        pCompanyNameNode->SetAttribute(COMPANY_VALUE, KStringConvert::WideToUtf8(iter->second));

        pCompanyNamesNode->LinkEndChild(pCompanyNameNode);
    }

    return TRUE;
}

BOOL ProcessLib::GenMaxProcessItemIdNode( TiXmlElement* pRootNode )
{
    TiXmlElement* pMaxProcessItemIdNode = new TiXmlElement(MAX_PROCESS_ITEM_ID_NODE);

    if (NULL == pMaxProcessItemIdNode)
    {
        return false;
    }

    pMaxProcessItemIdNode->SetAttribute(MAX_PROCESS_ITEM_ID, m_nMaxProcessItemMapId);
    pRootNode->LinkEndChild(pMaxProcessItemIdNode);

    return TRUE;
}

BOOL ProcessLib::GenProcessNodes( TiXmlElement* pRootNode )
{
    TiXmlElement* pProcessesNodes = new TiXmlElement(PROCESSES_NODE);

    if (NULL == pProcessesNodes)
    {
        return false;
    }

    pRootNode->LinkEndChild(pProcessesNodes);

    map<wstring, KProcessInfo>::iterator iter = m_mapProcesses.begin();

    for (; iter != m_mapProcesses.end(); iter++)
    {
        TiXmlElement* pProcessNode = new TiXmlElement(PROCESS_NODE);

        if (NULL == pProcessNode)
        {
            return false;
        }

        pProcessNode->SetAttribute(PROCESS_NAME, KStringConvert::WideToUtf8(iter->first));
        pProcessesNodes->LinkEndChild(pProcessNode);

        GenProcessItemNodes(pProcessNode, iter->second);
    }

    return TRUE;
}

BOOL ProcessLib::GenProcessItemNodes( TiXmlElement* pRootNode, KProcessInfo& processInfo)
{
    map<int, KProcessItem> mapProcessItems;
    processInfo.GetProcessItemsMap(mapProcessItems);

    map<int, KProcessItem>::iterator iter = mapProcessItems.begin();

    for (; iter != mapProcessItems.end(); iter++)
    {
        TiXmlElement* pProcessItemNode = new TiXmlElement(PROCESS_ITEM_NODE);

        if (NULL == pProcessItemNode)
        {
            return false;
        }

        pProcessItemNode->SetAttribute(PROCESS_ITEM_ID, iter->second.ProcessItemId());

        pProcessItemNode->SetAttribute(PROCESS_DISPLAY_NAME, KStringConvert::WideToUtf8(iter->second.DisplayName()));

        pProcessItemNode->SetAttribute(PROCESS_DESCRIPTION, KStringConvert::WideToUtf8(iter->second.Desc()));

        pProcessItemNode->SetAttribute(PROCESS_PATHINDEX, iter->second.PathIndex());
        pProcessItemNode->SetAttribute(PROCESS_PATH_LEVEL, iter->second.PathLevel());

        pProcessItemNode->SetAttribute(PROCESS_COMPANYNAMEINDEX, iter->second.CompanyIndex());

        pProcessItemNode->SetAttribute(PROCESS_SIGNNAMEINDEX, iter->second.SignNameIndex());
        pProcessItemNode->SetAttribute(PROCESS_PROCESSTYPE, iter->second.ProcessType());
        pProcessItemNode->SetAttribute(PROCESS_CANCLOSE, iter->second.CanClose());
        pProcessItemNode->SetAttribute(PROCESS_SOFTID, KStringConvert::WideToUtf8(iter->second.SoftId()));

        pRootNode->LinkEndChild(pProcessItemNode);
    }


    return FALSE;
}

BOOL ProcessLib::GetCompanyNameMap( map<int, wstring>& mapCompanyName )
{
    mapCompanyName = m_mapCompanyNames;

    return TRUE;
}

BOOL ProcessLib::GetPathMap( map<int, wstring>& mapPath )
{
    mapPath = m_mapPath;

    return TRUE;
}

BOOL ProcessLib::GetProcessMap( map<wstring, KProcessInfo>& mapProcesses )
{
    mapProcesses = m_mapProcesses;

    return TRUE;
}

std::wstring ProcessLib::GetPath( int id )
{
    map<int, wstring>::iterator iter = m_mapPath.find(id);

    if (iter != m_mapPath.end())
    {
        return iter->second;
    }

    return L"";
}

std::wstring ProcessLib::GetCompanyName( int id )
{
    map<int, wstring>::iterator iter = m_mapCompanyNames.find(id);

    if (iter != m_mapCompanyNames.end())
    {
        return iter->second;
    }

    return L"";
}

std::wstring ProcessLib::GetCompanyName( LPCTSTR szFilePath )
{
    wstring strCompanyDesc = L"";

    KFileVersion  fileVersion(szFilePath);

    strCompanyDesc = fileVersion.GetCompany().AllocSysString();

    return strCompanyDesc;
}
std::wstring ProcessLib::GetProcessTypeString( KProcessType enumProcessType )
{
    wstring str = L"";

    switch (enumProcessType)
    {
    case enum_ProcessType_SystemCore:
        str = L"系统核心进程";
        break;
    case enum_ProcessType_SystemService:
        str = L"系统服务";
        break;
    case enum_ProcessType_SystemProgram:
        str = L"系统程序";
        break;
    case enum_ProcessType_KingsoftService:
        str = L"金山服务";
        break;
    case enum_ProcessType_KingsoftProgram:
        str = L"金山程序";
        break;
    case enum_ProcessType_3rdPartyService:
        str = L"第三方服务";
        break;
    case enum_ProcessType_3rdPartyProgram:
        str = L"第三方程序";
        break;
    }

    return str;
}

KProcessItem ProcessLib::GetProcessItem( int id )
{
    KProcessItem processItem;

    map<wstring, KProcessInfo>::iterator  iter = m_mapProcesses.begin();

    for (; iter != m_mapProcesses.end(); iter++)
    {
        map<int, KProcessItem> mapProcessItem;
        iter->second.GetProcessItemsMap(mapProcessItem);
        map<int, KProcessItem>::const_iterator processItemIter = mapProcessItem.find(id);

        if (processItemIter != mapProcessItem.end())
        {
            return processItemIter->second;
        }
    }

    return processItem;
}

std::wstring ProcessLib::GetSignName( int id )
{
    map<int, wstring>::iterator iter = m_mapSignNames.find(id);

    if (iter != m_mapSignNames.end())
    {
        return iter->second;
    }

    return L"";
}

BOOL ProcessLib::GenSignNameNodes( TiXmlElement* pRootNode )
{
    TiXmlElement* pSignNamesNode = new TiXmlElement(SIGN_NAMES);

    if (NULL == pSignNamesNode)
    {
        return false;
    }

    pRootNode->LinkEndChild(pSignNamesNode);

    map<int, wstring>::const_iterator iter = m_mapSignNames.begin();

    for (; iter != m_mapSignNames.end(); iter++)
    {
        // 生成子节点：Welcome
        TiXmlElement* pSignNameNode = new TiXmlElement(SIGN_NAME);

        if (NULL == pSignNameNode)
        {
            return FALSE;
        }

        pSignNameNode->SetAttribute(SIGN_NAME_ID, iter->first);
        pSignNameNode->SetAttribute(SIGN_NAME_VALUE, KStringConvert::WideToUtf8(iter->second));

        pSignNamesNode->LinkEndChild(pSignNameNode);
    }

    return TRUE;
}

BOOL ProcessLib::GetSignNameMap( map<int, wstring>& mapSignName )
{
    mapSignName = m_mapSignNames;
    return TRUE;
}

int ProcessLib::FindPathId( wstring path )
{
    map<int, wstring>::iterator iter = m_mapPath.begin();

    for (; iter != m_mapPath.end(); iter++)
    {
        if (_wcsicmp(iter->second.c_str(), path.c_str()) == 0)
        {
            return iter->first;
        }
    }

    return -1;
}

std::wstring ProcessLib::GetDigitalSignaturerName(LPCTSTR szFilePath)
{
    DWORD   dwWinTrust;
    HRESULT hr = S_OK;
    WinMod::CWinTrustSignerInfoChain signInfoChain;
    WinMod::CWinTrustVerifier   m_verifer;
    m_verifer.TryLoadDll();
    wstring strSignName;

    dwWinTrust = m_verifer.VerifyFile( szFilePath, &signInfoChain );

    if (dwWinTrust <= 0x0000FFFF)
        hr = AtlHresultFromWin32(dwWinTrust);
    else
        hr = dwWinTrust;

    if ( SUCCEEDED( hr ) )
    {
        if ( !signInfoChain.m_chain.IsEmpty())
        {
            strSignName = signInfoChain.m_chain.GetHead().m_strIsserName.AllocSysString();
        }
    }

    return strSignName;
}

int ProcessLib::FindCompanyId( wstring company )
{
    map<int, wstring>::iterator iter = m_mapCompanyNames.begin();

    for (; iter != m_mapCompanyNames.end(); iter++)
    {
        if (_wcsicmp(iter->second.c_str(), company.c_str()) == 0)
        {
            return iter->first;
        }
    }

    return -1;
}

int ProcessLib::FindSignNameId( wstring signName )
{
    map<int, wstring>::iterator iter = m_mapSignNames.begin();

    for (; iter != m_mapSignNames.end(); iter++)
    {
        if (_wcsicmp(iter->second.c_str(), signName.c_str()) == 0)
        {
            return iter->first;
        }
    }

    return -1;
}

int ProcessLib::AddSignNameNode( const wstring& strSignName )
{
    if (strSignName.empty())
    {
        return -1;
    }

    AddSignNameNode(++m_nMaxSignNamesMapId, strSignName);

    return m_nMaxSignNamesMapId;
}

BOOL ProcessLib::AddSignNameNode( int id, const wstring& strSignName )
{
    // 保证key唯一
    map<int, wstring>::const_iterator iter = m_mapSignNames.find(id);

    if (iter != m_mapSignNames.end())
    {
        return FALSE;
    }

    m_mapSignNames.insert(make_pair(id, strSignName));

    return TRUE;
}

BOOL ProcessLib::GetProcessInfo( wstring fileName, KProcessInfo& processInfo )
{
	fileName = KStringConvert::ToLower(fileName);
    map<wstring, KProcessInfo>::iterator iter = m_mapProcesses.find(fileName);

    if (iter != m_mapProcesses.end())
    {
        processInfo = iter->second;
        return TRUE;
    }

    return FALSE;
}

BOOL ProcessLib::AddProcessInfo( wstring fileName, KProcessInfo& processInfo )
{
    wstring fileNameKey = KStringConvert::ToLower(fileName);
    KProcessInfo newProcessInfo;
    newProcessInfo.ProcessName(fileNameKey);

    map<wstring, KProcessInfo>::const_iterator iter = m_mapProcesses.find(fileNameKey);

    if (iter != m_mapProcesses.end())
    {
        processInfo = iter->second;
    }
    else
    {
        m_mapProcesses.insert(make_pair(fileNameKey, newProcessInfo));

        processInfo = newProcessInfo;
    }

    return TRUE;
}


BOOL ProcessLib::AddProcessItem( wstring processName, KProcessItem& processItem, ProcessLibOverwriteRule& overwriteRule )
{
    wstring fileNameKey = KStringConvert::ToLower(processName);

    map<wstring, KProcessInfo>::iterator iter = m_mapProcesses.find(fileNameKey);

    // 进程名存在
    if (iter != m_mapProcesses.end())
    {
        map<int, KProcessItem>  mapProcessItems;
        iter->second.GetProcessItemsMap(mapProcessItems);

        map<int, KProcessItem>::iterator processItemIter = mapProcessItems.begin();

        // 如果只有有一条记录，按覆盖规则处理
        if (1 == mapProcessItems.size() )
        {
        }
        else
        {
            // 有多条，则按路径匹配
            for (; processItemIter != mapProcessItems.end(); processItemIter++)
            {
                if (processItemIter->second.PathIndex() == processItem.PathIndex())
                {
                    break;
                }
            }
        }

        if (processItemIter != mapProcessItems.end())
        {
            BOOL bIsModify = FALSE;
            KProcessItem newProcessItem = processItem;
			KProcessItem oldProcessItem = processItemIter->second;

            if (overwriteRule.Overwrite_CompanyName)
            {
				bIsModify = TRUE;
                oldProcessItem.CompanyIndex(newProcessItem.CompanyIndex());
            }

            if (overwriteRule.Overwrite_Desc)
            {
				bIsModify = TRUE;
                oldProcessItem.Desc(newProcessItem.Desc());
            }

            if (overwriteRule.Overwrite_Path)
            {
				bIsModify = TRUE;
                oldProcessItem.PathIndex(newProcessItem.PathIndex());
            }

            if (overwriteRule.Overwrite_SignName)
            {
				bIsModify = TRUE;
                oldProcessItem.SignNameIndex(newProcessItem.SignNameIndex());
            }

            if (overwriteRule.Overwrite_ProcessType)
            {
				bIsModify = TRUE;
                oldProcessItem.ProcessType(newProcessItem.ProcessType());
            }

            if (overwriteRule.Overwrite_SoftID)
            {
				bIsModify = TRUE;
                oldProcessItem.SoftId(newProcessItem.SoftId());
            }

            if (overwriteRule.Overwrite_CanClose)
            {
				bIsModify = TRUE;
                oldProcessItem.CanClose(newProcessItem.CanClose());
            }

            if (bIsModify)
            {
				processItem.ProcessItemId(oldProcessItem.ProcessItemId());
                return ModifyProcessItem(oldProcessItem.ProcessItemId(), oldProcessItem);
            }
            else
            {
                newProcessItem.ProcessItemId(++m_nMaxProcessItemMapId);

				processItem.ProcessItemId(newProcessItem.ProcessItemId());
                return iter->second.AddProcessItem(newProcessItem);
            }
        }

        if (!iter->second.FindProcessItem(processItem.PathIndex(), processItem.CompanyIndex(), processItem.SignNameIndex()))
        {
            processItem.ProcessItemId(++m_nMaxProcessItemMapId);
            return iter->second.AddProcessItem(processItem);
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        KProcessInfo processInfo;
        processInfo.ProcessName(fileNameKey);

        processItem.ProcessItemId(++m_nMaxProcessItemMapId);
        processInfo.AddProcessItem(processItem);

        m_mapProcesses.insert(make_pair(fileNameKey, processInfo));

        return TRUE;
    }
}

BOOL ProcessLib::AddProcessItem( wstring processName, KProcessItem& processItem )
{
    ProcessLibOverwriteRule overwriteRule;
    return AddProcessItem(processName, processItem, overwriteRule);
}

BOOL ProcessLib::ModifyProcessItem( int id, KProcessItem& newProcessItem )
{
    map<wstring, KProcessInfo>::iterator  iter = m_mapProcesses.begin();

    for (; iter != m_mapProcesses.end(); iter++)
    {
        map<int, KProcessItem> mapProcessItem;
        iter->second.GetProcessItemsMap(mapProcessItem);
        map<int, KProcessItem>::iterator processItemIter = mapProcessItem.find(id);

        if (processItemIter != mapProcessItem.end())
        {
            iter->second.RemoveProcessItem(newProcessItem.ProcessItemId());
            iter->second.AddProcessItem(newProcessItem);

            //processItemIter->second = newProcessItem;

            return TRUE;
        }
    }

    return FALSE;
}

BOOL ProcessLib::IsProcessNameExist( wstring strProcessName )
{
    wstring processNameLower = KStringConvert::ToLower(strProcessName);
    map<wstring, KProcessInfo>::const_iterator  iter = m_mapProcesses.find(processNameLower);

    return iter != m_mapProcesses.end();
}

BOOL ProcessLib::RemoveProcessItem( int processItemId )
{
    map<wstring, KProcessInfo>::iterator  iter = m_mapProcesses.begin();

    for (; iter != m_mapProcesses.end(); iter++)
    {
        map<int, KProcessItem> mapProcessItem;
        iter->second.GetProcessItemsMap(mapProcessItem);
        map<int, KProcessItem>::iterator processItemIter = mapProcessItem.find(processItemId);

        if (processItemIter != mapProcessItem.end())
        {
            iter->second.RemoveProcessItem(processItemId);

            return TRUE;
        }
    }

    return FALSE;
}

int ProcessLib::GetMaxProcessItemId()
{
    return m_nMaxProcessItemMapId;
}

BOOL ProcessLib::RemoveSignNameNode( int id )
{
    map<int, wstring>::iterator iter = m_mapSignNames.find(id);

    if (iter != m_mapSignNames.end())
    {
        m_mapSignNames.erase(iter);
        return TRUE;
    }

    return FALSE;
}

BOOL ProcessLib::Merge( ProcessLib* secondLib, ProcessLibOverwriteRule overWriteRule )
{
    map<int, wstring> secondCompanyNameMap;
    secondLib->GetCompanyNameMap(secondCompanyNameMap);
    MergeCompanyNameNode(secondCompanyNameMap);

    map<int, wstring> secondPathMap;
    secondLib->GetPathMap(secondPathMap);
    MergePathMap(secondPathMap);

    map<int, wstring> secondSignMap;
    secondLib->GetSignNameMap(secondSignMap);
    MergeSignNameMap(secondSignMap);

    map<wstring, KProcessInfo> secondProcessesMap;
    secondLib->GetProcessMap(secondProcessesMap);
    MergeProcessesMap(secondLib, secondProcessesMap, overWriteRule);


    return TRUE;
}

BOOL ProcessLib::MergeCompanyNameNode( map<int, wstring>& secondMap )
{
    return MergeIndexMap(&m_mapCompanyNames, &secondMap);
}

BOOL ProcessLib::MergeIndexMap( map<int, wstring>* firstMap, map<int, wstring>* secondMap )
{
    // 遍历第2个map，把数据都增加到第1个map中。
    // 规则：相同value的则不添加
    //        不同value则添加，id按照第一个map增加

    int nMaxFirstMapId = 1;
    map<int, wstring>::iterator firstMapIter = firstMap->begin();

    for (; firstMapIter != firstMap->end(); firstMapIter++)
    {
        nMaxFirstMapId = max(firstMapIter->first, nMaxFirstMapId);
    }

    map<int, wstring>::iterator secondMapIter = secondMap->begin();

    for (; secondMapIter != secondMap->end(); secondMapIter++)
    {
        if (!IsMapValueExist(firstMap, secondMapIter->second))
        {
            firstMap->insert(make_pair(++nMaxFirstMapId, secondMapIter->second));
        }
    }

    return TRUE;
}

BOOL ProcessLib::IsMapValueExist( map<int, wstring>* mapToSearch, wstring& strValue )
{
    map<int, wstring>::iterator mapIter = mapToSearch->begin();

    for (; mapIter != mapToSearch->end(); mapIter++)
    {
        if (_wcsicmp(mapIter->second.c_str(), strValue.c_str()) == 0)
        {
            return TRUE;
        }
    }

    return FALSE;
}

BOOL ProcessLib::MergeSignNameMap( map<int, wstring>& secondMap )
{
    return MergeIndexMap(&m_mapSignNames, &secondMap);
}

BOOL ProcessLib::MergePathMap( map<int, wstring>& secondMap )
{
    return MergeIndexMap(&m_mapPath, &secondMap);
}

BOOL ProcessLib::MergeProcessesMap(ProcessLib* secondLib, map<wstring, KProcessInfo>& secondMap, ProcessLibOverwriteRule& overWriteRule )
{
    // 遍历第2个map，把数据都增加到第1个map中。
    // 规则：
    // 1.如果process名称都不存在，则增加Process节点，并添加第2个map该结点下的processitem对象
    // 2.如果process名称存在，则遍历ProcessItem，如果不存在则添加processItem到该结点下。
    //   判断ProcessItem是否相同，按照覆盖规则来判断

    int nMaxProcessItemId = m_nMaxProcessItemMapId;

    map<wstring, KProcessInfo>::iterator secondMapIter = secondMap.begin();

    for (; secondMapIter != secondMap.end(); secondMapIter++)
    {
        map<int, KProcessItem> mapProcessItems;
        secondMapIter->second.GetProcessItemsMap(mapProcessItems);

        map<int, KProcessItem>::iterator mapProcessItemsIter = mapProcessItems.begin();

        for (; mapProcessItemsIter != mapProcessItems.end(); mapProcessItemsIter++)
        {
            KProcessItem  newProcessItem = mapProcessItemsIter->second;

            // 这里需要重新修改processItem中的路径，公司名称，数字签名的索引值
            newProcessItem.CompanyIndex(FindCompanyId(secondLib->GetCompanyName(newProcessItem.CompanyIndex())));
            newProcessItem.SignNameIndex(FindSignNameId(secondLib->GetSignName(newProcessItem.SignNameIndex())));
            newProcessItem.PathIndex(FindPathId(secondLib->GetPath(newProcessItem.PathIndex())));

            this->AddProcessItem(secondMapIter->first, newProcessItem, overWriteRule);
        }
    }

    return TRUE;
}

BOOL ProcessLib::ModifySignNameNode( int id, const wstring& newName )
{
    map<int, wstring>::iterator iter = m_mapSignNames.find(id);

    if (iter != m_mapSignNames.end())
    {
        iter->second = newName;
        return TRUE;
    }

    return FALSE;
}



std::wstring ProcessLib::GetFileName( LPCTSTR szFilePath )
{
    return KPath::GetFileName(wstring(szFilePath));
}

std::wstring ProcessLib::GetFileDesc( LPCTSTR szFilePath )
{
    wstring strFileDesc = L"";

    KFileVersion  fileVersion(szFilePath);

    strFileDesc = fileVersion.GetFileDescription().AllocSysString();

    return strFileDesc;
}

std::wstring ProcessLib::GetTwoLevelPath( LPCTSTR szFilePath, int& nPathLevel )
{
    wstring strFullPath = szFilePath;

    vector<wstring>  dirs = KStringConvert::Split(strFullPath, L"\\");

    if (dirs.size() < 3)
    {
        nPathLevel = 0;
        return L"";
    }

    // c:\ksafe\a.exe
    if (dirs.size() == 3)
    {
        nPathLevel = 1;
        // 取中间第2个
        return dirs[1];
    }

    // c:\a\c\ksafe\b.exe
    if (dirs.size() >= 4)
    {
        nPathLevel = 2;
        return dirs[dirs.size() - 3] + L"\\" + dirs[dirs.size() - 2];
    }

    return L"";
}


BOOL ProcessLib::FindInfo( LPCTSTR szFilePath,  KProcessItem& pInfo/*= NULL*/ )
{
    wstring fileName = KStringConvert::ToLower(GetFileName(szFilePath));

    pair<multimap<wstring, KProcessItem>::iterator, multimap<wstring, KProcessItem>::iterator> queryRange;
    queryRange = m_mapProcessItems.equal_range(fileName);
    multimap<wstring, KProcessItem>::iterator iter;

	wstring wTest;

    int itemsCount = 0;

    // 多条记录先按路径筛选
    vector<KProcessItem> processItemsFound;

	int nPathLevel = 0;
	wstring twoLevelPath = GetTwoLevelPath(szFilePath, nPathLevel);
	wstring oneLevelPath = GetOneLevelPath(szFilePath);
    for (iter = queryRange.first; iter != queryRange.second; iter++)
    {
        wstring filePath;

		if (!wcschr(GetPath(iter->second.PathIndex()).c_str(), L'\\'))
		{
            filePath = oneLevelPath;
		}
		else
		{
            filePath = twoLevelPath;
		}

        if (-1 == iter->second.PathIndex())
        {
			if (_wcsicmp(iter->first.c_str(), fileName.c_str()) == 0)
			{
				processItemsFound.push_back(iter->second);
			}
        }
        else if (_wcsicmp(GetPath(iter->second.PathIndex()).c_str(), filePath.c_str()) == 0)
        {
            processItemsFound.push_back(iter->second);
        }
    }

    // 只筛选到1条，返回
    if (1 == processItemsFound.size())
    {
        pInfo = processItemsFound[0];
        return TRUE;
    }
	else
	{
		itemsCount = 0;
		for (DWORD ex = 0; ex < processItemsFound.size(); ex ++)
		{
			if (ex == 0)
			{
				wTest = processItemsFound[0].Desc();
				itemsCount++;
			}
			else
			{
				if (wTest != processItemsFound[ex].Desc())
					itemsCount ++;
			}
		}

		if (1 == itemsCount)
		{
		    pInfo = processItemsFound[0];
			return TRUE;
		}
	}

    // 重新按公司名匹配
    processItemsFound.clear();
    int companyIndex = FindCompanyId(GetCompanyName(szFilePath));

	if (companyIndex != -1)
	{
		for (iter = queryRange.first; iter != queryRange.second; iter++)
		{
			if (iter->second.CompanyIndex() == companyIndex)
			{
				processItemsFound.push_back(iter->second);
			}
		}

		// 只筛选到1条，返回
		if (1 == processItemsFound.size())
		{
			pInfo = processItemsFound[0];
			return TRUE;
		}
		else
		{
			itemsCount = 0;
			for (DWORD ex = 0; ex < processItemsFound.size(); ex ++)
			{
				if (ex == 0)
				{
					wTest = processItemsFound[0].Desc();
					itemsCount++;
				}
				else
				{
					if (wTest != processItemsFound[ex].Desc())
						itemsCount ++;
				}
			}

			if (1 == itemsCount)
			{
				pInfo = processItemsFound[0];
				return TRUE;
			}
		}
	}
    
    // 重新按数字签名匹配
    processItemsFound.clear();
    int signIndex = FindSignNameId(GetDigitalSignaturerName(szFilePath));

	if (signIndex != -1)
	{
		for (iter = queryRange.first; iter != queryRange.second; iter++)
		{
			if (iter->second.SignNameIndex() == signIndex)
			{
				processItemsFound.push_back(iter->second);
			}
		}

		// 只要筛选到，直接返回第一条
		if (processItemsFound.size() > 0)
		{
			pInfo = processItemsFound[0];
			return TRUE;
		}
	}

    return FALSE;
}

BOOL ProcessLib::ClearCompnayMap()
{
    m_mapCompanyNames.clear();
    return TRUE;
}

BOOL ProcessLib::ClearSignNameMap()
{
    m_mapSignNames.clear();
    return TRUE;
}

BOOL ProcessLib::ClearPathMap()
{
    m_mapPath.clear();
    return TRUE;
}

BOOL ProcessLib::ClearProcessMap()
{
    m_mapProcesses.clear();
    return TRUE;
}

BOOL ProcessLib::ClearAll()
{
    m_mapProcessItems.clear();
    return  ClearCompnayMap() &&
            ClearSignNameMap() &&
            ClearPathMap() &&
            ClearProcessMap();
}

std::wstring ProcessLib::GetOneLevelPath( LPCTSTR szFilePath )
{
    wstring strFullPath = szFilePath;
    // 用\ 分隔。如:c:\windows\system32
    // 拆分后为 c:  windows   system32
    vector<wstring>  dirs = KStringConvert::Split(strFullPath, L"\\");

    // c:\a.exe
    if (dirs.size() < 3)
    {
        return L"";
    }

    // c:\ksafe\a.exe
    if (dirs.size() >= 3)
    {
        // 取中间第2个
        return dirs[dirs.size() - 2];
    }

	return L"";
}

void ProcessLib::TempFun()
{
    /*int  pathCount = 0;
    vector<int>  programfilesPath;

    map<int, wstring>::iterator iterPath = m_mapPath.begin();
    for (; iterPath!= m_mapPath.end(); iterPath++)
    {
    	if (iterPath->second.find(L"windows\\") != -1)
    	{
    		programfilesPath.push_back(iterPath->first);
    		pathCount++;
    	}
    }

    int itemCount = 0;
    map<wstring, KProcessInfo>::iterator  iter = m_mapProcesses.begin();

    for (;iter != m_mapProcesses.end(); iter++ )
    {
    	map<int, KProcessItem> mapProcessItems;
    	iter->second.GetProcessItemsMap(mapProcessItems);

    	map<int,KProcessItem>::iterator  iterProcessitem = mapProcessItems.begin();
    	for (; iterProcessitem != mapProcessItems.end(); iterProcessitem++)
    	{
    		if (find(programfilesPath.begin(), programfilesPath.end(), iterProcessitem->second.PathIndex()) != programfilesPath.end())
    		{
    			KProcessItem newprocessItem = iterProcessitem->second;
    			if (newprocessItem.PathLevel() == 2)
    			{
    				newprocessItem.PathLevel(1);
    				itemCount++;
    				this->ModifyProcessItem(newprocessItem.ProcessItemId(), newprocessItem);
    			}
    		}
    	}
    }

    vector<int>::iterator iterPathVector = programfilesPath.begin();

    int pathMCount = 0;
    for (; iterPathVector != programfilesPath.end(); iterPathVector++)
    {
    	wstring path = GetPath(*iterPathVector);
    	path = KStringConvert::Replace(path, L"windows\\", L"");
    	ModifyPathNode(*iterPathVector, path);
    	pathMCount++;
    }

    wstring info = KStringConvert::Format(L"ProgramCount:%d, ItemCount:%d,PathCount:%d",
    	pathCount, itemCount, pathMCount);*/
}