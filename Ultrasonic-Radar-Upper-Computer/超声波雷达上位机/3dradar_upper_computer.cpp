/*
* 3dradar_upper_computer.cpp
*
* Created:	2023/05/12 19:20
*  Author:	Bonjir
*    Note:
*	# Communication Protocol£º
*	* // package: length of data package (1 byte) + data package (len bytes)
*	* //data package:
*	* {[CMD](cmd_type)}
*	* {[DAT](dat_type)data}
*	* {[ACK]}
*	# Predefine: _CRT_SECURE_NO_WARNINGS  WIN32_LEAN_AND_MEAN
*/

#include <windows.h>
#include <windowsx.h>
#include <gl/glut.h>
#include <cmath>
#include <cstdio>
#include <string>
#include <ctime>
#include "serialport.h"
#include <WinBase.h>
using std::string;
using std::wstring;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
// OpenGL func
void Render();
void Initialize();
GLvoid KillGLWindow(HWND hWnd, HGLRC hRC, HDC hDC);
void SetupPixelFormat(HDC hDC);

void CALLBACK DealWithKeyBdAndMouseFunc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);
void CALLBACK DealWithSerialFunc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);
POINT GetWindowCenterClientPos(HWND hWnd);
void SetCursorCenterPos(HWND hWnd);
void ClipMouseAndContinueGame(HWND hMain);
void UnclipMouseAndPause(HWND hMain);
void ReadComboBoxAndOpenSerialPort(HWND hWnd);
void CloseSerialPort();

#define MAP_EDGE 20
#define OBSERVER_HEIGHT 2
#define RADIUS 5
#define HORIZONTAL_SPEED 0.1
#define VERTICAL_SPEED 0.1
#define SPEED_UP_RATIO 2

#define DEGREE_STEP 3
#define DAT_ARR_LEN (180 / DEGREE_STEP + 1)

HDC g_HDC;
CSerialPort serial;

bool mouse_clipped_and_gaming = 0;
bool key[256];
float angle[2]/*roundY:0;up:1*/, eye[3];
float points_data[DAT_ARR_LEN][DAT_ARR_LEN][3] = { 0 };
int distances[DAT_ARR_LEN][DAT_ARR_LEN];
float virtual_real_ratio = 1;
float current_scanning_phi, current_scanning_theta;

int WINAPI WinMain(	_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
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
	return (int)msg.wParam;
}

#define IDT_DEAL_WITH_KEYBOARD_MSG	0
#define IDT_DEAL_WITH_SERIAL_MSG	1
#define IDC_S_SERIALPORT	0
#define IDC_S_BAUDRATE		1
#define IDC_CB_SERIALPORT	2
#define IDC_CB_BAUDRATE		3
#define IDC_B_SERIALSWITCH	4

