cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_STANDARD 20)
set(FETCHCONTENT_UPDATES_SUBMODULES OFF)
include(FetchContent)

project(Neoneuron)

if (UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK REQUIRED gtkmm-3.0)
    include_directories(${GTK_INCLUDE_DIRS})
    list(APPEND LIBRARIES ${GTK_LIBRARIES})
endif ()

if (MSVC)
    add_compile_definitions(NOMINMAX)
endif ()

FetchContent_Declare(
        neon
        GIT_REPOSITORY https://github.com/gaeqs/Neon.git
        GIT_TAG task-improvements
)

FetchContent_MakeAvailable(neon)

FetchContent_Declare(
        mindset
        GIT_REPOSITORY https://github.com/gaeqs/Mindset.git
        GIT_TAG master
)

FetchContent_MakeAvailable(mindset)

FetchContent_Declare(
        nativefiledialog
        GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended.git
        GIT_TAG a1a401062819beb8c3da84518ab1fe7de88632db
)

FetchContent_MakeAvailable(nativefiledialog)

set(IMBLUEPRINT_IMGUI_TARGET imgui)
FetchContent_Declare(
        imblueprint
        GIT_REPOSITORY https://github.com/gaeqs/ImBlueprint.git
        GIT_TAG master
)

FetchContent_MakeAvailable(imblueprint)

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
list(APPEND LIBRARIES neon nfd hey pugixml mindset imblueprint)

include_directories(src)
add_subdirectory(src)