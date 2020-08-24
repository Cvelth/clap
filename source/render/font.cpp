#include "render/font.hpp"
#include "render/detail/state.hpp"

#include "essential/log.hpp"

#include FT_OUTLINE_H
#include "msdfgen/msdfgen.h"

#include <limits>
#include <algorithm>

static msdfgen::Point2 point(const FT_Vector *vector) {
	return msdfgen::Point2{
		double(vector->x) / 64.,
		double(vector->y) / 64.
	};
}
static unsigned char to_byte(float input) {
	return (unsigned char) std::clamp(input * std::numeric_limits<unsigned char>::max(),
									  0.f,
									  float(std::numeric_limits<unsigned char>::max())
	);
};

msdfgen::Shape to_shape(FT_Face &face, size_t char_id) {
	FT_Error error = FT_Load_Char(face, FT_ULong(char_id), FT_LOAD_NO_SCALE);
	if (error) {
		clap::log::error::major << "Unable to read a character.";
		clap::log::info::critical << "Character id: 0x" << std::hex << char_id << ".";
	}

	struct current_t {
		msdfgen::Shape shape;
		msdfgen::Contour *contour = nullptr;
		msdfgen::Point2 position;
		const size_t char_id;

		current_t(size_t char_id) : char_id(char_id) {}
	};
	static auto move_to = [](const FT_Vector *to, void *_current) {
		auto *current = reinterpret_cast<current_t *>(_current);
		if (!current->contour || !current->contour->edges.empty())
			current->contour = &current->shape.addContour();
		current->position = point(to);
		return 0;
	};
	static auto line_to = [](const FT_Vector *to, void *_current) {
		auto *current = reinterpret_cast<current_t *>(_current);
		auto endpoint = point(to);
		if (endpoint != current->position) {
			current->contour->addEdge(new msdfgen::LinearSegment(current->position, endpoint));
			current->position = endpoint;
		}
		return 0;
	};
	static auto conic_to = [](const FT_Vector *control, const FT_Vector *to, void *_current) {
		auto *current = reinterpret_cast<current_t *>(_current);
		if (!current->contour) {
			clap::log::error::major << "Character glyph is broken.";
			clap::log::info::critical << "Character id: 0x" << std::hex << current->char_id << ".";
			return 1;
		}
		current->contour->addEdge(new msdfgen::QuadraticSegment(current->position, point(control), point(to)));
		current->position = point(to);
		return 0;
	};
	static auto cubic_to = [](const FT_Vector *control_1, const FT_Vector *control_2, const FT_Vector *to, void *_current) {
		auto *current = reinterpret_cast<current_t *>(_current);
		if (!current->contour) {
			clap::log::error::major << "Character glyph is broken.";
			clap::log::info::critical << "Character id: 0x" << std::hex << current->char_id << ".";
			return 1;
		}
		current->contour->addEdge(new msdfgen::CubicSegment(current->position, point(control_1), point(control_2), point(to)));
		current->position = point(to);
		return 0;
	};
	FT_Outline_Funcs functions{ move_to, line_to, conic_to, cubic_to, 0, 0 };

	current_t current(char_id);
	error = FT_Outline_Decompose(&face->glyph->outline, &functions, &current);
	if (error) {
		clap::log::error::major << "Unable to decompose a character.";
		clap::log::info::critical << "Character id: 0x" << std::hex << char_id << ".";
	}
	if (!current.shape.contours.empty() && current.shape.contours.back().edges.empty())
		current.shape.contours.pop_back();

	current.shape.normalize();
	msdfgen::edgeColoringSimple(current.shape, 3);
	return current.shape;
}

class boundaries {
public:
	boundaries() : 
		x_min_(std::numeric_limits<double>::max()),
		x_max_(std::numeric_limits<double>::min()),
		y_min_(std::numeric_limits<double>::max()),
		y_max_(std::numeric_limits<double>::min()) {}

	auto x_min() const { return x_min_; }
	auto x_max() const { return x_max_; }
	auto y_min() const { return y_min_; }
	auto y_max() const { return y_max_; }

	void update(double x, double y) {
		x_min_ = std::min(x_min_, x);
		x_max_ = std::max(x_max_, x);
		y_min_ = std::min(y_min_, y);
		y_max_ = std::max(y_max_, y);
	}
private:		   
	double x_min_;
	double x_max_;
	double y_min_;
	double y_max_;

};
struct bitmap_data {
	double scale, translation_x, translation_y;
};

void calculate_boundaries(boundaries &boundaries, msdfgen::Shape const &shape) {
	for (auto const &contour : shape.contours)
		for (auto const &edge : contour.edges)
			for (double segment = 0.; segment < 1.; segment += .25)
				boundaries.update(edge->point(segment).x, edge->point(segment).y);
}

