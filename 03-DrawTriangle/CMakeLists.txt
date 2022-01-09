add_executable(03-DrawTriangle WIN32 Main.cpp Game.cpp)
target_include_directories(03-DrawTriangle PUBLIC "${PROJECT_SOURCE_DIR}/d3dx")
target_compile_definitions(03-DrawTriangle PRIVATE UNICODE D3DX12_NO_STATE_OBJECT_HELPERS D3DX12_NO_CHECK_FEATURE_SUPPORT_CLASS)
target_link_libraries(03-DrawTriangle PRIVATE 
fccs
d3d12.lib
dxgi.lib
dxguid.lib
uuid.lib
kernel32.lib
user32.lib
comdlg32.lib
advapi32.lib
shell32.lib
ole32.lib
oleaut32.lib
runtimeobject.lib
)
