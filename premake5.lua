include "Dependencies.lua"

workspace "LowLevelGameplay"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "LowLevelGameplay/vendor/Box2D"
group""

project "LowLevelGameplay"
	location "LowLevelGameplay"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. " / %{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	defines { "SFML_STATIC", "_CONSOLE" }

	includedirs
	{
		"%{prj.name}/vendor/SFML-2.6.0/include",
		"%{IncludeDir.Box2D}"
	}

	libdirs
	{
		"%{prj.name}/vendor/SFML-2.6.0/lib"
	}

	links
	{
		"opengl32",
		"freetype",
		"winmm",
		"gdi32",
		"flac",
		"vorbisenc",
		"vorbisfile",
		"vorbis",
		"ogg",
		"ws2_32",
		"Box2D"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "Off"
		systemversion "latest"

	filter "configurations:Debug"
		defines { "_DEBUG" }
		symbols "On"
		
		links
		{
			"sfml-graphics-s-d",
			"sfml-window-s-d",
			"sfml-system-s-d",
			"sfml-audio-s-d",
			"sfml-network-s-d"
		}

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		
		links
		{
			"sfml-graphics-s",
			"sfml-window-s",
			"sfml-system-s",
			"sfml-audio-s",
			"sfml-network-s"
		}