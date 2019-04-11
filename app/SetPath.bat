@echo off

:AX107x or AX207x or AX207xb
rem 这里选择使用的MCU类型, AX107X_TAG or AX207X_TAG or AX207XB_TAG
rem 移到 MakeAll.bat里面设定, 
rem SET MCU_Item=AX207X_TAG

rem 检查MakeFile工具程式的目录是否存在
SET TOOLS_PATH=C:\BC_Tools\bin
if exist %TOOLS_PATH%\make.exe goto Find_MakeFile_Tools
@echo 没有找到 C:\BC_Tools\bin 的目录
@echo 请设定SetPath.bat内的 TOOLS_PATH 至正确的位置, 方能正常运行 make的功能
goto END
:Find_MakeFile_Tools

:IAR51_Check
rem 检查IAR C51 的目录是否存在
SET IAR_SourePath=%ProgramFiles%\IAR Systems\Embedded Workbench 4.05 Evaluation version
if exist "%IAR_SourePath%" goto Find_IAR51_Path
SET IAR_SourePath=%ProgramFiles(x86)%\IAR Systems\Embedded Workbench 4.05 Evaluation version
if exist "%IAR_SourePath%" goto Find_IAR51_Path
@echo 没有找到已安装的 IAR Systems\Embedded Workbench 4.05 Evaluation version
@echo 请设定 SetPath.bat 内的 IAR_SourePath 至 IAR的安装目录
goto END
:Find_IAR51_Path

rem 建立编译器环境
SET Path=%TOOLS_PATH%;%IAR_SourePath%\8051\bin;%IAR_SourePath%\common\bin;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\WindowsPowerShell\v1.0\
SET XLINK_COLUMNS=132
SET XLINK_CPU=x51
SET XLINK_DFLTDIR=%IAR_SourePath%\8051\lib\
SET C_INCLUDE=%IAR_SourePath%\8051\INC;%IAR_SourePath%\8051\INC\CLIB\
SET A8051_INC=%IAR_SourePath%\8051\INC
SET ROOT_PATH=%~dp0

rem 建立 Project的输出目录
SET BUILD_DIR=DebugBC

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
if not exist "%BUILD_DIR%\Obj" mkdir "%BUILD_DIR%\Obj"
if not exist "%BUILD_DIR%\List" mkdir "%BUILD_DIR%\List"
if not exist "%BUILD_DIR%\Exe" mkdir "%BUILD_DIR%\Exe"
if not exist "%BUILD_DIR%\Deps" mkdir "%BUILD_DIR%\Deps"
if not exist "%BUILD_DIR%\Exe\make.bat" xcopy Debug\Exe\*.* "%BUILD_DIR%\Exe" /s

:END
@echo on
