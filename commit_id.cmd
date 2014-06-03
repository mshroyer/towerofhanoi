@echo off
setlocal ENABLEDELAYEDEXPANSION

hg --debug id -i >commitid 2>NUL || echo default >commitid
set /p commitid= <commitid
del commitid

echo %commitid:~0,40%

endlocal

