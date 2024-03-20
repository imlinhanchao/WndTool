#pragma once
#include <afx.h>

namespace Plugin {
	typedef void (*PLUGIN_EXECUTE_CALL)();

	typedef struct _PLUGIN_EXECUTE
	{
		TCHAR szName[64];
		PLUGIN_EXECUTE_CALL pExecute;
		CWnd* pSettingDlg;

	}PLUGIN_EXECUTE, * PPLUGIN_EXECUTE;

	typedef void (*LOADER)(HWND);
	typedef int  (*GETEXECUTESCOUNT)();
	typedef PPLUGIN_EXECUTE (*GETEXECUTES)();
}