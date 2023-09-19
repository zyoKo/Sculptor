project "Sculptor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir("./build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("./build/obj/" .. outputdir .. "/%{prj.name}")

    pchheader "SculptorPch.h"
    pchsource "src/SculptorPch.cpp"

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
        vendordir .. "/spdlog/include",
        vendordir .. "/glm/include",
        vendordir .. "/Vulkan/include"
    }

    defines
    {
        "GLFW_INCLUDE_NONE",
        "GLFW_INCLUDE_VULKAN",
        "_CRT_SECURE_NO_WARNINGS",
        "GLM_FORCE_RADIANS",
        "GLM_FORCE_DEPTH_ZERO_TO_ONE"
    }

    libdirs 
    {
        vendordir .. "/Vulkan/lib"
    }

    links
    {
        "GLFW",
        "Glad",
        "vulkan-1.lib"
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