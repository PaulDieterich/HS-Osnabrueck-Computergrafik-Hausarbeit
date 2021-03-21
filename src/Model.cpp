//
//  Model.cpp
//  ogl4
//
//  Created by Philipp Lensing on 21.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "Model.h"
#include "ParticleEmitter.h"


Model::Model() : pMeshes(NULL), MeshCount(0), pMaterials(NULL), MaterialCount(0)
{
    
}
Model::Model(const char* ModelFile, bool FitSize) : pMeshes(NULL), MeshCount(0), pMaterials(NULL), MaterialCount(0)
{
    bool ret = load(ModelFile, FitSize);
    if(!ret)
		throw std::exception();
 }

Model::~Model()
{
	delete[] pMeshes;
	for (unsigned int i = 0; i < MaterialCount; i++)
	{
		Texture::ReleaseShared(pMaterials->DiffTex);
	}
	delete[] pMaterials;
    deleteNodes(&RootNode);
}

void Model::deleteNodes(Node* pNode)
{
    if(!pNode)
        return;
    for(unsigned int i=0; i<pNode->ChildCount; ++i)
        deleteNodes(&(pNode->Children[i]));
    if(pNode->ChildCount>0)
        delete [] pNode->Children;
    if(pNode->MeshCount>0)
        delete [] pNode->Meshes;
}

bool Model::load(const char* ModelFile, bool FitSize)
{
    const aiScene* pScene = aiImportFile( ModelFile,aiProcessPreset_TargetRealtime_Fast | aiProcess_TransformUVCoords );
    
    if(pScene==NULL || pScene->mNumMeshes<=0)
        return false;
    
    Filepath = ModelFile;
    Path = Filepath;
    size_t pos = Filepath.rfind('/');
    if(pos == std::string::npos)
        pos = Filepath.rfind('\\');
    if(pos !=std::string::npos)
        Path.resize(pos+1);
    
    loadMeshes(pScene, FitSize);
    loadMaterials(pScene);
    loadNodes(pScene);
    
    return true;
}

void Model::loadMeshes(const aiScene* pScene, bool FitSize)
{
	if (!pScene->HasMeshes()) return;

	MeshCount = pScene->mNumMeshes;
	pMeshes = new Mesh[MeshCount];
	
	this->calcBoundingBox(pScene, BoundingBox);
	float scale = 1;
	if (FitSize) {
		Vector bBMax = BoundingBox.Max;
		Vector bBMin = BoundingBox.Min;

		float x = fmax(bBMax.X, abs(bBMin.X));
		float y = fmax(bBMax.Y, abs(bBMin.Y));
		float z = fmax(bBMax.Z, abs(bBMin.Z));

		float maxDist = fmax(fmax(x, y), z);
		scale = 5.0f / 2.0f / maxDist;
	}

	for (unsigned int meshIndex = 0; meshIndex < pScene->mNumMeshes; meshIndex++)
	{
		// Das ist das Mesh aus der Szene. Dieses müssen wir in ein Mesh Objekt überführen und dann in das Mesh array der Model klasse (pMeshes) überführen
		aiMesh* mesh = pScene->mMeshes[meshIndex];

		VertexBuffer& VB = pMeshes[meshIndex].VB;
		IndexBuffer& IB = pMeshes[meshIndex].IB;

		// Vertexbuffer befüllen
		VB.begin();

		// Add vertex for each in mesh
		for (unsigned int vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++)
		{
			aiVector3D aiVector = mesh->mVertices[vertexIndex];

			// Normalen
			if (mesh->HasNormals()) {
				aiVector3D aiVector = mesh->mNormals[vertexIndex];
				VB.addNormal(aiVector.x, aiVector.y, aiVector.z);
			}

			// Tangenten laden
			if (mesh->HasTangentsAndBitangents()) {
				aiVector3D *tangents = mesh->mTangents;
				aiVector3D *bitangents = mesh->mBitangents;

				int tangentSize = sizeof(tangents) / sizeof(aiVector);
				int bitangentSize = sizeof(bitangents) / sizeof(aiVector);

				for (size_t i = 0; i < tangentSize; i++)
				{	
					VB.addTexcoord1(tangents[i].x, tangents[i].y, tangents[i].z);
				}
				for (size_t i = 0; i < bitangentSize; i++)
				{
					VB.addTexcoord2(bitangents[i].x, bitangents[i].y, bitangents[i].z);
				}

			}

 			// textcoords für jeden Channel
			for (int posTex = 0; posTex < 4; posTex++) {
				if (mesh->HasTextureCoords(posTex)) {
					aiVector3D vec = mesh->mTextureCoords[posTex][vertexIndex];
					Vector tmp = Vector(vec.x, vec.y, vec.z);
					switch (posTex)
					{
					case 0:
						VB.addTexcoord0(tmp.X, -tmp.Y, tmp.Z);
						break;
					case 1:
						VB.addTexcoord1(tmp.X, -tmp.Y, tmp.Z);
						break;
					case 2:
						VB.addTexcoord2(tmp.X, -tmp.Y, tmp.Z);
						break;
					case 3:
						VB.addTexcoord3(tmp.X, -tmp.Y, tmp.Z);
						break;
					default:
						break;
					}
				}
			}
			

			// Vertex hinzufügen
			VB.addVertex(Vector(aiVector.x, aiVector.y, aiVector.z) * scale);

		}

		// End vertex buffer
		VB.end();


		// Index pro Face in dem Mesh anlegen
		IB.begin();
		for (unsigned int faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++)
		{
			aiFace face = mesh->mFaces[faceIndex];
			for (unsigned int i = 0; i < face.mNumIndices; i++)
			{
				IB.addIndex(face.mIndices[i]);
			}
		}
		IB.end();

		pMeshes[meshIndex].MaterialIdx = mesh->mMaterialIndex; // ???
	}

}


