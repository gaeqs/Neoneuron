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

#ifndef NEONEURON_STATICNEURONCOLORANDSCALESE_H
#define NEONEURON_STATICNEURONCOLORANDSCALESE_H

#include <neoneuron/render/extension/neuron/NeuronColorAndScaleSE.h>

namespace neoneuron
{

    struct StaticNeuronColorAndScaleSEData
    {
        rush::Vec4f color = rush::Vec4f(0.0f, 0.470588f, 0.980392f, 1.0f);
        rush::Vec4f selectedColor = rush::Vec4f(0.980392f, 0.470588f, 0.0f, 1.0f);
        float scale = 1.0f;
        float selectedScale = 1.0f;
    };

    class StaticNeuronColorAndScaleSE : public NeuronColorAndScaleSE
    {
        std::shared_ptr<neon::ShaderUniformDescriptor> _uboDescriptor;
        std::shared_ptr<neon::ShaderUniformBuffer> _ubo;

      protected:
        uint32_t newInstance() override;

        void deleteInstance(uint32_t id) override;

      public:

        static inline const std::string SHADER_PATH = "shader/extension/neuron/static_neuron_color_and_scale.glsl";

        explicit StaticNeuronColorAndScaleSE(neon::Application* application);

        ~StaticNeuronColorAndScaleSE() override = default;

        [[nodiscard]] std::string generateShaderCode(size_t uniformSet) const override;

        [[nodiscard]] std::optional<std::shared_ptr<neon::ShaderUniformDescriptor>> getUBODescriptor() const override;

        [[nodiscard]] std::optional<std::shared_ptr<neon::ShaderUniformBuffer>> getUBO() const override;
    };
} // namespace neoneuron

#endif // NEONEURON_STATICNEURONCOLORANDSCALESE_H
