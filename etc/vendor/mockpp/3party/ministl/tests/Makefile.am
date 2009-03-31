INCLUDES = -I$(top_srcdir)/3party -I$(top_builddir) -I$(top_srcdir)
DEFAULT_INCLUDES = $(INCLUDES)

CLEANFILES = *.~* *.~~* *~

# ministl_test_LDFLAGS =

ministl_test_SOURCES = ministl_test.cpp

ministl_test_LDADD = -lstdc++

if BUILTIN_MINISTL

TESTS          = $(check_PROGRAMS)

check_PROGRAMS = ministl_test

endif

