cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project("openssl-playground")

# Config mode means do not use CMake official configuration.
# Here i use my own "openssl-config.cmake"
find_package(OpenSSL CONFIG REQUIRED)

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

add_executable(${PROJECT_NAME} 
    "src/main.cpp"
)

target_link_libraries(${PROJECT_NAME} OpenSSL::SSL OpenSSL::Crypto)