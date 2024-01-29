cmake_minimum_required(VERSION 3.10)

project(VLX_I2C)

add_executable(VLX_I2C vlx.cpp)

add_subdirectory(vl53l0x_api)

target_link_libraries(VLX_I2C PUBLIC VLX_API)
target_include_directories(VLX_I2C PUBLIC 
"${PROJECT_BINARY_DIR}"
"${PROJECT_SOURCE_DIR}/vl53l0x_api")