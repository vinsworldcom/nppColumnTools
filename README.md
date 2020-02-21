# Notepad++ Column Tools

Author:  Michael J. Vincent

## Description

This Notepad++ plugin provides highlighting of the current column in the 
same way the "Settings" -> "Preferences" -> "Editing" -> "Enable current 
line highlighting" option provides highlighting of the current line.  It 
also provides a "ruler" showing column numbers much the same as line 
numbers from "Settings" -> "Preferences" -> "Editing" -> "Display line 
number".

This was more of a proof of concept.  I wrote the column highlighting in 
about 8 hours while looking at the demo plugin, the Scintilla documentation, 
other examples of working plugins and my little (to almost non-existent) 
experience with C/C++.

The ruler functionality is "borrowed" from the NppHorizontalRuler plugin 
discussed here:

https://notepad-plus-plus.org/community/topic/13369/a-notepad-beautiful-ruler-plugin-finally

To be fair, this plugin doesn't really "highlight" the current column; rather, 
uses the edge column marker to do the job.  Thus, if you're using the edge 
column marker, you "lose" it, but this plugin should remember the state on 
startup so when you disable the plugin, the original edge column marker state 
should be returned.  I tried experimenting with the SCI_MULTIEDGE* messages 
but they aren't in the Scintilla.h provided with the Notepad++ demo plugin.

## Compiling

I compiled with MS Visual Studio Community 2017 and this seems to work OK.

For 32-bit:
```
    [x86 Native Tools Command Prompt for VS 2017]
    C:\> set Configuration=Release
    C:\> set Platform=x86
    C:\> msbuild
```

For 64-bit:
```
    [x64 Native Tools Command Prompt for VS 2017]
    C:\> set Configuration=Release
    C:\> set Platform=x64
    C:\> msbuild
```

## Installation

Copy the:


+ 32-bit:  ./bin/ColumnTools.dll
+ 64-bit:  ./bin64/ColumnTools.dll

to the Notepad++ plugins folder:
  + In N++ <7.6, directly in the plugins/ folder
  + In N++ >=7.6, in a directory called ColumnTools in the plugins/ folder (plugins/ColumnTools/)
