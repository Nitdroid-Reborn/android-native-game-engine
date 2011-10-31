#include "Collider.h"

#include <cfloat>

Collider::Collider(IGeometry * geometry) :
    m_minX(FLT_MAX), m_maxX(-FLT_MAX), m_minZ(FLT_MAX), m_maxZ(-FLT_MAX)
{
    for(int i = 0; i < 10; i++)
        for(int j = 0; j < 10; j++)
            m_triangles[i][j] = 0;
    
    int verticesCount = geometry->GetVerticesCount();
    
    Vertex3D * vertices = geometry->GetVertices();
    
    for(int i = 0; i < verticesCount; i++)
    {
        if(vertices[i].position[0] < m_minX)
            m_minX = vertices[i].position[0];
        else if(vertices[i].position[0] > m_maxX)
            m_maxX = vertices[i].position[0];
        
        if(vertices[i].position[2] < m_minZ)
            m_minZ = vertices[i].position[2];
        else if(vertices[i].position[2] > m_maxZ)
            m_maxZ = vertices[i].position[2];
    }
    
    int triangleCount = geometry->GetIndicesCount() / 3;
    U16 * indices = geometry->GetIndices();
    
    Vertex3D * v[3];
    
    for(int i = 0; i < triangleCount; i++)
    {
        int x[3], z[3];
        
        v[0] = &vertices[indices[3 * i]];
        v[1] = &vertices[indices[3 * i + 1]];
        v[2] = &vertices[indices[3 * i + 2]];
        
        for(int j = 0; j < 3; j++)
        {
            x[j] = Index(v[j]->position[0], m_minX, m_maxX, 10);
            z[j] = Index(v[j]->position[2], m_minZ, m_maxZ, 10);
            
            bool add = true;
            
            for(int k = j - 1; j >= 0; j--)
                if(x[j] == x[k] && z[j] == z[k])
                    add = false;
            
            if(add)
            {
                Triangle * t = new Triangle(v[0], v[1], v[2]);
                m_triangles[x[j]][z[j]]->Append(t);
            }
        }
    }
}

Collider::~Collider()
{
    for(int i = 0; i < 10; i++)
        for(int j = 0; j < 10; j++)
            if(m_triangles[i][j] != 0)
                delete m_triangles[i][j];
}

int Collider::Index(float pos, float min, float max, int count)
{
    float step = (max - min) / (float)count;
    return (int)pos / step;
}

Collider::Triangle::Triangle(Vertex3D *v1, Vertex3D *v2, Vertex3D *v3)
{
    next = 0;
    vertices[1] = v1;
    vertices[2] = v2;
    vertices[3] = v3;
}

Collider::Triangle::~Triangle()
{
    if(next != 0)
        delete next;
}

void Collider::Triangle::Append(Triangle *triangle)
{
    if(next != 0)
        next->Append(triangle);
    else
        next = triangle;
}
