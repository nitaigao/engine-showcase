CLEAN_FILES = html/*

.PHONY: api-index apidoc apidoc-dist remote-install remote-install-raw

remote-install-raw: apidoc all
	-scp html/*.html html/*.png html/*.gif html/*.css ewald-arnold@mockpp.sourceforge.net:/home/groups/m/mo/mockpp/htdocs/api-doc

remote-install: apidoc-dist
	-scp  $(top_builddir)/$(PACKAGE)-$(VERSION)-apidoc.zip  ewald-arnold@mockpp.sourceforge.net:/home/groups/m/mo/mockpp/htdocs
	echo "###########################"
	echo "Auspacken nicht vergessen"
	echo "###########################"

doc: apidoc

apidoc:
	-test -L tutorial && rm tutorial
	ln -s ../../examples/tutorial/html tutorial
	-test -L handbook && rm handbook
	ln -s ../../docs/en/var handbook
	-test -d html && rm -rf html
	$(DOXYGEN) $(top_builddir)/mockpp.doxygen
	-test -f $(PNGCRUSH) &&  $(PNGCRUSH) -d new-png html/*.png && cp new-png/*.png html/ && rm -r new-png

apidoc-dist: apidoc
	-test -r $(PACKAGE)-$(VERSION)-apidoc.tar.bz2 && rm $(PACKAGE)-$(VERSION)-apidoc.tar.bz2
	-test -r $(PACKAGE)-$(VERSION)-apidoc.tar.gz && rm $(PACKAGE)-$(VERSION)-apidoc.tar.gz
	-test -r $(PACKAGE)-$(VERSION)-apidoc.tar.zip && rm $(PACKAGE)-$(VERSION)-apidoc.tar.zip
	-test -d $(PACKAGE)-$(VERSION)-apidoc && rm -rf $(PACKAGE)-$(VERSION)-apidoc
	mkdir -p $(PACKAGE)-$(VERSION)-apidoc/api-doc
	-find html | xargs -n 1 cp -v -t $(PACKAGE)-$(VERSION)-apidoc/api-doc
	-rm $(PACKAGE)-$(VERSION)-apidoc/api-doc/*.md5
	-rm $(PACKAGE)-$(VERSION)-apidoc/api-doc/*.map
	cp $(srcdir)/README $(PACKAGE)-$(VERSION)-apidoc/
	test -f mockpp.tag && cp mockpp.tag  $(PACKAGE)-$(VERSION)-apidoc
	-test -r tar.lst && rm tar.lst
	find $(PACKAGE)-$(VERSION)-apidoc/api-doc | xargs -n 1 echo >>tar.lst
	tar cf $(PACKAGE)-$(VERSION)-apidoc.tar --files-from=tar.lst
	$(ZIPPER) $(PACKAGE)-$(VERSION)-apidoc.tar
	mv $(PACKAGE)-$(VERSION)-apidoc.tar.$(ZIPPER_EXT) $(top_builddir)
	zip -r $(PACKAGE)-$(VERSION)-apidoc.zip . -i '$(PACKAGE)-$(VERSION)-apidoc/*'
	mv $(PACKAGE)-$(VERSION)-apidoc.zip $(top_builddir)
	rm -rf $(PACKAGE)-$(VERSION)-apidoc


release-check: gz-dist
	sh $(srcdir)/release-check  $(distdir) `pwd`

#################################################################

htmldir = $(pkgdatadir)/html

if DOXYGEN_DOC

install-data-hook: doc
	$(mkdir_p)                       $(DESTDIR)$(htmldir)/api-doc
	-find html | xargs -n 1 cp -v -t $(DESTDIR)$(htmldir)/api-doc
#	$(INSTALL) html/*                $(DESTDIR)$(htmldir)/api-doc

else

install-data-hook:
	@echo ""
	@echo "***********************************************************"
	@echo "* Installation of then api documentation is disabled.     *"
	@echo "* Use --enable-doxygen to turn this on.                   *"
	@echo "***********************************************************"
	@echo ""

endif

# Automake's "distcheck" is sensitive to having files left over
# after "make uninstall", so we have to clean up the install hook.
uninstall-local:
	rm -rf $(DESTDIR)$(htmldir)/api-doc

#################################################################
