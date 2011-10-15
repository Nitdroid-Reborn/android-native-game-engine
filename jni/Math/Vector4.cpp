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

void Vector4::RegisterInLua() {
    lua_State*L = ScriptManager::Get()->getState();

    using namespace luabind;

    luabind::module(L)
    [
        luabind::class_<Vector4>("Vector4")
            .def(luabind::constructor<>())
            .def(luabind::constructor<float, float, float, float>())
            .def(luabind::constructor<const float*>())
            .def(luabind::constructor<const Vector3&>())
            .def(luabind::constructor<const Vector4&>())
            .def("Set", &Vector4::Set)
            .def("SetX", &Vector4::SetX)
            .def("SetY", &Vector4::SetY)
            .def("SetZ", &Vector4::SetZ)
            .def("SetW", &Vector4::SetW)
            .def("GetX", &Vector4::GetX)
            .def("GetY", &Vector4::GetY)
            .def("GetZ", &Vector4::GetZ)
            .def("GetW", &Vector4::GetW)
            .def_readwrite("x", &Vector4::x)
            .def_readwrite("y", &Vector4::y)
            .def_readwrite("z", &Vector4::z)
            .def_readwrite("w", &Vector4::w)
            .def("Zero", &Vector4::Zero)
            .def("One", &Vector4::One)
            .def("DotProduct", &Vector4::DotProduct)
            .def("RotateX", &Vector4::RotateX)
            .def("RotateY", &Vector4::RotateY)
            .def("RotateZ", &Vector4::RotateZ)
            .def("RotateAxis", &Vector4::RotateAxis)
            .def("GetRotatedX", &Vector4::GetRotatedX)
            .def("GetRotatedY", &Vector4::GetRotatedY)
            .def("GetRotatedZ", &Vector4::GetRotatedZ)
            .def("GetRotatedAxis", &Vector4::GetRotatedAxis)
            .def("Lerp", &Vector4::Lerp)
            .def("QuadricInterpolate", &Vector4::QuadraticInterpolate)
            .def(luabind::const_self + luabind::other<const Vector4&>())
            .def(luabind::const_self - luabind::other<const Vector4&>())
            .def(luabind::const_self * float())
            .def(luabind::const_self / float())
    ];
}
