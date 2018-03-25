# Project: NppColHighLight
# Makefile created by Dev-C++ 5.9.2

PROJECT  = ColHighLight
CPP      = g++.exe
CC       = gcc.exe
WINDRES  = windres.exe
RM       = rm.exe -f

DLLRES   = $(PROJECT)_private.res
OBJ      = $(PROJECT).o PluginDefinition.o $(DLLRES)
LINKOBJ  = $(PROJECT).o PluginDefinition.o $(DLLRES)
LIBS     = -L"C:/strawberry/c/lib" -L"C:/strawberry/c/x86_64-w64-mingw32/lib" -static-libgcc
INCS     = -I"C:/strawberry/c/include" -I"C:/strawberry/c/x86_64-w64-mingw32/include" -I"C:/strawberry/c/lib/gcc/x86_64-w64-mingw32/4.7.3/include"
CXXINCS  = -I"C:/strawberry/c/include" -I"C:/strawberry/c/x86_64-w64-mingw32/include" -I"C:/strawberry/c/lib/gcc/x86_64-w64-mingw32/4.7.3/include" -I"C:/strawberry/c/lib/gcc/x86_64-w64-mingw32/4.7.3/include/c++"
BIN      = $(PROJECT).dll
CXXFLAGS = $(CXXINCS) -Wall -g3 -DBUILDING_DLL=1 -DUNICODE 
CFLAGS   = $(INCS) -Wall -g3 -DBUILDING_DLL=1 -DUNICODE 
DEF      = lib$(PROJECT).def
STATIC   = lib$(PROJECT).a

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

clean: clean-custom
	${RM} $(OBJ) $(BIN) $(DEF) $(STATIC) $(PROJECT).layout

$(BIN): $(LINKOBJ)
	$(CPP) -shared $(LINKOBJ) -o $(BIN) $(LIBS) -municode -mthreads -Wl,-Bstatic,--output-def,$(DEF),--out-implib,$(STATIC),--add-stdcall-alias

$(PROJECT).o: $(PROJECT).cpp
	$(CPP) -c $(PROJECT).cpp -o $(PROJECT).o $(CXXFLAGS)

PluginDefinition.o: PluginDefinition.cpp
	$(CPP) -c PluginDefinition.cpp -o PluginDefinition.o $(CXXFLAGS)

$(PROJECT)_private.res: $(PROJECT)_private.rc
	$(WINDRES) -i $(PROJECT)_private.rc --input-format=rc -o $(PROJECT)_private.res -O coff
