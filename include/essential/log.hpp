#pragma once
#include <fstream>
#include <map>
#include <ostream>
#include <string>

/**
 * @brief Contains implementation details
 * 
 * These are discouraged to be directly used from outside the scope of the project.
*/
namespace clap::log::detail {
	/**
	 * @brief lists all the acceptable mask values to be used when specifying specifics error/warning/message type and level.
	*/
	enum class mask {
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
		info_0 = 0x0000,

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
	class stream;

	/**
	 * @brief Defines operator<<() for predefined error/warning/message levels.
	*/
	template <clap::log::detail::mask level>
	class log_t {
		/**
		 * @brief Passes 't' into a 'log_object' to be processed
		 * @tparam T is the type of 't' object
		 * @param log_object specifies type of error/warning/message occured. 
				It must be one of the objects defined in clap::log namespace. @see clamp::log
		 * @param t is the data to be passed to the logger
		 * @return A 'stream' object to enable additional data to be chained using operator<<()
		*/
		template <typename T>
		friend inline stream operator<<(log_t const &log_object, T const &t) {
			return log_object.to_stream(t);
		}

	protected:
		template <typename T>
		stream to_stream(T const &t) const;
	};
}
namespace clap::detail {
	class logger_t;
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
		inline detail::log_t<detail::mask::error_1> critical;
		/**
		 * @brief Indicates a major error with message passed using operator<<()
		*/
		inline detail::log_t<detail::mask::error_2> major;
		/**
		 * @brief Indicates a minor error with message passed using operator<<()
		*/
		inline detail::log_t<detail::mask::error_3> minor;
		/**
		 * @brief Indicates a negligible error with message passed using operator<<()
		*/
		inline detail::log_t<detail::mask::error_4> negligible;
	}
	/**
	 * @brief Contains object used to pass warnings into logging system.
	*/
	namespace warning {
		/**
		 * @brief Indicates a critical warning with message passed using operator<<()
		*/
		inline detail::log_t<detail::mask::warning_1> critical;
		/**
		 * @brief Indicates a major warning with message passed using operator<<()
		*/
		inline detail::log_t<detail::mask::warning_2> major;
		/**
		 * @brief Indicates a minor warning with message passed using operator<<()
		*/
		inline detail::log_t<detail::mask::warning_3> minor;
		/**
		 * @brief Indicates a negligible warning with message passed using operator<<()
		*/
		inline detail::log_t<detail::mask::warning_4> negligible;
	}
	/**
	 * @brief Contains object used to pass messages into logging system.
	*/
	namespace message {
		/**
		 * @brief Indicates a critical message passed using operator<<()
		*/
		inline detail::log_t<detail::mask::message_1> critical;
		/**
		 * @brief Indicates a major message passed using operator<<()
		*/
		inline detail::log_t<detail::mask::message_2> major;
		/**
		 * @brief Indicates a minor message passed using operator<<()
		*/
		inline detail::log_t<detail::mask::message_3> minor;
		/**
		 * @brief Indicates a negligible message passed using operator<<()
		*/
		inline detail::log_t<detail::mask::message_4> negligible;
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
		inline detail::log_t<detail::mask::info_1> critical;
		/**
		 * @brief Indicates an additional information to be bundled with previous
		 *		error, warning or message.
		 *
		 * It's only logged if both previous error/warning/message was written (its severity level was acceptable)
		 *		and the severity level of info itself is acceptable.
		*/
		inline detail::log_t<detail::mask::info_2> major;
		/**
		 * @brief Indicates an additional information to be bundled with previous
		 *		error, warning or message.
		 *
		 * It's only logged if both previous error/warning/message was written (its severity level was acceptable)
		 *		and the severity level of info itself is acceptable.
		*/
		inline detail::log_t<detail::mask::info_3> minor;
		/**
		 * @brief Indicates an additional information to be bundled with previous
		 *		error, warning or message.
		 *
		 * It's only logged if both previous error/warning/message was written (its severity level was acceptable)
		 *		and the severity level of info itself is acceptable.
		*/
		inline detail::log_t<detail::mask::info_4> negligible;
	}
}

namespace clap::log::detail {
	/**
	 * @brief Defines logger operations
	*/
	class stream {
		template <clap::log::detail::mask level> friend class log_t;

	public:
		~stream() { finish_writing(); }

