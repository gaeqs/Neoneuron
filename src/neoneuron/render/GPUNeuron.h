//
// Created by gaeqs on 10/10/24.
//

#ifndef GPUNEURON_H
#define GPUNEURON_H


#include <neon/Neon.h>
#include <neoneuron/structure/Neuron.h>

namespace neoneuron {
    struct GPUNeuronSegment {
        /**
        * The end position of the segment (x, y, z) and the radius (w).
        * This merge is done to save a float position (vec3 has the same padding as vec4 in std430)
        */
        rush::Vec4f endAndRadius;

        /**
        * The type of the segment.
        */
        uint32_t type;

        /**
        * The position of the parent in the storage buffer.
        */
        uint32_t parent;

        /**
        * Reserved memory required by the padding.
        */
        uint32_t reserved[2]; // Padding must be 32 bits because of the vec4.
    };

    class GPUNeuron {
        neon::InstanceData* _instanceData;
        std::vector<neon::InstanceData::Instance> _instances;
        const Neuron* _neuron;

    public:
        GPUNeuron(GPUNeuron&& other) noexcept;

        GPUNeuron(const GPUNeuron& other) = delete;

        GPUNeuron(neon::InstanceData* instanceData, const Neuron* neuron);

        ~GPUNeuron();

        void refreshGPUData() const;

        GPUNeuron& operator=(GPUNeuron&& other) noexcept;
    };
}

#endif //GPUNEURON_H
