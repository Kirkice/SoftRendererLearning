#pragma once

typedef struct
{
	int render_state;			//渲染状态

	int width;					//viewport宽
	int height;					//viewport高

	UINT32** frameBuffer;		//颜色缓冲区 [二维数组]
	float** zBuffer;			//深度缓冲区 [二维数组]
	UINT32** texture;			//贴图缓冲区 [二维数组]

	float texture_u_max;        //当前贴图U坐标最大值：texture_width - 1
	float texture_v_max;        //当前贴图V坐标最大值：texture_height - 1

	UINT32 background;			//背景颜色
	UINT32 foreground;			//前景颜色

	Matrix4x4 world;
	Matrix4x4 view;
	Matrix4x4 projection;
	Matrix4x4 wvp;				// world * view * projection

	Camera camera;

	Light light;

} Device;

void device_init(Device* device, int width, int height, void* fb);
void device_clear(Device* device);
void device_draw_mesh(Device* device, Mesh mesh);

void device_set_light(Device* device, Light light);
void device_set_camera(Device* device, Camera camera);
void device_update_transform(Device* device);

void device_primitive(Device* device, const Vertex* vx1, const Vertex* vx2, const Vertex* vx3);
void device_rasterize(Device* device, Vertex* vx1, Vertex* vx2, Vertex* vx3);

void device_process_vertex(Device* device, Vertex* outvx, const Vertex* vx);
void device_process_fragment(Device* device, const Vertex* vx, float rhw, int x, int y);

void device_draw_scanline(Device* device, float y, Vertex* vxa, Vertex* vxb, Vertex* vxc, Vertex* vxd);
void device_draw_pixel(Device* device, int x, int y, UINT32 color);
void device_draw_line(Device* device, Vector4* v1, Vector4* v2);

int device_cvv_cut(const Vector4* v);
void device_project_viewport(Device* device, Vector4* outv, const Vector4* v);

UINT32 device_texture_sample(const Device* device, float u, float v);
void device_set_texture(Device* device, void* bits, long pitch, int w, int h);

Vertex vertex_shader(Vertex vx, Device* device);
UINT32 fragment_shader(Vertex vx, Device* device);
