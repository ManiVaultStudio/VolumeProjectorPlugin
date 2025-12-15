cmake_minimum_required(VERSION 3.22)

option(MV_DRV_USE_FAISS "Enable Faiss library integration" OFF)  # Add this line

# -----------------------------------------------------------------------------
# DVRView Plugin
# -----------------------------------------------------------------------------
set(DVRVIEWPLUGIN "DVRViewPlugin")
project("${DVRVIEWPLUGIN}"
        LANGUAGES CXX
)

# -----------------------------------------------------------------------------
# Dependencies
# -----------------------------------------------------------------------------
find_package(Qt6 COMPONENTS Widgets WebEngineWidgets OpenGL OpenGLWidgets REQUIRED)
find_package(ManiVault COMPONENTS Core PointData ImageData VolumeData CONFIG QUIET)

# --- OpenMP Support ---
find_package(OpenMP REQUIRED)
if(OpenMP_CXX_FOUND)
    message(STATUS "Found OpenMP: ${OpenMP_CXX_FLAGS}")
else()
    message(WARNING "OpenMP not found.")
endif()

# -----------------------------------------------------------------------------
# Conda Faiss Integration
# -----------------------------------------------------------------------------
if(MV_DRV_USE_FAISS)
    # Set the path to your Faiss library.
    set(FAISS_DIR "" CACHE PATH "Path to your folder containing the Faiss include and library folders")
    if(FAISS_DIR STREQUAL "")
        message(FATAL_ERROR "FAISS_DIR is not set. Please set it to your folder that contains the Faiss include and library folders.")
    endif()

    if(WIN32)
        set(FAISS_INCLUDE_DIR "${FAISS_DIR}/Library/include")
        set(FAISS_LIBRARY_DIR "${FAISS_DIR}/Library/lib")
    else()
        set(FAISS_INCLUDE_DIR "${FAISS_DIR}/include")
        set(FAISS_LIBRARY_DIR "${FAISS_DIR}/lib")
    endif()

    message(STATUS "Using Faiss include directory: ${FAISS_INCLUDE_DIR}")
    message(STATUS "Using Faiss library directory: ${FAISS_LIBRARY_DIR}")
endif()

# -----------------------------------------------------------------------------
# hnswlib Integration
# -----------------------------------------------------------------------------
find_package(hnswlib CONFIG QUIET)
if(NOT hnswlib_FOUND AND NOT TARGET hnswlib::hnswlib)
    include(FetchContent)
    FetchContent_Declare(
      hnswlib 
      GIT_REPOSITORY https://github.com/nmslib/hnswlib
      GIT_TAG v0.8.0
    )
    FetchContent_MakeAvailable(hnswlib)
else()
    message(STATUS "DVRViewPlugin: Using external hnswlib")
endif()
    
# -----------------------------------------------------------------------------
# Source Files
# -----------------------------------------------------------------------------
# Define the plugin sources
set(PLUGIN_SOURCES
    src/DVRViewPlugin.h
    src/DVRViewPlugin.cpp
)
set(AUX
    PluginInfo.json
)
set(PLUGIN_RENDERERS
    src/VolumeRenderer.h
    src/VolumeRenderer.cpp
    src/MCArrays.h
)
set(PLUGIN_GRAPHICS
    src/TrackballCamera.h 
    src/TrackballCamera.cpp
)
set(PLUGIN_WIDGETS
    src/DVRWidget.h
    src/DVRWidget.cpp
)
set(PLUGIN_ACTIONS
    src/SettingsAction.h
    src/SettingsAction.cpp
    src/GlobalSettingsAction.h
    src/GlobalSettingsAction.cpp
)
set(RESOURCES
    res/ViewerResources.qrc
)

qt_add_resources(RESOURCE_FILES ${RESOURCES})

set(SOURCES ${PLUGIN_SOURCES} ${PLUGIN_WIDGETS} ${PLUGIN_GRAPHICS} ${PLUGIN_RENDERERS} ${PLUGIN_ACTIONS})

source_group( Plugin FILES ${PLUGIN_SOURCES})
source_group( Widget FILES ${PLUGIN_WIDGETS})
source_group( Actions FILES ${PLUGIN_ACTIONS})
source_group( Renderers FILES ${PLUGIN_RENDERERS})
source_group( Graphics FILES ${PLUGIN_GRAPHICS})
source_group( Resources FILES ${RESOURCES})
source_group( Aux FILES ${AUX})

