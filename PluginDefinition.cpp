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
#include "SettingsDlg.h"
#include "HorizontalRuler.h"
#include "NppHorizontalRuler.h"
#include "menuCmdID.h"
#include "Scintilla.h"

//
// The plugin data that Notepad++ needs
//
FuncItem funcItem[nbFunc];

//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;
HINSTANCE g_hInst;

bool g_bIsActiveHi    = false;
bool g_bBsUnindent    = false;
bool g_bIndentGuideLF = false;
bool g_bRulerStart    = false;
int  g_iEdgeModeOrig;
Sci_Position g_iEdgeColOrig;

extern HorizontalRuler mainHRuler;

//
// Initialize your plugin data here
// It will be called while plugin loading
void pluginInit( HANDLE /* hModule */ )
{

}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{

}

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{

    //--------------------------------------------//
    //-- STEP 3. CUSTOMIZE YOUR PLUGIN COMMANDS --//
    //--------------------------------------------//
    // with function :
    // setCommand(int index,                      // zero based number to indicate the order of command
    //            TCHAR *commandName,             // the command name that you want to see in plugin menu
    //            PFUNCPLUGINCMD functionPointer, // the symbol of function (function pointer) associated with this command. The body should be defined below. See Step 4.
    //            ShortcutKey *shortcut,          // optional. Define a shortcut to trigger this command
    //            bool check0nInit                // optional. Make this menu item be checked visually
    //            );
    setCommand( MENU_ENABLE,     TEXT( "&Enable all" ),       enableAll, NULL,
                false );                                      
    setCommand( MENU_SEPARATOR1, TEXT( "-SEPARATOR-" ),       NULL,      NULL,
                false );                                      
    setCommand( MENU_HIGHLIGHT,  TEXT( "Column &highlight" ), colHiToggle, NULL,
                false );                                      
    setCommand( MENU_RULER,      TEXT( "&Ruler" ),            rulToggle, NULL,
                false );                                      
    setCommand( MENU_SEPARATOR1, TEXT( "-SEPARATOR-" ),       NULL,      NULL,
                false );                                      
    setCommand( MENU_SETTINGS,   TEXT( "&Settings" ),         doSettings, NULL,
                false );
}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
    // Don't forget to deallocate your shortcut here
}

//
// This function help you to initialize your plugin commands
//
bool setCommand( size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc,
                 ShortcutKey *sk, bool check0nInit )
{
    if ( index >= nbFunc )
        return false;

    if ( !pFunc )
        return false;

    lstrcpy( funcItem[index]._itemName, cmdName );
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = sk;

    return true;
}

//----------------------------------------------//
//-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
//----------------------------------------------//
HWND getCurScintilla()
{
    int which = -1;
    ::SendMessage( nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0,
                   ( LPARAM )&which );
    return ( which == 0 ) ? nppData._scintillaMainHandle :
           nppData._scintillaSecondHandle;
}

void enableAll()
{
    if ( g_bIsActiveHi && mainHRuler.GetEnable() )
    {
        colHi( false );
        ruler( false );
        ::SendMessage( nppData._nppHandle, NPPM_SETMENUITEMCHECK,
                       funcItem[MENU_ENABLE]._cmdID, MF_UNCHECKED );
        ::SendMessage( nppData._nppHandle, NPPM_SETMENUITEMCHECK,
                       funcItem[MENU_HIGHLIGHT]._cmdID, MF_UNCHECKED );
        ::SendMessage( nppData._nppHandle, NPPM_SETMENUITEMCHECK,
                       funcItem[MENU_RULER]._cmdID, MF_UNCHECKED );
    }
    else
    {
        colHi( true );
        ruler( true );
        ::SendMessage( nppData._nppHandle, NPPM_SETMENUITEMCHECK,
                       funcItem[MENU_ENABLE]._cmdID, MF_CHECKED );
        ::SendMessage( nppData._nppHandle, NPPM_SETMENUITEMCHECK,
                       funcItem[MENU_HIGHLIGHT]._cmdID, MF_CHECKED );
        ::SendMessage( nppData._nppHandle, NPPM_SETMENUITEMCHECK,
                       funcItem[MENU_RULER]._cmdID, MF_CHECKED );
    }

}

void syncEnable()
{
    if ( g_bIsActiveHi && mainHRuler.GetEnable() )
        ::SendMessage( nppData._nppHandle, NPPM_SETMENUITEMCHECK,
                       funcItem[MENU_ENABLE]._cmdID, MF_CHECKED );
    else
        ::SendMessage( nppData._nppHandle, NPPM_SETMENUITEMCHECK,
                       funcItem[MENU_ENABLE]._cmdID, MF_UNCHECKED );
}

void doBufferSets()
{
    ::SendMessage( getCurScintilla(), SCI_SETBACKSPACEUNINDENTS, g_bBsUnindent, 0 );

    if ( g_bIndentGuideLF )
        ::SendMessage( getCurScintilla(), SCI_SETINDENTATIONGUIDES, SC_IV_LOOKFORWARD, 0 );
}

