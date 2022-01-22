#define UNICODE
#include <Windows.h>
#include <fccs/fccs.h>
#include <vector>
#include <thread>
#include <d3dx12.h>
#include "pass.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    constexpr uint32_t width = 800, height = 600;
    fccs::window::WindowDesc windowDesc = { width, height, L"fccs" };
    auto window = fccs::window::CreateFCCSWindow(windowDesc);
    window->OpenWindow();

    auto device = fccs::rhi::CreateDeivce();
    auto queue = device->CreateCommandQueue(fccs::rhi::CommandQueueType::Graphics);
    auto swapchain = fccs::rhi::CreateSwapChain(queue, width, height, window->GetHWND());

    //Pass
    RenderTexturePass rendertexture(device);
    //


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

            //GPU
            {
                std::thread rendertextureThread(
                    [&]() {
                        rendertexture.Execute();
                    }
                );

                rendertextureThread.join();
            }
            

            std::vector<fccs::rhi::ICommandList*> pLists;
            pLists.emplace_back(rendertexture.list);

            queue->ExecuteCommandLists(pLists.size(), pLists.data());
            swapchain->Present(1);
        }
    }

    fccs::DestroyResource(window);
    fccs::DestroyResource(device);
    fccs::DestroyResource(queue);
    fccs::DestroyResource(swapchain);
    return 0;
}