# -----------------------------------------------------------------------------
# CMake Target
# -----------------------------------------------------------------------------
add_library(${DVRVIEWPLUGIN} SHARED ${SOURCES} ${RESOURCES} ${RESOURCE_FILES} ${AUX})

# -----------------------------------------------------------------------------
# Target Include Directories
# -----------------------------------------------------------------------------
target_include_directories(${DVRVIEWPLUGIN} PRIVATE "${ManiVault_INCLUDE_DIR}")
target_include_directories(${DVRVIEWPLUGIN} PRIVATE "${DVRVolumeData_INCLUDE_DIR}")

# -----------------------------------------------------------------------------
# Target Properties
# -----------------------------------------------------------------------------
target_compile_features(${DVRVIEWPLUGIN} PRIVATE cxx_std_20)

# Enable unity build
if(MV_UNITY_BUILD)
    set_target_properties(${DVRVIEWPLUGIN} PROPERTIES UNITY_BUILD ON)
endif()

if(MV_DRV_USE_FAISS)
  message(STATUS "Compiling with -DUSE_FAISS")
  target_compile_definitions(
    ${DVRVIEWPLUGIN}
    PRIVATE
      USE_FAISS
  )
endif()

# -----------------------------------------------------------------------------
# Target Library Linking
# -----------------------------------------------------------------------------
# Link to Qt libraries
target_link_libraries(${DVRVIEWPLUGIN} PRIVATE Qt6::Widgets)
target_link_libraries(${DVRVIEWPLUGIN} PRIVATE Qt6::WebEngineWidgets)
target_link_libraries(${DVRVIEWPLUGIN} PRIVATE Qt6::OpenGL)
target_link_libraries(${DVRVIEWPLUGIN} PRIVATE Qt6::OpenGLWidgets)

# Link to ManiVault and data plugins
target_link_libraries(${DVRVIEWPLUGIN} PRIVATE ManiVault::Core)
target_link_libraries(${DVRVIEWPLUGIN} PRIVATE ManiVault::PointData)
target_link_libraries(${DVRVIEWPLUGIN} PRIVATE ManiVault::ImageData)
target_link_libraries(${DVRVIEWPLUGIN} PRIVATE ManiVault::DVRVolumeData)

target_link_libraries(${DVRVIEWPLUGIN} PRIVATE hnswlib::hnswlib)

if(OpenMP_CXX_FOUND)
    target_link_libraries(${DVRVIEWPLUGIN} PRIVATE OpenMP::OpenMP_CXX)
endif()

if(MV_DRV_USE_FAISS)
    target_link_libraries(${DVRVIEWPLUGIN} PRIVATE faiss)
endif()

# -----------------------------------------------------------------------------
# Target Installation
# -----------------------------------------------------------------------------
# Install the shared plugin library to the "Plugins" folder in the ManiVault install directory
install(TARGETS ${DVRVIEWPLUGIN}
    RUNTIME DESTINATION Plugins COMPONENT PLUGINS # Windows .dll
    LIBRARY DESTINATION Plugins COMPONENT PLUGINS # Linux/Mac .so
)

add_custom_command(TARGET ${DVRVIEWPLUGIN} POST_BUILD
    COMMAND "${CMAKE_COMMAND}"
        --install ${CMAKE_CURRENT_BINARY_DIR}
        --config $<CONFIGURATION>
        --prefix ${ManiVault_INSTALL_DIR}/$<CONFIGURATION>
)

# -----------------------------------------------------------------------------
# Miscellaneous
# -----------------------------------------------------------------------------
# Automatically set the debug environment (command + working directory) for MSVC
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set_property(TARGET ${DVRVIEWPLUGIN} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<IF:$<CONFIG:DEBUG>,${ManiVault_INSTALL_DIR}/Debug,$<IF:$<CONFIG:RELWITHDEBINFO>,${ManiVault_INSTALL_DIR}/RelWithDebInfo,${ManiVault_INSTALL_DIR}/Release>>)
    set_property(TARGET ${DVRVIEWPLUGIN} PROPERTY VS_DEBUGGER_COMMAND $<IF:$<CONFIG:DEBUG>,"${ManiVault_INSTALL_DIR}/Debug/ManiVault Studio.exe",$<IF:$<CONFIG:RELWITHDEBINFO>,"${ManiVault_INSTALL_DIR}/RelWithDebInfo/ManiVault Studio.exe","${ManiVault_INSTALL_DIR}/Release/ManiVault Studio.exe">>)

endif()