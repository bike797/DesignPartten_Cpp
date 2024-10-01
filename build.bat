echo off
set CPPVERSION=23
cls

@echo [101mg++ %* -std=c++%CPPVERSION% -o gout.exe [0m
@g++ %* -std=c++%CPPVERSION% -o gout.exe

@IF %ERRORLEVEL% EQU 0 (
	echo [37;4m[ g++ compiling success !! run gout.exe ][0m
  	gout.exe
) ELSE (
	echo [37;4m[ Error !!, g++ fail to compile. ][0m
)

rem @echo.
rem 
rem @echo [101mcl %* /permissive- /std:c++latest /utf-8 /nologo /EHsc /link /out:vout.exe [0m
rem @cl  %* /permissive- /std:c++latest /utf-8 /nologo /EHsc /link /out:vout.exe
rem 
rem @IF %ERRORLEVEL%  EQU 0 (
rem 	echo [37;4m[ cl compiling success !! run vout.exe ][0m
rem 	vout.exe
rem )ELSE (
rem 	echo [37;4m[ Error !!, cl fail to compile. ][0m
rem )