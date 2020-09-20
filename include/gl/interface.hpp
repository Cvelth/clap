#pragma once
namespace clap::gl::detail {
	class object_interface;
	bool verify_context(object_interface const &);
	bool verify_context(object_interface const &, object_interface const &);

	class context;
	class object_interface {
		friend bool verify_context(object_interface const &);
		friend bool verify_context(object_interface const &, object_interface const &);

	public:
		object_interface();
		object_interface(object_interface const &);
		object_interface(object_interface &&);
		object_interface &operator=(object_interface const &);
		object_interface &operator=(object_interface &&);

	protected:
		inline bool verify_context() {
			return gl::detail::verify_context(*this);
		}

	protected:
		context *const context;
	};
}