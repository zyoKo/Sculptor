project "ImGui"
	kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

	targetdir ("./build/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("./build/obj/" .. outputdir .. "/%{prj.name}")

	includedirs
	{
		vulkanSDKdir .. "/Include",

		"../GLFW/include",
	}

	files
	{
		"include/ImGui/imconfig.h",
		"include/ImGui/imgui.h",
		"include/ImGui/imgui_impl_glfw.h",
		"include/ImGui/imgui_impl_vulkan.h",
		"include/ImGui/imgui_internal.h",
		"include/ImGui/imstb_rectpack.h",
		"include/ImGui/imstb_textedit.h",
		"include/ImGui/imstb_truetype.h",

		-- In root
		"imgui.cpp",
		"imgui_demo.cpp",
		"imgui_draw.cpp",
		"imgui_impl_glfw.cpp",
		"imgui_impl_vulkan.cpp",
		"imgui_tables.cpp",
		"imgui_widgets.cpp"
	}

	filter "system:windows"
		systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"