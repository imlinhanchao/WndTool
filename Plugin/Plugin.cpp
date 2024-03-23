#include "pch.h"
#include "plugin.h"
#include "SettingDlg.h"

void ShowTreeDlg(HWND hMainWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CSettingDlg* pDlg = new CSettingDlg();
	CWnd* pParent = CWnd::FromHandle(hMainWnd);
	BOOL nRet = pDlg->Create(IDD_DLG_SETTING, pParent);
	if (nRet)
	{
		pDlg->ShowWindow(SW_SHOW);
	}
}

CDialogEx* g_pSettingDlg = nullptr;

void Execute() {
	AfxMessageBox(_T("Plugin Execute"));
}

static PLUGIN_EXECUTE g_Execute[] = {
	{ _T("Plugin Setting"), &Execute, g_pSettingDlg },
};

const int g_nExecuteCount = sizeof(g_Execute) / sizeof(PLUGIN_EXECUTE);

void Loader(HWND hMain)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (hMain != NULL) {
		CWnd* pParent = CWnd::FromHandle(hMain);
		if (g_pSettingDlg == nullptr) g_pSettingDlg = new CSettingDlg(pParent);
		if (g_pSettingDlg->GetSafeHwnd() == NULL)
			g_pSettingDlg->Create(IDD_DLG_SETTING, pParent);
		g_Execute[0].pSettingDlg = g_pSettingDlg;
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