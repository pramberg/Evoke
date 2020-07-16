project "DefaultProject"
    kind "ConsoleApp"

    GetDefaultConfig()

    files
    {
        "Source/**.h",
        "Source/**.cpp",
    }

    includedirs
    {
        "../Evoke/Source",
        GetCommonIncludes()
    }

    links
    {
        "Evoke"
    }

    filter "system:windows"
        postbuildcommands
        {
            "{COPY} %{wks.location}Evoke/ThirdParty/ShaderConductor/bin/dxcompiler.dll %{cfg.buildtarget.directory}",
            "{COPY} %{wks.location}Evoke/ThirdParty/ShaderConductor/bin/ShaderConductor.dll %{cfg.buildtarget.directory}"
        }