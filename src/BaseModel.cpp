//
//  BaseModel.cpp
//  ogl4
//
//  Created by Philipp Lensing on 19.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "BaseModel.h"

BaseModel::BaseModel() : pShader(NULL), DeleteShader(false), ShadowCaster(true), BoundingBox(AABB::unitBox())
{
    Transform.identity();
}

BaseModel::~BaseModel()
{
    if(DeleteShader && pShader)
        delete pShader;
    DeleteShader = false;
    pShader = NULL;
}

void BaseModel::shader( BaseShader* shader, bool deleteOnDestruction )
{
    pShader = shader;
    DeleteShader = deleteOnDestruction;
}

void BaseModel::draw(const BaseCamera& Cam)
{
    if(!pShader) {
        log(WARN, "No shader found!");
        return;
    }
    
    pShader->modelTransform(transform());
    pShader->activate(Cam);
    
}

LineModel::LineModel(const Vector a, const Vector b)
{
    VB.begin();

    VB.addVertex(a);
    VB.addVertex(b);
  
    VB.end();
}

void LineModel::draw(const BaseCamera& cam)
{
 
    BaseModel::draw(cam);

    VB.activate();

    glDrawArrays(GL_LINES, 0, VB.vertexCount());

    VB.deactivate();
}


LinePlaneModel::LinePlaneModel(float DimX, float DimZ, int NumSegX, int NumSegZ)
{
    VB.begin();

    float StepX = DimX / (float)NumSegX;
    float StepZ = DimZ / (float)NumSegZ;

    float BeginX = -DimZ / 2.0f;
    float BeginZ = -DimX / 2.0f;

    for (int i = 0; i <= NumSegX; ++i)
    {
        VB.addVertex(BeginZ + i * StepX, 0, BeginX);
        VB.addVertex(BeginZ + i * StepX, 0, -BeginX);
    }
    for (int i = 0; i <= NumSegZ; ++i)
    {
        VB.addVertex(BeginZ, 0, BeginX + i * StepZ);
        VB.addVertex(-BeginZ, 0, BeginX + i * StepZ);
    }

    VB.end();

}
void LinePlaneModel::draw(const BaseCamera& Cam)
{
    BaseModel::draw(Cam);

    VB.activate();

    glDrawArrays(GL_LINES, 0, VB.vertexCount());

    VB.deactivate();
}

LineBoxModel::LineBoxModel(Vector origin): LineBoxModel(origin.X, origin.Y, origin.Z) {

}

LineBoxModel::LineBoxModel(float Width, float Height, float Depth)
{
    VB.begin();

    // Width - Depth umgekehrt ?? 

    float BeginX = Width / 2.0f;
    float BeginY = Height / 2.0f;
    float BeginZ = Depth / 2.0f;

    // Pro seite 
    for (int i = 0; i < 2; i++) {

        for (int j = 0; j < 2; j++) {

            // In X Richtung
            VB.addVertex(BeginX, BeginY - j * Height, BeginZ - i * Depth);
            VB.addVertex(-BeginX, BeginY - j * Height, BeginZ - i * Depth);

            // In Y Richtung
            VB.addVertex(BeginX - j * Width, BeginY, BeginZ - i * Depth);
            VB.addVertex(BeginX - j * Width, -BeginY, BeginZ - i * Depth);

            // In Z Richtung
            VB.addVertex(BeginX - i * Width, BeginY - j * Height, BeginZ);
            VB.addVertex(BeginX - i * Width, BeginY - j * Height, -BeginZ);
        }

    }

    VB.end();

}
void LineBoxModel::draw(const BaseCamera& Cam)
{
    BaseModel::draw(Cam);

    VB.activate();

    glDrawArrays(GL_LINES, 0, VB.vertexCount());

    VB.deactivate();

}

