@echo This will clean up all intermediate/user-specific files for Hikari. 
@echo ** You must close Visual C++ before running this file! **
@pause

@echo ========== Cleaning up... ==========
@del Hikari.ncb
@del Hikari.suo /AH
@del Hikari\*.user
@rmdir Hikari\objects\Release\ /S /Q
@rmdir Hikari\objects\Debug\ /S /Q
@del Hikari\lib\*.lib
@del Hikari\lib\*.dll
@del Hikari\lib\*.ilk
@del Hikari\lib\*.exp
@del Hikari\lib\*.pdb
@del Hikari\lib\*.idb
@del HikariDemo\*.user
@rmdir HikariDemo\objects\Release\ /S /Q
@rmdir HikariDemo\objects\Debug\ /S /Q
@rmdir HikariDemo\bin\release\ /S /Q
@rmdir HikariDemo\bin\debug\ /S /Q
@echo ============== Done! ===============
