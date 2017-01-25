cmake_minimum_required(VERSION 3.7)

project("Flocking Wars")
set(EXECUTABLE_NAME FlockingWars)
set(PROJECT_SOURCE_DIR src)
set(EXT_SOURCE_DIR ext)

set(CMAKE_CXX_STANDARD 14) # C++14!
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11


file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE EXT_FILES ${EXT_SOURCE_DIR}/*.cpp)

add_executable(${EXECUTABLE_NAME} ${SRC_FILES} ${EXT_FILES})

include_directories( ./include/ ./ext/ )
# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED system window)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif()

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
if (OPENGL_FOUND)
  target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_LIBRARIES})
  # or: target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_gl_LIBRARY})
  target_link_libraries(${EXECUTABLE_NAME} m)  # if you use maths.h
endif()

# GLEW
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    target_link_libraries(${EXECUTABLE_NAME} ${GLEW_LIBRARIES})
endif()

# Freetype
find_package(Freetype REQUIRED)
if (FREETYPE_FOUND)
    include_directories(${FREETYPE_INCLUDE_DIRS})
    target_link_libraries(${EXECUTABLE_NAME} ${FREETYPE_LIBRARIES})
endif()

# Lua
find_package(Lua REQUIRED)
if (LUA_FOUND)
    include_directories(${LUA_INCLUDE_DIRS})
    target_link_libraries(${EXECUTABLE_NAME} ${LUA_LIBRARIES})
endif()
