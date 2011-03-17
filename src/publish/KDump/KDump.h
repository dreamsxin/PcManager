class KDump
{
public:
	static KDump s_Instance;
	static inline KDump & Instance()
	{
		return s_Instance;
	}
public:
	KDump();
	~KDump();

	BOOL Install();

private:
	CRITICAL_SECTION m_cs;
	BOOL m_bInited;
	BOOL m_bRegistered;
};