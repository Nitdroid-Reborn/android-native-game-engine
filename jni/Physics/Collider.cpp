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
    
    m_maxX += 0.01 * (m_maxX - m_minX);
    m_maxZ += 0.01 * (m_maxZ - m_minZ);
    
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
    return (int)((pos - min) / (max - min) * (float)count);
}

void Collider::Collide(const Vector3 & point, CollisionListener *listener)
{
    if(listener == 0)
        return;
    
    int x = Index(point.x, m_minX, m_maxX, 10);
    int z = Index(point.z, m_minZ, m_maxZ, 10);
    
    if(x < 0 || x >= 10 || z < 0 || z >= 10)        // point outside the boundaries of the object
        return;
    
    Triangle * triangle = m_triangles[x][z];
    
    CollisionObject * co;
    
    while(triangle != 0)
    {
        co = triangle->Collide(point);
        
        if(co != 0)
        {
            listener->Collision(co);
            delete co;
        }
        
        triangle = triangle->next;
    }
}

Collider::Triangle::Triangle(Vertex3D *v1, Vertex3D *v2, Vertex3D *v3)
{
    next = 0;
    vertices[0] = v1;
    vertices[1] = v2;
    vertices[2] = v3;
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

void Collider::Triangle::Init()
{    
    a.Set(vertices[1]->position[0] - vertices[0]->position[0],
          vertices[1]->position[1] - vertices[0]->position[1],
          vertices[1]->position[2] - vertices[0]->position[2]);
    
    b.Set(vertices[2]->position[0] - vertices[1]->position[0],
          vertices[2]->position[1] - vertices[1]->position[1],
          vertices[2]->position[2] - vertices[1]->position[2]);
        
    c.Set(vertices[2]->position[0] - vertices[0]->position[0],
          vertices[2]->position[1] - vertices[0]->position[1],
          vertices[2]->position[2] - vertices[0]->position[2]);
    
    v0.Set(vertices[0]->position[0],
           vertices[0]->position[1],
           vertices[0]->position[2]);
    
    v1.Set(vertices[1]->position[0],
           vertices[1]->position[1],
           vertices[1]->position[2]);

    v2.Set(vertices[2]->position[0],
           vertices[2]->position[1],
           vertices[2]->position[2]);
    
    normal = a.CrossProduct(c);
    normal.Normalize();
    
    c = -c;
    
    d = -normal.DotProduct(v0);
}

CollisionObject * Collider::Triangle::Collide(const Vector3 &point)
{
    float distance = normal.DotProduct(point) + d;
    Vector3 p = point;
    
    if(distance > 0)
        return 0;
    
    p = p - distance * normal;
    
    if(SameSide(p, v2, v1, v0) && SameSide(p, v0, v2, v1) && SameSide(p, v1, v0, v2))
    {
        CollisionObject * result = new CollisionObject();
        result->position = p;
        result->normal = normal;
        
        return result;
    }
    
    return 0;
}

bool Collider::Triangle::SameSide(Vector3 &p1, Vector3 &p2, Vector3 &a, Vector3 &b)
{
    Vector3 bma = b - a;
    Vector3 cp1 = bma.CrossProduct(p1 - a);
    Vector3 cp2 = bma.CrossProduct(p2 - a);
    
    if(cp1.DotProduct(cp2) >= 0)
        return true;
    
    return false;
}
