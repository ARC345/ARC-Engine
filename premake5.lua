include "./Vendor/premake/premake_customization/solution_items.lua"

workspace "ARC-Engine"
	architecture "x86_64"
	startproject "ARC-Editor"

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
IncludeDir["GLFW"] = "%{wks.location}/ARC-Engine/out/glfw/include"
IncludeDir["GLAD"] = "%{wks.location}/ARC-Engine/out/GLAD/include"
IncludeDir["ImGui"] = "%{wks.location}/ARC-Engine/out/imgui"
IncludeDir["yaml_cpp"] = "%{wks.location}/ARC-Engine/out/yaml-cpp/include"
IncludeDir["glm"] = "%{wks.location}/ARC-Engine/out/glm"
IncludeDir["sparsemap"] = "%{wks.location}/ARC-Engine/out/sparse-map/include"
IncludeDir["orderedmap"] = "%{wks.location}/ARC-Engine/out/ordered-map/include"
IncludeDir["meta"] = "%{wks.location}/ARC-Engine/out/meta"
IncludeDir["stb_image"] = "%{wks.location}/ARC-Engine/out/stb_image"
IncludeDir["entt"] = "%{wks.location}/ARC-Engine/out/entt/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/ARC-Engine/out/ImGuizmo"
IncludeDir["Box2D"] = "%{wks.location}/ARC-Engine/out/Box2D/include"

group "Dependencies"
	include "_vendor/premake"
	include "ARC-Engine/out/GLFW"
	include "ARC-Engine/out/GLAD"
	include "ARC-Engine/out/imgui"
	include "ARC-Engine/out/Box2D"
	include "ARC-Engine/out/yaml-cpp"
group ""

include "ARC-Engine"
include "Sandbox"
include "ARC-Editor"