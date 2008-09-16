INCLUDES = $(all_includes) -I$(top_srcdir) -I$(top_builddir) $(EA_EXTRA_INC)
DEFAULT_INCLUDES = $(INCLUDES)

SUBDIRS = chaining visiting compat util builder constraint matcher stub \
          production framework \
          . config docs tests po examples

lib_LTLIBRARIES = libmockpp.la

libmockpp_la_LDFLAGS = -no-undefined \
 -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

libmockpp_la_LIBADD = $(top_builddir)/mockpp/production/libmockpp_production.la \
	$(top_builddir)/mockpp/util/libutil.la $(top_builddir)/mockpp/stub/libstub.la \
	$(top_builddir)/mockpp/matcher/libmatcher.la $(top_builddir)/mockpp/constraint/libconstraint.la \
	$(top_builddir)/mockpp/compat/libcompat.la $(top_builddir)/mockpp/chaining/libchaining.la \
	$(top_builddir)/mockpp/visiting/libvisiting.la $(top_builddir)/mockpp/framework/libframework.la \
	$(top_builddir)/mockpp/builder/libbuilder.la \
	$(EA_EXTRA_LIB) $(CPPUNIT_LIBS)

libmockpp_la_SOURCES = ExpectationCounter.cpp mockpp.cpp Throwable.cpp \
			ThrowableList.cpp MockObject.cpp VerifiableList.cpp \
			Verifiable.cpp SelfDescribing.cpp\
			TrackingCounter.cpp MixedMockObject.cpp MockTimeServer.cpp

pkginclude_HEADERS = mockpp_config.h ReturnObjectList.h Expectation.h \
		AbstractExpectation.h ExpectationValue.h ExpectationSegment.h ExpectationList.h \
		ExpectationSet.h ExpectationMap.h ExpectationBoundary.h AbstractExpectationCollection.h \
		ExpectationConglomeration.h \
		ExpectationCounter.h mockpp.h Throwable.h ThrowableList.h MockObject.h \
		VerifiableList.h Verifiable.h SelfDescribing.h \
		TrackingCounter.h MixedMockObject.h mockpp_dbc.h MockTimeServer.h \
	CountParameters.h

EXTRA_DIST = \
   mockpp_config-bcb5.h \
   mockpp_config-msvc_71.h \
   gen_countparams_N.pl \
   install-win.bat

CLEANFILES = *.~* *.~~* *~ mockpp_config.h *.old

.PHONY: chainer mockpp_config-bcb5.h mockpp_config-msvc_71.h mockpp_config.h metrics templates

metrics:
	TODAY=`date +%y%m%d`; \
	test -d var || mkdir var ; \
    $(top_srcdir)/tool/cxxmetric \
    $(srcdir)/*.h $(srcdir)/*.cpp \
    $(srcdir)/compat/*.h $(srcdir)/compat/*.cpp  \
    $(srcdir)/constraint/*.h $(srcdir)/constraint/*.cpp  \
    $(srcdir)/builder/*.h $(srcdir)/builder/*.cpp  \
    $(srcdir)/matcher/*.h $(srcdir)/matcher/*.cpp  \
    $(srcdir)/chaining/*.h $(srcdir)/chaining/*.cpp  \
    $(srcdir)/stub/*.h $(srcdir)/stub/*.cpp  \
    $(srcdir)/util/*.h $(srcdir)/util/*.cpp  >>var/metrics-lib2-$$TODAY.txt; \
    $(top_srcdir)/tool/cxxmetric $(srcdir)/tests/*.cpp >>var/metrics-test-$$TODAY.txt

templates : visitable-template.h chainable-template.h

chainable-template.h visitable-template.h: $(srcdir)/visitable-template.raw $(pkginclude_HEADERS)
	-cpp -I. -I.. -I $(top_srcdir)  $< >$@.tmp
	filelen=`wc -l $@.tmp | awk '{ print $$1 }'`; \
	splitpos=`grep -n SPLIT_START visitable-template.h.tmp | sed s/:/\ /g | awk '{ print $$1 }'`; \
	tail -n `expr $$filelen - $$splitpos` $@.tmp >$(srcdir)/$@
	rm $@.tmp
	-indent -nut $(srcdir)/$@

mockpp_config.h : ../config.h
	echo "/* Edit this file with care. It is automatically generated from config.h. */" >$@
	TODAY=`date +%Y-%m-%d` ; echo "/* Generated: $$TODAY */" >>$@
	grep "#define[ ]\+VERSION[ ]\+" ../config.h | sed "s/#define[ ]\+VERSION[ ]\+\(.*\)/#define MOCKPP_VERSION \\1/g">>$@
	grep "#define[ ]\+PACKAGE[ ]\+" ../config.h | sed "s/#define[ ]\+PACKAGE[ ]\+\(.*\)/#define MOCKPP_PACKAGE \\1/g">>$@
	grep "MOCKPP_UNICODE "     ../config.h >>$@
	grep "MOCKPP_VERSION_NUMBER " ../config.h >>$@
	grep MOCKPP_UNUSED         ../config.h >>$@
	grep HAVE_LIMITS           ../config.h >>$@
	grep HAVE_WSTRING          ../config.h >>$@
	grep HAVE_VALUES           ../config.h >>$@
	grep CXXTEST_USE_MINI_STL  ../config.h >>$@
	grep MOCKPP_USE_MINI_STL   ../config.h >>$@
	grep MOCKPP_USE_CXXTEST    ../config.h >>$@
	grep MOCKPP_USE_CPPUNIT    ../config.h >>$@
	grep MOCKPP_USE_BOOSTTEST  ../config.h >>$@
	grep MOCKPP_NO_RTTI        ../config.h >>$@
	grep MOCKPP_NO_EXCEPTIONS  ../config.h >>$@
	grep MOCKPP_NO_TEMPLATE_HINT  ../config.h >>$@
	-test $@ -ef $(srcdir) || cp $@ $(srcdir)

