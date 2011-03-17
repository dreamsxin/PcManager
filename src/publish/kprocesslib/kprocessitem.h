#pragma once

/*
对应XML文件中
<ProcessItem id="1" DisplayName="abc" Description="123" PathIndex="1" PathLevel="2" CompanyNameIndex="1" SignNameIndex="1" ProcessType="1" CanClose="" SoftId="" />
节点
*/

class KProcessItem
{
public:
    KProcessItem(void);
    ~KProcessItem(void);

    int PathLevel() const
    {
        return m_pathLevel;
    }
    void PathLevel(int val)
    {
        m_pathLevel = val;
    }

    std::wstring ProcessName() const
    {
        return m_processName;
    }
    void ProcessName(std::wstring val)
    {
        m_processName = val;
    }

    int ProcessItemId() const
    {
        return m_processItemId;
    }
    void ProcessItemId(int val)
    {
        m_processItemId = val;
    }

    std::wstring DisplayName() const
    {
        return m_displayName;
    }
    void DisplayName(std::wstring& val)
    {
        m_displayName = val;
    }

    std::wstring Desc() const
    {
        return m_desc;
    }
    void Desc(std::wstring val)
    {
        m_desc = val;
    }

    int PathIndex() const
    {
        return m_pathIndex;
    }
    void PathIndex(int val)
    {
        m_pathIndex = val;
    }

    int CompanyIndex() const
    {
        return m_companyIndex;
    }
    void CompanyIndex(int val)
    {
        m_companyIndex = val;
    }

    int SignNameIndex() const
    {
        return m_signNameIndex;
    }
    void SignNameIndex(int val)
    {
        m_signNameIndex = val;
    }

    int ProcessType() const
    {
        return m_processType;
    }
    void ProcessType(int val)
    {
        m_processType = val;
    }

    BOOL CanClose() const
    {
        return m_bCanClose;
    }
    void CanClose(BOOL val)
    {
        m_bCanClose = val;
    }

    std::wstring SoftId() const
    {
        return m_softId;
    }
    void SoftId(std::wstring val)
    {
        m_softId = val;
    }

    KProcessItem& operator=(const KProcessItem& right);

private:
    int m_processItemId;

    wstring m_processName;

    wstring m_displayName;

    wstring m_desc;

    int m_pathIndex;

    int m_pathLevel;

    int m_companyIndex;

    int m_signNameIndex;

    int m_processType;

    BOOL m_bCanClose;

    wstring m_softId;
};
