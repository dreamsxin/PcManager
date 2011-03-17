#pragma once

#include <miniutil/bkjson.h>

/*

    Usage:

    定义结构体：
    struct struct_name
    {
        value_type value_name;
    };

    定义序列化格式：
    BDX_DEF_BEGIN(struct_name)
        BDX_DEF(value_name, value_serialize_name)
    BDX_DEF_END()

    序列化：
    BDX_ToJson(struct_name &value, CString& strJson, BOOL bFileName = FALSE);

    反序列化：
    BDX_FromJson(LPCTSTR lpszJson, T &value, BOOL bFileName = FALSE);


    ****************************************************************

    自定义序列化文本格式：

    T_Cursor标识DOM结构文档的节点，一个实现的例子是BkJson::Handle，需要实现以下方法

    T_Cursor operator[](LPCTSTR lpszKey);
    T_Cursor operator[](int nIndex);
    T_Cursor& operator=(LPCWSTR szValue);
    T_Cursor& operator=(LONG lValue);
    int Count();
    void SetCount(int nNewCount);
*/

template <class T_Cursor, class T>
void BDX_Work(T_Cursor &h, T *pvalue, BOOL bSerialize)
{
    if (bSerialize)
        h = *pvalue;
    else
        *pvalue = (T)h;
}

template <class T_Cursor>
void BDX_Work(T_Cursor &h, WORD *pvalue, BOOL bSerialize)
{
    if (bSerialize)
        h = (DWORD)*pvalue;
    else
        *pvalue = LOWORD((DWORD)h);
}

template <class T_Cursor, class T>
void BDX_Work(T_Cursor &h, CAtlArray<T> *pvalue, BOOL bSerialize)
{
    int nCount = 0;

    if (bSerialize)
    {
        nCount = (int)pvalue->GetCount();

        h.SetCount(nCount);
    }
    else
    {
        nCount = h.Count();

        pvalue->SetCount(nCount);
    }

    for (int i = 0; i < nCount; i ++)
        BDX_Work(h[i], &((*pvalue)[i]), bSerialize);
}

template <class T_Cursor, class T>
void BDX_Work(T_Cursor &h, CAtlList<T> *pvalue, BOOL bSerialize)
{
    int nCount = 0;

    if (bSerialize)
    {
        nCount = (int)pvalue->GetCount();

        h.SetCount(nCount);

        int i = 0;
        POSITION pos = pvalue->GetHeadPosition();

        while (pos)
            BDX_Work(h[i ++], &(pvalue->GetNext(pos)), TRUE);
    }
    else
    {
        nCount = h.Count();

        for (int i = 0; i < nCount; i ++)
        {
            T tmpValue;
            BDX_Work(h[i], &tmpValue, FALSE);

            pvalue->AddTail(tmpValue);
        }
    }
}

template <class T_Cursor, class T_Key, class T_Value, class T_KTraits, class T_VTraits>
void BDX_Work(T_Cursor &h, CAtlMap<T_Key, T_Value, T_KTraits, T_VTraits> *pvalue, BOOL bSerialize)
{
    if (bSerialize)
    {
        int nCount = (int)pvalue->GetCount(), i = 0;
        POSITION pos = pvalue->GetStartPosition();

        h.SetCount(nCount);

        while (pos)
        {
            CAtlMap<T_Key, T_Value, T_KTraits, T_VTraits>::CPair *pPair = pvalue->GetNext(pos);

            if (pPair)
            {
                h[i].SetCount(2);

                T_Key key = pPair->m_key;
                T_Value value = pPair->m_value;

                BDX_Work(h[i][0], &key, TRUE);
                BDX_Work(h[i][1], &value, TRUE);

                i ++;
            }
        }
    }
    else
    {
        int nCount = h.Count();

        for (int i = 0; i < nCount; i ++)
        {
            T_Key key;
            T_Value value;

            BDX_Work(h[i][0], &key, FALSE);
            BDX_Work(h[i][1], &value, FALSE);

            (*pvalue)[key] = value;
        }
    }
}

#define BDX_DEF_BEGIN(struct_name)                                      \
template <class T_Cursor>                                               \
void BDX_Work(T_Cursor &h, struct_name *pvalue, BOOL bSerialize)        \
{                                                                       \

#define BDX_DEF(value_name, value_serialize_name)                       \
    BDX_Work(h[value_serialize_name], &pvalue->value_name, bSerialize); \

#define BDX_DEF_LAZY(value_name)                                        \
    BDX_DEF(value_name, L#value_name)                                   \

#define BDX_DEF_END()                                                   \
}                                                                       \

BDX_DEF_BEGIN(SYSTEMTIME)
    BDX_DEF_LAZY(wYear)
    BDX_DEF_LAZY(wMonth)
    BDX_DEF_LAZY(wDayOfWeek)
    BDX_DEF_LAZY(wDay)
    BDX_DEF_LAZY(wHour)
    BDX_DEF_LAZY(wMinute)
    BDX_DEF_LAZY(wSecond)
    BDX_DEF_LAZY(wMilliseconds)
BDX_DEF_END()

template <class T, class T_Cursor>
void BDX_Serialize(T_Cursor &h, T *pvalue)
{
    BDX_Work(h, pvalue, TRUE);
}

template <class T, class T_Cursor>
void BDX_Unserialize(T_Cursor h, T *pvalue)
{
    BDX_Work(h, pvalue, FALSE);
}

template <class T>
void BDX_ToJson(T &value, CString& strJson, BOOL bFileName = FALSE)
{
    BkJson::Document doc;

    BDX_Work(doc, &value, TRUE);

    if (bFileName)
        doc.SaveFile(strJson);
    else
        strJson = doc.Dump();
}

template <class T>
BOOL BDX_FromJson(LPCTSTR lpszJson, T &value, BOOL bFileName = FALSE)
{
    BOOL bRet = FALSE;
    BkJson::Document doc;

    if (bFileName)
        bRet = doc.LoadFile(lpszJson);
    else
        bRet = doc.LoadString(lpszJson);

    if (bRet)
        BDX_Work(doc, &value, FALSE);

    return bRet;
}
