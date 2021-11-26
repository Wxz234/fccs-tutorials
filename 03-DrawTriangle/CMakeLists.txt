add_executable(03-DrawTriangle WIN32 DrawTriangle.cpp)
target_link_libraries(03-DrawTriangle PRIVATE FCCS)

add_custom_command(TARGET 03-DrawTriangle POST_BUILD  
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE_DIR:FCCS>/FCCS.dll" $<TARGET_FILE_DIR:03-DrawTriangle>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/DrawTriangle.hlsl" $<TARGET_FILE_DIR:03-DrawTriangle>
)   
