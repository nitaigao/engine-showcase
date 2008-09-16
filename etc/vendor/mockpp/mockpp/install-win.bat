@echo off
rem Install mockpp on windows platforms
rem Call from within mockpp directory where this file is located.
rem synopsis:  install-win  <destination-directory>

if not "%1" == "" goto :install
echo synopsis:  %0 "destination-directory"
goto :EOF

:install
echo on

md "%1"
md "%1\mockpp"
md "%1\ministl"
md "%1\cxxtest"

xcopy /Y "*.h"             "%1\mockpp\"
xcopy /Y "chaining\*.h"    "%1\mockpp\chaining\"
xcopy /Y "visiting\*.h"    "%1\mockpp\visiting\"
xcopy /Y "compat\*.h"      "%1\mockpp\compat\"
xcopy /Y "util\*.h"        "%1\mockpp\util\"
xcopy /Y "builder\*.h"     "%1\mockpp\builder\"
xcopy /Y "constraint\*.h"  "%1\mockpp\constraint\"
xcopy /Y "matcher\*.h"     "%1\mockpp\matcher\"
xcopy /Y "stub\*.h"        "%1\mockpp\stub\"
xcopy /Y "production\*.h"  "%1\mockpp\production\"
xcopy /Y "framework\*.h"   "%1\mockpp\framework\"

xcopy /Y "..\3party\cxxtest\cxxtest\*.h"   "%1\cxxtest\"

xcopy /Y "..\3party\ministl\*.h"           "%1\ministl\"
