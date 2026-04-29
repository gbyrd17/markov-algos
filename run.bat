@echo off
vcpkg x-update-baseline
cmake --build build 
.\build\markov_algos.exe
uv run .\analyze.py
