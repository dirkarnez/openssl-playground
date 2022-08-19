cmake_minimum_required(VERSION 3.14)

project("openssl-playground")

# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(openssl STATIC IMPORTED)

# set_property(
#   TARGET openssl APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG
# )

# set_target_properties(clip PROPERTIES
#   IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
#   IMPORTED_LOCATION_DEBUG "${CLIP_ROOT}/build/Release/clip.lib"
# )

set_property(
  TARGET openssl APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE
)

set_target_properties(openssl PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${OPENSSL_ROOT}/lib64/libopenssl.a"
)
  
set_target_properties(openssl PROPERTIES
  MAP_IMPORTED_CONFIG_MINSIZEREL Release
  MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release
)

target_include_directories(openssl INTERFACE "${OPENSSL_ROOT}/include")



add_library(crypto STATIC IMPORTED)

# set_property(
#   TARGET openssl APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG
# )

# set_target_properties(clip PROPERTIES
#   IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
#   IMPORTED_LOCATION_DEBUG "${CLIP_ROOT}/build/Release/clip.lib"
# )

set_property(
  TARGET crypto APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE
)

set_target_properties(crypto PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${OPENSSL_ROOT}/lib64/libcrypto.a"
)
  
set_target_properties(crypto PROPERTIES
  MAP_IMPORTED_CONFIG_MINSIZEREL Release
  MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release
)

target_include_directories(crypto INTERFACE "${OPENSSL_ROOT}/include")


SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

add_executable(${PROJECT_NAME} 
    "src/main.cpp"
)

target_link_libraries(${PROJECT_NAME} crypto ws2_32)