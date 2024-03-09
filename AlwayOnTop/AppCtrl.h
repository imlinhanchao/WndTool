#pragma once

#ifdef ALWAYONTOP_EXPORTS
#define ALWAYONTOP_API __declspec(dllexport)
#else
#define ALWAYONTOP_API __declspec(dllimport)
#endif

extern "C" {
	ALWAYONTOP_API CWnd* GetSettingDlg(CWnd* pParent);
}


