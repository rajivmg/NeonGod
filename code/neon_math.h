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

union vec2
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
	inline vec2();
	inline vec2(r32 X, r32 Y);
	inline vec2 operator+(vec2 const & V2);
	inline vec2 operator-(vec2 const & V2);
	inline vec2 operator-();
	inline ~vec2();
};

union vec3
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
	inline vec3();
	inline vec3(r32 X, r32 Y, r32 Z);
	inline vec3(vec2 V2, r32 Z);
	inline vec3 operator+(vec3 const & V3);
	inline vec3 operator-(vec3 const & V3);
	inline vec3 operator-();
	inline ~vec3();
};

union vec4
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
	inline vec4();
	inline vec4(r32 X, r32 Y, r32 Z, r32 W);
	inline vec4(vec2 V21, vec2 V22);
	inline vec4(vec3 V3, r32 W);
	inline vec4 operator+(vec4 const & V4);
	inline vec4 operator-(vec4 const & V4);
	inline vec4 operator-();
	inline ~vec4();
};

union mat2
{
	r32 Elements[4];
	struct
	{
		r32 _00, _01;
		r32 _10, _11;
	};
	inline mat2();
	inline ~mat2();
};

union mat3
{
	r32 Elements[9];
	struct
	{
		r32 _00, _01, _02;
	 	r32 _10, _11, _12;
	 	r32 _20, _21, _22;
	};
	inline mat3();
	inline ~mat3();
};

union mat4
{
	r32 Elements[16];
	struct
	{
		r32 _00, _01, _02, _03;
		r32 _10, _11, _12, _13;
		r32 _20, _21, _22, _23;
		r32 _30, _31, _32, _33;
	};
	inline mat4();
	inline ~mat4();
};

// vec2 operator+(vec2 const & A, vec2 const & B);

#endif