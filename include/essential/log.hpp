#pragma once
#include <filesystem>
#include <fstream>
#include <map>
#include <string>

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
		 * @brief Adds a stream (e.g. std::cout) as an output target for the logger.
		 * @param stream is the stream reference.
		 * @param mask is the mask defining which entries are to be sent to the stream.
		 *		It's recommended to only use masks defined in clap::logger_mask.
		 *		@see clap::logger_mask
		*/
		void add_stream(std::ostream &stream, log::detail::severity_mask mask);

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
		 * @brief Enables an exception to be thrown when an error/warning/message with specified severity is logged.
		 * @param mask is the mask defining which error/messages are to be written to this file.
		 *		It's recommended to only use masks defined in clap::logger_mask.
		 *		@see clap::logger_mask
		*/
		inline void enable_exceptions(log::detail::severity_mask mask) { exception_mask = mask; }
		/**
		 * @brief Disables an exception thrown when an error/warning/message with specified severity is logged.
		 * @param mask is the mask defining which error/messages are to be written to this file.
		 *		It's recommended to only use masks defined in clap::logger_mask.
		 *		@see clap::logger_mask
		*/
		inline void disable_exceptions() { enable_exceptions(log::detail::severity_mask::none); }

		/**
		 * @brief Enables std::terminate() to be called when a new entry with specified severity is logged.
		 * @param mask is the mask defining which entries are to be written to this file.
		 *		It's recommended to only use masks defined in clap::logger_mask.
		 *		@see clap::logger_mask
		*/
		inline void enable_termination(log::detail::severity_mask mask) { termination_mask = mask; }

		/**
		 * @brief Disables std::terminate() called when a new entry with specified severity is logged.
		 * @param mask is the mask defining which entries are to be written to this file.
		 *		It's recommended to only use masks defined in clap::logger_mask.
		 *		@see clap::logger_mask
		*/
		inline void disable_termination() { enable_termination(log::detail::severity_mask::none); }


	protected:
		logger_state_t()
			: exception_mask(log::detail::severity_mask::none),
			termination_mask(log::detail::severity_mask::error_every) {}

	private:
		/**
		 * @brief Stores pointers to the streams owned by this logger.
		 *
		 * The destructors of these streams are called by the logger when its lifetime ends.
		*/
		std::map<std::ofstream *, std::pair<log::detail::severity_mask, bool>> owned_streams;

		/**
		 * @brief Stores pointers to the streams **not** owned by this logger.
		 *
		 * The destructors of these streams are **not** called by the logger when its lifetime ends.
		*/
		std::map<std::ostream *, std::pair<log::detail::severity_mask, bool>> other_streams;

		/**
		 * @brief Stores mask specifying errors/warnings/messages that require an exception to be thrown.
		*/
		log::detail::severity_mask exception_mask;

		/**
		 * @brief Stores mask specifying errors/warnings/messages that require a std::terminate() call.
		*/
		log::detail::severity_mask termination_mask;
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
	auto lambda = [&stream, &rhs](auto &stream_pair) {
		if (stream_pair.first) { // is stream healthy?
			if (static_cast<bool>(stream.severity & clap::log::detail::severity_mask::info_every)) { // is this an info-entry?
				if (static_cast<bool>(stream_pair.second.first & stream.severity) && stream_pair.second.second) // should the entry be written? 
					*stream_pair.first << rhs; // write it.
			} else { // it isn't an info entry
				if (static_cast<bool>(stream_pair.second.first & stream.severity)) { // should the entry be written? 
					*stream_pair.first << rhs; // write it.
					stream_pair.second.second = true; // following info-entries should be written.
				} else
					stream_pair.second.second = false; // following info-entries shouldn't be written.
			}
		} else
			clap::log::warning::critical << "Cannot write an log entry. One of the streams seems to be corrupted.";
	};

	for (auto &stream_pair : stream.logger_state_ref.owned_streams)
		lambda(stream_pair);
	for (auto &stream_pair : stream.logger_state_ref.other_streams)
		lambda(stream_pair);

	return std::move(stream);
}