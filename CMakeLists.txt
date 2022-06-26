cmake_minimum_required(VERSION 3.10)

project(CWindow)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(SRC_GENERAL src/main.cpp)

set(compiler_options -g -O0)

if(BUILD_LINUX)
    # set(SRC_PLATFORM src/platform/x11/x11_error.h src/platform/x11/x11_window.h src/platform/x11/x11_window.cpp src/platform/x11/x11_input.h src/platform/x11/x11_input.cpp)
    set(SRC_PLATFORM src/platform/x11/x11_window.cpp src/platform/x11/x11_input.cpp)
elseif(BUILD_WINDOWS)
    set(SRC_PLATFORM src/platform/windows/win32_window.cpp)
endif()

find_package(X11 REQUIRED)

add_executable(CWindow ${SRC_GENERAL} ${SRC_PLATFORM})

target_link_libraries(CWindow PRIVATE X11)
target_compile_options(CWindow PRIVATE ${compiler_options})
target_include_directories(CWindow PUBLIC include)