#include "EngineConfig.h"


void matrix4x4_identity(Matrix4x4* outm)
{
	outm->m[0][0] = outm->m[1][1] = outm->m[2][2] = outm->m[3][3] = 1.0f;
	outm->m[0][1] = outm->m[0][2] = outm->m[0][3] = 0.0f;
	outm->m[1][0] = outm->m[1][2] = outm->m[1][3] = 0.0f;
	outm->m[2][0] = outm->m[2][1] = outm->m[2][3] = 0.0f;
	outm->m[3][0] = outm->m[3][1] = outm->m[3][2] = 0.0f;
}

void matrix4x4_zero(Matrix4x4* outm)
{
	outm->m[0][0] = outm->m[0][1] = outm->m[0][2] = outm->m[0][3] = 0.0f;
	outm->m[1][0] = outm->m[1][1] = outm->m[1][2] = outm->m[1][3] = 0.0f;
	outm->m[2][0] = outm->m[2][1] = outm->m[2][2] = outm->m[2][3] = 0.0f;
	outm->m[3][0] = outm->m[3][1] = outm->m[3][2] = outm->m[3][3] = 0.0f;
}

void matrix4x4_mul_matrix44(Matrix4x4* outm, const Matrix4x4* m1, const Matrix4x4* m2)
{
	Matrix4x4 temp;
	int i, j;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			temp.m[j][i] = (m1->m[j][0] * m2->m[0][i]) +
				(m1->m[j][1] * m2->m[1][i]) +
				(m1->m[j][2] * m2->m[2][i]) +
				(m1->m[j][3] * m2->m[3][i]);
		}
	}
	outm[0] = temp;
}

/*
cot fovy�İ������
aspect ��Ļ��߱�
f,n   Զ���������
| cot/aspect  0      0        0 |
| 0			  cot	 0        0 |
| 0           0      f/(f-n)  1 |
| 0           0      fn/(n-f) 0 |
*/
void matrix4x4_perspective(Matrix4x4* outm, float fovy, float aspect, float zn, float zf)
{
	float cot = 1.0f / (float)tan(fovy * 0.5f);

	matrix4x4_zero(outm);
	outm->m[0][0] = cot / aspect;
	outm->m[1][1] = cot;
	outm->m[2][2] = zf / (zf - zn);
	outm->m[2][3] = 1.0f;
	outm->m[3][2] = (zn * zf) / (zn - zf);
}

/*
r ��Ӧ���ҷ�������� �൱��x��
u ��Ӧ���Ϸ�������� �൱��y��
d ��Ӧ��ǰ��������� �൱��z��
p ��ӦEyeλ��
| r.x	u.x		d.x       0 |
| r.y	u.y		d.y       0 |
| r.z	u.z		d.z		 0 |
| -P*R	-P*U	-P*D     1 |
*/
void matrix4x4_view(Matrix4x4* outm, const Vector3* eye, const Vector3* lookat, const Vector3* up)
{
	Vector3 r, u, d;

	//������ǰ����
	vector3_sub(&d, lookat, eye);
	vector3_normalize(&d);
	//������������
	vector3_cross(&r, up, &d);
	vector3_normalize(&r);
	//������������
	vector3_cross(&u, &d, &r);

	outm->m[0][0] = r.x;
	outm->m[1][0] = r.y;
	outm->m[2][0] = r.z;
	outm->m[3][0] = -vector3_dot(&r, eye);

	outm->m[0][1] = u.x;
	outm->m[1][1] = u.y;
	outm->m[2][1] = u.z;
	outm->m[3][1] = -vector3_dot(&u, eye);

	outm->m[0][2] = d.x;
	outm->m[1][2] = d.y;
	outm->m[2][2] = d.z;
	outm->m[3][2] = -vector3_dot(&d, eye);

	outm->m[0][3] = outm->m[1][3] = outm->m[2][3] = 0.0f;
	outm->m[3][3] = 1.0f;
}

