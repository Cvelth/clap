#include "log.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>

//Temporary. To be redesigned after c++20 date-time support arrives.
#include <time.h>
#ifdef _MSC_VER
	#pragma warning(disable: 4996)
#endif
std::string current_time_stamp(const char *mask) {
	auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	struct tm time_struct = *localtime(&now);
	char buffer[80];
	strftime(buffer, sizeof(buffer), mask, &time_struct);

	return buffer;
}
std::string verbose_time_stamp() {
	return current_time_stamp("at %r on %B %d, %Y");
}
std::string filename_time_stamp() {
	return current_time_stamp("%Y-%m-%d %H-%M-%S");
}

void engine::log::detail::stream::initialize_writing() {
	static size_t message_count = 0;
	if (static_cast<bool>(mask & ~detail::mask::info_every)) {
		std::move(*this) << "\n > "; 
		
		if (static_cast<bool>(mask & detail::mask::error_every))
			std::move(*this) << "error ";
		else if (static_cast<bool>(mask & detail::mask::warning_every))
			std::move(*this) << "warning ";
		else if (static_cast<bool>(mask & detail::mask::message_every))
			std::move(*this) << "message ";

		std::move(*this) << "[0x" << std::hex << message_count++ << std::dec << "] " 
			<< verbose_time_stamp() << " <\n";
	}
}

void engine::log::detail::stream::finish_writing() {
	std::move(*this) << '\n';

	if (static_cast<bool>(mask & logger_ref.exception_mask)) {
		std::move(*this) << "\nBecause of the error a 'logger_exception' is raised.\n";
		throw engine::detail::logger_exception();
	}

	if (static_cast<bool>(mask & logger_ref.termination_mask)) {
		std::move(*this) << "\nBecause of the error the program is being terminated.\n";
		std::terminate();
	}
}

engine::detail::logger_t &engine::logger() {
	static detail::logger_t object;
	return object;
}

engine::detail::logger_t::~logger_t() {
	log::message::minor << "Logging is over. Logger destructor was called.";
	log::info::major << other_streams.size() + owned_streams.size() << " streams were used.";
	log::info::major << owned_streams.size() << " streams were owned.";

	for (auto &stream_ptr : owned_streams)
		delete stream_ptr.first;
}

void engine::detail::logger_t::add_stream(std::ostream &stream, logger_mask mask) {
	if (!stream) {
		log::warning::major << "'engine::detail::logger' cannot use the stream passed into 'add_stream'.";
		return;
	}
	other_streams.insert(std::make_pair(&stream, std::make_pair(mask, false)));

	log::message::minor << "Logging to a stream was initialized.";
	if (stream.rdbuf() == std::cout.rdbuf())
		log::info::minor << "Stream is 'std::cout'";
	else if (stream.rdbuf() == std::cerr.rdbuf())
		log::info::minor << "Stream is 'std::cerr'";
	else {
		log::info::minor << "Stream is a user-object";
		log::info::critical << "Make sure the lifetime of the stream exceeds lifetime of the logger.";
	}
}

void engine::detail::logger_t::add_file(std::string const &filename, logger_mask mask) {
	add_file_wo_timestamp(filename + filename_time_stamp(), mask);
}

void engine::detail::logger_t::add_file_wo_timestamp(std::string const &filename, logger_mask mask) {
	auto full_filename = "log/" + filename + ".log";
	auto directory_path = full_filename.substr(0, full_filename.find_last_of('/'));
	
	if (!std::filesystem::exists(directory_path)) {
		std::filesystem::create_directories(directory_path);
		log::message::negligible << "Creating '" << directory_path << "' directory ";
	}

	auto ptr = new std::ofstream();
	ptr->open(full_filename);
	if (ptr->fail()) {
		delete ptr;
		log::warning::major << "'engine::detail::logger' cannot open file '" << full_filename << "'.";
		return;
	}

	owned_streams.insert(std::make_pair(ptr, std::make_pair(mask, false)));

	log::message::minor << "Logging to '" << full_filename << "' was initialized.";
}

engine::logger_mask mask_from_level(size_t level) {
	return engine::logger_mask(level);
}
