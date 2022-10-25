@echo off
IF "%~1"=="-Release" (
    CALL ".\build\x64_release\typheus.exe"
) ELSE (
    CALL ".\build\x64_debug\typheus.exe"
)