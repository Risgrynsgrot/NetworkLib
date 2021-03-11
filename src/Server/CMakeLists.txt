set(target_name Server)

set(src_server
    "main.cpp"
    "Server.cpp"
    "Server.h"
    )
add_executable(${target_name} ${src_server})

target_include_directories(${target_name} PUBLIC ${CMAKE_SOURCE_DIR}/src)

set_target_properties(${target_name} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Bin)
set_target_properties(${target_name} PROPERTIES FOLDER ${PROJECT_NAME})
set_target_properties(${target_name} PROPERTIES OUTPUT_NAME ${target_name}_$<CONFIG>)

set_target_properties(${target_name}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_link_libraries(${target_name} "Spooder")
