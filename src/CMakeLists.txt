file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/neoneuron/*.cpp")

cmrc_add_resource_library(
        neoneuron_resources
        ALIAS neoneuron::resources
        NAMESPACE resources
        WHENCE resources
        resources/font/SourceSans3.ttf
        resources/model/guide/plane_guide.json
        resources/shader/guide/plane_guide.vert
        resources/shader/guide/plane_guide.frag
        resources/shader/guide/plane_guide.json
        resources/model/guide/sphere_guide.json
        resources/model/guide/sphere_guide.obj
        resources/shader/guide/sphere_guide.vert
        resources/shader/guide/sphere_guide.frag
        resources/shader/guide/sphere_guide.json
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