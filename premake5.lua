workspace "GCEngine"
    architecture "x64"
    startproject "Sandbox"
    configurations { "Debug", "Release", "Dist" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Included directories relative to root folder
IncludeDir = {}
IncludeDir["GLFW"] = "GCEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "GCEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "GCEngine/vendor/imgui"

include "GCEngine/vendor/GLFW"
include "GCEngine/vendor/Glad"
include "GCEngine/vendor/imgui"

project "GCEngine"
    location "GCEngine"
    kind "SharedLib"
    language "C++"
    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "GCEPCH.h"
    pchsource "GCEngine/src/GCEPCH.cpp"
    
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs
    {
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "10.0"
    
        defines
        {
            "GCE_PLATFORM_WINDOWS",
            "GCE_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox/")
        }

    filter "configurations:Debug"
        defines "GCE_DEBUG"
        buildoptions "/MDd"
        symbols "On"
        
    filter "configurations:Release"
        defines "GCE_RELEASE"
        buildoptions "/MD"
        optimize "On"

    filter "configurations:Dist"
        defines "GCE_DIST"
        buildoptions "/MD"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "GCEngine/vendor/spdlog/include",
        "GCEngine/src"
    }

    links
    {
        "GCEngine"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "GCE_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "GCE_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "GCE_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "GCE_DIST"
        optimize "On"