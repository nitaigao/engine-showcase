INCLUDES = $(all_includes) -I$(top_srcdir) -I$(top_builddir) $(EA_EXTRA_INC)
DEFAULT_INCLUDES = $(INCLUDES)

noinst_LTLIBRARIES = libstub.la

libstubinclude_HEADERS = CustomStub.h DefaultResultStub.h ReturnStub.h ReturnObjectListStub.h \
	StubSequence.h TestFailureStub.h ThrowStub.h VoidStub.h TypelessStubSequence.h \
	TypelessStub.h 	StubHolder.h Stub.h ReturnAndTriggerStub.h TriggerStub.h RandomStub.h

libstubincludedir = $(includedir)/mockpp/stub

CLEANFILES = *.~* *.~~* *~ *.old

EXTRA_DIST = \
   gen_returntriggerstub_N.pl \
   gen_triggerstub_N.pl

libstub_la_SOURCES = VoidStub.cpp
