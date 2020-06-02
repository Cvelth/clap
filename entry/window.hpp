#pragma once
#include "../engine/window_interface.hpp"
#include "../engine/gl/shader.hpp"
#include "../engine/gl/vertex_array.hpp"

using engine::window_mode;

class window : public engine::window_interface {
public:
	using window_interface::window_interface;

protected:
	virtual void initialize() override;
	virtual void render() override;
	virtual void cleanup() override;

private:
	engine::gl::vertex_array::single vertex_array;
	engine::gl::shader::program program;
};