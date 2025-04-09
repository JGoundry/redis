cmake_minimum_required(VERSION 3.31)
project(redis)

# C++ standard
set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS        FALSE)

# Executable names
set(SERVER server)
set(CLIENT client)

# Library vars
set(BOOST_MIN_VERSION         1.87)
set(BOOST_REQUIRED_COMPONENTS container system )
set(BOOST_TARGET_LIBS         Boost::container )

# TODO: Create seperate CMakeLists.txt for each lib

# ----- RESP lib -----
add_library(resp_lib STATIC
        src/resp/parseRespRequest.cpp
        src/resp/RespTypes.cpp
)

target_include_directories(resp_lib 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/resp)


# ----- Network lib -----
add_library(network_lib STATIC
        src/network/RespHandler.cpp
)

target_include_directories(network_lib 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/network 
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/resp)

target_link_libraries(network_lib PRIVATE resp_lib)

# ----- Main Executable -----

# Files
file(GLOB SERVER_SOURCE_FILES src/*.cpp src/*.hpp src/*.c src/*.h)

# Executable
add_executable(${SERVER} ${SERVER_SOURCE_FILES})

target_include_directories(${SERVER} 
    PRIVATE 
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(${SERVER} 
    PRIVATE 
    ${BOOST_TARGET_LIBS}
    network_lib
    resp_lib)

# Packages
find_package(Boost ${BOOST_MIN_VERSION} REQUIRED COMPONENTS ${BOOST_REQUIRED_COMPONENTS})
