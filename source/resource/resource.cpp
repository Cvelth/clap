#include "resource/resource.hpp"

#include "essential/log.hpp"
using namespace clap::literals;

#include "ryml.hpp"

#include <algorithm>
#include <charconv>
#include <fstream>
#include <iterator>
#include <mutex>
#include <queue>
#include <shared_mutex>
#include <thread>

[[maybe_unused]] static std::string_view to_std(c4::csubstr const &input) { 
	return std::string_view{ input.data(), input.size() }; 
}

namespace clap::resource::detail {
	class configuration_loader {
	protected:
		template <typename T>
		static void load_impl(value<T> &configuration_value, std::string_view name, ryml::NodeRef const &node, 
							  std::filesystem::path const &absolute_filename);
	public:
		static void load(std::string_view name, ryml::NodeRef const &node,
						 std::queue<std::filesystem::path> &left_to_identify,
						 std::filesystem::path const &absolute_filename);
		static void update_instance_extensions(std::span<char const *> extensions) {
			std::ranges::copy(extensions, std::inserter(configuration::instance_extensions.underlying,
														configuration::instance_extensions.underlying.begin()));
			}
	};

	template<>
	inline void configuration_loader::load_impl(value<std::string> &configuration_value,
												std::string_view name, ryml::NodeRef const &node,
												std::filesystem::path const &absolute_filename) {
		if (node.has_val())
			configuration_value.underlying = to_std(node.val());
		else
			clap::log << cL::warning << cL::critical << "clap"_tag << "resource"_tag << "configuration"_tag << "load"_tag
				<< "Ignore '" << name << "' configuration value in '" << absolute_filename << "'."
				<< "String is expected.";
	}
	template<>
	inline void configuration_loader::load_impl(value<uint32_t> &configuration_value,
												std::string_view name, ryml::NodeRef const &node,
												std::filesystem::path const &absolute_filename) {
		if (node.has_val()) {
			uint32_t output;
			auto [ptr, errc] = std::from_chars(node.val().begin(), node.val().end(), output);
			if (errc != std::errc())
				clap::log << cL::warning << cL::critical << "clap"_tag << "resource"_tag << "configuration"_tag << "load"_tag
					<< "Ignore '" << name << "' configuration value in '" << absolute_filename << "'."
					<< "Unsigned integer is expected. '" << to_std(node.val()) << "' was provided instead."
					<< cL::extra << "Conversion to 'uint32_t' using 'std::from_chars(...)' has failed.";
			else
				configuration_value.underlying = output;
		} else
			clap::log << cL::warning << cL::critical << "clap"_tag << "resource"_tag << "configuration"_tag << "load"_tag
				<< "Ignore '" << name << "' configuration value in '" << absolute_filename << "'.";
	}
	template<>
	inline void configuration_loader::load_impl(value<std::set<std::string>> &configuration_value,
												std::string_view name, ryml::NodeRef const &node,
												std::filesystem::path const &absolute_filename) {
		if (node.is_seq())
			if (node.has_children()) {
				typename std::decay<decltype(configuration_value)>::type::underlying_t output;
				for (auto &child : node.children()) {
					if (child.has_val())
						output.emplace(to_std(child.val()));
					else
						clap::log << cL::warning << cL::major << "clap"_tag << "resource"_tag << "configuration"_tag << "load"_tag
							<< "Ignore a value-less list entry in '" << name << "' configuration value in '" 
							<< absolute_filename << "'.";
				}
				configuration_value.underlying = std::move(output);
			} else
				configuration_value.underlying = {};
		else
			clap::log << cL::warning << cL::critical << "clap"_tag << "resource"_tag << "configuration"_tag << "load"_tag
				<< "Ignore '" << name << "' configuration value in '" << absolute_filename << "'."
				<< "List of strings is expected."
				<< cL::extra << "ryml::NodeRef::is_seq()"_fun << "false";
	}
	template<>
	inline void configuration_loader::load_impl(value<std::list<std::filesystem::path>> &configuration_value,
												std::string_view name, ryml::NodeRef const &node,
												std::filesystem::path const &absolute_filename) {
		if (node.is_seq())
			if (node.has_children()) {
				typename std::decay<decltype(configuration_value)>::type::underlying_t output;
				for (auto &child : node.children()) {
					if (child.has_val())
						output.emplace_back(to_std(child.val()));
					else
						clap::log << cL::warning << cL::major << "clap"_tag << "resource"_tag << "configuration"_tag << "load"_tag
							<< "Ignore a value-less list entry in '" << name << "' configuration value in '" 
							<< absolute_filename << "'.";
				}
				configuration_value.underlying = std::move(output);
			} else
				configuration_value.underlying = {};
		else
			clap::log << cL::warning << cL::critical << "clap"_tag << "resource"_tag << "configuration"_tag << "load"_tag
				<< "Ignore '" << name << "' configuration value in '" << absolute_filename << "'."
				<< "List of strings is expected."
				<< cL::extra << "ryml::NodeRef::is_seq()"_fun << "false";
	}

