#include "essential/log.hpp"

#include <fstream>
#include <iostream>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>
#include <semaphore>

#ifdef _MSC_VER
// For SetConsoleCP and SetConsoleOutputCP
# include <Windows.h>
#endif

using namespace clap::literals;

//Temporary. To be redesigned after c++20 date-time support arrives.
#include <time.h>
#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable: 4996)
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
# pragma warning(pop)
#endif

static std::mutex logger_mutex;
static std::counting_semaphore logger_semaphore(0);

struct queue_element {
	clap::logger::detail::entry_type_t type;
	std::vector<clap::logger::detail::entry_t::chunk_t> chunks;
	std::vector<std::string> tags;

	queue_element() {}
	queue_element(clap::logger::detail::entry_type_t &&type,
				  std::vector<clap::logger::detail::entry_t::chunk_t> &&chunks,
				  std::vector<std::string> &&tags)
		: type(std::move(type)), chunks(std::move(chunks)), tags(std::move(tags)) {}
};
static std::queue<queue_element> logger_queue;

template <typename underlying_t, bool is_owning>
class stream_wrapper {
public:
	inline stream_wrapper(underlying_t *underlying_stream) requires (!is_owning)
		: underlying(underlying_stream) {}
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

	inline void write(char data) {
		if (underlying && *underlying)
			(*underlying) << data;
	}
	inline void write(char const *data) {
		if (underlying && *underlying)
			(*underlying) << data;
	}
	inline void flush() {
		if (underlying && *underlying)
			underlying->flush();
	}
private:
	underlying_t *underlying;
};
using ostream = stream_wrapper<std::ostream, false>;
using ofstream = stream_wrapper<std::ofstream, true>;

namespace clap::logger::detail {
	class stream {
		friend stream_preferences;
	public:
		inline stream(ostream &&stream, clap::logger::detail::severity_mask_t mask,
					  clap::logger::detail::severity_mask_t extra_mask)
			: underlying(std::move(stream)), mask(mask), extra_mask(extra_mask) {}
		inline stream(ofstream &&stream, clap::logger::detail::severity_mask_t mask,
					  clap::logger::detail::severity_mask_t extra_mask)
			: underlying(std::move(stream)), mask(mask), extra_mask(extra_mask) {}

		inline void write(char data) {
			std::visit([data](auto &stream) { stream.write(data); }, underlying);
		}
		inline void write(char const *data) {
			std::visit([data](auto &stream) { stream.write(data); }, underlying);
		}
		inline void flush() { std::visit([](auto &stream) { stream.flush(); }, underlying); }
		inline void auto_flush() { if (auto_flush_pref) flush(); }

		inline bool are_tags_enabled() const { return tags_pref; }
		inline clap::logger::detail::severity_mask_t get_mask() const { return mask; }
		inline clap::logger::detail::severity_mask_t get_extra_mask() const { return extra_mask; }
	private:
		std::variant<ostream, ofstream> underlying;
		clap::logger::detail::severity_mask_t mask;
		clap::logger::detail::severity_mask_t extra_mask;

		bool auto_flush_pref = false,
			tags_pref = false;
	};
}
 
static std::mutex stream_mutex;
static std::vector<clap::logger::detail::stream> used_streams;
static void start_logging();
clap::logger::detail::stream_preferences clap::logger::add_stream(std::ostream &stream,
																  detail::severity_mask_t mask,
																  detail::severity_mask_t extra) {
	if (!stream)
		clap::log << cL::warning << cL::major << "clap"_tag << "essential"_tag << "log"_tag << "logger"_tag
			<< "Ignore the stream passed into 'clap::logger::add_stream(...)'."
			<< cL::extra << cL::insignificant
			<< "The stream cannot be written to." << cL::extra
			<< "stream.operator!()"_fun << "true";
	else {
		start_logging();

		auto logger_stream = clap::log.add_entry();
		logger_stream << cL::minor << "clap"_tag << "essential"_tag << "log"_tag << "logger"_tag
			<< "Add an additional stream to the logger." << cL::extra;
		if (stream.rdbuf() == std::cout.rdbuf())
			logger_stream << "The stream is 'std::cout'";
		else if (stream.rdbuf() == std::cerr.rdbuf())
			logger_stream << "The stream is 'std::cerr'";
		else if (stream.rdbuf() == std::clog.rdbuf())
			logger_stream << "The stream is 'std::clog'";
		else
			logger_stream << "The stream is a user-managed object"
				<< cL::negligible << "Make sure its lifetime exceeds lifetime of the logger.";

		std::scoped_lock _(stream_mutex);
		return &used_streams.emplace_back(&stream, mask, extra);
	}
	return nullptr;
}
clap::logger::detail::stream_preferences clap::logger::add_file(std::filesystem::path const &path,
																detail::severity_mask_t mask,
																detail::severity_mask_t extra) {
	return add_file_wo_timestamp(std::filesystem::path(path) += filename_time_stamp(), mask, extra);
}

