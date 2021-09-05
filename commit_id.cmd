@echo off
setlocal ENABLEDELAYEDEXPANSION

git rev-parse HEAD >commitid 2>NUL || echo main >commitid
set /p commitid= <commitid
del commitid

echo %commitid%

endlocal

