#include "DE/maths/mat.hpp"
#include "DE/maths/vec.hpp"

namespace deep 
{

	/*
	================
	fmat2x2::fmat2x2
	================
	*/
	fmat2x2::fmat2x2(float _x1, float _y1, float _x2, float _y2)
		: x1(_x1), y1(_y1), x2(_x2), y2(_y2)
	{ }

	/*
	================
	fmat3x3::fmat3x3
	================
	*/
	fmat3x3::fmat3x3(
			float x1, float y1, float z1,
			float x2, float y2, float z2,
			float x3, float y3, float z3
	)
	{ 
		data[0] = x1; data[1] = y1; data[2] = z1;
		data[3] = x2; data[4] = y2; data[5] = z2;
		data[6] = x3; data[7] = y3; data[8] = z3;
	}

	fmat4x4::fmat4x4(
			float x1, float y1, float z1, float w1,
			float x2, float y2, float z2, float w2,
			float x3, float y3, float z3, float w3,
			float x4, float y4, float z4, float w4
	)
	{
		data[to_utype(fmat4x4_index::x1)] = x1; data[to_utype(fmat4x4_index::y1)] = y1; data[to_utype(fmat4x4_index::z1)] = z1; data[to_utype(fmat4x4_index::w1)] = w1;
		data[to_utype(fmat4x4_index::x2)] = x2; data[to_utype(fmat4x4_index::y2)] = y2; data[to_utype(fmat4x4_index::z2)] = z2; data[to_utype(fmat4x4_index::w2)] = w2;
		data[to_utype(fmat4x4_index::x3)] = x3; data[to_utype(fmat4x4_index::y3)] = y3; data[to_utype(fmat4x4_index::z3)] = z3; data[to_utype(fmat4x4_index::w3)] = w3;
		data[to_utype(fmat4x4_index::x4)] = x4; data[to_utype(fmat4x4_index::y4)] = y4; data[to_utype(fmat4x4_index::z4)] = z4; data[to_utype(fmat4x4_index::w4)] = w4;
	}

}
