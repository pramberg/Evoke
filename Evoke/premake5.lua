project "Evoke"
    kind "StaticLib"
    
    GetDefaultConfig()

    pchheader "PCH.h"
    pchsource "Source/PCH.cpp"
    
    files
    {
        "Source/**.h",
        "Source/**.cpp",
    }

    removefiles
    {
        "Source/Platform/SDL/**"
    }
    
    includedirs
    {
        "Source",
        GetCommonIncludes()
    }

    links
    {
        "GLFW",
        "opengl32.lib"
    }

