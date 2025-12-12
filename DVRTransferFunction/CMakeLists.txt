cmake_minimum_required(VERSION 3.23)

# -----------------------------------------------------------------------------
# TransferFunction Plugin
# -----------------------------------------------------------------------------
set(DVRTRANSFERFUNCTION "DVRTransferFunction")
PROJECT("DVRTransferFunction"
        LANGUAGES CXX
)

# -----------------------------------------------------------------------------
# Dependencies
# -----------------------------------------------------------------------------
find_package(Qt6 COMPONENTS Widgets WebEngineWidgets OpenGL OpenGLWidgets REQUIRED)
find_package(ManiVault COMPONENTS Core PointData ImageData CONFIG QUIET)

# -----------------------------------------------------------------------------
# Source files
# -----------------------------------------------------------------------------
set(PLUGIN
    src/TransferFunctionPlugin.h
    src/TransferFunctionPlugin.cpp
)

set(UI
    src/TransferFunctionWidget.h
    src/TransferFunctionWidget.cpp
    src/InteractiveShape.h
    src/InteractiveShape.cpp
)

set(Actions
    src/SelectionAction.h
    src/SelectionAction.cpp
    src/SettingsAction.h
    src/SettingsAction.cpp
    src/DatasetsAction.h
    src/DatasetsAction.cpp
    src/PointAction.h
    src/PointAction.cpp
    src/MaterialSettings.h
    src/MaterialSettings.cpp
    src/MaterialColorPickerAction.h 
    src/MaterialColorPickerAction.cpp
    src/GradientPickerAction.h
    src/GradientPickerAction.cpp
    src/MaterialTransitionsAction.h
    src/MaterialTransitionsAction.cpp
    src/GlobalAlphaAction.h
    src/GlobalAlphaAction.cpp
)

set(RESOURCES
    res/TransferFunctionResources.qrc
)

set(TEXTURES
    res/textures/gaussian_texture.png
    res/textures/gaussian1D_texture.png
)

set(AUX
    src/TransferFunctionPlugin.json
)

qt_add_resources(RESOURCE_FILES ${RESOURCES})

set(SOURCES ${PLUGIN} ${UI} ${Actions})

source_group(Plugin FILES ${PLUGIN})
source_group(UI FILES ${UI})
source_group(Actions FILES ${Actions})
source_group(Resources FILES ${RESOURCES})
source_group(Textures FILES ${TEXTURES})
source_group(Aux FILES ${AUX})

# -----------------------------------------------------------------------------
# CMake Target
# -----------------------------------------------------------------------------
add_library(${DVRTRANSFERFUNCTION} SHARED ${SOURCES} ${RESOURCES} ${RESOURCE_FILES} ${AUX})

# -----------------------------------------------------------------------------
# Target include directories
# -----------------------------------------------------------------------------
target_include_directories(${DVRTRANSFERFUNCTION} PRIVATE "${ManiVault_INCLUDE_DIR}")

# -----------------------------------------------------------------------------
# Target properties
# -----------------------------------------------------------------------------
target_compile_features(${DVRTRANSFERFUNCTION} PRIVATE cxx_std_20)

if(MV_UNITY_BUILD)
    set_target_properties(${DVRTRANSFERFUNCTION} PROPERTIES UNITY_BUILD ON)
endif()

# -----------------------------------------------------------------------------
# Target library linking
# -----------------------------------------------------------------------------
target_link_libraries(${DVRTRANSFERFUNCTION} PRIVATE Qt6::Widgets)
target_link_libraries(${DVRTRANSFERFUNCTION} PRIVATE Qt6::WebEngineWidgets)
target_link_libraries(${DVRTRANSFERFUNCTION} PRIVATE Qt6::OpenGL)
target_link_libraries(${DVRTRANSFERFUNCTION} PRIVATE Qt6::OpenGLWidgets)

target_link_libraries(${DVRTRANSFERFUNCTION} PRIVATE ManiVault::Core)
target_link_libraries(${DVRTRANSFERFUNCTION} PRIVATE ManiVault::PointData)
target_link_libraries(${DVRTRANSFERFUNCTION} PRIVATE ManiVault::ImageData)

# -----------------------------------------------------------------------------
# Target installation
# -----------------------------------------------------------------------------
install(TARGETS ${DVRTRANSFERFUNCTION}
    RUNTIME DESTINATION Plugins COMPONENT PLUGINS # Windows .dll
    LIBRARY DESTINATION Plugins COMPONENT PLUGINS # Linux/Mac .so
)

add_custom_command(TARGET ${DVRTRANSFERFUNCTION} POST_BUILD
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
    set_property(TARGET ${DVRTRANSFERFUNCTION} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<IF:$<CONFIG:DEBUG>,${ManiVault_INSTALL_DIR}/Debug,$<IF:$<CONFIG:RELWITHDEBINFO>,${ManiVault_INSTALL_DIR}/RelWithDebInfo,${ManiVault_INSTALL_DIR}/Release>>)
    set_property(TARGET ${DVRTRANSFERFUNCTION} PROPERTY VS_DEBUGGER_COMMAND $<IF:$<CONFIG:DEBUG>,"${ManiVault_INSTALL_DIR}/Debug/ManiVault Studio.exe",$<IF:$<CONFIG:RELWITHDEBINFO>,"${ManiVault_INSTALL_DIR}/RelWithDebInfo/ManiVault Studio.exe","${ManiVault_INSTALL_DIR}/Release/ManiVault Studio.exe">>)
endif()
