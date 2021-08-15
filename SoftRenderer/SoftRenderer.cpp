/* $Header: /SoftRenderer/SoftRenderer.cpp					8/11/21 20:55p Kirk¡¤Zhu			$ */
/*--------------------------------------------------------------------------------------------*
*                                                                                             *
*						Project Name : SoftRenderer                                           *
*                                                                                             *
*						File Name : SoftRenderer.cpp										  *
*                                                                                             *
*                       Programmer : Kirk¡¤Zhu                                                 *
*                                                                                             *
*---------------------------------------------------------------------------------------------*/
#pragma once
#include "EngineConfig.h"

Mesh mesh;
Camera camera;
int screen_width = 1280, screen_height = 720;

void tick(float time_delta);
void start();

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow)
{
	TCHAR* title = (TCHAR*)_T("SoftRendererLearning");

	if (Init(screen_width, screen_height, title, hinstance) == MRE_INIT_SUCCEED)
	{
		start();
		Start(&tick);
	}
	return 0;
}

void start()
{
	//Ïà»ú
	float aspect = (float)screen_width / (float)screen_height;
	float fovy = 3.14 / 2.f;
	float clip_near = 1.f;
	float clip_far = 500.f;
	camera = *camera_create_perspective(fovy, aspect, clip_near, clip_far);
	Vector3 eye = { 0.f,0.f,-1.f }, lookat = { 0.f, 0.f, 0.f }, up = { 0.f, 1.f,0.f };
	camera_lookat(&camera, eye, lookat, up);
	device_set_camera(&m_SoftRender.device, camera);
	Vector3 camera_pos = vector3_zero();
	camera_pos.z = -1.5;
	camera_pos.y += 1;
	camera_set_position(&camera, camera_pos);

	//mesh
	mesh = *mesh_create("untitled.obj", "MC003_Kozakura_Mari.bmp");
	Vector3 position = vector3_zero();
	mesh_set_position(&mesh, position);
	Vector3 rotation = vector3_zero();
	mesh_set_rotation(&mesh, rotation);


	//µÆ¹â
	Vector3 position2 = { -10, -10, -10 };
	Color col = { 1,1,1 };
	Light light = *light_create_point(col, position2);
	device_set_light(&m_SoftRender.device, light);

	m_SoftRender.device.render_state = RENDER_STATE_TEXTURE;
}

float x = 3.5;
float y = 0;
void tick(float time_delta)
{
	if (m_SoftRender.window.keys[VK_LEFT])	x += 0.02f;
	if (m_SoftRender.window.keys[VK_RIGHT])	x -= 0.02f;
	if (m_SoftRender.window.keys[VK_UP])	y -= 0.02f;
	if (m_SoftRender.window.keys[VK_DOWN])	y += 0.02f;

	Vector3 rotation = { 0, x, 0 };
	mesh_set_rotation(&mesh, rotation);

	Vector3 position = vector3_zero();
	position.z = y;
	position.y = -0.5;
	mesh_set_position(&mesh, position);

	device_draw_mesh(&m_SoftRender.device, mesh);
}
