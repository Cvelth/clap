#pragma once
#include <string>

namespace engine {
	enum class window_mode {
		fullscreen,
		windowed,
		borderless_window
	};
	class glfw_window;

	class window_interface {
	public:
		explicit window_interface(std::string const &title, window_mode mode, size_t width, size_t height);
		explicit window_interface(std::string const &title, window_mode mode = window_mode::borderless_window);
		virtual ~window_interface();

		void update();
		void poll_events();
		void wait_events();

		size_t width();
		size_t height();

		bool should_close();

		int loop();

	protected:
		virtual void initialize() = 0;
		virtual void render() = 0;
		virtual void cleanup() = 0;

	protected:
		double aspect_ratio;

	private:
		glfw_window *handle;
	};
}