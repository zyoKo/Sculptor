project "VulkanRenderer"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir("./build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("./build/obj/" .. outputdir .. "/%{prj.name}")

    pchheader "VulkanRendererPch.h"
    pchsource "src/VulkanRendererPch.cpp"

    files 
    {
        sourcedir .. "/**.cpp",
        sourcedir .. "/**.h"
    }

    includedirs 
    {
        sourcedir,
        vendordir .. "/GLFW/include",
        vendordir .. "/glad/include",
        vendordir .. "/spdlog/include"
    }

    defines
    {
        "GLFW_INCLUDE_NONE",
        "_CRT_SECURE_NO_WARNINGS"
    }

    links
    {
        "GLFW",
        "Glad"
    }

    filter { "configurations:Debug" }
        buildoptions "/MTd"
        defines "DEBUG"
        runtime "Debug"
        symbols "on"

    filter { "configurations:Release" }
        buildoptions "/MT"
        defines "RELEASE"
        runtime "Release"
        optimize "on"