	private:
		/**
		 * @brief Initializes new error/warnings/message/info processing.
		 * @tparam T is the type of 't' object
		 * @param logger_ref is a reference to the logger used
		 * @param mask is the mask defining type and level of the current error/warning/message.
		 * @param t is the data being processed by the logger
		*/
		template <typename T>
		stream(clap::detail::logger_t &logger_ref, mask mask, T const &t) : logger_ref(logger_ref), mask(mask) {
			initialize_writing();
			write(t);
		}
		stream(stream const &) = delete;

		/**
		 * @brief Writes the header of the error/warning/message.
		 * 
		 * The header includes type, level, time and id of the entity being logged.
		*/
		void initialize_writing();

		/**
		 * @brief Writes the data to the contolled streams
		 * @tparam T is the type of 't' object
		 * @param t is the data being processed by the logger
		*/
		template<typename T>
		inline void write(T const &t);

		/**
		 * @brief Finalizes current error/warning/message
		 * 
		 * Throws clap::detail::logger_exception or calles std::terminate if applicable
		 * @see clap::detail::logger_exception
		 * @see clap::detail::logger_t::enable_exceptions
		 * @see clap::detail::logger_t::enable_termination
		*/
		void finish_writing();

		/**
		 * @brief Writes additional data to currently active output streams
		 * @tparam T is the type of 't' object
		 * @param stream is the reference to the stream object used
		 * @param t is the data being processed by the logger
		 * @return a reference to the stream object used
		*/
		template <typename T>
		friend inline stream &&operator<<(stream &&stream, T const &t) {
			stream.write(t);
			return std::move(stream);
		}

	private:
		/**
		 * @brief A reference to the logger the stream object writes to
		*/
		clap::detail::logger_t &logger_ref; 

		/**
		 * @brief A mask of the error/warnings/message processed by the stream
		*/
		log::detail::mask mask;
	};
}

namespace clap {
	/**
	 * @brief Provides access to a singleton logger class
	 * @return A reference to an object of said class
	*/
	detail::logger_t &logger();
}

namespace clap::detail {
	/**
	 * @brief Allows to specify logging parameters.
	*/
	class logger_t {
		friend clap::log::detail::stream;
		friend logger_t &clap::logger();
	public:
		~logger_t();

		/**
		 * @brief Adds a stream (e.g. std::cout) as an output target for the logger.
		 * @param stream is the stream reference.
		 * @param mask is the mask defining which error/messages are to be sent to the stream.
		 *		It's recommended to only use masks defined in clap::logger_mask. 
		 *		@see clap::logger_mask
		*/
		void add_stream(std::ostream &stream, log::detail::mask mask);

		/**
		 * @brief Adds a text file as an output target for the logger.
		 * 
		 * File is named "{filename} {year}-{month}-{day} {hour}-{minute}-{second}.log" 
		 *		specifying the moment of its creation 
		 *		and is placed in the folder specified by 'path' parameter.
		 * 
		 * @param path specifies the path where the log file is created.
		 * @param mask is the mask defining which error/messages are to be written to this file.
		 *		It's recommended to only use masks defined in clap::logger_mask.
		 *		@see clap::logger_mask
		*/
		void add_file(std::string const &path, log::detail::mask mask);

		/**
		 * @brief Adds a text file as an output target for the logger.
		 *
		 * File is named "{filename} {year}-{month}-{day} {hour}-{minute}-{second}.log"
		 *		specifying the moment of its creation
		 *		and is placed in the folder specified by 'path' parameter.
		 *
		 * @param path specifies the path where the log file is created.
		 * @param mask is the mask defining which error/messages are to be written to this file.
		 *		It's recommended to only use masks defined in clap::logger_mask.
		 *		@see clap::logger_mask
		*/
		void add_file_wo_timestamp(std::string const &path, log::detail::mask mask);

		/**
		 * @brief Enables an exception to be thrown when an error/warning/message with specified severity is logged.
		 * @param mask is the mask defining which error/messages are to be written to this file.
		 *		It's recommended to only use masks defined in clap::logger_mask.
		 *		@see clap::logger_mask
		*/
		inline void enable_exceptions(log::detail::mask mask) { exception_mask = mask; }
		/**
		 * @brief Disables an exception thrown when an error/warning/message with specified severity is logged.
		 * @param mask is the mask defining which error/messages are to be written to this file.
		 *		It's recommended to only use masks defined in clap::logger_mask.
		 *		@see clap::logger_mask
		*/
		inline void disable_exceptions() { enable_exceptions(log::detail::mask::none); }

