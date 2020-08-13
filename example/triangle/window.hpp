#pragma once
#include "window/window_interface.hpp"
#include "gl/gl.hpp"

using clap::window_mode;

class window : public clap::window_interface {
public:
	using window_interface::window_interface;

protected:
	virtual void initialize() override;
	virtual void render() override;
	virtual void cleanup() override;

private:
	clap::gl::vertex_array::single vertex_array;
	clap::gl::shader::program program;
};