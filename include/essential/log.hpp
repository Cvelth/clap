#pragma once
#include <filesystem>
#include <ostream>
#include <sstream>
#include <variant>
#include <vector>

namespace clap {
	namespace logger {
		class exception : public std::runtime_error {
		public:
			inline explicit exception() : std::runtime_error("A log entry has caused an exception to be thrown.") {}
		};
		namespace detail {
			using severity_level_t = uint8_t;
			template <severity_level_t level>
			struct severity_t {
				static constexpr severity_level_t value = level;
			};
		}
		namespace entry {
			enum class type : uint32_t {
				message = 0x0,
				warning = 0x8,
				error = 0x10,
				custom = 0x18
			};
			namespace severity {
				constexpr inline detail::severity_t<0x00> ignored;
				constexpr inline detail::severity_t<0x01> minimum;
				constexpr inline detail::severity_t<0x20> negligible;
				constexpr inline detail::severity_t<0x40> insignificant;
				constexpr inline detail::severity_t<0x60> minor;
				constexpr inline detail::severity_t<0x80> major;
				constexpr inline detail::severity_t<0xa0> serious;
				constexpr inline detail::severity_t<0xc0> important;
				constexpr inline detail::severity_t<0xe0> critical;
				constexpr inline detail::severity_t<0xff> maximum;

				template <detail::severity_level_t level >
				constexpr inline detail::severity_t<level> severity;
			}
		}
		namespace detail {
			template <severity_level_t severity_level>
			constexpr inline uint32_t operator<<(severity_t<severity_level>, entry::type rhs) {
				return uint32_t(severity_level) << uint32_t(rhs);
			}

			class severity_mask_t {
			public:
				template <severity_level_t severity_level>
				constexpr severity_mask_t(entry::type const &type,
										  detail::severity_t<severity_level> const &severity)
					: value(severity << type) {}
				constexpr inline severity_mask_t operator|=(severity_mask_t const &another) { return value |= another.value; }
				constexpr inline severity_mask_t operator&=(severity_mask_t const &another) { return value &= another.value; }
				constexpr inline severity_mask_t operator^=(severity_mask_t const &another) { return value ^= another.value; }
				constexpr inline severity_mask_t operator|(severity_mask_t const &another) const { return value | another.value; }
				constexpr inline severity_mask_t operator&(severity_mask_t const &another) const { return value & another.value; }
				constexpr inline severity_mask_t operator^(severity_mask_t const &another) const { return value ^ another.value; }
				constexpr inline severity_mask_t operator==(severity_mask_t const &another) const { return value == another.value; }
				constexpr inline severity_mask_t operator!=(severity_mask_t const &another) const { return value != another.value; }

				inline bool operator()(entry::type const &type, severity_level_t const &severity) const {
					auto masked = (value & (entry::severity::maximum << type)) >> uint32_t(type);
					return masked && masked <= severity;
				}
				template <severity_level_t severity_level>
				inline bool operator()(entry::type const &type,
									   detail::severity_t<severity_level> const &severity) const {
					return operator()(type, severity_level);
				}
			protected:
				constexpr severity_mask_t(uint32_t value) : value(value) {}
			private:
				uint32_t value;
			};
			template <severity_level_t severity_level>
			constexpr inline severity_mask_t operator*(entry::type const &type, detail::severity_t<severity_level> const &severity) {
				return severity_mask_t(type, severity);
			}

			class entry_t;
			template <typename value_t> struct entry_mode_visitor_t;
			class entry_type_t {
				friend entry_t;
			public:
				entry_type_t(entry::type value = entry::type::message,
							 std::u8string const &name = u8"message")
					: value(value), name(name) {}
				std::u8string const &operator*() const { return name; }
				std::u8string const *operator->() const { return &name; }
				operator entry::type() const { return value; }
			private:
				entry::type value;
				std::u8string name;
			};
			class extra_t {};

