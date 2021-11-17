if "%PREMAKE_HOME%" == "" (ECHO ERROR: PREMAKE_HOME environment variable is NOT defined please define itin the environment paths)
call "%PREMAKE_HOME%\premake5.exe" vs2019
pause