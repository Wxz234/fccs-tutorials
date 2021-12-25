add_executable(02-InitializeDevice WIN32 InitializeDevice.cpp)
target_link_libraries(02-InitializeDevice PRIVATE FCCS)
add_custom_command ( TARGET 02-InitializeDevice POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:FCCS> $<TARGET_FILE_DIR:02-InitializeDevice>
)