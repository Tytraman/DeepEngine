#include <DE/mat.hpp>

namespace de {

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

}
