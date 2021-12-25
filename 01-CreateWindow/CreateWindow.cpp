#include <Windows.h>
#include <FCCS.h>
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	auto window = FCCS::CreateFCCSWindow(L"fccs", 800, 600);
	while (window->IsRun()) {}
	FCCS::DestroyFObject(window);
	return 0;
}