HWND hSSerialPort, hSBaudRate, hCBSerialPort, hCBBaudRate, hBSerialSwitch;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HGLRC hRC;
	static HDC hDC;
	static HINSTANCE hInst = GetModuleHandle(NULL);

	switch (message)
	{
	case WM_CREATE:
	{
		hSSerialPort = CreateWindowEx
		(
			0,
			TEXT("static"), TEXT("SerialPort"),
			WS_CHILD | WS_VISIBLE | WS_BORDER | BS_CENTER,
			50, 25, 200, 50,
			hWnd, (HMENU)IDC_S_SERIALPORT, hInst, NULL
		);
		hSBaudRate = CreateWindowEx
		(
			0,
			TEXT("static"), TEXT("BaudRate"),
			WS_CHILD | WS_VISIBLE | WS_BORDER | BS_CENTER,
			50, 100, 200, 50,
			hWnd, (HMENU)IDC_S_BAUDRATE, hInst, NULL
		);
		hCBSerialPort = CreateWindowEx
		(
			0,
			TEXT("comboBox"), TEXT("SerialPortCB"),
			WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWNLIST & ~CBS_DISABLENOSCROLL,
			300, 25, 200, 100,
			hWnd, (HMENU)IDC_CB_SERIALPORT, hInst, NULL
		);
		hCBBaudRate = CreateWindowEx
		(
			0,
			TEXT("comboBox"), TEXT("BaudRateCB"),
			WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWNLIST & ~CBS_DISABLENOSCROLL,
			300, 100, 200, 100,
			hWnd, (HMENU)IDC_CB_BAUDRATE, hInst, NULL
		);
		hBSerialSwitch = CreateWindowEx
		(
			0,
			TEXT("button"), TEXT("SerialPort On"),
			WS_CHILD | WS_VISIBLE | WS_BORDER | BS_CENTER,
			50, 300, 200, 50,
			hWnd, (HMENU)IDC_B_SERIALSWITCH, hInst, NULL
		);

		ComboBox_AddString(hCBSerialPort, L"COM3");
		ComboBox_AddString(hCBSerialPort, L"COM4");
		ComboBox_AddString(hCBSerialPort, L"COM5");
		ComboBox_SetCurSel(hCBSerialPort, 0);

		ComboBox_AddString(hCBBaudRate, L"9600");
		ComboBox_AddString(hCBBaudRate, L"115200");
		ComboBox_SetCurSel(hCBBaudRate, 0);

		hDC = GetDC(hWnd);
		g_HDC = hDC;
		SetupPixelFormat(hDC);
		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);

		SetTimer(hWnd, IDT_DEAL_WITH_KEYBOARD_MSG, 1, (TIMERPROC)DealWithKeyBdAndMouseFunc);
		SetTimer(hWnd, IDT_DEAL_WITH_SERIAL_MSG, 1, (TIMERPROC)DealWithSerialFunc);
		break;
	}

	case WM_COMMAND:
	{
		switch (LOWORD(wParam)) {
		case IDC_B_SERIALSWITCH:
		{
			SetFocus(hWnd);

			if (serial.IsPortOpen())
			{
				CloseSerialPort();
			}
			else
			{
				ReadComboBoxAndOpenSerialPort(hWnd);
			}
			break;
		}
		case IDC_CB_BAUDRATE:
		case IDC_CB_SERIALPORT:
		{
			CloseSerialPort();
			break;
		}
		default:
		{
			break;
		}
		}
		break;
	}

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
			mouse_clipped_and_gaming = !mouse_clipped_and_gaming;
			if (mouse_clipped_and_gaming == 1)
				ClipMouseAndContinueGame(hWnd);
			else
				UnclipMouseAndPause(hWnd);
		}
		break;
	}

	case WM_ACTIVATE:
	{
		if (wParam)
			break;

		// window lose focus
		mouse_clipped_and_gaming = 0;
		UnclipMouseAndPause(hWnd);
		break;
	}

	case WM_CLOSE:
	{
		wglMakeCurrent(hDC, NULL);
		wglDeleteContext(hRC);
		PostQuitMessage(0);
		KillGLWindow(hWnd, hRC, hDC);
		break;
	}

	case WM_SIZE:
	{
		int height = HIWORD(lParam);
		int width = LOWORD(lParam);
		if (height == 0)
			height = 1;
		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(54.0f, (GLfloat)width / (GLfloat)height, 1.0f, 1000.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		break;
	}

	default:
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	}
	return (DefWindowProc(hWnd, message, wParam, lParam));
}

void Initialize()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
}

GLvoid KillGLWindow(HWND hWnd, HGLRC hRC, HDC hDC)
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hWnd, hDC);
	DestroyWindow(hWnd);
}

void Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0, OBSERVER_HEIGHT, 0,
		cos(angle[1] * 3.14 / 180) * RADIUS * sin(angle[0] * 3.14 / 180),
		OBSERVER_HEIGHT + sin(angle[1] * 3.14 / 180) * RADIUS,
		cos(angle[1] * 3.14 / 180) * RADIUS * cos(angle[0] * 3.14 / 180),
		0, 100, 0);

	glTranslatef(-eye[0], -eye[1], -eye[2]);

	// draw horizontal plane
	glLineWidth(0.5);
	for (float i = -MAP_EDGE; i <= MAP_EDGE; i += 1) {
		glBegin(GL_LINES);
		glColor3f(0, 0, 1);
		glVertex3f(i, 0, -MAP_EDGE);
		glColor3f(1, 0, 0);
		glVertex3f(i, 0, MAP_EDGE);
		glEnd();
		glBegin(GL_LINES);
		glColor3f(0, 0, 1);
		glVertex3f(-MAP_EDGE, 0, i);
		glColor3f(1, 0, 0);
		glVertex3f(MAP_EDGE, 0, i);
		glEnd();
	}

	glPointSize(5);
	glBegin(GL_POINTS);
	glColor3f(1, 1, 0);
	glVertex3f(0, 0, 0);
	glEnd();
	// draw the data points
	// note : (x, y, z) in opengl equals to (_y, _z, _x) in math
	glPointSize(3);
	glBegin(GL_POINTS);
	glColor3f(1, 1, 1);
	for (int deg_z = 0; deg_z <= 180; deg_z += DEGREE_STEP)
	{
		for (int deg_r = 0; deg_r <= 180; deg_r += DEGREE_STEP)
		{
			/*if (distances[deg_z / DEGREE_STEP][deg_r / DEGREE_STEP] <= 1700)
			{*/
			GLfloat _x = (GLfloat)points_data[deg_z / DEGREE_STEP][deg_r / DEGREE_STEP][0] * virtual_real_ratio,
				_y = (GLfloat)points_data[deg_z / DEGREE_STEP][deg_r / DEGREE_STEP][1] * virtual_real_ratio,
				_z = (GLfloat)points_data[deg_z / DEGREE_STEP][deg_r / DEGREE_STEP][2] * virtual_real_ratio;

			glVertex3f(_y, _z, _x);

			/* } */
		}
	}
	glEnd();


	glLineWidth(1);
	glBegin(GL_LINES);
	glColor3f(0.5, 1, 0.5);
	// note : (x, y, z) in opengl equals to (_y, _z, _x) in math
	glVertex3f(0, 0, 0);
	glVertex3f
	(
		MAP_EDGE * sin(current_scanning_theta) * sin(current_scanning_phi),
		MAP_EDGE * cos(current_scanning_theta),
		MAP_EDGE * sin(current_scanning_theta) * cos(current_scanning_phi)
	);
	glEnd();


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

POINT GetWindowCenterClientPos(HWND hWnd)
{
	RECT wrect;
	GetWindowRect(hWnd, &wrect);
	int width = wrect.right - wrect.left, height = wrect.bottom - wrect.top;

	POINT p;
	p.x = width / 2;
	p.y = height / 2;
	return p;
}

void SetCursorCenterPos(HWND hWnd)
{
	POINT p = GetWindowCenterClientPos(hWnd);
	ClientToScreen(hWnd, &p);
	SetCursorPos(p.x, p.y);
}

