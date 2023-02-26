project "Pine-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Pine/vendor/spdlog/include",
		"%{wks.location}/Pine/src",
		"%{wks.location}/Pine/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}"
	}

	links
	{
		"Pine"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "PN_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "PN_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "PN_DIST"
		runtime "Release"
		optimize "on"
