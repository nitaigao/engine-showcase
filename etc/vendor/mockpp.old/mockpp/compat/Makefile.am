INCLUDES = $(all_includes) -I$(top_srcdir) -I$(top_builddir) $(EA_EXTRA_INC)
DEFAULT_INCLUDES = $(INCLUDES)

noinst_LTLIBRARIES = libcompat.la

libcompat_la_SOURCES = \
  Exception.cpp \
  Asserter.cpp \
  Formatter.cpp \
  AssertionFailedError.cpp

libcompatincludedir = $(includedir)/mockpp/compat

libcompatinclude_HEADERS = Exception.h Asserter.h Formatter.h \
	AssertionFailedError.h

CLEANFILES = *.~* *.~~* *~ *.old



