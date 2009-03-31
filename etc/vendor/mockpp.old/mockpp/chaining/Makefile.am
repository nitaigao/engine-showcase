INCLUDES = $(all_includes) -I$(top_srcdir) -I$(top_builddir) $(EA_EXTRA_INC)
DEFAULT_INCLUDES = $(INCLUDES)

noinst_LTLIBRARIES = libchaining.la

libchaininginclude_HEADERS = AbstractDynamicChainingMock.h \
			AbstractInvocationDispatcher.h ChainingMockBuilder.h ChainingMockObjectSupport.h CoreMock.h \
			DynamicChainingMock.h DynamicChainingMockError.h FIFOInvocationDispatcher.h Invocation.h \
			InvocationDispatcher.h InvocationMocker.h Invokable.h LIFOInvocationDispatcher.h \
			StubMatchersCollection.h ChainableMockObject_macro.h mockpp_pti.h \
			ChainableMockMethod.h \
			ChainableMockMethod0.h \
			ChainableMockMethod1.h \
			ChainableMockMethod2.h \
			ChainableMockMethod3.h \
			ChainableMockMethod4.h \
			ChainableMockMethod5.h \
			ChainableMockMethod6.h \
			ChainableMockObject.h CountedChainableMethod.h \
      ReturnValueAndTrigger.h InvocationN.h OnConsecutiveCalls.h

EXTRA_DIST = \
   gen_chainablemethod_N.pl \
   gen_countchainable_N.pl \
   gen_consecutive_N.pl \
   gen_invocation_N.pl \
   gen_returntrigger_N.pl

libchainingincludedir = $(includedir)/mockpp/chaining

CLEANFILES = *.~* *.~~* *~ *.old

libchaining_la_SOURCES = \
  ChainingMockObjectSupport.cpp Invocation.cpp \
  ChainableMockObject.cpp

noinst_HEADERS = mockpp_pti.h

install-data-hook:
	@echo "******************************"
	@echo "*** installing optional headers in chaining"
	@for i in 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28; do \
	  file=ChainableMockMethod$${i}.h; \
	  test -f $(srcdir)/$$file && (echo "*** $$file"; $(INSTALL) $(srcdir)/$$file $(DESTDIR)$(libchainingincludedir)/$$file); \
	done; \
	echo "******************************"
