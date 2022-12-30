workspace "Pine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

includedir = {}
includedir["GLFW"] = "Pine/vendor/GLFW/include"

include "Pine/vendor/GLFW"

project "Pine"
	location "Pine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pnpch.h"
	pchsource "Pine/src/pnpch.cpp"

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{includedir.GLFW}"
	}

	links
	{
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"PN_PLATFORM_WINDOWS",
			"PN_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "PN_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "PN_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "PN_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Pine/vendor/spdlog/include",
		"Pine/src"
	}

	links
	{
		"Pine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"PN_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "PN_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "PN_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "PN_DIST"
		optimize "On"