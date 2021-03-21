//
//  BaseModel.hpp
//  ogl4
//
//  Created by Philipp Lensing on 19.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

// Note: Ich habe die Klasse etwas geändert. Die Bounding Box kann besser direkt hier stehen, so kann man
// Auch Models, die direkt von BaseModel erben, mit Collision versehen.
// Die einzelnen BaseModels, die hiervon erben, habe ich ebenfalls hier reingemergt.
#pragma once

#include <stdio.h>
#include "Camera.h"
#include "Matrix.h"
#include "BaseShader.h"
#include "Utility.h"
#include "Aabb.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ConstantShader.h"
#include "Utility.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace Logger;

class BaseModel
{
public:
    BaseModel();
    virtual ~BaseModel();
    virtual void draw(const BaseCamera& Cam);
    virtual const Matrix& transform() const { return Transform; }
    virtual void transform(const Matrix& m) { Transform = m;  }
    virtual void shader( BaseShader* shader, bool deleteOnDestruction=false );
    virtual BaseShader* shader() const { return pShader; }
    virtual const AABB& boundingBox() const { return BoundingBox; }
	bool shadowCaster() const { return ShadowCaster; }
	void shadowCaster(bool sc) { ShadowCaster = sc; }
protected:
    Matrix Transform;
    BaseShader* pShader;
    bool DeleteShader;
	bool ShadowCaster;
    AABB BoundingBox;

};

/**
 * Zeichnet eine simple Linie zwischen zwei Punkten
 */
class LineModel : public BaseModel
{
public:
    LineModel(const Vector a, const Vector b);
    virtual ~LineModel() {}
    virtual void draw(const BaseCamera& cam);
protected:
    VertexBuffer VB;
};

/**
 * Zeichnet eine simple viereckige, linienbasierte Fläche
 */
class LinePlaneModel : public BaseModel
{
public:
    LinePlaneModel(float DimX, float DimZ, int NumSegX, int NumSegZ);
    virtual ~LinePlaneModel() {}
    virtual void draw(const BaseCamera& Cam);
protected:
    VertexBuffer VB;
};

/**
 * Zeichnet eine linienbasierte Box mit angegebener Dimension
 */
class LineBoxModel : public BaseModel
{
public:
    LineBoxModel(Vector origin);
    LineBoxModel(float Width = 1, float Height = 1, float Depth = 1);
    virtual ~LineBoxModel() {}
    virtual void draw(const BaseCamera& Cam);
protected:
    VertexBuffer VB;

};

/**
 * Zeichnet ein auf dreieckigen Flächen basierendes Box Modell
 */
class TriangleBoxModel : public BaseModel
{
public:
    TriangleBoxModel(float Width = 1, float Height = 1, float Depth = 1);
    void drawPlane(const Vector& v0, const Vector& v1, const Vector& v2, const Vector& v3, const Vector& normal);
    virtual ~TriangleBoxModel() {}
    virtual void draw(const BaseCamera& Cam);
protected:
    VertexBuffer VB;
    IndexBuffer IB;
};

/**
 * Zeichnet ein auf dreieckigen Flächen basierendes Kugel Modell
 * Quelle: Phillip Lensing s.o.
 */
class TriangleSphereModel : public BaseModel
{
public:
    TriangleSphereModel(float Radius, int Stacks = 18, int Slices = 36);
    virtual ~TriangleSphereModel() {}
    virtual void draw(const BaseCamera& Cam);
protected:
    VertexBuffer VB;
    IndexBuffer IB;
};
