#include "precompiled/ui.hpp"

#include "resource/resource.hpp"

#include <fstream>

namespace clap::resource::load::shader {
	detail::shader::loader<detail::vertex> vertex;
	detail::shader::loader<detail::fragment> fragment;
	detail::shader::loader<detail::tesselation_control> tesselation_control;
	detail::shader::loader<detail::tesselation_evaluation> tesselation_evaluation;
	detail::shader::loader<detail::geometry> geometry;
	detail::shader::loader<detail::compute> compute;
}

template<typename tag_t> vk::ShaderStageFlagBits get_type();
template<> vk::ShaderStageFlagBits get_type<clap::resource::detail::vertex>() { return vk::ShaderStageFlagBits::eVertex; }
template<> vk::ShaderStageFlagBits get_type<clap::resource::detail::fragment>() { return vk::ShaderStageFlagBits::eFragment; }
template<> vk::ShaderStageFlagBits get_type<clap::resource::detail::tesselation_control>() { return vk::ShaderStageFlagBits::eTessellationControl; }
template<> vk::ShaderStageFlagBits get_type<clap::resource::detail::tesselation_evaluation>() { return vk::ShaderStageFlagBits::eTessellationEvaluation; }
template<> vk::ShaderStageFlagBits get_type<clap::resource::detail::geometry>() { return vk::ShaderStageFlagBits::eGeometry; }
template<> vk::ShaderStageFlagBits get_type<clap::resource::detail::compute>() { return vk::ShaderStageFlagBits::eCompute; }

namespace clap::resource::detail::shader {
	loaded_t::loaded_t(vk::UniqueShaderModule &&module, vk::ShaderStageFlagBits stage)
		: module(new vk::UniqueShaderModule(std::move(module))), stage(stage) {}
	loaded_t::loaded_t() : module(nullptr), stage(vk::ShaderStageFlagBits{ 0u }) {}
	loaded_t::~loaded_t() = default;

	template<typename tag_t> inline loaded_t get_impl(std::filesystem::directory_entry const &file) {
		if (auto &device = clap::ui::vulkan::device(); device)
			if (std::ifstream stream(file.path(), std::ios::ate | std::ios::binary); stream) {
				std::size_t size = stream.tellg();
				std::vector<std::byte> buffer(size);
				stream.seekg(0);
				stream.read(reinterpret_cast<char *>(buffer.data()), size);
				if (!buffer.empty())
					return loaded_t (
						device.createShaderModuleUnique (
							vk::ShaderModuleCreateInfo {
								.codeSize = buffer.size(),
								.pCode = reinterpret_cast<uint32_t const *>(buffer.data())
							}
						), get_type<tag_t>()
					);
			}
		return {};
	}
	template<typename tag_t> inline loaded_t loader<tag_t>::get(std::string_view const &identificator) {
		if (auto const *file = resource::detail::storage<tag_t>::get(identificator); file)
			return get_impl<tag_t>(*file);
		else
			return {};
	}
	template<typename tag_t> inline loaded_t loader<tag_t>::try_get(std::string_view const &identificator) {
		if (auto const *file = resource::detail::storage<tag_t>::try_get(identificator); file)
			return get_impl<tag_t>(*file);
		else
			return {};
	}

	template loaded_t loader<vertex>::get(std::string_view const &identifier);
	template loaded_t loader<fragment>::get(std::string_view const &identifier);
	template loaded_t loader<tesselation_control>::get(std::string_view const &identifier);
	template loaded_t loader<tesselation_evaluation>::get(std::string_view const &identifier);
	template loaded_t loader<geometry>::get(std::string_view const &identifier);
	template loaded_t loader<compute>::get(std::string_view const &identifier);
	template loaded_t loader<vertex>::try_get(std::string_view const &identifier);
	template loaded_t loader<fragment>::try_get(std::string_view const &identifier);
	template loaded_t loader<tesselation_control>::try_get(std::string_view const &identifier);
	template loaded_t loader<tesselation_evaluation>::try_get(std::string_view const &identifier);
	template loaded_t loader<geometry>::try_get(std::string_view const &identifier);
	template loaded_t loader<compute>::try_get(std::string_view const &identifier);
}
