workspace "SSPFramework"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["glm"] = "SSPFramework/vendor/glm"
IncludeDir["spdlog"] = "SSPFramework/vendor/spdlog"
IncludeDir["ImGui"] = "SSPFramework/vendor/imgui"
IncludeDir["ImGuiBackends"] = "SSPFramework/vendor/imgui/backends"
IncludeDir["Assimp"] = "SSPFramework/vendor/assimp/include"

include "SSPFramework/vendor/imgui"

project "SSPFramework"
	location "SSPFramework"
	kind "ConsoleApp"
	language "C++"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "%{prj.name}/src/pch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor",
		"%{prj.name}/src",
		"%{IncludeDir.glm}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuiBackends}",
		"%{IncludeDir.Assimp}"
	}

	libdirs
	{
		"%{prj.name}/vendor/assimp/lib/x64/"
	}

	links
	{
		"ImGui",
		"assimp-vc143-mt.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

	defines
	{
		"PLATFROM_WINDOWS",
		"ASSERTION_ENABLED"
	}

	postbuildcommands
    {
        ("{COPY} ../%{prj.name}/Assets ../bin/" .. outputdir .. "/%{prj.name}/Assets"),
		("{COPY} ../%{prj.name}/vendor/assimp/lib/x64/assimp-vc143-mt.dll ../bin/" .. outputdir .. "/%{prj.name}")
    }

	filter "configurations:Debug"
		defines "DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "DIST"
		optimize "On"