@echo off
pushd scripts
IF "%~1"=="-Release" (
    ECHO "RELEASE BUILD"
    FOR /F "tokens=* USEBACKQ" %%F IN (`python3 build.py -Release`) DO (
        SET var=%%F
    )
) ELSE (
    ECHO "DEBUG BUILD"
    FOR /F "tokens=* USEBACKQ" %%F IN (`python3 build.py -Debug`) DO (
        SET var=%%F
    )   
)

CALL %var%
popd
