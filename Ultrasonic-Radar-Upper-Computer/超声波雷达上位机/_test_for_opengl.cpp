#include <windows.h>
#include <gl/glut.h>
#include <cmath>

#define WIN32_LEAN_AND_MEAN

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void Render();

HDC g_HDC;
//float legAngle[2] = { 0.0f,0.0f };
//float armAngle[2] = { 0.0f,0.0f };
//float ambientLight[] = { 0.1f,0.3f,0.8f,1.0f };  //环境光
//float diffuseLight[] = { 0.25f,0.25f,0.25f,1.0f }; //散射光
//float lightPosition[] = { 1.0f,1.0f,1.0f,0.0f }; //光源位置
//float matAmbient[] = { 1.0f,1.0f,1.0f,1.0f };
//float matDiff[] = { 1.0f,1.0f,1.0f,1.0f };

void Initialize()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   //清理颜色为黑色
	glShadeModel(GL_SMOOTH);     //使用平滑明暗处理
	glEnable(GL_DEPTH_TEST);     //剔除隐藏面
	glEnable(GL_CULL_FACE);      //不计算多边形背面
	glFrontFace(GL_CCW);      //多边形逆时针方向为正面
	//glEnable(GL_LIGHTING);      //启用光照
	////为LIGHT0设置析质
	//glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiff);
	////现在开始调协LIGHT0
	//glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight); //设置环境光分量
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight); //设置散射光分量
	//glLightfv(GL_LIGHT0, GL_POSITION, lightPosition); //设置光源在场景中的位置
	////启用光
	//glEnable(GL_LIGHT0);
}

GLvoid KillGLWindow(HWND hWnd, HGLRC hRC, HDC hDC)
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hWnd, hDC);
	DestroyWindow(hWnd);
}

float trans = 3.0;
bool key[256];
#define edge 20
#define Height 2
#define Radius 5
#define Speed 0.1
float angle[2]/*roundY:0;up:1*/, eye[3];

#define VERTICAL_SPEED 0.1

void Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();     //复位旋转角度计数器
	gluLookAt(0, Height, 0,
		cos(angle[1] * 3.14 / 180) * Radius * sin(angle[0] * 3.14 / 180),
		Height + sin(angle[1] * 3.14 / 180) * Radius,
		cos(angle[1] * 3.14 / 180) * Radius * cos(angle[0] * 3.14 / 180),
		0, 100, 0);
	if (key['W']) {
		eye[0] += sin(angle[0] * 3.14 / 180) * Speed;
		eye[2] += cos(angle[0] * 3.14 / 180) * Speed;
	}
	else if (key['S']) {
		eye[0] -= sin(angle[0] * 3.14 / 180) * Speed;
		eye[2] -= cos(angle[0] * 3.14 / 180) * Speed;
	}
	if (key['A']) {
		eye[0] += sin((90 + angle[0]) * 3.14 / 180) * Speed;
		eye[2] += cos((90 + angle[0]) * 3.14 / 180) * Speed;
	}
	else if (key['D']) {
		eye[0] -= sin((90 + angle[0]) * 3.14 / 180) * Speed;
		eye[2] -= cos((90 + angle[0]) * 3.14 / 180) * Speed;
	}
	if (key[VK_LEFT])
	{
		if (angle[0] < 360)
			angle[0]++;
		else
			angle[0] = 0;
	}
	if (key[VK_RIGHT])
	{
		if (angle[0] > 0)
			angle[0]--;
		else
			angle[0] = 360;
	}
	if (key[VK_UP])
	{
		if (angle[1] < 90 - 1)
			angle[1]++;
	}
	if (key[VK_DOWN])
	{
		if (angle[1] > -90 + 1)
			angle[1]--;
	}

	if (key[VK_SPACE]) {
		eye[1] += VERTICAL_SPEED;
	}
	if (key[VK_SHIFT])
		eye[1] -= VERTICAL_SPEED;

	glTranslatef(-eye[0], -eye[1], -eye[2]);
	for (float i = -edge; i <= edge; i += 1) {
		glBegin(GL_LINES);
		glColor3f(0, 0, 1);
		glVertex3f(i, 0, -edge);
		glColor3f(1, 0, 0);
		glVertex3f(i, 0, edge);
		glEnd();
		glBegin(GL_LINES);
		glColor3f(0, 0, 1);
		glVertex3f(-edge, 0, i);
		glColor3f(1, 0, 0);
		glVertex3f(edge, 0, i);
		glEnd();
	}
	glFlush();
	SwapBuffers(g_HDC);
	Sleep(6);
}

