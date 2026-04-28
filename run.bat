@echo off
cmake --build build 
.\build\markov_algos.exe
uv run .\analyze.py
