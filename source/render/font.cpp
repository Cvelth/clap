#include "render/font.hpp"
#include "render/detail/state.hpp"

#include "essential/log.hpp"

#include FT_OUTLINE_H
#include "msdfgen/msdfgen.h"

namespace clap::render::detail {
	class font_face_t {
	public:
		FT_Face handle;

		font_face_t() : handle{} {}
		font_face_t(font_face_t const &) = delete;
		font_face_t(font_face_t &&other) noexcept : handle(std::move(other.handle)) {
			other.handle = {};
		}
		~font_face_t() {
			FT_Done_Face(handle);
		}

		msdfgen::Shape glyph_shape(unsigned char_id);
	};
}

static msdfgen::Point2 point(const FT_Vector *vector) {
	return msdfgen::Point2{
		double(vector->x) / 64.,
		double(vector->y) / 64.
	};
}

msdfgen::Shape clap::render::detail::font_face_t::glyph_shape(unsigned char_id) {
	FT_Error error = FT_Load_Char(handle, char_id, FT_LOAD_NO_SCALE);
	if (error) {
		clap::log::error::major << "Unable to read a character.";
		clap::log::info::critical << "Character id: 0x" << std::hex << char_id << ".";
	}

	struct current_t {
		msdfgen::Shape shape;
		msdfgen::Contour *contour = nullptr;
		msdfgen::Point2 position;
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
			return 1;
		}
		current->contour->addEdge(new msdfgen::CubicSegment(current->position, point(control_1), point(control_2), point(to)));
		current->position = point(to);
		return 0;
	};

	FT_Outline_Funcs functions{
		move_to,
		line_to,
		conic_to,
		cubic_to,
		0, 0
	};

	current_t current;
	error = FT_Outline_Decompose(&handle->glyph->outline, &functions, &current);
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

clap::render::font::font(detail::font_face_t &&face) noexcept : face(new detail::font_face_t(std::move(face))) {}
clap::render::font::font(font &&other) noexcept : face(other.face) { other.face = nullptr; }

clap::render::font::~font() {
	if (face)
		delete face;
}

//!TEMPORARY! 
#include "lodepng/lodepng.h"
#include <limits>
#include <algorithm>
//!TEMPORARY! 

clap::render::font clap::render::font::load(std::string const &filename) {
	detail::state::ensure_initialized();

	detail::font_face_t face;
	auto error = FT_New_Face(clap::render::detail::state::library(), filename.c_str(), 0, &face.handle);
	if (error == FT_Err_Unknown_File_Format) {
		log::error::major << "Unsupported font file cannot be loaded.";
		log::info::critical << "Path: " << filename << ".";
	} else if (error) {
		log::error::major << "Cannot open font file.";
		log::info::critical << "Path: " << filename << ".";
	}

	//!TEMPORARY! 
	auto shape = face.glyph_shape('W');

	double x_min = std::numeric_limits<double>::max();
	double x_max = std::numeric_limits<double>::min();
	double y_min = std::numeric_limits<double>::max();
	double y_max = std::numeric_limits<double>::min();
	auto update_bounds = [&](double x, double y) {
		x_min = std::min(x_min, x);
		x_max = std::max(x_max, x);
		y_min = std::min(y_min, y);
		y_max = std::max(y_max, y);
	};

	for (auto const &contour : shape.contours)
		for (auto const &edge : contour.edges)
			for (double segment = 0.; segment < 1.; segment += .25)
				update_bounds(edge->point(segment).x, edge->point(segment).y);

	auto delta_x = x_max - x_min;
	auto delta_y = y_max - y_min;

	auto limit_x = std::pow(2, std::ceil(std::log2(delta_x)));
	auto limit_y = std::pow(2, std::ceil(std::log2(delta_y)));

	const size_t x_size = 4, y_size = 4;
	auto scale = std::max(double(x_size / limit_x), double(y_size / limit_y));
	auto translation_x = (limit_x - delta_x) / 2;
	auto translation_y = (limit_y - delta_y) / 2;

	msdfgen::Bitmap<float, 3> bitmap(x_size, y_size);
	generateMSDF(bitmap, shape, 1.0, scale, { translation_x, translation_y });

	auto to_byte = [](float input) {
		return std::clamp(input * std::numeric_limits<unsigned char>::max(),
						  0.f,
						  float(std::numeric_limits<unsigned char>::max())
		);
	};
	std::vector<unsigned char> output;
	output.reserve(3ull * bitmap.width() * bitmap.height());
	for (auto j = bitmap.height() - 1; j >= 0; j--)
		for (auto i = 0; i < bitmap.width(); i++) {
			auto *temp = bitmap(i, j);
			output.push_back(to_byte(temp[0]));
			output.push_back(to_byte(temp[1]));
			output.push_back(to_byte(temp[2]));
		}

	lodepng::encode("W_t.png", output, //(unsigned char*)bitmap.operator const float *(),
					bitmap.width(), bitmap.height(), LCT_RGB);
	//!TEMPORARY! 

	return clap::render::font{ std::move(face) };
}