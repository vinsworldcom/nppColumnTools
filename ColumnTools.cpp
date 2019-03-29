//this file is part of notepad++
//Copyright (C)2003 Don HO <donho@altern.org>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "PluginDefinition.h"
#include "HorizontalRuler.h"
#include "NppHorizontalRuler.h"

extern FuncItem funcItem[nbFunc];
extern NppData nppData;

extern bool g_isActiveHi;

extern HorizontalRuler mainHRuler;
extern HorizontalRuler subHRuler;
extern HWND mainTabHwnd;
extern HWND subTabHwnd;

BOOL APIENTRY DllMain( HANDLE hModule,
                       DWORD  reasonForCall,
                       LPVOID /* lpReserved */ )
{
    switch ( reasonForCall )
    {
        case DLL_PROCESS_ATTACH:
            pluginInit( hModule );
            break;

        case DLL_PROCESS_DETACH:
            pluginCleanUp();
            break;

        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;
    }

    return TRUE;
}


extern "C" __declspec( dllexport ) void setInfo( NppData notpadPlusData )
{
    nppData = notpadPlusData;
    commandMenuInit();
}

extern "C" __declspec( dllexport ) const TCHAR * getName()
{
    return NPP_PLUGIN_NAME;
}

extern "C" __declspec( dllexport ) FuncItem * getFuncsArray( int *nbF )
{
    *nbF = nbFunc;
    return funcItem;
}


extern "C" __declspec( dllexport ) void beNotified( SCNotification *notifyCode )
{
    static int nHscrollPos;
    SCROLLINFO si;

    switch ( notifyCode->nmhdr.code )
    {
/*
        case SCN_PAINTED:
        {
            if ( g_isActiveHi )
                setColHi();
        }
        break;
*/

        case SCN_UPDATEUI:
        {
            if ( notifyCode->nmhdr.hwndFrom == nppData._scintillaMainHandle )
            {
                if ( mainHRuler.IsInit() )
                {
                    memset( &si, 0, sizeof( SCROLLINFO ) );
                    si.cbSize = sizeof( SCROLLINFO );
                    si.fMask = SIF_POS;// SIF_TRACKPOS POS????????
                    GetScrollInfo( nppData._scintillaMainHandle, SB_HORZ, &si );
                    nHscrollPos = si.nPos;
                    mainHRuler.PaintRuler();
                }
            }
            else if ( notifyCode->nmhdr.hwndFrom == nppData._scintillaSecondHandle )
            {
                if ( subHRuler.IsInit() )
                {
                    memset( &si, 0, sizeof( SCROLLINFO ) );
                    si.cbSize = sizeof( SCROLLINFO );
                    si.fMask = SIF_POS;// SIF_TRACKPOS POS????????
                    GetScrollInfo( nppData._scintillaSecondHandle, SB_HORZ, &si );
                    nHscrollPos = si.nPos;
                    subHRuler.PaintRuler();
                }
            }

            if ( g_isActiveHi )
                setColHi();
        }
        break;

        case SCN_SCROLLED:
        {
            if ( notifyCode->nmhdr.hwndFrom == nppData._scintillaMainHandle )
            {
                if ( mainHRuler.IsInit() )
                {
                    memset( &si, 0, sizeof( SCROLLINFO ) );
                    si.cbSize = sizeof( SCROLLINFO );
                    si.fMask = SIF_POS;// SIF_TRACKPOS POS????????
                    GetScrollInfo( nppData._scintillaMainHandle, SB_HORZ, &si );

                    if ( si.nPos != nHscrollPos )
                    {
                        nHscrollPos = si.nPos;
                        mainHRuler.PaintRuler();
                    }
                }
            }
            else if ( notifyCode->nmhdr.hwndFrom == nppData._scintillaSecondHandle )
            {
                if ( subHRuler.IsInit() )
                {
                    memset( &si, 0, sizeof( SCROLLINFO ) );
                    si.cbSize = sizeof( SCROLLINFO );
                    si.fMask = SIF_POS;// SIF_TRACKPOS POS????????
                    GetScrollInfo( nppData._scintillaSecondHandle, SB_HORZ, &si );

                    if ( si.nPos != nHscrollPos )
                    {
                        nHscrollPos = si.nPos;
                        subHRuler.PaintRuler();
                    }
                }
            }

            if ( g_isActiveHi )
                setColHi();
        }
        break;

        case NPPN_WORDSTYLESUPDATED:
        {
            if ( mainHRuler.IsInit() )
            {
                mainHRuler.GetRuleArea();
                mainHRuler.SecureArea();
                mainHRuler.PaintRuler();
            }

            if ( subHRuler.IsInit() )
            {
                subHRuler.GetRuleArea();
                subHRuler.SecureArea();
                subHRuler.PaintRuler();
            }

            if ( g_isActiveHi )
                setColHi();
        }
        break;

        case NPPN_READY:
        {
            RulerWndProcSet();
            mainHRuler.Init( nppData._nppHandle, nppData._scintillaMainHandle,
                             mainTabHwnd );
            subHRuler.Init( nppData._nppHandle, nppData._scintillaSecondHandle,
                            subTabHwnd );

            if ( mainHRuler.GetEnable() )
                SendMessage( nppData._nppHandle, NPPM_SETMENUITEMCHECK,
                             funcItem[MENU_RULER]._cmdID, TRUE );
            else
                SendMessage( nppData._nppHandle, NPPM_SETMENUITEMCHECK,
                             funcItem[MENU_RULER]._cmdID, FALSE );

            syncEnable();

            SendMessage( nppData._nppHandle, WM_SIZE, 0, 0 );
        }
        break;

        case NPPN_SHUTDOWN:
        {
            RulerWndProcUnset();
            commandMenuCleanUp();
        }
        break;

        default:
            return;
    }
}

// Here you can process the Npp Messages
// I will make the messages accessible little by little, according to the need of plugin development.
// Please let me know if you need to access to some messages :
// http://sourceforge.net/forum/forum.php?forum_id=482781
//
extern "C" __declspec( dllexport ) LRESULT messageProc( UINT Message,
        WPARAM wParam, LPARAM lParam )
{
    LRESULT result = TRUE;
    result = onHorizontalRulerMessageProc( Message, wParam, lParam );

    return result;
}

#ifdef UNICODE
extern "C" __declspec( dllexport ) BOOL isUnicode()
{
    return TRUE;
}
#endif //UNICODE