clap::logger::detail::stream_preferences clap::logger::add_file_wo_timestamp(std::filesystem::path const &path,
																			 detail::severity_mask_t mask,
																			 detail::severity_mask_t extra) {
	auto full_filename = std::filesystem::absolute((std::filesystem::path("log") / path) += ".log");
	auto directory_path = full_filename.parent_path();

	if (!std::filesystem::exists(directory_path)) {
		std::filesystem::create_directories(directory_path);
		clap::log << cL::negligible << "clap"_tag << "essential"_tag << "log"_tag << "logger"_tag
			<< "Create '" << directory_path << "' directory.";
	}

	ofstream stream;
	stream->open(full_filename.string());
	if (stream->good()) {
		start_logging();

		clap::log << cL::minor << "clap"_tag << "essential"_tag << "log"_tag << "logger"_tag
			<< "Add an additional stream to the logger."
			<< cL::extra << "The stream represents a file: '" << full_filename << "'.";
		std::scoped_lock _(stream_mutex);
		used_streams.emplace_back(std::move(stream), mask, extra);
		return &used_streams.back();
	} else
		clap::log << cL::warning << cL::major << "clap"_tag << "essential"_tag << "log"_tag << "logger"_tag
			<< "Ignore the stream created from the file passed into 'clap::logger::add_file[...](...)'."
			<< cL::extra << cL::insignificant
			<< "The stream cannot be written to." << cL::extra
			<< "stream.operator!()"_fun << "true"
			<< "path"_var << full_filename;
	return nullptr;
}

static std::optional<queue_element> get_next_entry() {
	std::scoped_lock _(logger_mutex);
	if (logger_queue.empty())
		return std::nullopt;
	else {
		auto out = std::move(logger_queue.front());
		logger_queue.pop();
		return std::move(out);
	}
}
static bool is_there_a_next_entry() {
	std::scoped_lock _(logger_mutex);
	return !logger_queue.empty();
}

static std::ostringstream helper_stream;
static bool helper_stream_initialized = false;
static size_t message_counter = 0;
class header_t {
public:
	header_t(clap::logger::detail::entry_type_t const &type, std::vector<std::string> &&tags)
		: header_string(*type), tag_string(), type(type)
		, tags(std::move(tags)), header_spacing(0), tags_spacing(0) {

		if (!helper_stream_initialized) {
			helper_stream << std::hex;
			helper_stream_initialized = true;
		} else {
			helper_stream.str("");
			helper_stream.clear();
		}

		helper_stream << message_counter++;
		header_string += " [0x" + helper_stream.str() + "] " + verbose_time_stamp();
	}
	std::string operator()(bool tags_enabled = false) {
		if (tags_enabled) {
			if (tag_string.empty() && !tags.empty()) {
				tag_string = "tags: ";
				for (auto it = tags.begin(); it != (--tags.end()); it++)
					if (*it != "")
						tag_string += *it + ", ";
				if (tags.back() != "")
					tag_string += tags.back();

				header_spacing = (header_string.size() < tag_string.size()
								  ? tag_string.size() - header_string.size()
								  : 0);
				tags_spacing = (header_string.size() > tag_string.size()
								? header_string.size() - tag_string.size()
								: 0);
			}
			return get_mark(type, true) + header_string + std::string(header_spacing, ' ') + get_mark(type, false)
				+ get_mark(type, true) + std::string(tags_spacing, ' ') + tag_string + get_mark(type, false);
		} else
			return get_mark(type, true) + header_string + get_mark(type, false);
	}
protected:
	static inline char const *get_mark(clap::logger::entry::type type, bool left) {
		switch (type) {
			case clap::logger::entry::type::error: return left ? "$ >> " : " << $\n";
			case clap::logger::entry::type::warning: return left ? "@ >> " : " << @\n";
			case clap::logger::entry::type::message: return left ? "! >> " : " << !\n";
			case clap::logger::entry::type::custom: return left ? "? >> " : " << ?\n";
			default: return left ? "  >> " : " <<  \n";
		}
	}
private:
	std::string header_string, tag_string;
	clap::logger::entry::type type;
	std::vector<std::string> &&tags;
	size_t header_spacing, tags_spacing;
};
inline bool check_chunk(clap::logger::detail::entry_t::chunk_t const &chunk,
						clap::logger::entry::type type,
						clap::logger::detail::severity_mask_t mask,
						clap::logger::detail::severity_mask_t extra_mask) {
	if (!chunk.is_extra)
		return mask(type, chunk.severity);
	else
		return extra_mask(type, chunk.severity);
}
inline std::string to_string(queue_element const &entry,
							 clap::logger::detail::severity_mask_t mask,
							 clap::logger::detail::severity_mask_t extra_mask) {
	std::string output;
	for (auto const &chunk : entry.chunks)
		if (check_chunk(chunk, entry.type, mask, extra_mask))
			output += chunk.value + '\n';
	return output;
}
inline std::string to_string(queue_element const &entry, clap::logger::detail::severity_mask_t mask) {
	return to_string(entry, mask, mask);
}
inline std::string to_string(queue_element const &entry) {
	return to_string(entry, clap::logger::mask::everything, clap::logger::mask::everything);
}

