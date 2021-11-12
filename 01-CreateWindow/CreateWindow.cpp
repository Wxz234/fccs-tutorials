#include <Windows.h>
#include <FCCS.h>
void Frame() {}
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	auto window = FCCS::CreateWindowExW(L"fccs", 800, 600);
	return window->Run(Frame);
}