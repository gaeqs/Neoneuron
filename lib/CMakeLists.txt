add_subdirectory(neon)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/lib/nativefiledialog/src/*.c")
add_library(nativefiledialog STATIC ${SOURCES})