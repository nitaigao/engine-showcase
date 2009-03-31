INCLUDES = $(all_includes) -I$(top_srcdir) -I$(top_srcdir)/3party/cxxtest -I$(top_builddir) $(EA_EXTRA_INC)
DEFAULT_INCLUDES = $(INCLUDES)

if BUILTIN_CXXTEST

lib_LTLIBRARIES = libmockpp_cxxtest.la

libmockpp_cxxtest_la_LDFLAGS = -no-undefined -version-info \
	$(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

libmockpp_cxxtestincludedir = $(includedir)/mockpp/3party/cxxtest

libmockpp_cxxtest_la_SOURCES = \
	Descriptions.cpp \
	DummyDescriptions.cpp \
	GlobalFixture.cpp \
	LinkedList.cpp \
	RealDescriptions.cpp \
	TestSuite.cpp \
	TestTracker.cpp \
	ValueTraits.cpp
#	Root.cpp

libmockpp_cxxtestinclude_HEADERS = \
	Descriptions.h \
	DummyDescriptions.h \
	ErrorFormatter.h \
	ErrorPrinter.h \
	Flags.h \
	GlobalFixture.h \
	Gui.h \
	LinkedList.h \
	Mock.h \
	ParenPrinter.h \
	QtGui.h \
	RealDescriptions.h \
	SelfTest.h \
	StdHeaders.h \
	StdioFilePrinter.h \
	StdioPrinter.h \
	StdValueTraits.h \
	TeeListener.h \
	TestListener.h \
	TestRunner.h \
	TestSuite.h \
	TestTracker.h \
	ValueTraits.h \
	Win32Gui.h \
	X11Gui.h \
	YesNoRunner.h

endif
