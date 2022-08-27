VULKAN_SDK = os.getenv("VULKAN_SDK")

-- Dependencies
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/GCEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/GCEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/GCEngine/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/GCEngine/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/GCEngine/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/GCEngine/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/GCEngine/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/GCEngine/vendor/ImGuizmo"
IncludeDir["Box2D"] = "%{wks.location}/GCEngine/vendor/Box2D/include"
IncludeDir["mono"] = "%{wks.location}/GCEngine/vendor/mono/include"
IncludeDir["shaderc"] = "%{wks.location}/GCEngine/vendor/shaderc/include"
IncludeDir["SPIRV_Cross"] = "%{wks.location}/GCEngine/vendor/SPIRV_Cross"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

-- Library Directories
LibraryDir = {}
LibraryDir["mono"] = "%{wks.location}/GCEngine/vendor/mono/lib/%{cfg.buildcfg}"
LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibraryDir["VulkanSDK_Debug"] = "%{wks.location}/GCEngine/vendor/VulkanSDK/Lib"

-- Libraries
Library = {}

Library["mono"] = "%{LibraryDir.mono}/libmono-static-sgen.lib"

Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"


-- Windows
Library["WinSock"] = "Ws2_32.lib"
Library["WinMM"] = "Winmm.lib"
Library["WinVersion"] = "Version.lib"
Library["BCrypt"] = "Bcrypt.lib"