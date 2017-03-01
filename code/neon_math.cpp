#include "neon_math.h"

/*----------  Vector2 constructors  ----------*/

Vector2::Vector2()
{
	Elements[0] = 0;
	Elements[1] = 0;
}

Vector2::Vector2(r32 X, r32 Y)
{
	Elements[0] = X;
	Elements[1] = Y;
}

Vector2::~Vector2()
{

}

/*----------  Vector3 constructors  ----------*/

Vector3::Vector3()
{
	Elements[0] = 0;
	Elements[1] = 0;
	Elements[2] = 0;
}

Vector3::Vector3(r32 X, r32 Y, r32 Z)
{
	Elements[0] = X;
	Elements[1] = Y;
	Elements[2] = Z;
}

Vector3::Vector3(Vector2 V2, r32 Z)
{
	Elements[0] = V2.X;
	Elements[1] = V2.Y;
	Elements[2] = Z;
}

Vector3::~Vector3()
{

}

/*----------  Vector4 constructors  ----------*/

Vector4::Vector4()
{
	Elements[0] = 0;
	Elements[1] = 0;
	Elements[2] = 0;
	Elements[3] = 0;
}

Vector4::Vector4(r32 X, r32 Y, r32 Z, r32 W)
{
	Elements[0] = X;
	Elements[1] = Y;
	Elements[2] = Z;
	Elements[3] = W;
}

Vector4::Vector4(Vector3 V3, r32 W)
{
	Elements[0] = V3.X;
	Elements[1] = V3.Y;
	Elements[2] = V3.Z;
	Elements[3] = W;
}

Vector4::Vector4(Vector2 V21, Vector2 V22)
{
	Elements[0] = V21.X;
	Elements[1] = V21.Y;
	Elements[2] = V22.X;
	Elements[3] = V22.Y;
}

Vector4::~Vector4()
{

}

/*----------  Vector2 operators  ----------*/

Vector2 Vector2::operator+(Vector2 const & V2)
{
	return Vector2(X + V2.X, Y + V2.Y);
}

Vector2 Vector2::operator-(Vector2 const & V2)
{
	return Vector2(X - V2.X, Y - V2.Y);
}

/*----------  Vector3 operators  ----------*/

Vector3 Vector3::operator+(Vector3 const & V3)
{
	return Vector3(X + V3.X, Y + V3.Y, Z + V3.Z);
}

Vector3 Vector3::operator-(Vector3 const & V3)
{
	return Vector3(X - V3.X, Y - V3.Y, Z - V3.Z);
}

/*----------  Vector4 operators  ----------*/

Vector4 Vector4::operator+(Vector4 const & V4)
{
	return Vector4(X + V4.X, Y + V4.Y, Z + V4.Z, W + V4.W);
}

Vector4 Vector4::operator-(Vector4 const & V4)
{
	return Vector4(X - V4.X, Y - V4.Y, Z - V4.Z, W - V4.W);
}

/*Vector2 operator+(Vector2 const & A, Vector2 const & B)
{
	return Vector2(A.X + B.X, A.Y + B.Y);
}*/