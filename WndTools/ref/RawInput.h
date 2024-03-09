#pragma once

namespace Easy {


class CRawInput
{
	CRawInput();
	~CRawInput();

public:
	typedef enum
	{
		HID = 0x01,
		KB = 0x02,
		MS = 0x04,
	} RAW_TYPE;

	static bool Register(HWND hWnd, WORD wRawType);
	static bool Remove(HWND hWnd, WORD wRawType);

private:
	static bool SetRawInput(HWND hWnd, WORD wRawType, bool bRegister);
};

}