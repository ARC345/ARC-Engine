@echo off
if "%PREMAKE5_HOME%" == "" (ECHO ERROR: PREMAKE_HOME environment variable is NOT defined please define it in the environment paths)
pushd ..\
call "%PREMAKE5_HOME%\premake5.exe" vs2019
popd

IF %ERRORLEVEL% NEQ 0 (
  PAUSE
)