#pragma once


#define KSAFE_RUN_XML_ROOT			_T("xml")
#define KSAFE_RUN_XML_ITEM			_T("item")
#define KSAFE_RUN_XML_ITEM_ID		_T("id")
#define KSAFE_RUN_XML_ITEM_TYPE		_T("type")
#define KSAFE_RUN_XML_ITEM_NAME		_T("name")
#define KSAFE_RUN_XML_ITEM_DESC		_T("desc")
#define KSAFE_RUN_XML_ITEM_WARNING	_T("warning")
#define KSAFE_RUN_XML_ITEM_DISPLAY	_T("display")
#define KSAFE_RUN_XML_ITEM_APPNAME	_T("appname")
#define KSAFE_RUN_XML_ITEM_ADVICE	_T("advice")
#define KSAFE_RUN_XML_ITEM_SYSTEM	_T("system")
#define KSAFE_RUN_XML_ITEM_NODELAY	_T("nodelay")
#define KSAFE_RUN_XML_ITEM_REGPATH	_T("regpath")
#define KSAFE_RUN_XML_ITEM_SHOW_REMAINS		_T("show_remain")
#define	KSAFE_RUN_XML_ITEM_EXTRA_FLAG		_T("extra_flag")
#define KSAFE_RUN_XML_ITEM_OPENRATE			_T("openrate")
#define KSAFE_RUN_XML_ITEM_USERNUM			_T("usercount")

class KSRunInfo
{
public:
	KSRunInfo()
	{
		nItemId = 0;
		nType	= 0;
		nAdvice	= 0;
		bNoDealyShow = FALSE;
		bSystem	= FALSE;
		bRegPath = FALSE;
		bShowRemain = TRUE;
		nExtraFlag = 0x00;
		nUserCount = 0;
	}

	DWORD		nItemId;		//每项惟一ID
	DWORD		nType;			//类型
	CString		strName;		//名字
	CString		strDesc;		//描述
	CString		strAppName;		//对应的APP的名字
	CString		strDisplay;		//界面中显示的名字
	DWORD		nAdvice;		//建议操作
	CString		strWarning;		//操作警告
	BOOL		bNoDealyShow;	//不显示延时启动
	BOOL		bSystem;		//是否是系统项
	BOOL		bRegPath;		//半匹配AppName
	BOOL		bShowRemain;	//如果是残留项，是否显示
	DWORD		nExtraFlag;		//1.0表示是否可以禁用，其他值保留使用
	CString		strOpenRate;	//开启率,库里保存百分比中的分数
	DWORD		nUserCount;		//用户人数
};

//一键优化中，系统设置相关项目的ID标记，若dwID & 0x80000，则代表是系统设置优化项目的ID 
#define KSAFE_ONEKEY_OPT_SYSCFG_ID_FLAG			0x80000

//[系统设置优化]注册表库字段定义：
#define KSAFE_REG_SYSOPT_XML_ROOT				_T("xml")
#define KSAFE_REG_SYSOPT_XML_ITEM				_T("item")
#define KSAFE_REG_SYSOPT_XML_ITEM_ID			_T("id")
#define KSAFE_REG_SYSOPT_XML_ITEM_TYPE			_T("type")
#define KSAFE_REG_SYSOPT_XML_ITEM_DESC			_T("desc")
#define KSAFE_REG_SYSOPT_XML_ITEM_VISTA			_T("nt6")
#define KSAFE_REG_SYSOPT_XML_ITEM_REGKEY		_T("regkey")
#define KSAFE_REG_SYSOPT_XML_ITEM_VALUE_TYPE	_T("value_type")
#define KSAFE_REG_SYSOPT_XML_ITEM_DEF_VALUE		_T("def_value")
#define KSAFE_REG_SYSOPT_XML_ITEM_OPT_VALUE		_T("opt_value")
#define KSAFE_REG_SYSOPT_XML_ITEM_COMPARE		_T("compare_type")

//比较类型
#define KSAFE_REGSYSOPT_COMPARE_TYPE_EQUAL	1
#define KSAFE_REGSYSOPT_COMPARE_TYPE_LESS	2
#define KSAFE_REGSYSOPT_COMPARE_TYPE_MORE	3

//如果某个优化项需要改动多于一处的注册表，这些注册表键用“;”隔开，同样的，它们的默认值和优化后的值也用“;”隔开
#define KSAFE_REGSYSOPT_SEPARATOR			_T(";")

#define KSAFE_REGSYSOPT_BACKSLASH			_T('\\')

//如果系统中默认不存在该键值，库中用NULL标记
#define KSAFE_REGSYSOPT_KEY_NULL			_T("NULL")

//注册表键值的类型
#define KSAFE_REG_VALUE_TYPE_REG_NONE		REG_NONE		//0
#define KSAFE_REG_VALUE_TYPE_REG_SZ			REG_SZ			//1
#define KSAFE_REG_VALUE_TYPE_REG_EXPAND_SZ	REG_EXPAND_SZ	//2
#define KSAFE_REG_VALUE_TYPE_REG_BINARY		REG_BINARY		//3
#define KSAFE_REG_VALUE_TYPE_REG_DWORD		REG_DWORD		//4

