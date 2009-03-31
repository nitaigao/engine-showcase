SUBDIRS =

CLEANFILES = *.~* *.~~* *~

EXTRA_DIST = \
  gen-mak.bat \
  run.bat \
  compile.bat \
  mockpp.bpg \
  mockpp_production.bpr \
  mockpp_production.bpf \
  mockpp_cxxtest.bpr \
  mockpp_cxxtest.bpf \
  readability.bpf \
  readability.bpr \
  mock_greeter.bpf \
  mock_greeter.bpr \
  basicmock.bpf \
  basicmock.bpr \
  visitmock.bpf \
  visitmock.bpr \
  visitmock2.bpf \
  visitmock2.bpr \
  chainmock.bpf \
  chainmock.bpr \
  chainmock2.bpf \
  chainmock2.bpr \
  poormock.bpf \
  poormock.bpr \
  poormock-se.bpf \
  poormock-se.bpr \
  cppunit.bpf \
  cppunit.bpr \
  verifying.bpf \
  verifying.bpr \
  mockpp2xml.bpf \
  mockpp2xml.bpr \
  xml2mockpp.bpf \
  xml2mockpp.bpr \
  mockpp.bpf \
  mockpp.bpr \
  mock_test.bpf \
  mock_test.bpr \
  mock_test_win32.bpf \
  mock_test_win32.bpr

mock_test_count:
	grep cpp $(srcdir)/mock_test.bpf | sort >mock_test.files
	cat mock_test.files | wc -l ; echo "Dateien in mock_test";

