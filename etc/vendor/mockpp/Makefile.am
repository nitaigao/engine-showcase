SUBDIRS = tool config 3party mockpp bcb5 bcb10 msvc71 msvc8 generator

LIBTOOL_DEPS = @LIBTOOL_DEPS@

libtool : $(LIBTOOL_DEPS)
	$(SHELL)  ./config.status --recheck

EXTRA_DIST = \
  bug-report \
  mockpp.pc.in \
  configure.in \
  mockpp.doxygen.in \
  mockpp.kdevelop \
  AUTHORS \
  COPYING \
  ChangeLog \
  INSTALL \
  README \
  TODO \
  Makefile.cvs \
  mockpp.lsm \
  gen_files_N.sh \
  acinclude.m4

CLEANFILES = *.~* *.~~* *~ cachegrind.out.*

.PHONY: dist src-dist zip-dist bz2-dist full-dist doc-dist pkgconfig

AUTOMAKE_OPTIONS = foreign

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = mockpp.pc

doc-dist:
	-rm -rf $(top_builddir)/mockpp/docs/api/html
	(cd $(top_builddir)/mockpp/docs/api && make apidoc-dist)
	(cd $(top_builddir)/mockpp/docs/en && make doc-dist)
	(cd $(top_builddir)/mockpp/examples/tutorial && make tutorial-dist)

doc:
	-rm -rf $(top_builddir)/mockpp/docs/api/html
	(cd $(top_builddir)/mockpp/docs/api && make apidoc)
	(cd $(top_builddir)/mockpp/docs/en && make doc)
	(cd $(top_builddir)/mockpp/examples/tutorial && make tutorial)

maintainer-clean: maintainer-clean-generic

src-dist: distdir
	zip -r $(distdir)-src.zip.tmp  $(distdir)/*
	$(AMTAR) chof - $(distdir) | $(ZIPPER) -c >$(distdir)-src.tar.$(ZIPPER_EXT).tmp
	$(AMTAR) chof - $(distdir) | gzip -c >$(distdir)-src.tar.gz.tmp
	mv $(distdir)-src.zip.tmp $(top_builddir)/$(distdir)-src.zip
	mv $(distdir)-src.tar.$(ZIPPER_EXT).tmp $(top_builddir)/$(distdir)-src.tar.$(ZIPPER_EXT)
	mv $(distdir)-src.tar.gz.tmp $(top_builddir)/$(distdir)-src.tar.gz
	-chmod -R a+w $(distdir) >/dev/null 2>&1; rm -rf $(distdir)
	-rm $(distdir)-src.zip.asc
	gpg -ba $(distdir)-src.zip
	-rm $(distdir)-src.tar.$(ZIPPER_EXT).asc
	gpg -ba $(distdir)-src.tar.$(ZIPPER_EXT)
	-rm $(distdir)-src.tar.gz.asc
	gpg -ba $(distdir)-src.tar.gz


full-dist: doc-dist src-dist

zip-dist: distdir
	zip  -r $(distdir)-src.zip  $(distdir)/*
	-chmod -R a+w $(distdir) >/dev/null 2>&1; rm -rf $(distdir)

gz-dist: distdir
	$(AMTAR) chof - $(distdir) | gzip -c >$(distdir).tar.gz
	-chmod -R a+w $(distdir) >/dev/null 2>&1; rm -rf $(distdir)

bz2-dist: distdir
	$(AMTAR) chof - $(distdir) | bzip2 -c >$(distdir).tar.bz2
	-chmod -R a+w $(distdir) >/dev/null 2>&1; rm -rf $(distdir)

#	cp $(distdir)-src.tar.$(ZIPPER_EXT) /usr/src/rpm/SOURCES
rpm: mockpp.spec # bz2-dist
	cp $(distdir)-src.tar.$(ZIPPER_EXT) /usr/src/packages/SOURCES
	rpmbuild -vv -bb mockpp.spec

install-data-local:
	@echo ""
	@echo "***********************************************************"
	@echo "* Installed. Enjoy mockpp and have a happy mocking.       *"
	@echo "* Please report bugs at http://mockpp.sf.net              *"
	@echo "***********************************************************"
	@echo ""

upload-bz2:
	scp mockpp-$(VERSION)-src.tar.bz2  ewald-arnold@mockpp.sourceforge.net:/home/groups/m/mo/mockpp/htdocs/download

release-check: gz-dist
	sh $(srcdir)/release-check.sh  $(distdir) `pwd`

MAINTAINERCLEANFILES = subdirs configure.in acinclude.m4 configure.files

#################################################################

htmldir = $(DESTDIR)$(pkgdatadir)/html

install-data-hook:
	$(mkdir_p) $(htmldir)
	$(INSTALL) $(srcdir)/AUTHORS $(srcdir)/COPYING $(srcdir)/README $(srcdir)/ChangeLog   $(DESTDIR)$(pkgdatadir)

#################################################################
