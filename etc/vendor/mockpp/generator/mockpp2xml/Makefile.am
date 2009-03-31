
INCLUDES = $(all_includes) -I$(top_srcdir)  -I$(top_builddir)
AM_LDFLAGS   = $(all_libraries) -lstdc++
AM_CPPFLAGS = -DYYDEBUG

####### Files

bin_PROGRAMS = mockpp2xml

mockpp2xml_SOURCES = main.cpp scanner.cpp yacc.cpp
noinst_HEADERS = scanner.ll yacc.hpp yacc.yy

mockpp2xml_LDADD = $(LIB_QT)
mockpp2xml_LDFLAGS = $(KDE_RPATH)

####### Build rules

parser:
	cd $(srcdir) && flex -B -8 -oscanner.cpp scanner.ll  ;\
	bison -t -d -v -o yacc.cpp yacc.yy

check-local:
	$(srcdir)/regressiontest.sh $(srcdir)/mockppidl_test.h  $(srcdir)/mockppidl-orig.xml
	$(srcdir)/regressiontest.sh $(srcdir)/dcopidl_test.h    $(srcdir)/dcopidl-orig.xml

EXTRA_DIST = \
  dcopidl_test.h \
  dcopidl-orig.xml \
  mockppidl_test.h \
  mockppidl-orig.xml \
  scanner.ll \
  yacc.yy \
  mstring.h \
  regressiontest.sh

MAINTAINERCLEANFILES = yacc.cc.output
