@echo off
@rem ����ѡ��ʹ�õ�MCU����, AX107X_TAG or AX207X_TAG or AX207XB_TAG 
@SET MCU_Item=AX207X_TAG
@call SetPath.bat
@if exist %TOOLS_PATH% goto RunMake
@echo "���趨SetPath.bat�ڵ� TOOLS_PATH ����ȷ��λ��, ������������ make�Ĺ���"
goto END

:RunMake
@echo on
@perl %TOOLS_PATH%\ShowTime.pl "Make IAR" Start
make -j -f make51.mak all
@Perl %TOOLS_PATH%\ShowTime.pl "Make IAR" Stop
@rm -f Timer.Log
@echo Ŀǰ�ı���ѡ���� %MCU_Item% ��Ҫ���, ���޸� MakeAll.bat �ڵ� MCU_Item
:END
