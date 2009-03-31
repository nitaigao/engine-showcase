INCLUDES = $(all_includes) -I$(top_srcdir) -I$(top_builddir) $(EA_EXTRA_INC)
DEFAULT_INCLUDES = $(INCLUDES)

CLEANFILES = *.~* *.~~* *~  gmon.out *profil.txt *.bbg *.gcno *.gcda

###########################################################################

if WORKING_WIN32
win32_app = mock_test_win32
endif

if WORKING_QT
qt_app = mock_test_qt
endif

if WORKING_X11
x11_app = mock_test_x11
endif

check_PROGRAMS = mock_test  vm2 \
                 $(x11_app) $(qt_app) $(win32_app) \
                 readability_test check_inst

SINGLE_TESTS   = $(check_PROGRAMS)

TESTS          = $(SINGLE_TESTS)

.PHONY: check_inst.cpp

profil: check
	gprof .libs/mock_test >>mockpp-profil.txt

coverage: check
	gcov IsNot.h

###########################################################################

AM_LDFLAGS     = $(all_libraries)

AM_CXXFLAGS = -I$(QTDIR)/include -I/usr/X11R6/include

###########################################################################

if BUILTIN_CXXTEST
cxxtest_lib=$(top_builddir)/3party/cxxtest/cxxtest/libmockpp_cxxtest.la
endif

if WORKING_QT
mock_test_qt_LDADD = $(top_builddir)/mockpp/libmockpp.la \
	$(cxxtest_lib) $(LIBDL) -lstdc++ $(EA_EXTRA_LIB)  -lqt-mt
endif

if WORKING_X11
mock_test_x11_LDADD = $(top_builddir)/mockpp/libmockpp.la \
	$(cxxtest_lib) $(LIBDL) -lstdc++ $(EA_EXTRA_LIB)  -lX11
endif

if WORKING_WIN32
mock_test_win32_LDADD = $(top_builddir)/mockpp/libmockpp.la \
	$(cxxtest_lib) $(LIBDL) -lstdc++ $(EA_EXTRA_LIB)  -lwindows
endif

mock_test_LDADD = $(top_builddir)/mockpp/libmockpp.la \
	$(top_builddir)/mockpp/production/libmockpp_production.la \
	$(EA_EXTRA_LIB) $(LIBBOOST) $(LIBDL) $(cxxtest_lib) -lstdc++

readability_test_LDADD = $(top_builddir)/mockpp/libmockpp.la \
	$(cxxtest_lib) $(LIBDL) -lstdc++ $(EA_EXTRA_LIB)

vm2_LDADD = $(top_builddir)/mockpp/libmockpp.la \
	$(cxxtest_lib) $(LIBDL) -lstdc++ $(EA_EXTRA_LIB)

check_inst_LDADD = $(top_builddir)/mockpp/libmockpp.la \
	$(LIBDL) -lstdc++ $(EA_EXTRA_LIB)

###########################################################################

raw-gen:
	cpp -DHAVE_CPPUNIT -DHAVE_CONFIG_H $(srcdir)/InvocationMocker_test.cpp $(INCLUDES) >x.cpp
	sed -e s,^#,//,g x.cpp >xx.cpp

raw-comp: raw-gen
	indent xx.cpp
	g++ xx.cpp -o xx

###########################################################################

