#include "EngineConfig.h"

void device_init(Device* device, int width, int height, void* fb) 
{
	int buff_array_head_size = sizeof(void*) * height;
	int texture_array_head_size = sizeof(void*) * 1024;
	int buff_size = width * height * 4;

	int total = buff_array_head_size * 2 + texture_array_head_size + buff_size * 2;
	char* ptr = (char*)malloc(total + 64);

	char* framebuf, * zbuf;

	assert(ptr);
	device->frameBuffer = (UINT32**)ptr;
	device->zBuffer = (float**)(ptr + sizeof(void*) * height);
	ptr += sizeof(void*) * height * 2;
	device->texture = (UINT32**)ptr;
	ptr += sizeof(void*) * 1024;
	framebuf = (char*)ptr;
	zbuf = (char*)ptr + width * height * 4;
	ptr += width * height * 8;

	if (fb != NULL)
	{
		framebuf = (char*)fb;
	}

	for (int i = 0; i < height; i++)
	{
		device->frameBuffer[i] = (UINT32*)(framebuf + width * 4 * i);
		device->zBuffer[i] = (float*)(zbuf + width * 4 * i);
	}

	device->texture[0] = (UINT32*)ptr;
	device->texture[1] = (UINT32*)ptr + 16;

	memset(device->texture[0], 0, 64);

	device->texture_u_max = 1.0f;
	device->texture_v_max = 1.0f;
	device->width = width;
	device->height = height;
	device->background = 0x555555;
	device->foreground = 0x000000;
	device->render_state = RENDER_STATE_TEXTURE;

	matrix4x4_identity(&device->world);
	matrix4x4_identity(&device->projection);
	matrix4x4_identity(&device->view);
}

void device_clear(Device* device)
{
	int y, x;

	//������ɫ������
	for (y = 0; y < device->height; y++)
	{
		UINT32* dst = device->frameBuffer[y];
		UINT32 cc = device->background;

		for (x = device->width; x > 0; dst++, x--)
		{
			dst[0] = cc;
		}
	}

	//������Ȼ�����
	for (y = 0; y < device->height; y++)
	{
		float* dst = device->zBuffer[y];
		for (x = device->width; x > 0; dst++, x--)
		{
			dst[0] = -1.f;
		}
	}
}


