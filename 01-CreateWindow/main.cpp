#include <Windows.h>
#include <fccs/fccs.h>

class Callback : public fccs::window::IWindowCallback {
public:
	void Initialize() {}
	void Update() {}
	void Release() {}
};


int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	auto window = fccs::window::createWindow();
	window->OpenWindow();
	Callback callback;
	return window->Run(&callback);
}
