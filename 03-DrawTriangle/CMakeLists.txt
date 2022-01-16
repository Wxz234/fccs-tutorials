add_executable(03-DrawTriangle WIN32 main.cpp)
target_include_directories(03-DrawTriangle PUBLIC "${PROJECT_SOURCE_DIR}/d3dx")
target_compile_definitions(03-DrawTriangle PRIVATE D3DX12_NO_STATE_OBJECT_HELPERS D3DX12_NO_CHECK_FEATURE_SUPPORT_CLASS)
target_link_libraries(03-DrawTriangle PRIVATE fccs d3d12)

