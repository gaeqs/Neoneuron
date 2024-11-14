//
// Created by gaeqs on 10/10/24.
//

#ifndef GPUNEURON_H
#define GPUNEURON_H


#include <neon/Neon.h>
#include <neoneuron/structure/Neuron.h>

namespace neoneuron {
    struct GPUNeuronSegment {
        uint32_t neuronId;

        uint32_t sectionId;

        /**
        * The type of the segment.
        */
        uint32_t type;

        /**
        * The position of the parent in the storage buffer.
        */
        uint32_t parent;

        /**
        * The end position of the segment (x, y, z) and the radius (w).
        * This merge is done to save a float position (vec3 has the same padding as vec4 in std430)
        */
        rush::Vec4f endAndRadius;
    };

    class GPUNeuron {
        std::weak_ptr<neon::Model> _model;
        size_t _instanceDataIndex;
        std::vector<neon::InstanceData::Instance> _instances;
        const Neuron* _neuron;
        bool _valid;

    public:
        GPUNeuron(GPUNeuron&& other) noexcept;

        GPUNeuron(const GPUNeuron& other) = delete;

        GPUNeuron(std::weak_ptr<neon::Model> model,
                  size_t instanceDataIndex,
                  const Neuron* neuron);

        ~GPUNeuron();

        void refreshGPUData() const;

        GPUNeuron& operator=(GPUNeuron&& other) noexcept;
    };
}

#endif //GPUNEURON_H
