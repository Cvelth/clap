local templated = require "script/premake/template"
templated.third_party "third_party"

templated.language = "C++"
templated.cppdialect = "C++latest"
templated.manifest_path = ".manifest"
templated.vpaths = function(project_name) return {
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
} end

templated.workspace "clap"
	defines {
		"CLAP_NAME=\"clap\"",
		"CLAP_VERSION_MAJOR=0",
		"CLAP_VERSION_MINOR=1",
		"CLAP_VERSION_PATCH=0"
	}

templated.project "essential"
	templated.kind "StaticLib"
	templated.files "essential"
templated.project "resource"
	templated.kind "StaticLib"
	templated.files "resource"
	links "essential"
	depends "ryml"
templated.project "vk"
	templated.kind "StaticLib"
	templated.files "vk"
	templated.pch "vk"
	links "resource"
	depends { "glfw", "vkfw", "vulkan" }
templated.project "ui"
	templated.kind "StaticLib"
	templated.files "ui"
	templated.pch "ui"
	links "vk"
	depends { "glfw", "vkfw", "vulkan" }

group "example"
templated.project "triangle"
	templated.location "../example/triangle/build"
	templated.kind "ConsoleApp"
	templated.files("triangle", "example")
	templated.pch("triangle", "example")
	links "ui"
