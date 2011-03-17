#pragma once

class KBasicServiceProvider;

class KBasicSPWrapper
{
public:
	KBasicSPWrapper(void);
	~KBasicSPWrapper(void);

	int Initialize();
	int Uninitialize();

private:
	KBasicServiceProvider* m_pBasicSP;
};
