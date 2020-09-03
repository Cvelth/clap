#pragma once
#include <filesystem>
#include <ostream>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace nowide {
	template<typename CharType, typename Traits>
	class basic_ofstream;
	typedef basic_ofstream<char, std::char_traits<char>> ofstream;

	std::string narrow(const std::wstring &s);
	inline std::wstring widen(const std::string &s);

	namespace utf {
		template<typename CharOut, typename CharIn>
		std::basic_string<CharOut> convert_string(const CharIn *begin, const CharIn *end);
	}
}

/**
 * @brief Contains implementation details
 *
 * Members of this namespaces are discouraged to be directly mentioned outside of the implementation of the logger.
*/
namespace clap::log::detail {

	/**
	 * @brief lists all the acceptable severity levels available for log entries.
	*/
	enum class severity : unsigned {
		none = 0x0000,

		error_4 = 0x1000,
		error_3 = 0x2000,
		error_2 = 0x4000,
		error_1 = 0x8000,
		error_0 = 0x0000,

		warning_4 = 0x0100,
		warning_3 = 0x0200,
		warning_2 = 0x0400,
		warning_1 = 0x0800,
		warning_0 = 0x0000,

		message_4 = 0x0010,
		message_3 = 0x0020,
		message_2 = 0x0040,
		message_1 = 0x0080,
		message_0 = 0x0000,

		info_4 = 0x0001,
		info_3 = 0x0002,
		info_2 = 0x0004,
		info_1 = 0x0008,
		info_0 = 0x0000
	};

	/**
	 * @brief defines several `severity` level combinations to be used when specifying severity masks
	*/
	enum class severity_mask : unsigned {
		none = (int) severity::none,

		error_4 = (int) severity::error_4,
		error_3 = (int) severity::error_3,
		error_2 = (int) severity::error_2,
		error_1 = (int) severity::error_1,
		error_0 = (int) severity::error_0,

		warning_4 = (int) severity::warning_4,
		warning_3 = (int) severity::warning_3,
		warning_2 = (int) severity::warning_2,
		warning_1 = (int) severity::warning_1,
		warning_0 = (int) severity::warning_0,

		message_4 = (int) severity::message_4,
		message_3 = (int) severity::message_3,
		message_2 = (int) severity::message_2,
		message_1 = (int) severity::message_1,
		message_0 = (int) severity::message_0,

		info_4 = (int) severity::info_4,
		info_3 = (int) severity::info_3,
		info_2 = (int) severity::info_2,
		info_1 = (int) severity::info_1,
		info_0 = (int) severity::info_0,

		error_1_4 = error_1 | error_2 | error_3 | error_4,
		error_1_3 = error_1 | error_2 | error_3,
		error_1_2 = error_1 | error_2,

		error_critical = error_1,
		error_major = error_1_2,
		error_minor = error_1_3,
		error_every = error_1_4,
		error_none = error_0,

		warning_1_4 = warning_1 | warning_2 | warning_3 | warning_4,
		warning_1_3 = warning_1 | warning_2 | warning_3,
		warning_1_2 = warning_1 | warning_2,

		warning_critical = warning_1,
		warning_major = warning_1_2,
		warning_minor = warning_1_3,
		warning_every = warning_1_4,
		warning_none = warning_0,

		message_1_4 = message_1 | message_2 | message_3 | message_4,
		message_1_3 = message_1 | message_2 | message_3,
		message_1_2 = message_1 | message_2,

		message_critical = message_1,
		message_major = message_1_2,
		message_minor = message_1_3,
		message_every = message_1_4,
		message_none = message_0,

		info_1_4 = info_1 | info_2 | info_3 | info_4,
		info_1_3 = info_1 | info_2 | info_3,
		info_1_2 = info_1 | info_2,

		info_critical = info_1,
		info_major = info_1_2,
		info_minor = info_1_3,
		info_every = info_1_4,
		info_none = info_0,

		every = error_every | warning_every | message_every | info_every
	};

	template <clap::log::detail::severity severity> class log_t;
	class logger_state_t;
	class stream;
}
template <typename rhs_t>
clap::log::detail::stream &&operator<<(clap::log::detail::stream &&stream, rhs_t const &rhs);

