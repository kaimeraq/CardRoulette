-- Premake script for CardRoulette console app module.
workspace "CardRoulette"
	architecture "x64"

	configurations
	{
		"Debug",
		"Development",
		"Shipping"
	}

outputos = "%{iif(cfg.system == 'windows', 'Win', cfg.system)}"
outputarch = "%{iif(cfg.architecture == 'x86_64', '64', cfg.architecture)}"
outputdir = "%{cfg.buildcfg}-" .. outputos .. outputarch

local function commonSettings()
    filter "system:windows"
        cppdialect "C++23"
        staticruntime "On"
        systemversion "latest"
        characterset "Unicode"
        warnings "Extra"
        
		defines 
		{ 
			"PLATFORM_WINDOWS", 
			"NOMINMAX", 
			"WIN32_LEAN_AND_MEAN" 
		}
        
		buildoptions
		{ 
			"/Zc:__cplusplus", 
			"/permissive-", 
			"/sdl" 
		}

    filter "configurations:Debug"
        defines
		{ 
			"CE_DEBUG", 
			"_DEBUG" 
		}

        symbols "On"
        optimize "Off"
        runtime "Debug"

    filter "configurations:Development"
        defines
		{ 
			"CE_DEVEL", 
			"NDEBUG" 
		}

        symbols "On"
        editandcontinue "Off"
        optimize "Debug"
        linktimeoptimization "Fast"
        runtime "Release"

    filter "configurations:Shipping"
        defines 
		{ 
			"CE_SHIPPING", 
			"NDEBUG" 
		}

        symbols "On"
        optimize "Full"
        linktimeoptimization "On"
        runtime "Release"

    filter {}
end

project "CardRoulette"
	kind "ConsoleApp"
	language "C++"

	links { "Engine" }

	targetdir ("bin/" .. outputdir)
	objdir	  ("int/" .. outputdir)

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"src",
		"lib/Engine/src"
	}

	commonSettings()

project "Engine"
	location "lib/Engine"
	kind "StaticLib"
	language "C++"

	targetdir ("%{prj.location}/bin/" .. outputdir)
	objdir	  ("%{prj.location}/int/" .. outputdir)

	pchheader "pch.h"
	pchsource "%{prj.location}/src/pch.cpp"
	forceincludes { "pch.h" }

	files
	{
		"%{prj.location}/src/**.h",
		"%{prj.location}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.location}/src"
	}

	commonSettings()