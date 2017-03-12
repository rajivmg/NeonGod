#ifndef NEON_MATH_H
#define NEON_MATH_H

/*r32 MapRange(r32 V, r32 StartA, r32 StartB, r32 TargetA, r32 TargetB)
{
	return(TargetA + ((StartB - StartA)/(TargetB- TargetA)) * (V - StartA));
}
*/

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////
//// NOTE:
////	1. Matrix ops are row ordered
////
////

union Vector2
{
	r32 Elements[2];
	struct
	{
		r32 X, Y;
	};
	struct
	{
		r32 S, T;
	};
	struct
	{
		r32 U, V;
	};
	inline Vector2();
	inline Vector2(r32 X, r32 Y);
	inline Vector2 operator+(Vector2 const & V2);
	inline Vector2 operator-(Vector2 const & V2);
	inline Vector2 operator-();
	inline ~Vector2();
};

union Vector3
{
	r32 Elements[3];
	struct
	{
		r32 X, Y, Z;
	};
	struct
	{
		r32 R, G, B;
	};
	inline Vector3();
	inline Vector3(r32 X, r32 Y, r32 Z);
	inline Vector3(Vector2 V2, r32 Z);
	inline Vector3 operator+(Vector3 const & V3);
	inline Vector3 operator-(Vector3 const & V3);
	inline Vector3 operator-();
	inline ~Vector3();
};

union Vector4
{
	r32 Elements[4];
	struct
	{
		r32 X, Y, Z, W;
	};
	struct
	{
		r32 R, G, B, A;
	};
	inline Vector4();
	inline Vector4(r32 X, r32 Y, r32 Z, r32 W);
	inline Vector4(Vector2 V21, Vector2 V22);
	inline Vector4(Vector3 V3, r32 W);
	inline Vector4 operator+(Vector4 const & V4);
	inline Vector4 operator-(Vector4 const & V4);
	inline Vector4 operator-();
	inline ~Vector4();
};

union Matrix2
{
	r32 Elements[4];
	struct
	{
		r32 _00, _01;
		r32 _10, _11;
	};
	inline Matrix2();
	inline ~Matrix2();
};

union Matrix3
{
	r32 Elements[9];
	struct
	{
		r32 _00, _01, _02;
	 	r32 _10, _11, _12;
	 	r32 _20, _21, _22;
	};
	inline Matrix3();
	inline ~Matrix3();
};

union Matrix4
{
	r32 Elements[16];
	struct
	{
		r32 _00, _01, _02, _03;
		r32 _10, _11, _12, _13;
		r32 _20, _21, _22, _23;
		r32 _30, _31, _32, _33;
	};
	inline Matrix4();
	inline ~Matrix4();
};

// Vector2 operator+(Vector2 const & A, Vector2 const & B);

#endif