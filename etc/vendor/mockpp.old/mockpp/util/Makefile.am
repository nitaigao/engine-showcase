INCLUDES = $(all_includes) -I$(top_srcdir) -I$(top_builddir) $(EA_EXTRA_INC)
DEFAULT_INCLUDES = $(INCLUDES)

noinst_LTLIBRARIES = libutil.la

libutil_la_SOURCES = \
  AssertMo.cpp \
  NotImplementedException.cpp

libutilincludedir = $(includedir)/mockpp/util

libutilinclude_HEADERS = AssertMo.h NotImplementedException.h AutoPointer.h

CLEANFILES = *.~* *.~~* *~ *.old
