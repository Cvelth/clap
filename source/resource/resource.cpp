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
#include <unordered_map>

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

bool is_configuration_file(std::filesystem::path const &path) {
	return clap::configuration::default_configuration_filenames->contains(path.filename().string())
		|| clap::configuration::additional_configuration_filenames->contains(path.filename().string());
}
bool is_ignored(std::filesystem::path const &path) {
	return clap::configuration::default_ignored_resource_filenames->contains(path.filename().string())
		|| clap::configuration::ignored_resource_filenames->contains(path.filename().string());
}

struct identified_resource_container_comparator_t : std::equal_to<> {
	using is_transparent = void;
};
struct identified_resource_container_hash_t {
	using is_transparent = identified_resource_container_comparator_t::is_transparent;
	using transparent_key_equal = identified_resource_container_comparator_t;
	size_t operator()(std::string_view txt) const { return std::hash<std::string_view>{}(txt); }
	size_t operator()(std::string const &txt) const { return std::hash<std::string_view>{}(txt); }
	size_t operator()(char const *txt) const { return std::hash<std::string_view>{}(txt); }
};
using identified_resource_container_t = std::unordered_map<std::string, std::filesystem::directory_entry,
														   identified_resource_container_hash_t,
														   identified_resource_container_comparator_t>;
namespace identified {
	namespace shader {
		static identified_resource_container_t vertex;
		static identified_resource_container_t fragment;
		static identified_resource_container_t tesselation_control;
		static identified_resource_container_t tesselation_evaluation;
		static identified_resource_container_t geometry;
		static identified_resource_container_t compute;
	}
	static identified_resource_container_t unknown;
}
namespace clap::resource {
	namespace shader {
		detail::storage<detail::vertex> vertex;
		detail::storage<detail::fragment> fragment;
		detail::storage<detail::tesselation_control> tesselation_control;
		detail::storage<detail::tesselation_evaluation> tesselation_evaluation;
		detail::storage<detail::geometry> geometry;
		detail::storage<detail::compute> compute;
	}
	detail::storage<detail::unknown> unknown;
}

void add_resource(std::string &&identifier, std::filesystem::directory_entry const &entry,
				  identified_resource_container_t &container) {
	auto &&[iterator, success] = container.try_emplace(std::move(identifier), entry);
	if (!success) 
		clap::log << cL::warning << cL::major << "clap"_tag << "resource"_tag << "shader"_tag << "identify"_tag
			<< "Ignore '" << std::filesystem::absolute(entry) << "'.\n" << cL::extra
			<< "'{resource_map}::try_emplace(...)' has returned 'false'";
}

identified_resource_container_t *filename_to_container(std::filesystem::path filename) {
	if (filename == "vertex")
		return &identified::shader::vertex;
	if (filename == "fragment")
		return &identified::shader::fragment;
	if (filename == "tesscontrol" || filename == "tesselation_control")
		return &identified::shader::tesselation_control;
	if (filename == "tesseval" || filename == "tesselation_evaluation")
		return &identified::shader::tesselation_evaluation;
	if (filename == "geometry")
		return &identified::shader::geometry;
	if (filename == "compute")
		return &identified::shader::compute;
	return nullptr;
}
void identify_shaders(std::filesystem::directory_entry const &resource_location) {
	for (auto &directory : std::filesystem::directory_iterator(resource_location)) {
		auto *container = filename_to_container(directory.path().filename());
		if (directory.is_directory() && container) {
			for (auto &entry : std::filesystem::recursive_directory_iterator(directory))
				if (!entry.is_directory() && !is_ignored(entry))
					add_resource(entry.path().lexically_relative(directory).replace_extension().string(), 
								 entry, *container);
		} else
			clap::log << cL::warning << cL::serious << "clap"_tag << "resource"_tag << "shader"_tag << "identify"_tag
				<< "Ignore '" << std::filesystem::absolute(directory) << "'.\n"
				<< "The name of 'resource/shader/*' directory has to signify the type of shaders it holds."
				<< "\nSupported directory names are:"
				<< "\n    - 'vertex'"
				<< "\n    - 'fragment'"
				<< "\n    - 'tesscontrol' or 'tesselation_control'"
				<< "\n    - 'tesseval' or 'tesselation_evaluation'"
				<< "\n    - 'geometry'"
				<< "\n    - 'compute'";
	}
}
void identify_unknown(std::filesystem::directory_entry const &resource_location) {
	for (auto &entry : std::filesystem::recursive_directory_iterator(resource_location))
		if (!entry.is_directory() && !is_ignored(entry))
			add_resource(entry.path().lexically_relative(resource_location).replace_extension().string(),
						 entry, identified::unknown);
}

