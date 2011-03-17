/********************************************************************
	created:	2010/11/17
	created:	17:11:2010   11:21
	author:		Jiang Fengbing
	
	purpose:	金山卫士ARP防火墙控制设置接口
*********************************************************************/
#pragma once

//////////////////////////////////////////////////////////////////////////

#include <unknwn.h>

//////////////////////////////////////////////////////////////////////////

[
    uuid(79865ec3-8708-4835-b20c-758f5eeb25b4)
]
interface IArpFwSetting : IUnknown
{
    /*
     * @brief 判断ARP防火墙是否安装
     */
    virtual STDMETHODIMP HasInstalled(BOOL* pbInstalled) = 0;

    /*
     * @brief 安装ARP防火墙
     */
    virtual STDMETHODIMP InstallArpFw() = 0;

    /*
     * @brief 卸载ARP防火墙
     */
    virtual STDMETHODIMP UnInstallArpFw() = 0;

    /*
     * @brief 启动/关闭, ARP防火墙
     */
    virtual STDMETHODIMP EnableArpFw(BOOL bEnable) = 0;

    /*
     * @brief 判断ARP防火墙是否开启
     */
    virtual STDMETHODIMP IsArpFwEnable(BOOL* pbEnable) = 0;

    /*
     * @brief 显示ARP防火墙的设置
     */
    virtual STDMETHODIMP ShowSetting(HWND hParent) = 0;

    /*
     * @brief 获得最后一条日志
     */
    virtual STDMETHODIMP GetLastLog(int nLog, wchar_t* pLog) = 0;
};

//////////////////////////////////////////////////////////////////////////
