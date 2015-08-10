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

I did some experimenting with pluginInit() and the en/disColHi() functions 
and ended up putting the logic in all places since I can't figure out when 
pluginInit() is actually called.

There will be bugs!


CAVEATS

When I have the NppExec console window open on the bottom and then close it, 
I lose the windos tabs until I open a new window.

This doesn't work with multiple views (Move / Clone to "Other View").


DEPENDENCIES

The Makefile is written for gmake.exe from the MinGW distribution.  There 
is a Dev-Cpp project file (.dev) included.

I compiled with the MinGW released with Strawberry Perl version:

This is perl 5, version 18, subversion 1 (v5.18.1) built for 
MSWin32-x86-multi-thread-64int

g++ (release with patches / build 20130526 by strawberryperl.com) 4.7.3

  - I need to compile as 32-bit to match my Notepad++ install
  - I have UNICODE enabled to match my Notepad++ install
  
You can edit the Makefile to suit your needs.
