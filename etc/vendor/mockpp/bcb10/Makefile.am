SUBDIRS =

CLEANFILES = *.~* *.~~* *~

EXTRA_DIST = \
  gen-mak.bat \
  run.bat \
  compile.bat \
	mockpp.bdsgroup \
	mockpp_production.bdsproj \
	mockpp_production.bpf \
	mockpp_cxxtest.bdsproj \
	mockpp_cxxtest.bpf \
	readability.bpf \
	readability.bdsproj \
	mock_greeter.bpf \
	mock_greeter.bdsproj \
	basicmock.bpf \
	basicmock.bdsproj \
	visitmock.bpf \
	visitmock.bdsproj \
	visitmock2.bpf \
	visitmock2.bdsproj \
	chainmock.bpf \
	chainmock.bdsproj \
	chainmock2.bpf \
	chainmock2.bdsproj \
  poormock.bpf \
  poormock.bdsproj \
  poormock-se.bpf \
  poormock-se.bdsproj \
	cppunit.bpf \
	cppunit.bdsproj \
	verifying.bpf \
	verifying.bdsproj \
	mockpp.bpf \
	mockpp.bdsproj \
	mock_test.bpf \
	mock_test.bdsproj \
	mock_test_win32.bpf \
	mock_test_win32.bdsproj

mock_test_count:
	grep cpp $(srcdir)/mock_test.bpf | sort >mock_test.files
	cat mock_test.files | wc -l ; echo "Dateien in mock_test";