//注册表根键类型
#define KSAFE_REG_ROOT_HKCU					_T("HKCU")
#define KSAFE_REG_ROOT_HKLM					_T("HKLM")

class KSysotpRegInfo
{
public:
	KSysotpRegInfo()
	{
		nID		= 0;
		nType	= 0;
		bNt6OnlyHave = FALSE;
		nRegValueType= KSAFE_REG_VALUE_TYPE_REG_SZ;
		nCompareType = 0;
	}

	DWORD		nID;				//每项惟一ID
	DWORD		nType;				//类型，根据类型决定界面上显示的类别，如“开关机加速”，“系统性能与稳定”，“通用优化”等.
	BOOL		bNt6OnlyHave;		//该优化项所适用的操作系统版本。0，通用。1，nt6独有。2，nt5独有。
	CString		strDesc;			//该优化项的描述
	CString		strRegKey;			//对应的注册表键
	CString		strDefaultValue;	//默认值
	CString		strOptimizedValue;	//优化后的值
	DWORD		nRegValueType;		//注册表值的类型
	DWORD		nCompareType;		//比较类型。1：是否等于，2：是否大于，3：是否小于
};

class CKSafeSysoptRegInfo
{
public:
	CKSafeSysoptRegInfo()
	{
		m_nID	= 0;
		m_nType = 1;
		m_bIsNt6OnlyHave = FALSE;
		m_hKeyRoot		 = HKEY_CURRENT_USER;
		m_nRegValueType	 = KSAFE_REG_VALUE_TYPE_REG_SZ;
		m_nCompareType   = KSAFE_REGSYSOPT_COMPARE_TYPE_EQUAL;
	}

	~CKSafeSysoptRegInfo(){}

	BOOL IsNt6OnlyHave()
	{
		return m_bIsNt6OnlyHave;
	}

	BOOL IsCanOptimize(CString strSysValue, CString strOptimizeValue)
	{
		BOOL bRet = FALSE;
		if (m_nCompareType == KSAFE_REGSYSOPT_COMPARE_TYPE_EQUAL)
		{
			bRet = (strSysValue.CompareNoCase(strOptimizeValue) != 0);
			goto Exit0;
		}
		else if (m_nCompareType == KSAFE_REGSYSOPT_COMPARE_TYPE_LESS)
		{
			bRet = (_wtoi(strSysValue.GetString()) < _wtoi(strOptimizeValue.GetString()));
			goto Exit0;
		}
		else if (m_nCompareType == KSAFE_REGSYSOPT_COMPARE_TYPE_MORE)
		{
			bRet = (_wtoi(strSysValue.GetString()) > _wtoi(strOptimizeValue.GetString()));
			goto Exit0;
		}
		else
		{
			goto Exit0;
		}
Exit0:
		return bRet;
	}

	LPCTSTR GetTitleFormatStr()
	{
		if (m_nType == 1)		
		{
			return _T("开关机加速优化项（%d）");
		}
		else if (m_nType == 2)
		{
			return _T("系统运行加速优化项（%d）");
		}
		else if (m_nType == 3)
		{
			return _T("系统稳定优化项（%d）");
		}
		else if (m_nType == 4)
		{
			return _T("通用优化项（%d）");
		}
		else if (m_nType == 5)
		{
			return _T("文件打开速度优化（%d）");
		}
		else if (m_nType == 6)
		{
			return _T("网速优化（%d）");
		}
		else
		{	//没有定义的类别也归入“通用优化项”
			return _T("通用优化项（%d）");
		}
	}

	LPCTSTR GetTitleStr()
	{
		if (m_nType == 1)		
		{
			return _T("开关机加速优化项");
		}
		else if (m_nType == 2)
		{
			return _T("系统运行加速优化项");
		}
		else if (m_nType == 3)
		{
			return _T("系统稳定优化项");
		}
		else if (m_nType == 4)
		{
			return _T("通用优化项");
		}
		else if (m_nType == 5)
		{
			return _T("文件打开速度优化");
		}
		else if (m_nType == 6)
		{
			return _T("网速优化");
		}
		else
		{	//没有定义的类别也归入“通用优化项”
			return _T("通用优化项");
		}
	}

public:
	DWORD		m_nID;					//ID
	DWORD		m_nType;				//类别
	BOOL		m_bIsNt6OnlyHave;		////该优化项所适用的操作系统版本。0，通用。1，nt6独有。2，nt5独有。
	CString		m_strDesc;				//描述
	HKEY		m_hKeyRoot;				//注册表根键
	CString		m_strSubKey;			//注册表子键
	CString 	m_strValueName;			//注册表键名
	DWORD		m_nRegValueType;		//注册表类型
	CString		m_strDefalutValue;		//库中保存的系统默认值
	CString		m_strOptimizedValue;	//优化后的值
	DWORD		m_nCompareType;			//比较类型
	CString		m_strInitValue;			//枚举到的系统初始值
};