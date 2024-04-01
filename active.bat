@echo off
g++ *.cpp -std=c++11 -pthread -lwinmm -o out.exe 
Start "" "out.exe"