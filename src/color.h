//
//  Color.h
//  Farbdarstellung in C++. Note: Ist nicht mehr die Version aus dem Praktikum
//  Ich habe ein paar statische Methoden hinzugefügt und die Color um einen Transparenz-Wert erweitert
//
//  @author Justin Falkenstein & Paul Dieterich
//
#pragma once
#include <iostream>

class Color
{
public:
    float R;
    float G;
    float B;
    float a;

    Color();
    Color( float r, float g, float b, float a = 1.0f);
    Color operator*(const Color& c) const;
    Color operator*(const float factor) const;
    Color operator+(const Color& c) const;
    Color& operator+=(const Color& c);

    // Ein paar schnelle Zugriffe
    static Color black() { return Color(0, 0, 0); }
    static Color white() { return Color(1, 1, 1); }
    static Color gold() { return Color(1, 0.85, 0); }
    static Color gray() { return Color(0.5, 0.5, 0.5); }
    static Color blue() { return Color(0, 0.2, 1); }
    static Color green() { return Color(0.2, 1, 0); }
    static Color purple() { return Color(0.5, 0, 0.7); }
    static Color red() { return Color(1, 0, 0); }
    static Color cyan() { return Color(0, 1, 0.9); }
    static Color orange() { return Color(0.8, 0.55, 0); }
    static Color brown() { return Color(0.65, 0.3, 0); }
    
};