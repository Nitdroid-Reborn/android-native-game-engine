#include <Math/MathLib.h>
#include <Scripts/ScriptManager.h>

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

void Vector2::RegisterInLua() {
    lua_State*L = ScriptManager::Get()->getState();

    using namespace luabind;

    luabind::module(L)
    [
        luabind::class_<Vector2>("Vector2")
            .def(luabind::constructor<>())
            .def(luabind::constructor<float, float>())
            .def(luabind::constructor<const float*>())
            .def(luabind::constructor<const Vector2&>())
            .def("Set", &Vector2::Set)
            .def("SetX", &Vector2::SetX)
            .def("SetY", &Vector2::SetY)
            .def("GetX", &Vector2::GetX)
            .def("GetY", &Vector2::GetY)
            .def_readwrite("x", &Vector2::x)
            .def_readwrite("y", &Vector2::y)
            .def("Zero", &Vector2::Zero)
            .def("One", &Vector2::One)
            .def("Normalize", &Vector2::Normalize)
            .def("GetNormalized", &Vector2::GetNormalized)
            .def("GetLength", &Vector2::GetLength)
            .def("GetSquaredLength", &Vector2::GetSquaredLength)
            .def("Lerp", &Vector2::Lerp)
            .def("QuadricInterpolate", &Vector2::QuadraticInterpolate)
            .def(luabind::const_self + luabind::other<const Vector2&>())
            .def(luabind::const_self - luabind::other<const Vector2&>())
            .def(luabind::const_self * float())
            .def(luabind::const_self / float())
    ];
}
