#pragma once
#include <string>

#include "window/event_handler_interface.hpp"

namespace clap {
	enum class window_mode {
		fullscreen,
		windowed,
		borderless_window
	};
	namespace detail {
		class glfw_window;
	}

	class window_interface : public event::handler_interface {
	public:
		explicit window_interface(std::string const &title, window_mode mode, size_t width, size_t height);
		explicit window_interface(std::string const &title, window_mode mode = window_mode::borderless_window);
		virtual ~window_interface();

		void redraw();
		void poll_events();
		void wait_events();
		void wait_events(double seconds);

		size_t width();
		size_t height();

		bool should_close();
		bool should_update() const;

		void update();

		int loop();

	protected:
		virtual void initialize() = 0;
		virtual void render() = 0;
		virtual void cleanup() = 0;

	protected:
		double aspect_ratio;
		bool should_redraw;

	private:
		detail::glfw_window *handle;
	};
}