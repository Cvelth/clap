local template = {}
local premake_template = require(path.getrelative(_SCRIPT_DIR, _MAIN_SCRIPT_DIR) .. "/script/premake/template")

template.supported_shader_stages = {
    "vertex", "fragment",
    "tesscontrol", "tesseval",
    "geometry", "compute"
}

function template.files(name, prefix)
    premake_template.files(name, prefix)
    local shader_source = "source/shader/" .. name .. "/"
    local shader_include = "include/shader/" .. name .. "/"
    local shader_resource = "resource/shader/"
    if prefix then 
        shader_source = prefix .. "/" .. name .. "/source/shader/"
        shader_include = prefix .. "/" .. name .. "/include/shader/"
        shader_resource = prefix .. "/" .. name .. "/resource/shader/"
    end

    for id, shader_stage in pairs(template.supported_shader_stages) do
        files {
            shader_source .. shader_stage .. "/**.*lsl",
            shader_include .. shader_stage .. "/**.*lsl",
        }
        filter {"files:" .. shader_source .. shader_stage .. "**.*lsl"}
            buildmessage("Compiling a " .. shader_stage .. " shader: " .. "%{file.abspath}")
            buildcommands {
                "{MKDIR} %{cfg.objdir}/" .. shader_stage,
                "glslc -fshader-stage=" .. shader_stage .. " %{file.relpath} "
                    .. "-o %{cfg.objdir}/" .. shader_stage .. "/%{file.basename}.spv",

                "{COPY} \"%{cfg.objdir}/" .. shader_stage .. "/%{file.basename}.spv\" \""
                    .. _MAIN_SCRIPT_DIR .. "/" .. shader_resource
                    .. shader_stage .. "/%{file.basename}.spv*\""
            }
            buildoutputs {
                "%{cfg.objdir}/" .. shader_stage .. "/%{file.basename}.spv",
                _MAIN_SCRIPT_DIR .. "/" .. shader_resource 
                    .. shader_stage .. "/%{file.basename}.spv"
            }
        filter {}
    end
end

return template