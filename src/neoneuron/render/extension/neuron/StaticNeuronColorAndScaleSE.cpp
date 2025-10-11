// Copyright (c) 2025. VG-Lab/URJC.
//
// Authors: Gael Rial Costas <gael.rial.costas@urjc.es>
//
// This file is part of Neoneuron <gitlab.gmrv.es/g.rial/neoneuron>
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 3.0 as published
// by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by gaeqs on 10/10/25.
//

#include <cmrc/cmrc.hpp>
#include "StaticNeuronColorAndScaleSE.h"

#include <format>

CMRC_DECLARE(resources);

namespace neoneuron
{

    uint32_t StaticNeuronColorAndScaleSE::newInstance()
    {
        return 0;
    }

    void StaticNeuronColorAndScaleSE::deleteInstance(uint32_t id)
    {
    }

    StaticNeuronColorAndScaleSE::StaticNeuronColorAndScaleSE(neon::Application* application)
    {
        std::vector bindings = {
            neon::ShaderUniformBinding::uniformBuffer(sizeof(StaticNeuronColorAndScaleSEData)),
        };

        _uboDescriptor = std::make_shared<neon::ShaderUniformDescriptor>(
            application, "neoneuron:static_neuron_color_and_scale_se_ubo_descriptor", bindings);
        _ubo = std::make_shared<neon::ShaderUniformBuffer>("neoneuron:static_neuron_color_and_scale_se_ubo",
                                                           _uboDescriptor);
        _ubo->uploadData(0, StaticNeuronColorAndScaleSEData());
    }

    std::string StaticNeuronColorAndScaleSE::generateShaderCode(size_t uniformSet) const
    {
        neon::CMRCFileSystem fs(cmrc::resources::get_filesystem());
        auto file = fs.readFile(SHADER_PATH);
        if (!file) {
            return "";
        }

        std::string firstLine = std::format("const uint EXTENSION_SET = {};\n", uniformSet);
        return firstLine + file->toString();
    }

    std::optional<std::shared_ptr<neon::ShaderUniformDescriptor>> StaticNeuronColorAndScaleSE::getUBODescriptor() const
    {
        return _uboDescriptor;
    }

    std::optional<std::shared_ptr<neon::ShaderUniformBuffer>> StaticNeuronColorAndScaleSE::getUBO() const
    {
        return _ubo;
    }
} // namespace neoneuron