workspace "RapidFuzz"
	architecture "x64"
	configurations { "Debug", "Release" }
	startproject "RapidFuzzSample"
	characterset "Unicode"
	
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "On"
		
	filter "configurations:Release"
		defines "NDEBUG"
		runtime "Release"
		optimize "On"

project "RapidFuzzSample"
	kind "ConsoleApp"
	language "C++"
	staticruntime "On"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files { 
		"rapidfuzz/**.hpp",
		"rapidfuzz/**.h",
		"rapidfuzz/**.c",
		"rapidfuzz/**.cpp",
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp" }
	
	includedirs {
		"./",
		"%{prj.name}",
		"rapidfuzz"
	}
	
	filter "system:windows"
		cppdialect "C++20"
		systemversion "latest"
