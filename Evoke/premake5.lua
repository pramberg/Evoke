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
    
    includedirs
    {
        "Source",
        "ThirdParty/ImGui",
        "ThirdParty/GLFW/include",
        "ThirdParty/Glad/include",
        "ThirdParty/ShaderConductor/include",
        "ThirdParty/EFSW/include",
        "ThirdParty/Assimp-5.0.1/include",
        "ThirdParty/PortableFileDialogs/",
        "ThirdParty/RLL/include",
        GetCommonIncludes()
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "ShaderConductor",
        "EFSW",
        "Assimp",
    }

    libdirs
    {
        "ThirdParty/ShaderConductor/lib",
        "ThirdParty/EFSW/lib",
    }

    filter "system:windows"
        defines "GLFW_INCLUDE_NONE"
