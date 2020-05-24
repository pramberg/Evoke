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