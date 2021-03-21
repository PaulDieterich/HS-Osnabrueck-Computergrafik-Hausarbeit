//
//  vector.h
//  Stellt einen Punkt im 3D-Raum dar. Note: Nicht mehr die Version aus dem Praktikum!
//  Es sind einige neue Operator�berladungen und Funktionen f�r Berechnungen wie der Lerp vorhanden!
//
//  @author Justin Falkenstein
//
#pragma once
#include <iostream>

class Vector
{
public:
    float X;
    float Y;
    float Z;
    
    Vector( float x, float y, float z);
    Vector(float xyz);
    Vector();
    
    /**
     * Berechnet das Skalarprodukt zweier Vektoren
     */
    float dot(const Vector& v) const;

    /**
     * Berechnet das Kreuzprodukt
     */
    Vector cross(const Vector& v) const;

    /**
     * Normalisiert diesen Vektor, indem er durch seine L�nge geteilt wird.
     * Damit erh�lt er die L�nge 1
     */
    Vector& normalize();

    /**
     * Interpoliert diesen Vektor mit dem gegebenen Vektor gleichm��ig auf allen Achsen anhand des Wertes
     */
    Vector& lerp(Vector& v, double x);

    /**
     * Gibt die L�nge des Vektors zur�ck
     */
    float length() const;
    float lengthSquared() const;

    /**
     * Berechnet den Reflektionsvektor
     */
    Vector reflection(const Vector& normal) const;

    /**
     * Bestimmt, ob der Vektor ein Dreieck schneidet
     */
    bool triangleIntersection(const Vector& d, const Vector& a, const Vector& b,
        const Vector& c, float& s) const;

    //
    // Operator-�berladungen
    //
    Vector operator+(const Vector& v) const;
    Vector operator-(const Vector& v) const;
    Vector& operator+=(const Vector& v);
    Vector& operator+=(const float c) const;
    Vector& operator-=(const Vector& v);
    Vector& operator-=(const float c);
    Vector operator*(float c) const;
    Vector operator/(float c) const;
    Vector operator/(Vector v) const;
    Vector operator-() const;
  
};

