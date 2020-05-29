#pragma once
#include <string>

namespace engine::error {
	void info(std::string description);
	void warn(std::string description);
	void critical(std::string description, int exit_code = -1);
}
