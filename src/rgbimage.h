//
//  rgbimage.h
//  Methoden zur Erzeugung von Bitmaps
//
//  @author Justin Falkenstein & Paul Dieterich
//
#pragma once

#include <iostream>
#include "color.h"

class RGBImage
{
public:
    RGBImage( unsigned int width, unsigned height);
    ~RGBImage();
    void setPixelColor( unsigned int x, unsigned int y, const Color& c);
    const Color& getPixelColor( unsigned int x, unsigned int y) const;
    bool saveToDisk( const char* filename);
    unsigned int width() const;
    unsigned int height() const;
	static RGBImage& GaussFilter(RGBImage& dst, const RGBImage& src, float factor = 1.0f);
    static RGBImage& SobelFilter(RGBImage& dst, const RGBImage& src, float factor = 1.0f);
    static unsigned char convertColorChannel( float f);
protected:
    Color* m_Image;
    unsigned int m_Height;
    unsigned int m_Width;
    
    
};
