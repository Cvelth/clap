#include "essential/log.hpp"

#include <iostream>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>
#include <semaphore>

#include "nowide/convert.hpp"
#include "nowide/fstream.hpp"
#include "nowide/iostream.hpp"

using namespace clap::literals;

static clap::logger::detail::severity_mask_t exception_mask = clap::logger::mask::nothing;
static clap::logger::detail::severity_mask_t termination_mask = clap::logger::mask::all_errors;
static bool nowide_substitution = true;

static std::mutex logger_mutex;
static std::counting_semaphore logger_semaphore(0);

struct queue_element {
	clap::logger::detail::entry_type_t type;
	std::vector<clap::logger::detail::entry_t::chunk_t> chunks;
	std::vector<std::u8string> tags;

	queue_element() {}
	queue_element(clap::logger::detail::entry_type_t &&type,
				  std::vector<clap::logger::detail::entry_t::chunk_t> &&chunks,
				  std::vector<std::u8string> &&tags)
		: type(std::move(type)), chunks(std::move(chunks)), tags(std::move(tags)) {}
};
static std::queue<queue_element> logger_queue;

template <typename underlying_t, bool is_owning>
class stream_wrapper {
public:
	inline stream_wrapper(underlying_t *stream) requires (!is_owning)
		: underlying(stream) {}
	inline stream_wrapper() requires (is_owning)
		: underlying(new underlying_t{}) {}
	inline ~stream_wrapper() {
		if constexpr (is_owning) if (underlying) delete underlying;
	}

	inline stream_wrapper(stream_wrapper const &another) requires (!is_owning)
		: underlying(another.underlying) {}
	inline stream_wrapper(stream_wrapper &&another) noexcept : underlying(another.underlying) {
		if constexpr (is_owning) another.underlying = nullptr;
	}

	inline stream_wrapper operator=(stream_wrapper const &another) requires (!is_owning) {
		underlying = another.underlying;
	}
	inline stream_wrapper operator=(stream_wrapper &&another) requires (!is_owning) {
		underlying = another.underlying; another.underlying = nullptr;
	}

	inline underlying_t *operator->() { return underlying; };
	inline underlying_t *operator->() const { return underlying; };
	inline underlying_t &operator*() { return *underlying; };
	inline underlying_t &operator*() const { return *underlying; };

	inline void write(char8_t data) {
		if (underlying && *underlying)
			(*underlying) << (char) data;
	}
	inline void write(char8_t const *data) {
		if (underlying && *underlying)
			if constexpr (std::is_same<std::wostream, underlying_t>::value)
				(*underlying) << nowide::widen((char const *) data);
			else
				(*underlying) << (char const *) data;
	}
	inline void flush() {
		if (underlying && *underlying)
			underlying->flush();
	}
private:
	underlying_t *underlying;
};
using default_stream = stream_wrapper<std::ostream, false>;
using wide_stream = stream_wrapper<std::wostream, false>;
using file_stream = stream_wrapper<nowide::ofstream, true>;

namespace clap::logger::detail {
	class universal_stream {
		friend universal_stream_preferences;
	public:
		inline universal_stream(default_stream &&stream, clap::logger::detail::severity_mask_t mask,
								clap::logger::detail::severity_mask_t extra_mask)
			: stream(std::move(stream)), mask(mask), extra_mask(extra_mask) {}
		inline universal_stream(wide_stream &&stream, clap::logger::detail::severity_mask_t mask,
								clap::logger::detail::severity_mask_t extra_mask)
			: stream(std::move(stream)), mask(mask), extra_mask(extra_mask) {}
		inline universal_stream(file_stream &&stream, clap::logger::detail::severity_mask_t mask,
								clap::logger::detail::severity_mask_t extra_mask)
			: stream(std::move(stream)), mask(mask), extra_mask(extra_mask) {}

		inline bool should_be_written(clap::logger::detail::entry_t::chunk_t const &chunk,
									  clap::logger::entry::type const &type) const {
			if (!chunk.is_extra)
				return mask(type, chunk.severity);
			else
				return extra_mask(type, chunk.severity);
		}
		inline void write(char8_t data) {
			std::visit([data](auto &stream) { stream.write(data); }, stream);
		}
		inline void write(char8_t const *data) {
			std::visit([data](auto &stream) { stream.write(data); }, stream);
		}
		inline void write(char data) { return write((char8_t) data); }
		inline void write(char const *data) { return write((char8_t const *) data); }
		inline void flush() { std::visit([](auto &stream) { stream.flush(); }, stream); }
		inline void auto_flush() { if (auto_flush_pref) flush(); }

