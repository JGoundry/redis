# Add subdirs for other libs
add_subdirectory(network)
add_subdirectory(resp)

# Create server executable
add_executable(${SERVER} main.cpp)

target_include_directories(${SERVER} 
    PRIVATE 
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/network
    ${CMAKE_CURRENT_SOURCE_DIR}/resp
)

target_link_libraries(${SERVER} 
    PRIVATE 
    ${BOOST_TARGET_LIBS}
    network_lib
    resp_lib
)
