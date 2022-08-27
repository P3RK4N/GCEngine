powershell -Command "GCI . -Include *.cpp, *.h, *.cs, *.py, *.c, *.hpp, *.glsl, *.lua -Recurse | foreach{(GC $_).Count} | measure-object -sum"
PAUSE