/**
* @file    bkxdx_cursor_bkjson.h
* @brief   ...
* @author  bbcallen
* @date    2009-11-20 15:58
*/

#ifndef BKXDX_CURSOR_BKJSON_H
#define BKXDX_CURSOR_BKJSON_H

#include "miniutil\bkjson.h"
#include "bkxdx_base.h"
#include "bkxdx_error.h"
#include "bkxdx_lexvalue.h"

NS_BKXDX_BEGIN

class CBKJsonCursorContext
{
public:
    CBKJsonCursorContext()
        : m_array_index(-1)
        , m_map_pos(NULL)
    {
    }

    BkJson::Handle  m_handle;
    int             m_array_index;  // index in parent(array)
    POSITION        m_map_pos;      // position in parent(map);
};



class CBKJsonCursorLexCast;

class CBKJsonCursor
{
public:
    typedef CBKJsonCursorLexCast LexCast;

    CBKJsonCursor();


    void InitAsLexCast(const CBKJsonCursor& tmpl);


    // 读取数据
    template <class T_Value>
    void ReadData(T_Value& value);

    // 写入数据
    template <class T_Value>
    void WriteData(T_Value& value, LPCWSTR lpszRootName = NULL);



    void SetCodePage(int code_page = CP_UTF8);
    int  GetCodePage();


    void RaiseError(int error_code);
    void ClearError();
    void RaiseIfError();


    void OpenForWrite(LPCWSTR lpszRootName = L"root");
    void OpenForRead();
    bool IsError();
    bool IsWriteMode();


    void BeginChild(LPCWSTR name);
    void EndChild();


    void BeginChildren();
    bool FindNextChild();
    void EndChildren();
    bool IsEndOfChildren();


    CString GetName();


    void BeginArray();
    void EndArray();
    void AppendArrayElem();
    bool FindNextArrayElem();
    bool IsEndOfArrayElem();


    // 处理泛型参数的默认模板, 如果需要处理特定参数, 则重载该函数
    template <class T_Value>
    void AccessObject(T_Value& value) {ObjectExchange(*this, value);}


    void    SetLexValue(LPCWSTR lpszValue);

    CString GetLexValue();

protected:
    void    _RaiseIfEmptyStack();

public:

    CAtlList<CBKJsonCursorContext>  m_ctx_stack;
    BkJson::Document                m_doc;
    bool                            m_write_to_doc;
    bool                            m_is_error;
    int                             m_error_code;
    int                             m_code_page;
};


inline CBKJsonCursor::CBKJsonCursor()
{
    m_write_to_doc  = false;
    m_is_error      = false;
    m_error_code    = 0;
    m_code_page     = CP_UTF8;
}



inline void CBKJsonCursor::InitAsLexCast(const CBKJsonCursor& tmplcast)
{
    m_write_to_doc  = tmplcast.m_write_to_doc;
    m_is_error      = tmplcast.m_is_error;
    m_error_code    = tmplcast.m_error_code;
    m_code_page     = tmplcast.m_code_page;
}



template <class T_Value>
inline void CBKJsonCursor::ReadData(T_Value& value)
{
    OpenForRead();
    
    XDX_EXCEPTION_WRAP_BEGIN
    AccessObject(value);
    XDX_EXCEPTION_WRAP_END
}

template <class T_Value>
inline void CBKJsonCursor::WriteData(T_Value& value, LPCWSTR lpszRootName)
{
    OpenForWrite(lpszRootName);

    XDX_EXCEPTION_WRAP_BEGIN
    AccessObject(value);
    XDX_EXCEPTION_WRAP_END
}



inline void CBKJsonCursor::SetCodePage(int code_page)
{
    m_code_page = code_page;
}

inline int  CBKJsonCursor::GetCodePage()
{
    return m_code_page;
}


inline void CBKJsonCursor::RaiseError(int error_code)
{
    m_is_error   = true;
    m_error_code = error_code;
    XDX_RAISE_EXCEPTION();
}

inline void CBKJsonCursor::ClearError()
{
    m_is_error   = false;
    m_error_code = 0;
}

inline void CBKJsonCursor::RaiseIfError()
{
    if (m_is_error)
    {
        XDX_RAISE_EXCEPTION();
    }
}

inline void CBKJsonCursor::OpenForWrite(LPCWSTR lpszRootName)
{
    UNREFERENCED_PARAMETER(lpszRootName);

    ClearError();
    m_ctx_stack.RemoveAll();
    m_ctx_stack.AddTail();
    m_ctx_stack.GetTail().m_handle = m_doc;
    m_write_to_doc = true;
    m_doc.Clear();
}

inline void CBKJsonCursor::OpenForRead()
{
    ClearError();
    m_ctx_stack.RemoveAll();
    m_ctx_stack.AddTail();
    m_ctx_stack.GetTail().m_handle = m_doc;
    m_write_to_doc = false;
}


