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

#include "ActivityNeuronColorAndScaleSE.h"

CMRC_DECLARE(resources);

namespace neoneuron
{

    uint32_t ActivityNeuronColorAndScaleSE::newInstance()
    {
        static constexpr float INIT = -100000.0f;
        auto instance = _instanceData->createInstance().getResult();
        _instanceData->uploadData(instance, 0, &INIT);
        return *instance.id;
    }

    void ActivityNeuronColorAndScaleSE::deleteInstance(uint32_t id)
    {
        neon::InstanceData::Instance instance = {&id};
        _instanceData->freeInstance(instance);
    }

    ActivityNeuronColorAndScaleSE::ActivityNeuronColorAndScaleSE(neon::Application* application) :
        _decay(1.0f)
    {
        std::vector bindings = {
            neon::ShaderUniformBinding::uniformBuffer(sizeof(ActivityNeuronColorAndScaleSEData)),
            neon::ShaderUniformBinding::uniformBuffer(sizeof(ActivityNeuronColorAndScaleSEVolatileData)),
            neon::ShaderUniformBinding::storageBuffer(sizeof(float) * ACTIVITY_INSTANCES),
        };

        _uboDescriptor = std::make_shared<neon::ShaderUniformDescriptor>(
            application, "neoneuron:activity_neuron_color_and_scale_se_ubo_descriptor", bindings);
        _ubo = std::make_shared<neon::ShaderUniformBuffer>("neoneuron:activity_neuron_color_and_scale_se_ubo",
                                                           _uboDescriptor);

        ActivityNeuronColorAndScaleSEData data;
        rush::Vec4f red = {1.0f, 0.0f, 0.0f, 1.0f};
        rush::Vec4f blue = {0.0f, 0.0f, 1.0f, 1.0f};

        for (size_t i = 0; i < ACTIVITY_REPRESENTATION_GRADIENT_SIZE; i++) {
            float n = static_cast<float>(i) / (ACTIVITY_REPRESENTATION_GRADIENT_SIZE - 1);
            data.gradient[i] = rush::mix(red, blue, n);
            data.sizes[i] = rush::mix(4.0f, 1.0f, n);
        }

        data.decay = _decay;

        _ubo->uploadData(GENERAL_DATA_BINDING, data);
        _ubo->uploadData(VOLATILE_BINDING, ActivityNeuronColorAndScaleSEVolatileData{0.0f});

        std::vector slots = {
            neon::StorageBufferInstanceData::Slot(sizeof(float), sizeof(float), ACTIVITY_BINDING, _ubo.get()),
        };

        std::vector<std::type_index> types = {typeid(float)};

        _instanceData =
            std::make_shared<neon::StorageBufferInstanceData>(application, ACTIVITY_INSTANCES, types, slots);
    }

    std::string ActivityNeuronColorAndScaleSE::generateShaderCode(size_t uniformSet) const
    {
        neon::CMRCFileSystem fs(cmrc::resources::get_filesystem());
        auto file = fs.readFile(SHADER_PATH);
        if (!file) {
            return "";
        }

        std::string firstLine = std::format("const uint EXTENSION_SET = {};\n", uniformSet);
        return firstLine + file->toString();
    }

    std::optional<std::shared_ptr<neon::ShaderUniformDescriptor>> ActivityNeuronColorAndScaleSE::getUBODescriptor()
        const
    {
        return _uboDescriptor;
    }

    std::optional<std::shared_ptr<neon::ShaderUniformBuffer>> ActivityNeuronColorAndScaleSE::getUBO() const
    {
        return _ubo;
    }

    void ActivityNeuronColorAndScaleSE::onTimeChanged(std::chrono::nanoseconds lastTime,
                                                      std::chrono::nanoseconds newTime, TimeChangeType type)
    {
        if (!_activity) {
            return;
        }

        if (type == TimeChangeType::JUMP) {
            auto seconds = std::chrono::duration_cast<std::chrono::duration<float>>(newTime);
            float f = seconds.count() - _decay;
            for (auto id : _idToIndex | std::views::values) {
                _instanceData->uploadData(neon::InstanceData::Instance(&id), 0, &f);
            }

            auto decay = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<double>{_decay});
            onTimeChanged(newTime - decay, newTime, TimeChangeType::FLOW);
            return;
        }

        if (lastTime > newTime) {
            // Loop detected. Process both segments
            onTimeChanged(lastTime, std::chrono::nanoseconds::max(), type);
            onTimeChanged(std::chrono::nanoseconds(0), newTime, type);
            return;
        }

        for (auto event : _activity->sequence.getRange(lastTime, newTime)) {
            GID gid = {_activity->activityId.datasetId, event.uid};
            auto it = _idToIndex.find(gid);
            if (it == _idToIndex.end()) {
                continue;
            }

            auto seconds = std::chrono::duration_cast<std::chrono::duration<float>>(event.timepoint);
            float secondsFloat = seconds.count();
            neon::InstanceData::Instance instance = {&it->second};
            _instanceData->uploadData(instance, 0, &secondsFloat);
        }

        auto seconds = std::chrono::duration_cast<std::chrono::duration<float>>(newTime);
        _ubo->uploadData(VOLATILE_BINDING, ActivityNeuronColorAndScaleSEVolatileData(seconds.count()));
    }

    std::vector<ActivityEntry<mindset::TimeGrid<double>>> ActivityNeuronColorAndScaleSE::getTimeGrids()
    {
        return {};
    }

    std::vector<ActivityEntry<mindset::EventSequence<std::monostate>>> ActivityNeuronColorAndScaleSE::
        getEventSequences()
    {
        if (_activity) {
            return {ActivityEntry(_activity->activityId, _activity->name, &_activity->sequence)};
        }

        return {};
    }

    void ActivityNeuronColorAndScaleSE::setActivity(GID activityId, std::string name,
                                                    mindset::EventSequence<std::monostate> sequence)
    {
        _activity = {activityId, std::move(name), std::move(sequence)};
    }

    void ActivityNeuronColorAndScaleSE::clearActivity()
    {
        _activity = {};
    }
} // namespace neoneuron