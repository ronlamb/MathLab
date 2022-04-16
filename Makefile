CPPFLAGS=-std=c++20 -pthread

ifdef DEBUG
	OUTDIR=debug
	CPPFLAGS += -DDEBUG=1
	LDLFLAGS=-DDEBUG=1
else
	OUTDIR=release
	CPPFLAGS += -O3
endif

LIBSRCDIR=MathLib
EXESRCDIR=MathLab

LIBSRCPATH=$(wildcard $(LIBSRCDIR)/*.cpp)
LIBSRC=$(notdir $(LIBSRCPATH))

EXESRCPATH=$(wildcard $(EXESRCDIR)/*.cpp)
EXESRC=$(notdir $(EXESRCPATH))

OBJDIR=$(OUTDIR)/obj
LIBDIR=$(OUTDIR)/lib

OBJLIBS = -L. -lmtoy
EXELIBS = -L$(LIBDIR) -lMathLib -lm

CPP = g++

LIBOBJ = $(patsubst %, $(OBJDIR)/%,$(LIBSRC:.cpp=.o))
EXEOBJ = $(patsubst %, $(OBJDIR)/%,$(EXESRC:.cpp=.o))

LIB=$(LIBDIR)/libMathLib.a
EXE=$(OUTDIR)/MathLab

EXEINCLUDES = -IMathLib

.SUFFIXES: .cpp
.PHONY: debug

default: lib exe

lib: $(OBJDIR) $(LIBDIR) $(LIB)
	@echo "Making library $(LIB)"

exe: $(OBJDIR) $(EXE)
	@echo "Making $(EXE)"

#####################################################################
# Library and executable targets
#####################################################################

$(OBJDIR) $(LIBDIR):
	@echo "Making executable $@ directory"
	@mkdir -p $@

$(LIB): $(LIBOBJ)
	ar crf $(LIB) $(LIBOBJS)

$(EXE): $(EXEOBJ)
	$(CPP) $(CPPFLAGS) -o $@ $< $(EXELIBS)

#####################################################################
# Library and executable targets
#####################################################################

$(OBJDIR)/%.o: $(EXESRCDIR)/%.cpp | $(OBJDIR)
	$(CPP) $(CPPFLAGS) $(EXEINCLUDES) -c $< -o $@

$(OBJDIR)/%.o: $(LIBSRCDIR)/%.cpp | $(OBJDIR)
	$(CPP) $(CPPFLAGS) -c $< -o $@

#####################################################################
# Dependency and helper targets
#####################################################################

showvers:
	@echo "CPPFLAGS: $(CPPFLAGS)"
	@echo "LIBSRC:   $(LIBSRC)"
	@echo "EXESRC:   $(EXESRC)"
	@echo "LIBDIR:   $(LIBDIR)"
	@echo "LIBOBJ:   $(LIBOBJ)"
	@echo "EXEOBJ:   $(EXEOBJ)"
	@echo "LIB:      $(LIB)"
	@echo "EXE:      $(EXE)"

depend: dep

dep: Makefile
	makedepend -- $(CPPFLAGS) $(SRC)

clean:
	rm -f $(LIBOBJ) $(EXEOBJ) $(LIB) $(EXE) Makefile.bak

