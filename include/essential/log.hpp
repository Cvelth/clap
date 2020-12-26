#pragma once
#include <filesystem>
#include <ostream>
#include <sstream>
#include <variant>
#include <vector>

namespace clap {
	namespace logger {
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

				template <detail::severity_level_t level>
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
									   detail::severity_t<severity_level> const &) const {
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
			class entry_type_t {
				friend entry_t;
			public:
				entry_type_t(entry::type value = entry::type::message,
							 std::string const &name = "message")
					: value(value), name(name) {}
				std::string const &operator*() const { return name; }
				std::string const *operator->() const { return &name; }
				operator entry::type() const { return value; }
			private:
				entry::type value;
				std::string name;
			};
			class extra_t {};

			class normal_t {};
			class special_t {
				friend entry_t;
			public:
				special_t(std::string &&value) : value(std::move(value)) {}
			private:
				std::string value;
			};
			class tag_t : public special_t { public: using special_t::special_t; };
			class variable_t : public special_t { public: using special_t::special_t; };
			class function_t : public special_t { public: using special_t::special_t; };
		}
		namespace mask {
			constexpr inline detail::severity_mask_t no_errors
				= entry::type::error * entry::severity::ignored;
			constexpr inline detail::severity_mask_t every_error
				= entry::type::error * entry::severity::minimum;
			constexpr inline detail::severity_mask_t negligible_errors
				= entry::type::error * entry::severity::negligible;
			constexpr inline detail::severity_mask_t insignificant_errors
				= entry::type::error * entry::severity::insignificant;
			constexpr inline detail::severity_mask_t minor_errors
				= entry::type::error * entry::severity::minor;
			constexpr inline detail::severity_mask_t major_errors
				= entry::type::error * entry::severity::major;
			constexpr inline detail::severity_mask_t serious_errors
				= entry::type::error * entry::severity::serious;
			constexpr inline detail::severity_mask_t important_errors
				= entry::type::error * entry::severity::important;
			constexpr inline detail::severity_mask_t critical_errors
				= entry::type::error * entry::severity::critical;

			constexpr inline detail::severity_mask_t no_warnings
				= entry::type::warning * entry::severity::ignored;
			constexpr inline detail::severity_mask_t every_warning
				= entry::type::warning * entry::severity::minimum;
			constexpr inline detail::severity_mask_t negligible_warnings
				= entry::type::warning * entry::severity::negligible;
			constexpr inline detail::severity_mask_t insignificant_warnings
				= entry::type::warning * entry::severity::insignificant;
			constexpr inline detail::severity_mask_t minor_warnings
				= entry::type::warning * entry::severity::minor;
			constexpr inline detail::severity_mask_t major_warnings
				= entry::type::warning * entry::severity::major;
			constexpr inline detail::severity_mask_t serious_warnings
				= entry::type::warning * entry::severity::serious;
			constexpr inline detail::severity_mask_t important_warnings
				= entry::type::warning * entry::severity::important;
			constexpr inline detail::severity_mask_t critical_warnings
				= entry::type::warning * entry::severity::critical;

			constexpr inline detail::severity_mask_t no_messages
				= entry::type::message * entry::severity::ignored;
			constexpr inline detail::severity_mask_t every_message
				= entry::type::message * entry::severity::minimum;
			constexpr inline detail::severity_mask_t negligible_messages
				= entry::type::message * entry::severity::negligible;
			constexpr inline detail::severity_mask_t insignificant_messages
				= entry::type::message * entry::severity::insignificant;
			constexpr inline detail::severity_mask_t minor_messages
				= entry::type::message * entry::severity::minor;
			constexpr inline detail::severity_mask_t major_messages
				= entry::type::message * entry::severity::major;
			constexpr inline detail::severity_mask_t serious_messages
				= entry::type::message * entry::severity::serious;
			constexpr inline detail::severity_mask_t important_messages
				= entry::type::message * entry::severity::important;
			constexpr inline detail::severity_mask_t critical_messages
				= entry::type::message * entry::severity::critical;

