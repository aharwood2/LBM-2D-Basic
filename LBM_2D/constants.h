#pragma once
#include <cmath>

// Velocity vectors D2Q9
static const int c[9][2] =
{
	{  1,  0 },
	{ -1,  0 },
	{  0,  1 },
	{  0, -1 },
	{  1,  1 },
	{ -1, -1 },
	{  1, -1 },
	{ -1,  1 },
	{  0,  0 }
};

// Weights D2Q9
static const double w[9] =
{
	1.0 / 9.0,
	1.0 / 9.0,
	1.0 / 9.0,
	1.0 / 9.0,
	1.0 / 36.0,
	1.0 / 36.0,
	1.0 / 36.0,
	1.0 / 36.0,
	4.0 / 9.0
};

// Vector of opposites
static const int vopp[9] = { 1, 0, 3, 2, 5, 4, 7, 6, 8 };

// Speed of sound
static const double cs = 1.0 / std::sqrt(3.0);
