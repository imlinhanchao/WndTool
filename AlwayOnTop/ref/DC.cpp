#include "pch.h"
#include "DC.h"

namespace Easy {

void DC::DrawColor(CStatic* pStc, COLORREF clr)
{
	CDC* pDC = pStc->GetDC();
	CRect rcCli;
	pStc->GetClientRect(rcCli);
	rcCli.DeflateRect(1, 1, 1, 1);
	pDC->FillSolidRect(rcCli, clr);
}

}