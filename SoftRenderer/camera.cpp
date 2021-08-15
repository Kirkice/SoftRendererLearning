#include "EngineConfig.h"

Camera* camera_create_perspective(float fovy, float aspect_ratio, float clip_near, float clip_far)
{
	Camera* camera = (Camera*)malloc(sizeof(Camera));
	memset(camera, 0, sizeof(Camera));

	camera->fovy = fovy;
	camera->aspect_ratio = aspect_ratio;
	camera->clip_far = clip_far;
	camera->clip_near = clip_near;

	matrix4x4_identity(&camera->matrix_view);
	matrix4x4_perspective(&camera->matrix_projection, fovy, aspect_ratio, clip_near, clip_far);
	return camera;
}

void camera_destroy(Camera* camera)
{
	if (camera)
	{
		free(camera);
		camera = NULL;
	}
}

void camera_set_position(Camera* camera, Vector3 pos)
{
	camera_lookat(camera, pos, camera->lookat, camera->up_direction);
}

void camera_lookat(Camera* camera, Vector3 eye, Vector3 lookat, Vector3 up)
{
	camera->position = eye;
	camera->lookat = lookat;
	camera->up_direction = up;
	matrix4x4_view(&camera->matrix_view, &camera->position, &camera->lookat, &camera->up_direction);
	device_set_camera(&m_SoftRender.device, *camera);
}
