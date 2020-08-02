os.chdir("../Assimp-5.0.1")

project "Assimp"
	kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"
    systemversion "latest"

    -- Assimp has quite a lot of warnings, we don't care.
    warnings "Off"

	AddTargetDirs()

	files {
        "include/**",
        "code/**.cpp",
        "code/**.h",
        "contrib/irrXML/*",
        "contrib/openddlparser/code/**",
        "contrib/zlib/*.h",
        "contrib/zlib/*.c",
        "contrib/unzip/*",
    }

    includedirs {
        "include",
        "code",
        "contrib",
        "contrib/irrXML",
        "contrib/zlib",
        "contrib/rapidjson/include",
        "contrib/zlib",
        "contrib/zip/src",
        "contrib/unzip",
        "contrib/openddlparser/include",
        "."
    }
    
    defines {
        "ASSIMP_BUILD_NO_OWN_ZLIB",
        "ASSIMP_BUILD_NO_C4D_IMPORTER",
        "ASSIMP_BUILD_NO_IFC_IMPORTER",
        "OPENDDLPARSER_BUILD"
    }

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
        optimize "on"
        
    -- Required when building debug in Visual Studio
    filter "system:windows"
        buildoptions { "/bigobj" }
