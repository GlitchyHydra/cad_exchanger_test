workspace "CadExchangerTest"
    architecture "x64"
    startproject "Sandbox"
    configurations {"Debug", "Release"}

IncludeDir = {}
IncludeDir["tbb"] = "Sandbox/vendor/oneapi_tbb/include"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group ""

project "Curves"
    location "Curves"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
	pchheader "hzpch.h"
	pchsource "%{prj.name}/src/hzpch.cpp"
	
    files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.hpp"
    }
    
    defines
	{
        "CURVELIBRARY_EXPORTS"
	}

    includedirs
	{
		"%{prj.name}/src",
    }
    
	postbuildcommands
	{
		("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
	}
	
    filter "configurations:Debug"
        runtime "Debug"
		symbols "on"

        defines
        {
            
        }
       
        links
        {
        }

	filter "configurations:Release"
        runtime "Release"
		optimize "on"

        defines
        {

        }

        
        links
        {
        }

		
project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
	pchheader "hzpch.h"
	pchsource "%{prj.name}/src/hzpch.cpp"
	
    files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.hpp"
    }
    
    defines
	{
        
	}

    includedirs
	{
		"%{prj.name}/src",
		"Curves/src",
        "%{IncludeDir.tbb}",
    }

    links 
	{ 
       "Curves",
    }
    
    libdirs
    {
        "%{prj.name}/vendor/oneapi_tbb/lib/intel64/vc14",
        "%{prj.name}/vendor/oneapi_tbb/redist/intel64/vc14"
    }

    filter "configurations:Debug"
        runtime "Debug"
		symbols "on"

        defines
        {
            
        }
       
        debug_libs = 
        {
            "tbb_debug",
            "tbb12_debug",
            "tbbbind_debug.lib",
            "tbbmalloc_debug",
            "tbbmalloc_proxy_debug"
        }

        postbuildcommands
	    {
		    ("{COPY} vendor/oneapi_tbb/redist/intel64/vc14/*debug.dll \"../bin/" .. outputdir .. "/Sandbox/\"")
	    }

        links
        {
        }

	filter "configurations:Release"
        runtime "Release"
		optimize "on"

        defines
        {

        }

        postbuildcommands
	    {
		    ("{COPY} vendor/oneapi_tbb/redist/intel64/vc14/*.dll \"../bin/" .. outputdir .. "/Sandbox/\"")
	    }
        
        links
        {
            
        }
