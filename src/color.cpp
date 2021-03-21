#include "color.h"

Color::Color()
{
    this->R = 0;
    this->G = 0;
    this->B = 0;
	this->a = 1;
}

Color::Color( float r, float g, float b, float a)
{
	this->R = r;
	this->G = g;
	this->B = b;
	this->a = a;
}

Color Color::operator*(const Color& c) const
{
    return Color(this->R * c.R, this->G * c.G, this->B * c.B);
}

Color Color::operator*(const float factor) const
{
	return Color(this->R * factor, this->G * factor, this->B * factor);
}

Color Color::operator+(const Color& c) const
{
    return Color(this->R + c.R, this->G + c.G, this->B + c.B);
}

Color& Color::operator+=(const Color& c)
{
	this->R += c.R;
	this->G += c.G;
	this->B += c.B;

	return *this;
}