TriangleBoxModel::TriangleBoxModel(float Width, float Height, float Depth)
{
    float BeginX = Width / 2.0f;
    float BeginY = Height / 2.0f;
    float BeginZ = Depth / 2.0f;

    // Vektoren
    Vector v0 = Vector(-BeginX, -BeginY, -BeginZ);
    Vector v1 = Vector(BeginX, -BeginY, -BeginZ);
    Vector v2 = Vector(-BeginX, -BeginY, BeginZ);
    Vector v3 = Vector(BeginX, -BeginY, BeginZ);
    Vector v4 = Vector(-BeginX, BeginY, -BeginZ);
    Vector v5 = Vector(BeginX, BeginY, -BeginZ);
    Vector v6 = Vector(-BeginX, BeginY, BeginZ);
    Vector v7 = Vector(BeginX, BeginY, BeginZ);

    VB.begin();

    drawPlane(v0, v4, v6, v2, Vector(-1, 0, 0)); // left
    drawPlane(v3, v7, v5, v1, Vector(1, 0, 0)); // right

    drawPlane(v1, v5, v4, v0, Vector(0, 0, -1)); // front
    drawPlane(v2, v6, v7, v3, Vector(0, 0, 1)); // back

    drawPlane(v7, v6, v4, v5, Vector(0, 1, 0)); // top
    drawPlane(v0, v2, v3, v1, Vector(0, -1, 0)); // bot

    VB.end();

    // Im Uhrzeigersinn von jeder Fläche beide Dreiecke zeichnen
    IB.begin();

    for (int i = 0; i < 6; i++) {

        int index = i * 4;
        IB.addIndex(index);
        IB.addIndex(index + 3);
        IB.addIndex(index + 2);

        IB.addIndex(index);
        IB.addIndex(index + 2);
        IB.addIndex(index + 1);
    }


    IB.end();


}
void TriangleBoxModel::drawPlane(const Vector& v0, const Vector& v1, const Vector& v2, const Vector& v3, const Vector& normal) {
    VB.addNormal(normal);
    VB.addTexcoord0(0, 1);
    VB.addVertex(v0);
    VB.addTexcoord0(0, 0);
    VB.addVertex(v1);
    VB.addTexcoord0(1, 0);
    VB.addVertex(v2);
    VB.addTexcoord0(1, 1);
    VB.addVertex(v3);
}
void TriangleBoxModel::draw(const BaseCamera& Cam)
{
    BaseModel::draw(Cam);

    VB.activate();
    IB.activate();

    glDrawElements(GL_TRIANGLES, IB.indexCount(), IB.indexFormat(), NULL);

    IB.deactivate();
    VB.deactivate();

}

TriangleSphereModel::TriangleSphereModel(float Radius, int Stacks, int Slices)
{
    VB.begin();
    for (int i = 0; i < Stacks; ++i)
        for (int j = 0; j < Slices; ++j)
        {
            float phi = (float)j * (float)M_PI * 2.0f / (float)(Slices - 1);
            float theta = (float)i * (float)M_PI / (float)(Stacks - 1);
            float x = Radius * sin(phi) * sin(theta);
            float z = Radius * cos(phi) * sin(theta);
            float y = Radius * cos(theta);
            VB.addNormal(Vector(x, y, z).normalize());
            VB.addTexcoord0(phi / ((float)M_PI * 2.0f), theta / (float)M_PI);
            VB.addTexcoord1((float)Slices * phi / ((float)M_PI * 2.0f), (float)Stacks * (theta / (float)M_PI));
            VB.addVertex(x, y, z);
        }
    VB.end();

    IB.begin();
    for (int i = 0; i < Stacks - 1; ++i)
        for (int j = 0; j < Slices - 1; ++j)
        {
            IB.addIndex(i * Slices + j + 1);
            IB.addIndex(i * Slices + j);
            IB.addIndex((i + 1) * Slices + j);

            IB.addIndex((i + 1) * Slices + j);
            IB.addIndex((i + 1) * Slices + j + 1);
            IB.addIndex(i * Slices + j + 1);
        }

    IB.end();
}
void TriangleSphereModel::draw(const BaseCamera& Cam)
{
    BaseModel::draw(Cam);

    VB.activate();
    IB.activate();
    glDrawElements(GL_TRIANGLES, IB.indexCount(), IB.indexFormat(), 0);
    IB.deactivate();
    VB.deactivate();
}
