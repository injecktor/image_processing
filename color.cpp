#include "color.h"

color_t::color_t() {
    set(0xff000000);
}

color_t::color_t(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
    : m_a(a), m_r(r), m_g(g), m_b(b) {
}

color_t::color_t(const unsigned hex) {
    set(hex);
}

color_t::color_t(unsigned hex, float alpha) {
    set(hex & 0xffffff | static_cast<unsigned>(alpha * 0xff) << 24);
}

color_t::color_t(unsigned hex, double alpha) {
    set(hex & 0xffffff | static_cast<unsigned>(alpha * 0xff) << 24);
}

void color_t::set(const unsigned hex) {
    m_a = (hex >> 24) & 0xff;
    m_r = (hex >> 16) & 0xff;
    m_g = (hex >> 8) & 0xff;
    m_b = hex & 0xff;
}
void color_t::set(unsigned hex, float alpha) {
    set(hex & 0xffffff | static_cast<unsigned>(alpha * 0xff) << 24);
}
void color_t::set(unsigned hex, double alpha) {
    set(hex & 0xffffff | static_cast<unsigned>(alpha * 0xff) << 24);
}

uint8_t& color_t::a() {
    return m_a;
}
uint8_t& color_t::r() {
    return m_r;
}
uint8_t& color_t::g() {
    return m_g;
}
uint8_t& color_t::b() {
    return m_b;
}
[[nodiscard]] uint32_t color_t::get_hex() const {
    return (m_a << 24) | (m_r << 16) | (m_g << 8) | m_b;
}

[[nodiscard]] color_t color_t::apply_alpha(color_t color_with_alpha, color_t background) {
    signed r_diff = static_cast<signed>(color_with_alpha.r()) - static_cast<signed>(background.r());
    signed g_diff = static_cast<signed>(color_with_alpha.g()) - static_cast<signed>(background.g());
    signed b_diff = static_cast<signed>(color_with_alpha.b()) - static_cast<signed>(background.b());
    auto alpha = static_cast<float>(color_with_alpha.a());
    alpha /= 0xff;
    alpha = 1 - alpha;
    return color_t(0xff, static_cast<uint8_t>(static_cast<signed>(color_with_alpha.r()) - static_cast<signed>(alpha * r_diff)),
        static_cast<uint8_t>(static_cast<signed>(color_with_alpha.g()) - static_cast<signed>(alpha * g_diff)),
        static_cast<uint8_t>(static_cast<signed>(color_with_alpha.b()) - static_cast<signed>(alpha * b_diff)));
}
