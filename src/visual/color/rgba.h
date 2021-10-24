#ifndef RGBA_H
#define RGBA_H

#include <cstdint>

union RGBA {
	unsigned char rgba[4];
	struct {
		unsigned char r;
		unsigned char g;
		unsigned char b;
        unsigned char a;
	};
	int i;
	uint32_t ui32;

	RGBA();
	RGBA(const int x);
	RGBA(const unsigned char r_, const unsigned char g_, const unsigned char b_, const unsigned char a_ = 255);

	RGBA operator+(const RGBA &other) const;

	RGBA operator/(const float &coef) const;
	RGBA operator*(const float &coef) const;
};

extern const unsigned char CLRMAX;


#endif // RGBA_H