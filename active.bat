@echo off
g++ *.cpp -std=c++11 -pthread -lwinmm -o out.exe 
@REM Start "" "out.exe"