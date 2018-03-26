# Project: NppColHighLight
# Makefile created by Dev-C++ 5.9.2

PROJECT    = ColHighLight

DEFINE     = -DBUILDING_DLL=1 -DUNICODE
DLLRES     = $(PROJECT)_private.res
OBJ        = $(PROJECT).o PluginDefinition.o $(DLLRES)
LINKOBJ    = $(PROJECT).o PluginDefinition.o $(DLLRES)
LIBS       = -static-libgcc
INCS       = 
CXXINCS    = 
BIN        = $(PROJECT).dll
CXXFLAGS   = $(CXXINCS) -Wall -g3 $(DEFINE)
CFLAGS     = $(INCS) -Wall -g3 $(DEFINE)
DEF        = lib$(PROJECT).def
STATIC     = lib$(PROJECT).a
DISTDIR    = $(PROJECT)
DISTNAME   = $(PROJECT).zip
RELEASEDIR = bin
RELEASE    = $(RELEASEDIR)/$(PROJECT).dll

CPP        = g++.exe
CC         = gcc.exe
WINDRES    = windres.exe
RM_F       = rm -f
MKDIR      = mkdir
RM_RF      = rm -rf
ZIP        = zip -r
CP         = cp
DIST_CP    = cp -r --parents

ECHO       = echo
NOECHO     = @

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

clean: clean-custom
	$(RM_F) $(OBJ) $(BIN) $(DEF) $(STATIC) $(PROJECT).layout

$(BIN): $(LINKOBJ)
	$(CPP) -shared $(LINKOBJ) -o $(BIN) $(LIBS) -municode -mthreads -Wl,-Bstatic,--output-def,$(DEF),--out-implib,$(STATIC),--add-stdcall-alias

$(PROJECT).o: $(PROJECT).cpp
	$(CPP) -c $(PROJECT).cpp -o $(PROJECT).o $(CXXFLAGS)

PluginDefinition.o: PluginDefinition.cpp
	$(CPP) -c PluginDefinition.cpp -o PluginDefinition.o $(CXXFLAGS)

$(PROJECT)_private.res: $(PROJECT)_private.rc
	$(WINDRES) -i $(PROJECT)_private.rc --input-format=rc -o $(PROJECT)_private.res -O coff

##########
# Distribution
realclean: clean distclean
	$(NOECHO) if [ -d $(RELEASEDIR) ]; then \
	    $(ECHO) $(RM_RF) $(RELEASEDIR); \
		$(RM_RF) $(RELEASEDIR); \
	fi

distclean: distdirclean distfileclean

distdirclean:
	$(NOECHO) if [ -d $(DISTDIR) ]; then \
	    $(ECHO) $(RM_RF) $(DISTDIR); \
		$(RM_RF) $(DISTDIR); \
	fi

distfileclean:
	$(NOECHO) if [ -f $(DISTNAME) ]; then \
	    $(ECHO) $(RM_F) $(DISTNAME); \
	    $(RM_F) $(DISTNAME); \
    fi

dist: manifest $(DISTNAME) distdirclean

$(DISTNAME): $(RELEASE)
	$(MKDIR) $(DISTDIR)
	$(NOECHO) for i in `cat MANIFEST`; do $(DIST_CP) $${i} $(DISTDIR); done
	$(ZIP) $(DISTNAME) $(DISTDIR)

$(RELEASE): $(BIN) $(RELEASEDIR)
	$(CP) $(BIN) $(RELEASEDIR)

$(RELEASEDIR):
	$(NOECHO) if [ ! -d $(RELEASEDIR) ]; then \
	    $(ECHO) $(MKDIR) $(RELEASEDIR); \
	    $(MKDIR) $(RELEASEDIR); \
	fi