void device_draw_mesh(Device* device, Mesh mesh)
{
	device->world = mesh.matrix_world;
	device_update_transform(device);

	device_set_texture(device, mesh.texture.texbuf, TEXTURE_SIZE_MAX * 4, TEXTURE_SIZE_MAX, TEXTURE_SIZE_MAX);

	for (int i = 0; i < (int)mesh.n_face; i++)
	{
		Face face = mesh.faces[i];

		Vertex vx1 = { { 0, 0, 0, 1 },{ 0, 0 },{ 0.5, 0.5, 0.5 }, 1, {0,0,0}, { 0,0,0 } };
		Vertex vx2 = { { 0, 0, 0, 1 },{ 0, 1 },{ 0.5, 0.5, 0.5 }, 1, { 0,0,0 }, { 0,0,0 } };
		Vertex vx3 = { { 0, 0, 0, 1 },{ 1, 1 },{ 0.5, 0.5, 0.5 }, 1, { 0,0,0 }, { 0,0,0 } };

		vx1.pos.x = mesh.vertices[face.vertex_index[0] - 1].x;
		vx1.pos.y = mesh.vertices[face.vertex_index[0] - 1].y;
		vx1.pos.z = mesh.vertices[face.vertex_index[0] - 1].z;

		vx2.pos.x = mesh.vertices[face.vertex_index[1] - 1].x;
		vx2.pos.y = mesh.vertices[face.vertex_index[1] - 1].y;
		vx2.pos.z = mesh.vertices[face.vertex_index[1] - 1].z;

		vx3.pos.x = mesh.vertices[face.vertex_index[2] - 1].x;
		vx3.pos.y = mesh.vertices[face.vertex_index[2] - 1].y;
		vx3.pos.z = mesh.vertices[face.vertex_index[2] - 1].z;

		vx1.pos_world.x = mesh.vertices[face.vertex_index[0] - 1].x;
		vx1.pos_world.y = mesh.vertices[face.vertex_index[0] - 1].y;
		vx1.pos_world.z = mesh.vertices[face.vertex_index[0] - 1].z;

		vx2.pos_world.x = mesh.vertices[face.vertex_index[1] - 1].x;
		vx2.pos_world.y = mesh.vertices[face.vertex_index[1] - 1].y;
		vx2.pos_world.z = mesh.vertices[face.vertex_index[1] - 1].z;

		vx3.pos_world.x = mesh.vertices[face.vertex_index[2] - 1].x;
		vx3.pos_world.y = mesh.vertices[face.vertex_index[2] - 1].y;
		vx3.pos_world.z = mesh.vertices[face.vertex_index[2] - 1].z;

		vector3_mul(&vx1.pos_world, &vx1.pos_world, &device->world);
		vector3_mul(&vx2.pos_world, &vx2.pos_world, &device->world);
		vector3_mul(&vx3.pos_world, &vx3.pos_world, &device->world);

		if (mesh.useuv)
		{
			vx1.texcoord.u = mesh.texcoords[face.texcoord_index[0] - 1].u;
			vx1.texcoord.v = mesh.texcoords[face.texcoord_index[0] - 1].v;
			vx2.texcoord.u = mesh.texcoords[face.texcoord_index[1] - 1].u;
			vx2.texcoord.v = mesh.texcoords[face.texcoord_index[1] - 1].v;
			vx3.texcoord.u = mesh.texcoords[face.texcoord_index[2] - 1].u;
			vx3.texcoord.v = mesh.texcoords[face.texcoord_index[2] - 1].v;;
		}

		if (mesh.normals)
		{
			vx1.normal.x = mesh.normals[face.normal_index[0] - 1].x;
			vx1.normal.y = mesh.normals[face.normal_index[0] - 1].y;
			vx1.normal.z = mesh.normals[face.normal_index[0] - 1].z;

			vx2.normal.x = mesh.normals[face.normal_index[1] - 1].x;
			vx2.normal.y = mesh.normals[face.normal_index[1] - 1].y;
			vx2.normal.z = mesh.normals[face.normal_index[1] - 1].z;

			vx3.normal.x = mesh.normals[face.normal_index[2] - 1].x;
			vx3.normal.y = mesh.normals[face.normal_index[2] - 1].y;
			vx3.normal.z = mesh.normals[face.normal_index[2] - 1].z;

			vector3_normalize(&face.normal);
			vector3_mul(&face.normal, &face.normal, &device->world);
			Vector3 dir;
			vector3_sub(&dir, &vx1.pos_world, &device->camera.position);
			vector3_normalize(&dir);
			float dot = vector3_dot(&face.normal, &dir);
			if (dot > 0.4)
			{
				continue;
			}
		}

		device_primitive(device, &vx1, &vx2, &vx3);
	}
}

void device_set_light(Device* device, Light light)
{
	device->light = light;
}

void device_set_camera(Device* device, Camera camera)
{
	device->camera = camera;
	device->projection = camera.matrix_projection;
	device->view = camera.matrix_view;
	device_update_transform(device);
}

void device_update_transform(Device* device)
{
	Matrix4x4 m;
	matrix4x4_mul_matrix44(&m, &device->world, &device->view);
	matrix4x4_mul_matrix44(&device->wvp, &m, &device->projection);
}

