#version 330 core
#pragma debug(on)

layout (location = 0) in vec3 dePos;

uniform vec3  mTrs;
uniform float mRotX;
uniform float mRotY;
uniform float mRotZ;
uniform vec3  mScl;

uniform mat4 view;
uniform mat4 proj;

mat4 translate(mat4 identify, vec3 translation)
{
	return identify *
		mat4(
			1.0,           0.0,           0.0,           0.0,
			0.0,           1.0,           0.0,           0.0,
			0.0,           0.0,           1.0,           0.0,
			translation.x, translation.y, translation.z, 1.0
		);
}

mat4 rotateX(mat4 identify, float value)
{
	float rad = radians(value);
	return identify *
		mat4(
			1.0,  0.0,      0.0,      0.0,
			0.0,  cos(rad), sin(rad), 0.0,
			0.0, -sin(rad), cos(rad), 0.0,
			0.0,  0.0,      0.0,      1.0
		);
}

mat4 rotateY(mat4 identify, float value)
{
	float rad = radians(value);
	return identify *
		mat4(
			cos(rad),  0.0, sin(rad), 0.0,
			0.0,       1.0, 0.0,      0.0,
			-sin(rad), 0.0, cos(rad), 0.0,
			0.0,       0.0, 0.0,      1.0
		);
}

mat4 rotateZ(mat4 identify, float value)
{
	float rad = radians(value);
	return identify *
		mat4(
			cos(rad),  sin(rad), 0.0, 0.0,
		   -sin(rad),  cos(rad), 0.0, 0.0,
			0.0,       0.0,      1.0, 0.0,
			0.0,       0.0,      0.0, 1.0
		);
}

mat4 scale(mat4 identify, vec3 scaling)
{
	return identify *
		mat4(
			scaling.x, 0.0,       0.0,       0.0,
			0.0,       scaling.y, 0.0,       0.0,
			0.0,       0.0,       scaling.z, 0.0,
			0.0,       0.0,       0.0,       1.0
		);
}

void main()
{
	mat4 model = translate(mat4(1.0), mTrs);
	     model = rotateX(model, mRotX);
		 model = rotateY(model, mRotY);
		 model = rotateZ(model, mRotZ);
		 model = scale(model, mScl);

	gl_Position = proj * view * model * vec4(dePos, 1.0f);
}
