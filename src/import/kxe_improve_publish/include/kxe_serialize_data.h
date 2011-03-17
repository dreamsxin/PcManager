//////////////////////////////////////////////////////////////////////
///		@file		idl.h
///		@author		luopeng
///		@date		2008-9-8 09:07:58
///
///		@brief		用于http的中间封装
///		
//////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include <map>
#include "../json/json.h"
#include <sstream>
#include "../loki/TypeManip.h"
#include "../loki/static_check.h"
#include "../loki/Typelist.h"

#if _MSC_VER == 1200
#include "../loki/HierarchyGenerators.h"
#include "../common/kxeconv_atl.h"
#endif

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

/**
 @page kxehttp_json_data_struct http_json_data_struct
 @section kxe_http_json_input_one 客户端调用的Json数据结构:仅有一个参数的调用
 @code
 root :
 {
	 request:
	{
		param_count : "1"
		param_1
		{
			// any json data
		}
	}
 }
 @endcode
 @section kxe_http_json_input_two 客户端调用的Json数据结构:有两个参数的调用
 @code
 root :
 {
 	request:
	{
		param_count : "2"
		param_1
		{
			// any json data
		}
		param_2
		{
			// any json data
		}
	}
 }
 @endcode
 @section kxehttp_json_output 客户端返回的Json数据结构
 @code
 root
 {
	error:
	{
		error_code:0
		error_msg:""
	}
	result:
	{
		// any json data
	}
 }
 @endcode
 */

/**
 * @defgroup kxeipc_idl_interface_group KXEIPC IDL Interface
 * @{
 */

#if defined JSON_DLL
	#if _MSC_VER > 1200
		#pragma comment(lib, "json.lib")
	#else
		#pragma comment(lib, "jsonv6.lib")
	#endif

#else

	#if defined _DEBUG && defined _MT
		#if defined _DLL
			#pragma comment(lib, "lib_jsonD.lib")
		#else
			#pragma comment(lib, "lib_jsonMTD.lib")
		#endif // defined _MT
	#elif defined _MT
		#if defined _DLL
			#pragma comment(lib, "lib_json.lib")
		#else
			#pragma comment(lib, "lib_jsonMT.lib")
		#endif // defined _DLL
	#endif // #defined _DEBUG

#endif

#define DECLARE_VERSION(Major, Minor) \
	const static int nCurrentVersion = Major << 16 | Minor;\
	int nVersion;

// template
// <
// 	typename T1,
// 	typename T2
// >
// struct KxERequestT2Param
// {
// 	int nCount;
// 	T1 param1;
// 	T2 param2;
// };

_KAN_DOM_BEGIN

#if _MSC_VER > 1200
template
<
	typename _KDOMCursor, 
	typename T1,
	typename T2,
	template
	<
		typename,
		typename
	>
	class _Request
>
inline long KXMLDataExchange(_KDOMCursor& _domCur, _Request<T1, T2>& _obj, bool _bWrite)						 
{
	int _nRetCode = -1;

	KXE_JSON_DDX_MEMBER("param_count", nCount);
	KXE_JSON_DDX_MEMBER("param_1", param1);
	KXE_JSON_DDX_MEMBER("param_2", param2);

	return 0;
}
#endif

_KAN_DOM_END

// template
// <
// 	typename T
// >
// struct KxERequestT1Param
// {
// 	int nCount;
// 	T param1;
// };

_KAN_DOM_BEGIN

#if _MSC_VER > 1200
template
<
	typename _KDOMCursor, 
	typename T,
	template
	<
		typename
	>
	class _Request
>
inline long KXMLDataExchange(_KDOMCursor& _domCur, _Request<T>& _obj, bool _bWrite)						 
{
	int _nRetCode = -1;

	KXE_JSON_DDX_MEMBER("param_count", nCount);
	KXE_JSON_DDX_MEMBER("param_1", param1);

	return 0;
}
#endif

_KAN_DOM_END

/**
 * @brief 用于IPC-RPC调用时返回标志错误的结构
 */
typedef struct _KXE_ERROR
{
	int nError;  ///< 返回的错误码
	std::string strErrorMsg; ///< 返回的错误内容,可以为空
} KXE_ERROR;

KXE_JSON_DDX_BEGIN(KXE_ERROR)
	KXE_JSON_DDX_MEMBER("error_code", nError);
	KXE_JSON_DDX_MEMBER("error_msg", strErrorMsg);
KXE_JSON_DDX_END()

/**
 * @brief 用于作为空参数时的结构,程序员需要空参数时,可以直接引用此结构
 */
typedef struct _KXE_NULL_PARAMETER
{
	int nNullValue;
} KXE_NULL_PARAEMETER;

KXE_JSON_DDX_BEGIN(KXE_NULL_PARAEMETER)
	KXE_JSON_DDX_MEMBER_DEFAULT("NullValue", nNullValue, 0);
KXE_JSON_DDX_END()

