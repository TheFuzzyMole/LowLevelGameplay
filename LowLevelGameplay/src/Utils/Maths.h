#pragma once
#include <math.h>

namespace LLGP
{
#define PI 3.1415926535
#define EPSILON 1.401298E-45f
#define RAD2DEG 57.295779513
#define DEG2RAD 0.01745329252

	static const double dmod(double x, double y) { return x - (int)(x / y) * y; }
	static float clamp(float value, float min, float max) { return std::fmaxf(min, std::fminf(max, value)); }
}