void device_primitive(Device* device, const Vertex* vx1, const Vertex* vx2, const Vertex* vx3)
{
	//���㴦��
	Vertex processed_vx1 = *vx1;
	Vertex processed_vx2 = *vx2;
	Vertex processed_vx3 = *vx3;
	device_process_vertex(device, &processed_vx1, vx1);
	device_process_vertex(device, &processed_vx2, vx2);
	device_process_vertex(device, &processed_vx3, vx3);

	//��Ҫ�������㴦����CVV�ü����Vector,W����ΪͶӰ�������������
	Vector4 c1, c2, c3;
	c1 = processed_vx1.pos;
	c2 = processed_vx2.pos;
	c3 = processed_vx3.pos;

	//CVV�ü�
	if (device_cvv_cut(&c1) != 0) return;
	if (device_cvv_cut(&c2) != 0) return;
	if (device_cvv_cut(&c3) != 0) return;

	//����ͶӰ�任��ĵ�Ϊ����������ʽ����Ҫת����ͨ����
	Vector4 nc1, nc2, nc3;
	vector4_rhw(&nc1, &c1);
	vector4_rhw(&nc2, &c2);
	vector4_rhw(&nc3, &c3);

	//�����任��CVV�ü��������Ļӳ������Ļ�����
	Vector4 p1, p2, p3;
	//CVV��ͶӰ����Ļ�����
	device_project_viewport(device, &p1, &nc1);
	device_project_viewport(device, &p2, &nc2);
	device_project_viewport(device, &p3, &nc3);

	int render_state = device->render_state;
	if (render_state & RENDER_STATE_WIREFRAME)
	{
		device_draw_line(device, &p1, &p2);
		device_draw_line(device, &p1, &p3);
		device_draw_line(device, &p3, &p2);
	}

	if (render_state & RENDER_STATE_VERTEX)
	{
		device_draw_pixel(device, (int)p1.x, (int)p1.y, device->foreground);
		device_draw_pixel(device, (int)p2.x, (int)p2.y, device->foreground);
		device_draw_pixel(device, (int)p3.x, (int)p3.y, device->foreground);
	}

	if (render_state & (RENDER_STATE_COLOR | RENDER_STATE_TEXTURE))
	{
		//��Ҫ��������դ���Ķ���
		Vertex x1 = processed_vx1;
		Vertex x2 = processed_vx2;
		Vertex x3 = processed_vx3;

		//������Ļ����
		x1.pos = p1;
		x2.pos = p2;
		x3.pos = p3;

		//����ͶӰ�任�������������
		x1.pos.w = c1.w;
		x2.pos.w = c2.w;
		x3.pos.w = c3.w;

		//���ö����ȥW������������RHW
		vertex_rhw(&x1);
		vertex_rhw(&x2);
		vertex_rhw(&x3);

		device_rasterize(device, &x1, &x2, &x3);
	}
}

void device_rasterize(Device* device, Vertex* vx1, Vertex* vx2, Vertex* vx3)
{
	//�������ζ�����������ϵΪ��Ļ����ϵ��ԭ�������Ͻ�
	//ʹ��˳��Ϊ P1-P2-P3, P1������Ļ���������p3��Զ��P2���м䣬
	if (vx1->pos.y > vx2->pos.y)
	{
		Vertex* temp = vx2;
		vx2 = vx1;
		vx1 = temp;
	}
	if (vx2->pos.y > vx3->pos.y)
	{
		Vertex* temp = vx2;
		vx2 = vx3;
		vx3 = temp;
	}
	if (vx1->pos.y > vx2->pos.y)
	{
		Vertex* temp = vx2;
		vx2 = vx1;
		vx1 = temp;
	}

	//�����ߵ�б�ʵ���
	//12 =  p1-p2��ֱ��
	//13 =  p1-p3��ֱ��
	float invSlop_12, invSlop_13;
	if (vx2->pos.y - vx1->pos.y > 0)
	{
		invSlop_12 = (vx2->pos.x - vx1->pos.x) / (vx2->pos.y - vx1->pos.y);
	}
	else
	{
		invSlop_12 = 0;
	}
	if (vx3->pos.y - vx1->pos.y > 0)
	{
		invSlop_13 = (vx3->pos.x - vx1->pos.x) / (vx3->pos.y - vx1->pos.y);
	}
	else
	{
		invSlop_13 = 0;
	}

	//p2λ���ұߵ����
	if (invSlop_12 > invSlop_13)
	{
		//����������Y��
		for (int y = (int)(vx1->pos.y + 1.f); y < (int)(vx3->pos.y + 1.f); y++)
		{
			//��һ�׶Σ���Ⱦƽ��������
			if (y < vx2->pos.y)
			{
				device_draw_scanline(device, y + 1.f, vx1, vx3, vx1, vx2);
			}
			else //��һ�׶Σ���Ⱦƽ��������
			{
				device_draw_scanline(device, y + 1.f, vx1, vx3, vx2, vx3);
			}
		}
	}
	//p2λ����ߵ����
	else
	{
		//����������Y��
		for (int y = (int)(vx1->pos.y + 1.f); y < (int)(vx3->pos.y + 1.f); y++)
		{
			//��һ�׶Σ���Ⱦƽ��������
			if (y < vx2->pos.y)
			{
				device_draw_scanline(device, y + 1.f, vx1, vx2, vx1, vx3);
			}
			else //��һ�׶Σ���Ⱦƽ��������
			{
				device_draw_scanline(device, y + 1.f, vx2, vx3, vx1, vx3);
			}
		}
	}
}