template<size_t bitmap_size>
bitmap_data calculate_bitmap_data(std::vector<msdfgen::Shape> const &shapes) {
	boundaries b;
	for (auto const &shape : shapes)
		calculate_boundaries(b, shape);

	auto limit_x = std::pow(2, std::ceil(std::log2(b.x_max() - b.x_min())));
	auto limit_y = std::pow(2, std::ceil(std::log2(b.y_max() - b.y_min())));

	bitmap_data out;
	out.scale = std::min(double(bitmap_size / limit_x),
						 double(bitmap_size / limit_y));
	out.translation_x = (limit_x - (b.x_max() + b.x_min())) / 2;
	out.translation_y = (limit_y - (b.y_max() + b.y_min())) / 2;
	return out;
}

//!TEMPORARY! 
#include "lodepng/lodepng.h"
//!TEMPORARY! 


clap::render::font clap::render::font::load(std::filesystem::path const &filename) {
	detail::state::ensure_initialized();

	FT_Face face;
	auto error = FT_New_Face(clap::render::detail::state::library(), filename.string().c_str(), 0, &face);
	if (error == FT_Err_Unknown_File_Format) {
		log::error::major << "Unsupported font file cannot be loaded.";
		log::info::critical << "Path: " << filename << ".";
	} else if (error) {
		log::error::major << "Cannot open font file.";
		log::info::critical << "Path: " << filename << ".";
	}
	if (face->charmap->encoding != FT_ENCODING_UNICODE) {
		log::error::minor << "Only unicode fonts are supported.";
		log::info::critical << "Path: " << filename << ".";
	}
	log::message::minor << "A new font face was loaded.";
	log::info::major << "Path: " << filename << ".";

	const auto msdf_pixel_width = gl::texture::_2d_array::maximum_width();
	const auto msdf_pixel_height = gl::texture::_2d_array::maximum_height();
	const auto msdf_pixel_count = gl::texture::_2d_array::maximum_count();

	const auto character_msdf_size = 32;
	constexpr auto color_count = 3;

	const auto msdf_width = msdf_pixel_width / character_msdf_size;
	const auto msdf_height = msdf_pixel_height / character_msdf_size;
	const auto msdf_count = msdf_pixel_count / character_msdf_size;

	const auto maximum_character_count = msdf_width * msdf_height * msdf_count;

	std::unordered_map<size_t, detail::glyph_info> font_data;
	std::vector<msdfgen::Shape> shapes;

	size_t char_code;
	unsigned index;
	size_t counter = 0;
	char_code = FT_Get_First_Char(face, &index);
	while (index != 0) {
		if (char_code > 40 && char_code < 60) { //!!!TEMPORARY!!!
			font_data.insert(std::pair(char_code, detail::glyph_info{ counter++ }));
			shapes.push_back(to_shape(face, char_code));
		}
		char_code = FT_Get_Next_Char(face, FT_ULong(char_code), &index);
	}

	if (maximum_character_count < font_data.size()) {
		log::error::critical << "Font is too big to fully load.";
		log::info::critical << "Number of characters in the font: " << font_data.size();
		log::info::critical << "Maximum number of supported characters per font: " << maximum_character_count;
	}

	auto bitmap_data = calculate_bitmap_data<character_msdf_size>(shapes);

	std::vector<msdfgen::Bitmap<float, color_count>> msdfs;
	for (auto &shape : shapes) {
		msdfs.emplace_back(character_msdf_size, character_msdf_size);
		generateMSDF(msdfs.back(), shape, 1.0, bitmap_data.scale, 
					 { bitmap_data.translation_x, bitmap_data.translation_y });
	}

	size_t width, height, count;
	if (font_data.size() < msdf_width) {
		width = font_data.size();
		height = 1;
		count = 1;
	} else if (font_data.size() < msdf_width * msdf_height) {
		width = msdf_width;
		height = size_t(std::ceil(float(font_data.size()) / msdf_width));
		count = 1;
	} else {
		width = msdf_width;
		height = msdf_height;
		count = size_t(std::ceil(float(font_data.size()) / msdf_width / msdf_height));
	}
	size_t characters_per_layer = width * height;
	size_t layer_size = characters_per_layer * color_count;
	size_t bitmap_size = character_msdf_size * character_msdf_size;

	size_t w = -2, h = -2;
	std::vector<std::vector<unsigned char>> output;
	for (auto &bitmap : msdfs) {
		if (++w >= width) {
			w = 0;
			if (++h >= height) {
				h = 0;
				output.emplace_back(layer_size * bitmap_size);
			}
		}

		for (size_t j = 0; j < size_t(bitmap.height()); j++)
			for (size_t i = 0; i < size_t(bitmap.width()); i++) {
				auto index = (
					(i + w * character_msdf_size) +
					(j + h * character_msdf_size) * width * character_msdf_size
				) * color_count;

				auto *temp = bitmap(int(i), int(bitmap.height() - j + 1));
				for (size_t i = 0; i < color_count; i++)
					output.back()[index + i] = to_byte(temp[i]);
			}
	}

	static size_t texture_counter = 0;
	for (auto &texture : output)
		lodepng::encode("texture_" + std::to_string(texture_counter++) + "_" + face->family_name + ".png",
						texture,
						unsigned(width * character_msdf_size),
						unsigned(height * character_msdf_size),
						LCT_RGB);

	return clap::render::font{ gl::texture::_2d_array(nullptr, 32, 32, 1) };
}