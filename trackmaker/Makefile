ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS      = $(shell root-config --libs)
ARCH          = $(shell root-config --arch)
OSTYPE        = $(shell uname -s)

ifeq ($(ARCH),macosx)
CXX             = g++-32 -g -O
else
CXX             = $(shell root-config --cxx) -g
endif
CXXFLAGS	= $(BUILDBITS) -O -Wall -fPIC -I../unpacker -I../hitmaker -I../kalman/include -I$(KALTESTROOT)/include
LD              = $(CXX)
LN              = ln -sf

CXXFLAGS	+= $(ROOTCFLAGS)
LIBS		= -L. -l$(LIBNAME) -L../unpacker -lJREvent  $(ROOTLIBS)

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

SRCS		= $(strip $(subst .$(ObjSuf),.$(SrcSuf),$(OBJS)))
HDRS            = $(strip $(subst .$(ObjSuf),.$(HdrSuf),$(OBJS)))

LIBNAME         = JTracks
DICTNAME        = $(LIBNAME)Dict

SOSRCS          = JTrack.$(SrcSuf) \
		  JTrackMaker.$(SrcSuf) \
		  JVTrackFinder.$(SrcSuf) \
		  JSimpleTrackFinder.$(SrcSuf) \
		  JTripletTrackFinder.$(SrcSuf) \
		  JVTrackFitter.$(SrcSuf) \
		  JSimpleTrackFitter.$(SrcSuf) \
		  JHelix.$(SrcSuf)

SOOBJS		= $(strip $(subst .$(SrcSuf),.$(ObjSuf),$(SOSRCS))) \
		  $(DICTNAME).$(ObjSuf)

SOHDRS	        = $(subst .$(SrcSuf),.h,$(SOSRCS))

SONAME          = lib$(LIBNAME).$(DllSuf)

all:		$(SONAME)

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
	rm -f lib$(LIBNAME).* $(DICTNAME).* *.log

$(DICTNAME).$(SrcSuf): $(SOHDRS) LinkDef.h
	@echo "Generating dictionary ..."
	rootcint -f $(DICTNAME).$(SrcSuf) \
	         -c -I../unpacker $(SOHDRS) LinkDef.h
