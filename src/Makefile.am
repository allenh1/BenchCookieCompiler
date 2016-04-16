# compile flags
BUILT_SOURCES = syntax_bison.hh
AM_CFLAGS = -O2 -fPIC
AM_CXXFLAGS = -O2 -fPIC -std=c++14

AM_YFLAGS = -d -p syntax
AM_LFLAGS = -o$(LEX_OUTPUT_ROOT).c

CLEAN_FILES = *~

# targets
bin_PROGRAMS = bcc
PARSER = syntax_bison.yy syntax_flex.ll
ARCHES = x86_64_compiler.cpp arm_compiler.cpp
bcc_SOURCES = command.cpp command.h $(PARSER)

# bring the "stable" arches with us
EXTRA_DIST = $(ARCHES)

