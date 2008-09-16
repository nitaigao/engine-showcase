SUBDIRS = common images

remote-install: html-files all
	-scp var/*.html                       ewald-arnold@mockpp.sourceforge.net:/home/groups/m/mo/mockpp/htdocs/handbook
	-scp $(srcdir)/common/*.css           ewald-arnold@mockpp.sourceforge.net:/home/groups/m/mo/mockpp/htdocs/handbook/common
	-scp $(srcdir)/images/*.png           ewald-arnold@mockpp.sourceforge.net:/home/groups/m/mo/mockpp/htdocs/handbook/images
	-scp $(srcdir)/images/callouts/*.png  ewald-arnold@mockpp.sourceforge.net:/home/groups/m/mo/mockpp/htdocs/handbook/images/callouts

CLEANFILES = *.~* *.~~* *~

doc-dist: doc
	-test -d $(PACKAGE)-$(VERSION)-handbook && rm -rf $(PACKAGE)-$(VERSION)-handbook
	mkdir -p mockpp-$(VERSION)-handbook/handbook/
	cp -R -H var/*  mockpp-$(VERSION)-handbook/handbook
	cp $(srcdir)/README mockpp-$(VERSION)-handbook/
	tar cf mockpp-$(VERSION)-handbook.tar mockpp-$(VERSION)-handbook/*
	$(ZIPPER) mockpp-$(VERSION)-handbook.tar
	test -f mockpp-$(VERSION)-handbook.tar.$(ZIPPER_EXT) && mv mockpp-$(VERSION)-handbook.tar.$(ZIPPER_EXT) $(top_builddir)
	-zip  -r mockpp-$(VERSION)-handbook.zip mockpp-$(VERSION)-handbook/*
	test -f mockpp-$(VERSION)-handbook.zip && mv mockpp-$(VERSION)-handbook.zip $(top_builddir)
#	rm -r mockpp-$(VERSION)-handbook

doc: html-files pdf-files

################################################################

html-chunk-files:
	-test -L tutorial && rm tutorial
	ln -s ../../examples/tutorial/html tutorial
	-test -L api-doc && rm api-doc
	ln -s ../../docs/api/html api-doc
	-rm -rf ./var/common
	-rm -rf ./var/images
	mkdir -pv var/common
	mkdir -pv var/images/callouts
	cp $(srcdir)/common/*.css           var/common
	cp $(srcdir)/images/*.png           var/images
	cp $(srcdir)/images/callouts/*.png  var/images/callouts
	xsltproc --nonet -o var/  $(srcdir)/customize-chunked.xsl  $(srcdir)/index.docbook

html-flat-files:
	-test -L tutorial && rm tutorial
	ln -s ../../examples/tutorial/html tutorial
	-test -L api-doc && rm api-doc
	ln -s ../../docs/api/html api-doc
	mkdir -pv var/common
	mkdir -pv var/images/callouts
	-rm -rf ./var/*.html
	-rm -rf ./var/common
	-rm -rf ./var/images
	cp $(srcdir)/common/*.css           var/common
	cp $(srcdir)/images/*.png           var/images
	cp $(srcdir)/images/callouts/*.png  var/images/callouts
	xsltproc --nonet -o var/  $(srcdir)/customize-flat.xsl  $(srcdir)/index.docbook

html-files: html-chunk-files

################################################################

pdf-files:
	mkdir -pv var
	-rm -rf ./var/*.fo
	-rm -rf ./var/*.pdf
	cp $(srcdir)/common/*.css           var/common
	cp $(srcdir)/images/*.png           var/images
	cp $(srcdir)/images/callouts/*.png  var/images/callouts
	$(EA_PDF_CATALOG) xsltproc --nonet --output var/mockpp.fo $(srcdir)/customize-pdf.xsl $(srcdir)/index.docbook
	$(FOP) var/mockpp.fo var/mockpp.pdf && cp -v var/mockpp.pdf $(top_builddir)/mockpp-$(VERSION)-handbook.pdf

# -c /home/ea/LinLibertineFont/fop-userfont.xml

svg-files:
	xsltproc --nonet -output var/mockpp.fo http://docbook.sourceforge.net/release/xsl/current/fo/docbook.xsl $(srcdir)/index.docbook
	$(FOP) var/mockpp.fo -svg var/mockpp.svg

.PHONY: html-files pdf-files doc-dist remote-install doc \
        svg-files html-chunk-files html-flat-files

#################################################################

htmldir = $(pkgdatadir)/html

if DOCBOOK_DOC

install-data-hook: doc
	$(mkdir_p)                                 $(DESTDIR)$(htmldir)/handbook/common
	$(mkdir_p)                                 $(DESTDIR)$(htmldir)/handbook/images/callouts
	$(INSTALL) var/mockpp.pdf                  $(DESTDIR)$(pkgdatadir)/mockpp-$(VERSION)-handbook.pdf
	$(INSTALL) var/*.html                      $(DESTDIR)$(htmldir)/handbook
	$(INSTALL) $(srcdir)/common/*.css          $(DESTDIR)$(htmldir)/handbook/common
	$(INSTALL) $(srcdir)/images/*.png          $(DESTDIR)$(htmldir)/handbook/images
	$(INSTALL) $(srcdir)/images/callouts/*.png $(DESTDIR)$(htmldir)/handbook/images/callouts

else

install-data-hook:
	@echo ""
	@echo "***********************************************************"
	@echo "* Installation of the handbook is disabled.               *"
	@echo "* Use --enable-docbook to turn this on.                   *"
	@echo "***********************************************************"
	@echo ""

endif

# Automake's "distcheck" is sensitive to having files left over
# after "make uninstall", so we have to clean up the install hook.
uninstall-local:
	rm -rf $(DESTDIR)$(htmldir)/handbook
	rm -f  $(pkgdatadir)/mockpp-$(VERSION)-handbook.pdf

#################################################################


EXTRA_DIST=appendix.docbook bookinfo.docbook credits.docbook customize.xsl  customize-flat.xsl \
	dev_advanced.docbook dev_advanced_easymock.docbook customize-chunked.xsl customize-pdf.xsl \
	dev_advanced_intro.docbook dev_poor_mans_mock.docbook dev_advanced_jmock.docbook dev_basic.docbook \
	dev_helper.docbook dev_intro.docbook faq.docbook index.docbook dev_production.docbook \
	dev_embedded.docbook dev_test_framework.docbook generator.docbook
