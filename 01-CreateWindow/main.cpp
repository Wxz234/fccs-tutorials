#define UNICODE
#include <Windows.h>
#include <fccs/fccs.h>

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	constexpr uint32_t width = 800, height = 600;
	fccs::window::WindowDesc windowDesc = { width, height, L"fccs" };
	auto window = fccs::window::CreateFCCSWindow(windowDesc);
    window->OpenWindow();
    MSG msg = {};
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            
        }
    }

	fccs::DestroyResource(window);
	return 0;
}
