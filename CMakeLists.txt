# Copyright (c) 2025. VG-Lab/URJC.
#
# Authors: Gael Rial Costas <gael.rial.costas@urjc.es>
#
# This file is part of Neoneuron <gitlab.gmrv.es/g.rial/neoneuron>
#
# This library is free software; you can redistribute it and/or modify it under
# the terms of the GNU Lesser General Public License version 3.0 as published
# by the Free Software Foundation.
#
# This library is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this library; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

cmake_minimum_required(VERSION 3.15)

option(NEONEURON_APPIMAGE "Configure Neoneuron for AppImage creation" ON)

set(NEONEURON_GENERATED_DIR ${CMAKE_BINARY_DIR}/generated)

set(CMAKE_BUILD_WITH_INSTALL_RPATH  TRUE)
set(CMAKE_INSTALL_RPATH            "$ORIGIN/../lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

set(CMAKE_CXX_STANDARD 20)
set(SPIRV_WERROR OFF)
set(IMBLUEPRINT_EXAMPLES OFF)
set(FETCHCONTENT_UPDATES_SUBMODULES OFF)
set(MINDSET_TESTS OFF)
set(BRION_BUILD_PYTHON_BINDINGS OFF)

include(FetchContent)

project(Neoneuron LANGUAGES CXX VERSION 0.1)

if (MSVC)
    add_compile_definitions(NOMINMAX)
endif ()

FetchContent_Declare(
        rush
        GIT_REPOSITORY https://github.com/gaeqs/Rush.git
        GIT_TAG mesh
)

FetchContent_MakeAvailable(rush)


FetchContent_Declare(
        neon
        GIT_REPOSITORY https://github.com/gaeqs/Neon.git
        GIT_TAG master
)

FetchContent_MakeAvailable(neon)

FetchContent_Declare(
        mindset
        GIT_REPOSITORY https://github.com/gaeqs/Mindset.git
        GIT_TAG master
)

FetchContent_MakeAvailable(mindset)

set(IMBLUEPRINT_IMGUI_TARGET imgui)

FetchContent_Declare(
        imblueprint
        GIT_REPOSITORY https://github.com/gaeqs/ImBlueprint.git
        GIT_TAG master
)

FetchContent_MakeAvailable(imblueprint)

FetchContent_Declare(
        cxxopts
        GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
        GIT_TAG master
)

FetchContent_MakeAvailable(cxxopts)

if (TARGET morphio_obj)
    target_precompile_headers(morphio_obj PUBLIC <cstdint>)
endif ()

find_package(Vulkan REQUIRED)
find_package(pugixml REQUIRED)

# Try extracting VulkanSDK path from ${Vulkan_INCLUDE_DIRS}
if (NOT ${Vulkan_INCLUDE_DIRS} STREQUAL "")
    set(VULKAN_PATH ${Vulkan_INCLUDE_DIRS})
    STRING(REGEX REPLACE "/Include" "" VULKAN_PATH ${VULKAN_PATH})
endif ()

if (WIN32)
    include_directories(AFTER ${VULKAN_PATH}/Include)
    link_directories(${VULKAN_PATH}/Bin;${VULKAN_PATH}/Lib;)
    set(VULKAN_LIB_LIST "vulkan-1")
else ()
    list(APPEND LIBRARIES assimp)
    set(VULKAN_LIB_LIST "vulkan")
endif ()

find_package(glfw3 CONFIG REQUIRED)
list(APPEND LIBRARIES glfw ${GLFW_LIBRARIES})
list(APPEND LIBRARIES ${VULKAN_LIB_LIST})
list(APPEND LIBRARIES neon hey pugixml mindset imblueprint HighFive cxxopts)

# Create version.h

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git")
    execute_process(
            COMMAND git rev-parse --short HEAD
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_COMMIT
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
    )
else ()
    set(GIT_COMMIT "unknown")
endif ()

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.h.in
        ${NEONEURON_GENERATED_DIR}/neoneuron/version.h
)

if (RELEASE_DEBUG)
    add_compile_definitions(RELEASE_DEBUG)
endif ()

include_directories(src)
add_subdirectory(src)

# Install
install(TARGETS neoneuron
        RUNTIME DESTINATION bin
)

install(FILES
        ${CMAKE_SOURCE_DIR}/data/neoneuron.desktop
        DESTINATION share/applications
)

install(FILES
        ${CMAKE_SOURCE_DIR}/data/neoneuron.png
        DESTINATION share/icons/hicolor/512x512/apps
)

install(FILES
        ${CMAKE_SOURCE_DIR}/data/neoneuron1024.png
        DESTINATION share/icons/hicolor/1024x1024/apps
)