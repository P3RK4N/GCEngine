workspace "GCEngine"
    architecture "x64"
    startproject "Sandbox"
    configurations { "Debug", "Release", "Dist" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

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
        "%{prj.name}/src"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "10.0"
    
        defines
        {
            "GCE_PLATFORM_WINDOWS",
            "GCE_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox/")
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
        systemversion "10.0"

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