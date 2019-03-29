/*
This file is:
The zlib/libpng License
Copyright (c) 2012 freesftys<freesftys@gmail.com>

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from
the use of this software.

Permission is granted to anyone to use this software for any purpose, including
commercial applications, and to alter it and redistribute it freely, subject to
the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
       claim that you wrote the original software. If you use this software in
       a product, an acknowledgment in the product documentation would be
       appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
       misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.

http://opensource.org/licenses/Zlib

http://sourceforge.jp/projects/opensource/wiki/licenses%2Fzlib_libpng_license
*/

#include "NppHorizontalRuler.h"
#include "HorizontalRuler.h"
#include "PluginDefinition.h"

#include <Tchar.h>
#include <Commctrl.h>

extern NppData nppData;
extern FuncItem funcItem[];

HorizontalRuler mainHRuler;
HorizontalRuler subHRuler;

HWND mainTabHwnd;
WNDPROC mainOldWndProc;
HWND subTabHwnd;
WNDPROC subOldWndProc;

LRESULT onHorizontalRulerMessageProc( UINT Message, WPARAM wParam,
                                      LPARAM /* lParam */)
{
    switch ( Message )
    {
        case WM_SIZE:
            if ( wParam == SIZE_MINIMIZED )
                break;

            if ( mainHRuler.IsInit() )
            {
                mainHRuler.GetInitPos();
                mainHRuler.GetRuleArea();
                mainHRuler.SecureArea();
                mainHRuler.PaintRuler();
            }

            if ( subHRuler.IsInit() )
            {
                subHRuler.GetInitPos();
                subHRuler.GetRuleArea();
                subHRuler.SecureArea();
                subHRuler.PaintRuler();
            }

            return TRUE;
    }

    return TRUE;
}

void RulerWndProcSet()
{
    mainTabHwnd = FindWindowEx( nppData._nppHandle, NULL,
                                ( LPCWSTR )L"systabcontrol32", NULL );
    mainOldWndProc = ( WNDPROC )SetWindowLongPtr( mainTabHwnd, GWLP_WNDPROC,
                     ( LONG_PTR )RulerMainWndProc );

    subTabHwnd = FindWindowEx( nppData._nppHandle, mainTabHwnd,
                               ( LPCWSTR )L"systabcontrol32", NULL );
    subOldWndProc = ( WNDPROC )SetWindowLongPtr( subTabHwnd, GWLP_WNDPROC,
                    ( LONG_PTR )RulerSubWndProc );

    return;
}

void RulerWndProcUnset()
{
    SetWindowLongPtr( mainTabHwnd, GWLP_WNDPROC, ( LONG_PTR )mainOldWndProc );
    SetWindowLongPtr( subTabHwnd, GWLP_WNDPROC, ( LONG_PTR )subOldWndProc );

    return;
}

LRESULT CALLBACK RulerMainWndProc( HWND hwnd, UINT uMsg, WPARAM wParam,
                                   LPARAM lParam )
{
    if ( hwnd == subTabHwnd )
        return CallWindowProc( mainOldWndProc, hwnd, uMsg, wParam, lParam );

    switch ( uMsg )
    {
        case WM_DRAWITEM:

        case WM_PAINT:
            if ( mainHRuler.IsInit() )
                mainHRuler.PaintRuler();

            break;

        case WM_NCLBUTTONDBLCLK:
            if ( mainHRuler.IsInit() )
                mainHRuler.EdgeLine( LOWORD( lParam ), HIWORD( lParam ) );

            break;

        case WM_NCHITTEST:
            if ( mainHRuler.IsInit() )
            {
                if ( mainHRuler.HitDrawArea( LOWORD( lParam ), 
                     HIWORD( lParam ) ) )
                    return HTBORDER;
            }

            break;

        case TCM_GETIMAGELIST:
            if ( mainHRuler.IsInit() )
            {
                mainHRuler.GetInitPos();
                mainHRuler.GetRuleArea();
                mainHRuler.SecureArea();
                mainHRuler.PaintRuler();
            }

            break;

        default:
            break;
    }

    return CallWindowProc( mainOldWndProc, hwnd, uMsg, wParam, lParam );
}

LRESULT CALLBACK RulerSubWndProc( HWND hwnd, UINT uMsg, WPARAM wParam,
                                  LPARAM lParam )
{
    if ( hwnd == mainTabHwnd )
        return CallWindowProc( subOldWndProc, hwnd, uMsg, wParam, lParam );

    switch ( uMsg )
    {
        case WM_DRAWITEM:

        case WM_PAINT:
            if ( subHRuler.IsInit() )
                subHRuler.PaintRuler();

            break;

        case WM_NCLBUTTONDBLCLK:
            if ( subHRuler.IsInit() )
                subHRuler.EdgeLine( LOWORD( lParam ), HIWORD( lParam ) );

            break;

        case WM_NCHITTEST:
            if ( subHRuler.IsInit() )
            {
                if ( subHRuler.HitDrawArea( LOWORD( lParam ), 
                     HIWORD( lParam ) ) )
                    return HTBORDER;
            }

            break;

        case WM_MOUSEACTIVATE:
            return MA_ACTIVATE;
            break;

        case TCM_GETIMAGELIST:
            if ( subHRuler.IsInit() )
            {
                subHRuler.GetInitPos();
                subHRuler.GetRuleArea();
                subHRuler.SecureArea();
                subHRuler.PaintRuler();
            }

            break;
    }

    return CallWindowProc( subOldWndProc, hwnd, uMsg, wParam, lParam );
}

void enRuler()
{
    if ( mainHRuler.GetEnable() )
        return;

    mainHRuler.SetEnable( 1 );
    subHRuler.SetEnable( 1 );
    mainHRuler.SendSizeToMain();
}

void disRuler()
{
    if ( !mainHRuler.GetEnable() )
        return;

    mainHRuler.SetEnable( 0 );
    subHRuler.SetEnable( 0 );
    mainHRuler.SendSizeToMain();
}
