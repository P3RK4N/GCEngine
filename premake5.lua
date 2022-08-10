workspace "GCEngine"
    architecture "x64"
    startproject "GCEditor"
    configurations { "Debug", "Release", "Dist" }
    flags { "MultiProcessorCompile" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Included directories relative to root folder
IncludeDir = {}
IncludeDir["GLFW"] = "GCEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "GCEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "GCEngine/vendor/imgui"
IncludeDir["glm"] = "GCEngine/vendor/glm"
IncludeDir["stb_image"] = "GCEngine/vendor/stb_image"
IncludeDir["entt"] = "GCEngine/vendor/entt/include"
IncludeDir["yaml_cpp"] = "GCEngine/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "GCEngine/vendor/ImGuizmo"

group "Dependencies"
    include "GCEngine/vendor/GLFW"
    include "GCEngine/vendor/Glad"
    include "GCEngine/vendor/imgui"
    include "GCEngine/vendor/yaml-cpp"
group ""

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
        "%{prj.name}/vendor/stb_image/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
        "%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
        "%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp"
    }

    includedirs
    {
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.yaml_cpp}",
        "%{IncludeDir.ImGuizmo}"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "yaml-cpp",
        "opengl32.lib"
    }

    filter "files:vendor/ImGuizmo/**.cpp"
        flags { "NoPCH" }

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
        symbols "on"
        
    filter "configurations:Release"
        defines "GCE_RELEASE"
        optimize "on"

    filter "configurations:Dist"
        defines "GCE_DIST"
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
        "%{IncludeDir.glm}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.entt}"
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
        symbols "on"
        
    filter "configurations:Release"
        defines "GCE_RELEASE"
        optimize "on"

    filter "configurations:Dist"
        defines "GCE_DIST"
        optimize "on"


project "GCEditor"
    location "GCEditor"
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
        "%{IncludeDir.glm}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.ImGuizmo}"
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
        symbols "on"
        
    filter "configurations:Release"
        defines "GCE_RELEASE"
        optimize "on"

    filter "configurations:Dist"
        defines "GCE_DIST"
        optimize "on"