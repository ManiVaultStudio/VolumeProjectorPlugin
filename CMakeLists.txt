cmake_minimum_required(VERSION 3.22)

if(NOT DEFINED MV_DRV_USE_VCPKG)
    set(MV_DRV_USE_VCPKG OFF)
elseif(MV_DRV_USE_VCPKG)
    message(STATUS "DVRPlugins: Using vcpkg to install dependencies")
endif()

option(MV_UNITY_BUILD "Combine target source files into batches for faster compilation" OFF)

set(PROJECT "DVRPlugins")
PROJECT(${PROJECT})

# -----------------------------------------------------------------------------
# CMake Options
# -----------------------------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /DWIN32 /EHsc /MP /permissive- /Zc:__cplusplus")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MD")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
endif()

# -----------------------------------------------------------------------------
# DVRPlugins
# -----------------------------------------------------------------------------

add_subdirectory(DVRVolumeData)         # add first since other projects depend on it
add_subdirectory(DVRVolumeLoader)
add_subdirectory(DVRViewPlugin)
add_subdirectory(DVRTransferFunction)

set_target_properties(DVRVolumeData DVRVolumeLoader DVRViewPlugin DVRTransferFunction
    PROPERTIES
    FOLDER DVRPlugins
)
