#include "essential/log.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "nowide/convert.hpp"
#include "nowide/fstream.hpp"
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
	return current_time_stamp("%Y.%m.%d %I-%M-%S %p");
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
		std::move(*this) << "\nRaise a 'logger_exception'.\n";
		throw clap::log::detail::logger_exception();
	}

	if (static_cast<bool>(severity & logger_state_ref.termination_mask)) {
		std::move(*this) << "\nTerminate the program.\n";
		std::terminate();
	}
}

std::mutex clap::log::detail::stream::mutex;

clap::log::detail::logger_state_t &clap::logger() {
	static log::detail::logger_state_t object;
	return object;
}

clap::log::detail::logger_state_t::~logger_state_t() {
	log::message::minor << "Stop the logging. Logger destructor was called.";
	log::info::major << used_streams.size() << " streams were used.";

	used_streams.clear();
}

void clap::log::detail::logger_state_t::add_stream(std::ostream &stream, logger_mask mask) {
	if (!stream) {
		log::warning::major << "Ignore the stream passed into 'add_stream'.\n"
			"'clap::detail::logger' doesn't support it.";
		return;
	}

	log::message::minor << "Initialize logging to an additional stream.";
	if (stream.rdbuf() == std::cout.rdbuf()) {
		log::info::critical << "Requested stream is 'std::cout'.";
		if (replace_std_with_nowide) {
			log::info::critical << "'nowide::cout' is used instead.\n"
				"Call 'clap::logger::disable_nowide_substitution() before adding the stream to prevent this behaviour.";
			used_streams.emplace_back(nowide::cout, mask);
		} else
			used_streams.emplace_back(stream, mask);
	} else if (stream.rdbuf() == std::cerr.rdbuf()) {
		log::info::critical << "Requested stream is 'std::cerr'.";
		if (replace_std_with_nowide) {
			log::info::critical << "'nowide::cerr' is used instead.\n"
				"Call 'clap::logger::disable_nowide_substitution() before adding the stream to prevent this behaviour.";
			used_streams.emplace_back(nowide::cerr, mask);
		} else
			used_streams.emplace_back(stream, mask);
	} else if (stream.rdbuf() == std::clog.rdbuf()) {
		log::info::critical << "Requested stream is 'std::clog'.";
		if (replace_std_with_nowide) {
			log::info::critical << "'nowide::clog' is used instead.\n"
				"Call 'clap::logger::disable_nowide_substitution() before adding the stream to prevent this behaviour.";
			used_streams.emplace_back(nowide::clog, mask);
		} else
			used_streams.emplace_back(stream, mask);
	} else {
		log::info::critical << "Requested stream is a user-controled object.\n"
			"Make sure its lifetime exceeds lifetime of the logger.";
		used_streams.emplace_back(stream, mask);
	}
}

void clap::log::detail::logger_state_t::add_stream(std::wostream &stream, logger_mask mask) {
	if (!stream) {
		log::warning::major << "Ignore the stream passed into 'add_stream'.\n"
			"'clap::detail::logger' doesn't support it.";
		return;
	}

	log::message::minor << "Initialize logging to an additional stream.";
	if (stream.rdbuf() == std::wcout.rdbuf()) {
		log::info::critical << "Requested stream is 'std::wcout'.";
		if (replace_std_with_nowide) {
			log::info::critical << "'nowide::cout' is used instead.\n"
				"Call 'clap::logger::disable_nowide_substitution() before adding the stream to prevent this behaviour.";
			used_streams.emplace_back(nowide::cout, mask);
		} else
			used_streams.emplace_back(stream, mask);
	} else if (stream.rdbuf() == std::wcerr.rdbuf()) {
		log::info::critical << "Requested stream is 'std::wcerr'.";
		if (replace_std_with_nowide) {
			log::info::critical << "'nowide::cerr' is used instead.\n"
				"Call 'clap::logger::disable_nowide_substitution() before adding the stream to prevent this behaviour.";
			used_streams.emplace_back(nowide::cerr, mask);
		} else
			used_streams.emplace_back(stream, mask);
	} else if (stream.rdbuf() == std::wclog.rdbuf()) {
		log::info::critical << "Requested stream is 'std::wclog'.";
		if (replace_std_with_nowide) {
			log::info::critical << "'nowide::clog' is used instead.\n"
				"Call 'clap::logger::disable_nowide_substitution() before adding the stream to prevent this behaviour.";
			used_streams.emplace_back(nowide::clog, mask);
		} else
			used_streams.emplace_back(stream, mask);
	} else {
		log::info::critical << "Requested stream is a user-controled object.\n"
			"Make sure its lifetime exceeds lifetime of the logger.";
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
		log::message::negligible << "Create '" << directory_path << "' directory ";
	}

	file_wrapper wrapper;
	(&wrapper).open(nowide::narrow(full_filename.wstring()));
	if ((&wrapper).fail()) {
		log::warning::major << "Fail opening file: '" << full_filename << "'.\n"
			"It cannot be used as a logger target.";
		return;
	}

	used_streams.emplace_back(std::move(wrapper), mask);
	log::message::minor << "Initialize logging to a file: '" << full_filename << "'.";
}

clap::log::detail::stream_wrapper::operator bool() const {
	return std::visit(
		overload{
			[](auto const *stream) {
				return stream && bool(*stream);
			},
			[](file_wrapper const &stream) {
				return bool(*stream);
			}
		}, stream);
}

void clap::log::detail::nowide_ofstream_destructor_callable::operator()(nowide::ofstream *ptr) {
	delete ptr;
}

template std::basic_string<char8_t> nowide::utf::convert_string(const char32_t *begin, const char32_t *end);