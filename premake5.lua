local third_party = require "third_party/third_party"
third_party.acquire "third_party"

workspace "clap"
	configurations { "release", "debug" }
	architecture "x86_64"

	filter "configurations:debug"
		defines { "_DEBUG" }
		symbols "On"
		optimize "Debug"
	filter "configurations:release"
		defines { "NDEBUG" }
		optimize "Full"
		flags { "NoBufferSecurityCheck", "NoRuntimeChecks" }
	filter "action:vs*"
		flags { "MultiProcessorCompile", "NoMinimalRebuild" }
		linkoptions { "/ignore:4006", "/ignore:4099" }
		defines {
			"_CRT_SECURE_NO_DEPRECATE",
			"_CRT_SECURE_NO_WARNINGS",
			"_CRT_NONSTDC_NO_WARNINGS"
		}
	filter {}
	
newoption {
	trigger = "output_directory",
	description = "A directory path for output binaries to be moved to.",
	value = "path"
}
newoption {
	trigger = "build_directory",
	description = "A directory path for temporary files to be generated in.",
	value = "path"
}

	targetdir (_OPTIONS["output_directory"]
		or "output/%{cfg.system}_%{cfg.buildcfg}")
	location (_OPTIONS["build_directory"] or "build")

function setup_vpaths(project_name)
	vpaths {
		{
			["precompiled"] = "**/precompiled/**.h*"
		}, {
			["precompiled/source"] = "**/precompiled/**.c*"
		}, {
			["include/detail"] = {
				"include/" .. project_name .. "/detail/**.h*",
				"example/" .. project_name .. "/include/detail/**.h*"
		 	}
		}, {
			["source/include/detail"] = {
				"source/" .. project_name .. "/detail/**.h*",
				"example/" .. project_name .. "/source/detail/**.h*"
			}
		}, {
			["source/include"] = {
				"source/" .. project_name .. "/**.h*",
				"example/" .. project_name .. "/source/**.h*"
			}
		}, {
			["source/detail"] = {
				"source/" .. project_name .. "/detail/**.c*",
				"example/" .. project_name .. "/source/detail/**.c*"
			}
		}, {
			["source"] = {
				"source/" .. project_name .. "/**.c*",
				"example/" .. project_name .. "/source/**.c*"
			}
		}, {
			["include"] = {
				"include/" .. project_name .. "/**.h*",
				"example/" .. project_name .. "/include/**.h*"
			}
		}
	}
end

function project_template(project_name)
	project(project_name)
		targetdir (_OPTIONS["output_directory"]
			or "output/%{cfg.system}_%{cfg.buildcfg}")
	
		language "C++"
		cppdialect "C++latest"
		flags "FatalWarnings"
		warnings "Extra"

		includedirs {
			"include",
			"source"
		}
	
		defines {
			"ENGINE_NAME=\"clap\"",
			"ENGINE_VERSION_MAJOR=0",
			"ENGINE_VERSION_MINOR=1",
			"ENGINE_VERSION_PATCH=0"
		}

		setup_vpaths(project_name)
end

function default_project(project_name)
	project_template(project_name)
		location (_OPTIONS["build_directory"] or "build")
		files {
			"include/" .. project_name .. "/**.hpp",
			"source/" .. project_name .. "/**.hpp",
			"source/" .. project_name .. "/**.cpp"
		}
end
function default_pch_project(project_name)
	default_project(project_name)
		pchheader("precompiled/" .. project_name .. ".hpp")
		pchsource("source/precompiled/" .. project_name .. ".cpp")
		files {"source/precompiled/" .. project_name .. ".*"}
end

function example_project(project_name)
	project_template(project_name)
		targetdir (_OPTIONS["output_directory"]
			or "output/%{cfg.system}_%{cfg.buildcfg}")
		location ((_OPTIONS["build_directory"] or "build") .. "/example")
		files {
			"example/" .. project_name .. "/include/**.hpp",
			"example/" .. project_name .. "/source/**.hpp",
			"example/" .. project_name .. "/source/**.cpp"
		}
		includedirs {
			"example/" .. project_name .. "/include",
			"example/" .. project_name .. "/source"
		}
end
function example_pch_project(project_name)
	example_project(project_name)

	pchheader("precompiled/" .. project_name .. ".hpp")
	pchsource("example/" .. project_name
		.. "/source/precompiled/" .. project_name .. ".cpp")
	files {
		"example/" .. project_name
			.. "/source/precompiled/" .. project_name .. ".*"
	}
end

default_project "essential"
	kind "StaticLib"

	filter "configurations:release"
		third_party.link_release { "nowide" }
	filter "configurations:debug"
		third_party.link_debug { "nowide" }
	filter {}

default_pch_project "vk"
	kind "StaticLib"
	links "essential"

	filter "configurations:release"
		third_party.link_release { "glfw", "vkfw" }
	filter "configurations:debug"
		third_party.link_debug { "glfw", "vkfw" }
	filter {}

	includedirs { "%VULKAN_SDK%/Include" }
	libdirs { "%VULKAN_SDK%/Lib" }
	links { "vulkan-1" }

default_pch_project "ui"
	kind "StaticLib"
	links "vk"

group "example"
example_pch_project "triangle"
	kind "ConsoleApp"
	
	links "ui"
	defines {
		"APPLICATION_NAME=\"clap example: triangle\"",
		"APPLICATION_VERSION_MAJOR=0",
		"APPLICATION_VERSION_MINOR=1",
		"APPLICATION_VERSION_PATCH=0"
	}
