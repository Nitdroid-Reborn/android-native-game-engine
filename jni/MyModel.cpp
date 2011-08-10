#include <GLES/gl.h>
#include <GLES/glext.h>
#include <cstdio>
#include "MyModel.h"
#include "FileIO/FileIO.h"
#include "FileIO/BufferStream.h"

#define PI 3.14156


MyModel::MyModel()
{
	numMaterials=0;
	numMeshes=0;
	numTriangles=0;
	numVertices=0;
	numJoints = 0;

	vertices=NULL;
	triangles=NULL;
	meshes=NULL;
	materials=NULL;
	joints = NULL;
	rotation=180;
	position = Vector3(0,0,0);
	direction = Vector3(0,0,1);
}

MyModel::~MyModel()
{
	int i;
	for(i=0;i<numMeshes;i++)delete[] meshes[i].trianglesIndices;

	for(i=0;i<numJoints;i++)
	{
		delete[] joints[i].keyFramesRot;
		delete[] joints[i].keyFramesTrans;
	}

	numMeshes=0;
	if(meshes!=NULL)
	{
		delete[] meshes;
		meshes=NULL;
	}

	numMaterials=0;
	if(materials!=NULL)
	{
		delete[] materials;
		materials=NULL;
	}

	numTriangles=0;
	if(triangles!=NULL)
	{
		delete[] triangles;
		triangles=NULL;
	}

	numVertices=0;
	if(vertices!=NULL)
	{
		delete[] vertices;
		vertices=NULL;
	}

	numJoints = 0;
	if(joints!=NULL)
	{
		delete[] joints;
		joints=NULL;
	}

        delete[] indices;

}



