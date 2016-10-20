ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS      = $(shell root-config --libs)
ARCH          = $(shell root-config --arch)
OSTYPE        = $(shell uname -s)

ifeq ($(ARCH),macosx)
CXX             = g++-32 -g -O
else
CXX             = $(shell root-config --cxx) -g
endif
CXXFLAGS	= -O -Wall -fPIC
LD              = $(CXX)
LN              = ln -sf

CXXFLAGS	+= $(ROOTCFLAGS)
LIBS		= -L. -lJREvent $(ROOTLIBS)

ObjSuf		= o
SrcSuf		= cxx
HdrSuf		= h

ifeq ($(OSTYPE),Darwin)
DllSuf          = dylib
SOFLAGS         = -dynamiclib -flat_namespace -single_module -undefined dynamic_lookup
LDFLAGS         = $(BUILDBITS) -Xlinker -bind_at_load
else
SOFLAGS         = -shared -rdynamic
DllSuf          = so
endif

OBJS		= JMonEvent.$(ObjSuf) \
		  JMonFile.$(ObjSuf) \
		  JMonHist.$(ObjSuf) \
		  JPed.$(ObjSuf) \
		  $(PROGRAM).$(ObjSuf)

PROGRAM		= unpacker


SRCS		= $(strip $(subst .$(ObjSuf),.$(SrcSuf),$(OBJS)))
HDRS            = $(strip $(subst .$(ObjSuf),.$(HdrSuf),$(OBJS)))

LIBNAME         = JREvent
DICTNAME        = $(LIBNAME)Dict

SOSRCS          = JREvent.$(SrcSuf) \
		  JCluster.$(SrcSuf)\
		  JChMapper.$(SrcSuf) \
		  JPadGeom.$(SrcSuf)

SOOBJS		= $(strip $(subst .$(SrcSuf),.$(ObjSuf),$(SOSRCS))) \
		  $(DICTNAME).$(ObjSuf)

SOHDRS	        = $(subst .$(SrcSuf),.h,$(SOSRCS))

SONAME          = lib$(LIBNAME).$(DllSuf)

all:		$(PROGRAM)

$(PROGRAM):	$(OBJS) $(SONAME)
		$(CXX) $(OBJS) $(LIBS) -o $(PROGRAM) $(LDFLAGS)

$(SONAME): $(SOOBJS)
	$(LD) $(SOFLAGS) -o $(SONAME) $(SOOBJS) $(LDFLAGS)
ifeq ($(OSTYPE),Darwin)
	$(LN) $(SONAME) lib$(LIBNAME).so
endif

.SUFFIXES: .$(SrcSuf)
.$(SrcSuf).$(ObjSuf):
	        $(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *.so *.o *~ core *.pcm *.d

distclean: clean
	rm -f $(PROGRAM) lib$(LIBNAME).* $(DICTNAME).*

$(DICTNAME).$(SrcSuf): $(SOHDRS) LinkDef.h
	@echo "Generating dictionary ..."
	rootcint -f $(DICTNAME).$(SrcSuf) \
	         -c $(SOHDRS) $(RCINTDEFINES) LinkDef.h
