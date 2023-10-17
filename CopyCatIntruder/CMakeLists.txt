find_package(Threads REQUIRED)

set(VSOMEIP_NAME "vsomeip3")
set(VSOMEIP_CONFIG_FILE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories(${VSOMEIP_INCLUDE_DIRS})

# add_subdirectory(json)

add_library(vsomeip_copy_service INTERFACE)
target_sources(vsomeip_copy_service INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/copy_service.hpp"
)
target_include_directories(vsomeip_copy_service INTERFACE
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

    add_executable (copy_service copy_service_main.cpp)
    target_link_libraries(copy_service vsomeip_copy_service ${VSOMEIP_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} nlohmann_json::nlohmann_json)
endif()