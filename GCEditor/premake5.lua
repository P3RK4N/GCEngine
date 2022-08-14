project "GCEditor"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   staticruntime "off"
   
   targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
   objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
   
   files
   {
      "src/**.h",
      "src/**.cpp"
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
      defines
      {
         -- "GCE_PLATFORM_WINDOWS"
      }

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