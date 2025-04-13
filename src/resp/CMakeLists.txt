file(GLOB RESP_SOURCE_FILES *.cpp *.c)

add_library(resp_lib STATIC
    ${RESP_SOURCE_FILES}
)

target_include_directories(resp_lib
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
