@echo off
g++ *.cpp -std=c++14 -lwinmm -o out.exe 
start "" "out.exe"