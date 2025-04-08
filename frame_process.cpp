#include "frame_process.h"

frame_process_t::frame_process_t(const unsigned width, const unsigned height) : m_width(width),
    m_height(height),
    m_resolution(width * height) {
    m_image_buffer.reserve(m_resolution);
    m_background_bit_mask.reserve((m_resolution - 1) / 8 + 1);
    for (unsigned i = 0; i < m_resolution; ++i) {
        m_background_bit_mask.emplace_back(0xFF);
        m_image_buffer.emplace_back(color_t::white);
    }
}

void frame_process_t::set_pixel(const color_t color, const unsigned x, const unsigned y) {
    if (x >= m_width || y >= m_height) return;
    m_image_buffer[y * m_width + x] = color;
    m_background_bit_mask[(y * m_width + x) / 8] &= ~(1 << ((y * m_width + x) % 8));
}


void frame_process_t::set_background(const color_t color) {
    for (unsigned i = 0; i < m_resolution; ++i) {
        if (m_background_bit_mask[i / 8] & (1 << (i % 8))) {
            m_image_buffer[i] = color;
        }
    }
}

void frame_process_t::circle(const color_t color, const unsigned x, const unsigned y, const unsigned radius,
                             const bool fill) {
    ASSERT(radius != 0, "Can't draw circle with zero radius");
    const auto s_radius = static_cast<signed>(radius);
    for (signed i = -s_radius + 1; i <= s_radius - 1; ++i) {
        for (signed j = -s_radius + 1; j <= s_radius - 1; ++j) {
            if (is_in_circle(i, j, radius) && (fill || i == -s_radius + 1 || j == -s_radius + 1 || !
                                               is_in_circle(i - 1, j, radius) || !is_in_circle(i + 1, j, radius)
                                               || !is_in_circle(i, j - 1, radius) || !is_in_circle(i, j + 1, radius))) {
                set_pixel(color, x + i, y + j);
            }
        }
    }
}

void frame_process_t::line(const color_t color, const unsigned width, const unsigned x1, const unsigned y1,
                           const unsigned x2, const unsigned y2) {
    double x = x1, y = y1;
    const double dx = static_cast<signed>(x2) - static_cast<signed>(x1);
    const double dy = static_cast<signed>(y2) - static_cast<signed>(y1);
    if (abs(dx) > abs(dy)) {
        while (static_cast<unsigned>(x) != x2) {
            const double tangent = dy / dx;
            const double upper = ceil(y);
            const double lower = floor(y);
            set_pixel(color_t(color.get_hex(), 1. - (upper - y)), static_cast<unsigned>(x), static_cast<unsigned>(upper));
            set_pixel(color_t(color.get_hex(), 1. - (y - lower)), static_cast<unsigned>(x), static_cast<unsigned>(lower));
            y = y + tangent;
            x += 1;
        }
    } else {
        while (static_cast<unsigned>(y) != y2) {
            const double tangent = dx / dy;
            set_pixel(color, static_cast<unsigned>(round(x)), static_cast<unsigned>(y));
            x = x + tangent;
            y += 1;
        }
    }
}

void frame_process_t::square(const color_t color, const unsigned width, const unsigned x1, const unsigned y1,
                             const unsigned length) {
    line(color, width, x1, y1, x1 + length, y1);
    line(color, width, x1, y1, x1, y1 + length);
    line(color, width, x1 + length, y1, x1 + length, y1 + length);
    line(color, width, x1, y1 + length, x1 + length, y1 + length);
}

void frame_process_t::rectangle(const color_t color, const unsigned width, const unsigned x1, const unsigned y1,
                                const unsigned x2, const unsigned y2) {
    line(color, width, x1, y1, x2, y1);
    line(color, width, x1, y1, x1, y2);
    line(color, width, x2, y1, x2, y2);
    line(color, width, x1, y2, x2, y2);
}

void frame_process_t::triangle(const color_t color, const unsigned width, const unsigned x1, const unsigned y1,
                               const unsigned x2, const unsigned y2, const unsigned x3,
                               const unsigned y3) {
    line(color, width, x1, y1, x2, y2);
    line(color, width, x3, y3, x2, y2);
    line(color, width, x1, y1, x3, y3);
}

void frame_process_t::generate_image(const std::string &file_name, const image_type image_type) const {
    std::shared_ptr<image_format_t> img_gen;
    std::ofstream file;
    switch (image_type) {
        case image_type::ppm: {
            img_gen = std::make_shared<ppm_t>(&file, m_width, m_height);
        }
        break;
        case image_type::png: {
            ASSERT(false, "Not implemented");
        }
        break;
        default: {
            ASSERT(false, "Unknown image format");
        }
        break;
    }
    file.open("images/" + file_name + '.' + img_gen->get_format_extension(), 
        std::ofstream::out | std::ofstream::binary);
    ASSERT(file.is_open(), "Could not open file");
    img_gen->init();
    img_gen->generate(m_image_buffer);
    file.close();
}

bool frame_process_t::is_in_circle(const signed x, const signed y, const unsigned radius) {
    if (pow(x, 2) + pow(y, 2) <= pow(radius, 2)) {
        return true;
    }
    return false;
}
