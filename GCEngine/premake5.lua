project "GCEngine"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   staticruntime "off"

   targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
   objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

   pchheader "GCEPCH.h"
   pchsource "src/GCEPCH.cpp"
    
   files
   {
      "src/**.h",
      "src/**.cpp",
      "vendor/stb_image/**.cpp",
      "vendor/stb_image/**.h",
      "vendor/glm/glm/**.hpp",
      "vendor/glm/glm/**.inl",
      "vendor/ImGuizmo/ImGuizmo.h",
      "vendor/ImGuizmo/ImGuizmo.cpp"
   }
      
   defines
   {
      "IMGUI_API=__declspec(dllexport)",
      "_CRT_SECURE_NO_WARNINGS",
      "GLFW_INCLUDE_NONE"
   }

   includedirs
   {
      "vendor/spdlog/include",
      "src",
      "%{IncludeDir.GLFW}",
      "%{IncludeDir.Glad}",
      "%{IncludeDir.ImGui}",
      "%{IncludeDir.glm}",
      "%{IncludeDir.stb_image}",
      "%{IncludeDir.entt}",
      "%{IncludeDir.yaml_cpp}",
      "%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.VulkanSDK}"
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


   filter "system:windows"
      systemversion "latest"
      defines
      {
         "GLFW_INCLUDE_NONE"
      }

   filter "configurations:Debug"
      defines "GCE_DEBUG"
      runtime "Debug"
      symbols "on"
      
      links
      {
         "%{Library.ShaderC_Debug}",
			"%{Library.SPIRV_Cross_Debug}",
			"%{Library.SPIRV_Cross_GLSL_Debug}"
      }

   filter "configurations:Release"
      defines "GCE_RELEASE"
      runtime "Release"
      optimize "on"

      links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}

   filter "configurations:Dist"
      defines "GCE_DIST"
      runtime "Release"
      optimize "on"

      links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}