void device_process_vertex(Device* device, Vertex* outvx, const Vertex* vx)
{
	Vertex temp = vertex_shader(*vx, device);
	outvx->pos = temp.pos;
	outvx->col = temp.col;
}

void device_process_fragment(Device* device, const Vertex* vx, float rhw, int x, int y)
{
	int render_state = device->render_state;
	float w = 1.0f / rhw;
	if (render_state & RENDER_STATE_COLOR)
	{
		Vertex vxf = *vx;
		float r = vxf.col.r * w;
		float g = vxf.col.g * w;
		float b = vxf.col.b * w;

		UINT32 temp = fragment_shader(*vx, device);
		device->frameBuffer[y][x] = temp;// (R << 16) | (G << 8) | (B);
	}
	if (render_state & RENDER_STATE_TEXTURE)
	{
		Vertex vxf = *vx;
		vxf.texcoord.u = vx->texcoord.u * w;
		vxf.texcoord.v = vx->texcoord.v * w;

		UINT32 temp = fragment_shader(vxf, device);
		device->frameBuffer[y][x] = temp;
	}
}

void device_draw_scanline(Device* device, float y, Vertex* vxa, Vertex* vxb, Vertex* vxc, Vertex* vxd)
{
	//���ֵ��t
	float gradient1 = (float)vxa->pos.y != vxb->pos.y ? (y - vxa->pos.y) / (vxb->pos.y - vxa->pos.y) : 1;
	float gradient2 = (float)vxc->pos.y != vxd->pos.y ? (y - vxc->pos.y) / (vxd->pos.y - vxc->pos.y) : 1;

	Vertex svx, evx, step;
	vertex_interp(&svx, vxa, vxb, gradient1);
	vertex_interp(&evx, vxc, vxd, gradient2);

	float scanlinewidth = evx.pos.x - svx.pos.x;

	vertex_get_setp(&step, &svx, &evx, scanlinewidth);

	int sx = (int)(svx.pos.x + 1.f);
	int ex = (int)(evx.pos.x + 1.f);
	int width = device->width;
	float* zbuffer = device->zBuffer[(int)y];

	for (int x = sx; x < ex; x++)
	{
		if (x >= 0 && x < width)
		{
			//Z��Ȳ���
			float rhw = svx.rhw;
			if (rhw >= zbuffer[x])
			{
				zbuffer[x] = rhw;
				//ƬԪ����
				device_process_fragment(device, &svx, rhw, x, (int)y);
			}
		}
		vertex_roll(&svx, &step);
		if (x >= width) break;
	}

}

void device_draw_pixel(Device* device, int x, int y, UINT32 color)
{
	if (((UINT32)x) < (UINT32)device->width &&
		((UINT32)y) < (UINT32)device->height)
	{
		device->frameBuffer[y][x] = color;
	}
}

void device_draw_line(Device* device, Vector4* v1, Vector4* v2)
{
	Vector4 v;
	vector4_sub(&v, v2, v1);
	int distance = (int)vector4_length(&v);

	if (distance < 1)
	{
		return;
	}

	//mid = p1 + (p2 - p1) / 2;
	vector4_div(&v, 2);
	Vector4 mid;
	vector4_add(&mid, v1, &v);

	device_draw_pixel(device, (int)mid.x, (int)mid.y, device->foreground);

	// Recursive  ���c�����c�����c���K�c
	device_draw_line(device, v1, &mid);
	device_draw_line(device, &mid, v2);
}

int device_cvv_cut(const Vector4* v)
{
	//��ǰֻ�ж��˵��Ƿ���CVV��
	float w = v->w;
	int check = 0;
	if (v->z < 0.0f) check |= 1;
	if (v->z > w) check |= 2;
	if (v->x < -w) check |= 4;
	if (v->x > w) check |= 8;
	if (v->y < -w) check |= 16;
	if (v->y > w) check |= 32;
	return check;
}

void device_project_viewport(Device* device, Vector4* outv, const Vector4* v)
{
	//��CVV�ڵĵ�ӳ������Ļ������
	//��Ļ����ϵԭ�������Ͻ�
	outv->x = (v->x + 1.0f) * device->width * 0.5f;
	outv->y = (1.0f - v->y) * device->height * 0.5f;
	outv->z = v->z;
	outv->w = 1.0f;
}