void MyModel::Load(const char* filename)
{
    int len = FileIO::get()->GetAssetSize(filename);
    int* buffer = new int[len];
    int read;
    if(!FileIO::get()->ReadAsset(filename, (U8*)buffer, (U32)len, (U32&)read)) {
        delete[] buffer;
        LOGE("blad readed %d", read);
        return;
    }
    BufferStream sb((U8*)buffer, len);

    int i;
    char tempChar;
    float tempFloat;
    int tempInt;
    char tempString[40];

    //Read ms3d header
    //====================
    char id[11];
    int version;
    memset(id, '\0', 11);
    sb.read((U8*)id, 10);
    sb.read((U8*)&version, sizeof(int));


    if(strcmp(id, "MS3D000000")!=0 || version!=4)
    {
        delete[] buffer;
            return;
    }
    //====================

    //Read vertices data
    //====================
    sb.read((U8*)&numVertices, sizeof(short));

    vertices = new MyModelVertex[numVertices];


    for(i=0;i<numVertices;i++)
    {
        sb.read((U8*)tempString, 1);
        sb.read((U8*)vertices[i].position, sizeof(float)*3);
        sb.read((U8*)tempString, 1);
        sb.read((U8*)tempString, 1);
    }
    //====================

    //Read triangles data
    //====================
    sb.read((U8*)&numTriangles, sizeof(short));
    triangles = new MyModelTriangle[numTriangles];


    for(i=0;i<numTriangles;i++)
    {
        sb.read((U8*)tempString, sizeof(short));
        sb.read((U8*)triangles[i].vertexIndices, sizeof(short)*3);
        sb.read((U8*)triangles[i].vertexNormal, sizeof(float)*9);
        sb.read((U8*)triangles[i].s, sizeof(float)*3);
        sb.read((U8*)triangles[i].t, sizeof(float)*3);
        sb.read((U8*)tempString, 1);
        sb.read((U8*)&triangles[i].groupIndex, 1);
    }
    //====================

    //Read meshes data
    //====================
    sb.read((U8*)&numMeshes, sizeof(short));
    meshes = new MyModelMesh[numMeshes];

    for(i=0;i<numMeshes;i++)
    {
        sb.read((U8*)tempString, 1);
        sb.read((U8*)tempString, 32);
        sb.read((U8*)&meshes[i].numTriangles, sizeof(short));

        meshes[i].trianglesIndices = new unsigned short[meshes[i].numTriangles];
        sb.read((U8*)meshes[i].trianglesIndices, sizeof(short)*meshes[i].numTriangles);
        sb.read((U8*)&meshes[i].materialIndex, 1);
    }

    LOGI("Num vertices %d, num triangles %d, num meshes %d", numVertices, numTriangles, numMeshes);
	//====================
/*
	//Read materials data
	//====================
	fread(&numMaterials, sizeof(short), 1, pf);
	materials = new MyModelMaterial[numMaterials];

	for(i=0;i<numMaterials;i++)
	{
		fread(tempString, sizeof(char), 32, pf);
		fread(materials[i].ambient, sizeof(float), 4, pf);
		fread(materials[i].diffuse, sizeof(float), 4, pf);
		fread(materials[i].specular, sizeof(float), 4, pf);
		fread(materials[i].emissive, sizeof(float), 4, pf);
		fread(&materials[i].shinniness, sizeof(float), 1, pf);
		fread(&materials[i].transparency, sizeof(float), 1, pf);
		fread(tempString, sizeof(char), 1, pf); //mode
		fread(materials[i].texture, sizeof(char), 128, pf);
		fread(materials[i].alphamap, sizeof(char), 128, pf);

		materials[i].textureID=250;

                if(strcmp(materials[i].texture, "")!=0)
		{
			materials[i].textureID = LoadGLTexture(materials[i].texture);
                }
        }
	//====================

	//Read joints data
	//====================
	fread(&fps, sizeof(float), 1, pf);
	fread(&tempFloat, sizeof(float), 1, pf);
	fread(&numFrames, sizeof(int), 1, pf);

	fread(&numJoints, sizeof(short), 1, pf);
	joints = new MyModelJoint[numJoints];

	for(i=0;i<numJoints;i++)
	{
		fread(tempString, sizeof(char), 1, pf);
		fread(joints[i].name, sizeof(char), 32, pf);
		fread(joints[i].parentName, sizeof(char), 32, pf);
		fread(joints[i].rotation, sizeof(float), 3, pf);
		fread(joints[i].position, sizeof(float), 3, pf);
		fread(&joints[i].numKeyFramesRot, sizeof(short), 1, pf);
		fread(&joints[i].numKeyFramesTrans, sizeof(short), 1, pf);

		joints[i].keyFramesRot = new MyModelKeyFrameRot[joints[i].numKeyFramesRot];
		joints[i].keyFramesTrans = new MyModelKeyFramePos[joints[i].numKeyFramesTrans];

		for(int k=0;k<joints[i].numKeyFramesRot;k++)
		{
			fread(&joints[i].keyFramesRot[k].time, sizeof(float), 1, pf);
			fread(joints[i].keyFramesRot[k].rotation, sizeof(float), 3, pf);
		}

		for(int k=0;k<joints[i].numKeyFramesTrans;k++)
		{
			fread(&joints[i].keyFramesTrans[k].time, sizeof(float), 1, pf);
			fread(joints[i].keyFramesTrans[k].position, sizeof(float), 3, pf);
		}



	}
	//====================

	fclose(pf);
	SetupJoints();
        SetFrame(0.0);*/



 //   delete[] buffer;

    numIndices=0;
    for(int i=0;i<numMeshes;i++) {
        numIndices+=meshes[i].numTriangles*3;
    }

    LOGI("num indices %d", numIndices);

    indices = new short[numIndices];
    int indicesIndex=0;

    for(int i=0;i<numMeshes;i++) {

        for(int j=0;j<meshes[i].numTriangles;j++) {
            indices[indicesIndex++]=triangles[meshes[i].trianglesIndices[j]].vertexIndices[0];
            indices[indicesIndex++]=triangles[meshes[i].trianglesIndices[j]].vertexIndices[1];
            indices[indicesIndex++]=triangles[meshes[i].trianglesIndices[j]].vertexIndices[2];
        }
    }
}


void MyModel::Render()
{
        //for(int i=0; i<numMeshes;i++)
        //{
                /*int materialIndex = meshes[i].materialIndex;
		if(materialIndex >= 0)
		{
			glMaterialfv(GL_FRONT, GL_AMBIENT, materials[materialIndex].ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, materials[materialIndex].diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, materials[materialIndex].specular);
			//glMaterialfv(GL_FRONT, GL_EMISSION, materials[materialIndex].emissive);
			glMaterialf(GL_FRONT, GL_SHININESS, materials[materialIndex].shinniness);

			glBindTexture(GL_TEXTURE_2D, materials[materialIndex].textureID);
			glEnable(GL_TEXTURE_2D);
			
                }*/


                glEnableClientState(GL_VERTEX_ARRAY);
                glVertexPointer(3, GL_FLOAT, 0, vertices);
                glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, indices);

                glDisableClientState(GL_VERTEX_ARRAY);
