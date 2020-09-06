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

	virtual bool on_mouse_button(clap::event::mouse_button button, 
								 clap::event::mouse_button_action action,
								 clap::event::modificator_mask modificators) override;

private:
	clap::gl::vertex_array::single vertex_array;
	clap::gl::shader::program program;
	clap::gl::shader::variables uniforms;

	unsigned mode;
};