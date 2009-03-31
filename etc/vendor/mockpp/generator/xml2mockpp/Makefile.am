SUBDIRS      = expat . new-chain-meth new-visit-meth orig-chain-meth orig-visit-meth

INCLUDES = $(all_includes) -I$(top_srcdir) -I$(top_builddir) $(EA_EXTRA_INC)

AM_CXXFLAGS   = $(INCLUDES)

DEFAULT_INCLUDES = $(INCLUDES)

####### Files

bin_PROGRAMS = xml2mockpp

xml2mockpp_SOURCES = GenChainableMethod.cpp GenerateMethod.cpp \
	GenVisitableMethod.cpp MockppIdlClass.cpp MockppIdlParser.cpp expatwrap.cpp xml2mockpp.cpp \
	xmlexcept.cpp xmlparse_base.cpp xmlparser.cpp

xml2mockpp_LDADD = $(top_builddir)/generator/xml2mockpp/expat/libexpat.la

####### Build rules

remedy-all:
	cp $(srcdir)/new-chain-meth/MockppIdlTest_mock_user.cpp.new   $(srcdir)/new-chain-meth/MockppIdlTest_mock_user.cpp
	cp $(srcdir)/new-chain-meth/*.cpp    $(srcdir)/orig-chain-meth
	cp $(srcdir)/new-chain-meth/*.h      $(srcdir)/orig-chain-meth
	cp $(srcdir)/new-visit-meth/MockppIdlTest_mock_user.cpp.new   $(srcdir)/new-visit-meth/MockppIdlTest_mock_user.cpp
	cp $(srcdir)/new-visit-meth/*.cpp    $(srcdir)/orig-visit-meth
	cp $(srcdir)/new-visit-meth/*.h      $(srcdir)/orig-visit-meth

check-local: remedy-all
	$(srcdir)/regressiontest.sh $(srcdir)/../mockpp2xml/mockppidl-orig.xml $(srcdir)/  $(top_srcdir)

EXTRA_DIST = \
  regressiontest.sh \
  GenChainableMethod.h \
  GenerateMethod.h \
	GenVisitableMethod.h \
	MockppIdlClass.h \
	MockppIdlParser.h \
	expatwrap.h \
	xmlexcept.h \
	xmlparse_base.h \
	xmlparser.h

MAINTAINERCLEANFILES =
