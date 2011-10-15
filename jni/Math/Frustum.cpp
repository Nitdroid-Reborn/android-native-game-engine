#include <Math/MathLib.h>


void Frustum::Update(Matrix4x4 projection, Matrix4x4 view)
{
	Matrix4x4 clip;

	//Multiply the matrices
        clip=projection*view;

	//calculate planes
	planes[RIGHT_PLANE].normal.x=clip.entries[3]-clip.entries[0];
	planes[RIGHT_PLANE].normal.y=clip.entries[7]-clip.entries[4];
	planes[RIGHT_PLANE].normal.z=clip.entries[11]-clip.entries[8];
	planes[RIGHT_PLANE].distance=clip.entries[15]-clip.entries[12];

	planes[LEFT_PLANE].normal.x=clip.entries[3]+clip.entries[0];
	planes[LEFT_PLANE].normal.y=clip.entries[7]+clip.entries[4];
	planes[LEFT_PLANE].normal.z=clip.entries[11]+clip.entries[8];
	planes[LEFT_PLANE].distance=clip.entries[15]+clip.entries[12];

	planes[BOTTOM_PLANE].normal.x=clip.entries[3]+clip.entries[1];
	planes[BOTTOM_PLANE].normal.y=clip.entries[7]+clip.entries[5];
	planes[BOTTOM_PLANE].normal.z=clip.entries[11]+clip.entries[9];
	planes[BOTTOM_PLANE].distance=clip.entries[15]+clip.entries[13];

	planes[TOP_PLANE].normal.x=clip.entries[3]-clip.entries[1];
	planes[TOP_PLANE].normal.y=clip.entries[7]-clip.entries[5];
	planes[TOP_PLANE].normal.z=clip.entries[11]-clip.entries[9];
	planes[TOP_PLANE].distance=clip.entries[15]-clip.entries[13];

	planes[FAR_PLANE].normal.x=clip.entries[3]-clip.entries[2];
	planes[FAR_PLANE].normal.y=clip.entries[7]-clip.entries[6];
	planes[FAR_PLANE].normal.z=clip.entries[11]-clip.entries[10];
	planes[FAR_PLANE].distance=clip.entries[15]-clip.entries[14];

	planes[NEAR_PLANE].normal.x=clip.entries[3]+clip.entries[2];
	planes[NEAR_PLANE].normal.y=clip.entries[7]+clip.entries[6];
	planes[NEAR_PLANE].normal.z=clip.entries[11]+clip.entries[10];
	planes[NEAR_PLANE].distance=clip.entries[15]+clip.entries[14];

	//normalize planes
        for(int i=0; i<6; ++i)
                 planes[i].Normalize();
}

//is a point in the frustum?
bool Frustum::IsPointInside(const Vector3 &point) const
{
	for(int i=0; i<6; ++i)
	{
		if(planes[i].ClassifyPoint(point)==POINT_BEHIND_PLANE)
			return false;
	}

	return true;
}

//is a bounding box in the frustum?
bool Frustum::IsBoundingBoxInside(const BoundingBox& bbox) const
{
    //loop through planes
    for(int i=0; i<6; ++i)
    {
        //if a point is not behind this plane, try next plane
        if(planes[i].ClassifyPoint(bbox.vertices[0])!=POINT_BEHIND_PLANE)
            continue;
        if(planes[i].ClassifyPoint(bbox.vertices[1])!=POINT_BEHIND_PLANE)
            continue;
        if(planes[i].ClassifyPoint(bbox.vertices[2])!=POINT_BEHIND_PLANE)
            continue;
        if(planes[i].ClassifyPoint(bbox.vertices[3])!=POINT_BEHIND_PLANE)
            continue;
        if(planes[i].ClassifyPoint(bbox.vertices[4])!=POINT_BEHIND_PLANE)
            continue;
        if(planes[i].ClassifyPoint(bbox.vertices[5])!=POINT_BEHIND_PLANE)
            continue;
        if(planes[i].ClassifyPoint(bbox.vertices[6])!=POINT_BEHIND_PLANE)
            continue;
        if(planes[i].ClassifyPoint(bbox.vertices[7])!=POINT_BEHIND_PLANE)
            continue;

        //All vertices of the box are behind this plane
        return false;
    }

    return true;
}


