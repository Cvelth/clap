#include "essential/log.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "nowide/iostream.hpp"

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

void clap::log::detail::stream::initialize_entry() {
	static size_t message_count = 0;
	if (static_cast<bool>(severity & ~detail::severity_mask::info_every)) {
		std::move(*this) << "\n!>> ";

		if (static_cast<bool>(severity & detail::severity_mask::error_every))
			std::move(*this) << "error ";
		else if (static_cast<bool>(severity & detail::severity_mask::warning_every))
			std::move(*this) << "warning ";
		else if (static_cast<bool>(severity & detail::severity_mask::message_every))
			std::move(*this) << "message ";
		else
			std::move(*this) << "unknown entry ";

		std::move(*this) << "[0x" << std::hex << message_count++ << std::dec << "] "
			<< verbose_time_stamp() << " <<!\n";
	}
}

void clap::log::detail::stream::finish_entry() {
	std::move(*this) << '\n';
	for (auto &wrapper : logger_state_ref.used_streams)
		std::visit(
			[](auto &ptr) { 
				if (ptr && *ptr)
					ptr->flush(); 
			}, *wrapper
		);

	if (static_cast<bool>(severity & logger_state_ref.exception_mask)) {
		std::move(*this) << "\nBecause of the error a 'logger_exception' is raised.\n";
		throw clap::log::detail::logger_exception();
	}

	if (static_cast<bool>(severity & logger_state_ref.termination_mask)) {
		std::move(*this) << "\nBecause of the error the program is being terminated.\n";
		std::terminate();
	}
}

clap::log::detail::logger_state_t &clap::logger() {
	static log::detail::logger_state_t object;
	return object;
}

clap::log::detail::logger_state_t::~logger_state_t() {
	log::message::minor << "Logging is over. Logger destructor was called.";
	log::info::major << used_streams.size() << " streams were used.";

	used_streams.clear();
}

void clap::log::detail::logger_state_t::add_stream(std::ostream &stream, logger_mask mask) {
	if (!stream) {
		log::warning::major << "'clap::detail::logger' cannot use the stream passed into 'add_stream'.";
		return;
	}

	log::message::minor << "Logging to a stream was initialized.";
	if (stream.rdbuf() == std::cout.rdbuf()) {
		log::info::critical << "Stream is 'std::cout'";
		if (replace_std_with_nowide) {
			log::info::critical << "'nowide::cout' is used instead";
			log::info::critical << "call 'clap::logger::disable_nowide_substitution() to prevent this behaviour";
			used_streams.emplace_back(nowide::cout, mask);
		} else
			used_streams.emplace_back(stream, mask);
	} else if (stream.rdbuf() == std::cerr.rdbuf()) {
		log::info::critical << "Stream is 'std::cerr'";
		if (replace_std_with_nowide) {
			log::info::critical << "'nowide::cerr' is used instead";
			log::info::critical << "call 'clap::logger::disable_nowide_substitution() to prevent this behaviour";
			used_streams.emplace_back(nowide::cerr, mask);
		} else
			used_streams.emplace_back(stream, mask);
	} else if (stream.rdbuf() == std::clog.rdbuf()) {
		log::info::critical << "Stream is 'std::clog'";
		if (replace_std_with_nowide) {
			log::info::critical << "'nowide::clog' is used instead";
			log::info::critical << "call 'clap::logger::disable_nowide_substitution() to prevent this behaviour";
			used_streams.emplace_back(nowide::clog, mask);
		} else
			used_streams.emplace_back(stream, mask);
	} else {
		log::info::critical << "Stream is a user-controled object";
		log::info::critical << "Make sure the lifetime of the stream exceeds lifetime of the logger.";
		used_streams.emplace_back(stream, mask);
	}
}

void clap::log::detail::logger_state_t::add_stream(std::wostream &stream, logger_mask mask) {
	if (!stream) {
		log::warning::major << "'clap::detail::logger' cannot use the stream passed into 'add_stream'.";
		return;
	}

	log::message::minor << "Logging to a stream was initialized.";
	if (stream.rdbuf() == std::wcout.rdbuf()) {
		log::info::critical << "Stream is 'std::wcout'";
		if (replace_std_with_nowide) {
			log::info::critical << "'nowide::cout' is used instead";
			log::info::critical << "call 'clap::logger::disable_nowide_substitution() to prevent this behaviour";
			used_streams.emplace_back(nowide::cout, mask);
		} else
			used_streams.emplace_back(stream, mask);
	} else if (stream.rdbuf() == std::wcerr.rdbuf()) {
		log::info::critical << "Stream is 'std::wcerr'";
		if (replace_std_with_nowide) {
			log::info::critical << "'nowide::cerr' is used instead";
			log::info::critical << "call 'clap::logger::disable_nowide_substitution() to prevent this behaviour";
			used_streams.emplace_back(nowide::cerr, mask);
		} else
			used_streams.emplace_back(stream, mask);
	} else if (stream.rdbuf() == std::wclog.rdbuf()) {
		log::info::critical << "Stream is 'std::wclog'";
		if (replace_std_with_nowide) {
			log::info::critical << "'nowide::clog' is used instead";
			log::info::critical << "call 'clap::logger::disable_nowide_substitution() to prevent this behaviour";
			used_streams.emplace_back(nowide::clog, mask);
		} else
			used_streams.emplace_back(stream, mask);
	} else {
		log::info::critical << "Stream is a user-controled object";
		log::info::critical << "Make sure the lifetime of the stream exceeds lifetime of the logger.";
		used_streams.emplace_back(stream, mask);
	}
}

void clap::log::detail::logger_state_t::add_file(std::filesystem::path const &filename, logger_mask mask) {
	add_file_wo_timestamp(std::filesystem::path(filename) += filename_time_stamp(), mask);
}

void clap::log::detail::logger_state_t::add_file_wo_timestamp(std::filesystem::path const &filename, logger_mask mask) {
	auto full_filename = std::filesystem::absolute((std::filesystem::path("log") / filename) += ".log");
	auto directory_path = full_filename.parent_path();

	if (!std::filesystem::exists(directory_path)) {
		std::filesystem::create_directories(directory_path);
		log::message::negligible << "Creating '" << directory_path << "' directory ";
	}

	auto ptr = new nowide::ofstream();
	ptr->open(nowide::narrow(full_filename.wstring()));
	if (ptr->fail()) {
		delete ptr;
		log::warning::major << "'clap::detail::logger' cannot open file '" << full_filename << "'.";
		return;
	}

	used_streams.emplace_back(ptr, mask);
	log::message::minor << "Logging to '" << full_filename << "' was initialized.";
}

clap::log::detail::stream_wrapper::~stream_wrapper() {
	std::visit(
		overload{
			[](nowide::ofstream *stream) {
				if (stream)
					delete stream;
			},
			[](auto *stream) {}
		}, stream
	);
}

clap::log::detail::stream_wrapper::stream_wrapper(stream_wrapper &&other)
	: stream(other.stream), mask(other.mask), write_next_info(other.write_next_info) {
	std::visit(
		[](auto &stream) {
			stream = nullptr;
		}, other.stream);
}

clap::log::detail::stream_wrapper::operator bool() const {
	return std::visit(
		[](auto const *stream) {
			return stream && bool(*stream);
		}, stream
	);
}
