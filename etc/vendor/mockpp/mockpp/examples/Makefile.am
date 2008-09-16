INCLUDES = $(all_includes) -I$(top_srcdir) -I$(top_builddir) $(EA_EXTRA_INC)
DEFAULT_INCLUDES = $(INCLUDES)

SUBDIRS= tutorial

if BUILTIN_CXXTEST
cxxtest_lib=$(top_builddir)/3party/cxxtest/cxxtest/libmockpp_cxxtest.la
endif

noinst_PROGRAMS  = mock_greeter

all: $(bin_PROGRAMS)

mock_greeter_SOURCES    = mock_greeter.cpp

mock_greeter_LDADD = $(top_builddir)/mockpp/libmockpp.la  $(EA_EXTRA_LIB) $(LIBDL) $(CPPUNIT_LIBS) \
	-lstdc++ $(cxxtest_lib)

mock_greeter_LDFLAGS = -L/opt/intel_cc_80/lib
