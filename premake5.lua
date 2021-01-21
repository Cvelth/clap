local templated = require "script/premake/template"
local clapped = require "script/template"
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
		["shader/include/*"] = {
			"include/" .. project_name .. "/shader/*/**.*lsl",
			"example/" .. project_name .. "/include/shader/*/**.*lsl"
		 }
	}, {
		["shader/*"] = {
			"source/" .. project_name .. "/shader/*/**.*lsl",
			"example/" .. project_name .. "/source/shader/*/**.*lsl",
		 }
	}, {
		["include/detail/**"] = {
			"include/" .. project_name .. "/detail/**.h*",
			"example/" .. project_name .. "/include/detail/**.h*"
		 }
	}, {
		["source/detail/include/**"] = {
			"source/" .. project_name .. "/detail/**.h*",
			"example/" .. project_name .. "/source/detail/**.h*"
		}
	}, {
		["source/include/**"] = {
			"source/" .. project_name .. "/**.h*",
			"example/" .. project_name .. "/source/**.h*"
		}
	}, {
		["source/detail/**"] = {
			"source/" .. project_name .. "/detail/**.c*",
			"example/" .. project_name .. "/source/detail/**.c*"
		}
	}, {
		["source/**"] = {
			"source/" .. project_name .. "/**.c*",
			"example/" .. project_name .. "/source/**.c*"
		}
	}, {
		["include/**"] = {
			"include/" .. project_name .. "/**.h*",
			"example/" .. project_name .. "/include/**.h*"
		}
	}, {
		[""] = "**.manifest"
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
	clapped.files "essential"
templated.project "resource"
	templated.kind "StaticLib"
	clapped.files "resource"
	links "essential"
	depends "ryml"
templated.project "ui"
	templated.kind "StaticLib"
	templated.pch "ui"
	clapped.files "ui"
	links "resource"
	depends { "glfw", "vkfw", "vulkan", "vma", "vkma" }

group "example"
templated.project "triangle"
	templated.location "../example/triangle/build"
	templated.kind "ConsoleApp"
	templated.pch("triangle", "example")
	clapped.files("triangle", "example")
	links "ui"
	depends { "glfw", "vkfw", "vulkan", "vma", "vkma" }
