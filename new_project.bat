@echo off

setlocal enableextensions enabledelayedexpansion

REM Prompt the user to input the new file name
set /p new_project_name=Enter new Project:

REM Set source and destination folder paths
set source_folder=Examples/Windows/TP_3D
set destination_folder=Projects/Windows/%new_project_name%

REM Copy folder and all its contents
xcopy "%source_folder%" "%destination_folder%" /E /I

REM Prompt the user to input the new file name
rem set /p new_file_name=Enter new file name (without extension):

REM Rename specific files
ren "%destination_folder%\TP_3D.vcxproj.filters" "%new_project_name%.vcxproj.filters"
ren "%destination_folder%\TP_3D.vcxproj.user" "%new_project_name%.vcxproj.user"





set "inputFile=%destination_folder%\TP_3D.sln"
set "outputFile=%destination_folder%\%new_project_name%.sln"

for /f "delims=" %%a in ('type "%inputFile%"') do (
  set "line=%%a"
  set "line=!line:TP_3D=%new_project_name%!"
  echo !line!>>"%outputFile%"
)
del "%inputFile%"




set "inputFile=%destination_folder%\TP_3D.vcxproj"
set "outputFile=%destination_folder%\%new_project_name%.vcxproj"

for /f "delims=" %%a in ('type "%inputFile%"') do (
  set "line=%%a"
  set "line=!line:TP_3D=%new_project_name%!"
  echo !line!>>"%outputFile%"
)
del "%inputFile%"


rd /s /q  "%destination_folder%/x64"
xcopy "%source_folder%\x64/Debug\res" "%destination_folder%\x64\Debug\res" /E /I
copy "%source_folder%\x64/Debug\glew32.dll" "%destination_folder%\x64\Debug\glew32.dll"




echo Folder copied, files renamed, and text replaced successfully.