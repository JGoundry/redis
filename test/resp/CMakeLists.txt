add_executable(parseRespRequest_test parseRespRequest_test.cpp)

target_link_libraries(parseRespRequest_test
    PRIVATE
    GTest::gtest_main
    resp_lib
)

target_include_directories(parseRespRequest_test
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/resp
)
