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
namespace clap::resource {
	namespace detail {
		namespace shader {
			struct loaded_t {
				std::unique_ptr<vk::UniqueShaderModule> module;
				vk::ShaderStageFlagBits stage;

				loaded_t(vk::UniqueShaderModule &&module, vk::ShaderStageFlagBits stage);
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
	namespace load::shader {
		extern detail::shader::loader<detail::vertex> vertex;
		extern detail::shader::loader<detail::fragment> fragment;
		extern detail::shader::loader<detail::tesselation_control> tesselation_control;
		extern detail::shader::loader<detail::tesselation_evaluation> tesselation_evaluation;
		extern detail::shader::loader<detail::geometry> geometry;
		extern detail::shader::loader<detail::compute> compute;
	}
}