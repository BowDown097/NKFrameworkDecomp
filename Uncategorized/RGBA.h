#pragma once

#include <array>
#include <cstdint>
#include <string>

class CVec4;
class CRGBA {
public:
	CRGBA() = default;
	~CRGBA() = default;

	CRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	CRGBA(uint32_t color);
	CRGBA(const CRGBA& other) = default;
	CRGBA(const CVec4& color);
	CRGBA(const std::string& color);

	CRGBA& operator*(const float& brightness);
	CRGBA& operator=(const std::string& color);

	union {
		uint32_t mColor = 0;
		std::array<uint8_t, 4> mChannels;
	};
};