		inline bool are_tags_enabled() const { return tags_pref; }
	private:
		std::variant<default_stream, wide_stream, file_stream> stream;
		clap::logger::detail::severity_mask_t mask;
		clap::logger::detail::severity_mask_t extra_mask;

		bool auto_flush_pref = false,
			tags_pref = false;
	};
}

static std::mutex stream_mutex;
static std::vector<clap::logger::detail::universal_stream> used_streams;
static void start_logging();

template <typename requested_stream_t, typename replacement_stream_t>
static clap::logger::detail::universal_stream_preferences
add_stream_impl(clap::logger::detail::entry_stream_t &logger_stream,
				requested_stream_t &requested_stream,
				replacement_stream_t &replacement_stream,
				std::string const &stream_name,
				clap::logger::detail::severity_mask_t const &mask,
				clap::logger::detail::severity_mask_t const &extra) {
	logger_stream << "Requested stream"_var << "std::" + stream_name;
	if (nowide_substitution) {
		logger_stream << "'nowide::" + stream_name + "' is used instead." << cL::negligible <<
			"Call 'clap::logger::disable_nowide_substitution()' before adding the stream "
			"to prevent the substitution.";
		std::scoped_lock _(stream_mutex);
		used_streams.emplace_back(&replacement_stream, mask, extra);
		return &used_streams.back();
	} else {
		std::scoped_lock _(stream_mutex);
		used_streams.emplace_back(&requested_stream, mask, extra);
		return &used_streams.back();
	}
}

clap::logger::detail::universal_stream_preferences
clap::logger::add_stream(std::ostream &stream, detail::severity_mask_t mask,
						 detail::severity_mask_t extra) {
	if (!stream) {
		clap::log << cL::warning << cL::major << "clap"_tag << "essential"_tag << "log"_tag << "logger"_tag
			<< "Ignore the stream passed into 'clap::logger::add_stream(...)'."
			<< cL::extra << cL::insignificant
			<< "The stream cannot be written to." << cL::extra
			<< "stream.operator!()"_fun << "true";
		return nullptr;
	} else {
		start_logging();
		auto logger_stream = clap::log.add_entry();
		logger_stream << cL::minor << "clap"_tag << "essential"_tag << "log"_tag << "logger"_tag
			<< "Add an additional stream to the logger.";
		if (stream.rdbuf() == std::cout.rdbuf())
			return add_stream_impl(logger_stream, std::cout, nowide::cout,
								   "cout", mask, extra);
		else if (stream.rdbuf() == std::cerr.rdbuf())
			return add_stream_impl(logger_stream, std::cerr, nowide::cerr,
								   "cerr", mask, extra);
		else if (stream.rdbuf() == std::clog.rdbuf())
			return add_stream_impl(logger_stream, std::clog, nowide::clog,
								   "clog", mask, extra);
		else {
			logger_stream << "Requested stream"_var << "a user-managed object"
				<< cL::negligible << "Make sure its lifetime exceeds lifetime of the logger.";
			std::scoped_lock _(stream_mutex);
			used_streams.emplace_back(&stream, mask, extra);
			return &used_streams.back();
		}
	}
}
clap::logger::detail::universal_stream_preferences
clap::logger::add_stream(std::wostream &stream, detail::severity_mask_t mask,
						 detail::severity_mask_t extra) {
	if (!stream) {
		clap::log << cL::warning << cL::major << "clap"_tag << "essential"_tag << "log"_tag << "logger"_tag
			<< "Ignore the stream passed into 'clap::logger::add_stream(...)'."
			<< cL::extra << cL::insignificant
			<< "The stream cannot be written to." << cL::extra
			<< "stream.operator!()"_fun << "true";
		return nullptr;
	} else {
		start_logging();

		auto logger_stream = clap::log.add_entry();
		logger_stream << cL::minor << "clap"_tag << "essential"_tag << "log"_tag << "logger"_tag
			<< "Add an additional stream to the logger.";
		if (stream.rdbuf() == std::wcout.rdbuf())
			return add_stream_impl(logger_stream, std::wcout, nowide::cout,
								   "wcout", mask, extra);
		else if (stream.rdbuf() == std::wcerr.rdbuf())
			return add_stream_impl(logger_stream, std::wcerr, nowide::cerr,
								   "wcerr", mask, extra);
		else if (stream.rdbuf() == std::wclog.rdbuf())
			return add_stream_impl(logger_stream, std::wclog, nowide::clog,
								   "wclog", mask, extra);
		else {
			logger_stream << "Requested stream"_var << "a user-controlled object"
				<< cL::negligible << "Make sure its lifetime exceeds lifetime of the logger.";
			std::scoped_lock _(stream_mutex);
			used_streams.emplace_back(&stream, mask, extra);
			return &used_streams.back();
		}
	}
}

