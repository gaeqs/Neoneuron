//
// Created by gaeqs on 31/03/25.
//

#ifndef GPUSYNAPSE_H
#define GPUSYNAPSE_H

#include <mindset/mindset.h>
#include <rush/vector/vec.h>
#include <neon/render/model/InstanceData.h>
#include <neon/render/model/Model.h>

#include <neoneuron/render/synapse/GPUSynapseCreateInfo.h>

namespace neoneuron
{

    struct GPUSynapseData
    {
        mindset::UID uid;
        mindset::UID preNeuronId;
        mindset::UID postNeuronId;
        float property;
        rush::Vec4f prePosition;  // x, y, z, w = valid
        rush::Vec4f postPosition; // x, y, z, w = valid
    };

    class GPUSynapse
    {
        neon::InstanceData* _instanceData;
        neon::InstanceData::Instance _instance;
        bool _valid;

        std::optional<rush::Vec3f> _prePosition;
        std::optional<rush::Vec3f> _postPosition;

      public:
        GPUSynapse(GPUSynapse&& other) noexcept;

        GPUSynapse(const GPUSynapse& other) = delete;

        GPUSynapse(const GPUSynapseCreateInfo& info, const mindset::Synapse* synapse);

        ~GPUSynapse();

        [[nodiscard]] std::optional<rush::Vec3f> getPrePosition() const;

        [[nodiscard]] std::optional<rush::Vec3f> getPostPosition() const;

        void refreshGPUData(const GPUSynapseCreateInfo& info, const mindset::Synapse* synapse);
    };

} // namespace neoneuron

#endif //GPUSYNAPSE_H
