#pragma once

#ifdef PLUGIN_EXPORTS
#define PLUGIN_API __declspec(dllexport)
#else
#define PLUGIN_API __declspec(dllimport)
#endif

typedef void (*PLUGIN_EXECUTE_CALL)();

typedef struct _PLUGIN_EXECUTE
{
	TCHAR szName[64];
	PLUGIN_EXECUTE_CALL pExecute;
	CWnd* pSettingDlg;

}PLUGIN_EXECUTE, * PPLUGIN_EXECUTE;


extern "C" {
	PLUGIN_API void Loader(HWND hMain = NULL);
	PLUGIN_API int  GetExecutesCount();
	PLUGIN_API PPLUGIN_EXECUTE GetExecutes();

	PLUGIN_API void ShowTreeDlg(HWND hMainWnd);
}
