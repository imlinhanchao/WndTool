#pragma once


class CLogApp
{
public:
	CLogApp(void);
	~CLogApp(void);

	typedef enum { 
		FILE = 1 << 0, 
		PRINT = 1 << 1, 
		LDEBUG = 1 << 2, 
		LIST = 1 << 3, 
		ALL = 0xffffff
	} LOG_TYPE;

	static void SetList(CListBox* pList) { m_pListBox = pList; }
	static void Init(DWORD dwType, CString sPath=_T(""));
	static CString Write(const TCHAR* pszFormat, ...);
	static CString Debug(const TCHAR* pszFormat, ...);
	static CString Print(const TCHAR* pszFormat, ...);
	static CString List(const TCHAR* pszFormat, ...);
	static CString WriteFile(const TCHAR* pszFormat, ...);
private:
	static CString GetCurDirectory();

	static DWORD m_dwLogType;
	static CString m_sPath;
	static CListBox* m_pListBox;
};