void CALLBACK DealWithKeyBdAndMouseFunc(HWND hWnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	if (mouse_clipped_and_gaming == 0)
	{
		return;
	}

	// keyboard messages
	bool bSpeedUp = key[VK_CONTROL];
	if (key['W']) {
		eye[0] += (float)(sin(angle[0] * 3.14 / 180)) * HORIZONTAL_SPEED * (bSpeedUp ? SPEED_UP_RATIO : 1);
		eye[2] += (float)cos(angle[0] * 3.14 / 180) * HORIZONTAL_SPEED * (bSpeedUp ? SPEED_UP_RATIO : 1);
	}
	else if (key['S']) {
		eye[0] -= (float)sin(angle[0] * 3.14 / 180) * HORIZONTAL_SPEED * (bSpeedUp ? SPEED_UP_RATIO : 1);
		eye[2] -= (float)cos(angle[0] * 3.14 / 180) * HORIZONTAL_SPEED * (bSpeedUp ? SPEED_UP_RATIO : 1);
	}
	if (key['A']) {
		eye[0] += (float)sin((90 + angle[0]) * 3.14 / 180) * HORIZONTAL_SPEED * (bSpeedUp ? SPEED_UP_RATIO : 1);
		eye[2] += (float)cos((90 + angle[0]) * 3.14 / 180) * HORIZONTAL_SPEED * (bSpeedUp ? SPEED_UP_RATIO : 1);
	}
	else if (key['D']) {
		eye[0] -= (float)sin((90 + angle[0]) * 3.14 / 180) * HORIZONTAL_SPEED * (bSpeedUp ? SPEED_UP_RATIO : 1);
		eye[2] -= (float)cos((90 + angle[0]) * 3.14 / 180) * HORIZONTAL_SPEED * (bSpeedUp ? SPEED_UP_RATIO : 1);
	}
	if (key[VK_SPACE]) {
		eye[1] += VERTICAL_SPEED * (bSpeedUp ? SPEED_UP_RATIO : 1);
	}
	if (key[VK_SHIFT])
		eye[1] -= VERTICAL_SPEED * (bSpeedUp ? SPEED_UP_RATIO : 1);

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

	// mouse messages
	POINT p, pCenter = GetWindowCenterClientPos(hWnd);
	GetCursorPos(&p);
	ScreenToClient(hWnd, &p);
	int change[2] = { -(p.x - pCenter.x) / 2,-(p.y - pCenter.y) / 2 };
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
	ClientToScreen(hWnd, &pCenter);
	SetCursorPos(pCenter.x, pCenter.y);
}

void ClipMouseAndContinueGame(HWND hMain)
{
	SetCursorCenterPos(hMain);
	//ShowCursor(true);
	ShowCursor(false);

	ShowWindow(hSSerialPort, SW_HIDE);
	ShowWindow(hSBaudRate, SW_HIDE);
	ShowWindow(hCBSerialPort, SW_HIDE);
	ShowWindow(hCBBaudRate, SW_HIDE);
	ShowWindow(hBSerialSwitch, SW_HIDE);
}

void UnclipMouseAndPause(HWND hMain)
{
	//ShowCursor(false);
	ShowCursor(true);
	/*HCURSOR hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	SetCursor(hCursor);
	CloseHandle(hCursor);*/ // TODO

	ShowWindow(hSSerialPort, SW_SHOW);
	ShowWindow(hSBaudRate, SW_SHOW);
	ShowWindow(hCBSerialPort, SW_SHOW);
	ShowWindow(hCBBaudRate, SW_SHOW);
	ShowWindow(hBSerialSwitch, SW_SHOW);
}

void ReadComboBoxAndOpenSerialPort(HWND hWnd)
{
	OutputDebugString(L"Open port\n");

	TCHAR szSerialPort[10], szBaudRate[10];
	int serialport, baudrate;
	GetWindowText(hCBSerialPort, szSerialPort, 10);
	GetWindowText(hCBBaudRate, szBaudRate, 10);
	swscanf_s(szSerialPort, L"COM%d", &serialport);
	swscanf_s(szBaudRate, L"%d", &baudrate);


	TCHAR szText[50];
	swprintf_s(szText, L"Try on port COM%d, BaudRate %d\n", serialport, baudrate);
	OutputDebugString(szText);

	int dRet = serial.InitPort(serialport);
	if (dRet == false)
	{
		swprintf_s(szText, L"Fail to open serial port COM%d!", serialport);
		MessageBox(hWnd, szText, L"Error", MB_ICONERROR | MB_TOPMOST);
		return;
	}
	dRet = serial.OpenListenThread();
	if (dRet == false)
	{
		swprintf_s(szText, L"Fail to open listening thread!");
		MessageBox(hWnd, szText, L"Error", MB_ICONERROR | MB_TOPMOST);
		return;
	}

	MessageBox(hWnd, L"Open serial port success!", L"Prompt", MB_ICONINFORMATION | MB_TOPMOST);
	SetWindowText(hBSerialSwitch, L"SerialPort Off");
	OutputDebugString(L"Open serial port success\n");

	//ShowCursor(0); // DEBUG
}

