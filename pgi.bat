@echo off
set PGI=C:\PROGRA~1\PGI
set PATH=C:\Program Files\Java\jre7\bin;%PATH%
set PATH=C:\Program Files\PGI\flexlm;%PATH%
set PATH=C:\cygwin\bin;%PATH%
set PATH=%PGI%\win64\2014\cuda\6.0\bin;%PATH%
set PATH=%PGI%\win64\2014\cuda\6.5\bin;%PATH%
set PATH=C:\Program Files (x86)\Windows Kits\8.1\bin\x64;%PATH%
set PATH=C:\Program Files\PGI\Microsoft Open Tools 12\bin\amd64;%PATH%
set PATH=%PGI%\win64\14.10\bin;%PATH%
set PATH=%PATH%;.
set FLEXLM_BATCH=1
title PGI Workstation 14.10 (64)
set TMP=C:\temp
set CYGROOT=C:\cygwin
set MANPATH=/C/PROGRA~1/PGI/win64/14.10/man:/usr/share/man
set PS1=PGI$ 
echo PGI Workstation 14.10 (64)
C:\cygwin\bin\bash -i
