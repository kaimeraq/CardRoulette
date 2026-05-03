@echo off
REM Delete specific folders
rd /s /q .vs
rd /s /q bin
rd /s /q int

rd /s /q lib\Engine\.vs
rd /s /q lib\Engine\bin
rd /s /q lib\Engine\int

REM Delete sln and vcxproj* files in base folder
del /q *.sln
del /q *.vcxproj*

del /q lib\Engine\*.sln
del /q lib\Engine\*.vcxproj*

REM Remove empty folders recursively inside src
robocopy src\ src\ /S /MOVE

robocopy lib\Engine\src\ lib\Engine\src\ /S /MOVE