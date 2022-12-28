workspace "Pine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Pine"
	location "Pine"
	kind "SharedLib"
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
		"%{prj.name}/vendor/spdlog/include"
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

	filter "configurations:Distribution"
		defines "PN_DISTRIBUTION"
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

	filter "configurations:Distribution"
		defines "PN_DISTRIBUTION"
		optimize "On"