libmocktesting_la_SOURCES = \
	VisitableMockObject_test.cpp \
	Formatter_test.cpp Throwable_test.cpp ThrowableList_test.cpp ExpectationCounter_test.cpp \
	ExpectationCounterRange_test.cpp ExpectationMap_test.cpp ExpectationList_test.cpp \
	ConstraintList_test.cpp 		ExpectationSet_test.cpp ExpectationConglomeration_test.cpp \
	ExpectationSegment_test.cpp 		ExpectationValue_test.cpp ExpectationBoundary_test.cpp \
	Verifiable_test.cpp Exception_test.cpp 		mockpp_test.cpp ReturnObjectList_test.cpp \
	VisitableMockObject_template_test.cpp 		Assert_test.cpp AssertMo_test.cpp MockObject_test.cpp \
	VisitableMockObject_1_test.cpp 		VisitableMockObject_2_test.cpp VisitableMockObject_3_test.cpp \
	VisitableMockObject_4_test.cpp 		VisitableMockObject_5_test.cpp VisitableMockObject_void_test.cpp \
	VisitableMockObject_1_void_test.cpp 		VisitableMockObject_2_void_test.cpp \
	VisitableMockObject_3_void_test.cpp VisitableMockObject_4_void_test.cpp \
	VisitableMockObject_5_void_test.cpp VerifyingTestCase_test.cpp classes_ABCDE.cpp \
	VerifyingTestCaller_test.cpp TrackingCounter_test.cpp MixedMockObject_5_test.cpp TimeServer_test.cpp \
	VisitableMockMethod_2_test.cpp VisitableMockMethod_1_test.cpp VisitableMockMethod_test.cpp \
	ResponseVector_test.cpp NoException_test.cpp \
	And_test.cpp IsAnything_test.cpp IsCloseTo_test.cpp \
	IsEqual_test.cpp IsGreaterOrEqual_test.cpp IsGreaterThan_test.cpp IsInstanceOf_test.cpp \
	IsLessOrEqual_test.cpp IsLessThan_test.cpp IsNot_test.cpp IsNothing_test.cpp IsSame_test.cpp \
	Or_test.cpp ReturnStub_test.cpp RandomStub_test.cpp ReturnObjectListStub_test.cpp ReturnAndTriggerStub_test.cpp StringContains_test.cpp StubSequence_test.cpp \
	TestFailureStub_test.cpp ThrowStub_test.cpp VoidStub_test.cpp ChainingMockObjectSupport_test.cpp \
	CustomStub_test.cpp DefaultResultStub_test.cpp Invocation_test.cpp \
	AnyArgumentsMatcher_test.cpp InvokedRecorder_test.cpp InvokeAtLeastOnceMatcher_test.cpp \
	InvokeOnceMatcher_test.cpp InvokeCountMatcher_test.cpp InvokedAfterMatcher_test.cpp \
	TestFailureMatcher_test.cpp NoArgumentsMatcher_test.cpp ArgumentsMatcher_test.cpp \
	ConstraintSet_test.cpp ArgumentsMatchBuilder_test.cpp InvocationMockerBuilder_test.cpp \
	AbstractDynamicChainingMock_test.cpp AbstractInvocationDispatcher_test.cpp ChainingMockBuilder_test.cpp \
	CoreMock_test.cpp DynamicChainingMockError_test.cpp FIFOInvocationDispatcher_test.cpp \
	InvocationMocker_test.cpp LIFOInvocationDispatcher_test.cpp InvokeAtMostMatcher_test.cpp \
	InvokedBeforeMatcher_test.cpp StubBuilder_test.cpp MatchBuilder_test.cpp \
	InvokeAtLeastMatcher_test.cpp ChainableMockObject_1_test.cpp \
	ChainableMockObject_1_void_test.cpp ChainableMockObject_2_test.cpp ChainableMockObject_2_void_test.cpp \
	ChainableMockObject_3_test.cpp ChainableMockObject_3_void_test.cpp ChainableMockObject_4_test.cpp \
	ChainableMockObject_4_void_test.cpp ChainableMockObject_5_test.cpp ChainableMockObject_5_void_test.cpp \
	ChainableMockObject_test.cpp ChainableMockObject_void_test.cpp TypelessStubSequence_test.cpp \
	StringEndsWith_test.cpp StringStartsWith_test.cpp UnlimitedMatcher_test.cpp mockpp_pti_test.cpp \
	OutBound_test.cpp TriggeredOutbound_test.cpp ChainableMockObjectPolymorphism_test.cpp \
	ChainableMockMethod_test.cpp ChainableMockMethod_1_test.cpp ChainableMockMethod_2_test.cpp