			class normal_t {};
			class special_t {
				friend entry_t;
				template <typename value_t> friend struct entry_mode_visitor_t;
			public:
				special_t(std::u8string &&value) : value(std::move(value)) {}
			private:
				std::u8string value;
			};
			class tag_t : public special_t { public: using special_t::special_t; };
			class variable_t : public special_t { public: using special_t::special_t; };
			class function_t : public special_t { public: using special_t::special_t; };
		}
		namespace mask {
			constexpr inline detail::severity_mask_t no_errors = entry::type::error * entry::severity::ignored;
			constexpr inline detail::severity_mask_t all_errors = entry::type::error * entry::severity::minimum;
			constexpr inline detail::severity_mask_t minimum_errors = entry::type::error * entry::severity::minimum;
			constexpr inline detail::severity_mask_t negligible_errors = entry::type::error * entry::severity::negligible;
			constexpr inline detail::severity_mask_t insignificant_errors = entry::type::error * entry::severity::insignificant;
			constexpr inline detail::severity_mask_t minor_errors = entry::type::error * entry::severity::minor;
			constexpr inline detail::severity_mask_t major_errors = entry::type::error * entry::severity::major;
			constexpr inline detail::severity_mask_t serious_errors = entry::type::error * entry::severity::serious;
			constexpr inline detail::severity_mask_t important_errors = entry::type::error * entry::severity::important;
			constexpr inline detail::severity_mask_t critical_errors = entry::type::error * entry::severity::critical;

			constexpr inline detail::severity_mask_t no_warnings = entry::type::warning * entry::severity::ignored;
			constexpr inline detail::severity_mask_t all_warnings = entry::type::warning * entry::severity::minimum;
			constexpr inline detail::severity_mask_t negligible_warnings = entry::type::warning * entry::severity::negligible;
			constexpr inline detail::severity_mask_t insignificant_warnings = entry::type::warning * entry::severity::insignificant;
			constexpr inline detail::severity_mask_t minor_warnings = entry::type::warning * entry::severity::minor;
			constexpr inline detail::severity_mask_t major_warnings = entry::type::warning * entry::severity::major;
			constexpr inline detail::severity_mask_t serious_warnings = entry::type::warning * entry::severity::serious;
			constexpr inline detail::severity_mask_t important_warnings = entry::type::warning * entry::severity::important;
			constexpr inline detail::severity_mask_t critical_warnings = entry::type::warning * entry::severity::critical;

			constexpr inline detail::severity_mask_t no_messages = entry::type::message * entry::severity::ignored;
			constexpr inline detail::severity_mask_t all_messages = entry::type::message * entry::severity::minimum;
			constexpr inline detail::severity_mask_t negligible_messages = entry::type::message * entry::severity::negligible;
			constexpr inline detail::severity_mask_t insignificant_messages = entry::type::message * entry::severity::insignificant;
			constexpr inline detail::severity_mask_t minor_messages = entry::type::message * entry::severity::minor;
			constexpr inline detail::severity_mask_t major_messages = entry::type::message * entry::severity::major;
			constexpr inline detail::severity_mask_t serious_messages = entry::type::message * entry::severity::serious;
			constexpr inline detail::severity_mask_t important_messages = entry::type::message * entry::severity::important;
			constexpr inline detail::severity_mask_t critical_messages = entry::type::message * entry::severity::critical;

			constexpr inline detail::severity_mask_t no_custom_entries = entry::type::custom * entry::severity::ignored;
			constexpr inline detail::severity_mask_t all_custom_entries = entry::type::custom * entry::severity::minimum;
			constexpr inline detail::severity_mask_t negligible_custom_entries = entry::type::custom * entry::severity::negligible;
			constexpr inline detail::severity_mask_t insignificant_custom_entries = entry::type::custom * entry::severity::insignificant;
			constexpr inline detail::severity_mask_t minor_custom_entries = entry::type::custom * entry::severity::minor;
			constexpr inline detail::severity_mask_t major_custom_entries = entry::type::custom * entry::severity::major;
			constexpr inline detail::severity_mask_t serious_custom_entries = entry::type::custom * entry::severity::serious;
			constexpr inline detail::severity_mask_t important_custom_entries = entry::type::custom * entry::severity::important;
			constexpr inline detail::severity_mask_t critical_custom_entries = entry::type::custom * entry::severity::critical;

			constexpr inline detail::severity_mask_t nothing = no_errors | no_warnings | no_messages | no_custom_entries;
			constexpr inline detail::severity_mask_t everything = all_errors | all_warnings | all_messages | all_custom_entries;
		}

