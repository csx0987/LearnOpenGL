cmake_minimum_required(VERSION 3.7)

project(LearnOpenGL
        VERSION 1.0.0
        LANGUAGES C CXX
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")


# set(CMAKE_C_COMPILER "gcc")
# set(CMAKE_CXX_COMPILER "clang")
set(CMAKE_CXX_STANDARD 17)

set ( PRJ_LIBRARIES )
set ( PRJ_INCLUDE_DIRS )
set ( PRJ_SRC_LIST )

# find_package(glfw3 REQUIRED)
# find_package(OpenGL REQUIRED)

# if (OpenGL_FOUND)
#   message("OpenGL found")
# else()
#   message(FATAL_ERROR "Cannot find OpenGL")
# endif()

# if (glfw3_FOUND)
#   message("GLFW found")
# else()
#   message(FATAL_ERROR "Cannot find GLFW")
# endif()

# find_package(OpenGL REQUIRED)

IF (WIN32)
    include(C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake)
    set ( MY_GLFW "C:/dev/vcpkg/packages/glfw3_x64-windows" )
    set ( MY_GLFW_LIB "${MY_GLFW}/lib/glfw3dll.lib" )
    set ( MY_GLFW_INC "${MY_GLFW}/include" )
    set ( MY_ASSIMP "C:/dev/vcpkg/packages/assimp_x64-windows" )
    set ( MY_ASSIMP_INC "${MY_ASSIMP}/include" )
    # set ( MY_ASSIMP_LIB "${MY_ASSIMP}/lib/assimp-vc142-mt.lib" )
    find_package(assimp CONFIG REQUIRED)
    set ( MY_ASSIMP_LIB assimp::assimp )
ELSEIF (APPLE)
    include_directories("/usr/local/Cellar/glfw/3.3.6/include") 
    include_directories("/usr/local/Cellar/assimp/5.1.5/include")
    set ( MY_GLFW "/usr/local/Cellar/glfw/3.3.6")
    set ( MY_GLFW_INC "${MY_GLFW}/include" )
    set ( MY_GLFW_LIB "${MY_GLFW}/lib/libglfw.3.3.dylib" )
    set ( MY_ASSIMP "/usr/local/Cellar/assimp/5.1.5" )
    set ( MY_ASSIMP_INC "${MY_ASSIMP}/include" )
    set ( MY_ASSIMP_LIB "${MY_ASSIMP}/lib/libassimp.5.1.4.dylib" )
ENDIF()



include_directories("${CMAKE_CURRENT_SOURCE_DIR}/thridPart/include") 
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/thridPart/imgui") 
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/thridPart")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/core/header")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/testUnit")


file ( GLOB glad_header_files "${CMAKE_SOURCE_DIR}/thridPart/include/glad/*.h" )
file ( GLOB glad_src_files "${CMAKE_SOURCE_DIR}/thridPart/src/glad.c" )
file ( GLOB khr_header_files "${CMAKE_SOURCE_DIR}/thridPart/include/KHR/*.h" )
file ( GLOB imgui_header_files "${CMAKE_SOURCE_DIR}/thridPart/imgui/*.h" )
file ( GLOB imgui_src_files "${CMAKE_SOURCE_DIR}/thridPart/imgui/*.cpp" )
file ( GLOB core_header_files "${CMAKE_SOURCE_DIR}/core/header/*.h" )
file ( GLOB core_src_files "${CMAKE_SOURCE_DIR}/core/src/*.cpp" )
file ( GLOB glm_header_files "${CMAKE_SOURCE_DIR}/thridPart/glm/*.hpp" )
file ( GLOB test_header_files "${CMAKE_SOURCE_DIR}/testUnit/*.h" )
file ( GLOB test_src_files "${CMAKE_SOURCE_DIR}/testUnit/*.cpp" )

#src
list ( APPEND PRJ_SRC_LIST ${glad_src_files} )
list ( APPEND PRJ_SRC_LIST ${imgui_src_files} )
list ( APPEND PRJ_SRC_LIST ${core_src_files} )
list ( APPEND PRJ_SRC_LIST ${test_src_files} )

# header
list ( APPEND PRJ_INCLUDE_DIRS ${glad_header_files} )
list ( APPEND PRJ_INCLUDE_DIRS ${khr_header_files} )
# list ( APPEND PRJ_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/core/header/*.h" )
list ( APPEND PRJ_INCLUDE_DIRS ${MY_GLFW_INC} )
list ( APPEND PRJ_INCLUDE_DIRS ${MY_ASSIMP_INC} )
list ( APPEND PRJ_INCLUDE_DIRS ${imgui_header_files} )
list ( APPEND PRJ_INCLUDE_DIRS ${core_header_files} )
list ( APPEND PRJ_INCLUDE_DIRS ${glm_header_files} )
list ( APPEND PRJ_INCLUDE_DIRS ${test_header_files} )

# 动态链接库
list ( APPEND PRJ_LIBRARIES ${MY_GLFW_LIB} )
list ( APPEND PRJ_LIBRARIES ${MY_ASSIMP_LIB} )
# set(SOURCE 
#   core/src/Game.cpp
# )

# ${SOURCE}

add_executable(LearnOpenGL ${PRJ_SRC_LIST} Main.cpp)

target_include_directories(LearnOpenGL
  PRIVATE
    ${PRJ_INCLUDE_DIRS}
)

target_link_libraries(LearnOpenGL 
  PRIVATE 
    ${PRJ_LIBRARIES}
)