# Notepad++ Column Tools

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

The ruler functionality is "borrowed" from the 
[NppHorizontalRuler](https://notepad-plus-plus.org/community/topic/13369/a-notepad-beautiful-ruler-plugin-finally) 
plugin.

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

## Usage

Plugins => Column Tools and select the option(s) you want.

### Column Highlight

To be fair, this plugin doesn't really "highlight" the current column; rather, 
uses the edge column marker to do the job.  Thus, if you're using the edge 
column marker, you "lose" it, but this plugin should remember the state on 
startup so when you disable the plugin, the original edge column marker state 
should be returned.

### Ruler

The horizonal ruler displays across the top of each tab in each Scintilla view 
in Notepad++ (both main and secondary, if visible).

#### Set Vertical Line

Double-click on the ruler to set a vertical line at that column.  If Column 
Highlight is enabled, the vertical line will not stay and will move with the 
cursor.  This is a Scintilla limitation and can not be "fixed" with this 
plugin.  To keep the vertical line in position, disable Column Highlight.

#### Multiple Vertical Lines (Scintilla EDGE_MULTILINE)

For [multiple vertical lines](https://www.scintilla.org/ScintillaDoc.html#SCI_MULTIEDGEADDLINE)
SHIFT+double-click in the ruler.  Again, if Column Highlight is enabled, 
the vertical lines will not stay and will move with the cursor.  This is a 
Scintilla limitation and can not be "fixed" with this plugin.  To keep the 
vertical lines in position, disable Column Highlight.  To remove all 
vertical lines, use CTRL+double-click in the ruler.  Again, this is a 
[Scintilla limitation](https://www.scintilla.org/ScintillaDoc.html#SCI_MULTIEDGECLEARALL); 
individual vertical lines cannot be removed, it's all or nothing.

**NOTE:** The ruler does not line up correctly unless 
[SC_TECHNOLOGY_DEFAULT](https://www.scintilla.org/ScintillaDoc.html#SCI_GETTECHNOLOGY) 
is set.

### Settings

#### Backspace Unindent

This implements the Scintilla 
[SCI_SETBACKSPACEUNINDENTS](https://www.scintilla.org/ScintillaDoc.html#SCI_SETBACKSPACEUNINDENTS)
feature.

#### Indent Guides LookForward

This implements the Scintilla 
[SCI_SETINDENTATIONGUIDES](https://www.scintilla.org/ScintillaDoc.html#SCI_SETINDENTATIONGUIDES)
feature to `SC_IV_LOOKFORWARD` which is supposedly "good for Python", but seems 
to be good for all languages as well despite what it says for `SC_IV_LOOKBOTH` 
being "good for most languages".
