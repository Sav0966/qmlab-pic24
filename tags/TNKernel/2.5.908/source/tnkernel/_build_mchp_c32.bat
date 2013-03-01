@echo off
REM This commands call bat-file, that compile TNKernel source files for PIC32 and archive it to library file
REM
REM (1) parameter - sources compilation optimization level (See MPLAB C32 User Guide, 1.8.6 for reference)
REM - 0 - Do not optimize
REM - 1 - Optimize
REM - 2 - Optimize even more
REM - 3 - Optimize yet more
REM - s - Optimize for size
REM
REM
REM

REM if exist tnkernel_rev.h del tnkernel_rev.h
REM SubWCRev ..\ tnkernel_rev.h.tpl tnkernel_rev.h

echo === Target is PIC32 ===
echo.

call _c32.bat s

pause
