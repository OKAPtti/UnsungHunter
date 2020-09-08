#include"GameFrame/ArsGameFrame.h"
#include "ArsConfiguration.h"
#include "Input/ArsInput.h"
#include "DirectX/ArsDirectGraphics.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
	LPSTR lpCmdLine, int nCmdShow)
{
	{

		// COM������
		CoInitializeEx(nullptr, COINIT_MULTITHREADED);

		//�E�B���h�E���t���X�N���[���ɂ��邩�ǂ���.
		bool isFullScreen = false;
		if (MessageBox(nullptr, "�t���X�N���[���ɂ��܂����H", "�m�F", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES) {
			isFullScreen = true;
		}

		using namespace Alrescha;

		WNDCLASS wc;
		MSG msg;
		HWND hwnd;
		TCHAR className[] = "GameWindow";

		wc.style = CS_DBLCLKS;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInst;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = className;

		RegisterClass(&wc);

		hwnd = CreateWindow(className, WindowName, WS_OVERLAPPEDWINDOW | WS_THICKFRAME,
			WindowPositionX, WindowPositionY, WindowWidth, WindowHeight,
			NULL, NULL, hInst, NULL);

		if (!hwnd) { _ASSERT_EXPR(false, L"�E�B���h�E�n���h���̍쐬�Ɏ��s���܂���"); }

		ShowWindow(hwnd, nCmdShow);
		UpdateWindow(hwnd);
		SetFocus(hwnd);

		if (FullScreen) {
			ShowCursor(FALSE);
		}
		else {
			RECT rc = { 0,0, static_cast<LONG>(WindowWidth), static_cast<LONG>(WindowHeight) };
			AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
			SetWindowPos(hwnd, NULL, WindowPositionX, WindowPositionY,
				rc.right - rc.left, rc.bottom - rc.top,
				SWP_SHOWWINDOW | SWP_NOZORDER);
		}

		//Direct3D������.
		Get3D().Initialize(hwnd, isFullScreen);

		auto pGameFrame = std::make_unique<GameFrame>();
		pGameFrame->Initialize(hwnd);

		while (1) {
			if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
			{
				if (!GetMessage(&msg, NULL, 0, 0))
					break;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				if (!pGameFrame->Run()) { PostQuitMessage(0); }
			}
		}
		return (int)msg.wParam;
	}

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	using namespace Alrescha;

	switch (uMsg) {
		//�E�B���h�E���j�󂳂ꂽ�Ƃ�.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	GetInput().SetWindowMessage(uMsg, wParam, lParam);

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}