cmake_minimum_required(VERSION 3.27)
project(GeoAlgorithmStudy)
set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-dev")
#target_compile_options(${PROJECT_NAME} PRIVATE "-Wno-dev")

add_executable(${PROJECT_NAME}
    main.cpp
    VTKViewer.h
    tutorials/01_bottle.h
    tutorials/01_bottle.cpp)

include(ThirdLibraryPaths.cmake)

# VTK
find_package(VTK REQUIRED COMPONENTS
    CommonColor
    CommonCore
    FiltersSources
    RenderingCore

    InteractionStyle
    RenderingContextOpenGL2
    #    RenderingFreeType
    RenderingGL2PSOpenGL2
    RenderingOpenGL2
)
if(VTK_FOUND)
    message(STATUS "VTK found")
    target_link_libraries(${PROJECT_NAME} PRIVATE ${VTK_LIBRARIES})
    vtk_module_autoinit(TARGETS ${PROJECT_NAME} MODULES ${VTK_LIBRARIES})
else()
    message(FATAL_ERROR "Could not find VTK")
endif()


# OCCT
find_package(OpenCASCADE REQUIRED)
if(OpenCASCADE_FOUND)
    message(STATUS "OpenCASCADE found")
    include_directories(${OpenCASCADE_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCASCADE_LIBRARIES})
else()
    message(FATAL_ERROR "Could not find OpenCASCADE")
endif()
