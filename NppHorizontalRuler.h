#ifndef NPPHORIZONTALRULER_H
#define NPPHORIZONTALRULER_H

#include "HorizontalRuler.h"

HorizontalRuler mainHRuler;
HorizontalRuler subHRuler;

HWND mainTabHwnd;
WNDPROC mainOldWndProc;
HWND subTabHwnd;
WNDPROC subOldWndProc;

LRESULT CALLBACK RulerMainWndProc( HWND , UINT , WPARAM ,LPARAM );
LRESULT CALLBACK RulerSubWndProc( HWND , UINT , WPARAM ,LPARAM );
LRESULT onHorizontalRulerMessageProc( UINT , WPARAM , LPARAM );

#endif
