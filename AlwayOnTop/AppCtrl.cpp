#include "pch.h"
#include "AppCtrl.h"
#include "resource.h"
#include "SettingDlg.h"


CDialogEx g_SettingDlg;

CWnd* GetSettingDlg(CWnd* pParent)
{
	if (g_SettingDlg.GetSafeHwnd() == NULL)
		g_SettingDlg.Create(IDD_DLG_SETTING, pParent);

	return &g_SettingDlg;
}