namespace clap {
	/**
	 * @brief Provides access to a singleton `logger_state_t` class
	 * @return A reference to an object of said class
	 * @see clap::log::detail::logger_state_t
	*/
	log::detail::logger_state_t &logger();
}

namespace clap::log::detail {
	/**
	 * @brief Defines operator<< for log entry definition.
	*/
	class stream {
		template <clap::log::detail::severity level> friend class log_t;

	public:
		~stream() { finish_entry(); }

	private:
		/**
		 * @brief Initializes new log entry.
		 * @param logger_state is the state of the logger.
		 * @param severity describes type and level of given log entry.
		*/
		stream(logger_state_t &logger_state, severity severity) : logger_state_ref(logger_state), severity(severity) {
			initialize_entry();
		}

		/**
		 * @brief Copy constructor is deleted.
		*/
		stream(stream const &) = delete;

		/**
		 * @brief Writes the header of the entry.
		 *
		 * The header includes type, level, time and id of the entity being logged.
		*/
		void initialize_entry();

		/**
		 * @brief Finalizes current entry
		 *
		 * Throws clap::detail::logger_exception or calles std::terminate if applicable
		 * @see clap::detail::logger_exception
		 * @see clap::detail::logger_state_t::enable_exceptions
		 * @see clap::detail::logger_state_t::enable_termination
		*/
		void finish_entry();

		/**
		 * @brief Writes passed object (`rhs`) to currently active output streams
		 * @tparam rhs_t is the type of `rhs` object
		 * @param stream is the reference to the stream object used
		 * @param rhs is the object to be written into active output streams
		 * @return a reference to the stream object used
		*/
		template <typename rhs_t>
		friend stream &&::operator<<(stream &&stream, rhs_t const &rhs);

	private:
		/**
		 * @brief A reference to the logger the stream object writes to
		*/
		logger_state_t &logger_state_ref;

		/**
		 * @brief A severity level of the entry processed by the stream
		*/
		log::detail::severity severity;
	};

	/**
	 * @brief Base class used to define standard log-objects.
	*/
	template <clap::log::detail::severity severity>
	class log_t {

	public:
		/**
		 * @brief Implisit conversion from 'log_t' to 'stream'.
		*/
		operator stream() const {
			return stream(logger(), severity);
		}
	};
}

