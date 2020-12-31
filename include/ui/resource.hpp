#pragma once
#include <memory>
#include <utility>
namespace vk {
	template <typename Type, typename Dispatch> class UniqueHandle;
	class DispatchLoaderStatic;
	class ShaderModule;

	using UniqueShaderModule = UniqueHandle<ShaderModule, DispatchLoaderStatic>;
	enum class ShaderStageFlagBits : uint32_t;
}
namespace clap::resource_manager::detail {
	namespace shader {
		struct loaded_t {
			std::unique_ptr<vk::UniqueShaderModule> module;
			vk::ShaderStageFlagBits stage;

			loaded_t(vk::UniqueShaderModule &&module, vk::ShaderStageFlagBits stage);
			loaded_t(loaded_t const &) = delete;
			loaded_t(loaded_t&&) = default;
			loaded_t();
			~loaded_t();
			operator bool() const { return bool(module); }
		};
		template <typename tag_t> class loader {
		public:
			loaded_t get(std::string_view const &identificator);
			loaded_t try_get(std::string_view const &identificator);
			inline decltype(auto) operator[](std::string_view const &identificator) {
				return get(identificator);
			}
		};
	}
	class vertex; class fragment;
	class tesselation_control;
	class tesselation_evaluation;
	class geometry; class compute;

	class unknown;
}
namespace clap::resource::load::shader {
	extern resource_manager::detail::shader::loader<resource_manager::detail::vertex> vertex;
	extern resource_manager::detail::shader::loader<resource_manager::detail::fragment> fragment;
	extern resource_manager::detail::shader::loader<resource_manager::detail::tesselation_control> tesselation_control;
	extern resource_manager::detail::shader::loader<resource_manager::detail::tesselation_evaluation> tesselation_evaluation;
	extern resource_manager::detail::shader::loader<resource_manager::detail::geometry> geometry;
	extern resource_manager::detail::shader::loader<resource_manager::detail::compute> compute;
}