add_executable(04-PostProcess WIN32 PostProcess.cpp)
target_include_directories(04-PostProcess PUBLIC "${PROJECT_SOURCE_DIR}/d3dx")
target_compile_definitions(04-PostProcess PRIVATE D3DX12_NO_STATE_OBJECT_HELPERS D3DX12_NO_CHECK_FEATURE_SUPPORT_CLASS)
target_link_libraries(04-PostProcess PRIVATE FCCS)
add_custom_command ( TARGET 04-PostProcess POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:FCCS> $<TARGET_FILE_DIR:04-PostProcess>
)