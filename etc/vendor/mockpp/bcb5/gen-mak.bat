if exist c:\Programme\Borland\CBuilder5\Bin\make.exe   set BCB_BIN=c:\Programme\Borland\CBuilder5\Bin
if exist m:\ep\utilit\borland\cbuilder5\bin\make.exe   set BCB_BIN=m:\ep\utilit\borland\cbuilder5\bin
if exist d:\programme\borland\cbuilder5\bin\make.exe   set BCB_BIN=d:\programme\borland\cbuilder5\bin
if exist h:\programme\borland\cbuilder5\bin\make.exe   set BCB_BIN=h:\programme\borland\cbuilder5\bin

set path=%path%;%BCB_BIN%
set COMPILE=c:\tmp\compile

if not exist c:\tmp\.                         md c:\tmp
if not exist c:\tmp\compile\.                 md c:\tmp\compile
if not exist c:\tmp\compile\mockpp_44\.       md c:\tmp\compile\mockpp_44

%BCB_BIN%\bpr2mak  mockpp.bpr
%BCB_BIN%\bpr2mak  mockpp_cxxtest.bpr
%BCB_BIN%\bpr2mak  mockpp_production.bpr
%BCB_BIN%\bpr2mak  mock_test.bpr
%BCB_BIN%\bpr2mak  mock_test_win32.bpr
%BCB_BIN%\bpr2mak  readability.bpr
%BCB_BIN%\bpr2mak  jmock_test.bpr
%BCB_BIN%\bpr2mak  mock_greeter.bpr
%BCB_BIN%\bpr2mak  basicmock.bpr
%BCB_BIN%\bpr2mak  visitmock.bpr
%BCB_BIN%\bpr2mak  visitmock2.bpr
%BCB_BIN%\bpr2mak  chainmock.bpr
%BCB_BIN%\bpr2mak  chainmock2.bpr
%BCB_BIN%\bpr2mak  poormock.bpr
%BCB_BIN%\bpr2mak  poormock-se.bpr
%BCB_BIN%\bpr2mak  verifying.bpr
%BCB_BIN%\bpr2mak  cppunit.bpr
%BCB_BIN%\bpr2mak  xml2mockpp.bpr
%BCB_BIN%\bpr2mak  mockpp2xml.bpr

