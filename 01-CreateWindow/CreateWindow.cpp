#include <Windows.h>
#include <FCCS.h>
using namespace FCCS;
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {

	RefCountPtr<IWindow> window = CreateWindowExW(L"fccs", 800, 600);
	window->ShowWindow();
	while (window->IsActive()) {

	}
	return 0;
}