workspace "FightLabDev"
	architecture "x86_64"
	startproject "Client"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	
	include "FightLab/vendors/GLFW"

project "FightLab"
	location "FightLab"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"Engine/src",
		"Engine/vendors/GLFW/include",
		"Engine/vendors/vulkan/Include",
		"Engine/vendors/GLM/glm/",
		"Engine/vendors/STB"
	}

	links
	{
		"GLFW",
		"Engine/vendors/vulkan/Lib/vulkan-1.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"PC_BUILD_STATIC"
		}

	filter "configurations:Debug"
		defines "PC_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "PC_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "PC_DIST"
		runtime "Release"
		optimize "on"

project "Client"
	location "Client"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Engine/src",
		"Engine/vendors/GLFW/include",
		"Engine/vendors/vulkan/Include",
		"Engine/vendors/GLM/glm",
		"Engine/vendors/STB"

	}

	links
	{
		"Engine"
	}

	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		defines "PC_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "PC_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "PC_DIST"
		runtime "Release"
		optimize "on"