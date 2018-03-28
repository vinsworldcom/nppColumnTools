PROJECT    = ColHighlight

SOURCES    = \
  $(PROJECT).cpp \
  PluginDefinition.cpp

RESOURCES  = \
  $(PROJECT)_private.rc

DEFINE     = -DBUILDING_DLL=1 -DUNICODE
LIBS       = -static-libgcc
INCS       = 

# Remember to update MANIFEST if using RELEASEDIR                                                 
RELEASEDIR = bin

##########
OBJECTS    = $(SOURCES:.cpp=.o)
OBJRES     = $(PROJECT)_private.res
OBJLINK    = $(OBJECTS) $(OBJRES)
BIN        = $(PROJECT).dll

CXXFLAGS   = $(CXXINCS) -Wall -g3 $(DEFINE)

DEF        = lib$(PROJECT).def
STATIC     = lib$(PROJECT).a

DISTDIR    = $(PROJECT)
DISTNAME   = $(PROJECT).zip

ifdef RELEASEDIR
RELEASE    = $(RELEASEDIR)/$(BIN)
endif

##########
SHELL      = cmd.exe
ECHO       = echo
NOECHO     = @

MAKE       = gmake.exe
CC         = gcc.exe
CXX        = g++.exe
WINDRES    = windres.exe

MKDIR      = mkdir
RM_F       = del /q
RM_RF      = rmdir /s/q
CP         = copy /y
DIST_CP    = echo f|xcopy /y
IF         = if
ENDIF      = 
EXIST_F_   = exist
NEXIST_F_  = not $(EXIST_F_)
_EXIST_F   = 
EXIST_D_   = exist
NEXIST_D_  = not $(EXIST_D_)
_EXIST_D   = 
CONT       = &&
LAST       =
FORCPMAN   = for /f %%i in (MANIFEST) do $(DIST_CP) %%i $(DISTDIR)\%%i > nul
ZIP        = zip -r

-include Makefile.binsh

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

clean: clean-custom
	$(RM_F) $(OBJLINK) $(BIN) $(DEF) $(STATIC) $(PROJECT).layout

$(BIN): $(OBJLINK)
	$(CXX) -shared $(OBJLINK) -o $(BIN) $(LIBS) -municode -mthreads -Wl,-Bstatic,--output-def,$(DEF),--out-implib,$(STATIC),--add-stdcall-alias

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJRES): $(RESOURCES)
	$(WINDRES) -i $(RESOURCES) --input-format=rc -o $(OBJRES) -O coff

##########
# Distribution
REALCLEAN  = clean distclean
ifdef RELEASEDIR
release: $(RELEASE)

REALCLEAN += releaseclean

releaseclean:
	$(NOECHO) $(IF) $(EXIST_D_) $(RELEASEDIR) $(_EXIST_D) \
	    $(ECHO) $(RM_RF) $(RELEASEDIR) $(CONT) \
		$(RM_RF) $(RELEASEDIR) $(LAST) \
	$(ENDIF)

endif
realclean: $(REALCLEAN)

distclean: distdirclean distfileclean

distdirclean:
	$(NOECHO) $(IF) $(EXIST_D_) $(DISTDIR) $(_EXIST_D) \
	    $(ECHO) $(RM_RF) $(DISTDIR) $(CONT) \
		$(RM_RF) $(DISTDIR) $(LAST) \
	$(ENDIF)

distfileclean:
	$(NOECHO) $(IF) $(EXIST_F_) $(DISTNAME) $(_EXIST_F) \
	    $(ECHO) $(RM_F) $(DISTNAME) $(CONT) \
	    $(RM_F) $(DISTNAME) $(LAST) \
	$(ENDIF)

dist: manifest $(DISTNAME)

$(DISTDIR):
	$(MKDIR) $(DISTDIR)

$(DISTNAME): $(RELEASE) | $(DISTDIR)
	$(NOECHO) $(FORCPMAN)
	$(ZIP) $(DISTNAME) $(DISTDIR)

$(RELEASE): $(BIN) | $(RELEASEDIR)
	$(CP) $(BIN) $(RELEASEDIR)

$(RELEASEDIR):
	$(NOECHO) $(IF) $(NEXIST_D_) $(RELEASEDIR) $(_EXIST_D) \
	    $(ECHO) $(MKDIR) $(RELEASEDIR) $(CONT) \
	    $(MKDIR) $(RELEASEDIR) $(LAST) \
	$(ENDIF)
