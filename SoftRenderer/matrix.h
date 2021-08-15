#pragma once

void matrix4x4_identity(Matrix4x4* outm);
void matrix4x4_zero(Matrix4x4* outm);
void matrix4x4_mul_matrix44(Matrix4x4* outm, const Matrix4x4* m1, const Matrix4x4* m2);
void matrix4x4_perspective(Matrix4x4* outm, float fovy, float aspect, float zn, float zf);
void matrix4x4_view(Matrix4x4* outm, const Vector3* eye, const Vector3* lookat, const Vector3* up);
void matrix4x4_rotate_axis(Matrix4x4* outm, Vector3 axis, float angle);
void matrix4x4_rotation_x(Matrix4x4* outm, float angle);
void matrix4x4_rotation_y(Matrix4x4* outm, float angle);
void matrix4x4_rotation_z(Matrix4x4* outm, float angle);
void matrix4x4_rotation(Matrix4x4* outm, float pitch, float yaw, float roll);