	inline void configuration_loader::load(std::string_view name, ryml::NodeRef const &node,
										   std::queue<std::filesystem::path> &left_to_identify,
										   std::filesystem::path const &absolute_filename) {
		if (name == "application_name")
			load_impl(configuration::application_name, name, node, absolute_filename);
		else if (name == "application_version_major")
			load_impl(configuration::application_version_major, name, node, absolute_filename);
		else if (name == "application_version_minor")
			load_impl(configuration::application_version_minor, name, node, absolute_filename);
		else if (name == "application_version_patch")
			load_impl(configuration::application_version_patch, name, node, absolute_filename);
		else if (name == "instance_extensions")
			load_impl(configuration::instance_extensions, name, node, absolute_filename);
		else if (name == "instance_layers")
			load_impl(configuration::instance_layers, name, node, absolute_filename);
		else if (name == "device_extensions")
			load_impl(configuration::device_extensions, name, node, absolute_filename);
		else if (name == "device_layers")
			load_impl(configuration::device_layers, name, node, absolute_filename);
		else if (name == "additional_resourse_paths") {
			decltype(configuration::additional_resourse_paths) delta({});
			load_impl(delta, name, node, absolute_filename);
			for (auto const &value : delta)
				left_to_identify.push(value);
			std::ranges::move(std::move(delta), 
							  std::back_inserter(configuration::additional_resourse_paths.underlying));
		} else if (name == "additional_configuration_filenames")
			load_impl(configuration::additional_configuration_filenames, name, node, absolute_filename);
		else if (name == "ignored_resource_filenames")
			load_impl(configuration::ignored_resource_filenames, name, node, absolute_filename);
		else
			clap::log << cL::warning << cL::critical << "clap"_tag << "resource"_tag << "configuration"_tag << "load"_tag
			<< "Ignore an unsupported '" << name << "' configuration value in '"
			<< absolute_filename << "'.";
	}
}

void clap::resource::detail::update_instance_extensions(std::span<char const *> extensions) {
	configuration_loader::update_instance_extensions(std::move(extensions));
}

static void on_ryml_error(const char *msg, size_t msg_len, ryml::Location, void *) {
	clap::log << cL::error << cL::important << "clap"_tag << "resource"_tag << "configuration"_tag << "ryml"_tag
		<< "RYML error: '" << std::string_view{ msg, msg_len } << "'\n";
}
inline void initialize_ryml() {
	static bool initialization_status = false;
	if (!initialization_status) {
		ryml::Callbacks callbacks = ryml::get_callbacks();
		callbacks.m_error = &on_ryml_error;
		ryml::set_callbacks(callbacks);
		initialization_status = true;
	}
}
void load_configuration_file(std::filesystem::directory_entry directory_entry,
							 std::queue<std::filesystem::path> &left_to_identify) {
	auto absolute_filename = std::filesystem::absolute(directory_entry.path());
	if (std::ifstream file_stream(absolute_filename); !file_stream)
		clap::log << cL::error << cL::important << "clap"_tag << "resource"_tag << "configuration"_tag << "load"_tag
			<< "Ignore '" << absolute_filename << "'"
			<< cL::extra << "std::ifstream::open(...)"_fun << "false";
	else {
		clap::log << cL::message << cL::minor << "clap"_tag << "resource"_tag << "configuration"_tag << "load"_tag
			<< "Load a configuration file: '" << absolute_filename << "'";
		std::string content{
			std::istreambuf_iterator<std::string::value_type>(file_stream.rdbuf()),
			std::istreambuf_iterator<std::string::value_type>()
		};
		c4::substr string_view(content.data(), content.size());

		initialize_ryml();
		[[maybe_unused]] auto tree = ryml::parse(string_view);
		[[maybe_unused]] auto root = tree.rootref();

		if (root.is_map()){
			for (auto const &node : root.children())
				if (node.has_key())
					clap::resource::detail::configuration_loader::load(
						to_std(node.key()), node, left_to_identify, absolute_filename
					);
				else
					clap::log << cL::error << cL::important << "clap"_tag << "resource"_tag << "configuration"_tag << "load"_tag << "ryml"_tag
						<< "Ignore a key-less '" << absolute_filename << "' entry.";
		} else
			clap::log << cL::error << cL::important << "clap"_tag << "resource"_tag << "configuration"_tag << "load"_tag
				<< "Ignore '" << absolute_filename << "'"
				<< cL::extra << "Configuration file root is expected to be a map." 
				<< cL::minor << "ryml::parse(...).rootref().is_map()"_fun << "false";
	}
}

