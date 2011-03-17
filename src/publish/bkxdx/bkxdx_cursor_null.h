/**
* @file    bkxdx_cursor_null.h
* @brief   ...
* @author  bbcallen
* @date    2009-11-20 15:48
*/

#ifndef BKXDX_CURSOR_NULL_H
#define BKXDX_CURSOR_NULL_H

#include "bkxdx_base.h"
#include "bkxdx_lexvalue.h"

NS_BKXDX_BEGIN

class CNullCursor
{
public:
    void Prepare(LPCSTR root_name, bool write_to_doc);


    void BeginChild(LPCSTR name);
    void EndChild();




    template <class T_Value>
    void SetObject(T_Value& value) {ObjectExchange(*this, value, true);}

    template <class T_Value>
    void GetObject(T_Value& value) {ObjectExchange(*this, value, false);}






    template <class T_Value>
    void SetFinalObject(T_Value& value) {ObjectExchangeDefault(*this, value, true);}

    template <class T_Value>
    void GetFinalObject(T_Value& value) {ObjectExchangeDefault(*this, value, false);}


    void SetFinalObject(CLexValue& value) {;}

    void GetFinalObject(CLexValue& value) {;}

};

NS_BKXDX_END

#endif//BKXDX_CURSOR_NULL_H