			constexpr inline detail::severity_mask_t no_custom_entries
				= entry::type::custom * entry::severity::ignored;
			constexpr inline detail::severity_mask_t every_custom_entry
				= entry::type::custom * entry::severity::minimum;
			constexpr inline detail::severity_mask_t negligible_custom_entries
				= entry::type::custom * entry::severity::negligible;
			constexpr inline detail::severity_mask_t insignificant_custom_entries
				= entry::type::custom * entry::severity::insignificant;
			constexpr inline detail::severity_mask_t minor_custom_entries
				= entry::type::custom * entry::severity::minor;
			constexpr inline detail::severity_mask_t major_custom_entries
				= entry::type::custom * entry::severity::major;
			constexpr inline detail::severity_mask_t serious_custom_entries
				= entry::type::custom * entry::severity::serious;
			constexpr inline detail::severity_mask_t important_custom_entries
				= entry::type::custom * entry::severity::important;
			constexpr inline detail::severity_mask_t critical_custom_entries
				= entry::type::custom * entry::severity::critical;

			constexpr inline detail::severity_mask_t nothing
				= no_errors | no_warnings | no_messages | no_custom_entries;
			constexpr inline detail::severity_mask_t everything
				= every_error | every_warning | every_message | every_custom_entry;
		}

		namespace literals {
			inline detail::tag_t operator""_tag(char const *value, size_t size) {
				return std::string(value, size);
			};
			inline detail::variable_t operator""_var(char const *value, size_t size) {
				return std::string(value, size);
			};
			inline detail::function_t operator""_fun(char const *value, size_t size) {
				return std::string(value, size);
			};
		}
		namespace manipulators {
			using namespace entry::severity;

			inline detail::entry_type_t const error(entry::type::error, "error");
			inline detail::entry_type_t const warning(entry::type::warning, "warning");
			inline detail::entry_type_t const message(entry::type::message, "message");

			class custom : public detail::entry_type_t {
			public:
				custom(std::string const &name) : entry_type_t(entry::type::custom, name) {}
			};

			constexpr inline detail::extra_t extra;
		}

		namespace detail {
			class stream;
			class stream_preferences {
			public:
				stream_preferences(stream *ptr) : stream_ptr(ptr) {}
				stream_preferences &with_tags(bool enable = true);
				stream_preferences &with_auto_flush(bool enable = true);

				stream_preferences(stream_preferences const &) = delete;
				stream_preferences(stream_preferences &&) = delete;
				stream_preferences operator=(stream_preferences const &) = delete;
				stream_preferences operator=(stream_preferences &&) = delete;
			private:
				stream *stream_ptr;
			};
		}

		detail::stream_preferences add_stream(std::ostream &stream,
											  detail::severity_mask_t mask,
											  detail::severity_mask_t extra_mask);
		detail::stream_preferences add_file(std::filesystem::path const &path,
											detail::severity_mask_t mask,
											detail::severity_mask_t extra_mask);
		detail::stream_preferences add_file_wo_timestamp(std::filesystem::path const &path,
														 detail::severity_mask_t mask,
														 detail::severity_mask_t extra_mask);

		inline detail::stream_preferences add(std::ostream &stream,
											  detail::severity_mask_t mask,
											  detail::severity_mask_t extra_mask) {
			return add_stream(stream, mask, extra_mask);
		}
		inline detail::stream_preferences add(std::filesystem::path const &path,
											  detail::severity_mask_t mask,
											  detail::severity_mask_t extra_mask) {
			return add_file(path, mask, extra_mask);
		}
		inline detail::stream_preferences add_stream(std::ostream &stream,
													 detail::severity_mask_t mask) {
			return add_stream(stream, mask, mask);
		}
		inline detail::stream_preferences add_file(std::filesystem::path const &path,
												   detail::severity_mask_t mask) {
			return add_file(path, mask, mask);
		}
		inline detail::stream_preferences add_file_wo_timestamp(std::filesystem::path const &path,
																detail::severity_mask_t mask) {
			return add_file_wo_timestamp(path, mask, mask);
		}
		inline detail::stream_preferences add(std::ostream &stream,
											  detail::severity_mask_t mask) {
			return add_stream(stream, mask, mask);
		}
		inline detail::stream_preferences add(std::filesystem::path const &path,
											  detail::severity_mask_t mask) {
			return add_file(path, mask, mask);
		}

		void start();
		void stop();
		bool is_logging();

		namespace detail {
			class logger_stream_t;
			class entry_t {
				friend logger_stream_t;
			public:
				~entry_t();

