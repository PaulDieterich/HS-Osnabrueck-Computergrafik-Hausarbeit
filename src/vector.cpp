#include "vector.h"
#include <assert.h>
#include <math.h>
#define EPSILON 1e-6

float getSurface(const Vector &a, const Vector &b, const Vector &c) {
    return ((b-a).cross((c-b))).length() / 2;
}

Vector::Vector( float x, float y, float z)
{
    this->X = x;
    this->Y = y;
    this->Z = z;
}

Vector::Vector(float xyz) : X(xyz), Y(xyz), Z(xyz)
{
}

Vector::Vector()
{
}

float Vector::dot(const Vector& v) const
{
	return (this->X * v.X) + (this->Y * v.Y) + (this->Z * v.Z);
}

Vector Vector::cross(const Vector& v) const
{
	return Vector((this->Y * v.Z) - (this->Z * v.Y), (this->Z * v.X) - (this->X * v.Z), (this->X * v.Y) - (this->Y * v.X));
}

Vector Vector::operator/(float c) const
{
    return Vector(this->X / c, this->Y / c, this->Z / c);
}

Vector Vector::operator/(Vector v) const
{
	return Vector(X / v.X, Y / v.Y, Z / v.Z);
}

Vector Vector::operator+(const Vector& v) const
{
	return Vector((this->X + v.X), (this->Y + v.Y), (this->Z + v.Z));
}

Vector Vector::operator-(const Vector& v) const
{
	return Vector((this->X - v.X), (this->Y - v.Y), (this->Z - v.Z));
}

// Scalar multiply
Vector Vector::operator*(float c) const
{
	return Vector(this->X * c, this->Y * c, this->Z * c);
}

// Negate?
Vector Vector::operator-() const
{
	return Vector(this->X * -1, this->Y * -1, this->Z * -1);
}



Vector& Vector::operator+=(const Vector& v)
{
    this->X += v.X;
    this->Y += v.Y;
    this->Z += v.Z;
	return *this;
}

Vector& Vector::operator+=(const float c) const
{
	return Vector(X + c, Y + c, Z + c);
}

Vector& Vector::operator-=(const Vector& v)
{
	this->X -= v.X;
	this->Y -= v.Y;
	this->Z -= v.Z;
	return *this;
}

Vector& Vector::operator-=(const float c)
{
	return Vector(X -= c, Y -= c, Z -= c);
}

Vector& Vector::normalize()
{
    float c = 1 / this->length();
    this->X = this->X*c;
    this->Y = this->Y*c;
    this->Z = this->Z*c;
	return *this;
}

Vector& Vector::lerp(Vector& v, double i)
{
	return Vector(
		X * (1.0 - i) + (double)v.X * i, 
		Y * (1.0 - i) + (double)v.Y * i, 
		Z * (1.0 - i) + (double)v.Z * i
	);
}

float Vector::length() const
{
	return sqrt(this->lengthSquared());
}

float Vector::lengthSquared() const
{
	return pow(this->X, 2) + pow(this->Y, 2) + pow(this->Z, 2);
}

Vector Vector::reflection( const Vector& normal) const
{
    // r=v-2s⋅n
    const float dn = 2 * this->dot(normal);
	return *this - (normal * dn);
}



bool Vector::triangleIntersection( const Vector& d, const Vector& a, const Vector& b, const Vector& c, float& s) const
{
	const Vector o = *this;

	// n ist der Vektor, der über df skaliert wird und die Ebene schneidet?
	const Vector ba = b-a;
	const Vector ca = c-a;
	const Vector n_counter = ba.cross(ca);
	const float n_numerator = (ba.cross(ca)).length(); // Was ist n?
    const Vector n = Vector(n_counter.X / n_numerator, n_counter.Y / n_numerator, n_counter.Z / n_numerator);

    // d (Faktor) ist der Faktor für die Skalierung des Richtungsvektors
	const float df = n.dot(a); // müsste hier nicht p hin? Wieso geht das mit einem Punkt? Geht das mit jedem Punkt? Umstellung der Ebenengleichung

	// s ist Skalierfaktor für den Richtungsvektor
	const float s_counter = df - n.dot(o);
	const float s_numerator = n.dot(d);
	s = s_counter / s_numerator;

	if (s < 0) return false;
	const Vector p = o + (d*s); // Strahlgleichung

	// Wenn der Flächeninhalt des Dreiecks mit punkt p kleiner ist als der des Ausgangsdreicks, liegt der Punkt im Dreieck
	return  getSurface(a, b, c) + EPSILON >= (getSurface(a, b, p) + getSurface(a, c, p) + getSurface(b, c, p));
}


