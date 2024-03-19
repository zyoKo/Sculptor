-- Utility Functions
function GetEnvironmentalVariable(variableName)
    local value = os.getenv(variableName)
    if value ~= nil then
        return value
    else
        return ""
    end
end

-- Workspace Settings
workspace "Sculptor"
    architecture "x86_64"
    startproject "Sculptor"

    configurations
    {
        "Debug",
        "Release"
    }

    flags 
    {
        "MultiProcessorCompile"
    }

    sourcedir = "./src"
    vendordir = "./vendor"
    outputdir = "%{cfg.buildcfg}_%{cfg.system}_%{cfg.architecture}"

    -- VulkanSDK path
    -- 1. Getting SDK path from Environmental Variables
    vulkanSDKdir = GetEnvironmentalVariable("VULKAN_SDK")
    -- 2. Local project
    -- vulkanSDKdir = vendordir .. "/Vulkan"

    group "Dependencies"
        include "Sculptor/vendor/GLFW"
        include "Sculptor/vendor/glad"
        include "Sculptor/vendor/imgui"
    group ""

    group "Core"
        include "Sculptor"
    group ""

-- this action can be used with premake.exe as a parameter
-- Usage: premake.exe clean    
newaction
{
    trigger = "clean",
    description = "Remove all binaries, intermediate binaries, and VS Files.",
    execute = function()
        print("Removing binaries & intermediate binaries")
        os.rmdir("./Sculptor/build")
        print("Removing project files")
        os.rmdir("./.vs")
        os.remove("**.sln")
        os.remove("**.vcxproj")
        os.remove("**.vcxproj.filters")
        os.remove("**.vcxproj.user")
        print("Done")
    end
}