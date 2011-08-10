#include <Math/MathLib.h>

void Vector2::Normalize()
{
	float length;
	float scalefactor;
	length=GetLength();

	if(length==1 || length==0)
		return;

	scalefactor = 1.0f/length;
	x *= scalefactor;
	y *= scalefactor;
}

Vector2 Vector2::GetNormalized() const
{
	Vector2 result(*this);

	result.Normalize();

	return result;
}

Vector2 operator*(float f, const Vector2 &vec)
{
	return vec*f;
}

bool Vector2::operator==(const Vector2 &vec) const
{
	if(x==vec.x && y==vec.y)
		return true;

	return false;
}



