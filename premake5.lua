-------------------
-- Common utilities
-------------------
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

projectroot = path.getabsolute(".")

function AddTargetDirs()
    targetdir (projectroot .. "/Binaries/" .. outputdir .. "/%{prj.name}")
    objdir (projectroot .. "/Intermediate/" .. outputdir .. "/%{prj.name}")
end

function GetDefaultConfig()
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    systemversion "latest"

    AddTargetDirs()

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
        projectroot .. "/Evoke/ThirdParty/GLM/",
        projectroot .. "/Evoke/ThirdParty/EnTT/include"
    }
end

function IncludeThirdPartyDependencies()
    matches = os.matchfiles("Evoke/ThirdParty/_Premake/*.lua")
    for i, match in ipairs(matches)
    do
        include(match)
    end
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
IncludeThirdPartyDependencies()

group ""
include "Evoke"
include "DefaultProject"
