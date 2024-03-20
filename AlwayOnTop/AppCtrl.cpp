#include "pch.h"
#include "AppCtrl.h"
#include "resource.h"
#include "SettingDlg.h"
#include "CBorderDlg.h"

CDialogEx g_SettingDlg;

void Execute() {
	HWND hWnd = GetForegroundWindow();
	CWnd* pWnd = CWnd::FromHandle(hWnd);
	if (hWnd == NULL || pWnd == NULL)
		return;
	CBorderDlg dlg;
	dlg.Create(IDD_DLG_BORDER, pWnd);
	dlg.Init(hWnd);
}

static PLUGIN_EXECUTE g_Execute[] = {
	{ _T("Alway On Top"), &Execute, &g_SettingDlg },
};

const int g_nExecuteCount = sizeof(g_Execute) / sizeof(PLUGIN_EXECUTE);

void Loader(HWND hMain)
{
	if (hMain != NULL) {
		CWnd* pParent = CWnd::FromHandle(hMain);
		if (g_SettingDlg.GetSafeHwnd() == NULL)
			g_SettingDlg.Create(IDD_DLG_SETTING, pParent);
	}
}

int GetExecutesCount()
{
	return g_nExecuteCount;
}

PPLUGIN_EXECUTE GetExecutes()
{
	return g_Execute;
}

