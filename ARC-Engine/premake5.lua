workspace "ARC-Engine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "ARC-Engine"
	location "ARC-Engine"
	kind "SharedLib"
	language "C++"

	targetdir ("_bin/" .. outputdir .. "/%{prj.name}")
	objdir ("_int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor",
		"%{prj.name}/vendor/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"ARC_PLATFORM_WINDOWS",
			"ARC_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../_bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "ARC_DEBUG"
		symbols "On"
	filter "configurations:Release"
		defines "ARC_RELEASE"
		optimize "On"
	filter "configurations:Dist"
		defines "ARC_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

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
		"ARC-Engine/vendor",
		"ARC-Engine/vendor/spdlog/include"
	}

	links
	{
		"ARC-Engine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"ARC_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "ARC_DEBUG"
		symbols "On"
	filter "configurations:Release"
		defines "ARC_RELEASE"
		optimize "On"
	filter "configurations:Dist"
		defines "ARC_DIST"
		optimize "On" 
