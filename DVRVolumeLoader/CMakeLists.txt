cmake_minimum_required(VERSION 3.22)

# -----------------------------------------------------------------------------
# DVRVolumeLoader Plugin
# -----------------------------------------------------------------------------
set(DVRVOLUMELOADER "DVRVolumeLoader")
PROJECT(${DVRVOLUMELOADER}
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
set(PLUGIN_SOURCES
    src/DVRVolumeLoader.h
    src/DVRVolumeLoader.cpp
)

set(AUX
    PluginInfo.json
)

source_group( Plugin FILES ${PLUGIN_SOURCES})
source_group( Aux FILES ${AUX})

# -----------------------------------------------------------------------------
# CMake Target
# -----------------------------------------------------------------------------
add_library(${DVRVOLUMELOADER} SHARED ${PLUGIN_SOURCES} ${AUX})

# -----------------------------------------------------------------------------
# Target include directories
# -----------------------------------------------------------------------------
target_include_directories(${DVRVOLUMELOADER} PRIVATE "${ManiVault_INCLUDE_DIR}")
target_include_directories(${DVRVOLUMELOADER} PRIVATE "${DVRVolumeData_INCLUDE_DIR}")

# -----------------------------------------------------------------------------
# Target properties
# -----------------------------------------------------------------------------
target_compile_features(${DVRVOLUMELOADER} PRIVATE cxx_std_20)

# -----------------------------------------------------------------------------
# Target library linking
# -----------------------------------------------------------------------------
target_link_libraries(${DVRVOLUMELOADER} PRIVATE Qt6::Widgets)
target_link_libraries(${DVRVOLUMELOADER} PRIVATE Qt6::WebEngineWidgets)
target_link_libraries(${DVRVOLUMELOADER} PRIVATE ManiVault::Core)
target_link_libraries(${DVRVOLUMELOADER} PRIVATE ManiVault::PointData)
target_link_libraries(${DVRVOLUMELOADER} PRIVATE ManiVault::DVRVolumeData)

# -----------------------------------------------------------------------------
# Target installation
# -----------------------------------------------------------------------------
install(TARGETS ${DVRVOLUMELOADER}
    RUNTIME DESTINATION Plugins COMPONENT PLUGINS # Windows .dll
    LIBRARY DESTINATION Plugins COMPONENT PLUGINS # Linux/Mac .so
)

add_custom_command(TARGET ${DVRVOLUMELOADER} POST_BUILD
    COMMAND "${CMAKE_COMMAND}"
        --install ${CMAKE_CURRENT_BINARY_DIR}
        --config $<CONFIGURATION>
        --prefix ${ManiVault_INSTALL_DIR}/$<CONFIGURATION>
)

# -----------------------------------------------------------------------------
# Miscellaneous
# -----------------------------------------------------------------------------
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set_property(TARGET ${DVRVOLUMELOADER} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<IF:$<CONFIG:DEBUG>,${ManiVault_INSTALL_DIR}/Debug,$<IF:$<CONFIG:RELWITHDEBINFO>,${ManiVault_INSTALL_DIR}/RelWithDebInfo,${ManiVault_INSTALL_DIR}/Release>>)
    set_property(TARGET ${DVRVOLUMELOADER} PROPERTY VS_DEBUGGER_COMMAND $<IF:$<CONFIG:DEBUG>,"${ManiVault_INSTALL_DIR}/Debug/ManiVault Studio.exe",$<IF:$<CONFIG:RELWITHDEBINFO>,"${ManiVault_INSTALL_DIR}/RelWithDebInfo/ManiVault Studio.exe","${ManiVault_INSTALL_DIR}/Release/ManiVault Studio.exe">>)
endif()
