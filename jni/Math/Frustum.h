#ifndef __FRUSTUM_H__
#define __FRUSTUM_H__

class BoundingBox;

enum FRUSTUM_PLANES
{
	LEFT_PLANE=0,
	RIGHT_PLANE,
	TOP_PLANE,
	BOTTOM_PLANE,
	NEAR_PLANE,
	FAR_PLANE
};

enum FRUSTUM_CLASSIFICATION
{
	OUTSIDE_FRUSTUM=0,
	IN_FRUSTUM
};

class Frustum
{
public:
	void Update(Matrix4x4 projection, Matrix4x4 view);
        bool IsPointInside(const Vector3 &point) const;

        bool IsBoundingBoxInside(const BoundingBox& bbox) const;
        bool IsBoundingBoxIntersected(const BoundingBox& bbox) const;
        bool IsBoundingBoxIntersectedWithNearPlan(const BoundingBox& bbox) const;
	
	Plane planes[6];
};

#endif
