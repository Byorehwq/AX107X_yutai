cd /d %~dp0

hexscript.exe --script
copy /y ..\List\app.map app.map

if exist C:\upload\upload.bat    (call c:\upload\upload.bat app.hex)
