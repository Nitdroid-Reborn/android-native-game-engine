#include <Math/MathLib.h>

void Vector4::RotateX(double angle)
{
	(*this)=GetRotatedX(angle);
}

Vector4 Vector4::GetRotatedX(double angle) const
{
	Vector3 v3d(x, y, z);

	v3d.RotateX(angle);

	return Vector4(v3d.x, v3d.y, v3d.z, w);
}

void Vector4::RotateY(double angle)
{
	(*this)=GetRotatedY(angle);
}

Vector4 Vector4::GetRotatedY(double angle) const
{
	Vector3 v3d(x, y, z);

	v3d.RotateY(angle);

	return Vector4(v3d.x, v3d.y, v3d.z, w);
}

void Vector4::RotateZ(double angle)
{
	(*this)=GetRotatedZ(angle);
}

Vector4 Vector4::GetRotatedZ(double angle) const
{
	Vector3 v3d(x, y, z);

	v3d.RotateZ(angle);

	return Vector4(v3d.x, v3d.y, v3d.z, w);
}

void Vector4::RotateAxis(double angle, const Vector3 & axis)
{
	(*this)=GetRotatedAxis(angle, axis);
}

Vector4 Vector4::GetRotatedAxis(double angle, const Vector3 & axis) const
{
	Vector3 v3d(x, y, z);

	v3d.RotateAxis(angle, axis);

	return Vector4(v3d.x, v3d.y, v3d.z, w);
}


Vector4 operator*(float scaleFactor, const Vector4 &vec)
{
	return vec*scaleFactor;
}

bool Vector4::operator==(const Vector4 &vec) const
{
	if(x==vec.x && y==vec.y && z==vec.z && w==vec.w)
		return true;

	return false;
}

Vector4::operator Vector3()
{
	if(w==0.0f || w==1.0f)
		return Vector3(x, y, z);
	else
		return Vector3(x/w, y/w, z/w);
}
