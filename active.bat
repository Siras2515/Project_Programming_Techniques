@echo off
g++ *.cpp -std=c++14 -Wextra -lwinmm -pthread -o out.exe 
Start "" "out.exe"