		namespace literals {
			inline detail::tag_t operator""_tag(char const *value, size_t size) { return std::u8string((char8_t const *) value, size); };
			inline detail::variable_t operator""_var(char const *value, size_t size) { return std::u8string((char8_t const *) value, size); };
			inline detail::function_t operator""_fun(char const *value, size_t size) { return std::u8string((char8_t const *) value, size); };
			//inline detail::tag_t operator""_tag(char8_t const *value, size_t) { return value; };
			//inline detail::value_t operator""_var(char8_t const *value, size_t) { return value; };
			//inline detail::variable_t operator""_var(char8_t const *value, size_t) { return value; };
			//inline detail::function_t operator""_fun(char8_t const *value, size_t) { return value; };
		}
		namespace manipulators {
			using namespace entry::severity;

			inline detail::entry_type_t const error(entry::type::error, u8"error");
			inline detail::entry_type_t const warning(entry::type::warning, u8"warning");
			inline detail::entry_type_t const message(entry::type::message, u8"message");

			class custom : public detail::entry_type_t {
			public:
				custom(std::u8string const &name) : entry_type_t(entry::type::custom, name) {}
			};

			constexpr inline detail::extra_t extra;
		}
		namespace detail {
			class universal_stream;
			class universal_stream_preferences {
			public:
				universal_stream_preferences(universal_stream *ptr) : stream_ptr(ptr) {}
				universal_stream_preferences &with_tags(bool enable = true);
				universal_stream_preferences &with_auto_flush(bool enable = true);

				universal_stream_preferences(universal_stream_preferences const &) = delete;
				universal_stream_preferences(universal_stream_preferences &&) = delete;
				universal_stream_preferences operator=(universal_stream_preferences const &) = delete;
				universal_stream_preferences operator=(universal_stream_preferences &&) = delete;
			private:
				universal_stream *stream_ptr;
			};
		}

		detail::universal_stream_preferences add_stream(std::ostream &stream, detail::severity_mask_t mask, detail::severity_mask_t extra);
		detail::universal_stream_preferences add_stream(std::wostream &stream, detail::severity_mask_t mask, detail::severity_mask_t extra);
		detail::universal_stream_preferences add_file(std::filesystem::path const &path, detail::severity_mask_t mask, detail::severity_mask_t extra);
		detail::universal_stream_preferences add_file_wo_timestamp(std::filesystem::path const &path, detail::severity_mask_t mask, detail::severity_mask_t extra);

		inline detail::universal_stream_preferences add_stream(std::ostream &stream, detail::severity_mask_t mask) { return add_stream(stream, mask, mask); }
		inline detail::universal_stream_preferences add_stream(std::wostream &stream, detail::severity_mask_t mask) { return add_stream(stream, mask, mask); }
		inline detail::universal_stream_preferences add_file(std::filesystem::path const &path, detail::severity_mask_t mask) { return add_file(path, mask, mask); }
		inline detail::universal_stream_preferences add_file_wo_timestamp(std::filesystem::path const &path, detail::severity_mask_t mask) { return add_file_wo_timestamp(path, mask, mask); }

		void enable_exceptions(detail::severity_mask_t mask);
		void enable_termination(detail::severity_mask_t mask);
		void enable_nowide_substitution(bool value = true);

		inline void disable_exceptions() { enable_exceptions(mask::nothing); }
		inline void disable_termination() { enable_termination(mask::nothing); }
		inline void disable_nowide_substitution() { enable_nowide_substitution(false); }

		void start();
		void stop();
		bool is_logging();

		namespace detail {
			class logger_t;
			class entry_stream_t;
			class entry_t {
				friend entry_stream_t;
			protected:
				template <typename value_t> void append(value_t &&value);
				template <severity_level_t level> void append(severity_t<level> const &value);
			private:
				entry_t()
					: is_in_use(false), is_extra(false)
					, current_type(manipulators::message)
					, current_severity(manipulators::minor.value)
					, current_extra_severity(manipulators::important.value)
					, current_mode(detail::normal_t{}) {}
				~entry_t();
				entry_t(entry_t const &) = delete;
				entry_t(entry_t &&) = delete;
				entry_t &operator=(entry_t const &) = delete;
				entry_t &operator=(entry_t &&) = delete;
			public:
				struct chunk_t {
					std::u8string value;
					severity_level_t severity;
					bool is_extra;
					bool is_special;

