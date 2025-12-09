 cmake_minimum_required(VERSION 3.22)

# -----------------------------------------------------------------------------
# DVRVolumeData Plugin
# -----------------------------------------------------------------------------
set(VOLUMEDATA "DVRVolumeData")
PROJECT(${VOLUMEDATA}        
        LANGUAGES CXX
)

# -----------------------------------------------------------------------------
# Dependencies
# -----------------------------------------------------------------------------
find_package(Qt6 COMPONENTS Widgets WebEngineWidgets REQUIRED)
find_package(ManiVault COMPONENTS Core PointData CONFIG QUIET)

# -----------------------------------------------------------------------------
# Source files
# -----------------------------------------------------------------------------
set(VOLUME_DATA_HEADERS 
    src/Volume.h
    src/VolumeData.h
    src/Volumes.h
    src/Size3D.h
    src/Size3D.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/dvrvolumedata_export.h
)

set(VOLUME_DATA_SOURCES 
    src/Volume.cpp
    src/VolumeData.cpp
    src/Volumes.cpp
)

set(ACTIONS_SOURCES 
    src/InfoAction.h
    src/InfoAction.cpp
)

set(ACTIONS_SOURCES 
    src/InfoAction.h
    src/InfoAction.cpp
)

set(AUX 
    src/VolumeData.json
)

source_group(Actions FILES ${ACTIONS_SOURCES})
source_group(Plugin FILES ${VOLUME_DATA_HEADERS} ${VOLUME_DATA_SOURCES} )
source_group(Aux FILES ${AUX})

# -----------------------------------------------------------------------------
# CMake Target
# -----------------------------------------------------------------------------
add_library(${VOLUMEDATA} SHARED ${VOLUME_DATA_HEADERS} ${VOLUME_DATA_SOURCES} ${ACTIONS_SOURCES} ${AUX})
add_library(ManiVault::${VOLUMEDATA} ALIAS ${VOLUMEDATA})

# -----------------------------------------------------------------------------
# Target include directories
# -----------------------------------------------------------------------------
target_include_directories(${VOLUMEDATA} PRIVATE PRIVATE "${ManiVault_INCLUDE_DIR}")

# -----------------------------------------------------------------------------
# Target properties
# -----------------------------------------------------------------------------
target_compile_features(${VOLUMEDATA} PRIVATE cxx_std_20)

# Generate a header file that contains the EXPORT macro for this library.
include(GenerateExportHeader)
generate_export_header(${VOLUMEDATA})

# Retrieve the file name of the generated export header.
file(GLOB EXPORT_HEADER_FILE_NAME 
    RELATIVE ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/*_export.h)
list(APPEND VOLUME_DATA_HEADERS ${CMAKE_CURRENT_BINARY_DIR}/${EXPORT_HEADER_FILE_NAME})

set_target_properties(${VOLUMEDATA} PROPERTIES PUBLIC_HEADER "${VOLUME_DATA_HEADERS}")

if(MV_UNITY_BUILD)
    set_target_properties(${VOLUMEDATA} PROPERTIES UNITY_BUILD ON)
endif()

# -----------------------------------------------------------------------------
# Target library linking
# -----------------------------------------------------------------------------
target_link_libraries(${VOLUMEDATA} PRIVATE Qt6::Widgets)
target_link_libraries(${VOLUMEDATA} PRIVATE Qt6::WebEngineWidgets)
target_link_libraries(${VOLUMEDATA} PRIVATE ManiVault::Core)
target_link_libraries(${VOLUMEDATA} PRIVATE ManiVault::PointData)

# -----------------------------------------------------------------------------
# Target installation
# -----------------------------------------------------------------------------

install(TARGETS ${VOLUMEDATA}
    EXPORT "ManiVaultPublicExport"
    RUNTIME DESTINATION "$<CONFIGURATION>/Plugins/" COMPONENT PLUGINS # Windows .dll
    LIBRARY DESTINATION "$<CONFIGURATION>/Plugins/" COMPONENT PLUGINS # Linux/Mac .so
    ARCHIVE DESTINATION "$<CONFIGURATION>/lib/"     COMPONENT LINKLIB # Windows .lib
    PUBLIC_HEADER DESTINATION "$<CONFIGURATION>/include/${VOLUMEDATA}/" COMPONENT HEADERS
)

add_custom_command(TARGET ${VOLUMEDATA} POST_BUILD
    COMMAND ${CMAKE_COMMAND}
        --install ${CMAKE_CURRENT_BINARY_DIR}
        --config $<CONFIGURATION>
        --prefix ${MV_INSTALL_DIR}
)

# -----------------------------------------------------------------------------
# Miscellaneous
# -----------------------------------------------------------------------------
# Automatically set the debug environment (command + working directory) for MSVC
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set_property(TARGET ${VOLUMEDATA} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<IF:$<CONFIG:DEBUG>,${ManiVault_INSTALL_DIR}/Debug,$<IF:$<CONFIG:RELWITHDEBINFO>,${ManiVault_INSTALL_DIR}/RelWithDebInfo,${ManiVault_INSTALL_DIR}/Release>>)
    set_property(TARGET ${VOLUMEDATA} PROPERTY VS_DEBUGGER_COMMAND $<IF:$<CONFIG:DEBUG>,"${ManiVault_INSTALL_DIR}/Debug/ManiVault Studio.exe",$<IF:$<CONFIG:RELWITHDEBINFO>,"${ManiVault_INSTALL_DIR}/RelWithDebInfo/ManiVault Studio.exe","${ManiVault_INSTALL_DIR}/Release/ManiVault Studio.exe">>)

endif()