void Model::loadMaterials(const aiScene* pScene)
{
	if (pScene->HasMaterials()) {
		MaterialCount = pScene->mNumMaterials;
		pMaterials = new Material[MaterialCount];

		// Für jedes Material der Szene die Texturen in ein Material Objekt laden und in unsere
		// pMaterials überführen
		for (unsigned int i = 0; i < pScene->mNumMaterials; i++)
		{
			aiMaterial* mat = pScene->mMaterials[i]; // Material der Szene
			aiColor3D tmp;

			mat->Get(AI_MATKEY_COLOR_DIFFUSE, tmp);
			pMaterials[i].DiffColor = Color(tmp.r, tmp.g, tmp.b);

			mat->Get(AI_MATKEY_COLOR_AMBIENT, tmp);
			pMaterials[i].AmbColor = Color(tmp.r, tmp.g, tmp.b);

			mat->Get(AI_MATKEY_COLOR_SPECULAR, tmp);
			pMaterials[i].SpecColor = Color(tmp.r, tmp.g, tmp.b);


			// Texturen parsen, laden und in das Material Objekt überführen
			for (unsigned int textureIndex = 0; textureIndex < mat->GetTextureCount(aiTextureType_DIFFUSE); textureIndex++)
			{
				aiString as;
				mat->GetTexture(aiTextureType_DIFFUSE, textureIndex, &as);
				stringstream ss;
				ss << Path << as.data << ends;
				pMaterials[i].DiffTex = Texture::LoadShared(ss.str().c_str());
			}

			// TODO optimize
			for (unsigned int textureIndex = 0; textureIndex < mat->GetTextureCount(aiTextureType_NORMALS); textureIndex++)
			{
				aiString as;
				mat->GetTexture(aiTextureType_NORMALS, textureIndex, &as);
				stringstream ss;
				ss << Path << as.data << ends;
				pMaterials[i].NormalTex = Texture::LoadShared(ss.str().c_str());
			}

			float shine = 0.0f;
			mat->Get(AI_MATKEY_SHININESS, pMaterials[i].SpecExp);
			
		}
	}

}
void Model::calcBoundingBox(const aiScene* pScene, AABB& Box)
{
	Vector min(0.0f, 0.0f, 0.0f);
	Vector max(0.0f, 0.0f, 0.0f);

// Jedes Mesh durchgehen um die maximalen und minimalen X, Y und Z koordinaten berechnen
	for (unsigned int mashNr = 0; mashNr < pScene->mNumMeshes; mashNr++) {
		const aiMesh* mesh = pScene->mMeshes[mashNr];
			//Jeden Vertex durchgehen des meshes durchgehen
		for (unsigned int vertNr = 0; vertNr < mesh->mNumVertices; vertNr++) {
			aiVector3D v = pScene->mMeshes[mashNr]->mVertices[vertNr];
			//minimal vertex Berechnen
			if (v.x < min.X) min.X = v.x;
			if (v.y < min.Y) min.Y = v.y;
			if (v.z < min.Z) min.Z = v.z;
			//maximal vertex Berechnen
			if (v.x > max.X) max.X = v.x;
			if (v.y > max.Y) max.Y = v.y;
			if (v.z > max.Z) max.Z = v.z;

		}
	}
	Box.Min = min;
	Box.Max = max;

}

void Model::loadNodes(const aiScene* pScene)
{
    deleteNodes(&RootNode);
    copyNodesRecursive(pScene->mRootNode, &RootNode);
}

void Model::copyNodesRecursive(const aiNode* paiNode, Node* pNode)
{
    pNode->Name = paiNode->mName.C_Str();
    pNode->Trans = convert(paiNode->mTransformation);
    
    if(paiNode->mNumMeshes > 0)
    {
        pNode->MeshCount = paiNode->mNumMeshes;
        pNode->Meshes = new int[pNode->MeshCount];
        for(unsigned int i=0; i<pNode->MeshCount; ++i)
            pNode->Meshes[i] = (int)paiNode->mMeshes[i];
    }
    
    if(paiNode->mNumChildren <=0)
        return;
    
    pNode->ChildCount = paiNode->mNumChildren;
    pNode->Children = new Node[pNode->ChildCount];
    for(unsigned int i=0; i<paiNode->mNumChildren; ++i)
    {
        copyNodesRecursive(paiNode->mChildren[i], &(pNode->Children[i]));
        pNode->Children[i].Parent = pNode;
    }
}

