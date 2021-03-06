include "./_vendor/premake/premake_customization/solution_items.lua"

workspace "ARC-Engine"
	architecture "x86_64"
	startproject "Sandbox"

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
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/ARC-Engine/out/GLFW/include"
IncludeDir["GLAD"] = "%{wks.location}/ARC-Engine/out/GLAD/include"
IncludeDir["ImGui"] = "%{wks.location}/ARC-Engine/out/imgui"
IncludeDir["yaml_cpp"] = "%{wks.location}/ARC-Engine/out/yaml-cpp/include"
IncludeDir["glm"] = "%{wks.location}/ARC-Engine/out/glm"
IncludeDir["sparsemap"] = "%{wks.location}/ARC-Engine/out/sparse-map/include"
IncludeDir["orderedmap"] = "%{wks.location}/ARC-Engine/out/ordered-map/include"
IncludeDir["meta"] = "%{wks.location}/ARC-Engine/out/meta"
IncludeDir["stb_image"] = "%{wks.location}/ARC-Engine/out/stb_image"
IncludeDir["ImGuizmo"] = "%{wks.location}/ARC-Engine/out/ImGuizmo"

group "Dependencies"
	include "_vendor/premake"
	include "ARC-Engine/out/GLFW"
	include "ARC-Engine/out/GLAD"
	include "ARC-Engine/out/imgui"
	include "ARC-Engine/out/yaml-cpp"
group ""

include "ARC-Engine"
include "Sandbox"
include "ARC-Editor"