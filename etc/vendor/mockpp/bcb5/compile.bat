if exist c:\Programme\Borland\CBuilder5\Bin\make.exe   set BCB_BIN=c:\Programme\Borland\CBuilder5\Bin
if exist m:\ep\utilit\borland\cbuilder5\bin\make.exe   set BCB_BIN=m:\ep\utilit\borland\cbuilder5\bin
if exist d:\programme\borland\cbuilder5\bin\make.exe   set BCB_BIN=d:\programme\borland\cbuilder5\bin
if exist h:\Programme\Borland\CBuilder5\Bin\make.exe   set BCB_BIN=h:\Programme\Borland\CBuilder5\Bin

set path=%path%;%BCB_BIN%

%BCB_BIN%\make -f mockpp_cxxtest.mak
pause
%BCB_BIN%\make -f mockpp_production.mak
pause
%BCB_BIN%\make -f mockpp.mak
pause
%BCB_BIN%\make -f mock_test.mak
pause
%BCB_BIN%\make -f mock_test_win32.mak
pause
%BCB_BIN%\make -f readability.mak
pause
%BCB_BIN%\make -f mock_greeter.mak
pause
%BCB_BIN%\make -f basicmock.mak
pause
%BCB_BIN%\make -f visitmock.mak
pause
%BCB_BIN%\make -f visitmock2.mak
pause
%BCB_BIN%\make -f chainmock.mak
pause
%BCB_BIN%\make -f chainmock2.mak
pause
%BCB_BIN%\make -f poormock.mak
pause
%BCB_BIN%\make -f poormock-se.mak
pause
%BCB_BIN%\make -f verifying.mak
pause
%BCB_BIN%\make -f cppunit.mak
pause
