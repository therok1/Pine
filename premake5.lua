include "./vendor/premake/premake_customization/solution_items.lua"

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

includedir = {}
includedir["GLFW"] = "%{wks.location}/Pine/vendor/GLFW/include"
includedir["Glad"] = "%{wks.location}/Pine/vendor/Glad/include"
includedir["ImGui"] = "%{wks.location}/Pine/vendor/imgui"
includedir["glm"] = "%{wks.location}/Pine/vendor/glm"
includedir["stb_image"] = "%{wks.location}/Pine/vendor/stb_image"
includedir["entt"] = "%{wks.location}/Pine/vendor/entt/include"
includedir["yaml_cpp"] = "%{wks.location}/Pine/vendor/yaml-cpp/include"
includedir["ImGuizmo"] = "%{wks.location}/Pine/vendor/ImGuizmo"

group "Dependencies"
	include "vendor/premake"
	include "Pine/vendor/GLFW"
	include "Pine/vendor/Glad"
	include "Pine/vendor/imgui"
	include "Pine/vendor/yaml-cpp"
group ""

include "Pine"
include "Sandbox"
include "Pine-Editor"
