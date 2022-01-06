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
IncludeDir["glm"] = "ARC-Engine/out/glm"

group "Dependencies"
	include "ARC-Engine/out/GLFW"
	include "ARC-Engine/out/GLAD"
	include "ARC-Engine/out/imgui"
group ""

project "ARC-Engine"
	location "ARC-Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("_bin/" .. outputdir .. "/%{prj.name}")
	objdir ("_int/" .. outputdir .. "/%{prj.name}")
	
	pchheader "arc_pch.h"
	pchsource "%{prj.name}/src/PCH/arc_pch.cpp"
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/out/glm/glm/**.hpp",
		"%{prj.name}/out/glm/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/out/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW",
		"GLAD",
		"ImGui",
		"opengl32.lib"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	disablewarnings { "26812" }

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"ARC_PLATFORM_WINDOWS",
			"ARC_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		defines "ARC_DEBUG"
		runtime "Debug"
		symbols "on"
	filter "configurations:Release"
		defines "ARC_RELEASE"
		runtime "Release"
		optimize "on"
	filter "configurations:Dist"
		defines "ARC_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"ARC-Engine/out/spdlog/include",
		"ARC-Engine/out"
	}

	links
	{
		"ARC-Engine"
	}

	disablewarnings { "26812" }

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"ARC_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "ARC_DEBUG"
		runtime "Debug"
		symbols "on"
	filter "configurations:Release"
		defines "ARC_RELEASE"
		runtime "Release"
		optimize "on"
	filter "configurations:Dist"
		defines "ARC_DIST"
		runtime "Release"
		optimize "on"
