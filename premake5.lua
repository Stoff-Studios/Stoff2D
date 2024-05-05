workspace "stoff2d"
   configurations { "Debug", "Release" }
   platforms { "x64" }

-- Module: core
project "stoff2d_core"

   kind "StaticLib"

   language "C"

   targetdir "bin/%{cfg.buildcfg}"

   objdir "bin-int"

   includedirs { 
       "stoff2d/stoff2d_core/include", 
       "stoff2d/vendor/include", "stoff2d/vendor/src"
   }

   libdirs { 
       "stoff2d/vendor/lib"
   }

   links {
       "glfw3", "opengl32"
   }

   filter { "system:windows" }
       links { 
           "user32", "gdi32"
       }

   files { 
       "stoff2d/stoff2d_core/**.h", 
       "stoff2d/stoff2d_core/**.c",
       "stoff2d/vendor/**.h",
       "stoff2d/vendor/**.c"
   }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

-- Module: ecs
project "stoff2d_ecs"

   kind "StaticLib"

   language "C"

   targetdir "bin/%{cfg.buildcfg}"

   objdir "bin-int"

   includedirs { 
       "stoff2d/stoff2d_ecs/include",
       "stoff2d/stoff2d_core/include",
       "stoff2d/vendor/include" 
   }

   filter { "system:windows" }
       links { "user32", "gdi32" }

   files { 
       "stoff2d/stoff2d_ecs/**.h", 
       "stoff2d/stoff2d_ecs/**.c", 
       "stoff2d/stoff2d_core/**.h",
       "stoff2d/vendor/**.h"
   }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

-- Example: shooter
project "shooter"

   kind "ConsoleApp"
   language "C"

   targetdir "bin/%{cfg.buildcfg}"
   objdir "bin-int"
   includedirs { 
       "stoff2d/stoff2d_core/include", 
       "stoff2d/stoff2d_ecs/include", 
       "stoff2d/vendor/include", 
       "examples/shooter/include"
   }

   links { "stoff2d_core", "stoff2d_ecs" }

   filter { "system:windows" }
       links { "user32", "gdi32" }

   files { 
       "examples/shooter/**.c", 
       "examples/shooter/**.h"
   }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
