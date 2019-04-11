@echo off
@call SetPath.bat
@echo on

rm -f %BUILD_DIR%/Obj/*.* %BUILD_DIR%/Deps/*.* %BUILD_DIR%/List/*.* %BUILD_DIR%/Exe/app.* %BUILD_DIR%/Exe/APP.*

:END
