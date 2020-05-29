#include "error.hpp"
#include <ctime>
#include <chrono>
#include <iostream>
#include <fstream>

#ifdef _MSC_VER
	#pragma warning(disable: 4996)
#endif

class temporary_logger {
public:
	temporary_logger(std::string filename) {
		output_file.open(filename);
	}

	template <typename T>
	friend temporary_logger& operator<<(temporary_logger &tl, T const &value) {
		tl.output_file << value;
		std::cerr << value;
		return tl;
	}

private:
	std::ofstream output_file;
};

void log_error(std::string type, std::string description) {
	static temporary_logger error_logger("errors.txt");
	auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	error_logger << "  > " << std::ctime(&now) 
		<< type << (type != "" ? ": " : "") 
		<< description << '\n';
}

void engine::error::info(std::string description) {
	log_error("", description);
}
void engine::error::warn(std::string description) {
	log_error("Warning", description);
}
void engine::error::critical(std::string description, int exit_code) {
	log_error("Critical error", description);
	exit(exit_code);
}
