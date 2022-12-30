workspace "Pine"
	architecture "x64"

	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

includedir = {}
includedir["GLFW"] = "Pine/vendor/GLFW/include"
includedir["Glad"] = "Pine/vendor/Glad/include"
includedir["ImGui"] = "Pine/vendor/imgui"

include "Pine/vendor/GLFW"
include "Pine/vendor/Glad"
include "Pine/vendor/imgui"

project "Pine"
	location "Pine"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

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
		"%{includedir.GLFW}",
		"%{includedir.Glad}",
		"%{includedir.ImGui}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"PN_PLATFORM_WINDOWS",
			"PN_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "PN_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "PN_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "PN_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

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
		systemversion "latest"

		defines
		{
			"PN_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "PN_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "PN_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "PN_DIST"
		runtime "Release"
		optimize "On"