					chunk_t(severity_level_t level, bool is_extra = false, bool is_special = false)
						: severity(level), is_extra(is_extra), is_special(is_special) {}
					template <severity_level_t level> chunk_t(severity_t<level> severity, bool is_extra)
						: severity(level), is_extra(is_extra) {}
					chunk_t &operator=(std::u8string const &new_value) { value = new_value; return *this; }
					chunk_t &operator=(std::u8string &&new_value) { value = std::move(new_value); return *this; }
					chunk_t &operator+=(std::u8string const &new_value) { value += new_value; return *this; }
					chunk_t &operator+=(std::u8string &&new_value) { value += std::move(new_value); return *this; }
					operator std::u8string() const { return value; }
				};
			private:
				std::basic_ostringstream<char8_t> underlying;
				std::vector<chunk_t> chunks;
				std::vector<std::u8string> tags;

				bool is_in_use, is_extra;
				entry_type_t current_type;
				severity_level_t current_severity;
				severity_level_t current_extra_severity;
				std::variant<detail::normal_t, detail::function_t, detail::variable_t>
					current_mode;
			};
			class entry_stream_t {
				friend logger_t;
				struct entry_deleter_t { inline void operator()(entry_t *ptr) { delete ptr; } };
			public:
				entry_stream_t()
					: underlying(is_logging() ? new entry_t() : nullptr, entry_deleter_t{}) {}

				template <typename value_t>
				friend inline entry_stream_t operator<<(entry_stream_t stream, value_t &&value) {
					stream.append(std::forward<value_t>(value));
					return std::move(stream);
				}
				auto *operator->() { return underlying.get(); }
				auto const *operator->() const { return underlying.get(); }
			protected:
				template <typename value_t> inline void append(value_t &&value) {
					if (underlying)
						underlying->append(std::forward<value_t>(value));
				}
				template <severity_level_t level> void append(severity_t<level> const &value) {
					if (underlying)
						underlying->append(value);
				}
			private:
				std::shared_ptr<entry_t> underlying;
			};
			class logger_t {
			public:
				constexpr logger_t() = default;
				template <typename value_t>
				friend inline entry_stream_t operator<<(logger_t const &logger, value_t const &value) {
					return (logger.add_entry() << value);
				}
				entry_stream_t add_entry() const { return entry_stream_t{}; }
			private:
				logger_t(logger_t const &) = delete;
				logger_t(logger_t &&) = delete;
				logger_t &operator=(logger_t const &) = delete;
				logger_t &operator=(logger_t &&) = delete;
			};

			namespace concepts {
				template<typename LHS, typename RHS>
				concept Printable = requires (LHS & lhs, RHS const &rhs) {
					{ lhs << rhs } -> std::same_as<LHS &>;
				};
				template<typename T>
				concept Convertible_To_String = requires (T const &t) {
					{ std::to_string(t) } -> std::same_as<std::string>;
				};
				template<typename T>
				concept Convertible_To_WString = requires (T const &t) {
					{ std::to_wstring(t) } -> std::same_as<std::wstring>;
				};
			}
			template<class> struct always_false { static inline constexpr bool value = false; };
		}
	}
	constexpr inline logger::detail::logger_t log;

	namespace logger_literals = logger::literals;
	namespace literals = logger_literals;
}
namespace cL = clap::logger::manipulators;
using namespace clap::literals;

