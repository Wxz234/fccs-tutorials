#define UNICODE
#include <Windows.h>
#include <fccs/fccs.h>

using namespace fccs;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    constexpr uint32_t width = 800, height = 600;
    window::WindowDesc windowDesc = { width, height, L"fccs" };
    auto window = window::CreateFCCSWindow(windowDesc);
    window->OpenWindow();

    auto device = rhi::CreateDeivce();
    auto queue = device->CreateCommandQueue(rhi::CommandQueueType::Graphics);
    auto swapchain = rhi::CreateSwapChain(queue, width, height, window->GetHWND());
    rhi::TextureDesc textureDesc = {};
    textureDesc.width = 2000;
    textureDesc.height = 2000;
    textureDesc.format = rhi::Format::RGBA8_UNORM;
    auto texture = device->CreateTexture(textureDesc);

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
            swapchain->Present(1);
        }
    }

    DestroyResource(texture);
    DestroyResource(window);
    DestroyResource(device);
    DestroyResource(queue);
    DestroyResource(swapchain);
    return 0;
}
