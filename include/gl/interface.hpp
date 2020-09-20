#pragma once
namespace clap::gl {
	namespace detail {
		class context;
	}
	class object_interface {
	public:
		object_interface();
	protected:
		detail::context * const context;
	};
}