template<clap::logger::detail::severity_level_t level>
inline void clap::logger::detail::entry_t::append(severity_t<level> const &) { current_severity = level; }
template<> inline void clap::logger::detail::entry_t::append(extra_t const &) { is_extra = true; }
template<> inline void clap::logger::detail::entry_t::append(tag_t const &tag) { tags.emplace_back(tag.value); }
template<> inline void clap::logger::detail::entry_t::append(tag_t &&tag) { tags.emplace_back(std::move(tag.value)); }
template<> inline void clap::logger::detail::entry_t::append(function_t const &in) { current_mode = in; }
template<> inline void clap::logger::detail::entry_t::append(function_t &&in) { current_mode = std::move(in); }
template<> inline void clap::logger::detail::entry_t::append(variable_t const &in) { current_mode = in; }
template<> inline void clap::logger::detail::entry_t::append(variable_t &&in) { current_mode = std::move(in); }
template<> inline void clap::logger::detail::entry_t::append(std::filesystem::path const &path) { append(path.u8string()); }
template<> inline void clap::logger::detail::entry_t::append(std::filesystem::path &path) { append(path.u8string()); }
template<> inline void clap::logger::detail::entry_t::append(std::filesystem::path &&path) { append(path.u8string()); }

template<> inline void clap::logger::detail::entry_t::append(entry_type_t &&value) {
	if (is_in_use) {
		clap::log << cL::warning << cL::critical
			<< "clap"_tag << "essential"_tag << "log"_tag << "stream"_tag
			<< "Ignore an attempt to change the type of a log entry after the logging has already begun."
			<< cL::extra
			<< "Entry type can only be changed before something is written to the stream.";
	} else
		current_type = std::move(value);
}
template<> inline void clap::logger::detail::entry_t::append(entry_type_t const &value) {
	if (is_in_use) {
		clap::log << cL::warning << cL::critical
			<< "clap"_tag << "essential"_tag << "log"_tag << "stream"_tag
			<< "Ignore an attempt to change the type of a log entry after the logging has already begun."
			<< cL::extra
			<< "Entry type can only be changed before something is written to the stream.";
	} else
		current_type = value;
}
template<> inline void clap::logger::detail::entry_t::append(manipulators::custom &&value) { append((entry_type_t &&) std::move(value)); }
template<> inline void clap::logger::detail::entry_t::append(manipulators::custom const &value) { append((entry_type_t const &) value); }

template<typename value_t>
inline void clap::logger::detail::entry_t::append(value_t &&value) {
	while (!chunks.empty() && chunks.back().value == u8"")
		chunks.pop_back();
	std::visit(
		[this, &value](auto mode) {
			using mode_t = std::decay_t<decltype(mode)>;
			if constexpr (std::is_same<normal_t, mode_t>::value) {
				is_in_use = true;

				underlying.str(u8"");
				if constexpr (concepts::Printable<decltype(underlying), decltype(std::forward<value_t>(value))>)
					underlying << std::forward<value_t>(value) << '\n';
				else if constexpr (std::is_constructible<std::u8string, decltype(std::forward<value_t>(value))>::value)
					underlying << std::u8string(std::forward<value_t>(value));
				else if constexpr (std::is_constructible<std::string, decltype(std::forward<value_t>(value))>::value)
					underlying << (char8_t const *) std::string(std::forward<value_t>(value)).c_str();
				else if constexpr (concepts::Convertible_To_String<decltype(std::forward<value_t>(value))>)
					underlying << (char8_t const *) std::to_string(std::forward<value_t>(value)).c_str();
				else
					static_assert(!always_false<value_t>::value,
								  "Fail to log an object. Make sure operator<<(std::ostream&, [object_t] const &) is defined.");

				if (underlying.str() != u8"") {
					if (chunks.empty() || chunks.back().severity != current_severity
						|| chunks.back().is_extra != is_extra || chunks.back().is_special)
						chunks.emplace_back(current_severity, is_extra);
					chunks.back() += std::move(underlying.str());
				}
			} else {
				current_mode = normal_t{};
				if (!chunks.empty())
					chunks.back().is_special = true;

				if constexpr (std::is_same<function_t, mode_t>::value)
					append(u8"A call to '" + mode.value + u8"' has returned '");
				else if constexpr (std::is_same<variable_t, mode_t>::value)
					append(u8"The value of '" + mode.value + u8"' is '");
				else
					static_assert(!always_false<mode_t>::value,
								  "Fail to log an object. The mode of the logging stream is not supported.");

				append(std::forward<value_t>(value));
				append(u8"'");
				if (!chunks.empty())
					chunks.back().is_special = true;
			}
		}
	, std::move(current_mode));
}