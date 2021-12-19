#include <Windows.h>
#include <FCCS.h>
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	auto window = FCCS::CreateWindowExW(L"fccs", 800, 600);
	auto renderer = FCCS::CreateRenderer(window);
	auto device = renderer->GetDevice();

	while (window->IsRun()) {
		renderer->Begin();
		renderer->End();
	}
	FCCS::DestroyObject(renderer);
	FCCS::DestroyObject(window);
	return 0;
}