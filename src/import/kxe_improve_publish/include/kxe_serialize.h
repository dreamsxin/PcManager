//////////////////////////////////////////////////////////////////////
///		@file		kxe_serialize.h
///		@author		luopeng
///		@date		2008-9-23 09:07:58
///	
///		@brief		
//////////////////////////////////////////////////////////////////////
#pragma once
#include "../kxeevent/kxeeventid_def.h"
#include "../kxebase/kxesubsystemid_def.h"
#include "../json/json.h"
#include "../DOMUtil/JSONUtil/KJSONDataAccess.h"
#include "../scom/scom/scomhelper.h"
#include "../loki/static_check.h"
#include <string>

_KAN_DOM_BEGIN

// UUID
template<class _KDOMCursor>
inline long KXMLDataExchange(_KDOMCursor& domCur, UUID& value, bool bWrite)
{
	std::wstringstream ws;
	if (bWrite)
	{
		ws << value;
		return KXMLDataExchange(domCur, ws.str(), bWrite);
	}
	else
	{
		std::wstring strValue;
		int nRet = KXMLDataExchange(domCur, strValue, bWrite);
		if (nRet == 0)
		{
			ws.str(strValue);
			ws >> value;
		}
		return nRet;
	}
}

#if _MSC_VER > 1200
template<class _KDOMCursor, class T>
inline long KXMLDataExchange(_KDOMCursor& domCur, T& value, bool bWrite)
{
	STATIC_CHECK(0, TypeNotDefined_KJson);
	return 0;
}
#endif

_KAN_DOM_END

/**
 * @defgroup kxe_json_serialize_marco_group KXEngine Json Serialize Macro
 * @{
 */

/**
* @brief 开始定义将enum数据结构与Json传输数据之间进行序列化与反序列化的中间数据 
* @param[in] _enum 需要定义的枚举类型的中间数据结构结构
*/
#define KXE_JSON_DDX_ENUM(_enum) \
	inline std::ostream& operator<<(std::ostream& os, const _enum& enumValue)\
	{\
		return os << (const int&)enumValue;\
	}\
	inline std::istream& operator>>(std::istream& is, _enum& enumValue)\
	{\
		return is >> (int&)enumValue;\
	}\
	KDX_ENUM_DEFINE(_enum)

template
<
	typename T
>
struct KxERequestT1Param
{
	int nCount;
	T param1;
};

template
<
	typename T1,
	typename T2
>
struct KxERequestT2Param
{
	int nCount;
	T1 param1;
	T2 param2;
};

#if _MSC_VER == 1200

#define KXE_JSON_REQUEST_PARAM_1(_class) \
	_KAN_DOM_BEGIN\
	template<typename _KDOMCursor>\
	inline long KXMLDataExchange(_KDOMCursor& _domCur, KxERequestT1Param<_class>& _obj, bool _bWrite)\
	{\
		int _nRetCode = -1;\
		KXE_JSON_DDX_MEMBER("param_count", nCount);\
		KXE_JSON_DDX_MEMBER("param_1", param1);\
		return 0;\
	}\
	_KAN_DOM_END

#define KXE_JSON_REQUEST_PARAM_2(_class1, _class2) \
	_KAN_DOM_BEGIN\
	template<typename _KDOMCursor>\
	inline long KXMLDataExchange(_KDOMCursor& _domCur, KxERequestT2Param<_class1, _class2>& _obj, bool _bWrite)\
	{\
		int _nRetCode = -1;\
		KXE_JSON_DDX_MEMBER("param_count", nCount);\
		KXE_JSON_DDX_MEMBER("param_1", param1);\
		KXE_JSON_DDX_MEMBER("param_2", param2);\
		return 0;\
	}\
	_KAN_DOM_END

#endif

#if _MSC_VER == 1200
#define KXE_JSON_DDX_BEGIN(_class) \
	KXE_JSON_REQUEST_PARAM_1(_class)\
	KDX_BEGIN(_class)
#else
/**
 * @brief 开始定义将数据结构与Json传输数据之间进行序列化与反序列化的中间数据 
 * @param[in] _class 需要定义的中间数据结构
 */
#define KXE_JSON_DDX_BEGIN(_class) KDX_BEGIN(_class)
#endif

/**
 * @brief 定义将数据结构与Json传输数据之间进行序列化与反序列化的中间数据中的数据成员 
 * @param[in] _name json中存储的数据名
 * @param[in] _member json中存储的数据
 */
#define KXE_JSON_DDX_MEMBER(_name, _member) KDX_OBJ_NM(_name, _member);

/**
 * @brief 定义将数据结构与Json传输之间进行序列化与反序列化的中间数据中的数据成员,如果没有找到值,则用_Default填充 
 * @param[in] _name json中存储的数据名
 * @param[in] _member json中存储的数据
 * @param[in] _default 在传入的数据没有指定的值时,使用_default填充
 */
#define KXE_JSON_DDX_MEMBER_DEFAULT(_name, _member, _default) KDX_OBJ_EX(_name, _member, _default);

/**
 * @brief 结束定义将数据结构与配置之间进行序列化与反序列化的中间数据 
 */
#define KXE_JSON_DDX_END() KDX_END()

#include "kxe_serialize_data.h"

#if _MSC_VER == 1200

KXE_JSON_REQUEST_PARAM_1(long);
KXE_JSON_REQUEST_PARAM_1(short);
KXE_JSON_REQUEST_PARAM_1(int);
KXE_JSON_REQUEST_PARAM_1(char);
KXE_JSON_REQUEST_PARAM_1(unsigned long);
KXE_JSON_REQUEST_PARAM_1(unsigned short);
KXE_JSON_REQUEST_PARAM_1(unsigned int);
KXE_JSON_REQUEST_PARAM_1(unsigned char);
KXE_JSON_REQUEST_PARAM_1(std::string);
KXE_JSON_REQUEST_PARAM_1(std::wstring);
KXE_JSON_REQUEST_PARAM_1(UUID);


KXE_JSON_REQUEST_PARAM_2(std::wstring, std::wstring);
KXE_JSON_REQUEST_PARAM_2(unsigned int, std::wstring);
#endif

 /**
  * @}
  */
