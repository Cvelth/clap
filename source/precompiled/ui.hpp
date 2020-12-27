#pragma once

#define VKFW_NO_STRUCT_CONSTRUCTORS
#include "vkfw/vkfw.hpp"

#include "essential/log.hpp"
#include "essential/utility.hpp"
#include "resource/resource.hpp"
#include "ui/detail/manager.hpp"
#include "ui/detail/vulkan.hpp"
#include "ui/compound.hpp"
#include "ui/loop.hpp"
#include "ui/zone.hpp"
using namespace clap::literals;

#include <concepts>
#include <functional>
#include <iterator>
#include <limits>
#include <string_view>
#include <thread>
#include <variant>