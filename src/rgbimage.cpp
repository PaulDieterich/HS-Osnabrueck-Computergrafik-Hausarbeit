#include "rgbimage.h"
#include "color.h"
#include "assert.h"
#include "Matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif
using namespace std;
RGBImage::RGBImage( unsigned int width, unsigned int height)
{
    this->m_Width = width;
	this->m_Height = height;
    this->m_Image = new Color[width * height];

}

RGBImage::~RGBImage()
{
    delete this->m_Image;
}


void RGBImage::setPixelColor( unsigned int x, unsigned int y, const Color& c)
{
    if (x > this->width() || y > this->height()) {
        cout << "Warning: setPixelColor could not be executed because one of the coordinates is bigger than the bounds.\n";
        return;
    };

    this->m_Image[y*m_Width+x] = c;
}

const Color& RGBImage::getPixelColor( unsigned int x, unsigned int y) const
{
    if (x > this->width() || y > this->height()) {
        cout << "Warning: getPixelColor: coordinates are than the bounds. Returning closest color.\n";
        return this->m_Image[this->width()*this->height()];
    }
	return this->m_Image[y*this->width()+x];
}

unsigned int RGBImage::width() const
{
	return this->m_Width;
}
unsigned int RGBImage::height() const
{
	return this->m_Height;
}

RGBImage & RGBImage::GaussFilter(RGBImage & dst, const RGBImage & src, float factor)
{
	int height = src.height();
	int width = src.width();

	float K[7] = { 0.006f, 0.061f, 0.242f, 0.383f, 0.242f, 0.061f, 0.006f };

	RGBImage GStrich = RGBImage(width, height);
	Color color(0,0,0);

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			
			for (int i = 0; i < 7; i++)
			{
				// Je Pixel den Gaußfilter anwenden
				int xxx = x - i - 3;
				if (!(xxx <= 0 || xxx > width))
				{
					color += src.getPixelColor(xxx, y) * K[i];
				}
			}
			GStrich.setPixelColor(x, y, color*factor);
		}
	}
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			color = Color(0, 0, 0); // reset
			for (int i = 0; i < 7; i++)
			{
				int yyy = y - i - 3;
				if (!(yyy <= 0 || yyy > height)) {
					color += GStrich.getPixelColor(x, yyy) * K[i];
				}
			}
			dst.setPixelColor(y, x, color*factor);

		}
	}


	dst.saveToDisk(ASSET_DIRECTORY "gausss_mixmap.bmp");
	return dst;

	// I = dst = Eingangsbild (Height-Map)


	// G = src = gefiltertes Ergebnis
}

RGBImage& RGBImage::SobelFilter(RGBImage& dst, const RGBImage& src, float factor)
{
    int height = src.height();
    int width = src.width();

    //create filter-mask
    int K[9] = { 1, 0, -1,
                 2, 0, -2,
                 1, 0, -1 };
    int KT[9] = { 1, 2, 1,
                  0, 0, 0,
                 -1, -2, -1 };

    float U = 0.0f;
    float V = 0.0f;

    for (int y = 1; y < height - 1; y++)
        for (int x = 1; x < width - 1; x++) {

            for (int j = 0; j < 3; j++)
                for (int i = 0; i < 3; i++) {
                    U += src.getPixelColor((x - 1) + i, (y - 1) + j).R * K[i + (j * 3)];
                    V += src.getPixelColor((x - 1) + i, (y - 1) + j).R * KT[i + (j * 3)];
                }

            float f = sqrt(U * U + V * V) * factor;
            dst.setPixelColor(x, y, Color(f, f, f));
            U = 0.0f;
            V = 0.0f;
        }

    dst.saveToDisk(ASSET_DIRECTORY "mixmap-ours.bmp");
    return dst;
}


unsigned char RGBImage::convertColorChannel( float v)
{
	if (v < 0.0) return 0;
	else if (v > 1.0) return 255;

	return v * 255;
}

bool RGBImage::saveToDisk( const char* Filename)
{
    FILE *file = fopen(Filename, "wb");

    // 54 Für Fileheader und Infoheader, 3 pro Pixel für je R, G, B
    const unsigned int filesize = 54 + 3 * this->width() * this->height();

    if (file == NULL) return false;

    // Prepare the file headers
    unsigned char bmpFileHeader[14] = { 'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0 };
    unsigned char bmpInfoHeader[40] = { 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0 };

    // Code von Stackoverflow. TODO
    // write bmpFileHeader
    bmpFileHeader[2] = (unsigned char)(filesize);
    bmpFileHeader[3] = (unsigned char)(filesize>>8);
    bmpFileHeader[4] = (unsigned char)(filesize>>16);
    bmpFileHeader[5] = (unsigned char)(filesize>>24);
    // write bmpInfoHeader
    bmpInfoHeader[4] = (unsigned char)(this->width());
    bmpInfoHeader[5] = (unsigned char)(this->width() >> 8);
    bmpInfoHeader[6] = (unsigned char)(this->width() >> 16);
    bmpInfoHeader[7] = (unsigned char)(this->width() >> 24);
    bmpInfoHeader[8] = (unsigned char)(this->height());
    bmpInfoHeader[9] = (unsigned char)(this->height() >> 8);
    bmpInfoHeader[10] = (unsigned char)(this->height() >> 16);
    bmpInfoHeader[11] = (unsigned char)(this->height() >> 24);

    // Header schreiben
    fwrite(bmpFileHeader, 1, 14, file);
    fwrite(bmpInfoHeader, 1, 40, file);

    // Erst height, dann width, damit das Bild zeilenweise durchlaufen wird
    // Dabei die height umgekehrt entlang gehen, weil sonst die Bilder an der X Achse gespiegelt werden
    for (int i = this->height(); i > 0; i--) {
        for (int j = 0; j < this->width(); j++) {

            // Pixelkanäle konvertieren und in umgekehrter Reihenfolge speichern
            Color pxl = this->getPixelColor(i, j);
            unsigned char color[3] = { 
                convertColorChannel(pxl.B), 
                convertColorChannel(pxl.G), 
                convertColorChannel(pxl.R) };

            fwrite(color, 1, 3, file);
        }
    }


    fclose(file);
    return true;
}