os.chdir("../NativeFileDialog")

project "NativeFileDialog"
	kind "StaticLib"
    language "C"
    cppdialect "C++17"
    staticruntime "On"
    systemversion "latest"
    architecture "x86_64"

    AddTargetDirs()
    
    files {
        "src/*.h",
        "src/include/*.h",
        "src/nfd_common.c",
    }

    includedirs { "src/include/" }

    filter "system:windows"
        language "C++"
        files { "src/nfd_win.cpp" }

    filter { "action:gmake or action:xcode4" }
        buildoptions { "-fno-exceptions" }

    filter "system:macosx"
        language "C"
        files { "src/nfd_cocoa.m" }

    filter { "system:linux", "options:linux_backend=gtk3" }
        language "C"
        files { "src/nfd_gtk.c" }
        buildoptions { "`pkg-config --cflags gtk+-3.0`" }

    filter { "system:linux", "options:linux_backend=zenity" }
        language "C"
        files { "src/nfd_zenity.c" }

    filter "action:vs*"
        defines { "_CRT_SECURE_NO_WARNINGS" }    

	filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"

    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
        optimize "on"
        
    -- Required when building debug in Visual Studio
    filter "system:windows"
        buildoptions { "/bigobj" }
