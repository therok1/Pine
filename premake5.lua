include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "Pine"
	architecture "x86_64"
	startproject "Pine-Editor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items 
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/premake"
	include "Pine/vendor/Box2D"
	include "Pine/vendor/GLFW"
	include "Pine/vendor/Glad"
	include "Pine/vendor/imgui"
	include "Pine/vendor/yaml-cpp"
group ""

group "Core"
	include "Pine"
	include "Pine-ScriptCore"
group ""

group "Tools"
	include "Pine-Editor"
group ""

group "Misc"
	include "Sandbox"
group ""