inline bool CBKJsonCursor::IsError()
{
    return !m_is_error;
}



inline bool CBKJsonCursor::IsWriteMode()
{
    return m_write_to_doc;
}



inline void CBKJsonCursor::BeginChild(LPCWSTR name)
{
    assert(!m_ctx_stack.IsEmpty());
    assert(!m_is_error);
    RaiseIfError();
    _RaiseIfEmptyStack();


    CBKJsonCursorContext& top = m_ctx_stack.GetTail();


    if (!m_write_to_doc && !top.m_handle.HasKey(name))
    {   // read mode
        RaiseError(ERROR_BKXDX_CHILD_NOT_FOUND);
    }

    
    CBKJsonCursorContext child;
    child.m_handle = top.m_handle[name];
    m_ctx_stack.AddTail(child);
    return;
}

inline void CBKJsonCursor::EndChild()
{
    assert(!m_ctx_stack.IsEmpty());
    assert(!m_is_error);
    RaiseIfError();
    _RaiseIfEmptyStack();


    m_ctx_stack.RemoveTailNoReturn();
}







inline void CBKJsonCursor::BeginChildren()
{
    assert(!m_ctx_stack.IsEmpty());
    assert(!m_is_error);
    RaiseIfError();
    _RaiseIfEmptyStack();


    CBKJsonCursorContext& top = m_ctx_stack.GetTail();


    if (IsWriteMode())
        top.m_handle.RemoveAllChildren();


    if (0 == top.m_handle.Count())
    {
        m_ctx_stack.AddTail(CBKJsonCursorContext());
        return;
    }


    CBKJsonCursorContext elem;
    elem.m_map_pos     = top.m_handle.FindFirstChild();
    elem.m_handle      = top.m_handle.GetAt(elem.m_map_pos);
    m_ctx_stack.AddTail(elem);
}

inline bool CBKJsonCursor::FindNextChild()
{
    assert(!IsWriteMode());
    assert(!m_ctx_stack.IsEmpty());
    assert(!m_is_error);
    RaiseIfError();
    _RaiseIfEmptyStack();



    if (IsEndOfChildren())
        return false;



    CBKJsonCursorContext elem = m_ctx_stack.RemoveTail();
    _RaiseIfEmptyStack();


    CBKJsonCursorContext& top = m_ctx_stack.GetTail();


    // m_array_index is -1 before first call to FindNextArrayElem()
    if (NULL == elem.m_map_pos)
    {   // fill a null handle back to stack
        m_ctx_stack.AddTail(CBKJsonCursorContext());
        return false;
    }


    elem.m_map_pos = top.m_handle.FindNextChild(elem.m_map_pos);
    elem.m_handle  = top.m_handle.GetAt(elem.m_map_pos);
    m_ctx_stack.AddTail(elem);
    return true;
}

inline void CBKJsonCursor::EndChildren()
{
    assert(!m_ctx_stack.IsEmpty());
    assert(!m_is_error);
    RaiseIfError();
    _RaiseIfEmptyStack();


    m_ctx_stack.RemoveTailNoReturn();
}

inline bool CBKJsonCursor::IsEndOfChildren()
{
    assert(!m_ctx_stack.IsEmpty());
    assert(!m_is_error);
    RaiseIfError();
    _RaiseIfEmptyStack();


    // it's never to the end in write mode
    if (IsWriteMode())
        return false;


    if (m_ctx_stack.GetCount() <= 1)
        return true;



    CBKJsonCursorContext& top = m_ctx_stack.GetTail();
    if (NULL == top.m_map_pos)
        return true;


    return false;
}




inline CString CBKJsonCursor::GetName()
{
    assert(!m_ctx_stack.IsEmpty());
    assert(!m_is_error);
    RaiseIfError();
    _RaiseIfEmptyStack();



    CBKJsonCursorContext& top = m_ctx_stack.GetTail();



    return top.m_handle.GetName();
}





inline void CBKJsonCursor::BeginArray()
{
    assert(!m_ctx_stack.IsEmpty());
    assert(!m_is_error);
    RaiseIfError();
    _RaiseIfEmptyStack();


    CBKJsonCursorContext& top = m_ctx_stack.GetTail();


    if (IsWriteMode())
        top.m_handle.SetCount(0);


    if (0 == top.m_handle.Count())
    {
        m_ctx_stack.AddTail(CBKJsonCursorContext());
        return;
    }


    CBKJsonCursorContext elem;
    elem.m_array_index = 0;
    elem.m_handle      = top.m_handle[elem.m_array_index];
    m_ctx_stack.AddTail(elem);
}


inline void CBKJsonCursor::EndArray()
{
    assert(!m_ctx_stack.IsEmpty());
    assert(!m_is_error);
    RaiseIfError();
    _RaiseIfEmptyStack();


    m_ctx_stack.RemoveTailNoReturn();
}


