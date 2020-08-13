#pragma once
#include <fstream>
#include <map>
#include <ostream>
#include <string>

namespace engine::log::detail {
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

	template <engine::log::detail::mask level>
	class log_t {
		template <typename T>
		friend inline stream operator<<(log_t const &log_object, T const &t) {
			return log_object.to_stream(t);
		}

		template <typename T>
		stream to_stream(T const &t) const;
	};
}
namespace engine::detail {
	class logger_t;
}

namespace engine::log {
	namespace error {
		inline detail::log_t<detail::mask::error_1> critical;
		inline detail::log_t<detail::mask::error_2> major;
		inline detail::log_t<detail::mask::error_3> minor;
		inline detail::log_t<detail::mask::error_4> negligible;
	}
	namespace warning {
		inline detail::log_t<detail::mask::warning_1> critical;
		inline detail::log_t<detail::mask::warning_2> major;
		inline detail::log_t<detail::mask::warning_3> minor;
		inline detail::log_t<detail::mask::warning_4> negligible;
	}
	namespace message {
		inline detail::log_t<detail::mask::message_1> critical;
		inline detail::log_t<detail::mask::message_2> major;
		inline detail::log_t<detail::mask::message_3> minor;
		inline detail::log_t<detail::mask::message_4> negligible;
	}
	namespace info {
		inline detail::log_t<detail::mask::info_1> critical;
		inline detail::log_t<detail::mask::info_2> major;
		inline detail::log_t<detail::mask::info_3> minor;
		inline detail::log_t<detail::mask::info_4> negligible;
	}
}

namespace engine::log::detail {
	class stream {
		template <engine::log::detail::mask level> friend class log_t;

	public:
		~stream() { finish_writing(); }

	private:
		template <typename T>
		stream(engine::detail::logger_t &logger_ref, mask mask, T const &t) : logger_ref(logger_ref), mask(mask) {
			initialize_writing();
			write(t);
		}
		stream(stream const &) = delete;

		void initialize_writing();
		template<typename T>
		inline void write(T const &t);
		void finish_writing();

		template <typename T>
		friend inline stream &&operator<<(stream &&stream, T const &t) {
			stream.write(t);
			return std::move(stream);
		}

	private:
		engine::detail::logger_t &logger_ref;
		log::detail::mask mask;
	};
}

namespace engine {
	detail::logger_t &logger();
}

namespace engine::detail {
	class logger_t {
		friend engine::log::detail::stream;
		friend logger_t &engine::logger();
	public:
		~logger_t();

		void add_stream(std::ostream &stream, log::detail::mask mask);
		void add_file(std::string const &string, log::detail::mask mask);
		void add_file_wo_timestamp(std::string const &string, log::detail::mask mask);

		inline void enable_exceptions(log::detail::mask mask) { exception_mask = mask; }
		inline void disable_exceptions() { enable_exceptions(log::detail::mask::none); }

		inline void enable_termination(log::detail::mask mask) { termination_mask = mask; }
		inline void disable_termination() { enable_termination(log::detail::mask::none); }


	protected:
		logger_t()
			: exception_mask(log::detail::mask::none),
			termination_mask(log::detail::mask::error_every) {}

	private:
		std::map<std::ofstream *, std::pair<log::detail::mask, bool>> owned_streams;
		std::map<std::ostream *, std::pair<log::detail::mask, bool>> other_streams;

		log::detail::mask exception_mask;
		log::detail::mask termination_mask;
	};

	class logger_exception : public std::exception {
	public:
		using std::exception::exception;
	};
}

namespace engine {
	using logger_mask = log::detail::mask;
}

inline engine::log::detail::mask operator|(engine::log::detail::mask const lhs, engine::log::detail::mask const rhs) {
	return static_cast<engine::log::detail::mask>(static_cast<unsigned>(lhs) | static_cast<unsigned>(rhs));
}
inline engine::log::detail::mask operator&(engine::log::detail::mask const lhs, engine::log::detail::mask const rhs) {
	return static_cast<engine::log::detail::mask>(static_cast<unsigned>(lhs) & static_cast<unsigned>(rhs));
}
inline engine::log::detail::mask operator^(engine::log::detail::mask const lhs, engine::log::detail::mask const rhs) {
	return static_cast<engine::log::detail::mask>(static_cast<unsigned>(lhs) ^ static_cast<unsigned>(rhs));
}
inline engine::log::detail::mask operator~(engine::log::detail::mask const lhs) {
	return static_cast<engine::log::detail::mask>(~static_cast<unsigned>(lhs));
}

template<engine::log::detail::mask level>
template<typename T>
inline engine::log::detail::stream engine::log::detail::log_t<level>::to_stream(T const &t) const {
	return engine::log::detail::stream(engine::logger(), level, t);
}

template<typename T>
inline void engine::log::detail::stream::write(T const &t) {
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