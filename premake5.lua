-- premake5.lua
workspace "VulkanRenderingEngine"
   configurations { "Debug", "Release" }

project "VulkanRenderingEngine"
   kind "ConsoleApp"
   language "C++"
   architecture "x86_64"
   targetdir "Build/Bin/%{prj.name}/%{cfg.buildcfg}"
   objdir "Build/Obj/%{prj.name}/%{cfg.buildcfg}"
   location "VulkanRenderingEngine"

   files { "**.h", "**.c" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"