mockpp_config-bcb5.h:
	perl  -i.bak -pe "s/(.*mockpp_production)_(\d+)(\.lib|\.dll)/\1_$(LT_CURRENT)\3/g"  $(top_srcdir)/bcb5/*.bpr $(top_srcdir)/bcb5/*.bpf $(top_srcdir)/bcb5/*.bpg
	perl  -i.bak -pe "s/(.*mockpp_cxxtest)_(\d+)(\.lib|\.dll)/\1_$(LT_CURRENT)\3/g"     $(top_srcdir)/bcb5/*.bpr $(top_srcdir)/bcb5/*.bpf $(top_srcdir)/bcb5/*.bpg
	perl  -i.bak -pe "s/\\\\(mockpp)_(\d+)\\\\/\\\\\1_$(LT_CURRENT)\\\\/g"              $(top_srcdir)/bcb5/*.bpr
	perl  -i.bak -pe "s/\\\\(mockpp)_(\d+)/\\\\\1_$(LT_CURRENT)/g"                      $(top_srcdir)/bcb5/*.bat
	perl  -i.bak -pe "s/(.*mockpp)_(\d+)(\.lib|\.dll)/\1_$(LT_CURRENT)\3/g"             $(top_srcdir)/bcb5/*.bpr $(top_srcdir)/bcb5/*.bpf $(top_srcdir)/bcb5/*.bpg
	perl  -i.bak -pe "s/(cppunit)_(\d+)(\.exe)/\1_$(LT_CURRENT)\3/g"                    $(top_srcdir)/bcb5/*.bpr $(top_srcdir)/bcb5/*.bpf
	perl  -i.bak -pe "s/(readability_test)_(\d+)(\.exe)/\1_$(LT_CURRENT)\3/g"           $(top_srcdir)/bcb5/*.bpr $(top_srcdir)/bcb5/*.bpf
	perl  -i.bak -pe "s/(verifying)_(\d+)(\.exe)/\1_$(LT_CURRENT)\3/g"                  $(top_srcdir)/bcb5/*.bpr $(top_srcdir)/bcb5/*.bpf
	perl  -i.bak -pe "s/(basicmock)_(\d+)(\.exe)/\1_$(LT_CURRENT)\3/g"                  $(top_srcdir)/bcb5/*.bpr $(top_srcdir)/bcb5/*.bpf
	perl  -i.bak -pe "s/(chainmock)_(\d+)(\.exe)/\1_$(LT_CURRENT)\3/g"                  $(top_srcdir)/bcb5/*.bpr $(top_srcdir)/bcb5/*.bpf
	perl  -i.bak -pe "s/(chainmock2)_(\d+)(\.exe)/\1_$(LT_CURRENT)\3/g"                  $(top_srcdir)/bcb5/*.bpr $(top_srcdir)/bcb5/*.bpf
	perl  -i.bak -pe "s/(visitmock)_(\d+)(\.exe)/\1_$(LT_CURRENT)\3/g"                  $(top_srcdir)/bcb5/*.bpr $(top_srcdir)/bcb5/*.bpf
	perl  -i.bak -pe "s/(visitmock2)_(\d+)(\.exe)/\1_$(LT_CURRENT)\3/g"                  $(top_srcdir)/bcb5/*.bpr $(top_srcdir)/bcb5/*.bpf
	perl  -i.bak -pe "s/(poormock)_(\d+)(\.exe)/\1_$(LT_CURRENT)\3/g"                   $(top_srcdir)/bcb5/*.bpr $(top_srcdir)/bcb5/*.bpf
	perl  -i.bak -pe "s/(poormock_se)_(\d+)(\.exe)/\1_$(LT_CURRENT)\3/g"                $(top_srcdir)/bcb5/*.bpr $(top_srcdir)/bcb5/*.bpf
	perl  -i.bak -pe "s/(poormock-se)_(\d+)(\.exe)/\1_$(LT_CURRENT)\3/g"                $(top_srcdir)/bcb5/*.bpr $(top_srcdir)/bcb5/*.bpf
	perl  -i.bak -pe "s/(mock_greeter)_(\d+)(\.exe)/\1_$(LT_CURRENT)\3/g"               $(top_srcdir)/bcb5/*.bpr $(top_srcdir)/bcb5/*.bpf
	perl  -i.bak -pe "s/(mock_test)_(\d+)(\.exe)/\1_$(LT_CURRENT)\3/g"                  $(top_srcdir)/bcb5/*.bpr $(top_srcdir)/bcb5/*.bpf
	perl  -i.bak -pe "s/(mock_test_win32)_(\d+)(\.exe)/\1_$(LT_CURRENT)\3/g"                  $(top_srcdir)/bcb5/*.bpr $(top_srcdir)/bcb5/*.bpf
	perl  -i.bak -pe "s/(jmock_test)_(\d+)(\.exe)/\1_$(LT_CURRENT)\3/g"                 $(top_srcdir)/bcb5/*.bpr $(top_srcdir)/bcb5/*.bpf
	echo "// #define's for Borland BCB5" >$@.new
	echo "// derived from automatically generated config.h. See Makefile.am." >>$@.new
	TODAY=`date +%Y-%m-%d` ; echo "/* Generated: $$TODAY */" >>$@.new
	echo "" >>$@.new
	echo "#pragma warn -8030 // temporary variables" >>$@.new
	echo "" >>$@.new
	echo "/* Define to use partial specialisation instead of meta programming. */" >>$@.new
	echo "#define MOCKPP_COUNTED_WEAKNESS" >>$@.new
	echo "" >>$@.new
	echo "/* Define to diable use of "long long" type. */" >>$@.new
	echo "#define UPS_NO_LONG_LONG" >>$@.new
	echo "" >>$@.new
	echo "/* Version number of package as long int */" >>$@.new
	echo "#define MOCKPP_VERSION_NUMBER \"$(MOCKPP_VERSION_NUMBER)\"" >>$@.new
	echo "" >>$@.new
	$(SHELL) $(srcdir)/config/gen-config-common.sh $@.new $(VERSION) $(PACKAGE)
	grep -v "Generated:" $@.new       >$@.new.1
	grep -v "Generated:" $(srcdir)/$@ >$@.new.2
	-cmp $@.new.1 $@.new.2 || cp $@.new $(srcdir)/$@

