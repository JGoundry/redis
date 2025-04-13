cmake_minimum_required(VERSION 3.31)
project(redis)

# C++ standard
set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS        FALSE)

# Misc
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Executable names
set(SERVER server)

# Library vars
set(BOOST_MIN_VERSION         1.87)
set(BOOST_REQUIRED_COMPONENTS container system)
set(BOOST_TARGET_LIBS         Boost::container)

# Packages
find_package(Boost ${BOOST_MIN_VERSION} REQUIRED COMPONENTS ${BOOST_REQUIRED_COMPONENTS})
find_package(GTest REQUIRED)

# Add sub dirs
add_subdirectory(src)
add_subdirectory(test)
