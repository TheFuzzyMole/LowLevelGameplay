project "LowLevelGameplay"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "Off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. " / %{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	defines { "SFML_STATIC", "_CONSOLE" }

	includedirs
	{
		"src",
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
