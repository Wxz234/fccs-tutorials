add_executable(04-RenderTexture WIN32 main.cpp)
target_include_directories(04-RenderTexture PUBLIC "${PROJECT_SOURCE_DIR}/d3dx")
target_compile_definitions(04-RenderTexture PRIVATE D3DX12_NO_STATE_OBJECT_HELPERS D3DX12_NO_CHECK_FEATURE_SUPPORT_CLASS)
target_link_libraries(04-RenderTexture PRIVATE fccs d3d12)