if WORKING_QT
mock_test_qt_SOURCES = mock_test_qt.cpp $(libmocktesting_la_SOURCES)
endif

if WORKING_X11
mock_test_x11_SOURCES = mock_test_x11.cpp $(libmocktesting_la_SOURCES)
endif

if WORKING_WIN32
mock_test_win32_SOURCES = mock_test_win32.cpp $(libmocktesting_la_SOURCES)
endif

mock_test_SOURCES  = mock_test.cpp $(libmocktesting_la_SOURCES)

readability_test_SOURCES = readability_test.cpp classes_ABCDE.cpp

vm2_SOURCES = vm2.cpp

check_inst_SOURCES = check_inst.cpp

noinst_HEADERS = SelectFramework.h classes_ABCDE.h MyBuilderNamespace.h MyMatchBuilder.h IncCounter.h

###########################################################################

mock_test_LDFLAGS = -L/opt/intel_cc_80/lib

if WORKING_QT
mock_test_qt_LDFLAGS = -L/opt/intel_cc_80/lib -L$(QTDIR)/lib
endif

if WORKING_X11
mock_test_x11_LDFLAGS = -L/opt/intel_cc_80/lib -L/usr/X11R6/lib
endif

if WORKING_WIN32
mock_test_win32_LDFLAGS = -L/opt/intel_cc_80/lib -L/windows
endif

readability_test_LDFLAGS = -L/opt/intel_cc_80/lib

check_inst_LDFLAGS = -L/opt/intel_cc_80/lib

###########################################################################

mock_test_count:
	list=mock_test.files; \
	test -f $$list && rm $$list; \
	for f in $(mock_test_SOURCES); do \
	  echo $$f >> $$list.tmp; \
	done; \
	cat $$list.tmp | wc -l ; echo "Dateien in mock_test"; \
	cat $$list.tmp | sort >$$list

###########################################################################

VALGRIND_OPTS=--show-reachable=yes --leak-check=yes -v
VG_SUPPRESS=vg-suppress-`hostname`.txt

# --suppressions=$(srcdir)/$(VG_SUPPRESS)
 vg-mock:
	-rm *vglog*
	LD_LIBRARY_PATH=../.libs  valgrind --tool=memcheck $(VALGRIND_OPTS) --logfile=mock.vglog ./.libs/lt-mock_test; \
	grep "mock[^/]" *vglog* || cat *vglog*

#create suppression file for valgrind
vg-setup:
	for fut in $(SINGLE_TESTS); do \
	  LD_LIBRARY_PATH=../.libs  valgrind  --tool=memcheck --gen-suppressions=yes $(VALGRIND_OPTS) .libs/$$fut; \
	done;
	echo "Store all the suppression block to $(srcdir)/$(VG_SUPPRESS)"

###########################################################################

check_inst.cpp:
	-rm $(top_srcdir)/mockpp/chainable-template.h
	-rm $(top_srcdir)/mockpp/visitable-template.h
	echo ""                                                      >check_inst.cpp
	echo "// Don't edit, this file is automatically generated"  >>check_inst.cpp
	echo "// to check #include-ing of *all* headers"            >>check_inst.cpp
	echo "// /*"                                                   >>check_inst.cpp
	(N=`pwd`/check_inst.cpp; cd $(srcdir)/../..; find . -name "*.h" \
	        | grep -v "mockpp_config" | grep -v "/config/" | grep -v "/cxxtest/" | grep -v "/tests/" | grep -v "/ministl/" | grep -v "/docs/" | grep -v "/examples/" | grep -v "/generator/"\
	        | sed -e "s,\./,#include <,g" -e "s/\.h/\.h>/g" >>$$N)
	echo "// */"             >>check_inst.cpp
	echo "int main()"     >>check_inst.cpp
	echo "{"              >>check_inst.cpp
	echo " return 0;"     >>check_inst.cpp
	echo "}"              >>check_inst.cpp