		/**
		 * @brief Enables std::terminate() to be called when an error/warning/message with specified severity is logged.
		 * @param mask is the mask defining which error/messages are to be written to this file.
		 *		It's recommended to only use masks defined in clap::logger_mask.
		 *		@see clap::logger_mask
		*/
		inline void enable_termination(log::detail::mask mask) { termination_mask = mask; }

		/**
		 * @brief Disables std::terminate() called when an error/warning/message with specified severity is logged.
		 * @param mask is the mask defining which error/messages are to be written to this file.
		 *		It's recommended to only use masks defined in clap::logger_mask.
		 *		@see clap::logger_mask
		*/
		inline void disable_termination() { enable_termination(log::detail::mask::none); }


	protected:
		logger_t()
			: exception_mask(log::detail::mask::none),
			termination_mask(log::detail::mask::error_every) {}

	private:
		/**
		 * @brief Stores pointers to the streams owned by this logger.
		 * 
		 * The destructors of these streams are called by the logger when its lifetime ends.
		*/
		std::map<std::ofstream *, std::pair<log::detail::mask, bool>> owned_streams;

		/**
		 * @brief Stores pointers to the streams **not** owned by this logger.
		 *
		 * The destructors of these streams are **not** called by the logger when its lifetime ends.
		*/
		std::map<std::ostream *, std::pair<log::detail::mask, bool>> other_streams;

		/**
		 * @brief Stores mask specifying errors/warnings/messages that require an exception to be thrown.
		*/
		log::detail::mask exception_mask;

		/**
		 * @brief Stores mask specifying errors/warnings/messages that require a std::terminate() call.
		*/
		log::detail::mask termination_mask;
	};

	/**
	 * @brief Is throws for every error/warning/message requiring an exception to be thrown.
	 * @see clap::detail::logger_t::enable_exceptions()
	 * @see clap::detail::logger_t::diable_exceptions()
	*/
	class logger_exception : public std::exception {
	public:
		using std::exception::exception;
	};
}

namespace clap {
	/**
	 * @brief lists all the acceptable mask values to be used when specifying specifics error/warning/message type and level.
	*/
	using logger_mask = log::detail::mask;
}

inline clap::log::detail::mask operator|(clap::log::detail::mask const lhs, clap::log::detail::mask const rhs) {
	return static_cast<clap::log::detail::mask>(static_cast<unsigned>(lhs) | static_cast<unsigned>(rhs));
}
inline clap::log::detail::mask operator&(clap::log::detail::mask const lhs, clap::log::detail::mask const rhs) {
	return static_cast<clap::log::detail::mask>(static_cast<unsigned>(lhs) & static_cast<unsigned>(rhs));
}
inline clap::log::detail::mask operator^(clap::log::detail::mask const lhs, clap::log::detail::mask const rhs) {
	return static_cast<clap::log::detail::mask>(static_cast<unsigned>(lhs) ^ static_cast<unsigned>(rhs));
}
inline clap::log::detail::mask operator~(clap::log::detail::mask const lhs) {
	return static_cast<clap::log::detail::mask>(~static_cast<unsigned>(lhs));
}

template<clap::log::detail::mask level>
template<typename T>
inline clap::log::detail::stream clap::log::detail::log_t<level>::to_stream(T const &t) const {
	return clap::log::detail::stream(clap::logger(), level, t);
}

template<typename T>
inline void clap::log::detail::stream::write(T const &t) {
	auto lambda = [&t, this](auto &stream_pair) {
		if (stream_pair.first) { // is stream healthy?
			if (static_cast<bool>(mask & mask::info_every)) { // is this an info-entry?
				if (static_cast<bool>(stream_pair.second.first & mask) && stream_pair.second.second) // should the entry be written? 
					*stream_pair.first << t; // write it.
			} else { // it isn't an info entry
				if (static_cast<bool>(stream_pair.second.first & mask)) { // should the entry be written? 
					*stream_pair.first << t; // write it.
					stream_pair.second.second = true; // following info-entries should be written.
				} else
					stream_pair.second.second = false; // following info-entries shouldn't be written.
			}
		} else
			log::warning::critical << "Cannot write an log entry. One of the streams seems to be corrupted.";
	};
	for (auto &stream_pair : logger_ref.owned_streams)
		lambda(stream_pair);
	for (auto &stream_pair : logger_ref.other_streams)
		lambda(stream_pair);
}