//Temporary. To be redesigned after c++20 date-time support arrives.
#include <time.h>
#ifdef _MSC_VER
#pragma warning(push)
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
#ifdef _MSC_VER
#pragma warning(pop)
#endif

clap::logger::detail::universal_stream_preferences
clap::logger::add_file(std::filesystem::path const &path, detail::severity_mask_t mask,
					   detail::severity_mask_t extra) {
	return add_file_wo_timestamp(std::filesystem::path(path) += filename_time_stamp(), mask, extra);
}

clap::logger::detail::universal_stream_preferences
clap::logger::add_file_wo_timestamp(std::filesystem::path const &path, detail::severity_mask_t mask,
									detail::severity_mask_t extra) {
	auto full_filename = std::filesystem::absolute((std::filesystem::path("log") / path) += ".log");
	auto directory_path = full_filename.parent_path();

	if (!std::filesystem::exists(directory_path)) {
		std::filesystem::create_directories(directory_path);
		clap::log << cL::negligible << "clap"_tag << "essential"_tag << "log"_tag << "logger"_tag
			<< "Create '" << directory_path << "' directory.";
	}

	file_stream stream;
	stream->open(nowide::narrow(full_filename.wstring()));
	if (stream->good()) {
		start_logging();

		clap::log << cL::minor << "clap"_tag << "essential"_tag << "log"_tag << "logger"_tag
			<< "Add an additional stream to the logger."
			<< cL::extra << "The stream represents a file: '" << full_filename << "'.";
		std::scoped_lock _(stream_mutex);
		used_streams.emplace_back(std::move(stream), mask, extra);
		return &used_streams.back();
	} else {
		clap::log << cL::warning << cL::major << "clap"_tag << "essential"_tag << "log"_tag << "logger"_tag
			<< "Ignore the stream created from the file passed into 'clap::logger::add_file[...](...)'."
			<< cL::extra << cL::insignificant
			<< "The stream cannot be written to." << cL::extra
			<< "stream.operator!()"_fun << "true"
			<< "path"_var << full_filename;
		return nullptr;
	}
}
void clap::logger::enable_exceptions(detail::severity_mask_t mask) { exception_mask = mask; }
void clap::logger::enable_termination(detail::severity_mask_t mask) { termination_mask = mask; }
void clap::logger::enable_nowide_substitution(bool value) { nowide_substitution = value; }

static std::optional<queue_element> get_next_entry() {
	std::scoped_lock _(logger_mutex);
	if (logger_queue.empty())
		return std::nullopt;
	else {
		auto out = std::move(logger_queue.front());
		logger_queue.pop();
		return out;
	}
}
static bool is_there_a_next_entry() {
	std::scoped_lock _(logger_mutex);
	return !logger_queue.empty();
}

static std::ostringstream stream;
static bool once = false;
static size_t message_counter = 0;
class header_t {
public:
	explicit header_t(clap::logger::detail::entry_type_t const &type)
		: header_string(*type), type(type) {
		if (!once) {
			stream << std::hex;
			once = true;
		} else
			stream.str("");

		stream << message_counter++;

		header_string = header_string + u8" [0x" + (char8_t const *) stream.str().c_str() + u8"] "
			+ (char8_t const *) verbose_time_stamp().c_str();
	}
	std::u8string operator()(std::vector<std::u8string> &&tags, bool tags_enabled = false) {
		if (tags_enabled && !tags.empty()) {
			std::u8string tag_string = u8"tags: ";
			for (auto it = tags.begin(); it != (--tags.end()); it++)
				if (*it != u8"")
					tag_string += *it + u8", ";
			if (tags.back() != u8"")
				tag_string += tags.back();

			size_t header_spacing = (header_string.size() < tag_string.size()
									 ? tag_string.size() - header_string.size()
									 : 0),
				tags_spacing = (header_string.size() > tag_string.size()
								? header_string.size() - tag_string.size()
								: 0);

			return get_mark(type, true) + header_string + decltype(header_string)(header_spacing, ' ') + get_mark(type, false)
				+ get_mark(type, true) + decltype(tag_string)(tags_spacing, ' ') + tag_string + get_mark(type, false);
		} else
			return get_mark(type, true) + header_string + get_mark(type, false);
	}
protected:
	static inline char8_t const *get_mark(clap::logger::entry::type type, bool left) {
		switch (type) {
			case clap::logger::entry::type::error: return left ? u8"$ >> " : u8" << $\n";
			case clap::logger::entry::type::warning: return left ? u8"@ >> " : u8" << @\n";
			case clap::logger::entry::type::message: return left ? u8"! >> " : u8" << !\n";
			case clap::logger::entry::type::custom: return left ? u8"? >> " : u8" << ?\n";
			default: return left ? u8"  >> " : u8" <<  \n";
		}
	}
private:
	std::u8string header_string;
	clap::logger::entry::type type;
};

