file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/neoneuron/*.cpp")

cmrc_add_resource_library(
        neoneuron_resources
        ALIAS neoneuron::resources
        NAMESPACE resources
        WHENCE resources
        resources/font/SourceSans3.ttf
        resources/model/guide/plane_guide.json
        resources/model/guide/sphere_guide.json
        resources/model/guide/sphere_guide.obj
        resources/model/guide/point_guide.json
        resources/model/selector/selector_resolver.json

        resources/shader/guide/plane_guide.vert
        resources/shader/guide/plane_guide.frag
        resources/shader/guide/plane_guide.json
        resources/shader/guide/sphere_guide.vert
        resources/shader/guide/sphere_guide.frag
        resources/shader/guide/sphere_guide.json
        resources/shader/guide/point_guide.vert
        resources/shader/guide/point_guide.frag
        resources/shader/guide/point_guide.json
        resources/shader/neuron/simple/neuron.frag
        resources/shader/neuron/simple/neuron.mesh
        resources/shader/neuron/simple/neuron.task
        resources/shader/neuron/simple/neuron_beta.mesh
        resources/shader/neuron/complex/neuron.frag
        resources/shader/neuron/complex/neuron.mesh
        resources/shader/neuron/complex/neuron.task
        resources/shader/neuron/complex/neuron_beta.mesh
        resources/shader/neuron/complex/joint.frag
        resources/shader/neuron/complex/joint.mesh
        resources/shader/neuron/complex/joint.task
        resources/shader/neuron/complex/soma.frag
        resources/shader/neuron/complex/soma.mesh
        resources/shader/neuron/complex/soma.task
        resources/shader/selector/selector_resolver.frag
        resources/shader/selector/selector_resolver.vert
        resources/shader/selector/selector_resolver.json
        resources/swc/data.swc
        resources/swc/test.swc
        resources/render.json
)

add_executable(neoneuron ${SOURCES})
target_link_libraries(neoneuron ${LIBRARIES} ${CMAKE_DL_LIBS} neoneuron::resources)