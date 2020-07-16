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
        "ThirdParty/ShaderConductor/include",
        "ThirdParty/EFSW/include",
        GetCommonIncludes()
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "ShaderConductor",
        "EFSW"
    }

    libdirs
    {
        "ThirdParty/ShaderConductor/lib",
        "ThirdParty/EFSW/lib",
    }

    filter "system:windows"
        links
        {
            --"opengl32.lib", -- This doesn't seem to be necessary?
            --"ShaderConductor"
        }

        defines "GLFW_INCLUDE_NONE"
