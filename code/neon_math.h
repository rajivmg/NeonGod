#ifndef NEON_MATH_H
#define NEON_MATH_H

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
	~Vector2();
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
	~Vector3();
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
	~Vector4();
};

// Vector2 operator+(Vector2 const & A, Vector2 const & B);

#endif