solution "TaxBlaster"
    configurations { "Debug", "Release" }

    project "TaxBlaster"
        kind "ConsoleApp"
        language "C++"
        files { "src/**.h", "src/**.cpp" }

        links { "sfml-graphics", "sfml-window", "sfml-system" }

        configuration { "linux", "gmake" }
            buildoptions { "-std=c++11" }

        configuration "Debug"
            defines { "DEBUG" }
            flags { "Symbols" }

        configuration "Release"
            defines { "NDEBUG" }
            flags { "Optimize" }
