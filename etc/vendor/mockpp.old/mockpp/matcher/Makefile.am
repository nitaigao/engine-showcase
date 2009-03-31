INCLUDES = $(all_includes) -I$(top_srcdir) -I$(top_builddir) $(EA_EXTRA_INC)
DEFAULT_INCLUDES = $(INCLUDES)

noinst_LTLIBRARIES = libmatcher.la

libmatcherinclude_HEADERS = AnyArgumentsMatcher.h ArgumentsMatcher.h \
	InvokeAtLeastOnceMatcher.h InvokeCountMatcher.h InvokedAfterMatcher.h InvokedRecorder.h \
	InvokeOnceMatcher.h NoArgumentsMatcher.h StatelessInvocationMatcher.h TestFailureMatcher.h \
	InvokeAtMostMatcher.h InvokedBeforeMatcher.h InvokeAtLeastMatcher.h InvocationMatcher.h \
	TypelessMatcher.h MatcherHolder.h  UnlimitedMatcher.h

libmatcherincludedir = $(includedir)/mockpp/matcher

libmatcher_la_SOURCES = InvokeAtLeastMatcher.cpp InvokeAtMostMatcher.cpp \
	InvokeCountMatcher.cpp InvokedRecorder.cpp InvokeOnceMatcher.cpp TestFailureMatcher.cpp \
	TypelessMatcher.cpp InvokeAtLeastOnceMatcher.cpp UnlimitedMatcher.cpp

CLEANFILES = *.~* *.~~* *~ *.old
