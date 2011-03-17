//////////////////////////////////////////////////////////////////////
///		@file		kxevipfun_id.h
///		@author		luopeng
///		@date		2009-9-8 14:07:58
///
///		@brief		VIP功能的ID定义
//////////////////////////////////////////////////////////////////////

#pragma once

/**
 * @defgroup kxe_vip_function_id_group KXEngine Function ID
 * @{
 */

/*
 * ID直接为16位，分别为高8位，与低8位
 * 0                  7                  15
 * ---------------------------------------
 * |   Reserved       |  VIPFunctionID   |
 * ---------------------------------------
 */

/// 病毒库下载
#define KXE_DWON_VIRUS_FUNCTION              0x00000001
#define KXE_DWON_VIRUS_FUNCTION_A            "0x00000001"
#define KXE_DWON_VIRUS_FUNCTION_W            L"0x00000001"
#define KXE_DWON_VIRUS_FUNCTION_T            _T("0x00000001")

/// 换肤功能
#define KXE_CHANGE_SKIN_FUNCTION             0x00000002
#define KXE_CHANGE_SKIN_FUNCTION_A           "0x00000002"
#define KXE_CHANGE_SKIN_FUNCTION_W           L"0x00000002"
#define KXE_CHANGE_SKIN_FUNCTION_T           _T("0x00000002")

/// 网银保护
#define KXE_PROTECT_BANK_FUNCTION            0x00000003
#define KXE_PROTECT_BANK_FUNCTION_A          "0x00000003"
#define KXE_PROTECT_BANK_FUNCTION_W          L"0x00000003"
#define KXE_PROTECT_BANK_FUNCTION_T          _T("0x00000003")

/// 流量管理
#define KXE_FLUX_MANAGEMENT_FUNCTION         0x00000004
#define KXE_FLUX_MANAGEMENT_FUNCTION_A       "0x00000004"
#define KXE_FLUX_MANAGEMENT_FUNCTION_W       L"0x00000004"
#define KXE_FLUX_MANAGEMENT_FUNCTION_T       _T("0x00000004")

/// 云查杀
#define KXE_CLOUD_SCAN_FUNCTION         	 0x00000005
#define KXE_CLOUD_SCAN_FUNCTION_A            "0x00000005"
#define KXE_CLOUD_SCAN_FUNCTION_W            L"0x00000005"
#define KXE_CLOUD_SCAN_FUNCTION_T            _T("0x00000005")

/// 金牌客服
#define KXE_GOLDEN_CUSTOMER_SERVIC           0x00000006
#define KXE_GOLDEN_CUSTOMER_SERVIC_A         "0x00000006"
#define KXE_GOLDEN_CUSTOMER_SERVIC_W         L"0x00000006"
#define KXE_GOLDEN_CUSTOMER_SERVIC_T         _T("0x00000006")

/**
 * @}
 */
