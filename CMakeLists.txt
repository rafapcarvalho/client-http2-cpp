cmake_minimum_required(VERSION 3.10)
project(client14)

# Set the C++ standard to C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the main.cpp file to the project
add_executable(client14 main.cpp)

# Link the curl library to the client14 executable
target_link_libraries(client14 curl)

# Link the jsoncpp library to the client14 executable
target_link_libraries(client14 jsoncpp)

# Include the curl headers
include_directories(/opt/rh/httpd24/root/usr/include/curl)

# Add the curl library path
link_directories(/opt/rh/httpd24/root/usr/lib64)

# Set the rpath for the curl library
set_target_properties(client14 PROPERTIES INSTALL_RPATH /opt/rh/httpd24/root/usr/lib64)