				struct chunk_t {
					std::string value;
					severity_level_t severity;
					bool is_extra;
					bool is_special;

					chunk_t(severity_level_t level, bool is_extra = false, bool is_special = false)
						: severity(level), is_extra(is_extra), is_special(is_special) {}
					template <severity_level_t level> chunk_t(severity_t<level>, bool is_extra)
						: severity(level), is_extra(is_extra) {}
					chunk_t &operator=(std::string const &new_value) { value = new_value; return *this; }
					chunk_t &operator=(std::string &&new_value) { value = std::move(new_value); return *this; }
					chunk_t &operator+=(std::string const &new_value) { value += new_value; return *this; }
					chunk_t &operator+=(std::string &&new_value) { value += std::move(new_value); return *this; }
					chunk_t &operator=(std::string_view const &new_value) { value = new_value; return *this; }
					chunk_t &operator+=(std::string_view const &new_value) { value += new_value; return *this; }
					operator std::string() const { return value; }
					char const *c_str() const { return value.c_str(); }
				};
			protected:
				template <typename value_t> void append(value_t &&value);
				template <severity_level_t level> void append(severity_t<level> const &value);
			private:
				entry_t() : is_in_use(false), is_in_extra_mode(false), type(manipulators::message)
					, severity(manipulators::minor.value), mode(detail::normal_t{}) {}
			private:
				std::vector<chunk_t> chunks;
				std::vector<std::string> tags;

				bool is_in_use, is_in_extra_mode;
				entry_type_t type;
				severity_level_t severity;
				std::variant<detail::normal_t, detail::function_t, detail::variable_t> mode;
			};

			class logger_t;
			class logger_stream_t {
				friend logger_t;
			public:
				logger_stream_t() : underlying(is_logging() ? new entry_t() : nullptr) {}

				template <typename value_t>
				friend inline logger_stream_t operator<<(logger_stream_t stream, value_t &&value) {
					stream.append(std::forward<value_t>(value));
					return stream;
				}
				auto *operator->() { return underlying.get(); }
				auto const *operator->() const { return underlying.get(); }
			protected:
				template <typename value_t> inline void append(value_t &&value) {
					if (underlying) underlying->append(std::forward<value_t>(value));
				}
				template <severity_level_t level> void append(severity_t<level> const &value) {
					if (underlying) underlying->append(value);
				}
			private:
				std::shared_ptr<entry_t> underlying;
			};
			class logger_t {
			public:
				constexpr logger_t() = default;
				template <typename value_t>
				friend inline logger_stream_t operator<<(logger_t const &logger, value_t const &value) {
					return (logger.add_entry() << value);
				}
				logger_stream_t add_entry() const { return logger_stream_t{}; }
			private:
				logger_t(logger_t const &) = delete;
				logger_t(logger_t &&) = delete;
				logger_t &operator=(logger_t const &) = delete;
				logger_t &operator=(logger_t &&) = delete;
			};

			namespace concepts {
				template <typename T1, typename T2>
				concept same_as = std::is_same<T1, T2>::value && std::is_same<T2, T1>::value;
				template<typename T>
				concept convertible_to_string = requires (T const &t) {
					{ std::to_string(t) } -> same_as<std::string>;
				};
				template<typename LHS, typename RHS>
				concept printable = requires (LHS & lhs, RHS const &rhs) {
					{ lhs << rhs } -> same_as<LHS &>;
				};
			}
			template<class> struct always_false : std::false_type {};
		}
	}
	constexpr inline logger::detail::logger_t log;
}
namespace clap::logger::literals {
	namespace cL = clap::logger::manipulators;
}
namespace clap {
	namespace logger_literals = logger::literals;
	namespace literals = logger_literals;
}

template<clap::logger::detail::severity_level_t level>
inline void clap::logger::detail::entry_t::append(severity_t<level> const &) { severity = level; }
template<> inline void clap::logger::detail::entry_t::append(extra_t const &) { is_in_extra_mode = true; }
template<> inline void clap::logger::detail::entry_t::append(tag_t const &tag) { tags.emplace_back(tag.value); }
template<> inline void clap::logger::detail::entry_t::append(tag_t &&tag) { tags.emplace_back(std::move(tag.value)); }
template<> inline void clap::logger::detail::entry_t::append(function_t const &func) { mode = func; }
template<> inline void clap::logger::detail::entry_t::append(function_t &&func) { mode = std::move(func); }
template<> inline void clap::logger::detail::entry_t::append(variable_t const &var) { mode = var; }
template<> inline void clap::logger::detail::entry_t::append(variable_t &&var) { mode = std::move(var); }
template<> inline void clap::logger::detail::entry_t::append(std::filesystem::path const &path) { append(path.string()); }
template<> inline void clap::logger::detail::entry_t::append(std::filesystem::path &path) { append(path.string()); }
template<> inline void clap::logger::detail::entry_t::append(std::filesystem::path &&path) { append(path.string()); }

