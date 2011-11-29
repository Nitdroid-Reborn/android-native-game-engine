#ifndef COLLIDER_H
#define COLLIDER_H

#include <Graphics/IGeometry.h>

struct CollisionObject
{
    CollisionObject(){}
    Vector3 position;
    Vector3 normal;
    float distance;
};

class CollisionListener
{
public:
    virtual ~CollisionListener() {}
    
    virtual void Collision(const CollisionObject * collision) = 0;
};

class Collider
{
public:
    Collider(IGeometry * geometry);
    ~Collider();
    
    bool Collide(const Vector3 & point, CollisionObject &co);
    
private:
    class Triangle
    {
    public:
        Triangle(Vertex3D * v1, Vertex3D * v2, Vertex3D * v3);
        ~Triangle();
        
        void Append(Triangle * triangle);
        
        bool Collide(const Vector3 &point, CollisionObject& result);
        
        Triangle * next;
        Vertex3D * vertices[3];
        
    private:
        Vector3 a, b, c, normal;
        Vector3 v0, v1, v2;
        float d;
        
        void Init();
        
        bool SameSide(Vector3 & p1, Vector3 & p2, Vector3 & a, Vector3 & b);
    };
    
    int Index(float pos, float min, float max, int count);
    
    
    Triangle * m_triangles[1][1];
    
    float m_minX, m_maxX;
    float m_minZ, m_maxZ;
};

#endif // COLLIDER_H
