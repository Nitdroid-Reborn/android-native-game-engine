#ifndef COLLIDER_H
#define COLLIDER_H

#include <Graphics/IGeometry.h>


class Collider
{
public:
    Collider(IGeometry * geometry);
    ~Collider();
    
private:
    class Triangle
    {
    public:
        Triangle(Vertex3D * v1, Vertex3D * v2, Vertex3D * v3);
        ~Triangle();
        
        void Append(Triangle * triangle);
        
        Triangle * next;
        Vertex3D * vertices[3];
    };
    
    int Index(float pos, float min, float max, int count);
    
    
    Triangle * m_triangles[10][10];
    
    float m_minX, m_maxX;
    float m_minZ, m_maxZ;
};

#endif // COLLIDER_H