void Model::applyMaterial( unsigned int index)
{
    if(index>=MaterialCount)
        return;
    
    PhongShader* pPhong = dynamic_cast<PhongShader*>(shader());
    if(!pPhong) {
        return;
    }
    

    Material* pMat = &pMaterials[index];
    pPhong->ambientColor(pMat->AmbColor);
    pPhong->diffuseColor(pMat->DiffColor);
    pPhong->specularExp(pMat->SpecExp);
    pPhong->specularColor(pMat->SpecColor);
    pPhong->diffuseTexture(pMat->DiffTex);
	
	// TODO: das wirft einen Fehler o.O
	//pPhong->normalTexture(pMat->NormalTex);
}

void Model::draw(const BaseCamera& Cam)
{
    if(!pShader) {
        std::cout << "BaseModel::draw() no shader found" << std::endl;
        return;
    }
    pShader->modelTransform(transform());
    
    std::list<Node*> DrawNodes;
    DrawNodes.push_back(&RootNode);
    
    while(!DrawNodes.empty())
    {
        Node* pNode = DrawNodes.front();
        Matrix GlobalTransform;
        
        if(pNode->Parent != NULL)
            pNode->GlobalTrans = pNode->Parent->GlobalTrans * pNode->Trans;
        else
            pNode->GlobalTrans = transform() * pNode->Trans;
        
        pShader->modelTransform(pNode->GlobalTrans);
    
        for(unsigned int i = 0; i<pNode->MeshCount; ++i )
        {
            Mesh& mesh = pMeshes[pNode->Meshes[i]];
            mesh.VB.activate();
            mesh.IB.activate();
            applyMaterial(mesh.MaterialIdx);
            pShader->activate(Cam);
            glDrawElements(GL_TRIANGLES, mesh.IB.indexCount(), mesh.IB.indexFormat(), 0);
            mesh.IB.deactivate();
            mesh.VB.deactivate();
        }
        for(unsigned int i = 0; i<pNode->ChildCount; ++i )
            DrawNodes.push_back(&(pNode->Children[i]));
        
        DrawNodes.pop_front();
    }
}

/**
 * ¯\_(ツ)_/¯
 * Prüft Collision zwischen zwei Modellen über AABBs
 */
const bool Model::collides(const BaseModel* b, float tolerance)
{
	// Positionen der Modelle in der Welt
	Vector posA = Transform.translation();
	Vector posB = b->transform().translation();

	// Größe (Max-Min) der Boundary Boxen der beiden Modelle
	// Da Objekte skaliert sein können muss das noch mit multipliziert werden
	Vector sizeA = BoundingBox.size();
	Vector sizeB = b->boundingBox().size();

	// Collision nur dann, wenn das Objekt alle drei Achsen des anderen Objekts schneidet
	bool colX = posA.X + sizeA.X >= posB.X && posB.X + sizeB.X >= posA.X;
	bool colY = posA.Y + sizeA.Y >= posB.Y && posB.Y + sizeB.Y >= posA.Y;
	bool colZ = posA.Z + sizeA.Z >= posB.Z && posB.Z + sizeB.Z >= posA.Z;
	return colX && colY && colZ;

}

const bool Model::collides(ParticleEmitter* p)
{
	// Der Scheiß Asteroid muss ja zwischen minLoc und maxLoc des Partikels liegen, egal auf welcher Achse.
	// Der Partikel mit min und maxLoc richtet sich ja nach der Ship transformation und dem Forward vector, also sind rotation und so schon drin.
	// Das heißt theoretisch haben wir bei Sichtkontakt zwangsläufig einen direkten durchschnitt
	Vector asteroidPos = this->Transform.translation();
	Vector laser = p->getMaxLoc();

	Vector diff = asteroidPos - laser;

	return diff.length() <= 50;
}


const bool Model::collides(const Vector& b, float tolerance)
{
	// Positionen der Modelle in der Welt
	Vector posA = Transform.translation();

	// Größe (Max-Min) der Boundary Boxen des Modells, etwas vergrößert für einfacheren Hit
	Vector sizeA = BoundingBox.size() * Transform.scale().length() * tolerance;

	// Collision nur dann, wenn das Objekt alle drei Achsen des Punktes schneidet
	bool colX = posA.X + sizeA.X >= b.X && b.X >= posA.X;
	bool colY = posA.Y + sizeA.Y >= b.Y && b.Y >= posA.Y;
	bool colZ = posA.Z + sizeA.Z >= b.Z && b.Z >= posA.Z;
	return colX && colY && colZ;
}

void Model::transform(const Matrix& m)
{
	BaseModel::transform(m);
}

Matrix Model::convert(const aiMatrix4x4& m)
{
    return Matrix(m.a1, m.a2, m.a3, m.a4,
                  m.b1, m.b2, m.b3, m.b4,
                  m.c1, m.c2, m.c3, m.c4,
                  m.d1, m.d2, m.d3, m.d4);
}



