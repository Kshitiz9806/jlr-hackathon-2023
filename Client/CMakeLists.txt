find_package(Threads REQUIRED)

set(VSOMEIP_NAME "vsomeip3")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories(${VSOMEIP_INCLUDE_DIRS})

add_library(vsomeip_client INTERFACE)
target_sources(vsomeip_client INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/client.hpp"
)

target_include_directories(vsomeip_client INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Android")
    # This will get us acces to
    #   VSOMEIP_INCLUDE_DIRS - include directories for vSomeIP
    #   VSOMEIP_LIBRARIES    - libraries to link against
    find_package(${VSOMEIP_NAME})
    if (NOT ${VSOMEIP_NAME}_FOUND)
        message("${VSOMEIP_NAME} was not found. Please specify vsomeip_DIR")
    endif()

    add_executable (client client_main.cpp)
    target_link_libraries(client vsomeip_client ${VSOMEIP_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
endif()
