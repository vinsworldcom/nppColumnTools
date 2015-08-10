# Project: NppColHighLight
# Makefile created by Dev-C++ 5.9.2

PROJECT  = NppColHighLight
CPP      = g++.exe
CC       = gcc.exe
WINDRES  = windres.exe
OBJ      = NppColHighLight.o PluginDefinition.o
LINKOBJ  = NppColHighLight.o PluginDefinition.o
LIBS     = -L"C:/strawberry/c/lib" -L"C:/strawberry/c/x86_64-w64-mingw32/lib" -static-libgcc
INCS     = -I"C:/strawberry/c/include" -I"C:/strawberry/c/x86_64-w64-mingw32/include" -I"C:/strawberry/c/lib/gcc/x86_64-w64-mingw32/4.7.3/include"
CXXINCS  = -I"C:/strawberry/c/include" -I"C:/strawberry/c/x86_64-w64-mingw32/include" -I"C:/strawberry/c/lib/gcc/x86_64-w64-mingw32/4.7.3/include" -I"C:/strawberry/c/lib/gcc/x86_64-w64-mingw32/4.7.3/include/c++"
BIN      = $(PROJECT).dll
CXXFLAGS = $(CXXINCS) -Wall -g3 -DBUILDING_DLL=1 -DUNICODE 
CFLAGS   = $(INCS) -Wall -g3 -DBUILDING_DLL=1 -DUNICODE 
RM       = rm.exe -f
DEF      = libNppColHighLight.def
STATIC   = libNppColHighLight.a

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

clean: clean-custom
	${RM} $(OBJ) $(BIN) $(DEF) $(STATIC) $(PROJECT).layout

$(BIN): $(LINKOBJ)
	$(CPP) -shared $(LINKOBJ) -o $(BIN) $(LIBS) -municode -mthreads -Wl,-Bstatic,--output-def,$(DEF),--out-implib,$(STATIC),--add-stdcall-alias

NppColHighLight.o: NppColHighLight.cpp
	$(CPP) -c NppColHighLight.cpp -o NppColHighLight.o $(CXXFLAGS)

PluginDefinition.o: PluginDefinition.cpp
	$(CPP) -c PluginDefinition.cpp -o PluginDefinition.o $(CXXFLAGS)
