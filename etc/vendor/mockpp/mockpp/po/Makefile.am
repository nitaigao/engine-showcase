# EXTRA_DIST = $(srcdir)/$(PACKAGE).pot $(POTEMPLATES) $(GMOFILES)

GMOFILES      = de.gmo
POTEMPLATES   = $(GMOFILES:.gmo=.po)

CLEANFILES = *.~* *.~~* *~

merge:
	@catalogs='$(POTEMPLATES)'; \
	for cat in $$catalogs; do \
	name=$(PACKAGE).pot ; \
	echo $(srcdir)/$$cat $(srcdir)/$$name; \
	msgmerge $(srcdir)/$$cat $(srcdir)/$$name > $(srcdir)/$$cat.new ; \
	if diff $(srcdir)/$$cat $(srcdir)/$$cat.new; then \
		rm $(srcdir)/$$cat.new;  \
	else  \
		mv $(srcdir)/$$cat.new $(srcdir)/$$cat ; \
	fi; \
	done

.po.mo:
	$(MSGFMT) -o $@ $<

.po.gmo:
	rm -f $(srcdir)/$*.gmo
	$(MSGFMT) -o $(srcdir)/$*.gmo $<

localedir = $(kde_locale)

xx-install-yes: all
	@catalogs='$(GMO_INSTALL)'; \
	for cat in $$catalogs; do \
	destdir=$(localedir); \
	lang=`echo $$cat | sed 's/\.kde[12]\.gmo//'`; \
	name=$(PACKAGE).mo \
	dir=$$destdir/$$lang/LC_MESSAGES; \
	$(mkinstalldirs) $$dir; \
	$(INSTALL_DATA) $$cat $$dir/$$name; \
	  echo "installing $$cat as $$dir/$$name"; \
	done

xx-uninstall-local:
	@catalogs='$(GMO_INSTALL)'; \
	for cat in $$catalogs; do \
	destdir=$(localedir); \
	lang=`echo $$cat | sed 's/\.gmo//'`; \
	name=$(PACKAGE).mo; \
	dir=$$destdir/$$lang/LC_MESSAGES; \
	rm -f $$cat $$dir/$$name; \
	echo "removing $$dir/$$name" ; \
	done
