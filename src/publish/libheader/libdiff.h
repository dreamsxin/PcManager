#pragma once

//
// 库差量合并
//
//@Remark
// 依赖于：src_3rdlib\open_vcdiff\lib\vcdcom or vcddec
//
//
HRESULT LibDiffCombine(LPCWSTR pOldLibPath, LPCWSTR pDiffLibPath, LPCWSTR pNewLibPath);