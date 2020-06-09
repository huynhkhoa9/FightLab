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
		"FightLab/src",
		"FightLab/vendors/GLFW/include",
		"FightLab/vendors/vulkan/Include",
		"FightLab/vendors/GLM/glm/",
		"FightLab/vendors/STB",
		"FightLab/vendors/Assimp/include",
		"FightLab/vendors/VulkanMemoryAllocation/src"
	}

	links
	{
		"GLFW",
		"FightLab/vendors/vulkan/Lib/vulkan-1.lib"
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
		buildoptions "/MTd"
		links
		{
			"FightLab/vendors/Assimp/lib/Debug/assimp-vc142-mtd.lib"
		}

		postbuildcommands
		{
			("{COPY} vendors/Assimp/bin/Debug/assimp-vc142-mtd.dll ../bin/" .. outputdir .. "/Client")
		}
		
	filter "configurations:Release"
		defines "PC_RELEASE"
		runtime "Release"
		optimize "on"
		buildoptions "/MT"
		links
		{
			"FightLab/vendors/Assimp/lib/Release/assimp-vc142-mt.lib"
		}
		postbuildcommands
		{
			("{COPY} vendors/Assimp/bin/Release/assimp-vc142-mt.dll ../bin/" .. outputdir .. "/Client")
		}
	filter "configurations:Dist"
		defines "PC_DIST"
		runtime "Release"
		optimize "on"
		buildoptions "/MT"
		links
		{
			"FightLab/vendors/Assimp/lib/MinSizeRel/assimp-vc142-mt.lib"
		}
		postbuildcommands
		{
			("{COPY} vendors/Assimp/bin/MinSizeRel/assimp-vc142-mtd.dll ../bin/" .. outputdir .. "/Client")
		}
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
		"FightLab/src",
		"FightLab/vendors/GLFW/include",
		"FightLab/vendors/vulkan/Include",
		"FightLab/vendors/GLM/glm",
		"FightLab/vendors/STB",
		"FightLab/vendors/Assimp/include"

	}

	links
	{
		"FightLab"
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