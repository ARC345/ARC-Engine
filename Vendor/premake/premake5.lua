project "Premake"
	kind "Utility"

	targetdir ("%{wks.location}/_bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/_int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{wks.location}/**premake5.lua"
	}

	postbuildmessage "Regenerating project files with Premake5!"
	postbuildcommands
	{
		"%{prj.location}_bin/premake5 %{_ACTION} --file=\"%{wks.location}premake5.lua\""
	}
