workspace "InputDeviceCalibrator"
    architecture "x64"
    startproject "InputDeviceCalibrator"
    configurations {"Debug", "Release"}

include "vendor/GraphicsLibrary"

project "InputDeviceCalibrator"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin-int/%{cfg.buildcfg}"

    files {
        "src/*.h",
        "src/*.cpp"
    }

    includedirs {
        "vendor/GraphicsLibrary/include"
    }

    links {
        "GraphicsLibrary"
    }

    filter "system:linux"
        defines "IDC_LINUX"
        links { "X11", "Xrandr", "Xi"}
        files {
            "src/Platform/Linux/**.h",
            "src/Platform/Linux/**.cpp"
        }

    filter "system:windows"
        defines "IDC_WINDOWS"
    
    filter "configurations:Debug"
		defines "IDC_DEBUG"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines "IDC_RELEASE"
		optimize "On"
		runtime "Release"