#
# Created by gmakemake (Ubuntu Jul 25 2014) on Mon Oct  7 19:28:40 2024
#

#
# Definitions
#

.SUFFIXES:
.SUFFIXES:	.a .o .c .C .cpp .s .S
.c.o:
		$(COMPILE.c) $<
.C.o:
		$(COMPILE.cc) $<
.cpp.o:
		$(COMPILE.cc) $<
.S.s:
		$(CPP) -o $*.s $<
.s.o:
		$(COMPILE.cc) $<
.c.a:
		$(COMPILE.c) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.C.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.cpp.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%

CC =		gcc
CXX =		g++

RM = rm -f
AR = ar
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LINK.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c
CPP = $(CPP) $(CPPFLAGS)
########## Flags from header.mak

CC =     gcc
CFLAGS = -ggdb -std=c99 -Wall -Wextra -pedantic

########## End of flags from header.mak


CPP_FILES =	
C_FILES =	emp.c log.c parse.c rpt.c
PS_FILES =	
S_FILES =	
H_FILES =	emp.h log.h parse.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	emp.o log.o parse.o 

#
# Main targets
#

all:	rpt 

rpt:	rpt.o $(OBJFILES)
	$(CC) $(CFLAGS) -o rpt rpt.o $(OBJFILES) $(CLIBFLAGS)

#
# Dependencies
#

emp.o:	emp.h log.h parse.h
log.o:	emp.h log.h parse.h
parse.o:	parse.h
rpt.o:	emp.h log.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -f $(OBJFILES) rpt.o core

realclean:        clean
	-/bin/rm -f rpt 