UINT32 device_texture_sample(const Device* device, float u, float v)
{
	int x, y;
	//ӳ��UV ����ʵ��ͼ���
	u = u * device->texture_u_max;
	v = v * device->texture_v_max;
	x = (int)(u + 1.f);
	y = (int)(v + 1.f);
	x = clamp(x, 0, (int)device->texture_u_max);
	y = clamp(y, 0, (int)device->texture_v_max);
	return device->texture[y][x];
}

void device_set_texture(Device* device, void* bits, long pitch, int w, int h)
{
	char* ptr = (char*)bits;
	int j;
	assert(w <= 1024 && h <= 1024);

	for (j = 0; j < h; ptr += pitch, j++)
	{
		device->texture[j] = (UINT32*)ptr;
	}

	device->texture_u_max = (float)(w - 1);
	device->texture_v_max = (float)(h - 1);
}

Vertex vertex_shader(Vertex vx, Device* device)
{
	Light light = device->light;

	Vector3 light_direction;
	vector3_sub(&light_direction, &vx.pos_world, &light.position);
	vector3_normalize(&light_direction);

	Vector3 normal;
	vector3_mul(&normal, &vx.normal, &device->world);
	vector3_normalize(&normal);

	Vector3 CameraPos = { 0,1,-1.5};
	Vector3 viewDir; 
	vector3_sub(&viewDir, &vx.pos_world, &CameraPos);
	Vector3 halfDir; 
	Vector3 lightDir;
	lightDir.x = light_direction.x * -1;
	lightDir.y = light_direction.y * -1;
	lightDir.z = light_direction.z * -1;

	Color specColor;
	specColor.r = 1;
	specColor.g = 0.85;
	specColor.b = 0.8;
	vector3_add(&halfDir, &viewDir, &lightDir);
	vector3_normalize(&halfDir);
	float spec = vector3_dot(&normal, &halfDir);
	spec = pow(spec, 30);

	Color mainColor;
	mainColor.r = 0.95;
	mainColor.g = 0.64;
	mainColor.b = 0.54;

	float ndotl = vector3_dot(&normal, &light_direction);
	ndotl = ndotl * 0.5 + 0.5;
	ndotl = MAX(ndotl, 0);

	Vertex outvx = vx;
	vector4_mul(&outvx.pos, &vx.pos, &device->wvp);

	if (device->render_state == RENDER_STATE_COLOR)
	{
		outvx.col.r = ndotl * light.color.r * vx.col.r + 0.2;
		outvx.col.g = ndotl * light.color.g * vx.col.g + 0.2;
		outvx.col.b = ndotl * light.color.b * vx.col.b + 0.2;
	}
	else if (device->render_state == RENDER_STATE_TEXTURE)
	{
		outvx.col.r = ndotl * light.color.r * mainColor.r + spec * specColor.r;
		outvx.col.g = ndotl * light.color.g * mainColor.g + spec * specColor.g;
		outvx.col.b = ndotl * light.color.b * mainColor.b + spec * specColor.b;
	}

	return outvx;
}

UINT32 fragment_shader(Vertex vx, Device* device)
{
	Light light = device->light;
	float r, g, b;
	UINT32 color;
	if (device->render_state == RENDER_STATE_COLOR)
	{
		r = (int)(vx.col.r * 255.0f);
		g = (int)(vx.col.g * 255.0f);
		b = (int)(vx.col.b * 255.0f);

		r = clamp(r, 0, 255);
		g = clamp(g, 0, 255);
		b = clamp(b, 0, 255);
	}
	else if (device->render_state == RENDER_STATE_TEXTURE)
	{
		//color = device_texture_sample(device, vx.texcoord.u, vx.texcoord.v);

		//r = color >> 16 & 255;
		//g = color >> 8 & 255;
		//b = color >> 0 & 255;

		r = vx.col.r * 255;
		g = vx.col.g * 255;
		b = vx.col.b * 255;


		r = clamp(r, 0, 255);
		g = clamp(g, 0, 255);
		b = clamp(b, 0, 255);
	}
	color = ((int)r << 16) | ((int)g << 8) | ((int)b);
	return color;
}
