INCLUDES = $(all_includes) -I$(top_srcdir) -I$(top_builddir) $(EA_EXTRA_INC)
DEFAULT_INCLUDES = $(INCLUDES)

noinst_LTLIBRARIES = libvisiting.la

libvisitinginclude_HEADERS = \
VisitableMockObject_macro.h VisitableMockObject_template.h VisitableMockObject.h \
VisitableMockMethod.h VisitableMockMethod0.h VisitableMockMethod1.h VisitableMockMethod2.h \
	VisitableMockMethod3.h VisitableMockMethod4.h VisitableMockMethod5.h VisitableMockMethod6.h \
	ResponseVector1.h ResponseVector2.h \
	ResponseVector3.h ResponseVector4.h ResponseVector5.h ResponseVector6.h \
	CountedVisitableMethod.h

EXTRA_DIST = \
   gen_visitablemethod_N.pl \
   gen_countvisitable_N.pl \
   gen_responsevector_N.pl

libvisitingincludedir = $(includedir)/mockpp/visiting

CLEANFILES = *.~* *.~~* *~ *.old

libvisiting_la_SOURCES = \
	VisitableMockMethod.cpp  VisitableMockObject.cpp

noinst_HEADERS =

install-data-hook:
	@echo "******************************"
	@echo "*** installing optional headers in visiting"
	@for i in 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28; do \
	  file=VisitableMockMethod$${i}.h; \
	  test -f $(srcdir)/$$file && (echo "*** $$file"; $(INSTALL) $(srcdir)/$$file $(DESTDIR)$(libvisitingincludedir)/$$file); \
	  file=ResponseVector$${i}.h; \
	  test -f $(srcdir)/$$file && (echo "*** $$file"; $(INSTALL) $(srcdir)/$$file $(DESTDIR)$(libvisitingincludedir)/$$file); \
	done; \
	echo "******************************"
  
