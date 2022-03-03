// MyPic.cpp : 实现文件
//

#include "stdafx.h"
#include "AsmtoByte.h"
#include "MyPic.h"

HWND SmallestWindowFromPoint(POINT point)
{
	RECT rect, rcTemp;
	HWND hParent, hWnd, hTemp;

	hWnd = ::WindowFromPoint(point);
	if( hWnd != NULL ) {
		::GetWindowRect(hWnd, &rect);
		hParent = ::GetParent(hWnd);

		// Has window a parent ?
		if(hParent != NULL) {
			// Search down the Z-Order
			hTemp = hWnd;
			do {
				hTemp = ::GetWindow(hTemp, GW_HWNDNEXT);

				// Search window contains the point, hase the same parent, and is visible?
				::GetWindowRect(hTemp, &rcTemp);
				if(::PtInRect(&rcTemp, point) && ::GetParent(hTemp) == hParent && ::IsWindowVisible(hTemp)){
					// Is it smaller?
					if(((rcTemp.right - rcTemp.left) * (rcTemp.bottom - rcTemp.top)) < ((rect.right - rect.left) * (rect.bottom - rect.top))) {
						// Found new smaller window!
						hWnd = hTemp;
						::GetWindowRect(hWnd, &rect);
					}
				}
			} while(hTemp != NULL);
		}
	}

	return hWnd;
}


void CMyPic::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	HWND deskWnd = ::GetDesktopWindow();
	HDC deskDC = ::GetWindowDC(deskWnd);
	int oldRop2 = SetROP2(deskDC, R2_NOTXORPEN);

	CPoint pt;
	GetCursorPos(&pt);


	HPEN newPen = ::CreatePen(0, 3, RGB(125, 0, 125));
	HGDIOBJ oldPen = ::SelectObject(deskDC, newPen);


	::SetROP2(deskDC, oldRop2);
	::SelectObject( deskDC, oldPen);

	::DeleteObject(newPen);
	::ReleaseDC(deskWnd, deskDC);
	deskDC = NULL;


	CStatic::OnTimer(nIDEvent);
}
