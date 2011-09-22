#include <Math/MathLib.h>
#include <Scripts/ScriptManager.h>

float Vector3::GetLength() const
{
    return (float)sqrt(x*x + y*y + z*z);
}

void Vector3::Normalize()
{
    float f = (float)sqrt(x*x + y*y + z*z);
    if(f != 0.0f && f!=1.0f)
    {
        x/=f;
        y/=f;
        z/=f;
    }
}


Vector3 Vector3::GetNormalized() const
{
	Vector3 result(*this);

	result.Normalize();

	return result;
}

Vector3 Vector3::CrossProduct(const Vector3 &vec) const
{	
	Vector3 result;

        result.Set(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x);
	
	return result;
}


Vector3 Vector3::GetRotatedX(double angle) const
{
	if(angle==0.0)
		return (*this);

	float sinAngle=(float)sin(M_PI*angle/180);
	float cosAngle=(float)cos(M_PI*angle/180);

	return Vector3(	x,	y*cosAngle - z*sinAngle,
						y*sinAngle + z*cosAngle);
}

void Vector3::RotateX(double angle)
{
	(*this)=GetRotatedX(angle);
}

Vector3 Vector3::GetRotatedY(double angle) const
{
	if(angle==0.0)
		return (*this);

	float sinAngle=(float)sin(M_PI*angle/180);
	float cosAngle=(float)cos(M_PI*angle/180);

	return Vector3(	x*cosAngle + z*sinAngle,
						y,
						-x*sinAngle + z*cosAngle);
}

void Vector3::RotateY(double angle)
{
	(*this)=GetRotatedY(angle);
}

Vector3 Vector3::GetRotatedZ(double angle) const
{
	if(angle==0.0)
		return (*this);

	float sinAngle=(float)sin(M_PI*angle/180);
	float cosAngle=(float)cos(M_PI*angle/180);
	
	return Vector3(x*cosAngle - y*sinAngle,
					x*sinAngle + y*cosAngle,
					z);
}

void Vector3::RotateZ(double angle)
{
	(*this)=GetRotatedZ(angle);
}

Vector3 Vector3::GetRotatedAxis(double angle, const Vector3 & axis) const
{
	if(angle==0.0)
		return (*this);

	Vector3 u=axis.GetNormalized();

	Vector3 rotMatrixRow0, rotMatrixRow1, rotMatrixRow2;

	float sinAngle=(float)sin(M_PI*angle/180);
	float cosAngle=(float)cos(M_PI*angle/180);
	float oneMinusCosAngle=1.0f-cosAngle;

	rotMatrixRow0.x=(u.x)*(u.x) + cosAngle*(1-(u.x)*(u.x));
	rotMatrixRow0.y=(u.x)*(u.y)*(oneMinusCosAngle) - sinAngle*u.z;
	rotMatrixRow0.z=(u.x)*(u.z)*(oneMinusCosAngle) + sinAngle*u.y;

	rotMatrixRow1.x=(u.x)*(u.y)*(oneMinusCosAngle) + sinAngle*u.z;
	rotMatrixRow1.y=(u.y)*(u.y) + cosAngle*(1-(u.y)*(u.y));
	rotMatrixRow1.z=(u.y)*(u.z)*(oneMinusCosAngle) - sinAngle*u.x;
	
	rotMatrixRow2.x=(u.x)*(u.z)*(oneMinusCosAngle) - sinAngle*u.y;
	rotMatrixRow2.y=(u.y)*(u.z)*(oneMinusCosAngle) + sinAngle*u.x;
	rotMatrixRow2.z=(u.z)*(u.z) + cosAngle*(1-(u.z)*(u.z));

	return Vector3(	this->DotProduct(rotMatrixRow0),
						this->DotProduct(rotMatrixRow1),
						this->DotProduct(rotMatrixRow2));
}

void Vector3::RotateAxis(double angle, const Vector3 & axis)
{
	(*this)=GetRotatedAxis(angle, axis);
}



void Vector3::PackTo01()
{
	(*this)=GetPackedTo01();	
}

Vector3 Vector3::GetPackedTo01() const
{
	Vector3 temp(*this);

	temp.Normalize();

	temp=temp*0.5f+Vector3(0.5f, 0.5f, 0.5f);
	
	return temp;
}

Vector3 operator*(float scaleFactor, const Vector3 &vec)
{
	return vec*scaleFactor;
}

bool Vector3::operator==(const Vector3 &vec) const
{
	if(x==vec.x && y==vec.y && z==vec.z)
		return true;

	return false;
}

void Vector3::RegisterInLua() {
    lua_State*L = ScriptManager::Get()->getState();

    using namespace luabind;

    luabind::module(L)
    [
        luabind::class_<Vector3>("Vector3")
            .def(luabind::constructor<>())
            .def(luabind::constructor<float, float, float>())
            .def(luabind::constructor<const float*>())
            .def(luabind::constructor<const Vector3&>())
            .def("Set", &Vector3::Set)
            .def("SetX", &Vector3::SetX)
            .def("SetY", &Vector3::SetY)
            .def("SetZ", &Vector3::SetZ)
            .def("GetX", &Vector3::GetX)
            .def("GetY", &Vector3::GetY)
            .def("GetZ", &Vector3::GetZ)
            .def_readwrite("x", &Vector3::x)
            .def_readwrite("y", &Vector3::y)
            .def_readwrite("z", &Vector3::z)
            .def("Zero", &Vector3::Zero)
            .def("One", &Vector3::One)
            .def("DotProduct", &Vector3::DotProduct)
            .def("CrossProduct", &Vector3::CrossProduct)
            .def("AngleWith", &Vector3::AngleWith)
            .def("Normalize", &Vector3::Normalize)
            .def("GetNormalized", &Vector3::GetNormalized)
            .def("GetLength", &Vector3::GetLength)
            .def("GetSquaredLength", &Vector3::GetSquaredLength)
            .def("RotateX", &Vector3::RotateX)
            .def("RotateY", &Vector3::RotateY)
            .def("RotateZ", &Vector3::RotateZ)
            .def("RotateAxis", &Vector3::RotateAxis)
            .def("GetRotatedX", &Vector3::GetRotatedX)
            .def("GetRotatedY", &Vector3::GetRotatedY)
            .def("GetRotatedZ", &Vector3::GetRotatedZ)
            .def("GetRotatedAxis", &Vector3::GetRotatedAxis)
            .def("Lerp", &Vector3::Lerp)
            .def("QuadricInterpolate", &Vector3::QuadraticInterpolate)
            .def(luabind::const_self + luabind::other<const Vector3&>())
            .def(luabind::const_self - luabind::other<const Vector3&>())
            .def(luabind::const_self * float())
            .def(luabind::const_self * luabind::other<const Vector3&>())
            .def(luabind::const_self / float())
    ];
}
