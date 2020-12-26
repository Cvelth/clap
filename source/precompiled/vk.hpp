#pragma once
#include "essential/log.hpp"
#include "essential/utility.hpp"
using namespace clap::literals;

#define VKFW_NO_LEADING_e_IN_ENUMS
#define VKFW_NO_STRUCT_CONSTRUCTORS
#include "vkfw/vkfw.hpp"

#include "vk/vk.hpp"

#include <string_view>