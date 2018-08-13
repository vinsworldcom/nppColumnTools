NAME

Notepad++ Highlight Current Column
Author:  Michael J. Vincent


DESCRIPTION

This Notepad++ plugin provides highlighting of the current column in the 
same way the "Settings" -> "Preferences" -> "Editing" -> "Enable current 
line highlightig" option provides highlighting of the current line.

This is more of a proof of concept.  I wrote this in about 8 hours while 
looking at the demo plugin, the Scintilla documentation, other examples of 
working plugins and my little (to almost non-existent) experience with C/C++.  

To be fair, this plugin doesn't really "highlight" the current column; rather, 
uses the edge column marker to do the job.  Thus, if you're using the edge 
column marker, you "lose" it, but this plugin should remember the state on 
startup so when you disable the plugin, the original edge column marker state 
should be returned.


CAVEATS

I sometimes lose the document tabs until I use the menu or open a new window.  
Probably due to me using SCN_PAINT event to update the location of the column 
marker.

I'm using this plugin on and off when I need to align text on multiple rows 
using the vertical column highlight as a guide.  No major issues, but ...

There could be bugs!


DEPENDENCIES

The Makefile is written for gmake.exe from the MinGW distribution.

I've compiled it fine, but needed to statically include glibc so I could 
change my C compiler in my path.  This leads to a much larger DLL.

I compiled with MS Visual Studio Community 2017 and this seems to work 
OK.

[x86 Native Tools Command Prompt for VS 2017]
Configuration=Release
Platform=Win32

  msbuild
