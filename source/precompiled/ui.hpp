#pragma once

#define VKFW_NO_STRUCT_CONSTRUCTORS
#include "vkfw/vkfw.hpp"

#include "essential/log.hpp"
#include "essential/utility.hpp"
#include "resource/resource.hpp"
#include "ui/detail/manager.hpp"
#include "ui/vulkan/core.hpp"
#include "ui/vulkan/pipeline.hpp"
#include "ui/compound.hpp"
#include "ui/loop.hpp"
#include "ui/zone.hpp"
using namespace clap::literals;

#include <algorithm>
#include <array>
#include <concepts>
#include <functional>
#include <iterator>
#include <limits>
#include <string_view>
#include <thread>
#include <variant>