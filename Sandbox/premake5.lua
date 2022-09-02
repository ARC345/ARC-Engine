project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/_bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/_int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"src/**.h",
		"src/**.cpp",
	}

	links
	{
		"ARC-Engine"
	}

	includedirs
	{
		"%{wks.location}/ARC-Engine/src",
		"%{wks.location}/ARC-Engine/out/spdlog/include",
		"%{wks.location}/ARC-Engine/out/inifile_cpp/include",
		"%{wks.location}/ARC-Engine/out",
		"%{IncludeDir.glm}",
		"%{IncludeDir.sparsemap}",
		"%{IncludeDir.orderedmap}",
		"%{IncludeDir.meta}",
		"%{IncludeDir.yaml_cpp}"
	}
	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"NOMINMAX",
		"GLFW_INCLUDE_NONE",
		"YAML_CPP_STATIC_DEFINE"
	}
	disablewarnings { "26812" }

	filter "system:windows"
		systemversion "latest"

		defines
		{
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