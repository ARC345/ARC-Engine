workspace "ARC-Engine"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
IncludeDir = {}
IncludeDir["GLFW"] = "ARC-Engine/out/GLFW/include"
IncludeDir["GLAD"] = "ARC-Engine/out/GLAD/include"
IncludeDir["ImGui"] = "ARC-Engine/out/imgui"

group "Dependencies"
	include "ARC-Engine/out/GLFW"
	include "ARC-Engine/out/GLAD"
	include "ARC-Engine/out/imgui"
group ""

project "ARC-Engine"
	location "ARC-Engine"
	kind "SharedLib"
	language "C++"
	staticruntime "off"
	targetdir ("_bin/" .. outputdir .. "/%{prj.name}")
	objdir ("_int/" .. outputdir .. "/%{prj.name}")
	
	pchheader "arc_pch.h"
	pchsource "%{prj.name}/src/PCH/arc_pch.cpp"
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/out/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"GLFW",
		"GLAD",
		"ImGui",
		"opengl32.lib"
	}
	
	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"ARC_PLATFORM_WINDOWS",
			"ARC_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../_bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		defines "ARC_DEBUG"
		runtime "Debug"
		symbols "On"
	filter "configurations:Release"
		defines "ARC_RELEASE"
		runtime "Release"
		optimize "On"
	filter "configurations:Dist"
		defines "ARC_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("_bin/" .. outputdir .. "/%{prj.name}")
	objdir ("_int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"ARC-Engine/src",
		"ARC-Engine/out/spdlog/include"
	}

	links
	{
		"ARC-Engine"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"ARC_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "ARC_DEBUG"
		runtime "Debug"
		symbols "On"
	filter "configurations:Release"
		defines "ARC_RELEASE"
		runtime "Release"
		optimize "On"
	filter "configurations:Dist"
		defines "ARC_DIST"
		runtime "Release"
		optimize "On"
