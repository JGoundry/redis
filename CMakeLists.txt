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
set(BOOST_REQUIRED_COMPONENTS container)
set(BOOST_TARGET_LIBS         Boost::container)

# Files
file(GLOB SERVER_SOURCE_FILES src/*.cpp)

# Executables
add_executable(${SERVER} ${SERVER_SOURCE_FILES})

# Packages
find_package(Boost ${BOOST_MIN_VERSION} REQUIRED COMPONENTS ${BOOST_REQUIRED_COMPONENTS})
target_link_libraries(${SERVER} PRIVATE ${BOOST_TARGET_LIBS})
