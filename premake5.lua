include "./vendor/premake/premake_customisation/solution_items.lua"
include "Dependencies.lua"

workspace "LowLevelGameplay"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
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

group "Dependencies"
	include "vendor/premake"
	include "LowLevelGameplay/vendor/Box2D"
group""

include "LowLevelGameplay"