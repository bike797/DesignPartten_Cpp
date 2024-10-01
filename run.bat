echo off
cls

if exist a.exe (
    del a.exe
)

rem g++ %1 -o -std=c++20
g++ %* -std=c++20 -o a.exe

if exist a.exe (
    echo [101m  --run prgram!!  %*  [0m
    a.exe
)