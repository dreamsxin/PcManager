#pragma once
/*
	进程库解析类
	用来解析进程库xml文件

	思路: 从XML文件读取后，用tinyxml解析，并转换成内存数据。
	公司		→     CompanyMap
	路径		→     PathMap
	数字签名   →     SignMap
	进程信息   →     ProcessInfoMap
						内部有ProcessItemMap

	为了加速查询，还增加了ProcessItem的MultiMap来优化查询。
*/

using namespace std;

class KProcessInfo;
class KProcessItem;
class TiXmlDocument;
class TiXmlElement;
struct ProcessLibOverwriteRule;
enum KProcessType;

class ProcessLib
{
public:
	ProcessLib(void);
	~ProcessLib(void);

	// 获取进程类型字符串
	static wstring GetProcessTypeString(KProcessType enumProcessType);

	// 获取数字签名的签名人
	static wstring GetDigitalSignaturerName(LPCTSTR szFilePath);

	// 从路径中获取文件名
	static wstring GetFileName(LPCTSTR szFilePath);
	
	// 获取文件中包含的描述信息
	static wstring GetFileDesc(LPCTSTR szFilePath);

	// 获取2级目录
	static wstring GetTwoLevelPath(LPCTSTR szFilePath, int& nPathLevel);
	
	// 获取1级目录
	static wstring GetOneLevelPath(LPCTSTR szFilePath);
	
	// 获取公司名称
	static wstring GetCompanyName(LPCTSTR szFilePath);

	// 从xml文件载入
	BOOL Load(LPCTSTR filePath);

	// 从内存中载入
	BOOL Load(char* buffer, long nLen);

	// 保存
	BOOL Save(LPCTSTR filePath);

	// 临时用的代码
	void TempFun();

	// 合并
	BOOL Merge(ProcessLib* secondLib, ProcessLibOverwriteRule overWriteRule);
	BOOL MergeCompanyNameNode( map<int, wstring>& secondMap);
	BOOL MergeSignNameMap(map<int, wstring>& secondMap);
	BOOL MergePathMap(map<int,wstring>& secondMap);
	BOOL MergeProcessesMap(ProcessLib* secondLib,map<wstring, KProcessInfo>& secondMap, ProcessLibOverwriteRule& overWriteRule);

	BOOL FindInfo(LPCTSTR szFileName, KProcessItem& pInfo);

	// 通过id查路径名
	wstring GetPath(int id);
	// 通过路径查id
	int FindPathId(wstring path);

	// 通过id查公司名
	wstring GetCompanyName(int id);
	// 公共公司名查id
	int FindCompanyId(wstring company);

	// 通过id查签名人
	wstring GetSignName(int id);
	// 通过签名人查id
	int FindSignNameId(wstring signName);

	// 通过id获取信息
	KProcessItem GetProcessItem(int id);

	BOOL GetProcessInfo(wstring fileName, KProcessInfo& processInfo);

	// 添加
	int  AddPathNode(const wstring&  strPath);
	int  AddCompanyNameNode(const wstring&  strCompanyName);
	int  AddSignNameNode(const wstring& strSignName);
	BOOL AddPathNode(int id, const wstring&  strPath);
	BOOL AddCompanyNameNode(int id, const wstring&  strCompanyName);
	BOOL AddSignNameNode(int id, const wstring& strSignName);
	BOOL AddProcessInfo(wstring fileName, KProcessInfo& processInfo);
	BOOL AddProcessItem(wstring processName, KProcessItem& processItem);
	BOOL AddProcessItem(wstring processName, KProcessItem& processItem, ProcessLibOverwriteRule& overwriteRule);
	
	// 删除
	BOOL RemovePathNode(int id);
	BOOL RemoveCompanyNameNode(int id);
	BOOL RemoveSignNameNode(int id);
	BOOL RemoveProcessItem(int processItemId);

	// 修改
	BOOL ModifyPathNode(int id,  const wstring& strNewPath);
	BOOL ModifyCompanyNameNode(int id, const wstring& strNewCompanyName);
	BOOL ModifySignNameNode(int id, const wstring& newName);
	BOOL ModifyProcessItem(int id, KProcessItem& newProcessItem);
	
	// 清空
	BOOL ClearAll();
	BOOL ClearCompnayMap();
	BOOL ClearSignNameMap();
	BOOL ClearPathMap();
	BOOL ClearProcessMap();

	// 获取map
    BOOL GetCompanyNameMap(map<int, wstring>& mapCompanyName);
	BOOL GetPathMap(map<int, wstring>& mapPath);
	BOOL GetProcessMap(map<wstring, KProcessInfo>& mapProcesses);
	BOOL GetSignNameMap(map<int, wstring>& mapSignName);

	BOOL IsProcessNameExist(wstring strProcessName);

	int GetMaxProcessItemId();
	int GetProcessItemCount();

private:
	// 解析
	BOOL ParseXml();
	BOOL ParsePathsNode();
	BOOL ParseCompanyNamesNode();
	BOOL ParseSignNamesNode();
	BOOL ParseProcessesNode();
	BOOL ParseMaxProcessItemIdNode();
	BOOL ParseProcessItemNode(TiXmlElement* nodes, KProcessInfo& processItem);

	// 根据内存数据生成xml节点
	BOOL GenPathNodes(TiXmlElement* pRootNode);
	BOOL GenCompanyNameNodes(TiXmlElement* pRootNode);
	BOOL GenSignNameNodes(TiXmlElement* pRootNode);
	BOOL GenMaxProcessItemIdNode(TiXmlElement* pRootNode);
	BOOL GenProcessNodes(TiXmlElement* pRootNode);
	BOOL GenProcessItemNodes(TiXmlElement* pRootNode, KProcessInfo& processInfo);
	
	
	BOOL MergeIndexMap(map<int,wstring>* firstMap, map<int, wstring>* secondMap);
	BOOL IsMapValueExist(map<int, wstring>* mapToSearch, wstring& value);

private:
	map<int, wstring>   				m_mapPath;              // 路径
	map<int, wstring>   				m_mapCompanyNames;      // 公司名
	map<int, wstring>   				m_mapSignNames;         // 数字签名人
	map<wstring, KProcessInfo>			m_mapProcesses;         // 进程
	multimap<wstring, KProcessItem>     m_mapProcessItems;      // 进程项列表，为了优化查询
	
	int 								m_nMaxPathMapId;        // 路径Map中最大ID
	int 								m_nMaxCompanyNamesMapId;// 公司Map中最大ID
	int 								m_nMaxSignNamesMapId;   // 数字签名中最大ID
	int 								m_nMaxProcessItemMapId; // 进程Map中最大ID
	
	TiXmlDocument*						m_pXmlDoc;             
};
