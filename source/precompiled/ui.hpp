#pragma once

#define VKFW_NO_STRUCT_CONSTRUCTORS
#include "vkfw/vkfw.hpp"

#define VKMA_NO_STRUCT_CONSTRUCTORS
#include "vkma.hpp"

#include "essential/log.hpp"
#include "essential/utility.hpp"
using namespace clap::literals;

#include <algorithm>
#include <array>
#include <concepts>
#include <functional>
#include <iterator>
#include <limits>
#include <memory>
#include <optional>
#include <semaphore>
#include <span>
#include <string_view>
#include <thread>
#include <variant>