#pragma once
#include "window/window_interface.hpp"
#include "gl/gl.hpp"
#include "render/text.hpp"

namespace clap::render {
	class text;
}

using clap::window_mode;

struct text_with_position {
	std::unique_ptr<clap::render::text> text;
	int x, y;
};

class window : public clap::window_interface {
public:
	using window_interface::window_interface;

protected:
	virtual void initialize() override;
	virtual void render() override;
	virtual void cleanup() override;

	virtual bool on_resize(size_t width, size_t height) override;
	virtual bool on_mouse_hover(clap::event::mouse_hover_action action) override;

private:
	clap::gl::shader::program text_program;
	clap::gl::shader::variables variables;
	std::vector<text_with_position> text;

	size_t counter_value;
	std::vector<text_with_position>::iterator counter_text;

	bool hover_value;
	std::vector<text_with_position>::iterator hover_text;
};