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

    libdirs
    {
        "ThirdParty/DXC/lib"
    }

    postbuildcommands
    {
        "{COPY} %{wks.location}Evoke/ThirdParty/DXC/lib/dxcompiler.dll %{cfg.buildtarget.directory}",
        "{COPY} %{wks.location}Evoke/ThirdParty/DXC/lib/dxil.dll %{cfg.buildtarget.directory}"
	}