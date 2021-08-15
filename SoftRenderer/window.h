#pragma once

typedef struct
{
	int width;
	int height;
	int keys[512];
	int exit;
	HWND handle;
	HDC context;
	unsigned char* screen_framebuf;

	HBITMAP dib_cur;				//��ǰλͼ
	HBITMAP dib_old;				//���滻������λͼ������ر�ʱ�滻��ȥ

}Window;

int window_init(Window* window, int w, int h, const TCHAR* title, HINSTANCE hinstance);
int window_close(Window* window);
void window_present(Window* window);
void window_dispatch();
LRESULT CALLBACK window_proc(HWND hWnd, UINT msg, WPARAM wParam, LRESULT lParam);

