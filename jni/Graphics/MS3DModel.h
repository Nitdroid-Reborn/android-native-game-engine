#ifndef __MYMODEL_H__
#define __MYMODEL_H__
#include <stdlib.h>
#include <vector>
#include "ModelGeometry.h"

using namespace std;

/*
class MyModelMaterial
{
public:
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float emissive[4];
	float shinniness;
	float transparency;
	char texture[128];
	char alphamap[128];
//	GLuint textureID;
};
*/
class MS3DModel
{
public:
    void Load(const char*filename, Vertex3D*&vertices, int& numVertices,
              U16*& indices, int& numIndices,
              Mesh*& meshes, int& meshesCount,
              Material*&materials, int& materialsCount);
};


#endif
