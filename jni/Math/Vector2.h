#ifndef __VECTOR2_H_
#define __VECTOR2_H__

#include <cmath>
//#include <Scripts/oolua/oolua.h>


class Vector2
{
public:
	float x;
	float y;

	Vector2(void):x(0.0f), y(0.0f)	{}

	Vector2(float newX, float newY):x(newX), y(newY)	{}

	Vector2(const float * pF):x(*pF), y((*pF)+1)	{}

	Vector2(const Vector2 &vec)	:x(vec.x), y(vec.y)	{}

	~Vector2() {}

	void Set(float newX, float newY)
	{	
		x=newX;	y=newY;	
	}
	
	void SetX(float newX) {x = newX;}
	void SetY(float newY) {y = newY;}

	float GetX() const {return x;}
	float GetY() const {return y;}

	void Zero(void)
	{
		x=y=0.0f;
	};
	void One(void)
	{
		x=y=1.0f;
	};
	
	void Normalize();

	Vector2 GetNormalized() const;
	
	float GetLength() const
	{	
		return (float)sqrt((x*x)+(y*y));
	}
	
	float GetSquaredLength() const
	{	
		return (x*x)+(y*y);
	}

	Vector2 Lerp(const Vector2 &v2, float factor) const
	{	
		return (*this)*(1.0f-factor) + v2*factor;	
	}

	Vector2 QuadraticInterpolate(const Vector2 &v2, const Vector2 &v3, float factor) const
	{	
		return (*this)*(1.0f-factor)*(1.0f-factor) + 2*v2*factor*(1.0f-factor) + v3*factor*factor;
	}

	Vector2 operator+(const Vector2 &vec) const
	{	
		return Vector2(x + vec.x, y + vec.y);
	}

	Vector2 operator-(const Vector2 &vec) const
	{	
		return Vector2(x - vec.x, y - vec.y);
	}

	Vector2 operator*(const float f) const
	{
		return Vector2(x*f, y*f);
	}
	
	Vector2 operator/(const float f) const
	{	
		return (f==0) ? Vector2(0.0f, 0.0f) : Vector2(x / f, y / f);
	}

	friend Vector2 operator*(float f, const Vector2 &vec);

	bool operator==(const Vector2 &vec) const;
	bool operator!=(const Vector2 &vec) const
	{	
		return !((*this)==vec);
	}

	void operator+=(const Vector2 &vec)
	{
		x+=vec.x;	y+=vec.y;
	}

	void operator-=(const Vector2 &vec)
	{	
		x-=vec.x;	y-=vec.y;
	}

	void operator*=(const float f)
	{	
		x*=f;	y*=f;	
	}

	void operator/=(const float f)
	{	
		if(f==0.0f)
			return;
		else
		{	
			x/=f; 
			y/=f;	
		}
	}


	Vector2 operator-(void) const {return Vector2(-x, -y);}
	Vector2 operator+(void) const {return *this;}

	operator float* () const {return (float*) this;}
	operator const float* () const {return (const float*) this;}

};

/*OOLUA_PROXY_CLASS(Vector2)
    OOLUA_TYPEDEFS
        OOLUA::Equal_op,
        OOLUA::Not_equal_op,
        OOLUA::Add_op,
        OOLUA::Sub_op
    OOLUA_END_TYPES

OOLUA_CONSTRUCTORS_BEGIN
    OOLUA_CONSTRUCTOR_2(float, float)
    OOLUA_CONSTRUCTOR_1(const Vector2&)
OOLUA_CONSTRUCTORS_END

    OOLUA_MEM_FUNC_2(void, Set, float, float)
    OOLUA_MEM_FUNC_1(void, SetX, float)
    OOLUA_MEM_FUNC_1(void, SetY, float)
    OOLUA_MEM_FUNC_0_CONST(float, GetX)
    OOLUA_MEM_FUNC_0_CONST(float, GetY)
    OOLUA_MEM_FUNC_0(void, Zero)
    OOLUA_MEM_FUNC_0(void, One)
    OOLUA_MEM_FUNC_0(void, Normalize)
    OOLUA_MEM_FUNC_0_CONST(Vector2, GetNormalized)
    OOLUA_MEM_FUNC_0_CONST(float, GetLength)
OOLUA_CLASS_END
*/
#endif
