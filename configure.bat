@echo off
REM ### CONFIGURATION BEGIN ###
set BUILD_TYPE=Debug
set BUILD=D:\dev\boost\geometry\build
set SRC=D:\dev\boost\geometry
set BOOST=D:\dev\boost\_svn\trunk
set G="Visual Studio 11"
REM ### CONFIGURATION END   ###
IF NOT EXIST %BUILD% mkdir %BUILD%
cd %BUILD% & ^
cmake -G %G% ^
    -DBOOST_ROOT=%BOOST% ^
    %SRC% & ^
fix_vs11_sln.bat .\boost_geometry_workshop.sln