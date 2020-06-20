#ifndef NPPHORIZONTALRULER_H
#define NPPHORIZONTALRULER_H

#include "HorizontalRuler.h"

LRESULT CALLBACK RulerMainWndProc( HWND , UINT , WPARAM ,LPARAM );
LRESULT CALLBACK RulerSubWndProc( HWND , UINT , WPARAM ,LPARAM );
LRESULT onHorizontalRulerMessageProc( UINT , WPARAM , LPARAM );

void ruler( bool enable );

#endif
