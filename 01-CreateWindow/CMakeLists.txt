add_executable(01-CreateWindow WIN32 CreateWindow.cpp)
target_link_libraries(01-CreateWindow PRIVATE FCCS)
add_custom_command ( TARGET 01-CreateWindow POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:FCCS> $<TARGET_FILE_DIR:01-CreateWindow>
)