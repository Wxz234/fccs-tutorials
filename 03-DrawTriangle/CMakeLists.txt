add_executable(03-DrawTriangle WIN32 DrawTriangle.cpp)
target_include_directories(03-DrawTriangle PUBLIC "${PROJECT_SOURCE_DIR}/d3dx")
target_compile_definitions(03-DrawTriangle PRIVATE D3DX12_NO_STATE_OBJECT_HELPERS D3DX12_NO_CHECK_FEATURE_SUPPORT_CLASS)
target_link_libraries(03-DrawTriangle PRIVATE FCCS d3d12)
add_custom_command ( TARGET 03-DrawTriangle POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:FCCS> $<TARGET_FILE_DIR:03-DrawTriangle>
)