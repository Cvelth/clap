#pragma once
#include "../engine/window_interface.hpp"

using engine::window_mode;

class window : public engine::window_interface {
public:
	using window_interface::window_interface;

protected:
	virtual void initialize() override;
	virtual void render() override;
	virtual void cleanup() override;
};