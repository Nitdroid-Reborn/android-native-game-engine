#ifndef __VECTOR3_H__
#define __VECTOR3_H__
#include <cmath>

class Vector3
{
public:
	float x;
	float y;
	float z;
//	float w;

	//constructors
	Vector3(void):x(0.0f), y(0.0f), z(0.0f)	{}

	Vector3(float newX, float newY, float newZ):x(newX), y(newY), z(newZ){}

	Vector3(const float * pft):	x(*pft), y(*(pft+1)), z(*(pft+2)) {}

	Vector3(const Vector3 &vec)	:	x(vec.x), y(vec.y), z(vec.z) {}

	~Vector3() {}

	void Set(float newX, float newY, float newZ)
	{	
		x=newX;
		y=newY;	
		z=newZ;
	}
	
	void SetX(float newX) {x = newX;}
	void SetY(float newY) {y = newY;}
	void SetZ(float newZ) {z = newZ;}

	float GetX() const {return x;}
	float GetY() const {return y;}
	float GetZ() const {return z;}

	void Zero(void)
	{
		x=y=z=0.0f;	
	}
	void One(void)
	{
		x=y=z=1.0f;	
	}
	
	Vector3 CrossProduct(const Vector3 &vec) const;

	float DotProduct(const Vector3 &vec) const
	{	
		return x*vec.x + y*vec.y + z*vec.z;	
	}

	float AngleWith(const Vector3 &vec) const
	{
		return (float)acos( DotProduct(vec))/(GetLength() * vec.GetLength());
	}
	
	void Normalize();

	Vector3 GetNormalized() const;
	
	float GetLength() const;
		
	float GetSquaredLength() const
	{	
		return (x*x)+(y*y)+(z*z);
	}


	void RotateX(double angle);
	Vector3 GetRotatedX(double angle) const;
	void RotateY(double angle);
	Vector3 GetRotatedY(double angle) const;
	void RotateZ(double angle);
	Vector3 GetRotatedZ(double angle) const;
	void RotateAxis(double angle, const Vector3 &axis);
	Vector3 GetRotatedAxis(double angle, const Vector3 &axis) const;

	void PackTo01();
	Vector3 GetPackedTo01() const;


	Vector3 Lerp(const Vector3 &v2, float factor) const
	{
		return (*this)*(1.0f-factor) + v2*factor;
	}

	Vector3 QuadraticInterpolate(const Vector3 &v2, const Vector3 &v3, float factor) const
	{	
		return (*this)*(1.0f-factor)*(1.0f-factor) + 2*v2*factor*(1.0f-factor) + v3*factor*factor;
	}


	Vector3 operator+(const Vector3 &vec) const
	{	
		return Vector3(x + vec.x, y + vec.y, z + vec.z);
	}
	
	Vector3 operator-(const Vector3 &vec) const
	{	
		return Vector3(x - vec.x, y - vec.y, z - vec.z);
	}

	Vector3 operator*(const float f) const
	{	
		return Vector3(x*f, y*f, z*f);
	}

        Vector3 operator*(const Vector3& vec) const
        {
                return Vector3(x*vec.x, y*vec.y, z*vec.z);
        }
	
	Vector3 operator/(const float f) const
	{	
		return (f==0.0f) ? Vector3(0.0f, 0.0f, 0.0f) : Vector3(x / f, y / f, z / f);
	}


	friend Vector3 operator*(float f, const Vector3 &vec);


	void Add(const Vector3 &v2, Vector3 &result)
	{
		result.x=x+v2.x;
		result.y=y+v2.y;
		result.z=z+v2.z;
	}

	void Subtract(const Vector3 &v2, Vector3 &result)
	{
		result.x=x-v2.x;
		result.y=y-v2.y;
		result.z=z-v2.z;
	}

	bool operator==(const Vector3 &vec) const;
	bool operator!=(const Vector3 &vec) const
	{	
		return !((*this)==vec);
	}

	void operator+=(const Vector3 &vec)
	{	
		x+=vec.x;	y+=vec.y;	z+=vec.z;	
	}

	void operator-=(const Vector3 &vec)
	{	
		x-=vec.x;	y-=vec.y;	z-=vec.z;
	}

	void operator*=(const float f)
	{	
		x*=f;	y*=f;	z*=f;
	}

        void operator*=(const Vector3& vec)
        {
                x*=vec.x; y*=vec.y; z*=vec.z;
        }
	
	void operator/=(const float f)
	{	
		if(f==0.0f)
			return;
		else
		{	
			x/=f; y/=f; z/=f;
		}
	}


	Vector3 operator-(void) const {return Vector3(-x, -y, -z);}
	Vector3 operator+(void) const {return *this;}

	operator float* () const {return (float*) this;}
	operator const float* () const {return (const float*) this;}

        static void RegisterInLua();
};

#endif
