//
//  Model.hpp
//  ogl4
//
//  Created by Philipp Lensing on 21.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#pragma once

#include <stdio.h>
#include "basemodel.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "texture.h"
#include "aabb.h"
#include <string>
#include "phongshader.h"
#include "ParticleEmitter.h"
#include <list>
#include <sstream>

using namespace std;

class Model : public BaseModel
{
public:
    Model();
    Model(const char* ModelFile, bool FitSize=true);
    virtual ~Model();

    bool load(const char* ModelFile, bool FitSize=true);
    virtual void draw(const BaseCamera& Cam);
    virtual const bool collides(const BaseModel* b, float tolerance = 0.0f);
    virtual const bool collides(ParticleEmitter* p);
    virtual const bool collides(const Vector& v, float tolerance = 1.5f);
    const AABB& boundingBox() const { return BoundingBox; }
    const Matrix& transform() const { return BaseModel::transform(); }
    void transform(const Matrix& m);
protected: // protected types
    struct Mesh
    {
        Mesh()  {}
        VertexBuffer VB;
        IndexBuffer IB;
        int MaterialIdx;
    };
    struct Material
    {
        Material() : DiffTex(NULL), DiffColor(1,1,1),SpecColor(0.3f,0.3f,0.3f), AmbColor(0,0,0), SpecExp(10) {}
        Color DiffColor;
        Color SpecColor;
        Color AmbColor;
        float SpecExp;
        const Texture* DiffTex;
		const Texture* NormalTex;
    };
    struct Node
    {
        Node() : Parent(NULL), Children(NULL), ChildCount(0), MeshCount(0), Meshes(NULL) {}
        Matrix Trans;
        Matrix GlobalTrans;
        int* Meshes;
        unsigned int MeshCount;
        Node* Parent;
        Node* Children;
        unsigned int ChildCount;
        std::string Name;
    };
    
protected: // protected methods
    void loadMeshes(const aiScene* pScene, bool FitSize);
    void loadMaterials(const aiScene* pScene);
    void calcBoundingBox( const aiScene* pScene, AABB& Box);

    void loadNodes(const aiScene* pScene);
    void copyNodesRecursive(const aiNode* paiNode, Node* pNode);
    Matrix convert(const aiMatrix4x4& m);
    void applyMaterial( unsigned int index);
    void deleteNodes(Node* pNode);

protected: // protected member variables
    Mesh* pMeshes;
    unsigned int MeshCount;
    Material* pMaterials;
    unsigned int MaterialCount;
    
    std::string Filepath; // stores pathname and filename
    std::string Path; // stores path without filename
    Node RootNode;

};

