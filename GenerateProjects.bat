@echo off
call vendor\bin\premake\premake5.exe vs2022
if %ERRORLEVEL% NEQ 0 (
	PAUSE
)