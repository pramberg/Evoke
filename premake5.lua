-------------------
-- Common utilities
-------------------
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

projectroot = path.getabsolute(".")

-- Temporary until I get a proper implementation of SDL2 working
libdirs { "SDL2", projectroot .. "/Evoke/ThirdParty/SDL/VisualC/x64/Release/" }

function GetDefaultConfig()
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    systemversion "latest"

    targetdir (projectroot .. "/Binaries/" .. outputdir .. "/%{prj.name}")
    objdir (projectroot .. "/Intermediate/" .. outputdir .. "/%{prj.name}")

    filter "configurations:Debug"
        defines "EV_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "EV_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Retail"
        defines "EV_RETAIL"
        runtime "Release"
        optimize "on"

    filter "system:windows"
        defines "EV_PLATFORM_WINDOWS"

    filter "system:linux"
        defines "EV_PLATFORM_LINUX"

    filter "system:macosx"
        defines "EV_PLATFORM_MAC"

    -- Reset filters
    filter {}
end

function GetCommonIncludes()
    return { 
        projectroot .. "/Evoke/ThirdParty/spdlog/include",
        projectroot .. "/Evoke/ThirdParty/SDL/include"
    }
end

--------
-- Setup
--------
workspace "Evoke"
    architecture "x86_64"

    configurations
    {
        "Debug",
        "Release",
        "Retail"
    }
    
    flags
    {
        "MultiProcessorCompile"
    }


group "Dependencies"

group ""
include "Evoke"
include "DefaultProject"