void CloseSerialPort()
{
	OutputDebugString(L"Close port\n");

	SetWindowText(hBSerialSwitch, L"SerialPort On");
	serial.CloseListenTread();
	serial.ClosePort();
}

double deg_to_rad(double deg)
{
	return deg * 3.1415 / 180;
}

void CALLBACK DealWithSerialFunc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	if (serial.IsPortOpen() == 0)
		return;
	if (serial.GetBytesInCOM() == 0)
		return;

	char ch = 0;
	do
		serial.ReadChar(ch);
	while (ch != '{' && serial.GetBytesInCOM());
	if (serial.GetBytesInCOM() == 0)
		return;

	int wait_times = 0;
	wstring sDat;
	sDat += (TCHAR)ch;
	do
	{
		if (serial.GetBytesInCOM() == 0)
		{
			Sleep(1);
			wait_times++;
			if (wait_times > 10)
			{
				TCHAR szOutputText[50];
				swprintf_s(szOutputText, L"GetBytesInCOM Time out, with receiving \"%s\"\n", sDat.c_str());
				OutputDebugString(szOutputText); // DEBUG
				return;
			}
			continue;
		}
		serial.ReadChar(ch);
		sDat += (TCHAR)ch;
	} while (ch != '}');

	OutputDebugString(L"RecvMsg In Serial: ");
	OutputDebugString((sDat + L'\n').c_str()); // DEBUG

	TCHAR szPackageType[20] = { 0 };
	memset(szPackageType, 0, sizeof(szPackageType));
	int distance, deg_z, deg_r;
	int dRet = swscanf(sDat.c_str(), L"{[%3s]", szPackageType);
	if (wcscmp(szPackageType, L"DAT") == 0) // Is Data Package
	{
		dRet = swscanf(sDat.c_str(), L"{[DAT](%3s)", szPackageType);
		if (wcscmp(szPackageType, L"dst") == 0) // Is Distance Package
		{
			sDat.erase(0, 11);
			swscanf_s(sDat.c_str(), L"degz:%d;degr:%d;dist:%d}", &deg_z, &deg_r, &distance);
			//TCHAR szOutputText[100];
			//swprintf_s(szOutputText, L"Package Analysed: (degz, degr, dist) = (%d, %d, %d)\n", deg_z, deg_r, distance);
			//OutputDebugString(szOutputText); // DEBUG

			distance = min(distance, 1700); // abandon large error data

			const double r_offset = 53.6; // the length of servo arm
			double phi, theta, _x, _y, _z, _r;
			if (deg_r <= 90)
				phi = deg_to_rad(deg_z), theta = deg_to_rad(90 - deg_r);
			else
				phi = deg_to_rad(deg_z + 180), theta = deg_to_rad(deg_r - 90);
			_r = distance;
			_z = _r * cos(theta);
			_x = _r * sin(theta) * cos(phi);
			_y = _r * sin(theta) * sin(phi);

			if (deg_r <= 90)
			{
				_x += r_offset * sin(phi);
				_y -= r_offset * cos(phi);
			}
			else
			{
				_x -= r_offset * sin(phi);
				_y += r_offset * cos(phi);
			}

			phi = deg_to_rad(deg_z + 180), theta = deg_to_rad(deg_r - 90);
			points_data[deg_z / DEGREE_STEP][deg_r / DEGREE_STEP][0] = _x;
			points_data[deg_z / DEGREE_STEP][deg_r / DEGREE_STEP][1] = _y;
			points_data[deg_z / DEGREE_STEP][deg_r / DEGREE_STEP][2] = _z;

			virtual_real_ratio = min(virtual_real_ratio, MAP_EDGE / abs(_x));
			virtual_real_ratio = min(virtual_real_ratio, MAP_EDGE / abs(_y));

			current_scanning_phi = phi;
			current_scanning_theta = theta;

			distances[deg_z / DEGREE_STEP][deg_r / DEGREE_STEP] = distance;
		}
	}
}