/**
 * @brief Contains objects used to pass messages/warning/errors to logging system.
 *
 * **Usage**: `clap::{type}::{level} << "Your message\n";`
*/
namespace clap::log {
	/**
	 * @brief Contains object used to pass errors into logging system.
	*/
	namespace error {
		/**
		 * @brief Indicates a critical error with message passed using operator<<()
		*/
		inline detail::log_t<detail::severity::error_1> critical;
		/**
		 * @brief Indicates a major error with message passed using operator<<()
		*/
		inline detail::log_t<detail::severity::error_2> major;
		/**
		 * @brief Indicates a minor error with message passed using operator<<()
		*/
		inline detail::log_t<detail::severity::error_3> minor;
		/**
		 * @brief Indicates a negligible error with message passed using operator<<()
		*/
		inline detail::log_t<detail::severity::error_4> negligible;
	}
	/**
	 * @brief Contains object used to pass warnings into logging system.
	*/
	namespace warning {
		/**
		 * @brief Indicates a critical warning with message passed using operator<<()
		*/
		inline detail::log_t<detail::severity::warning_1> critical;
		/**
		 * @brief Indicates a major warning with message passed using operator<<()
		*/
		inline detail::log_t<detail::severity::warning_2> major;
		/**
		 * @brief Indicates a minor warning with message passed using operator<<()
		*/
		inline detail::log_t<detail::severity::warning_3> minor;
		/**
		 * @brief Indicates a negligible warning with message passed using operator<<()
		*/
		inline detail::log_t<detail::severity::warning_4> negligible;
	}
	/**
	 * @brief Contains object used to pass messages into logging system.
	*/
	namespace message {
		/**
		 * @brief Indicates a critical message passed using operator<<()
		*/
		inline detail::log_t<detail::severity::message_1> critical;
		/**
		 * @brief Indicates a major message passed using operator<<()
		*/
		inline detail::log_t<detail::severity::message_2> major;
		/**
		 * @brief Indicates a minor message passed using operator<<()
		*/
		inline detail::log_t<detail::severity::message_3> minor;
		/**
		 * @brief Indicates a negligible message passed using operator<<()
		*/
		inline detail::log_t<detail::severity::message_4> negligible;
	}
	/**
	 * @brief Contains object used to pass extra information into logging system.
	*/
	namespace info {
		/**
		 * @brief Indicates an additional information to be bundled with previous
		 *		error, warning or message.
		 *
		 * It's only logged if both previous error/warning/message was written (its severity level was acceptable)
		 *		and the severity level of info itself is acceptable.
		*/
		inline detail::log_t<detail::severity::info_1> critical;
		/**
		 * @brief Indicates an additional information to be bundled with previous
		 *		error, warning or message.
		 *
		 * It's only logged if both previous error/warning/message was written (its severity level was acceptable)
		 *		and the severity level of info itself is acceptable.
		*/
		inline detail::log_t<detail::severity::info_2> major;
		/**
		 * @brief Indicates an additional information to be bundled with previous
		 *		error, warning or message.
		 *
		 * It's only logged if both previous error/warning/message was written (its severity level was acceptable)
		 *		and the severity level of info itself is acceptable.
		*/
		inline detail::log_t<detail::severity::info_3> minor;
		/**
		 * @brief Indicates an additional information to be bundled with previous
		 *		error, warning or message.
		 *
		 * It's only logged if both previous error/warning/message was written (its severity level was acceptable)
		 *		and the severity level of info itself is acceptable.
		*/
		inline detail::log_t<detail::severity::info_4> negligible;
	}
}

namespace clap::log::detail {
	template <typename interface_t, typename actual_t, 
		typename destructor_t = std::default_delete<typename actual_t>>
	class basic_polymorphic_wrapper {
	public:
		template <typename ...Ts>
		basic_polymorphic_wrapper(Ts const &...ts) : pointer(new actual_t{ts...}) {}
		basic_polymorphic_wrapper() : pointer(new actual_t{}) {}
		~basic_polymorphic_wrapper() { if (pointer) destructor_t{}((actual_t *) pointer); }

		basic_polymorphic_wrapper(basic_polymorphic_wrapper const &) = delete;
		basic_polymorphic_wrapper(basic_polymorphic_wrapper &&other) noexcept 
			: pointer(other.pointer) { other.pointer = nullptr; }

		operator interface_t *() { return pointer; }
		operator interface_t &() { return *pointer; }

		interface_t *operator->() { return pointer; };
		interface_t *operator->() const { return pointer; };
		interface_t &operator*() { return *pointer; };
		interface_t &operator*() const { return *pointer; };
		actual_t &operator&() { return *(actual_t *) pointer; };
		actual_t &operator&() const { return *(actual_t *) pointer; };
	private:
		interface_t *pointer;
	};

	struct nowide_ofstream_destructor_callable {
		void operator()(nowide::ofstream *ptr);
	};
	using file_wrapper = basic_polymorphic_wrapper<
		std::ostream, nowide::ofstream, 
		nowide_ofstream_destructor_callable
	>;

	/**
	 * @brief A single class able to wrap all the supported underlying stream types.
	*/
	class stream_wrapper {
		friend class logger_state_t;
		template <typename rhs_t> friend stream &&::operator<<(stream &&stream, rhs_t const &rhs);
	public:
		stream_wrapper(std::ostream &stream, log::detail::severity_mask mask)
			: stream(&stream), mask(mask), write_next_info(false) {}
		stream_wrapper(std::wostream &stream, log::detail::severity_mask mask)
			: stream(&stream), mask(mask), write_next_info(false) {}
		stream_wrapper(file_wrapper &&stream, log::detail::severity_mask mask)
			: stream(std::move(stream)), mask(mask), write_next_info(false) {}
		~stream_wrapper() = default;

