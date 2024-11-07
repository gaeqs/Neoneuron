file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/neoneuron/*.cpp")

cmrc_add_resource_library(
        neoneuron_resources
        ALIAS neoneuron::resources
        NAMESPACE resources
        WHENCE resources
        resources/model/guide/guide.json
        resources/shader/guide/guide.vert
        resources/shader/guide/guide.frag
        resources/shader/guide/guide.json
        resources/shader/neuron/neuron.frag
        resources/shader/neuron/neuron.mesh
        resources/shader/neuron/neuron.task
        resources/shader/neuron/neuron_beta.mesh
        resources/swc/data.swc
        resources/swc/test.swc
        resources/render.json
)

add_executable(neoneuron ${SOURCES})
target_link_libraries(neoneuron ${LIBRARIES} ${CMAKE_DL_LIBS} neoneuron::resources)