cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_STANDARD 20)

project(Neoneuron)

if (UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK REQUIRED gtkmm-3.0)
    include_directories(${GTK_INCLUDE_DIRS})
    list(APPEND LIBRARIES ${GTK_LIBRARIES})
endif ()



include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/neon/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/nativefiledialog/src/include)

add_subdirectory(lib)

list(APPEND LIBRARIES neon nativefiledialog)

include_directories(src)
add_subdirectory(src)