static bool is_active = false;
static std::thread logger_thread;
static void start_logging() {
#ifdef _MSC_VER
	static bool codepage_set = false;
	if (!codepage_set) {
		SetConsoleOutputCP(CP_UTF8);
		SetConsoleCP(CP_UTF8);
		codepage_set = true;
	}
#endif
	if (!is_active) {
		is_active = true;
		logger_thread = std::thread(
			[] {
				try {
					while (is_active || is_there_a_next_entry()) {
						logger_semaphore.acquire();
						if (auto entry = get_next_entry(); entry) {
							header_t header(entry->type, std::move(entry->tags));
							std::scoped_lock _(stream_mutex);
							for (auto &current_stream : used_streams) {
								auto body = to_string(entry.value(),
													  current_stream.get_mask(),
													  current_stream.get_extra_mask());
								if (!body.empty()) {
									current_stream.write(header(current_stream.are_tags_enabled()).c_str());
									current_stream.write(body.c_str());
									current_stream.write('\n');
									current_stream.auto_flush();
								}
							}
						}
					}
				} catch (...) {
					clap::log << cL::error << cL::important << "clap"_tag << "essential"_tag << "log"_tag << "logger_thread"_tag
						<< "Encounter an uncaught exception in the logger thread.";
				}
			}
		);

		struct stop_logging_on_destroy { ~stop_logging_on_destroy() { clap::logger::stop(); } };
		static stop_logging_on_destroy _;
	}
}

clap::logger::detail::entry_t::~entry_t() {
	std::visit([]<typename mode_t>(mode_t) {
		if constexpr (std::is_same<normal_t, mode_t>::value) {
		} else if constexpr (std::is_same<function_t, mode_t>::value) {
			clap::log << cL::warning << cL::important << "clap"_tag << "essential"_tag << "log"_tag << "entry"_tag
				<< "Ignore a function literal because nothing was supplied to the logger after it.";
		} else if constexpr (std::is_same<variable_t, mode_t>::value) {
			clap::log << cL::warning << cL::important << "clap"_tag << "essential"_tag << "log"_tag << "entry"_tag
				<< "Ignore a variable literal because nothing was supplied to the logger after it.";
		} else
			static_assert(always_false<mode_t>::value,
						  "Fail to log an object. "
						  "The mode of the logging stream is not supported.");
	}, std::move(mode));
	
	if (is_in_use) {
		{
			std::scoped_lock _(logger_mutex);
			logger_queue.emplace(std::move(type), std::move(chunks), std::move(tags));
		}
		logger_semaphore.release();
	} else
		clap::log << cL::warning << cL::minor << "clap"_tag << "essential"_tag << "log"_tag << "entry"_tag
			<< "Ignore an empty log entry.";
}

void clap::logger::start() { start_logging(); }
void clap::logger::stop() {
	if (is_active) {
		size_t stream_count = 0u;
		{
			std::scoped_lock guard(stream_mutex);
			stream_count = used_streams.size();
		}
		clap::log << cL::important << "clap"_tag << "essential"_tag << "log"_tag << "logger"_tag
			<< "Stop the logger." << cL::extra << stream_count << " streams were used.";
		is_active = false;
		if (logger_thread.joinable())
			logger_thread.join();
	}
}
bool clap::logger::is_logging() {
	return is_active;
}

clap::logger::detail::stream_preferences &
clap::logger::detail::stream_preferences::with_tags(bool enable) {
	if (stream_ptr)
		stream_ptr->tags_pref = enable;
	else {
		clap::log << cL::warning << cL::minor << "clap"_tag << "essential"_tag << "log"_tag << "stream"_tag
			<< "Fail to change the preferences of a broken stream.";
	}
	return *this;
}
clap::logger::detail::stream_preferences &
clap::logger::detail::stream_preferences::with_auto_flush(bool enable) {
	if (stream_ptr)
		stream_ptr->auto_flush_pref = enable;
	else {
		clap::log << cL::warning << cL::minor << "clap"_tag << "essential"_tag << "log"_tag << "stream"_tag
			<< "Fail to change the preferences of a broken stream.";
	}
	return *this;
}