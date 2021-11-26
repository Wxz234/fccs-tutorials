add_executable(03-DrawTriangle WIN32 DrawTriangle.cpp DrawTriangle.hlsl)
target_link_libraries(03-DrawTriangle PRIVATE FCCS)
set_source_files_properties(DrawTriangle.hlsl PROPERTIES VS_TOOL_OVERRIDE "None")

set_target_properties(03-DrawTriangle PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:03-DrawTriangle>)
add_custom_command(TARGET 03-DrawTriangle POST_BUILD  
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE_DIR:FCCS>/FCCS.dll" $<TARGET_FILE_DIR:03-DrawTriangle>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/DrawTriangle.hlsl" $<TARGET_FILE_DIR:03-DrawTriangle>
)   
