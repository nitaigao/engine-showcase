SUBDIRS      =

INCLUDES = $(all_includes) -I$(top_srcdir) -I$(top_builddir) $(EA_EXTRA_INC)

AM_CXXFLAGS   = $(INCLUDES)

DEFAULT_INCLUDES = $(INCLUDES)

#########

check_PROGRAMS = MockppIdlTest_mock

MockppIdlTest_mock_SOURCES = MockppIdlTest_mock.cpp MockppIdlTest_mock_user.cpp \
                             ../mockppidl_test.cpp

EXTRA_DIST = MockppIdlTest_mock.h

$(srcdir)/MockppIdlTest_mock.cpp: check-local

MockppIdlTest_mock_LDADD = $(top_builddir)/mockpp/libmockpp.la \
	$(LIBDL) -lstdc++ $(EA_EXTRA_LIB)

check-local:

TESTS          = $(check_PROGRAMS)

MAINTAINERCLEANFILES =
