#pragma once

#include <windows.h>
#include <windowsx.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <string.h>
#include <basetsd.h>
#include <assert.h>

#include "Types.h"
#include "utile.h"
#include "Device.h"
#include "window.h"
#include "camera.h"
#include "vector.h"
#include "vertex.h"
#include "memory.h"
#include "texture.h"
#include "matrix.h"
#include "mesh.h"
#include "light.h"

#define MRE_INDEX_NONE				-1
#define MRE_INIT_SUCCEED			0
#define MRE_TRUE					1
#define MRE_FALSE					0

#define RESOURCES_PATH				"Resources\\"

#define RENDER_STATE_WIREFRAME      1		// 渲染线框
#define RENDER_STATE_TEXTURE        2		// 渲染纹理
#define RENDER_STATE_COLOR          4		// 渲染颜色
#define RENDER_STATE_VERTEX         8		// 渲染顶点

#define TEXTURE_SIZE_MAX	        256		// 最大纹理宽高

typedef void(*AppTick)(float);

typedef struct
{

	Window window;
	Device device;
	float fps;
	float frame_time_last;

	AppTick logic_tick;
	AppTick physics_tick;
	AppTick render_tick;

} SoftRenderer;

extern SoftRenderer m_SoftRender;

int Init(int width, int height, const TCHAR* title, HINSTANCE hinstance);
void Start(AppTick tick);
void Tick(float time_delta);
void FPS();