#pragma once

//#define _PRODUCT_KNSD_
#define _PRODUCT_KSAFE_

#ifdef _PRODUCT_KSAFE_
#define	VULFIX_OFFICE_XML_URL 	_T("http://dl.ijinshan.com/vul/office/%s")
#define BK_FILE_PREFIX _T("ksafe_")	// 下载的文件的前缀 

#endif


#ifdef _PRODUCT_KNSD_
#define	VULFIX_OFFICE_XML_URL 	_T("http://dl.sd.keniu.com/vul/office/%s")
#define BK_FILE_PREFIX _T("knsd_")	// 下载的文件的前缀 

#endif