		stream_wrapper(stream_wrapper const &) = delete;
		stream_wrapper(stream_wrapper &&other) noexcept
			: stream(std::move(other.stream)), mask(other.mask), 
			write_next_info(other.write_next_info) {}

		/**
		 * @brief Default operator<<().
		 * @tparam rhs_t type of passed object
		 * @param rhs passed object
		 * @return `*this`
		*/
		template<typename rhs_t>
		stream_wrapper &operator<<(rhs_t const &rhs);

		inline stream_wrapper &operator<<(char8_t const *rhs) {
			return *this << (char *) rhs;
		}
		inline stream_wrapper &operator<<(wchar_t const *rhs) {
			return *this << nowide::narrow(rhs);
		}
		inline stream_wrapper &operator<<(char32_t const rhs) {
			return *this << nowide::utf::convert_string<char8_t>(&rhs, &rhs + 1);
		}
		template <typename Elem, typename Traits>
		inline stream_wrapper &operator<<(std::basic_string<Elem, Traits> const &rhs) {
			return *this << rhs.c_str();
		}
		template <typename Elem, typename Traits>
		inline stream_wrapper &operator<<(std::basic_string_view<Elem, Traits> const &rhs) {
			return *this << rhs.data();
		}

		inline stream_wrapper &operator<<(std::filesystem::path const &path) {
			return *this << path.u8string();
		}

		/**
		 * @brief Returns 'operator bool()' of an underlying stream.
		*/
		operator bool() const;

		/**
		 * @brief Returns a reference to an unrelying stream.
		 * @return
		*/
		auto &operator*() { return stream; };

	protected:
		std::variant<
			std::ostream *,
			std::wostream *,
			file_wrapper
		> stream;

		log::detail::severity_mask mask;
		mutable bool write_next_info;
	};

	/**
	 * @brief Allows to change logger state.
	*/
	class logger_state_t {
		friend clap::log::detail::stream;
		friend logger_state_t &clap::logger();
		template <typename rhs_t> friend stream &&::operator<<(stream &&stream, rhs_t const &rhs);
	public:
		~logger_state_t();

		/**
		 * @brief Adds a stream (e.g. `std::cout`) as an output target for the logger.
		 * @param stream is the stream reference.
		 * @param mask is the mask defining which entries are to be sent to the stream.
		 *		It's recommended to only use masks defined in clap::logger_mask.
		 *		@see clap::logger_mask
		*/
		void add_stream(std::ostream &stream, log::detail::severity_mask mask);

		/**
		 * @brief Adds a **wide** stream (e.g. `std::wcout`) as an output target for the logger.
		 * @param stream is the stream reference.
		 * @param mask is the mask defining which entries are to be sent to the stream.
		 *		It's recommended to only use masks defined in clap::logger_mask.
		 *		@see clap::logger_mask
		*/
		void add_stream(std::wostream &stream, log::detail::severity_mask mask);

		/**
		 * @brief Adds a text file as an output target for the logger.
		 *
		 * File is named "{path} {year}-{month}-{day} {hour}-{minute}-{second}.log"
		 *		specifying the moment of its creation
		 *
		 * @param path specifies the path where the log file is created.
		 * @param mask is the mask defining which entries are to be written to this file.
		 *		It's recommended to only use masks defined in clap::logger_mask.
		 *		@see clap::logger_mask
		*/
		void add_file(std::filesystem::path const &path, log::detail::severity_mask mask);

		/**
		 * @brief Adds a text file as an output target for the logger.
		 *
		 * File is named "{filename}.log"
		 *
		 * @param path specifies the path where the log file is created.
		 * @param mask is the mask defining which error/messages are to be written to this file.
		 *		It's recommended to only use masks defined in clap::logger_mask.
		 *		@see clap::logger_mask
		*/
		void add_file_wo_timestamp(std::filesystem::path const &path, log::detail::severity_mask mask);

