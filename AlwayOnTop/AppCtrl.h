#pragma once

#ifdef ALWAYONTOP_EXPORTS
#define ALWAYONTOP_API __declspec(dllexport)
#else
#define ALWAYONTOP_API __declspec(dllimport)
#endif

typedef void (*PLUGIN_EXECUTE_CALL)();

typedef struct _PLUGIN_EXECUTE
{
	TCHAR szName[64];
	PLUGIN_EXECUTE_CALL pExecute;
	CWnd* pSettingDlg;

}PLUGIN_EXECUTE, *PPLUGIN_EXECUTE;

extern "C" {
	ALWAYONTOP_API void Loader(HWND hMain=NULL);
	ALWAYONTOP_API int  GetExecutesCount();
	ALWAYONTOP_API PPLUGIN_EXECUTE GetExecutes();
}


