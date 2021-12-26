add_executable(04-PostProcess WIN32 PostProcess.cpp)
target_link_libraries(04-PostProcess PRIVATE FCCS d3dcompiler d3d12 DirectX-Headers)
add_custom_command ( TARGET 04-PostProcess POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:FCCS> $<TARGET_FILE_DIR:04-PostProcess>
)