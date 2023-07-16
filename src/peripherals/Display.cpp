#include "peripherals/Display.h"

#include "hardware/gpio.h"
#include "pico/time.h"

#include <array>
#include <list>
#include <numeric>
#include <string>

namespace Doncon::Peripherals {

static const std::array<uint8_t, 546> drum_bmp = {
    0x42, 0x4d, 0x22, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x82, 0x00, 0x00, 0x00, 0x6c, 0x00, 0x00, 0x00, 0x34,
    0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0, 0x01, 0x00, 0x00,
    0x23, 0x2e, 0x00, 0x00, 0x23, 0x2e, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x42, 0x47, 0x52, 0x73, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xf8,
    0x01, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0x87, 0xfe, 0x1f, 0xff, 0xf0, 0x00, 0xff, 0xfe, 0x7f, 0xff, 0xe7, 0xff,
    0xf0, 0x00, 0xff, 0xf9, 0xff, 0xff, 0xf9, 0xff, 0xf0, 0x00, 0xff, 0xe7, 0xff, 0xff, 0xfe, 0x7f, 0xf0, 0x00, 0xff,
    0xcf, 0xff, 0xff, 0xff, 0x3f, 0xf0, 0x00, 0xff, 0xbf, 0xfc, 0x03, 0xff, 0xdf, 0xf0, 0x00, 0xff, 0x7f, 0xc1, 0xf8,
    0x3f, 0xef, 0xf0, 0x00, 0xfe, 0xff, 0x1f, 0xff, 0x8f, 0xf7, 0xf0, 0x00, 0xfd, 0xfe, 0x7f, 0xff, 0xe7, 0xfb, 0xf0,
    0x00, 0xfb, 0xf8, 0xff, 0xff, 0xf1, 0xfd, 0xf0, 0x00, 0xf3, 0xf3, 0xff, 0xff, 0xfc, 0xfc, 0xf0, 0x00, 0xf7, 0xe7,
    0xff, 0xff, 0xfe, 0x7e, 0xf0, 0x00, 0xef, 0xcf, 0xff, 0xff, 0xff, 0x3f, 0x70, 0x00, 0xef, 0xdf, 0xff, 0xff, 0xff,
    0xbf, 0x70, 0x00, 0xdf, 0x9f, 0xff, 0xff, 0xff, 0x9f, 0xb0, 0x00, 0xdf, 0x3f, 0xff, 0xff, 0xff, 0xcf, 0xb0, 0x00,
    0xbf, 0x7f, 0xff, 0xff, 0xff, 0xef, 0xd0, 0x00, 0xbe, 0x7f, 0xff, 0xff, 0xff, 0xe7, 0xd0, 0x00, 0xbe, 0xff, 0xff,
    0xff, 0xff, 0xf7, 0xd0, 0x00, 0xbe, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xd0, 0x00, 0x7e, 0xff, 0xff, 0xff, 0xff, 0xf7,
    0xe0, 0x00, 0x7c, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xe0, 0x00, 0x7d, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xe0, 0x00, 0x7d,
    0xff, 0xff, 0xff, 0xff, 0xfb, 0xe0, 0x00, 0x7d, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xe0, 0x00, 0x7d, 0xff, 0xff, 0xff,
    0xff, 0xfb, 0xe0, 0x00, 0x7d, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xe0, 0x00, 0x7d, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xe0,
    0x00, 0x7c, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xe0, 0x00, 0x7e, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xe0, 0x00, 0xbe, 0xff,
    0xff, 0xff, 0xff, 0xf7, 0xd0, 0x00, 0xbe, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xd0, 0x00, 0xbe, 0x7f, 0xff, 0xff, 0xff,
    0xe7, 0xd0, 0x00, 0xbf, 0x7f, 0xff, 0xff, 0xff, 0xef, 0xd0, 0x00, 0xdf, 0x3f, 0xff, 0xff, 0xff, 0xcf, 0xb0, 0x00,
    0xdf, 0x9f, 0xff, 0xff, 0xff, 0x9f, 0xb0, 0x00, 0xef, 0xdf, 0xff, 0xff, 0xff, 0xbf, 0x70, 0x00, 0xef, 0xcf, 0xff,
    0xff, 0xff, 0x3f, 0x70, 0x00, 0xf7, 0xe7, 0xff, 0xff, 0xfe, 0x7e, 0xf0, 0x00, 0xf3, 0xf3, 0xff, 0xff, 0xfc, 0xfc,
    0xf0, 0x00, 0xfb, 0xf8, 0xff, 0xff, 0xf1, 0xfd, 0xf0, 0x00, 0xfd, 0xfe, 0x7f, 0xff, 0xe7, 0xfb, 0xf0, 0x00, 0xfe,
    0xff, 0x1f, 0xff, 0x8f, 0xf7, 0xf0, 0x00, 0xff, 0x7f, 0xc1, 0xf8, 0x3f, 0xef, 0xf0, 0x00, 0xff, 0xbf, 0xfc, 0x03,
    0xff, 0xdf, 0xf0, 0x00, 0xff, 0xcf, 0xff, 0xff, 0xff, 0x3f, 0xf0, 0x00, 0xff, 0xe7, 0xff, 0xff, 0xfe, 0x7f, 0xf0,
    0x00, 0xff, 0xf9, 0xff, 0xff, 0xf9, 0xff, 0xf0, 0x00, 0xff, 0xfe, 0x7f, 0xff, 0xe7, 0xff, 0xf0, 0x00, 0xff, 0xff,
    0x87, 0xfe, 0x1f, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xf8, 0x01, 0xff, 0xff, 0xf0, 0x00};

static const std::array<uint8_t, 546> don_l_bmp = {
    0x42, 0x4d, 0x22, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x82, 0x00, 0x00, 0x00, 0x6c, 0x00, 0x00, 0x00, 0x34,
    0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0, 0x01, 0x00, 0x00,
    0x23, 0x2e, 0x00, 0x00, 0x23, 0x2e, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x42, 0x47, 0x52, 0x73, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xfe, 0x3f,
    0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xe0, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0x80, 0x3f, 0xff, 0xff, 0xf0,
    0x00, 0xff, 0xff, 0x00, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xfc, 0x00, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xf8,
    0x00, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xf0, 0x00, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xe0, 0x00, 0x3f, 0xff,
    0xff, 0xf0, 0x00, 0xff, 0xe0, 0x00, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xc0, 0x00, 0x3f, 0xff, 0xff, 0xf0, 0x00,
    0xff, 0x80, 0x00, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xff, 0x80, 0x00, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xff, 0x00, 0x00,
    0x3f, 0xff, 0xff, 0xf0, 0x00, 0xff, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xff, 0x00, 0x00, 0x3f, 0xff, 0xff,
    0xf0, 0x00, 0xff, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xfe, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xfe,
    0x00, 0x00, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xfe, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xfe, 0x00, 0x00, 0x3f,
    0xff, 0xff, 0xf0, 0x00, 0xfe, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xfe, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xf0,
    0x00, 0xff, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xff, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xff, 0x00,
    0x00, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xff, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xff, 0x80, 0x00, 0x3f, 0xff,
    0xff, 0xf0, 0x00, 0xff, 0x80, 0x00, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xc0, 0x00, 0x3f, 0xff, 0xff, 0xf0, 0x00,
    0xff, 0xe0, 0x00, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xe0, 0x00, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xf0, 0x00,
    0x3f, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xf8, 0x00, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xfc, 0x00, 0x3f, 0xff, 0xff,
    0xf0, 0x00, 0xff, 0xff, 0x00, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0x80, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xff,
    0xff, 0xe0, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xfe, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0,
    0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00};

static const std::array<uint8_t, 546> don_r_bmp = {
    0x42, 0x4d, 0x22, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x82, 0x00, 0x00, 0x00, 0x6c, 0x00, 0x00, 0x00, 0x34,
    0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0, 0x01, 0x00, 0x00,
    0x23, 0x2e, 0x00, 0x00, 0x23, 0x2e, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x42, 0x47, 0x52, 0x73, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc7,
    0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x7f, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x1f, 0xff, 0xf0,
    0x00, 0xff, 0xff, 0xff, 0xc0, 0x0f, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xf0, 0x00, 0xff, 0xff,
    0xff, 0xc0, 0x01, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x00, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x00,
    0x7f, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x7f, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x3f, 0xf0, 0x00,
    0xff, 0xff, 0xff, 0xc0, 0x00, 0x1f, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x1f, 0xf0, 0x00, 0xff, 0xff, 0xff,
    0xc0, 0x00, 0x0f, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x0f, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x0f,
    0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x0f, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x07, 0xf0, 0x00, 0xff,
    0xff, 0xff, 0xc0, 0x00, 0x07, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x07, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0,
    0x00, 0x07, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x07, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x07, 0xf0,
    0x00, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x0f, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x0f, 0xf0, 0x00, 0xff, 0xff,
    0xff, 0xc0, 0x00, 0x0f, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x0f, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x00,
    0x1f, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x1f, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x3f, 0xf0, 0x00,
    0xff, 0xff, 0xff, 0xc0, 0x00, 0x7f, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x7f, 0xf0, 0x00, 0xff, 0xff, 0xff,
    0xc0, 0x00, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x01, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x03, 0xff,
    0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x0f, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x1f, 0xff, 0xf0, 0x00, 0xff,
    0xff, 0xff, 0xc0, 0x7f, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc7, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0,
    0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00};

static const std::array<uint8_t, 546> ka_l_bmp = {
    0x42, 0x4d, 0x22, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x82, 0x00, 0x00, 0x00, 0x6c, 0x00, 0x00, 0x00, 0x34,
    0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0, 0x01, 0x00, 0x00,
    0x23, 0x2e, 0x00, 0x00, 0x23, 0x2e, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x42, 0x47, 0x52, 0x73, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0x80, 0x3f, 0xff, 0xff,
    0xf0, 0x00, 0xff, 0xfe, 0x00, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xf8, 0x00, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xff,
    0xf0, 0x00, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xff, 0x80, 0x3f, 0xff,
    0xff, 0xff, 0xf0, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xfe, 0x01, 0xff, 0xff, 0xff, 0xff, 0xf0,
    0x00, 0xfc, 0x07, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xfc, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xf8, 0x1f,
    0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xf0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xf0, 0x3f, 0xff, 0xff, 0xff,
    0xff, 0xf0, 0x00, 0xe0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00,
    0xc0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xc1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xc1, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xf0, 0x00, 0xc1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x81, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xf0, 0x00, 0x83, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x83, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x83,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x83, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x83, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xf0, 0x00, 0x83, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x83, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0,
    0x00, 0x83, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x81, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xc1, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xc1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xc1, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xf0, 0x00, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00,
    0xe0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xf0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xf0, 0x3f, 0xff,
    0xff, 0xff, 0xff, 0xf0, 0x00, 0xf8, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xfc, 0x0f, 0xff, 0xff, 0xff, 0xff,
    0xf0, 0x00, 0xfc, 0x07, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xfe, 0x01, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xff,
    0x00, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xff, 0x80, 0x3f, 0xff, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xc0, 0x03, 0xff,
    0xff, 0xff, 0xf0, 0x00, 0xff, 0xf0, 0x00, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xf8, 0x00, 0x3f, 0xff, 0xff, 0xf0,
    0x00, 0xff, 0xfe, 0x00, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0x80, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff,
    0xf8, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00};

static const std::array<uint8_t, 546> ka_r_bmp = {
    0x42, 0x4d, 0x22, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x82, 0x00, 0x00, 0x00, 0x6c, 0x00, 0x00, 0x00, 0x34,
    0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0, 0x01, 0x00, 0x00,
    0x23, 0x2e, 0x00, 0x00, 0x23, 0x2e, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
    0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x42, 0x47, 0x52, 0x73, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc1, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x1f, 0xff,
    0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x07, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x01, 0xff, 0xf0, 0x00, 0xff,
    0xff, 0xff, 0xc0, 0x00, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x3f, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff,
    0xc0, 0x1f, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x0f, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x07, 0xf0,
    0x00, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x03, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0xf0, 0x00, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x81, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xc0, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x70, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x70, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x30, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x30, 0x00, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xf8, 0x30, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x30, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8,
    0x10, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x10, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x10, 0x00, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xfc, 0x10, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x10, 0x00, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xfc, 0x10, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x10, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x10,
    0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x10, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x10, 0x00, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xf8, 0x30, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x30, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xf8, 0x30, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x30, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x70, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x70, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0xf0, 0x00, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xc0, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x81, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03,
    0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x03, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x07, 0xf0, 0x00, 0xff,
    0xff, 0xff, 0xff, 0xf0, 0x0f, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x1f, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xfc,
    0x00, 0x3f, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x00, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x01, 0xff, 0xf0,
    0x00, 0xff, 0xff, 0xff, 0xc0, 0x07, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xc0, 0x1f, 0xff, 0xf0, 0x00, 0xff, 0xff,
    0xff, 0xc1, 0xff, 0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00};

Display::Display(const Config &config)
    : m_config(config), m_state(State::Idle), m_input_state({}), m_usb_mode(USB_MODE_DEBUG), m_player_id(0) {
    m_display.external_vcc = false;
    ssd1306_init(&m_display, 128, 64, m_config.i2c_address, m_config.i2c_block);
    ssd1306_clear(&m_display);
}

void Display::setInputState(const Utils::InputState &state) { m_input_state = state; }
void Display::setUsbMode(usb_mode_t mode) { m_usb_mode = mode; };
void Display::setPlayerId(uint8_t player_id) { m_player_id = player_id; };

void Display::setMenuState(const Utils::Menu::State &menu_state) { m_menu_state = menu_state; }

void Display::showIdle() { m_state = State::Idle; }
void Display::showMenu() { m_state = State::Menu; }

static std::string modeToString(usb_mode_t mode) {
    switch (mode) {
    case USB_MODE_SWITCH_TATACON:
        return "Switch Tatacon";
    case USB_MODE_SWITCH_HORIPAD:
        return "Switch Horipad";
    case USB_MODE_DUALSHOCK3:
        return "Dualshock 3";
    case USB_MODE_PS4_TATACON:
        return "PS4 Tatacon";
    case USB_MODE_DUALSHOCK4:
        return "Dualshock 4";
    case USB_MODE_XBOX360:
        return "Xbox 360";
    case USB_MODE_DEBUG:
        return "Debug";
    }
    return "?";
}

static std::vector<uint16_t> updateRollCounters(const Utils::InputState::Drum &drum, const uint32_t timeout_ms) {
    struct CounterState {
        uint32_t last_triggered_time;
        bool last_state;
        uint16_t count;
    };

    static CounterState ka_left_state = {0, false, 0};
    static CounterState don_left_state = {0, false, 0};
    static CounterState don_right_state = {0, false, 0};
    static CounterState ka_right_state = {0, false, 0};

    uint32_t now = to_ms_since_boot(get_absolute_time());

    auto check_roll = [&](CounterState &target, bool current_state) {
        if (current_state && (target.last_state != current_state)) {
            if ((now - target.last_triggered_time) > timeout_ms) {
                target.count = 0;
            }

            target.last_triggered_time = now;
            target.count++;
        }

        target.last_state = current_state;
    };

    check_roll(ka_left_state, drum.ka_left.triggered);
    check_roll(don_left_state, drum.don_left.triggered);
    check_roll(don_right_state, drum.don_right.triggered);
    check_roll(ka_right_state, drum.ka_right.triggered);

    return {ka_left_state.count, don_left_state.count, don_right_state.count, ka_right_state.count};
}

void Display::drawIdleScreen() {
    // Header
    std::string mode_string = modeToString(m_usb_mode) + " mode";
    ssd1306_draw_string(&m_display, 0, 0, 1, mode_string.c_str());
    ssd1306_draw_line(&m_display, 0, 10, 128, 10);

    // Drum
    ssd1306_bmp_show_image_with_offset(&m_display, drum_bmp.data(), drum_bmp.size(), 64 - 26, 12);

    if (m_input_state.drum.don_left.triggered) {
        ssd1306_bmp_show_image_with_offset(&m_display, don_l_bmp.data(), don_l_bmp.size(), 64 - 26, 12);
    }
    if (m_input_state.drum.don_right.triggered) {
        ssd1306_bmp_show_image_with_offset(&m_display, don_r_bmp.data(), don_r_bmp.size(), 64 - 26, 12);
    }
    if (m_input_state.drum.ka_left.triggered) {
        ssd1306_bmp_show_image_with_offset(&m_display, ka_l_bmp.data(), ka_l_bmp.size(), 64 - 26, 12);
    }
    if (m_input_state.drum.ka_right.triggered) {
        ssd1306_bmp_show_image_with_offset(&m_display, ka_r_bmp.data(), ka_r_bmp.size(), 64 - 26, 12);
    }

    // Roll counters
    auto roll_counters = updateRollCounters(m_input_state.drum, m_config.roll_counter_timeout_ms);
    int num = 1;
    for (const auto &counter : roll_counters) {
        ssd1306_draw_string(&m_display, 2, 12 * num, 1, std::to_string(counter).c_str());
        num++;
    }

    // Player "LEDs"
    if (m_player_id != 0) {
        for (uint8_t i = 0; i < 4; ++i) {
            if (m_player_id & (1 << i)) {
                ssd1306_draw_square(&m_display, ((127) - ((4 - i) * 6)) - 1, 2, 4, 4);
            } else {
                ssd1306_draw_square(&m_display, (127) - ((4 - i) * 6), 3, 2, 2);
            }
        }
    }

    // Menu hint
    ssd1306_draw_line(&m_display, 0, 54, 128, 54);
    ssd1306_draw_string(&m_display, 0, 56, 1, "Hold STA+SEL for Menu");
}

void Display::drawMenuScreen() {
    auto descriptor_it = Utils::Menu::descriptors.find(m_menu_state.page);
    if (descriptor_it == Utils::Menu::descriptors.end()) {
        return;
    }

    // Background
    switch (descriptor_it->second.type) {
    case Utils::Menu::Descriptor::Type::Root:
        // ssd1306_bmp_show_image(&m_display, menu_screen_top.data(), menu_screen_top.size());
        break;
    case Utils::Menu::Descriptor::Type::Selection:
    case Utils::Menu::Descriptor::Type::Value:
        // ssd1306_bmp_show_image(&m_display, menu_screen_sub.data(), menu_screen_sub.size());
        break;
    case Utils::Menu::Descriptor::Type::RebootInfo:
        break;
    }

    // Heading
    ssd1306_draw_string(&m_display, 0, 0, 1, descriptor_it->second.name.c_str());

    // Current Selection
    std::string selection;
    switch (descriptor_it->second.type) {
    case Utils::Menu::Descriptor::Type::Root:
    case Utils::Menu::Descriptor::Type::Selection:
    case Utils::Menu::Descriptor::Type::RebootInfo:
        selection = descriptor_it->second.items.at(m_menu_state.selection).first;
        break;
    case Utils::Menu::Descriptor::Type::Value:
        selection = std::to_string(m_menu_state.selection);
        break;
    }
    ssd1306_draw_string(&m_display, (127 - (selection.length() * 12)) / 2, 15, 2, selection.c_str());

    // Breadcrumbs
    switch (descriptor_it->second.type) {
    case Utils::Menu::Descriptor::Type::Root:
    case Utils::Menu::Descriptor::Type::Selection: {
        auto selection_count = descriptor_it->second.items.size();
        for (uint8_t i = 0; i < selection_count; ++i) {
            if (i == m_menu_state.selection) {
                ssd1306_draw_square(&m_display, ((127) - ((selection_count - i) * 6)) - 1, 2, 4, 4);
            } else {
                ssd1306_draw_square(&m_display, (127) - ((selection_count - i) * 6), 3, 2, 2);
            }
        }
    } break;
    case Utils::Menu::Descriptor::Type::RebootInfo:
        break;
    case Utils::Menu::Descriptor::Type::Value:
        break;
    }
}

void Display::update() {
    static const uint32_t interval_ms = 17; // Limit to ~60fps
    static uint32_t start_ms = 0;

    if (to_ms_since_boot(get_absolute_time()) - start_ms < interval_ms) {
        return;
    }
    start_ms += interval_ms;

    ssd1306_clear(&m_display);

    switch (m_state) {
    case State::Idle:
        drawIdleScreen();
        break;
    case State::Menu:
        drawMenuScreen();
        break;
    }

    ssd1306_show(&m_display);
};

} // namespace Doncon::Peripherals