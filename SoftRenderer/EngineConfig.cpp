#include "EngineConfig.h"

SoftRenderer m_SoftRender;

int Init(int width, int height, const TCHAR* title, HINSTANCE hinstance)
{
	memset(&m_SoftRender, 0, sizeof(SoftRenderer));

	m_SoftRender.fps = 0;
	m_SoftRender.frame_time_last = 0;

	if (window_init(&m_SoftRender.window, width, height, title, hinstance))
	{
		printf("window init failed");
		return -1;
	}

	device_init(&m_SoftRender.device, width, height, m_SoftRender.window.screen_framebuf);
	m_SoftRender.render_tick = NULL;

	return 0;
}

void Start(AppTick tick)
{
	m_SoftRender.render_tick = tick;

	while (m_SoftRender.window.exit == MRE_FALSE)
	{
		float now = (float)GetTickCount() * 0.001f;
		float time_delta = now - m_SoftRender.frame_time_last;
		Tick(time_delta);
	}
}

void Tick(float time_delta)
{
	//处理Windows消息
	window_dispatch();


	//清理缓冲区
	device_clear(&m_SoftRender.device);

	//提交渲染数据
	if (m_SoftRender.render_tick != NULL)
	{
		m_SoftRender.render_tick(time_delta);
	}

	//present
	window_present(&m_SoftRender.window);

	//更新FPS
	FPS();
}

void FPS()
{
	float now = (float)GetTickCount() * 0.001f;

	if (now - m_SoftRender.frame_time_last > 1.0f)
	{
		printf("FPS: %f\n", m_SoftRender.fps);
		m_SoftRender.frame_time_last = now;
		m_SoftRender.fps = 0;
	}
	else
	{
		m_SoftRender.fps++;
	}
}