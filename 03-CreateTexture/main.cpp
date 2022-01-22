#define UNICODE
#include <Windows.h>
#include <fccs/fccs.h>

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    constexpr uint32_t width = 800, height = 600;
    fccs::window::WindowDesc windowDesc = { width, height, L"fccs" };
    auto window = fccs::window::CreateFCCSWindow(windowDesc);
    window->OpenWindow();

    auto device = fccs::rhi::CreateDeivce();
    auto queue = device->CreateCommandQueue(fccs::rhi::CommandQueueType::Graphics);
    auto swapchain = fccs::rhi::CreateSwapChain(queue, width, height, window->GetHWND());
    fccs::rhi::TextureDesc textureDesc = {};
    textureDesc.width = 2000;
    textureDesc.height = 2000;
    textureDesc.format = fccs::rhi::Format::RGBA8_UNORM;
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


    fccs::DestroyResource(texture);
    fccs::DestroyResource(window);
    fccs::DestroyResource(device);
    fccs::DestroyResource(queue);
    fccs::DestroyResource(swapchain);
    return 0;
}
