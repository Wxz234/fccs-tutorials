#include <Windows.h>
#include <FCCS.h>

class Context : public FCCS::WindowContext {
public:
	void Initialize() {}
	void Update() {}
	void Release() {}
};
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	auto window = FCCS::CreateWindowExW(L"fccs", 800, 600);
	Context context;
	return window->Run(&context);
}