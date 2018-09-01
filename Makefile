PROJECT       = ColHighlight

CEXE          = 
CDLL          =
CXXEXE        =
CXXDLL        = $(PROJECT).dll

##########
# Virtual paths and helpers, dir names (no trailing slash)
VPATH = 
#vpath $.c src
#vpath %.h include

##########
# directory names (no trailing slash) containing Makefile(s) to execute
RECURMAKEDIRS = 

##########
# Remember to update MANIFEST if using RELEASEDIR
RELEASEDIR    = bin

########################################
ifdef RELEASEDIR
RELEASE      += $(RELEASEDIR)\$(CDLL)
RELEASE      += $(RELEASEDIR)\$(CEXE)
RELEASE      += $(RELEASEDIR)\$(CXXDLL)
RELEASE      += $(RELEASEDIR)\$(CXXEXE)
endif

BIN           = $(CDLL) $(CEXE) $(CXXDLL) $(CXXEXE)

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

##########
# Tools
########################################
SHELL         = cmd.exe
ECHO          = echo
NOECHO        = @

MAKE          = gmake.exe
CC            = gcc.exe
CXX           = g++.exe
WINDRES       = windres.exe
DLLWRAP       = dllwrap.exe

CAT           = type
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
FORCPREL      = for %%i in ($(BIN)) do $(CP) %%i $(RELEASEDIR)
ZIP           = zip -r

FORRECURSMAKE = @for %%i in ($(RECURMAKEDIRS)) do @$(MAKE) --directory=%%i $(TARGET)
FORRECURSCLEAN= @for %%i in ($(RECURMAKEDIRS)) do @$(MAKE) --directory=%%i clean

makefile:
	$(NOECHO) $(CAT) Makefile_* > Makefile

ifdef RECURMAKEDIRS
recursivemake:
	$(FORRECURSMAKE)

clean-recursivemake:
	$(FORRECURSCLEAN)

CLEAN-CUSTOM += clean-recursivemake
endif

##########
# Resources
RESOURCES     = \
  $(PROJECT)_private.rc

RINCS         = 

########################################
OBJRES        = $(RESOURCES:.rc=.res)

%.res: %.rc
	$(WINDRES) -i $< --input-format=rc -o $@ -O coff $(RINCS)

##########
# C++
CXXSOURCES    = \
  $(PROJECT).cpp \
  PluginDefinition.cpp \

CXXDEFINE     = -DUNICODE
CXXFLAGS      = -Wall -g3
CXXINCS       = 

########################################
CXXOPTS       = $(CXXINCS) $(CXXFLAGS) $(CXXDEFINE)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXOPTS)

##########
# C++ .dll
CXXDLIBS      = -s -static-libgcc
CXXDLOPTS     = --subsystem,windows,-Bstatic

########################################
CXXDOBJECTS   = $(PREOBJS) $(CXXSOURCES:.cpp=.o) $(POSTOBJS)
CXXDOBJLINK   = $(CXXDOBJECTS) $(OBJRES)
CXXDEFFILE    = lib$(PROJECT).def
CXXSTATICLIB  = lib$(PROJECT).a
CLEAN-CUSTOM += clean-cxxdll

##########
$(CXXDLL): $(CXXDOBJLINK)
	$(CXX) -shared $(CXXDOBJLINK) -o $(CXXDLL) $(CXXDLIBS) -Wl,$(CXXDLOPTS),--output-def,$(CXXDEFFILE),--out-implib,$(CXXSTATICLIB)
#	$(DLLWRAP) --output-def $(CXXDEFFILE) --implib $(CXXSTATICLIB) $(CXXDOBJLINK) $(CXXDLIBS) --add-stdcall-alias -o $(CXXDLL)

clean-cxxdll:
	$(RM_F) $(CXXDLL) $(CXXDOBJLINK) $(CXXDEFFILE) $(CXXSTATICLIB)

##########
# Disttribution / Clean
########################################
DISTDIR       = $(PROJECT)
DISTNAME      = $(PROJECT).zip

clean-custom: $(CLEAN-CUSTOM)

clean: clean-custom
	$(RM_F) $(PROJECT).layout *.orig *.bak

##########
# Clean / Dist
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
	$(NOECHO) $(FORCPREL)

$(RELEASEDIR):
	$(NOECHO) $(IF) $(NEXIST_D_) $(RELEASEDIR) $(_EXIST_D) \
	    $(ECHO) $(MKDIR) $(RELEASEDIR) $(CONT) \
	    $(MKDIR) $(RELEASEDIR) $(LAST) \
	$(ENDIF)

