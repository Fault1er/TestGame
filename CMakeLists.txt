cmake_minimum_required( VERSION 3.19 )
project( TestGame )

set( CMAKE_CXX_STANDARD 20 )

file( GLOB_RECURSE SRC_FILES
        ${CMAKE_SOURCE_DIR}/source/*cpp
        ${CMAKE_SOURCE_DIR}/source/*hpp
        ${CMAKE_SOURCE_DIR}/source/*cxx
        ${CMAKE_SOURCE_DIR}/source/*hxx
        ${CMAKE_SOURCE_DIR}/source/*c
        ${CMAKE_SOURCE_DIR}/source/*h
)

include_directories( ${CMAKE_SOURCE_DIR}/source )

add_executable( ${PROJECT_NAME} ${SRC_FILES} )

## OpenGL settings
find_package(OpenGL REQUIRED)
if ( OPENGL_FOUND )
    set( LIBS ${LIBS} opengl32 )
else()
    message( "OpenGL not found!!!" )
endif()

## SFML settings
set( SFML_DIR ${CMAKE_SOURCE_DIR}/dependencies/SFML )
set( SFML_INCLUDE_DIR ${SFML_DIR}/include )
set( SFML_LIBRARY_DIR ${SFML_DIR}/lib )

if ( CMAKE_BUILD_TYPE STREQUAL "Release" )
    set( SFML_LIBRARIES
            ${SFML_LIBRARY_DIR}/sfml-main.lib
            ${SFML_LIBRARY_DIR}/sfml-system.lib
            ${SFML_LIBRARY_DIR}/sfml-window.lib
            ${SFML_LIBRARY_DIR}/sfml-graphics.lib
    )
else()
    set( SFML_LIBRARIES
            ${SFML_LIBRARY_DIR}/sfml-main-d.lib
            ${SFML_LIBRARY_DIR}/sfml-system-d.lib
            ${SFML_LIBRARY_DIR}/sfml-window-d.lib
            ${SFML_LIBRARY_DIR}/sfml-graphics-d.lib
    )
endif()

include_directories( ${SFML_INCLUDE_DIR} )
target_link_libraries( ${PROJECT_NAME} PRIVATE ${SFML_LIBRARIES} ${LIBS} )