PROJECT       = ColHighlight
SRCEXT        = cpp
BINEXT        = dll

SOURCES       = \
  $(PROJECT).$(SRCEXT) \
  PluginDefinition.cpp

RESOURCES     = \
  $(PROJECT)_private.rc

# Additional source types: $(TYPESRC:.type=.o)
#TYPESRC      = file.type
PREOBJS       = 
POSTOBJS      = 
CLEAN-CUSTOM  = clean-cxxdll

DEFINE        = -DUNICODE
FLAGS         = -Wall -g3
INCS          = 
LIBS          = -static-libgcc -municode -mthreads

# Remember to update MANIFEST if using RELEASEDIR
RELEASEDIR    = bin

########################################
OBJECTS       = $(PREOBJS) $(SOURCES:.$(SRCEXT)=.o) $(POSTOBJS)
OBJRES        = $(RESOURCES:.rc=.res)
OBJLINK       = $(OBJECTS) $(OBJRES)
BIN           = $(PROJECT).$(BINEXT)

DISTDIR       = $(PROJECT)
DISTNAME      = $(PROJECT).zip

ifdef RELEASEDIR
RELEASE      += $(RELEASEDIR)\$(BIN)
endif

##########
SHELL         = cmd.exe
ECHO          = echo
NOECHO        = @

MAKE          = gmake.exe
CC            = gcc.exe
CXX           = g++.exe
WINDRES       = windres.exe

MKDIR         = mkdir
RM_F          = del /q
RM_RF         = rmdir /s/q
CP            = copy /y
DIST_CP       = echo f|xcopy /y
IF            = if
ENDIF         = 
EXIST_F_      = exist
NEXIST_F_     = not $(EXIST_F_)
_EXIST_F      = 
EXIST_D_      = exist
NEXIST_D_     = not $(EXIST_D_)
_EXIST_D      = 
CONT          = &&
LAST          =
FORCPMAN      = for /f %%i in (MANIFEST) do $(DIST_CP) %%i $(DISTDIR)\%%i > nul
ZIP           = zip -r

-include Makefile.binsh

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

clean-custom: $(CLEAN-CUSTOM)

clean: clean-custom
	$(RM_F) $(OBJLINK) $(BIN) $(PROJECT).layout *.orig *.bak

$(OBJRES): $(RESOURCES)
	$(WINDRES) -i $(RESOURCES) --input-format=rc -o $(OBJRES) -O coff

##########
# Distribution
REALCLEAN     = clean distclean
ifdef RELEASEDIR
release: $(RELEASE)

REALCLEAN    += releaseclean

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

# Additional Tools

# Additional targets
##########
# C++ .dll
DLLWRAP       = dllwrap.exe

CXXLINKOPTS   = -Bstatic
DEFINE       += -DBUILDING_DLL=1

CXXDEFFILE    = lib$(PROJECT).def
CXXSTATICLIB  = lib$(PROJECT).a

$(BIN): $(OBJLINK)
	$(CXX) -shared $(OBJLINK) -o $(BIN) $(LIBS) -Wl,$(CXXLINKOPTS),--output-def,$(CXXDEFFILE),--out-implib,$(CXXSTATICLIB),--add-stdcall-alias
#	$(DLLWRAP) --output-def $(CXXDEFFILE) --implib $(CXXSTATICLIB) $(OBJLINK) $(LIBS) --add-stdcall-alias -o $(BIN)

clean-cxxdll:
	$(RM_F) $(CXXDEFFILE) $(CXXSTATICLIB)

##########
# C++
CXXFLAGS      = $(INCS) $(FLAGS) $(DEFINE)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

