#ifndef __VECTOR4_H__
#define __VECTOR4_H__

class Vector4
{
public:
	float x;
	float y;
	float z;
	float w;

	Vector4(void):x(0.0f), y(0.0f), z(0.0f), w(0.0f)	{}

	Vector4(float newX, float newY, float newZ, float newW)	
		:	x(newX), y(newY), z(newZ), w(newW)	{}

	Vector4(const float * vec):x(*vec), y(*(vec+1)), z(*(vec+2)), w(*(vec+3))	{}

	Vector4(const Vector4 &vec):x(vec.x), y(vec.y), z(vec.z), w(vec.w)	{}

	Vector4(const Vector3 &vec):x(vec.x), y(vec.y), z(vec.z), w(1.0f)	{}

    Vector4(const Vector3 &vec, float _w):x(vec.x), y(vec.y), z(vec.z), w(_w)	{}

	~Vector4() {}

	void Set(float newX, float newY, float newZ, float newW)
	{	
		x=newX;	y=newY;	z=newZ; w=newW;	
	}
	
	void SetX(float newX) {x = newX;}
	void SetY(float newY) {y = newY;}
	void SetZ(float newZ) {z = newZ;}
	void SetW(float newW) {w = newW;}
	
	float GetX() const {return x;}
	float GetY() const {return y;}
	float GetZ() const {return z;}
	float GetW() const {return w;}

	void Zero(void)
	{	
		x=0.0f; y=0.0f; z=0.0f; w=0.0f;
	}

	void One(void)
	{	
		x=1.0f; y=1.0f; z=1.0f; w=1.0f;	
	}

	float DotProduct(const Vector4 & vec)
	{	
		return x*vec.x + y*vec.y + z*vec.z + w*vec.w;
	}

	void RotateX(double angle);
	Vector4 GetRotatedX(double angle) const;
	void RotateY(double angle);
	Vector4 GetRotatedY(double angle) const;
	void RotateZ(double angle);
	Vector4 GetRotatedZ(double angle) const;
	void RotateAxis(double angle, const Vector3 &axis);
	Vector4 GetRotatedAxis(double angle, const Vector3 &axis) const;
	
	Vector4 Lerp(const Vector4 &v2, float factor) const
	{	
		return (*this)*(1.0f-factor)+v2*factor;
	}

	Vector4 QuadraticInterpolate(const Vector4 &v2, const Vector4 &v3, float factor) const
	{	
		return (*this)*(1.0f-factor)*(1.0f-factor) + 2*v2*factor*(1.0f-factor) + v3*factor*factor;
	}

	Vector4 operator+(const Vector4 &vec) const
	{	
		return Vector4(x+vec.x, y+vec.y, z+vec.z, w+vec.w);
	}

	Vector4 operator-(const Vector4 &vec) const
	{	
		return Vector4(x-vec.x, y-vec.y, z-vec.z, w-vec.w);
	}

	Vector4 operator*(const float f) const
	{	
		return Vector4(x*f, y*f, z*f, w*f);
	}

	Vector4 operator/(const float f) const
	{	
		return f==0.0f	?	Vector4(0.0f, 0.0f, 0.0f, 0.0f)
						:	Vector4(x/f, y/f, z/f, w/f);
	}

	friend Vector4 operator*(float scaleFactor, const Vector4 &vec);

	bool operator==(const Vector4 &vec) const;
	bool operator!=(const Vector4 &vec) const
	{	
		return !((*this)==vec);	
	}

	void operator+=(const Vector4 &vec)
	{	
		x+=vec.x; y+=vec.y; z+=vec.z; w+=vec.w;
	}

	void operator-=(const Vector4 &vec)
	{
		x-=vec.x; y-=vec.y; z-=vec.z; w-=vec.w;
	}

	void operator*=(const float f)
	{
		x*=f; y*=f; z*=f; w*=f;
	}

	void operator/=(const float f)
	{	
		if(f==0.0f)
			return;
		else
		{	
			x/=f; y/=f; z/=f; w/=f;
		}
	}

	Vector4 operator-(void) const {return Vector4(-x, -y, -z, -w);}
	Vector4 operator+(void) const {return (*this);}

	operator float* () const {return (float*) this;}
	operator const float* () const {return (const float*) this;}

	operator Vector3();

        static void RegisterInLua();
};

#endif