		/**
		 * @brief Enables an exception to be thrown when a new entry with specified severity is logged.
		 * @param mask is the mask defining which entries are to be logged.
		 *		It's recommended to only use masks defined in clap::logger_mask.
		 *		@see clap::logger_mask
		 *
		 * It's disabled by default.
		*/
		inline void enable_exceptions(log::detail::severity_mask mask) { exception_mask = mask; }
		/**
		 * @brief Disables an exception thrown when a new entry with specified severity is logged.
		 *
		 * It's disabled by default.
		*/
		inline void disable_exceptions() { enable_exceptions(log::detail::severity_mask::none); }

		/**
		 * @brief Enables std::terminate() to be called when a new entry with specified severity is logged.
		 * @param mask is the mask defining which entries are to be logged.
		 *		It's recommended to only use masks defined in clap::logger_mask.
		 *		@see clap::logger_mask
		 *
		 * It's enabled for errors by default.
		*/
		inline void enable_termination(log::detail::severity_mask mask) { termination_mask = mask; }

		/**
		 * @brief Disables std::terminate() called when a new entry with specified severity is logged.
		 *
		 * It's enabled for errors by default.
		*/
		inline void disable_termination() { enable_termination(log::detail::severity_mask::none); }

		/**
		 * @brief Causes standard output streams (e.g. 'std::cout') passed to 'add_stream' to be substituted with nowide alternatives (e.g. 'nowide::cout').
		 * @param value is the new value of the option.
		 * @see logger_state_t::add_stream
		 *
		 * It's enabled by default.
		*/
		inline void enable_nowide_substitution(bool value = true) { replace_std_with_nowide = value; }

		/**
		 * @brief Disables substitution of standard output streams (e.g. 'std::cout') passed to 'add_stream' with 'nowide' alternatives (e.g. 'nowide::cout').
		 * @param value is the new value of the option.
		 * @see logger_state_t::add_stream
		 *
		 * It's enabled by default.
		*/
		inline void disable_nowide_substitution() { enable_nowide_substitution(false); }


	protected:
		logger_state_t()
			: exception_mask(log::detail::severity_mask::none),
			termination_mask(log::detail::severity_mask::error_every),
			replace_std_with_nowide(true) {}

	private:

		/**
		 * @brief Stores pointers to the streams used by this logger.
		 */
		std::vector<stream_wrapper> used_streams;

		/**
		 * @brief Stores mask specifying errors/warnings/messages that require an exception to be thrown.
		 *
		 * It's disabled by default.
		*/
		log::detail::severity_mask exception_mask;

		/**
		 * @brief Stores mask specifying errors/warnings/messages that require a std::terminate() call.
		 *
		 * It's enabled for errors by default.
		*/
		log::detail::severity_mask termination_mask;

		/**
		 * @brief Determines whether standard output (e.g. 'std::cout') should be replaced with nowide provided alternative (e.g. 'nowide::cout').
		 *
		 * It's enabled by default.
		*/
		bool replace_std_with_nowide;
	};

	/**
	 * @brief Is throws for every error/warning/message requiring an exception to be thrown.
	 * @see clap::detail::logger_state_t::enable_exceptions()
	 * @see clap::detail::logger_state_t::diable_exceptions()
	*/
	class logger_exception : public std::exception {
	public:
		using std::exception::exception;
	};
}

namespace clap {
	/**
	 * @brief lists all the acceptable mask values to be used when specifying an entry type and level.
	*/
	using logger_mask = log::detail::severity_mask;
}

inline clap::log::detail::severity_mask operator|(clap::log::detail::severity_mask const lhs, clap::log::detail::severity_mask const rhs) {
	return static_cast<clap::log::detail::severity_mask>(static_cast<unsigned>(lhs) | static_cast<unsigned>(rhs));
}
inline clap::log::detail::severity_mask operator&(clap::log::detail::severity_mask const lhs, clap::log::detail::severity_mask const rhs) {
	return static_cast<clap::log::detail::severity_mask>(static_cast<unsigned>(lhs) & static_cast<unsigned>(rhs));
}
inline clap::log::detail::severity_mask operator^(clap::log::detail::severity_mask const lhs, clap::log::detail::severity_mask const rhs) {
	return static_cast<clap::log::detail::severity_mask>(static_cast<unsigned>(lhs) ^ static_cast<unsigned>(rhs));
}
inline clap::log::detail::severity_mask operator~(clap::log::detail::severity_mask const lhs) {
	return static_cast<clap::log::detail::severity_mask>(~static_cast<unsigned>(lhs));
}