void SetupPixelFormat(HDC hDC)
{
	int nPixelFormat;
	static PIXELFORMATDESCRIPTOR pfd =
	{
	 sizeof(PIXELFORMATDESCRIPTOR),
	  1,
	  PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
	  PFD_TYPE_RGBA,
	  32,
	 0,0,0,0,0,0,
	 0,
	 0,
	 0,
	 0,0,0,0,
	 16,
	 0,
	 0,
	 PFD_MAIN_PLANE,
	 0,
	 0,0,0
	};
	nPixelFormat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, nPixelFormat, &pfd);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	bool done;
	int bits = 32;
	WNDCLASSEX windowClass;
	HWND hWnd;
	MSG msg;
	//int cxFrame = GetSystemMetrics(SM_CXSIZEFRAME), cyFrame = GetSystemMetrics(SM_CYSIZEFRAME);
	//cxFrame *= 2, cyFrame += GetSystemMetrics(SM_CYCAPTION);
	int cxScr = GetSystemMetrics(SM_CXFULLSCREEN), cyScr = GetSystemMetrics(SM_CYFULLSCREEN);

	int width = cxScr;// -cxFrame;
	int height = cyScr;// -cyFrame;
	{
		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc = WndProc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = hInstance;
		windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		windowClass.hbrBackground = NULL;
		windowClass.lpszMenuName = NULL;
		windowClass.lpszClassName = L"MyClass";
		windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
		if (!RegisterClassEx(&windowClass))
			return 0;
		hWnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
			L"MyClass",
			L"OpenGL Cube",
			WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
			0, 0,
			width, height,
			NULL,
			NULL,
			hInstance,
			NULL);
		if (!hWnd)
			return 0;
		ShowWindow(hWnd, SW_SHOW);
		ShowWindow(hWnd, SW_MAXIMIZE);
		UpdateWindow(hWnd);
	}
	Initialize();

	done = false;
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				done = TRUE;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			Render();
		}
	}
	return msg.wParam;
}
bool mouse_clipped = 1;


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HGLRC hRC;
	static HDC hDC;
	RECT wrect;
	GetWindowRect(hwnd, &wrect);
	int width = wrect.right - wrect.left, height = wrect.bottom - wrect.top;
	switch (message)
	{
	case WM_CREATE:
		hDC = GetDC(hwnd);
		g_HDC = hDC;
		SetupPixelFormat(hDC);
		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);

		// SetCursorPos to the center of window
		POINT p;
		p.x = width / 2;
		p.y = height / 2;
		ClientToScreen(hwnd, &p);
		SetCursorPos(p.x, p.y);
		ShowCursor(false);

		break;

	case WM_KEYUP:
	{
		key[wParam] = 0;
		break;
	}
	case WM_KEYDOWN:
	{
		key[wParam] = 1;
		if (wParam == VK_ESCAPE)
		{
			mouse_clipped = !mouse_clipped;
			if (mouse_clipped == 1)
			{
				// SetCursorPos to the center of window
				POINT p;
				p.x = width / 2;
				p.y = height / 2;
				ClientToScreen(hwnd, &p);
				SetCursorPos(p.x, p.y);
				ShowCursor(false);
			}
			else
				ShowCursor(true);
		}
		break;
	}
	case WM_ACTIVATE:
	{
		if (wParam)
			break;
		// window lose focus
		mouse_clipped = 0;
		ShowCursor(true);
		break;
	}
	case WM_MOUSEMOVE:
	{
		if (mouse_clipped) {
			POINT p;
			GetCursorPos(&p);
			ScreenToClient(hwnd, &p);
			int change[2] = { -(p.x - width / 2) / 2,-(p.y - height / 2) / 2 };
			if (change[0] > 0)
				if (angle[0] + change[0] < 360)angle[0] += change[0];
				else angle[0] += change[0] - 360;
			else
				if (angle[0] + change[0] > 0)angle[0] += change[0];
				else angle[0] = 360 + angle[0] + change[0];

			if (change[1] > 0)
				if (angle[1] + change[1] < 90)angle[1] += change[1]; else;
			else
				if (angle[1] + change[1] > -90)angle[1] += change[1];
			p.x = width / 2;
			p.y = height / 2;
			ClientToScreen(hwnd, &p);
			SetCursorPos(p.x, p.y);
		}
		break;
	}
	case WM_CLOSE:
		wglMakeCurrent(hDC, NULL);
		wglDeleteContext(hRC);
		PostQuitMessage(0);
		KillGLWindow(hwnd, hRC, hDC);
		break;

	case WM_SIZE:
		height = HIWORD(lParam);
		width = LOWORD(lParam);
		if (height == 0)
			height = 1;
		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(54.0f, (GLfloat)width / (GLfloat)height, 1.0f, 1000.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		break;
	default:return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return (DefWindowProc(hwnd, message, wParam, lParam));
}