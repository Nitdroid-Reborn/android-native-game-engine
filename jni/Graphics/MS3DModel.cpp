#include "MS3DModel.h"
#include <FileIO/IFileIO.h>
#include <string.h>
#define PI 3.14156

void MS3DModel::Load(const char* filename, Vertex3D*&vertices, int& numV,
                     U16*& indices, int& numI,
                     std::vector<Mesh>& meshesInfo,
                     std::vector<Material>& materialsInfo)
{
    IFileIO* io = IFileIO::get();
    EngineFileHandle fHandle;
    io->OpenFile(filename, "rb", fHandle);

    int i;
    char tempChar;
    char tempString[40];
	

    short numVertices;
    short numTriangles;

    //Read ms3d header
    //====================
    char id[11];
    int version;
    memset(id, '\0', 11);
    io->ReadFromFile(fHandle, id, sizeof(char)*10);
    io->ReadFromFile(fHandle, &version, sizeof(int));

    if(strcmp(id, "MS3D000000")!=0 || version!=4)
    {
        Logger::Log("Not a milkshape file");
        return;
    }


    //====================

    //Read vertices data
    //====================
    io->ReadFromFile(fHandle, &numVertices, sizeof(short));
    //fread(&numVertices, sizeof(short), 1,pf);
	
    vertices = new Vertex3D[numVertices];

	
    for(i=0;i<numVertices;i++)
    {
        io->ReadFromFile(fHandle, tempString, sizeof(char));
        io->ReadFromFile(fHandle, vertices[i].position, sizeof(float)*3);
        io->ReadFromFile(fHandle, &tempChar, sizeof(char));
        io->ReadFromFile(fHandle, tempString, sizeof(char));
    }

    //====================

    //Read triangles data
    //====================
    io->ReadFromFile(fHandle, &numTriangles, sizeof(short));

    indices = new U16[numTriangles*3];
    numI = numTriangles*3;
    //fread(&numTriangles, sizeof(short), 1, pf);

    //triangles = new MyModelTriangle[numTriangles];


    short indic[3];
    float norm[9];
    float u[3];
    float v[3];
    char group;

    for(i=0;i<numTriangles;i++)
    {
        io->ReadFromFile(fHandle, tempString, sizeof(short));
        io->ReadFromFile(fHandle, indic, sizeof(short)*3);
        io->ReadFromFile(fHandle, norm, sizeof(float)*9);
        io->ReadFromFile(fHandle, u, sizeof(float)*3);
        io->ReadFromFile(fHandle, v, sizeof(float)*3);
        io->ReadFromFile(fHandle, &group, sizeof(char));
        io->ReadFromFile(fHandle, tempString, sizeof(char));

        for(int j=0;j<3;j++) {


            vertices[indic[j]].texCoords[0]=u[j];
            vertices[indic[j]].texCoords[1]=1-v[j];
            vertices[indic[j]].normal[0]=norm[j*3];
            vertices[indic[j]].normal[1]=norm[j*3+1];
            vertices[indic[j]].normal[2]=norm[j*3+2];
            vertices[indic[j]].color[0]=255;
            vertices[indic[j]].color[1]=255;
            vertices[indic[j]].color[2]=255;
            vertices[indic[j]].color[3]=255;
            indices[i*3+j]=indic[j];

            if(indic[j]==2)
                Logger::Log("%f %f", vertices[indic[j]].texCoords[0], vertices[indic[j]].texCoords[1]);
        }

    }


    //====================

    //Read meshes data
    //====================

    short numMeshes;
    io->ReadFromFile(fHandle, &numMeshes, sizeof(short));

    short numTris;
    char materialIndex;
    for(i=0;i<numMeshes;i++)
    {
        io->ReadFromFile(fHandle, tempString, sizeof(char)*33);
        io->ReadFromFile(fHandle, &numTris, sizeof(short));
        unsigned short* trisIndices = new unsigned short[numTris];
        io->ReadFromFile(fHandle, trisIndices, sizeof(short)*numTris);
        io->ReadFromFile(fHandle, &materialIndex, sizeof(char));

        Mesh mesh;
        mesh.startIndex = trisIndices[0]*3;
        mesh.indicesCount = numTris*3;
        mesh.materialIndex = materialIndex;
        meshesInfo.push_back(mesh);

        delete[] trisIndices;
    }

    numV = numVertices;


    //====================

    //Read materials data
    //====================

    short numMaterials;
    io->ReadFromFile(fHandle, &numMaterials, sizeof(short));
    for(i=0;i<numMaterials;i++)
    {
        char texName[128];
        memset(texName, '\0', 128);
        Material mat;
        io->ReadFromFile(fHandle, tempString, sizeof(char)*32);
        io->ReadFromFile(fHandle, mat.ambient, sizeof(float)*4);
        io->ReadFromFile(fHandle, mat.diffuse, sizeof(float)*4);
        io->ReadFromFile(fHandle, mat.specular, sizeof(float)*4);
        io->ReadFromFile(fHandle, mat.emissive, sizeof(float)*4);
        io->ReadFromFile(fHandle, &mat.shinniness, sizeof(float));
        io->ReadFromFile(fHandle, &mat.transparency, sizeof(float));
        io->ReadFromFile(fHandle, tempString, sizeof(char));
        io->ReadFromFile(fHandle, texName, sizeof(char)*128);

        if(strcmp(texName, "")!=0) {
            mat.texture = IContentManager::get()->GetTextureManager()->GetTexture(texName);
        }

        io->ReadFromFile(fHandle, texName, sizeof(char)*128);

        materialsInfo.push_back(mat);
    }

    io->CloseFile(fHandle);
}


