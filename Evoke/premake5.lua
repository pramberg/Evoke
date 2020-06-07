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
        GetCommonIncludes(),
        GetGraphicsIncludes()
    }

    links
    {
        "GLFW",
        "Glad",
        "opengl32.lib"
    }

    filter "system:windows"
        defines "GLFW_INCLUDE_NONE"
