#
# Created by gmakemake (Ubuntu Jul 25 2014) on Mon Oct 14 14:23:30 2024
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

CC =             gcc
CFLAGS =	-ggdb -std=c99 -Wall -Wextra -pedantic

########## End of flags from header.mak


CPP_FILES =	
C_FILES =	bst.c concordance.c test_tree.c
PS_FILES =	
S_FILES =	
H_FILES =	bst.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	bst.o 

#
# Main targets
#

all:	concordance test_tree 

concordance:	concordance.o $(OBJFILES)
	$(CC) $(CFLAGS) -o concordance concordance.o $(OBJFILES) $(CLIBFLAGS)

test_tree:	test_tree.o $(OBJFILES)
	$(CC) $(CFLAGS) -o test_tree test_tree.o $(OBJFILES) $(CLIBFLAGS)

#
# Dependencies
#

bst.o:	bst.h
concordance.o:	bst.h
test_tree.o:	bst.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -f $(OBJFILES) concordance.o test_tree.o core

realclean:        clean
	-/bin/rm -f concordance test_tree 