bool is_ignored(std::filesystem::path const &path) {
	return clap::configuration::default_ignored_resource_filenames->contains(path.filename().string())
		|| clap::configuration::ignored_resource_filenames->contains(path.filename().string());
}
bool is_configuration_file(std::filesystem::path const &path) {
	[[maybe_unused]] auto target = path.filename().string();
	return clap::configuration::default_configuration_filenames->contains(path.filename().string())
		|| clap::configuration::additional_configuration_filenames->contains(path.filename().string());
}
void identify(std::filesystem::directory_entry directory_entry,
			  std::queue<std::filesystem::path> &left_to_identify) {
	if (!is_ignored(directory_entry.path())) {
		if (directory_entry.is_directory())
			clap::log << cL::warning << cL::minor << "clap"_tag << "resource"_tag << "identify"_tag
				<< "limited"_tag << "unfinished"_tag << "to_do"_tag
				<< "Ignore a resource directory: '"
				<< std::filesystem::absolute(directory_entry.path()) << "'.\n"
				<< "Configuration files are the only supported resource type "
					"and they must be put in the root of 'resource' directory.\n"
				<< "More resource type support is to be added in future iterations.";
		else if (is_configuration_file(directory_entry.path()))
			load_configuration_file(directory_entry, left_to_identify);
		else
			clap::log << cL::warning << cL::major << "clap"_tag << "resource"_tag << "identify"_tag
				<< "Ignore a resource: '"
				<< std::filesystem::absolute(directory_entry.path()) << "'.\n"
				<< cL::extra << "Resource identification failed. "
					"The resource is either unsupported or broken.";
	}
}

static std::shared_mutex identification_mutex;
static bool identification_status = false;
void clap::resource::identify() {
	if (!identification_status)
		std::thread(
			[] {
				std::scoped_lock guard(identification_mutex);

				static std::queue<std::filesystem::path> left_to_identify(
					typename std::queue<std::filesystem::path>::container_type(
						configuration::default_resourse_paths->begin(), 
						configuration::default_resourse_paths->end()
					)
				);
				while (!left_to_identify.empty()) {
					[[maybe_unused]] auto target = std::filesystem::absolute(left_to_identify.front());
					if (std::filesystem::exists(left_to_identify.front())) {
						clap::log << cL::message << cL::important << "clap"_tag << "resource"_tag << "identify"_tag
							<< "Identify resources from '" 
							<< std::filesystem::absolute(left_to_identify.front()) << "'.";
						for (auto &subpath : std::filesystem::directory_iterator(left_to_identify.front()))
							::identify(subpath, left_to_identify);
					} else
						clap::log << cL::warning << cL::negligible << "clap"_tag << "resource"_tag << "identify"_tag
							<< "Ignore a resource path: '"
							<< std::filesystem::absolute(left_to_identify.front()) << "'. "
								"It doesn't exist." << cL::extra
							<< "std::filesystem::exists(...)"_fun << "false";
					left_to_identify.pop();
				}

				identification_status = true;
			}
		).detach();
}
void clap::resource::wait() {
	std::shared_lock guard(identification_mutex);
}
bool clap::resource::were_identified() {
	return identification_status;
}