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

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/neoneuron/*.cpp")

cmrc_add_resource_library(
        neoneuron_resources
        ALIAS neoneuron::resources
        NAMESPACE resources
        WHENCE resources

        resources/imgui.ini
        resources/LICENSE.txt

        resources/font/SourceSans3.ttf
        resources/font/MaterialSymbols.ttf

        resources/icon/16x16/neoneuron.png
        resources/icon/32x32/neoneuron.png
        resources/icon/64x64/neoneuron.png
        resources/icon/128x128/neoneuron.png
        resources/icon/256x256/neoneuron.png
        resources/icon/512x512/neoneuron.png
        resources/icon/vglab.png
        resources/icon/urjc.png

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
        resources/shader/neuron/complex/neuron.frag
        resources/shader/neuron/complex/neuron.mesh
        resources/shader/neuron/complex/neuron.task
        resources/shader/neuron/complex/joint.frag
        resources/shader/neuron/complex/joint.mesh
        resources/shader/neuron/complex/joint.task
        resources/shader/neuron/complex/soma.frag
        resources/shader/neuron/complex/soma.mesh
        resources/shader/neuron/complex/soma.task
        resources/shader/selector/selector_resolver.frag
        resources/shader/selector/selector_resolver.vert
        resources/shader/selector/selector_resolver.json

        resources/shader/synapse/synapse.task
        resources/shader/synapse/synapse.mesh
        resources/shader/synapse/synapse.frag

        resources/shader/activity/activity.task
        resources/shader/activity/activity.mesh
        resources/shader/activity/activity.frag

        resources/texture/skybox/back.png
        resources/texture/skybox/bottom.png
        resources/texture/skybox/front.png
        resources/texture/skybox/left.png
        resources/texture/skybox/right.png
        resources/texture/skybox/top.png
        resources/texture/skybox/skybox.json

        resources/texture/white_skybox/white.png
        resources/texture/white_skybox/skybox.json

        resources/swc/data.swc
        resources/swc/test.swc
        resources/render.json
)

add_executable(neoneuron ${SOURCES})
target_link_libraries(neoneuron ${LIBRARIES} ${CMAKE_DL_LIBS} neoneuron::resources)

target_include_directories(neoneuron PUBLIC
        $<BUILD_INTERFACE:${NEONEURON_GENERATED_DIR}>
        $<INSTALL_INTERFACE:include>
)