inline clap::log::detail::severity_mask operator|(clap::log::detail::severity const lhs, clap::log::detail::severity const rhs) {
	return clap::log::detail::severity_mask(lhs) | clap::log::detail::severity_mask(rhs);
}
inline clap::log::detail::severity_mask operator&(clap::log::detail::severity const lhs, clap::log::detail::severity const rhs) {
	return clap::log::detail::severity_mask(lhs) & clap::log::detail::severity_mask(rhs);
}
inline clap::log::detail::severity_mask operator^(clap::log::detail::severity const lhs, clap::log::detail::severity const rhs) {
	return clap::log::detail::severity_mask(lhs) ^ clap::log::detail::severity_mask(rhs);
}
inline clap::log::detail::severity_mask operator~(clap::log::detail::severity const lhs) {
	return ~clap::log::detail::severity_mask(lhs);
}

inline clap::log::detail::severity_mask operator|(clap::log::detail::severity_mask const lhs, clap::log::detail::severity const rhs) {
	return lhs | clap::log::detail::severity_mask(rhs);
}
inline clap::log::detail::severity_mask operator|(clap::log::detail::severity const lhs, clap::log::detail::severity_mask const rhs) {
	return clap::log::detail::severity_mask(lhs) | rhs;
}
inline clap::log::detail::severity_mask operator&(clap::log::detail::severity_mask const lhs, clap::log::detail::severity const rhs) {
	return lhs & clap::log::detail::severity_mask(rhs);
}
inline clap::log::detail::severity_mask operator&(clap::log::detail::severity const lhs, clap::log::detail::severity_mask const rhs) {
	return clap::log::detail::severity_mask(lhs) & rhs;
}
inline clap::log::detail::severity_mask operator^(clap::log::detail::severity_mask const lhs, clap::log::detail::severity const rhs) {
	return lhs ^ clap::log::detail::severity_mask(rhs);
}
inline clap::log::detail::severity_mask operator^(clap::log::detail::severity const lhs, clap::log::detail::severity_mask const rhs) {
	return clap::log::detail::severity_mask(lhs) ^ rhs;
}

template<typename rhs_t>
clap::log::detail::stream &&operator<<(clap::log::detail::stream &&stream, rhs_t const &rhs) {
	for (auto &wrapper : stream.logger_state_ref.used_streams) {
		if (wrapper) { // is wrapped stream healthy?
			if (static_cast<bool>(stream.severity & clap::log::detail::severity_mask::info_every)) { // is this an info-entry?
				if (static_cast<bool>(wrapper.mask & stream.severity) && wrapper.write_next_info) // should the entry be written? 
					wrapper << rhs; // write it.
			} else { // it isn't an info entry
				if (static_cast<bool>(wrapper.mask & stream.severity)) { // should the entry be written? 
					wrapper << rhs; // write it.
					wrapper.write_next_info = true; // following info-entries should be written.
				} else
					wrapper.write_next_info = false; // following info-entries shouldn't be written.
			}
		} else {
			//clap::log::warning::critical << "Cannot write an log entry. One of the streams seems to be corrupted.";
		}
	}
	return std::move(stream);
}

namespace clap::log::detail {
	template<typename...Fs> struct overload : Fs... { using Fs::operator()...; };
	template<typename...Fs> overload(Fs...)->overload<Fs...>;
}

template<typename rhs_t>
inline clap::log::detail::stream_wrapper &clap::log::detail::stream_wrapper::operator<<(rhs_t const &rhs) {
	std::visit(
		overload{
			[&rhs](std::ostream *stream) {
				*stream << rhs;
			},
			[&rhs](std::wostream *stream) {
				// Temporary solution.
				std::ostringstream temp; temp << rhs;
				*stream << nowide::widen(temp.str());
			},
			[&rhs](file_wrapper const &stream) {
				*stream << rhs;
			}
		}, stream);
	return *this;
}