inline void CBKJsonCursor::AppendArrayElem()
{
    assert(IsWriteMode());
    assert(!m_ctx_stack.IsEmpty());
    assert(!m_is_error);
    RaiseIfError();
    _RaiseIfEmptyStack();


    CBKJsonCursorContext elem = m_ctx_stack.RemoveTail();
    _RaiseIfEmptyStack();


    CBKJsonCursorContext& top = m_ctx_stack.GetTail();
    

    elem.m_array_index = top.m_handle.Count();
    elem.m_handle      = top.m_handle[elem.m_array_index];
    

    m_ctx_stack.AddTail(elem);
}


inline bool CBKJsonCursor::FindNextArrayElem()
{
    assert(!IsWriteMode());
    assert(!m_ctx_stack.IsEmpty());
    assert(!m_is_error);
    RaiseIfError();
    _RaiseIfEmptyStack();



    if (IsEndOfArrayElem())
        return false;



    CBKJsonCursorContext elem = m_ctx_stack.RemoveTail();
    _RaiseIfEmptyStack();


    CBKJsonCursorContext& top = m_ctx_stack.GetTail();


    // m_array_index is -1 before first call to FindNextArrayElem()
    if (elem.m_array_index + 1 >= top.m_handle.Count())
    {   // fill a null handle back to stack
        m_ctx_stack.AddTail(CBKJsonCursorContext());
        return false;
    }


    elem.m_array_index++;
    elem.m_handle = top.m_handle[elem.m_array_index];
    m_ctx_stack.AddTail(elem);
    return true;
}



inline bool CBKJsonCursor::IsEndOfArrayElem()
{
    assert(!m_ctx_stack.IsEmpty());
    assert(!m_is_error);
    RaiseIfError();
    _RaiseIfEmptyStack();


    // it's never to the end in write mode
    if (IsWriteMode())
        return false;


    if (m_ctx_stack.GetCount() <= 1)
        return true;



    CBKJsonCursorContext& top = m_ctx_stack.GetTail();
    if (top.m_array_index < 0)
        return true;


    POSITION pos = m_ctx_stack.GetTailPosition();
    m_ctx_stack.GetPrev(pos);
    CBKJsonCursorContext& sencond_top = m_ctx_stack.GetAt(pos);


    if (top.m_array_index >= sencond_top.m_handle.Count())
        return true;


    return false;
}


inline void CBKJsonCursor::SetLexValue(LPCWSTR lpszValue)
{
    assert(!m_ctx_stack.IsEmpty());
    assert(!m_is_error);
    RaiseIfError();
    _RaiseIfEmptyStack();



    CBKJsonCursorContext& top = m_ctx_stack.GetTail();



    top.m_handle = lpszValue;
}

inline CString CBKJsonCursor::GetLexValue()
{
    assert(!m_ctx_stack.IsEmpty());
    assert(!m_is_error);
    RaiseIfError();
    _RaiseIfEmptyStack();



    CBKJsonCursorContext& top = m_ctx_stack.GetTail();



    return (LPCWSTR)top.m_handle;
}




inline void CBKJsonCursor::_RaiseIfEmptyStack()
{
    RaiseIfError();
    if (m_ctx_stack.IsEmpty())
        RaiseError(ERROR_BJXDX_EMPTY_CONTEXT_STACK);
}









class CBKJsonCursorLexCast
{
public:
    void    InitAsLexCast(const CBKJsonCursor& tmpl);

    void    SetLexValue(LPCWSTR lpszValue);
    CString GetLexValue();

    template <class T_Value>
    void SetValue(T_Value& value)
    {
        m_cursour.OpenForWrite();
        m_cursour.BeginChild(L"tmp");
        m_cursour.AccessObject(value);
        m_cursour.EndChild();
    }

    template <class T_Value>
    void GetValue(T_Value& value)
    {
        m_cursour.OpenForRead();
        m_cursour.BeginChild(L"tmp");
        m_cursour.AccessObject(value);
        m_cursour.EndChild();
    }


public:
    CBKJsonCursor m_cursour;
};




inline void CBKJsonCursorLexCast::InitAsLexCast(const CBKJsonCursor& tmpl)
{
    return m_cursour.InitAsLexCast(tmpl);
}

inline void CBKJsonCursorLexCast::SetLexValue(LPCWSTR lpszValue)
{
    m_cursour.OpenForWrite();
    m_cursour.BeginChild(L"tmp");
    m_cursour.SetLexValue(lpszValue);
    m_cursour.EndChild();
}

inline CString CBKJsonCursorLexCast::GetLexValue()
{
    CString strResult;
    m_cursour.OpenForRead();
    m_cursour.BeginChild(L"tmp");
    strResult = m_cursour.GetLexValue();
    m_cursour.EndChild();

    return strResult;
}





NS_BKXDX_END

#endif//BKXDX_CURSOR_BKJSON_H