powershell -Command "GCI . -Include *.cpp, *.h, *.c, *.hpp, *.glsl, *.lua -Recurse | foreach{(GC $_).Count} | measure-object -sum"
PAUSE