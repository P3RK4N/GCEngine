include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "GCEngine"
    architecture "x64"
    startproject "GCEditor"
    configurations { "Debug", "Release", "Dist" }
    solution_items { ".editorconfig" }
    flags { "MultiProcessorCompile" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
    include "vendor/premake"
    include "GCEngine/vendor/GLFW"
    include "GCEngine/vendor/Glad"
    include "GCEngine/vendor/imgui"
    include "GCEngine/vendor/yaml-cpp"
    include "GCEngine/vendor/Box2D"
group ""

group "Core"
    include "GCEngine"
    include "GCEngine-ScriptCore"
group ""
    
group "Tools"
    include "GCEditor"
group ""

group "Misc"
    include "Sandbox"
group ""