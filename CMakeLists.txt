cmake_minimum_required(VERSION 3.27)
project(GeoAlgorithmStudy)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

include(ThirdLibraryPaths.cmake)

# WIN32 indicate the exe don't display console window.
#add_executable(${PROJECT_NAME}
add_executable(${PROJECT_NAME} WIN32
    main.cpp
    VTKViewer.h
    tutorials/01_bottle.h
    tutorials/01_bottle.cpp
    QRenderWindow.h
    QRenderWindow.cpp
    RenderWindow.ui
    DDGStructure.cpp
    DDGStructure.h
    MouseInteractorStyle.h
    MouseInteractorStyle.cpp
    Signals.h
)

# VTK
find_package(VTK REQUIRED COMPONENTS
    CommonColor
    CommonCore
    FiltersSources
    RenderingCore
    RenderingAnnotation
    InteractionStyle
    RenderingContextOpenGL2
    RenderingFreeType
    RenderingGL2PSOpenGL2
    RenderingOpenGL2
    GUISupportQt
    RenderingQt
)
target_link_libraries(${PROJECT_NAME} PRIVATE ${VTK_LIBRARIES})
vtk_module_autoinit(TARGETS ${PROJECT_NAME} MODULES ${VTK_LIBRARIES})


# OCCT
find_package(OpenCASCADE REQUIRED)
include_directories(${OpenCASCADE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCASCADE_LIBRARIES})


# Qt6
set(Qt6_COMPONENTS Core Gui Widgets OpenGLWidgets)
foreach(_qt_comp IN LISTS Qt6_COMPONENTS)
    list(APPEND Qt6_MODULES "Qt6::${_qt_comp}")
endforeach()
find_package(Qt6 REQUIRED COMPONENTS ${Qt6_COMPONENTS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Qt6_MODULES})
