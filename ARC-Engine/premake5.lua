project "ARC-Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/_bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/_int/" .. outputdir .. "/%{prj.name}")
	
	pchheader "arc_pch.h"
	pchsource "src/PCH/arc_pch.cpp"
	files
	{
		"src/**.h",
		"src/**.cpp",
		"out/stb_image/**.cpp",
		"out/stb_image/**.h",
		"out/glm/glm/**.hpp",
		"out/glm/glm/**.inl",
		"out/ImGuizmo/ImGuizmo.h",
		"out/ImGuizmo/ImGuizmo.cpp",
		"out/sparse-map/include/tsl/**.h",
		"out/sparse-map/include/tsl/**.h",
		"out/ordered-map/include/tsl/**.h",
		"out/ordered-map/include/tsl/**.h",
		"out/meta/**.hpp"
	}

	links
	{
		"GLFW",
		"GLAD",
		"ImGui",
		"yaml-cpp",
		"opengl32.lib"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"NOMINMAX",
		"GLFW_INCLUDE_NONE",
		"YAML_CPP_STATIC_DEFINE"
	}

	includedirs
	{
		"src",
		"out/spdlog/include",
		"out/inifile_cpp/include",
		"out/yaml-cpp/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.sparsemap}",
		"%{IncludeDir.orderedmap}",
		"%{IncludeDir.meta}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}"
	}

	ComponentDirs = "%{prj.location}src\\ARC\\Components"
	ScriptsDir = "%{wks.location}Scripts\\"

	prebuildcommands{ "python %{ScriptsDir}GenComponentList.py %{ComponentDirs}" }

	filter "files:out/ImGuizmo/**.cpp"
		flags "NoPCH"

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