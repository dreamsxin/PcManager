#pragma once
/*
对应XML文件中
<Process Name="ksafe.exe">
	<ProcessItem id="1" DisplayName="abc" Description="123" PathIndex="1" PathLevel="2" CompanyNameIndex="1" SignNameIndex="1" ProcessType="1" CanClose="" SoftId="" />
	<ProcessItem id="2" DisplayName="abc" Description="45" PathIndex="2" PathLevel="2"  CompanyNameIndex="1" SignNameIndex="1" ProcessType="1" CanClose="" SoftId="" />
</Process>
节点

*/
class KProcessItem;
class KProcessInfo
{
public:
    KProcessInfo(void);
    ~KProcessInfo(void);

    BOOL AddProcessItem(KProcessItem& processItem);
    BOOL GetProcessItemsMap(std::map<int, KProcessItem>& mapProcessItems);

    std::wstring ProcessName() const;

    void ProcessName(std::wstring val);

    BOOL FindProcessItem(int pathIndex, int companyIndex, int signNameIndex);

	BOOL FindProcessItem(int companyIndex, int signNameIndex, vector<KProcessItem>& processItem);

    BOOL RemoveProcessItem(int processItemId);

private:
    std::map<int, KProcessItem> m_mapProcessitems;

    std::wstring m_processName;

};