void resetEdge()
{
    // Reset original edge properties - Main
    ::SendMessage( nppData._scintillaMainHandle, SCI_SETEDGEMODE,
                   g_iEdgeModeOrig, 0 );
    ::SendMessage( nppData._scintillaMainHandle, SCI_SETEDGECOLUMN,
                   g_iEdgeColOrig, 0 );
    // Reset original edge properties - Secondary
    ::SendMessage( nppData._scintillaSecondHandle, SCI_SETEDGEMODE,
                   g_iEdgeModeOrig, 0 );
    ::SendMessage( nppData._scintillaSecondHandle, SCI_SETEDGECOLUMN,
                   g_iEdgeColOrig, 0 );
}

void colHiToggle()
{
    if ( g_bIsActiveHi )
    {
        colHi( false );
        ::SendMessage( nppData._nppHandle, NPPM_SETMENUITEMCHECK,
                       funcItem[MENU_HIGHLIGHT]._cmdID, MF_UNCHECKED );
    }
    else
    {
        colHi( true );
        ::SendMessage( nppData._nppHandle, NPPM_SETMENUITEMCHECK,
                       funcItem[MENU_HIGHLIGHT]._cmdID, MF_CHECKED );
    }

    syncEnable();
}

void colHi( bool enable )
{
    g_bIsActiveHi = enable;

    HWND hCurScintilla = getCurScintilla();
    if ( enable )
    {
        ::SendMessage( hCurScintilla, SCI_SETEDGEMODE, EDGE_LINE, 0 );
        setColHi( hCurScintilla );
    }
    else
    {
        resetEdge();
    }
}

void setColHi( HWND hCurScintilla )
{
    // Get current cursor position
    bool rect = ( bool )::SendMessage( hCurScintilla, SCI_SELECTIONISRECTANGLE,
                                       0, 0 );
    Sci_Position vsp = 0;

    if ( rect )
        vsp = ( Sci_Position )::SendMessage( hCurScintilla,
                                    SCI_GETRECTANGULARSELECTIONCARETVIRTUALSPACE, 0, 0 );
    else
    {
        int sel = ( int )::SendMessage( hCurScintilla, SCI_GETMAINSELECTION, 0, 0 );
        vsp = ( Sci_Position )::SendMessage( hCurScintilla,
                                    SCI_GETSELECTIONNCARETVIRTUALSPACE, sel, 0 );
    }

    // Set edge column to current cursort position
    Sci_Position caretPos = GetColumnCaretPos( hCurScintilla );
    caretPos += vsp;
    ::SendMessage( hCurScintilla, SCI_SETEDGEMODE, EDGE_LINE, 0 );
    ::SendMessage( hCurScintilla, SCI_SETEDGECOLUMN, caretPos, 0 );
}

void rulToggle()
{
    if ( mainHRuler.GetEnable() )
    {
        ruler( false );
        ::SendMessage( nppData._nppHandle, NPPM_SETMENUITEMCHECK,
                       funcItem[MENU_RULER]._cmdID, MF_UNCHECKED );

        if ( ! g_bIsActiveHi )
            resetEdge();
    }
    else
    {
        ruler( true );
        ::SendMessage( nppData._nppHandle, NPPM_SETMENUITEMCHECK,
                       funcItem[MENU_RULER]._cmdID, MF_CHECKED );
    }

    syncEnable();
}

Sci_Position GetColumnCaretPos( HWND hCurScintilla )
{
    int i;

    char *curLin;
    wchar_t *wideCurLin;
    char *ansiCurLin;
    int nWideExchange;
    int nAnsiExchange;

    Sci_Position nCaret, nLineLength;
    int nTabSpace;

    // HWND hCurScintilla = getCurScintilla();

    nLineLength = ( Sci_Position )SendMessage( hCurScintilla, SCI_GETCURLINE, 0,
                                      0 );
    nLineLength += 2;
    curLin = new char[nLineLength];
    nCaret = ( Sci_Position )SendMessage( hCurScintilla, SCI_GETCURLINE,
                                 nLineLength, ( LPARAM )curLin );
    nTabSpace = ( int )SendMessage( hCurScintilla, SCI_GETTABWIDTH, 0,
                                    0 );

    curLin[nCaret] = '\0';
    //utf8->utf16
    nWideExchange = MultiByteToWideChar( CP_UTF8, 0, curLin, -1, 0, 0 );
    wideCurLin = new wchar_t[nWideExchange];
    MultiByteToWideChar( CP_UTF8, 0, curLin, -1, wideCurLin, nWideExchange );
    nAnsiExchange = WideCharToMultiByte( CP_ACP, 0, wideCurLin, nWideExchange,
                                         0, 0, NULL, NULL );
    //utf16->ansi
    ansiCurLin = new char[nAnsiExchange];
    WideCharToMultiByte( CP_ACP, 0, wideCurLin, nWideExchange, ansiCurLin,
                         nAnsiExchange, NULL, NULL );

    //???????
    nCaret = 0;

    for ( i = 0; i < nAnsiExchange - 1; i++ )
    {
        if ( ansiCurLin[i] == '\t' )
            nCaret = ( ( nCaret / nTabSpace ) + 1 ) * nTabSpace;
        else
            nCaret++;
    }

    delete[] ansiCurLin;
    delete[] wideCurLin;
    delete[] curLin;
    ansiCurLin = NULL;
    wideCurLin = NULL;
    curLin = NULL;

    return nCaret;
}
