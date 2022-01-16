add_executable(02-InitializeDevice WIN32 Main.cpp Game.cpp)
target_include_directories(02-InitializeDevice PUBLIC "${PROJECT_SOURCE_DIR}/d3dx")
target_compile_definitions(02-InitializeDevice PRIVATE UNICODE D3DX12_NO_STATE_OBJECT_HELPERS D3DX12_NO_CHECK_FEATURE_SUPPORT_CLASS)
target_link_libraries(02-InitializeDevice PRIVATE 
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
add_executable(02-InitializeDevice WIN32 main.cpp)
target_link_libraries(02-InitializeDevice PRIVATE fccs)
