@echo off
@rem 这里选择使用的MCU类型, AX107X_TAG or AX207X_TAG or AX207XB_TAG 
@SET MCU_Item=AX207X_TAG
@call SetPath.bat
@if exist %TOOLS_PATH% goto RunMake
@echo "请设定SetPath.bat内的 TOOLS_PATH 至正确的位置, 方能正常运行 make的功能"
goto END

:RunMake
@echo on
@perl %TOOLS_PATH%\ShowTime.pl "Make IAR" Start
make -j -f make51.mak all
@Perl %TOOLS_PATH%\ShowTime.pl "Make IAR" Stop
@rm -f Timer.Log
@echo 目前的编译选项是 %MCU_Item% 若要变更, 请修改 MakeAll.bat 内的 MCU_Item
:END
