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
IncludeDir["glm"] = "GCEngine/vendor/glm"

include "GCEngine/vendor/GLFW"
include "GCEngine/vendor/Glad"
include "GCEngine/vendor/imgui"

project "GCEngine"
    location "GCEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "GCEPCH.h"
    pchsource "GCEngine/src/GCEPCH.cpp"
    
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl"
    }

    includedirs
    {
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }

    defines
    {
        "IMGUI_API=__declspec(dllexport)"
    }

    filter "system:windows"
        systemversion "latest"
        defines
        {
            "GCE_PLATFORM_WINDOWS",
            "GCE_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines "GCE_DEBUG"
        buildoptions "/MDd"
        symbols "on"
        
    filter "configurations:Release"
        defines "GCE_RELEASE"
        buildoptions "/MD"
        optimize "on"

    filter "configurations:Dist"
        defines "GCE_DIST"
        buildoptions "/MD"
        optimize "on"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

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
        "GCEngine/src",
        "%{IncludeDir.glm}"
    }

    links
    {
        "GCEngine"
    }

    defines
    {
        "IMGUI_API=__declspec(dllimport)"
    }

    filter "system:windows"
        systemversion "latest"
        defines
        {
            "GCE_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "GCE_DEBUG"
        buildoptions "/MDd"
        symbols "on"
        
    filter "configurations:Release"
        defines "GCE_RELEASE"
        buildoptions "/MD"
        optimize "on"

    filter "configurations:Dist"
        defines "GCE_DIST"
        buildoptions "/MD"
        optimize "on"