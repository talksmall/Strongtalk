@REM Copyright 1996, Animorphic Systems. $Revision: 1.3 $
@PATH=%MSDEVDIR%\bin;%PATH%
@SET INCLUDE=%MSDEVDIR%\INCLUDE
@SET LIB=%MSDEVDIR%\LIB
@NMAKE /s /nologo /f "Delta.mak" CFG="Delta - Win32 Fast"
