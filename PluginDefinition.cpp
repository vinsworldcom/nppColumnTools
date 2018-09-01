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
#include "menuCmdID.h"

//
// The plugin data that Notepad++ needs
//
FuncItem funcItem[nbFunc];

//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;

bool g_isActiveHi = false;
bool g_isActiveRul = false;
int  g_iEdgeModeOrig;
int  g_iEdgeColOrig;

//
// Initialize your plugin data here
// It will be called while plugin loading
void pluginInit( HANDLE hModule )
{
}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
    disColHi();
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
    setCommand( 0, TEXT( "Column &highlight" ), highlight, NULL, true );
    setCommand( 1, TEXT( "Column &ruler" ), ruler, NULL, true );
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

void highlight()
{
    UINT state = ::GetMenuState(::GetMenu(nppData._nppHandle), funcItem[0]._cmdID, MF_BYCOMMAND);
    if (state & MF_CHECKED)
        disColHi();
    else
        enColHi();

    ::SendMessage(nppData._nppHandle, NPPM_SETMENUITEMCHECK, funcItem[0]._cmdID, !(state & MF_CHECKED));
}

void enColHi()
{
    if ( g_isActiveHi )
        return;

    g_isActiveHi = true;

    // Save original edge properties
    HWND hCurScintilla = getCurScintilla();
    g_iEdgeModeOrig = ::SendMessage( hCurScintilla, SCI_GETEDGEMODE, 0, 0 );
    g_iEdgeColOrig  = ::SendMessage( hCurScintilla, SCI_GETEDGECOLUMN, 0, 0 );

    // Debug
    // TCHAR szBuffer[100];
    // wsprintf( szBuffer, TEXT( "Mode = %i\n Column = %i" ), iEdgeModeOrig, iEdgeColOrig );
    // ::MessageBox( NULL, szBuffer, TEXT( "Column Highlight - SAVE" ), MB_OK );

    // Update menu
    HMENU hMenu = ::GetMenu( nppData._nppHandle );
    if ( hMenu )
        ::CheckMenuItem( hMenu, funcItem[0]._cmdID, MF_CHECKED );
}

void disColHi()
{
    if ( !g_isActiveHi )
        return;

    g_isActiveHi = false;

    // Reset original edge properties
    // HWND hCurScintilla = getCurScintilla();
    // ::SendMessage( hCurScintilla, SCI_SETEDGEMODE, iEdgeModeOrig, 0 );
    // ::SendMessage( hCurScintilla, SCI_SETEDGECOLUMN, iEdgeColOrig, 0 );

    // Reset original edge properties - Main
    ::SendMessage( nppData._scintillaMainHandle, SCI_SETEDGEMODE, g_iEdgeModeOrig, 0 );
    ::SendMessage( nppData._scintillaMainHandle, SCI_SETEDGECOLUMN, g_iEdgeColOrig, 0 );
    // Reset original edge properties - Secondary
    ::SendMessage( nppData._scintillaSecondHandle, SCI_SETEDGEMODE, g_iEdgeModeOrig, 0 );
    ::SendMessage( nppData._scintillaSecondHandle, SCI_SETEDGECOLUMN, g_iEdgeColOrig, 0 );

    // Debug
    // TCHAR szBuffer[100];
    // wsprintf( szBuffer, TEXT( "Mode = %i\n Column = %i" ), iEdgeModeOrig, iEdgeColOrig );
    // ::MessageBox( NULL, szBuffer, TEXT( "Column Highlight - RESET" ), MB_OK );

    // Update menu
    HMENU hMenu = ::GetMenu( nppData._nppHandle );
    if ( hMenu )
        ::CheckMenuItem( hMenu, funcItem[0]._cmdID, MF_UNCHECKED );
}

void setColHi()
{
    // Get current cursor position
    HWND hCurScintilla = getCurScintilla();
    int pos = ::SendMessage( hCurScintilla, SCI_GETCURRENTPOS, 0, 0 );
    int col = ::SendMessage( hCurScintilla, SCI_GETCOLUMN, pos, 0 );

    // Set edge column to current cursort position
    ::SendMessage( hCurScintilla, SCI_SETEDGECOLUMN, col, 0 );
}

void ruler()
{
    HWND hCurScintilla = getCurScintilla();

    UINT state = ::GetMenuState(::GetMenu(nppData._nppHandle), funcItem[1]._cmdID, MF_BYCOMMAND);
    if (state & MF_CHECKED)
    {
        g_isActiveRul = false;
        
        ::SendMessage(hCurScintilla, SCI_ANNOTATIONCLEARALL, 0, 0);
    }
    // Enable
    else
    {
        g_isActiveRul = true;

        int pos = ::SendMessage(hCurScintilla, SCI_GETCURRENTPOS, 0, 0);
        int lin = ::SendMessage(hCurScintilla, SCI_LINEFROMPOSITION, pos, 0);

        ::SendMessage(hCurScintilla, SCI_ANNOTATIONSETTEXT, lin, (LPARAM)"--- 0 ---|--- 10---|--- 20---|--- 30---|--- 40---|--- 50---|--- 60---|--- 70---|--- 80---|--- 90---|---100---|---110---|---120---|---130---|---140---|---150---|---160---|---170---|---180---|---190---|\n\
123456789|123456789|123456789|123456789|123456789|123456789|123456789|123456789|123456789|123456789|123456789|123456789|123456789|123456789|123456789|123456789|123456789|123456789|123456789|123456789|");
        /**
         * #define STYLE_DEFAULT 32
         * #define STYLE_LINENUMBER 33
         * #define STYLE_BRACELIGHT 34
         * #define STYLE_BRACEBAD 35
         * #define STYLE_CONTROLCHAR 36
         * #define STYLE_INDENTGUIDE 37
         * #define STYLE_CALLTIP 38
         */
        ::SendMessage(hCurScintilla, SCI_ANNOTATIONSETSTYLE, lin, STYLE_CALLTIP);
        ::SendMessage(hCurScintilla, SCI_ANNOTATIONSETVISIBLE, lin, 0);
    }

    ::SendMessage(nppData._nppHandle, NPPM_SETMENUITEMCHECK, funcItem[1]._cmdID, !(state & MF_CHECKED));
}
