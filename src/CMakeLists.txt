file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/neoneuron/*.cpp")

cmrc_add_resource_library(
        neoneuron_resources
        ALIAS neoneuron::resources
        NAMESPACE resources
        WHENCE resources
        resources/data.swc
        resources/neuron.task
        resources/neuron.mesh
        resources/neuron.frag
)

add_executable(neoneuron ${SOURCES})
target_link_libraries(neoneuron ${LIBRARIES} ${CMAKE_DL_LIBS} neoneuron::resources)