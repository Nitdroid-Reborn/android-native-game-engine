#include "MS3DModel.h"
#include <FileIO/IFileIO.h>
#define PI 3.14156

void MS3DModel::Load(const char* filename, Vertex3D*&vertices, int& numV,
                   U16*& indices, int& numI)
{
    IFileIO* io = IFileIO::get();
    EngineFileHandle fHandle;
    io->OpenFile(filename, "rb", fHandle);

    //Vertex3D* vertices;
    //U16* indices;

    int i;
    char tempChar;
    float tempFloat;
    int tempInt;
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
    for(i=0;i<numTriangles;i++)
    {
        io->ReadFromFile(fHandle, tempString, sizeof(short));
        io->ReadFromFile(fHandle, &indic, sizeof(short)*3);
        io->ReadFromFile(fHandle, &norm, sizeof(float)*9);
        io->ReadFromFile(fHandle, &u, sizeof(float)*3);
        io->ReadFromFile(fHandle, &v, sizeof(float)*3);
        io->ReadFromFile(fHandle, tempString, sizeof(char)*2);

        for(int j=0;j<3;j++) {
            vertices[indic[j]].texCoords[0]=u[j];
            vertices[indic[j]].texCoords[1]=v[j];
            vertices[indic[j]].normal[0]=norm[j*3];
            vertices[indic[j]].normal[1]=norm[j*3+1];
            vertices[indic[j]].normal[2]=norm[j*3+2];
            vertices[indic[j]].color[0]=255;
            vertices[indic[j]].color[1]=255;
            vertices[indic[j]].color[2]=255;
            vertices[indic[j]].color[3]=255;
            indices[i*3+j]=indic[j];
        }
    }

    for(int z=0;z<numTriangles*3;z++) {
      //  Logger::Log("%d", indices[z]);
    }
    //====================

    //Read meshes data
    //====================

    short numMeshes;
    io->ReadFromFile(fHandle, &numMeshes, sizeof(short));
    Logger::Log("Milkshape file %d", numMeshes);
    //meshes = new MyModelMesh[numMeshes];
    short numTris;
    char materialIndex;
    for(i=0;i<numMeshes;i++)
    {
        io->ReadFromFile(fHandle, tempString, sizeof(char)*33);
        io->ReadFromFile(fHandle, &numTris, sizeof(short));
        unsigned short* trisIndices = new unsigned short[numTris];
        io->ReadFromFile(fHandle, trisIndices, sizeof(short)*numTris);
        io->ReadFromFile(fHandle, &materialIndex, sizeof(char));

        delete[] trisIndices;
    }

    numV = numVertices;

    Logger::Log("Num vertices %d", numVertices);
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

                //materials[i].textureID=0;


               // qDebug()<<i<<materials[i].ambient[0]<<materials[i].ambient[1]<<materials[i].ambient[2];
		if(strcmp(materials[i].texture, "")!=0)
		{
                    //materials[i].textureID=0;
//			materials[i].textureID = LoadGLTexture(materials[i].texture);
		}

        }*/

    io->CloseFile(fHandle);
}


