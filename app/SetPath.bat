@echo off

:AX107x or AX207x or AX207xb
rem ����ѡ��ʹ�õ�MCU����, AX107X_TAG or AX207X_TAG or AX207XB_TAG
rem �Ƶ� MakeAll.bat�����趨, 
rem SET MCU_Item=AX207X_TAG

rem ���MakeFile���߳�ʽ��Ŀ¼�Ƿ����
SET TOOLS_PATH=C:\BC_Tools\bin
if exist %TOOLS_PATH%\make.exe goto Find_MakeFile_Tools
@echo û���ҵ� C:\BC_Tools\bin ��Ŀ¼
@echo ���趨SetPath.bat�ڵ� TOOLS_PATH ����ȷ��λ��, ������������ make�Ĺ���
goto END
:Find_MakeFile_Tools

:IAR51_Check
rem ���IAR C51 ��Ŀ¼�Ƿ����
SET IAR_SourePath=%ProgramFiles%\IAR Systems\Embedded Workbench 4.05 Evaluation version
if exist "%IAR_SourePath%" goto Find_IAR51_Path
SET IAR_SourePath=%ProgramFiles(x86)%\IAR Systems\Embedded Workbench 4.05 Evaluation version
if exist "%IAR_SourePath%" goto Find_IAR51_Path
@echo û���ҵ��Ѱ�װ�� IAR Systems\Embedded Workbench 4.05 Evaluation version
@echo ���趨 SetPath.bat �ڵ� IAR_SourePath �� IAR�İ�װĿ¼
goto END
:Find_IAR51_Path

rem ��������������
SET Path=%TOOLS_PATH%;%IAR_SourePath%\8051\bin;%IAR_SourePath%\common\bin;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\WindowsPowerShell\v1.0\
SET XLINK_COLUMNS=132
SET XLINK_CPU=x51
SET XLINK_DFLTDIR=%IAR_SourePath%\8051\lib\
SET C_INCLUDE=%IAR_SourePath%\8051\INC;%IAR_SourePath%\8051\INC\CLIB\
SET A8051_INC=%IAR_SourePath%\8051\INC
SET ROOT_PATH=%~dp0

rem ���� Project�����Ŀ¼
SET BUILD_DIR=DebugBC

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
if not exist "%BUILD_DIR%\Obj" mkdir "%BUILD_DIR%\Obj"
if not exist "%BUILD_DIR%\List" mkdir "%BUILD_DIR%\List"
if not exist "%BUILD_DIR%\Exe" mkdir "%BUILD_DIR%\Exe"
if not exist "%BUILD_DIR%\Deps" mkdir "%BUILD_DIR%\Deps"
if not exist "%BUILD_DIR%\Exe\make.bat" xcopy Debug\Exe\*.* "%BUILD_DIR%\Exe" /s

:END
@echo on
