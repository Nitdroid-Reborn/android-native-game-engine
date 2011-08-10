#ifndef __MYMODEL_H__
#define __MYMODEL_H__
#include <stdlib.h>
#include <vector>
#include "Math/MathLib.h"

using namespace std;

class MyModelVertex
{
public:
	float position[3];
        //char boneIndex;
};


class MyModelTriangle
{
public:
	unsigned short vertexIndices[3];
	float vertexNormal[3][3];
	float normal[3];
	float s[3];
	float t[3];
	char groupIndex;
};

class MyModelMesh
{
public:
	unsigned short numTriangles;
	unsigned short *trianglesIndices;
	char materialIndex;
};

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
	GLuint textureID;
};


class MyModelKeyFrameRot
{
public:
	float time;
	float rotation[3];
};

class MyModelKeyFramePos
{
public:
	float time;
	float position[3];
};

class MyModelJoint
{
public:
    char            name[32];
    char            parentName[32];
    float           rotation[3];
    float           position[3];
    unsigned short  numKeyFramesRot;
    unsigned short  numKeyFramesTrans;
	int				parent;

	Matrix4x4		matLocalSkeleton;
	Matrix4x4		matGlobalSkeleton;
	Matrix4x4		matLocal;
	Matrix4x4		matGlobal;


    MyModelKeyFrameRot *keyFramesRot;
    MyModelKeyFramePos *keyFramesTrans;
};




class MyModel
{
	Vector3 position;
	Vector3 direction;
	float rotation;
	unsigned short numMeshes;
	unsigned short numTriangles;
	unsigned short numMaterials;
	unsigned short numVertices;
	unsigned short numJoints;

	int animationStartFrame;
	int animationStopFrame;
	bool reverseAnimation;
public:

	float fps;
	int numFrames;
	float currentAnimationFrame;

        MyModelVertex * vertices;
	MyModelTriangle * triangles;
	MyModelMaterial *materials;
	MyModelMesh * meshes;
	MyModelJoint * joints;

        void Load(const char*filename);
	void Render();
	void SetupJoints();
	void RenderSkeleton();
	void InitVertices();
	int FindJointByName(char*);
	void SetFrame(float frame);
	void EvaluateJoint(int joint, float frame);
	vector<int> FindJointByParentName(char*);
	int FindRoot();
	void SetAnimation(int startFrame, int endFrame, bool reverse=false);
	void Update(float dt);

	
	Vector3 getPosition(){return position;}
	void setPosition(Vector3 newPosition){position = newPosition;}
	Vector3 getDirection(){return direction;}
	float getRotation();
	void setRotation(float newRotation);
	MyModel();

        short* indices;
        int numIndices;
	~MyModel();
};


#endif
