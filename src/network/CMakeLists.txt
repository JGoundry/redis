file(GLOB NETWORK_SOURCE_FILES *.cpp *.c)

add_library(network_lib STATIC
    ${NETWORK_SOURCE_FILES}
)

target_include_directories(network_lib
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(network_lib 
    PRIVATE
    resp_lib
)