void identify(std::filesystem::directory_entry const &directory_entry,
			  std::queue<std::filesystem::path> &left_to_identify) {
	if (!is_ignored(directory_entry.path())) {
		if (directory_entry.is_directory())
			if (directory_entry.path().filename() == "shader")
				identify_shaders(directory_entry);
			else
				identify_unknown(directory_entry);
		else if (is_configuration_file(directory_entry.path()))
			load_configuration_file(directory_entry, left_to_identify);
		else
			clap::log << cL::warning << cL::minor << "clap"_tag << "resource"_tag << "identify"_tag
			<< "Ignore a resource: '"
			<< std::filesystem::absolute(directory_entry.path()) << "'.\n"
			<< "Configuration files are the only resource type supported "
				"in the root of 'resource' directory.";
	}
}

void log(clap::logger::detail::logger_stream_t &logger_stream, std::string_view name, 
		 identified_resource_container_t const &container) {
	if (!container.empty()) {
		logger_stream << cL::important << "    " << name << " (" << container.size() << ")" 
			<< cL::minor << "        ";
		for (auto &entry : container)
			logger_stream << entry.first << "  ";
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

				{
					auto logger_stream = clap::log.add_entry();
					logger_stream << cL::message << cL::important << "clap"_tag << "resource"_tag << "identify"_tag
						<< "Complete resource identification.\n";
					::log(logger_stream, "Vertex Shaders", identified::shader::vertex);
					::log(logger_stream, "Fragment Shaders", identified::shader::fragment);
					::log(logger_stream, "Tesselation Control Shaders", identified::shader::tesselation_control);
					::log(logger_stream, "Tesselation Evaluation Shaders", identified::shader::tesselation_evaluation);
					::log(logger_stream, "Geometry Shaders", identified::shader::geometry);
					::log(logger_stream, "Compute Shaders", identified::shader::compute);
					::log(logger_stream, "Unknown Resources", identified::unknown);
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

template <typename tag_t>
identified_resource_container_t &get_container();
template<> inline identified_resource_container_t &get_container<clap::resource::detail::vertex>() { 
	return identified::shader::vertex; 
}
template<> inline identified_resource_container_t &get_container<clap::resource::detail::fragment>() {
	return identified::shader::fragment;
}
template<> inline identified_resource_container_t &get_container<clap::resource::detail::tesselation_control>() {
	return identified::shader::tesselation_control;
}
template<> inline identified_resource_container_t &get_container<clap::resource::detail::tesselation_evaluation>() {
	return identified::shader::tesselation_evaluation;
}
template<> inline identified_resource_container_t &get_container<clap::resource::detail::geometry>() {
	return identified::shader::geometry;
}
template<> inline identified_resource_container_t &get_container<clap::resource::detail::compute>() {
	return identified::shader::compute;
}
template<> inline identified_resource_container_t &get_container<clap::resource::detail::unknown>() {
	return identified::unknown;
}

template<typename tag_t> std::filesystem::directory_entry *get_impl(std::string_view const &identifier) {
	auto &container = get_container<tag_t>();
	if (auto iterator = container.find(identifier); iterator != container.end())
		return &iterator->second;
	else
		clap::log << cL::warning << cL::serious << "clap"_tag << "resource"_tag << "get"_tag
			<< "Fail to provide a resource: " << identifier << ". Make sure it was identified.";
	return nullptr;
}
template<typename tag_t> std::filesystem::directory_entry *
clap::resource::detail::storage<tag_t>::get(std::string_view const &identifier) {
	std::shared_lock guard(identification_mutex);
	return get_impl<tag_t>(identifier);
}
template<typename tag_t> std::filesystem::directory_entry *
clap::resource::detail::storage<tag_t>::try_get(std::string_view const &identifier) {
	if (identification_mutex.try_lock_shared()) {
		std::shared_lock guard(identification_mutex, std::adopt_lock);
		return get_impl<tag_t>(identifier);
	} else
		clap::log << cL::warning << cL::minor << "clap"_tag << "resource"_tag << "try_get"_tag
			<< "Fail to lock 'storage<...>::try_get(...)' mutex. Resource manager is already in use.";
	return nullptr;
}

namespace clap::resource::detail {
	using namespace std::filesystem;
	template directory_entry *storage<vertex>::get(std::string_view const &identifier);
	template directory_entry *storage<fragment>::get(std::string_view const &identifier);
	template directory_entry *storage<tesselation_control>::get(std::string_view const &identifier);
	template directory_entry *storage<tesselation_evaluation>::get(std::string_view const &identifier);
	template directory_entry *storage<geometry>::get(std::string_view const &identifier);
	template directory_entry *storage<compute>::get(std::string_view const &identifier);
	template directory_entry *storage<unknown>::get(std::string_view const &identifier);
	template directory_entry *storage<vertex>::try_get(std::string_view const &identifier);
	template directory_entry *storage<fragment>::try_get(std::string_view const &identifier);
	template directory_entry *storage<tesselation_control>::try_get(std::string_view const &identifier);
	template directory_entry *storage<tesselation_evaluation>::try_get(std::string_view const &identifier);
	template directory_entry *storage<geometry>::try_get(std::string_view const &identifier);
	template directory_entry *storage<compute>::try_get(std::string_view const &identifier);
	template directory_entry *storage<unknown>::try_get(std::string_view const &identifier);
}