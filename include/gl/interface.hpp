#pragma once
#include <ostream>

namespace clap::gl::detail {
	class context;
	namespace detail {
		class access_guard;
	}

	class object_interface {
	public:
		object_interface();
		object_interface(object_interface const &);
		object_interface(object_interface &&);
		object_interface &operator=(object_interface const &);
		object_interface &operator=(object_interface &&);

	public:
		static detail::access_guard access_context_s();
		detail::access_guard access_context() const;
		detail::access_guard access_context(object_interface const &another) const;

		friend inline std::ostream &operator<<(std::ostream &stream, object_interface const &object) {
			return (object, stream << "polymorphic gl object (passed as an 'gl::object_interface' pointer/reference).");
		}

	protected:
		context *context_ptr;
	};
}