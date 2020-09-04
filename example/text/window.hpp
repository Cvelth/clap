#pragma once
#include "window/window_interface.hpp"
#include "gl/gl.hpp"
#include "render/text.hpp"

namespace clap::render {
	class text;
}

using clap::window_mode;

class window : public clap::window_interface {
public:
	using window_interface::window_interface;

protected:
	virtual void initialize() override;
	virtual void render() override;
	virtual void cleanup() override;

	virtual void on_resize(size_t width, size_t height);

private:
	clap::gl::vertex_array::single vertex_array;
	clap::gl::shader::program text_atlas_program, text_program;
	clap::gl::shader::variables variables;
	std::unique_ptr<clap::render::text> text;
};