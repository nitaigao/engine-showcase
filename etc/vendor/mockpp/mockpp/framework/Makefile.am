INCLUDES = $(all_includes) -I$(top_srcdir) -I$(top_builddir) $(EA_EXTRA_INC)
DEFAULT_INCLUDES = $(INCLUDES)

noinst_LTLIBRARIES = libframework.la

libframeworkinclude_HEADERS = \
 CxxTestSupport.h CxxTestRunner.h \
 SelectUnittestFramework.h \
 VerifyingTestCase.h VerifyingTestCaller.h

EXTRA_DIST =

libframeworkincludedir = $(includedir)/mockpp/framework

CLEANFILES = *.~* *.~~* *~ *.old

libframework_la_SOURCES = \
  VerifyingTestCase.cpp VerifyingTestCaller.cpp

noinst_HEADERS =
