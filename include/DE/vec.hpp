#ifndef __DEEP_ENGINE_VEC_HPP__
#define __DEEP_ENGINE_VEC_HPP__

#include <DE/def.h>

#include <stdint.h>

namespace de {

	class Renderer;
	struct fmat2x2;
	struct fmat3x3;

	struct DE_API vec2 {
		int32_t x;
		int32_t y;

		vec2(int32_t x = 1, int32_t y = 1);

		void draw(Renderer &renderer) const;
	};

	/*
	==========
	vec2::vec2
	==========
	*/
	inline vec2::vec2(int32_t _x, int32_t _y)
		: x(_x), y(_y)
	{ }

	struct DE_API fvec2 {
		float x;
		float y;

		fvec2(float x = 1.0f, float y = 1.0f);

		void draw(Renderer &renderer) const;

		fvec2 operator+(const fvec2 &vec) const;
		fvec2 operator+(const vec2 &vec) const;
		fvec2 operator+(float valueToAdd) const;
		fvec2 operator-(const fvec2 &vec) const;
		fvec2 operator-(float valueToSub) const;
		fvec2 operator*(float scalar) const;
		fvec2 operator*(const fmat2x2 &mat) const;

		void operator-=(float value);
		void operator+=(float value);
		void operator*=(float scalar);

		static fvec2 add(const fvec2 &vec1, const fvec2 &vec2);
		static fvec2 add(const fvec2 &vec1, const vec2 &vec2);
		static fvec2 add(const fvec2 &vec, float valueToAdd);
		static fvec2 sub(const fvec2 &vec1, const fvec2 &vec2);
		static fvec2 sub(const fvec2 &vec, float valueToSub);
		static fvec2 scale(const fvec2 &vec, float scalar);
		static float dot(const fvec2 &vec1, const fvec2 &vec2);
		static float magn(const fvec2 &vec);
		static float angle(const fvec2 &vec1, const fvec2 &vec2);
		static fvec2 mul(const fvec2 &vec, const fmat2x2 &mat);
		static fvec2 inv(const fvec2 &vec);
		static fvec2 interpolateX(const fvec2 &vec1, const fvec2 &vec2, float y);
		static fvec2 interpolateY(const fvec2 &vec1, const fvec2 &vec2, float x);
	};

	struct DE_API fvec3 {
		float x;
		float y;
		float z;

		fvec3(float x = 0.0f, float y = 0.0f, float z = 1.0f);

		fvec3 operator+(const fvec3 &vec) const;
		fvec3 operator+(float valueToAdd) const;
		fvec3 operator-(const fvec3 &vec) const;
		fvec3 operator-(float valueToSub) const;
		fvec3 operator*(float scalar) const;
		fvec3 operator*(const fmat3x3 &mat) const;
		void operator*=(float scalar);

		static fvec3 add(const fvec3 &vec1, const fvec3 &vec2);
		static fvec3 add(const fvec3 &vec, float valueToAdd);
		static fvec3 sub(const fvec3 &vec1, const fvec3 &vec2);
		static fvec3 sub(const fvec3 &vec, float valueToSub);
		static fvec3 scale(const fvec3 &vec, float scalar);
		static float dot(const fvec3 &vec1, const fvec3 &vec2);
		static float magn(const fvec3 &vec);
		static float angle(const fvec3 &vec1, const fvec3 &vec2);
		static fvec3 mul(const fvec3 &vec, const fmat3x3 &mat);
		static fvec3 inv(const fvec3 &vec);
	};

}

#endif