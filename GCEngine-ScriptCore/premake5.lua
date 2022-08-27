project "GCEngine-ScriptCore"
   kind "SharedLib"
   language "C#"
   dotnetframework "4.7.2"

   -- targetdir("%{wks.location}/GCEditor/Resources/Scripts")
   -- objdir("%{wks.location}/GCEditor/Resources/Scripts/Intermediates")
   
   targetdir("../GCEditor/resources/Scripts")
   objdir("../GCEditor/resources/Scripts/Intermediates")
    
   files
   {
      "Source/**.cs",
      "Properties/**.cs"
   }

   filter "configurations:Debug"
      symbols "Default"
      optimize "off"
      
   filter "configurations:Release"
      symbols "Default"
      optimize "on"

   filter "configurations:Dist"
      symbols "off"
      optimize "Full"
