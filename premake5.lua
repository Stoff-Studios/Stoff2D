workspace "stoff2d"
   configurations { "Debug", "Release" }
   platforms { "x32", "x64" }

project "stoff2d"
   kind "StaticLib"
   language "C"
   targetdir "bin/%{cfg.buildcfg}"
   objdir "bin-int"
   includedirs { "stoff2d/include", "stoff2d/vendor/include" }
   libdirs { "stoff2d/vendor/lib" }
   links { "glfw3", "opengl32" }
   filter { "system:windows" }
       links { "user32", "gdi32" }

   files { "stoff2d/**.h", "stoff2d/**.c" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

project "Shooter"
   kind "ConsoleApp"
   language "C"
   targetdir "bin/%{cfg.buildcfg}"
   objdir "bin-int"
   includedirs { "stoff2d/include", "stoff2d/vendor/include", "examples/shooter/include" }
   links { "stoff2d" }
   filter { "system:windows" }
       links { "user32", "gdi32" }

   files { "examples/shooter/**.c", "examples/shooter/**.h" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
