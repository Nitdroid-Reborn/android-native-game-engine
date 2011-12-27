#include "MS3DModel.h"
#include <FileIO/IFileIO.h>
#include <string.h>
#include <ContentManager/IContentManager.h>
#include <FileIO/BufferStream.h>
#define PI 3.14156

void MS3DModel::Load(const char *filename,
                     Vertex3D *&vertices, int &numVertices,
                     U16 *&indices, int &numIndices,
                     Mesh *&meshes, int &meshesCount,
                     Material *&materials, int &materialsCount)
{
    IFileIO* io = IFileIO::get();
    EngineFileHandle fHandle;
    U32 fSize = io->GetSize(filename);
    if(fSize==0)return;

    U8 *buffer = new U8[fSize];
    io->Read(filename, buffer, fSize);
    BufferStream stream(buffer, fSize);
    //io->OpenFile(filename, "rb", fHandle);

    int i;
    char tempChar;
    char tempString[40];
	

    short numV;


    Vertex3D* tempVertices;
    short numTriangles;

    //Read ms3d header
    //====================
    char id[11];
    int version;
    memset(id, '\0', 11);
    stream.read(id, sizeof(char)*10);
    stream.read(&version, sizeof(int));
    //io->ReadFromFile(fHandle, id, sizeof(char)*10);
    //io->ReadFromFile(fHandle, &version, sizeof(int));

    if(strcmp(id, "MS3D000000")!=0 || version!=4)
    {
        delete[] buffer;
        Logger::Log(2, "Not a milkshape file");
        return;
    }


    //====================

    //Read vertices data
    //====================
    stream.read(&numV, sizeof(short));
    //io->ReadFromFile(fHandle, &numV, sizeof(short));
    //fread(&numVertices, sizeof(short), 1,pf);


    tempVertices = new Vertex3D[numV];

	
    for(i=0;i<numV;i++)
    {
        stream.read(tempString, sizeof(char));
        stream.read(tempVertices[i].position, sizeof(float)*3);
        stream.read(&tempChar, sizeof(char));
        stream.read(tempString, sizeof(char));
        //io->ReadFromFile(fHandle, tempString, sizeof(char));
        //io->ReadFromFile(fHandle, tempVertices[i].position, sizeof(float)*3);
        //io->ReadFromFile(fHandle, &tempChar, sizeof(char));
        //io->ReadFromFile(fHandle, tempString, sizeof(char));
    }

    //====================

    //Read triangles data
    //====================
    stream.read(&numTriangles, sizeof(short));
    //io->ReadFromFile(fHandle, &numTriangles, sizeof(short));

    indices = new U16[numTriangles*3];
    vertices = new Vertex3D[numTriangles*3];
    numVertices = numTriangles*3;
    numIndices = numTriangles*3;


    //triangles = new MyModelTriangle[numTriangles];


    short indic[3];
    float norm[9];
    float u[3];
    float v[3];
    char group;

    for(i=0;i<numTriangles;i++)
    {
        stream.read(tempString, sizeof(short));
        stream.read(indic, sizeof(short)*3);
        stream.read(norm, sizeof(float)*9);
        stream.read(u, sizeof(float)*3);
        stream.read(v, sizeof(float)*3);
        stream.read(&group, sizeof(char));
        stream.read(tempString, sizeof(char));


        /*io->ReadFromFile(fHandle, tempString, sizeof(short));
        io->ReadFromFile(fHandle, indic, sizeof(short)*3);
        io->ReadFromFile(fHandle, norm, sizeof(float)*9);
        io->ReadFromFile(fHandle, u, sizeof(float)*3);
        io->ReadFromFile(fHandle, v, sizeof(float)*3);
        io->ReadFromFile(fHandle, &group, sizeof(char));
        io->ReadFromFile(fHandle, tempString, sizeof(char));*/

        for(int j=0;j<3;j++) {
            tempVertices[indic[j]].texCoords[0]=u[j];
            tempVertices[indic[j]].texCoords[1]=1-v[j];
            tempVertices[indic[j]].normal[0]=norm[j*3];
            tempVertices[indic[j]].normal[1]=norm[j*3+1];
            tempVertices[indic[j]].normal[2]=norm[j*3+2];
            tempVertices[indic[j]].color[0]=255;
            tempVertices[indic[j]].color[1]=255;
            tempVertices[indic[j]].color[2]=255;
            tempVertices[indic[j]].color[3]=255;

            vertices[i*3+j]=tempVertices[indic[j]];
            indices[i*3+j]=i*3+j;
        }

    }
    delete[] tempVertices;

    //====================

    //Read meshes data
    //====================

    short numMeshes;
    stream.read(&numMeshes, sizeof(short));
    //io->ReadFromFile(fHandle, &numMeshes, sizeof(short));

    meshesCount = numMeshes;
    meshes = new Mesh[meshesCount];

    short numTris;
    char materialIndex;
    for(i=0;i<meshesCount;i++)
    {
        //io->ReadFromFile(fHandle, tempString, sizeof(char)*33);
        //io->ReadFromFile(fHandle, &numTris, sizeof(short));
        stream.read(tempString, sizeof(char)*33);
        stream.read(&numTris, sizeof(short));
        unsigned short* trisIndices = new unsigned short[numTris];
        stream.read(trisIndices, sizeof(short)*numTris);
        stream.read(&materialIndex, sizeof(char));
        //io->ReadFromFile(fHandle, trisIndices, sizeof(short)*numTris);
        //io->ReadFromFile(fHandle, &materialIndex, sizeof(char));

        meshes[i].startIndex = trisIndices[0]*3;
        meshes[i].indicesCount = numTris*3;
        meshes[i].materialIndex = materialIndex;

        delete[] trisIndices;
    }

    //====================

    //Read materials data
    //====================

    short numMaterials;
    //io->ReadFromFile(fHandle, &numMaterials, sizeof(short));
    stream.read(&numMaterials, sizeof(short));
    materialsCount = numMaterials;

    materials = new Material[numMaterials];

    for(i=0;i<materialsCount;i++)
    {
        char texName[128];
        memset(texName, '\0', 128);
        stream.read(tempString, sizeof(char)*32);
        stream.read(materials[i].ambient, sizeof(float)*4);
        stream.read(materials[i].diffuse, sizeof(float)*4);
        stream.read(materials[i].specular, sizeof(float)*4);
        stream.read(materials[i].emissive, sizeof(float)*4);
        stream.read(&materials[i].shinniness, sizeof(float));
        stream.read(&materials[i].transparency, sizeof(float));
        stream.read(tempString, sizeof(char));
        stream.read(texName, sizeof(char)*128);


        /*io->ReadFromFile(fHandle, tempString, sizeof(char)*32);
        io->ReadFromFile(fHandle, materials[i].ambient, sizeof(float)*4);
        io->ReadFromFile(fHandle, materials[i].diffuse, sizeof(float)*4);
        io->ReadFromFile(fHandle, materials[i].specular, sizeof(float)*4);
        io->ReadFromFile(fHandle, materials[i].emissive, sizeof(float)*4);
        io->ReadFromFile(fHandle, &materials[i].shinniness, sizeof(float));
        io->ReadFromFile(fHandle, &materials[i].transparency, sizeof(float));
        io->ReadFromFile(fHandle, tempString, sizeof(char));
        io->ReadFromFile(fHandle, texName, sizeof(char)*128);*/

        if(strcmp(texName, "")!=0) {
            std::string fullTexPath;
            if(filename[0]==':')
                fullTexPath.append(":textures/");
            else
                fullTexPath.append("textures/");

            fullTexPath.append(texName);

            materials[i].texture = IContentManager::get()->GetTextureManager()->GetTexture(fullTexPath.c_str());
        }

        stream.read(texName, sizeof(char)*128);
        //io->ReadFromFile(fHandle, texName, sizeof(char)*128);
    }

    delete[] buffer;
    //io->CloseFile(fHandle);
}


