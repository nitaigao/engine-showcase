INCLUDES = $(all_includes) -I$(top_srcdir) -I$(top_builddir) $(EA_EXTRA_INC)
DEFAULT_INCLUDES = $(INCLUDES)

libbuilderinclude_HEADERS = ArgumentsMatchBuilder.h \
                            ArgumentsMatchBuilderN.h \
                            BuilderNamespace.h \
                            IdentityBuilder.h \
                            InvocationMockerBuilder.h \
                            MatchBuilder.h \
                            StubBuilder.h

libbuilderincludedir = $(includedir)/mockpp/builder

EXTRA_DIST = \
   gen_argumentsmatchbuilder_N.pl 

CLEANFILES = *.~* *.~~* *~ *.old

noinst_LTLIBRARIES = libbuilder.la

libbuilder_la_SOURCES = BuilderNamespace.cpp IdentityBuilder.cpp