\
                /*glBegin(GL_TRIANGLES);
		{
			for(int j=0;j<meshes[i].numTriangles;j++)
			{

				int triangleIndex = meshes[i].trianglesIndices[j];
				const MyModelTriangle* tri = &triangles[triangleIndex];

				for(int k=0; k<3; k++)
				{
					
					int index = tri->vertexIndices[k];
					const MyModelJoint *joint = &joints[vertices[index].boneIndex];
					Vector3 normal;

					normal.x = tri->vertexNormal[k][0];
					normal.y = tri->vertexNormal[k][1];
					normal.z = tri->vertexNormal[k][2];

					joints[vertices[index].boneIndex].matGlobalSkeleton.InverseRotateVector3D(normal);
					joints[vertices[index].boneIndex].matGlobal.RotateVector3D(normal);

					glNormal3f(normal.x, normal.y, normal.z);

					glTexCoord2f(tri->s[k],1-tri->t[k]);

					Vector3 position;

					position.x = vertices[index].position[0];
					position.y = vertices[index].position[1];
					position.z = vertices[index].position[2];

					Vector3 temp, vert;
					temp = position;
					joints[vertices[index].boneIndex].matGlobalSkeleton.InverseTranslateVector3D(temp);
					joints[vertices[index].boneIndex].matGlobalSkeleton.InverseRotateVector3D(temp);
					vert = joint->matGlobal * temp;

					glVertex3f(vert.x, vert.y, vert.z);
				}
			}
		}
		glEnd();
                glDisable(GL_TEXTURE_2D);*/
        //}
}

int MyModel::FindJointByName(char*name)
{
	for(int i=0;i<numJoints;i++)
	{
		if(strcmp(joints[i].name,name)==0)return i;
	}

	return -1;
}

vector<int> MyModel::FindJointByParentName(char*name)
{
	vector<int> result;
	for(int i=0;i<numJoints;i++)
	{
		if(strcmp(joints[i].parentName,name)==0)result.push_back(i);
	}

	return result;
}

int MyModel::FindRoot()
{
	for(int i=0;i<numJoints;i++)
	{
		if(strcmp(joints[i].parentName,"")==0)return i;
	}
	return -1;
}

void MyModel::SetupJoints()
{
	for(int i=0;i<numJoints;i++)
	{
		joints[i].parent = FindJointByName(joints[i].parentName);
	}

	for (int i = 0; i < numJoints; i++)
	{
		joints[i].matLocalSkeleton.SetRotationEuler(joints[i].rotation[0], joints[i].rotation[1], joints[i].rotation[2]);
		joints[i].matLocalSkeleton.entries[12] = joints[i].position[0];
		joints[i].matLocalSkeleton.entries[13] = joints[i].position[1];
		joints[i].matLocalSkeleton.entries[14] = joints[i].position[2];
		
		if (joints[i].parent == -1)
		{
			joints[i].matGlobalSkeleton = joints[i].matLocalSkeleton;
		}
		else
		{
			joints[i].matGlobalSkeleton = joints[joints[i].parent].matGlobalSkeleton * joints[i].matLocalSkeleton;
		}


	}
}

void MyModel::SetFrame(float frame)
{
	if (frame < 0.0f)
	{
		for (int i = 0; i < numJoints; i++)
		{
			joints[i].matLocal = joints[i].matLocalSkeleton;
			joints[i].matGlobal = joints[i].matGlobalSkeleton;
		}
	}
	else
	{
		for (int i = 0; i < numJoints; i++)
		{
			EvaluateJoint(i, frame);
		}
	}

}