static bool is_active = false;
static std::thread logger_thread;
static void start_logging() {
	if (!is_active) {
		is_active = true;
		logger_thread = std::thread(
			[] {
				while (is_active || is_there_a_next_entry()) {
					logger_semaphore.acquire();
					auto entry_data = get_next_entry();
					if (entry_data) {
						header_t header(entry_data->type);
						std::scoped_lock _(stream_mutex);
						for (auto &stream : used_streams) {
							bool was_header_printed = false;
							for (auto const &chunk : entry_data->chunks) {
								if (stream.should_be_written(chunk, entry_data->type)) {
									if (!was_header_printed) {
										stream.write(header(std::move(entry_data->tags),
															stream.are_tags_enabled()).c_str());
										was_header_printed = true;
									}
									stream.write(chunk.value.c_str());
									stream.write("\n");
								}
							}
							if (was_header_printed) {
								stream.write(u8"\n");
								stream.auto_flush();
							}
						}
					}
				}
			}
		);

		struct auto_stopper_t { ~auto_stopper_t() { clap::logger::stop(); } };
		static auto_stopper_t auto_stopper;
	}
}
static void log_impl(clap::logger::detail::entry_type_t &&type,
					 std::vector<clap::logger::detail::entry_t::chunk_t> &&chunks,
					 std::vector<std::u8string> &&tags) {
	std::scoped_lock _(logger_mutex);
	logger_queue.emplace(std::move(type), std::move(chunks), std::move(tags));
}
void check_termination(clap::logger::entry::type const &type,
					   clap::logger::detail::severity_level_t &severity) {
	if (exception_mask(type, severity)) {
		clap::logger::stop();
		throw clap::logger::exception{};
	}
	if (termination_mask(type, severity)) {
		clap::logger::stop();
		std::terminate();
	}
}

clap::logger::detail::entry_t::~entry_t() {
	entry::type type = current_type;
	detail::severity_level_t maximum_severity = 0;
	for (auto const &chunk : chunks)
		if (!chunk.is_extra && chunk.severity > maximum_severity)
			maximum_severity = chunk.severity;

	std::visit([this]<typename mode_t>(mode_t) {
		if constexpr (std::is_same<normal_t, mode_t>::value) {
		} else if constexpr (std::is_same<function_t, mode_t>::value) {
			clap::log << cL::warning << cL::important << "clap"_tag << "essential"_tag << "log"_tag << "entry"_tag
				<< "Ignore a function literal because nothing was supplied to the logger after it.";
		} else if constexpr (std::is_same<variable_t, mode_t>::value) {
			clap::log << cL::warning << cL::important << "clap"_tag << "essential"_tag << "log"_tag << "entry"_tag
				<< "Ignore a variable literal because nothing was supplied to the logger after it.";
		} else
			static_assert(!always_false<value_t>::value,
						  "Fail to log an object. The mode of the logging stream is not supported.");
	}, std::move(current_mode));

	if (is_in_use) {
		log_impl(std::move(current_type), std::move(chunks), std::move(tags));
		logger_semaphore.release();
	} else {
		clap::log << cL::warning << cL::minor << "clap"_tag << "essential"_tag << "log"_tag << "entry"_tag
			<< "Ignore an empty log entry.";
	}

	check_termination(type, maximum_severity);
}

void clap::logger::start() { start_logging(); }
void clap::logger::stop() {
	if (is_active) {
		clap::log << cL::important << "clap"_tag << "essential"_tag << "log"_tag << "logger"_tag
			<< "Stop the logger." << cL::extra << used_streams.size() << " streams were used.";
		is_active = false;
		if (logger_thread.joinable())
			logger_thread.join();
	}
}
bool clap::logger::is_logging() {
	return is_active;
}

clap::logger::detail::universal_stream_preferences &
clap::logger::detail::universal_stream_preferences::with_tags(bool enable) {
	if (stream_ptr)
		stream_ptr->tags_pref = enable;
	else {
		clap::log << cL::warning << cL::minor << "clap"_tag << "essential"_tag << "log"_tag << "stream"_tag
			<< "Cannot change the preferences of a broken stream.";
	}
	return *this;
}
clap::logger::detail::universal_stream_preferences &
clap::logger::detail::universal_stream_preferences::with_auto_flush(bool enable) {
	if (stream_ptr)
		stream_ptr->auto_flush_pref = enable;
	else {
		clap::log << cL::warning << cL::minor << "clap"_tag << "essential"_tag << "log"_tag << "stream"_tag
			<< "Cannot change the preferences of a broken stream.";
	}
	return *this;
}