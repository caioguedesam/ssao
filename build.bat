@echo off
pushd scripts
FOR /F "tokens=* USEBACKQ" %%F IN (`python3 build.py -debug`) DO (
    SET var=%%F
)

CALL %var%
popd