mockpp_config-msvc_71.h:
	perl  -i.bak -pe "s/(.*mockpp_production)_(\d+)([d]?)(\.lib|\.dll)/\1_$(LT_CURRENT)\3\4/g"  $(top_srcdir)/msvc8/mockpp/examples/*.vcproj $(top_srcdir)/msvc8/mockpp/examples/tutorial/*.vcproj $(top_srcdir)/msvc8/mockpp/*.vcproj $(top_srcdir)/msvc8/mockpp/tests/*.vcproj
	perl  -i.bak -pe "s/(.*mockpp_cxxtest)_(\d+)([d]?)(\.lib|\.dll)/\1_$(LT_CURRENT)\3\4/g"     $(top_srcdir)/msvc8/mockpp/examples/*.vcproj $(top_srcdir)/msvc8/mockpp/examples/tutorial/*.vcproj $(top_srcdir)/msvc8/mockpp/*.vcproj $(top_srcdir)/msvc8/mockpp/tests/*.vcproj
	perl  -i.bak -pe "s/(.*mockpp)_(\d+)([d]?)(\.lib|\.dll)/\1_$(LT_CURRENT)\3\4/g"             $(top_srcdir)/msvc8/mockpp/examples/*.vcproj $(top_srcdir)/msvc8/mockpp/examples/tutorial/*.vcproj $(top_srcdir)/msvc8/mockpp/*.vcproj $(top_srcdir)/msvc8/mockpp/tests/*.vcproj
	echo "// #define's for MS Visual C++ 7.1 and later (NOT 6.0/7.0)" >$@.new
	echo "// derived from automatically generated config.h. See Makefile.am." >>$@.new
	TODAY=`date +%Y-%m-%d` ; echo "/* Generated: $$TODAY */" >>$@.new
	echo "" >>$@.new
	echo "/* Define to use sprintf_s instead of sprintf. */" >>$@.new
	echo "#if _MSC_VER >= 1400" >>$@.new
	echo "# define HAVE_SPRINTF_S" >>$@.new
	echo "#endif" >>$@.new
	echo "" >>$@.new
	echo "/* Version number of package as long int */" >>$@.new
	echo "#define MOCKPP_VERSION_NUMBER \"$(MOCKPP_VERSION_NUMBER)\"" >>$@.new
	echo "" >>$@.new
	$(SHELL) $(srcdir)/config/gen-config-common.sh $@.new $(VERSION) $(PACKAGE)
	grep -v "Generated:" $@.new       >$@.new.1
	grep -v "Generated:" $(srcdir)/$@ >$@.new.2
	-cmp $@.new.1 $@.new.2 || cp $@.new $(srcdir)/$@

i18n_files = $(srcdir)/*.cpp $(srcdir)/*.h \
             $(srcdir)/compat/*.h $(srcdir)/compat/*.cpp \
             $(srcdir)/util/*.h $(srcdir)/util/*.cpp

messages:
	cd $(srcdir) && xgettext `ls $(i18n_files)` -C -kMOCKPP_I18N_NOOP -kmockpp_i18n -o  $(srcdir)/po/$(PACKAGE).pot