void MyModel::EvaluateJoint(int index, float frame)
{
	MyModelJoint *joint = &joints[index];


	int lastRot=0;
	int nextRot=0;
	for(int i=0;i<joints[index].numKeyFramesRot;i++)
	{
		if(joints[index].keyFramesRot[i].time>frame)
		{
			nextRot=i;
			break;
		}
		lastRot=i;
	}

	int lastPos=0;
	int nextPos=0;
	for(int i=0;i<joints[index].numKeyFramesTrans;i++)
	{
		if(joints[index].keyFramesTrans[i].time>frame)
		{
			nextPos=i;
			break;
		}
		lastPos=i;
	}


	Vector3 newRotation;
	Vector3 newPosition;

	double scale = (frame-joints[index].keyFramesRot[lastRot].time)/(joints[index].keyFramesRot[nextRot].time-joints[index].keyFramesRot[lastRot].time);

    newRotation.x = joints[index].keyFramesRot[lastRot].rotation[0] + scale *(joints[index].keyFramesRot[nextRot].rotation[0]-joints[index].keyFramesRot[lastRot].rotation[0]);
	newRotation.y = joints[index].keyFramesRot[lastRot].rotation[1] + scale *(joints[index].keyFramesRot[nextRot].rotation[1]-joints[index].keyFramesRot[lastRot].rotation[1]);
	newRotation.z = joints[index].keyFramesRot[lastRot].rotation[2] + scale *(joints[index].keyFramesRot[nextRot].rotation[2]-joints[index].keyFramesRot[lastRot].rotation[2]);

	scale = (frame-joints[index].keyFramesTrans[lastPos].time)/(joints[index].keyFramesTrans[nextPos].time-joints[index].keyFramesTrans[lastPos].time);
	newPosition.x = joints[index].keyFramesTrans[lastPos].position[0] + scale *(joints[index].keyFramesTrans[nextPos].position[0]-joints[index].keyFramesTrans[lastPos].position[0]);
	newPosition.y = joints[index].keyFramesTrans[lastPos].position[1] + scale *(joints[index].keyFramesTrans[nextPos].position[1]-joints[index].keyFramesTrans[lastPos].position[1]);
	newPosition.z = joints[index].keyFramesTrans[lastPos].position[2] + scale *(joints[index].keyFramesTrans[nextPos].position[2]-joints[index].keyFramesTrans[lastPos].position[2]);

	Matrix4x4 animation;
	animation.SetRotationEuler(newRotation.x, newRotation.y, newRotation.z);

	animation.entries[12] = newPosition.x;
	animation.entries[13] = newPosition.y;
	animation.entries[14] = newPosition.z;


	joint->matLocal = joint->matLocalSkeleton * animation;
	
	// build up the hierarchy if joints
	// matGlobal = matGlobal(parent) * matLocal
	if (joint->parent == -1)
	{
		joint->matGlobal = joint->matLocal;
	}
	else
	{
		joint->matGlobal = joints[joint->parent].matGlobal * joint->matLocal;
	}
}

void MyModel::RenderSkeleton()
{
	glPointSize(5);
	glPushMatrix();

	for(int i=0;i<numJoints;i++)
	{
		Vector3 point(0,0,0);

		point=joints[i].matGlobalSkeleton*point;
		point=joints[i].matGlobal*point;

/*		glBegin(GL_POINTS);
		
			glVertex3f(point.x, point.y, point.z);
		
                glEnd();*/

	}
	glPopMatrix();
}

float MyModel::getRotation()
{
	return rotation;
}

void MyModel::setRotation(float newRotation)
{
	rotation = newRotation;
	direction = Vector3(0,0,1);
	direction.RotateY(newRotation-180);
}


void MyModel::SetAnimation(int startFrame, int endFrame, bool reverse)
{
	if(animationStartFrame!=startFrame && animationStopFrame!=endFrame)
	{
			animationStartFrame=startFrame;
			animationStopFrame=endFrame;
			currentAnimationFrame=startFrame;

			reverseAnimation = reverse;
	}
}
void MyModel::Update(float dt)
{
	float factor = (float(numFrames)/fps);
	if(reverseAnimation)
	{
		currentAnimationFrame-=(dt*factor);

		if(currentAnimationFrame<animationStartFrame)currentAnimationFrame=animationStopFrame;
	}
	else
	{
		currentAnimationFrame+=(dt*factor);

		if(currentAnimationFrame>animationStopFrame)currentAnimationFrame=animationStartFrame;
	}

	SetFrame(currentAnimationFrame/fps);
}
