@echo off
REM This commands call bat-file, that compile TNKernel source files for PIC24 and dsPIC Microchip cores and archive it to library file
REM
REM (1) parameter - sources compilation optimization level (See MPLAB C30 User Guide, 3.5.6 for reference)
REM - 0 - Do not optimize
REM - 1 - Optimize
REM - 2 - Optimize even more
REM - 3 - Optimize yet more
REM - s - Optimize for size
REM
REM (2) parameter - used code model (See MPLAB C30 User Guide, table 3.2)
REM - large-data - use it for all devices with Data Memory large then 6 kBytes
REM - small-data - use it for all other devices
REM
REM (3) parameter - obj and lib type
REM - elf  - ELF mode
REM - coff - COFF mode
REM
REM (4) parameter - target type - PIC24 or dsPIC
REM - __PIC24F__ or __PIC24H__     - PIC24F/H
REM - __dsPIC30F__ or __dsPIC33F__ - dsPIC30/33
REM
REM

if exist tnkernel_rev.h del tnkernel_rev.h
SubWCRev ..\\..\\..\\..\\..\\..\\P-0160 tnkernel_rev.h.tpl tnkernel_rev.h

echo === Target is PIC24 ===
echo.

call _c30.bat s large-data coff __PIC24F__
call _c30.bat s large-data elf __PIC24F__

echo === Target is dsPIC ===
echo.

call _c30.bat s large-data coff __dsPIC33F__
call _c30.bat s large-data elf __dsPIC33F__


pause
