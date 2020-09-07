#pragma once
#include "gl/buffer.hpp"
#include "gl/misc.hpp"
#include "gl/shader.hpp"
#include "gl/texture.hpp"
#include "gl/vertex_array.hpp"

#define FORCE_NVIDIA_GPU_ON_OPTIMUS extern "C" { \
	_declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001; \
}