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
        "ThirdParty/ImGui",
        "ThirdParty/GLFW/include",
        "ThirdParty/Glad/include",
        "ThirdParty/DXC/include",
        GetCommonIncludes()
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib",
        "dxcompiler.lib"
    }

    libdirs
    {
        "ThirdParty/DXC/lib"
    }

    filter "system:windows"
        defines "GLFW_INCLUDE_NONE"
