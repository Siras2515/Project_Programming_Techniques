@echo off
g++ *.cpp -o out.exe -lwinmm
start "" "out.exe"