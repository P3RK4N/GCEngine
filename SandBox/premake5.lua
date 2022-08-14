project "Sandbox"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   staticruntime "on"

   targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
   objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

   files
   {
      "%{prj.name}/src/**.h",
      "%{prj.name}/src/**.cpp"
   }

   includedirs
   {
      "%{wks.location}/GCEngine/vendor/spdlog/include",
      "%{wks.location}/GCEngine/src",
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

filter "configurations:Debug"
   defines "GCE_DEBUG"
   runtime "Debug"
   symbols "on"
   
filter "configurations:Release"
   defines "GCE_RELEASE"
   runtime "Release"
   optimize "on"

filter "configurations:Dist"
   defines "GCE_DIST"
   runtime "Release"
   optimize "on"

