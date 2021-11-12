#include <Windows.h>
#include <FCCS.h>
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	auto device = FCCS::RHI::CreateRHIDevice();

	return 0;
}