template<> inline void clap::logger::detail::entry_t::append(entry_type_t &&value) {
	using namespace clap::literals;
	if (is_in_use) {
		clap::log << cL::warning << cL::critical
			<< "clap"_tag << "essential"_tag << "log"_tag << "stream"_tag
			<< "Ignore an attempt to change log entry type after the logging has already begun."
			<< cL::extra
			<< "Entry type can only be changed before the stream is written to.";
	} else
		type = std::move(value);
}
template<> inline void clap::logger::detail::entry_t::append(entry_type_t const &value) {
	using namespace clap::literals;
	if (is_in_use) {
		clap::log << cL::warning << cL::critical
			<< "clap"_tag << "essential"_tag << "log"_tag << "stream"_tag
			<< "Ignore an attempt to change log entry type after the logging has already begun."
			<< cL::extra
			<< "Entry type can only be changed before the stream is written to.";
	} else
		type = value;
}
template<> inline void clap::logger::detail::entry_t::append(manipulators::custom &&value) {
	append(static_cast<entry_type_t &&>(std::move(value)));
}
template<> inline void clap::logger::detail::entry_t::append(manipulators::custom const &value) {
	append(static_cast<entry_type_t const &>(value));
}

template<typename value_t>
inline void clap::logger::detail::entry_t::append(value_t &&value) {
	while (!chunks.empty() && chunks.back().value == "")
		chunks.pop_back();
	std::visit(
		[this, &value](auto logger_mode) {
			using mode_t = std::decay_t<decltype(logger_mode)>;
			if constexpr (std::is_same<normal_t, mode_t>::value) {
				is_in_use = true;

				std::string output;
				if constexpr (std::is_constructible<std::string_view, decltype(std::forward<value_t>(value))>::value)
					output += std::string_view(std::forward<value_t>(value));
				else if constexpr (std::is_constructible<std::string, decltype(std::forward<value_t>(value))>::value)
					output += std::string(std::forward<value_t>(value));
				else if constexpr (concepts::convertible_to_string<decltype(std::forward<value_t>(value))>)
					output += std::to_string(std::forward<value_t>(value));
				else if constexpr (concepts::printable<std::ostringstream, decltype(std::forward<value_t>(value))>) {
					static std::ostringstream helper_stream;
					helper_stream.str(std::string());
					helper_stream.clear();
					helper_stream << std::forward<value_t>(value);
					output += helper_stream.str();
				} else
					static_assert(always_false<value_t>::value,
								  "Fail to log an object. "
								  "Make sure that either "
								  "'[typename]::operator std::string_view() const' or "
								  "'[typename]::operator std::string() const' or "
								  "'std::to_string([typename])' or "
								  "'operator<<(std::ostream&, [typename] const &)' is defined.");

				if (!output.empty()) {
					if (chunks.empty() || chunks.back().severity != severity
						|| chunks.back().is_extra != is_in_extra_mode || chunks.back().is_special)
						chunks.emplace_back(severity, is_in_extra_mode);
					chunks.back() += std::move(output);
				}
			} else {
				mode = normal_t{};
				if (!chunks.empty())
					chunks.back().is_special = true;
				if constexpr (std::is_same<function_t, mode_t>::value) {
					append("A call to '" + logger_mode.value + "' has returned '");
				} else if constexpr (std::is_same<variable_t, mode_t>::value) {
					mode = normal_t{};
					append("The value of '" + logger_mode.value + "' is '");
				} else
					static_assert(always_false<mode_t>::value,
								  "Fail to log an object. "
								  "The mode of the logging stream is not supported.");
				append(std::forward<value_t>(value));
				append("'");
				if (!chunks.empty())
					chunks.back().is_special = true;
			}
		}
	, std::move(mode));
}