/*
u  ��ת��x����
v  ��ת��y����
w  ��ת��z����
| uu+(1-uu)cos		uv(1-cos)+wsin		uw(1-cos)-vsin       0 |
| uv(1-cos)-wsin	vv+(1-vv)cos		vw(1-cos)+usin       0 |
| uw(1-cos)+vsin	vw(1-cos)-usin		ww+(1-ww)cos		 0 |
| 0					0					0				     1 |
*/
void matrix4x4_rotate_axis(Matrix4x4* outm, Vector3 axis, float angle)
{
	vector3_normalize(&axis);
	float u = axis.x;
	float v = axis.y;
	float w = axis.z;

	outm->m[0][0] = cosf(angle) + (u * u) * (1 - cosf(angle));
	outm->m[0][1] = u * v * (1 - cosf(angle)) + w * sinf(angle);
	outm->m[0][2] = u * w * (1 - cosf(angle)) - v * sinf(angle);
	outm->m[0][3] = 0;

	outm->m[1][0] = u * v * (1 - cosf(angle)) - w * sinf(angle);
	outm->m[1][1] = cosf(angle) + v * v * (1 - cosf(angle));
	outm->m[1][2] = w * v * (1 - cosf(angle)) + u * sinf(angle);
	outm->m[1][3] = 0;

	outm->m[2][0] = u * w * (1 - cosf(angle)) + v * sinf(angle);
	outm->m[2][1] = v * w * (1 - cosf(angle)) - u * sinf(angle);
	outm->m[2][2] = cosf(angle) + w * w * (1 - cosf(angle));
	outm->m[2][3] = 0;

	outm->m[3][0] = 0;
	outm->m[3][1] = 0;
	outm->m[3][2] = 0;
	outm->m[3][3] = 1;
}

/*
{ 1,	0,				0,				0 },
{ 0,	cos(angle), 	sin(angle),		0 },
{ 0,	-sin(angle),	cos(angle),		0 },
{ 0,	0,				0,				1 }
*/
void matrix4x4_rotation_x(Matrix4x4* outm, float angle)
{

	matrix4x4_zero(outm);

	outm->m[0][0] = outm->m[3][3] = 1;
	outm->m[1][1] = outm->m[2][2] = cosf(angle);
	outm->m[1][2] = sinf(angle);
	outm->m[2][1] = -sinf(angle);
}

/*
 { cos(angle),		0,			-sin(angle),			0 });
 { 0,				1,			0,						0 });
 { sin(angle),		0,			cos(angle),				0 });
 { 0,				0,			0,						1 });
*/
void matrix4x4_rotation_y(Matrix4x4* outm, float angle)
{
	matrix4x4_zero(outm);
	outm->m[1][1] = outm->m[3][3] = 1;
	outm->m[0][0] = outm->m[2][2] = cosf(angle);
	outm->m[0][2] = -sinf(angle);
	outm->m[2][0] = sinf(angle);
}

/*
 { cos(angle),	sin(angle),				0,		0 });
 { -sin(angle),	cos(angle),				0,		0 });
 { 0,					0,				1,		0 });
 { 0,					0,				0,		1 });
*/
void matrix4x4_rotation_z(Matrix4x4* outm, float angle)
{
	matrix4x4_zero(outm);

	outm->m[2][2] = outm->m[3][3] = 1;
	outm->m[0][0] = outm->m[1][1] = cosf(angle);
	outm->m[0][1] = sinf(angle);
	outm->m[1][0] = -sinf(angle);
}


void matrix4x4_rotation(Matrix4x4* outm, float pitch, float yaw, float roll)
{
	Matrix4x4 mx;
	Matrix4x4 my;
	Matrix4x4 mz;

	matrix4x4_rotation_x(&mx, pitch);
	matrix4x4_rotation_y(&my, yaw);
	matrix4x4_rotation_z(&mz, roll);

	matrix4x4_mul_matrix44(outm, &mx, &mz);
	matrix4x4_mul_matrix44(outm, outm, &my);
}
