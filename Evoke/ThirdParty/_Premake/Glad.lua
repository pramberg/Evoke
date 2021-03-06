os.chdir("../Glad")

project "Glad"
	kind "StaticLib"
	language "C"

	AddTargetDirs()

	files
	{
		"include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c"
    }

    includedirs
    {
        "include"
    }
    
    systemversion "latest"
    staticruntime "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
