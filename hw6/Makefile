#
# Created by gmakemake (Ubuntu Jul 25 2014) on Mon Oct 28 19:31:23 2024
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

CFLAGS = -std=c99 -ggdb -Wall -Wextra -pedantic

########## End of flags from header.mak


CPP_FILES =	
C_FILES =	String.c str_test1.c str_test2.c str_test3.c
PS_FILES =	
S_FILES =	
H_FILES =	String.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	String.o 

#
# Main targets
#

all:	str_test1 str_test2 str_test3 

str_test1:	str_test1.o $(OBJFILES)
	$(CC) $(CFLAGS) -o str_test1 str_test1.o $(OBJFILES) $(CLIBFLAGS)

str_test2:	str_test2.o $(OBJFILES)
	$(CC) $(CFLAGS) -o str_test2 str_test2.o $(OBJFILES) $(CLIBFLAGS)

str_test3:	str_test3.o $(OBJFILES)
	$(CC) $(CFLAGS) -o str_test3 str_test3.o $(OBJFILES) $(CLIBFLAGS)

#
# Dependencies
#

String.o:	String.h
str_test1.o:	String.h
str_test2.o:	String.h
str_test3.o:	String.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -f $(OBJFILES) str_test1.o str_test2.o str_test3.o core

realclean:        clean
	-/bin/rm -f str_test1 str_test2 str_test3 
