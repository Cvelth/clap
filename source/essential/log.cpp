#include "essential/log.hpp"

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

void clap::log::detail::stream::initialize_writing() {
	static size_t message_count = 0;
	if (static_cast<bool>(severity & ~detail::severity_mask::info_every)) {
		std::move(*this) << "\n > "; 
		
		if (static_cast<bool>(severity & detail::severity_mask::error_every))
			std::move(*this) << "error ";
		else if (static_cast<bool>(severity & detail::severity_mask::warning_every))
			std::move(*this) << "warning ";
		else if (static_cast<bool>(severity & detail::severity_mask::message_every))
			std::move(*this) << "message ";

		std::move(*this) << "[0x" << std::hex << message_count++ << std::dec << "] " 
			<< verbose_time_stamp() << " <\n";
	}
}

void clap::log::detail::stream::finish_writing() {
	std::move(*this) << '\n';

	if (static_cast<bool>(severity & logger_ref.exception_mask)) {
		std::move(*this) << "\nBecause of the error a 'logger_exception' is raised.\n";
		throw clap::detail::logger_exception();
	}

	if (static_cast<bool>(severity & logger_ref.termination_mask)) {
		std::move(*this) << "\nBecause of the error the program is being terminated.\n";
		std::terminate();
	}
}

clap::detail::logger_t &clap::logger() {
	static detail::logger_t object;
	return object;
}

clap::detail::logger_t::~logger_t() {
	log::message::minor << "Logging is over. Logger destructor was called.";
	log::info::major << other_streams.size() + owned_streams.size() << " streams were used.";
	log::info::major << owned_streams.size() << " streams were owned.";

	for (auto &stream_ptr : owned_streams)
		delete stream_ptr.first;
}

void clap::detail::logger_t::add_stream(std::ostream &stream, logger_mask mask) {
	if (!stream) {
		log::warning::major << "'clap::detail::logger' cannot use the stream passed into 'add_stream'.";
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

void clap::detail::logger_t::add_file(std::filesystem::path const &filename, logger_mask mask) {
	add_file_wo_timestamp(std::filesystem::path(filename) += filename_time_stamp(), mask);
}

void clap::detail::logger_t::add_file_wo_timestamp(std::filesystem::path const &filename, logger_mask mask) {
	auto full_filename = (std::filesystem::path("log") / filename) += ".log";
	auto directory_path = full_filename.parent_path();

	if (!std::filesystem::exists(directory_path)) {
		std::filesystem::create_directories(directory_path);
		log::message::negligible << "Creating '" << directory_path << "' directory ";
	}

	auto ptr = new std::ofstream();
	ptr->open(full_filename);
	if (ptr->fail()) {
		delete ptr;
		log::warning::major << "'clap::detail::logger' cannot open file '" << full_filename << "'.";
		return;
	}

	owned_streams.insert(std::make_pair(ptr, std::make_pair(mask, false)));

	log::message::minor << "Logging to '" << full_filename << "' was initialized.";
}