bool Frustum::IsBoundingBoxIntersected(const BoundingBox& bbox) const
{

    bool classyfiedVertices[8];
    for(int i=0;i<8;i++)classyfiedVertices[i]=true;

    //loop through planes
    for(int i=0; i<6; ++i)
    {
        //if a point is not behind this plane, try next plane
        if(planes[i].ClassifyPoint(bbox.vertices[0])==POINT_BEHIND_PLANE)
            classyfiedVertices[0]=false;

        if(planes[i].ClassifyPoint(bbox.vertices[1])==POINT_BEHIND_PLANE)
            classyfiedVertices[1]=false;


        if(planes[i].ClassifyPoint(bbox.vertices[2])==POINT_BEHIND_PLANE)
            classyfiedVertices[2]=false;


        if(planes[i].ClassifyPoint(bbox.vertices[3])==POINT_BEHIND_PLANE)
            classyfiedVertices[3]=false;


        if(planes[i].ClassifyPoint(bbox.vertices[4])==POINT_BEHIND_PLANE)
            classyfiedVertices[4]=false;


        if(planes[i].ClassifyPoint(bbox.vertices[5])==POINT_BEHIND_PLANE)
            classyfiedVertices[5]=false;


        if(planes[i].ClassifyPoint(bbox.vertices[6])==POINT_BEHIND_PLANE)
            classyfiedVertices[6]=false;


        if(planes[i].ClassifyPoint(bbox.vertices[7])==POINT_BEHIND_PLANE)
            classyfiedVertices[7]=false;

        for(int j=1;j<8;j++) {
            if(classyfiedVertices[0]!=classyfiedVertices[j])return true;
        }
    }
    return false;
}


bool Frustum::IsBoundingBoxIntersectedWithNearPlan(const BoundingBox& bbox) const
{
    bool classyfiedVertices[8];
    for(int i=0;i<8;i++)classyfiedVertices[i]=true;


    //if a point is not behind this plane, try next plane
    if(planes[NEAR_PLANE].ClassifyPoint(bbox.vertices[0])==POINT_BEHIND_PLANE)
        classyfiedVertices[0]=false;

    if(planes[NEAR_PLANE].ClassifyPoint(bbox.vertices[1])==POINT_BEHIND_PLANE)
        classyfiedVertices[1]=false;


    if(planes[NEAR_PLANE].ClassifyPoint(bbox.vertices[2])==POINT_BEHIND_PLANE)
        classyfiedVertices[2]=false;


    if(planes[NEAR_PLANE].ClassifyPoint(bbox.vertices[3])==POINT_BEHIND_PLANE)
        classyfiedVertices[3]=false;


    if(planes[NEAR_PLANE].ClassifyPoint(bbox.vertices[4])==POINT_BEHIND_PLANE)
        classyfiedVertices[4]=false;


    if(planes[NEAR_PLANE].ClassifyPoint(bbox.vertices[5])==POINT_BEHIND_PLANE)
        classyfiedVertices[5]=false;


    if(planes[NEAR_PLANE].ClassifyPoint(bbox.vertices[6])==POINT_BEHIND_PLANE)
        classyfiedVertices[6]=false;


    if(planes[NEAR_PLANE].ClassifyPoint(bbox.vertices[7])==POINT_BEHIND_PLANE)
        classyfiedVertices[7]=false;

    for(int j=1;j<8;j++) {
        if(classyfiedVertices[0]!=classyfiedVertices[j])return true;
    }
    return false;
}
