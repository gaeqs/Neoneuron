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

#ifndef NEONEURON_ACTIVITYNEURONCOLORANDSCALESE_H
#define NEONEURON_ACTIVITYNEURONCOLORANDSCALESE_H

#include <neon/render/model/PinnedStorageBufferInstanceData.h>

#include <neoneuron/render/TimeAware.h>
#include <neoneuron/render/particle/ParticleNeuronRepresentation.h>
#include <neoneuron/render/extension/neuron/NeuronColorAndScaleSE.h>

namespace neoneuron
{
    constexpr size_t ACTIVITY_COLOR_AND_SCALE_GRADIENT_SIZE = 256;

    struct ActivityNeuronColorAndScaleSEData
    {
        rush::Vec4f gradient[ACTIVITY_COLOR_AND_SCALE_GRADIENT_SIZE];
        float sizes[ACTIVITY_COLOR_AND_SCALE_GRADIENT_SIZE];
        float decay;
    };

    struct ActivityNeuronColorAndScaleSEVolatileData
    {
        float simulationTime;
    };

    class ActivityNeuronColorAndScaleSE : public NeuronColorAndScaleSE, public TimeAware
    {
        static constexpr size_t ACTIVITY_INSTANCES = 10'000'000;
        static constexpr size_t GENERAL_DATA_BINDING = 0;
        static constexpr size_t VOLATILE_BINDING = 1;
        static constexpr size_t ACTIVITY_BINDING = 2;

        std::shared_ptr<neon::ShaderUniformDescriptor> _uboDescriptor;
        std::shared_ptr<neon::ShaderUniformBuffer> _ubo;
        std::shared_ptr<neon::PinnedStorageBufferInstanceData> _instanceData;

        std::optional<CurrentEventSequence> _activity;

        float _decay;

      protected:
        uint32_t newInstance() override;

        void deleteInstance(uint32_t id) override;

      public:
        static inline const std::string SHADER_PATH = "shader/extension/neuron/activity_neuron_color_and_scale.glsl";

        explicit ActivityNeuronColorAndScaleSE(neon::Application* application);

        ~ActivityNeuronColorAndScaleSE() override = default;

        [[nodiscard]] std::string generateShaderCode(size_t uniformSet) const override;

        [[nodiscard]] std::optional<std::shared_ptr<neon::ShaderUniformDescriptor>> getUBODescriptor() const override;

        [[nodiscard]] std::optional<std::shared_ptr<neon::ShaderUniformBuffer>> getUBO() const override;

        void onTimeChanged(std::chrono::nanoseconds lastTime, std::chrono::nanoseconds newTime,
                           TimeChangeType type) override;

        std::vector<ActivityEntry<mindset::TimeGrid<double>>> getTimeGrids() override;

        std::vector<ActivityEntry<mindset::EventSequence<std::monostate>>> getEventSequences() override;

        void setActivity(GID activityId, std::string name, mindset::EventSequence<std::monostate> sequence);

        void clearActivity();
    };
} // namespace neoneuron

#endif // NEONEURON_ACTIVITYNEURONCOLORANDSCALESE_H
