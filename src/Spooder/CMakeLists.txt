set(target_name Spooder)

set(src_network
    "spooder.cpp"
    "CommonNetworkIncludes.h"
    "BitSerializer.h"
    "BitSerializer.cpp"
    "Serializer.cpp"
    "Serializer.h"
    "PacketTypes.h"
    "MathFunctions.h"
    "MessageFactory.cpp"
    "MessageFactory.h"
    "Address.cpp"
    "Address.h"
    "Socket.cpp"
    "Socket.h"
    "NetworkFunctions.cpp"
    "NetworkFunctions.h"
    "Timer.cpp"
    "Timer.h"
    )

add_library(${target_name} ${src_network})

target_include_directories(${target_name} PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(${target_name} "ws2_32")

set_target_properties(${target_name} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Bin)
set_target_properties(${target_name} PROPERTIES FOLDER ${PROJECT_NAME})
set_target_properties(${target_name} PROPERTIES OUTPUT_NAME ${target_name}_$<CONFIG>)

set_target_properties(${target_name}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
