INCLUDES = $(all_includes) -I$(top_srcdir) -I$(top_builddir) $(EA_EXTRA_INC)
DEFAULT_INCLUDES = $(INCLUDES)

LDADD         = ../../libmockpp.la $(EA_EXTRA_LIB) $(CPPUNIT_LIBS) $(LIBDL) -lstdc++

AM_LDFLAGS    = $(all_libraries)

noinst_PROGRAMS  = visitmock visitmock2 chainmock chainmock2 \
                   basicmock poormock poormock-se cppunit verifying

EXTRA_DIST = \
  tutorial.doxygen.in \
  tutorial.mak \
  tutorial.kdevelop \
  interface.h \
  Calculator.h \
  consumer.h \
  tutorial.dox \
  tut-basic.dox \
  tut-visit.dox \
  tut-visit2.dox \
  tut-chain.dox \
  tut-chain2.dox \
  tut-poor.dox \
  tut-poor-se.dox \
  tut-verifying.dox \
  tut-cppunit.dox

all:  bin

TESTS        = $(noinst_PROGRAMS)

check_PROGRAMS: $(noinst_PROGRAMS)

remote-install: tutorial
	-scp html/*.html html/*.png html/*.css html/*.php html/*.idx ewald-arnold@mockpp.sourceforge.net:/home/groups/m/mo/mockpp/htdocs/tutorial

tutorial:
	-test -d html && rm -rf html
	sed -e "s#<filename>#<filename>../api-doc/#g" ../../docs/api/mockpp.tag >mockpp-reloc.tag
	sed -e "s#SRCDIR/#$(srcdir)#g" \
      -e "s#TOP_SRCDIR#$(top_srcdir)#g" \
      -e "s#MOCKPP_VERSION#$(MOCKPP_VERSION)#g"  \
  $(srcdir)/tutorial.doxygen.in >tmp.doxygen
	$(DOXYGEN) tmp.doxygen
	-test -L api-doc && rm api-doc
	ln -s ../../docs/api/html api-doc
	-test -L handbook && rm handbook
	ln -s ../../docs/en/var handbook
	rm tmp.doxygen

tutorial-dist: tutorial
	mkdir -p mockpp-$(VERSION)-tutorial/tutorial/listings
	cp $(srcdir)/*.cpp $(srcdir)/*.h $(srcdir)/*.mak mockpp-$(VERSION)-tutorial/tutorial/listings
	cp $(srcdir)/README mockpp-$(VERSION)-tutorial
	cp -R html/* $(PACKAGE)-$(VERSION)-tutorial/tutorial
	tar cf mockpp-$(VERSION)-tutorial.tar --exclude=CVS  mockpp-$(VERSION)-tutorial/*
	$(ZIPPER) mockpp-$(VERSION)-tutorial.tar
	test -f mockpp-$(VERSION)-tutorial.tar.$(ZIPPER_EXT) && mv mockpp-$(VERSION)-tutorial.tar.$(ZIPPER_EXT) $(top_builddir)
	zip  -r mockpp-$(VERSION)-tutorial.zip mockpp-$(VERSION)-tutorial/*
	test -f mockpp-$(VERSION)-tutorial.zip && mv mockpp-$(VERSION)-tutorial.zip $(top_builddir)
	rm -r mockpp-$(VERSION)-tutorial

bin: $(noinst_PROGRAMS)

cppunit_SOURCES      = cppunit.cpp

verifying_SOURCES    = verifying.cpp

visitmock_SOURCES    = visitmock.cpp consumer.cpp

visitmock2_SOURCES   = visitmock2.cpp consumer.cpp

chainmock_SOURCES    = chainmock.cpp consumer.cpp

chainmock2_SOURCES   = chainmock2.cpp consumer.cpp

poormock_SOURCES     = poormock.cpp consumer.cpp

poormock_se_SOURCES  = poormock-se.cpp consumer.cpp

basicmock_SOURCES    = basicmock.cpp consumer.cpp

#################################################################

htmldir = $(pkgdatadir)/html

if DOXYGEN_DOC

install-data-hook: tutorial
	$(mkdir_p) $(DESTDIR)$(htmldir)/tutorial
	$(INSTALL) html/* $(DESTDIR)$(htmldir)/tutorial

else

install-data-hook:
	@echo ""
	@echo "***********************************************************"
	@echo "* Installation of the tutorial is disabled.               *"
	@echo "* Use --enable-doxygen to turn this on.                   *"
	@echo "***********************************************************"
	@echo ""

endif

# Automake's "distcheck" is sensitive to having files left over
# after "make uninstall", so we have to clean up the install hook.
uninstall-local:
	rm -rf $(DESTDIR)$(htmldir)/tutorial

#################################################################

